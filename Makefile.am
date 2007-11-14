#
# Copyright 2007 Bernardo Innocenti <bernie@codewiz.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

x11dir = ${sysconfdir}/X11
initdir = ${sysconfdir}/rc.d/init.d

dist_x11_DATA = xorg-dcon.conf xorg-emu.conf
dist_init_SCRIPTS = olpc-configure

dist_bin_SCRIPTS = olpc-session olpc-logbat olpc-netlog olpc-netstatus

sbin_PROGRAMS = olpc-bios-sig setolpckeys olpc-dm

AM_CFLAGS = -Wall -W

olpc_bios_sig_SOURCES = olpc-bios-sig.c

setolpckeys_SOURCES = setolpckeys.c

olpc_dm_SOURCES = olpc-dm.c
olpc_dm_LDFLAGS = -lpam_misc -lpam

EXTRA_DIST = autogen.sh


bumprev:
	oldver=`echo "@VERSION@" | sed -n -e 's/\([0-9]\+\)\.[0-9]\+/\1/p'`; \
	oldrev=`echo "@VERSION@" | sed -n -e 's/[0-9]\+\.\([0-9]\+\)/\1/p'`; \
	newver="$$oldver"; \
	newrev="$$(($$oldrev + 1))"; \
	echo "Bumping revision from $$oldver.$$oldrev to $$newver.$$newrev"; \
	sed -i -e "/^AC_INIT/ s/$$oldver\.$$oldrev/$$newver.$$newrev/" "$(top_srcdir)/configure.ac"; \
	#

