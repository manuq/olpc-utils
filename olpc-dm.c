#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <utmp.h>
#include <termios.h>
#include <setjmp.h>
#include <ctype.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/syslog.h>
#include <sys/sysmacros.h>
#include <sys/param.h>

#include <linux/major.h>

#define OLPC_USER        "olpc"

#define TTY_MODE         0620
#define	TTYGRPNAME       "tty"
#define _PATH_HUSHLOGIN  ".hushlogin"

#  include <security/pam_appl.h>
#  include <security/pam_misc.h>
#  define PAM_MAX_LOGIN_TRIES	3
#  define PAM_FAIL_CHECK if (retcode != PAM_SUCCESS) { \
       fprintf(stderr,"\n%s\n",pam_strerror(pamh, retcode)); \
       syslog(LOG_ERR,"%s",pam_strerror(pamh, retcode)); \
       pam_end(pamh, retcode); exit(1); \
   }
#  define PAM_END { \
	pam_setcred(pamh, PAM_DELETE_CRED); \
	retcode = pam_close_session(pamh,0); \
	pam_end(pamh,retcode); \
}

int     timeout = 60;

struct passwd *pwd;

static struct passwd pwdcopy;
char    hostaddress[16];	/* used in checktty.c */
char	*hostname;		/* idem */
static char	*username, *tty_name, *tty_number;
static char	thishost[100];
static pid_t	pid;

static inline void xstrncpy(char *dest, const char *src, size_t n) {
        strncpy(dest, src, n-1);
        dest[n-1] = 0;
}

static int childPid = 0;
static volatile int got_sig = 0;



static void
parent_sig_handler(int signal)
{
  if(childPid)
    kill(-childPid, signal);
  else
    got_sig = 1;
  if(signal == SIGTERM)
    kill(-childPid, SIGHUP); /* because the shell often ignores SIGTERM */
}


/* Use PAM to login as user */
void
olpc_login(void)
{
  extern int optind;
  extern char *optarg, **environ;
  int fflag, hflag, pflag, cnt;
  int quietlog;
  char *domain;
  char tbuf[MAXPATHLEN + 2];
  int retcode;
  pam_handle_t *pamh = NULL;
  struct pam_conv conv = { misc_conv, NULL };
  struct sigaction sa, oldsa_hup, oldsa_term;
  pid = getpid();

  signal(SIGQUIT, SIG_IGN);
  signal(SIGINT, SIG_IGN);

  setpriority(PRIO_PROCESS, 0, 0);
  
  gethostname(tbuf, sizeof(tbuf));
  xstrncpy(thishost, tbuf, sizeof(thishost));
  domain = index(tbuf, '.');
  
  username = tty_name = hostname = NULL;
  fflag = hflag = pflag = 0;

  for (cnt = getdtablesize(); cnt > 2; cnt--)
    close(cnt);
 
  /* TODO: This is not right, we should open the display :0 but we need
   *     to start X first.  Flow should go like this once we get rid of startx
   *     seteuid 0, setuid olpc -> start X -> start pam session -> fork ->
   *     seteuid olpc -> start clients
   */ 
  tty_name = "tty2";
  tty_number = "2";

  /* set pgid to pid */
  setpgrp();
  /* this means that setsid() will fail */
  
  openlog("olpc-login", LOG_ODELAY, LOG_AUTHPRIV);

  retcode = pam_start("olpc-login", OLPC_USER, &conv, &pamh);
  if(retcode != PAM_SUCCESS) 
    {
      fprintf(stderr, "olpc-login: PAM Failure, aborting: %s\n",
              pam_strerror(pamh, retcode));
      syslog(LOG_ERR, "Couldn't initialize PAM: %s",
             pam_strerror(pamh, retcode));
      exit(99);
    }
  retcode = pam_set_item(pamh, PAM_TTY, tty_name);
  PAM_FAIL_CHECK;

  /*
   * Authentication may be skipped (for example, during krlogin, rlogin, etc...), 
   * but it doesn't mean that we can skip other account checks. The account 
   * could be disabled or password expired (althought kerberos ticket is valid).
   * -- kzak@redhat.com (22-Feb-2006)
   */
  retcode = pam_acct_mgmt(pamh, 0);

  if(retcode == PAM_NEW_AUTHTOK_REQD) 
    retcode = pam_chauthtok(pamh, PAM_CHANGE_EXPIRED_AUTHTOK);

  PAM_FAIL_CHECK;

  /*
   * Grab the user information out of the password file for future usage
   * First get the username that we are actually using, though.
   */
  retcode = pam_get_item(pamh, PAM_USER, (const void **) &username);
  PAM_FAIL_CHECK;

  if (!username || !*username) 
    {
      fprintf(stderr, "\nSession setup problem, abort.\n");
      syslog(LOG_ERR, "NULL user name in %s:%d. Abort.",
             __FUNCTION__, __LINE__);
      pam_end(pamh, PAM_SYSTEM_ERR);
      exit(1);
    }

  if (!(pwd = getpwnam(username))) 
    {
      fprintf(stderr, "\nSession setup problem, abort.\n");
      syslog(LOG_ERR, "Invalid user name \"%s\" in %s:%d. Abort.",
             username, __FUNCTION__, __LINE__);
      pam_end(pamh, PAM_SYSTEM_ERR);
      exit(1);
    }

  /*
   * Create a copy of the pwd struct - otherwise it may get
   * clobbered by PAM
   */
  memcpy(&pwdcopy, pwd, sizeof(*pwd));
  pwd = &pwdcopy;
  pwd->pw_name   = strdup(pwd->pw_name);
  pwd->pw_passwd = strdup(pwd->pw_passwd);
  pwd->pw_gecos  = strdup(pwd->pw_gecos);
  pwd->pw_dir  = strdup(pwd->pw_dir);
  pwd->pw_shell  = strdup(pwd->pw_shell);
  if (!pwd->pw_name || !pwd->pw_passwd || !pwd->pw_gecos ||
	!pwd->pw_dir || !pwd->pw_shell) 
    {
      fprintf(stderr, "olpc-login: Out of memory\n");
      syslog(LOG_ERR, "Out of memory");
      pam_end(pamh, PAM_SYSTEM_ERR);
      exit(1);
    }
  username = pwd->pw_name;

  /*
   * Initialize the supplementary group list.
   * This should be done before pam_setcred because
   * the PAM modules might add groups during pam_setcred.
   */
  if (initgroups(username, pwd->pw_gid) < 0) 
    {
      syslog(LOG_ERR, "initgroups: %m");
      fprintf(stderr, "\nSession setup problem, abort.\n");
      pam_end(pamh, PAM_SYSTEM_ERR);
      exit(1);
    }

  retcode = pam_open_session(pamh, 0);
  PAM_FAIL_CHECK;

  retcode = pam_setcred(pamh, PAM_ESTABLISH_CRED);
  if (retcode != PAM_SUCCESS)
      pam_close_session(pamh, 0);
  PAM_FAIL_CHECK;

  /* committed to login -- turn off timeout */
  alarm((unsigned int)0);
  
  endpwent();
  
  /* This requires some explanation: As root we may not be able to
     read the directory of the user if it is on an NFS mounted
     filesystem. We temporarily set our effective uid to the user-uid
     making sure that we keep root privs. in the real uid. 
     
     A portable solution would require a fork(), but we rely on Linux
     having the BSD setreuid() */
  
  {
    char tmpstr[MAXPATHLEN];
    uid_t ruid = getuid();
    gid_t egid = getegid();

    /* avoid snprintf - old systems do not have it, or worse,
       have a libc in which snprintf is the same as sprintf */
    if (strlen(pwd->pw_dir) + sizeof(_PATH_HUSHLOGIN) + 2 > MAXPATHLEN)
      quietlog = 0;
    else 
      {
        sprintf(tmpstr, "%s/%s", pwd->pw_dir, _PATH_HUSHLOGIN);
                setregid(-1, pwd->pw_gid);
                setreuid(0, pwd->pw_uid);
                quietlog = (access(tmpstr, R_OK) == 0);
                setuid(0); /* setreuid doesn't do it alone! */
                setreuid(ruid, 0);
                setregid(-1, egid);
      }
  }
  
  /* for linux, write entries in utmp and wtmp */
  {
    struct utmp ut;
    struct utmp *utp;
    struct timeval tv;
	
    utmpname(_PATH_UTMP);
    setutent();

    /* Find pid in utmp.
       login sometimes overwrites the runlevel entry in /var/run/utmp,
       confusing sysvinit. I added a test for the entry type, and the problem
       was gone. (In a runlevel entry, st_pid is not really a pid but some number
       calculated from the previous and current runlevel).
       Michael Riepe <michael@stud.uni-hannover.de>
     */
    while ((utp = getutent()))
      if (utp->ut_pid == pid
          && utp->ut_type >= INIT_PROCESS
          && utp->ut_type <= DEAD_PROCESS)
        break;

	/* If we can't find a pre-existing entry by pid, try by line.
	   BSD network daemons may rely on this. (anonymous) */
    if (utp == NULL)
      {
        setutent();
        ut.ut_type = LOGIN_PROCESS;
        strncpy(ut.ut_line, tty_name, sizeof(ut.ut_line));
        utp = getutline(&ut);
      }
	
    if (utp) 
      {
        memcpy(&ut, utp, sizeof(ut)); 
      } 
    else 
      {
        /* some gettys/telnetds don't initialize utmp... */
        memset(&ut, 0, sizeof(ut));
      }
	
    if (ut.ut_id[0] == 0)
      strncpy(ut.ut_id, tty_number, sizeof(ut.ut_id));
	
    strncpy(ut.ut_user, username, sizeof(ut.ut_user));
    xstrncpy(ut.ut_line, tty_name, sizeof(ut.ut_line));
    gettimeofday(&tv, NULL);
    ut.ut_tv.tv_sec = tv.tv_sec;
    ut.ut_tv.tv_usec = tv.tv_usec;
    ut.ut_type = USER_PROCESS;
    ut.ut_pid = pid;
    if (hostname) 
      {
        xstrncpy(ut.ut_host, hostname, sizeof(ut.ut_host));
        if (hostaddress[0])
          memcpy(&ut.ut_addr_v6, hostaddress, sizeof(ut.ut_addr_v6));
      }
	
    pututline(&ut);
    endutent();

    updwtmp(_PATH_WTMP, &ut);
  }

  setgid(pwd->pw_gid);
  
  environ = (char**)malloc(sizeof(char*));
  memset(environ, 0, sizeof(char*));

  setenv("HOME", pwd->pw_dir, 0);    /* legal to override */
  setenv("PATH", _PATH_DEFPATH, 1);
  setenv("SHELL", pwd->pw_shell, 1);

  /* LOGNAME is not documented in login(1) but
     HP-UX 6.5 does it. We'll not allow modifying it.
     */
  setenv("LOGNAME", pwd->pw_name, 1);

  {
    int i;
    char ** env = pam_getenvlist(pamh);

    if (env != NULL) 
      {
        for (i=0; env[i]; i++)
          putenv(env[i]);
      }
  }

  /* allow tracking of good logins.
     -steve philp (sphilp@mail.alliance.net) */
  
  if (hostname) 
    syslog(LOG_INFO, "LOGIN ON %s BY %s FROM %s", tty_name, 
           pwd->pw_name, hostname);
  else 
    syslog(LOG_INFO, "LOGIN ON %s BY %s", tty_name,
           pwd->pw_name); 
  
  signal(SIGALRM, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
  signal(SIGTSTP, SIG_IGN);

  /*
   * We must fork before setuid() because we need to call
   * pam_close_session() as root.
   */
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = SIG_IGN;
  sigaction(SIGINT, &sa, NULL);

  sigaction(SIGHUP, &sa, &oldsa_hup); /* ignore while we detach from the tty */
  ioctl(0, TIOCNOTTY, NULL);

  sa.sa_handler = parent_sig_handler;
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGTERM, &sa, &oldsa_term);

  /*
   * FIXME: something bad happens here that causes olpc-dm to remain attached
   * to the tty from which it was spawned, react to things like ^C and kill
   * innocent sessions that happen to be running on that tty -- bernie
   */
  closelog();
  childPid = fork();
  if (childPid < 0) 
    {
      int errsv = errno;
      /* error in fork() */
      fprintf(stderr, "olpc-login: failure forking: %s", strerror(errsv));
      PAM_END;
      exit(1);
    }

  if (childPid) 
    {
      close(0); close(1); close(2); 
      sa.sa_handler = SIG_IGN;
      sigaction(SIGQUIT, &sa, NULL);
      sigaction(SIGINT, &sa, NULL);
      while(wait(NULL) == -1 && errno == EINTR) /**/ ;
      openlog("olpc-login", LOG_ODELAY, LOG_AUTHPRIV);
	  /*
	   * FIXME: something bad happens here that causes innocent
	   * sessions on same tty to die -- bernie
	   */
      PAM_END;
      exit(0);
    }

  sigaction(SIGHUP, &oldsa_hup, NULL);
  sigaction(SIGTERM, &oldsa_term, NULL);
  if(got_sig) exit(1);

  /* child */
  /*
   * Problem: if the user's shell is a shell like ash that doesnt do
   * setsid() or setpgrp(), then a ctrl-\, sending SIGQUIT to every
   * process in the pgrp, will kill us.
   */

  /* start new session */
  setsid();

  /* make sure we have a controlling tty */
  openlog("olpc-login", LOG_ODELAY, LOG_AUTHPRIV);	/* reopen */

  /*
   * TIOCSCTTY: steal tty from other process group.
   */
  if (ioctl(0, TIOCSCTTY, (char *)1)) 
    {
      syslog(LOG_ERR, "Couldn't set controlling terminal: %s", strerror(errno));
      exit(1);
    }

  signal(SIGINT, SIG_DFL);
  
  /* discard permissions last so can't get killed and drop core */
  if(setuid(pwd->pw_uid) < 0 && pwd->pw_uid) 
    {
      syslog(LOG_ALERT, "setuid() failed");
      exit(1);
    }
  
  /* wait until here to change directory! */
  if (chdir(pwd->pw_dir) < 0) 
    {
      printf("No directory %s!\n", pwd->pw_dir);
      if (chdir("/"))
        exit(0);
      pwd->pw_dir = "/";
      printf("Logging in with home = \"/\".\n");
    }
  
  /* exec startx. wait on child to cleanup */
  execl("/usr/bin/startx", "startx", "/usr/bin/olpc-session", "--", "-fp", "built-ins", "-wr", NULL);
  exit(0);
}

int
main (int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  olpc_login();
  return 0;
}
