#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SIGNATURE_SIZE 16

int main(int argc, char **argv)
{
  int fd;
  char * cmos;
  char * sig_start;
  char sig[SIGNATURE_SIZE + 1];
  int result = 1;

  fd = open("/dev/mem", "r");
  if (fd < 0) 
    {
      fprintf(stderr, "Can't open dev mem: %d\n", errno);
      return 1;
    }

  cmos = mmap(0, 0x100000, PROT_READ, MAP_SHARED, fd, 0xfff00000);
  sig_start = cmos + 0x000fffc0;

  memset(sig, 0x0, sizeof(sig));
  memcpy(sig, sig_start, SIGNATURE_SIZE);

  close(fd);

  printf ("%s\n", sig);

  if (strlen (sig) != 16)
    goto out; 
  
  if (strncmp (sig, "CL1   Q2", 8) != 0)
    goto out;

  if (!isdigit (*(sig + 9)))
    goto out;

  if (!isdigit (*(sig + 10)))
    goto out;

  /* TODO: for completeness we should store the results of the first
   *       check (Q2A or Q2B) and check against that
   */
  if (strncmp (sig + 11, "  Q2", 4) != 0) 
    goto out;

  result = 0;
  
 out:   
  return result;
}
