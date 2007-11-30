#
# Use ./autogen.sh to generate the configure
#

AC_PREREQ(2.61)
AC_INIT([olpc-utils],[0.50],[devel@lists.laptop.org])
AM_INIT_AUTOMAKE([foreign dist-bzip2 no-dist-gzip -Wall])
AC_CONFIG_SRCDIR([olpc-bios-sig.c])

AC_MSG_NOTICE([Configuring olpc-utils])

# Checks for programs.
AC_PROG_CC

AC_CHECK_HEADERS([security/pam_appl.h security/pam_misc.h],
	[], AC_MSG_ERROR([Missing PAM headers])
)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
