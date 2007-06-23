/*
    olpc-bios-sig - reads the XO's signature in the bios 
    Copyright (C) 2007  OLPC

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

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
