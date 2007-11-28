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

PKGNAME  = olpc-utils
PKGVER   = @VERSION@
PKGREL   = 1.$(RPMDIST)
NVR      = $(PKGNAME)-$(PKGVER)-$(PKGREL)
TARBALL  = $(PKGNAME)-$(PKGVER).tar.bz2
SRPM     = $(NVR).src.rpm
RPMDIST  = olpc2
DISTVAR  = olpc
SPECFILE = $(PKGNAME).spec
RPMBUILD = rpmbuild \
	--define "_sourcedir $(srcdir)" \
	--define "_builddir $(builddir)" \
	--define "_srcrpmdir $(builddir)" \
	--define "_rpmdir $(builddir)" \
	--define "dist .$(RPMDIST)" \
	--define "$(DISTVAR) 1" \
	#

CVS_TAG     = $(shell echo $(NVR) | sed s/\\\./_/g)
CVS_BRANCH  = OLPC-2
CVS_REPO    = rpms/$(PKGNAME)/$(CVS_BRANCH)
CVS_URL     = cvs://cvs.fedoraproject.org/cvs/pkgs?$(CVS_REPO)\#$(CVS_TAG)

dist_x11_DATA = \
	xorg-dcon.conf \
	xorg-emu.conf \
	xorg-dcon-1.3.conf \
	xorg-emu-1.3.conf \
	#

dist_init_SCRIPTS = olpc-configure

dist_bin_SCRIPTS = olpc-session olpc-logbat olpc-netlog olpc-netstatus

sbin_PROGRAMS = olpc-bios-sig setolpckeys olpc-dm

AM_CFLAGS = -Wall -W

olpc_bios_sig_SOURCES = olpc-bios-sig.c

setolpckeys_SOURCES = setolpckeys.c

olpc_dm_SOURCES = olpc-dm.c
olpc_dm_LDFLAGS = -lpam_misc -lpam

EXTRA_DIST = autogen.sh $(SPECFILE) cvs-import.sh

#
# Here comes the crazy package build machinery automation -- bernie
#

.PHONY: bumprev
bumprev:
	ver=`echo "@VERSION@" | sed -n -e 's/\([0-9]\+\)\.[0-9]\+/\1/p'`; \
	rev=`echo "@VERSION@" | sed -n -e 's/[0-9]\+\.\([0-9]\+\)/\1/p'`; \
	newver="$$ver"; \
	newrev="$$(($$rev + 1))"; \
	echo "Bumping revision from $$ver.$$rev to $$newver.$$newrev"; \
	sed -i -e "/^AC_INIT/ s/$$ver\.$$rev/$$newver.$$newrev/" "$(top_srcdir)/configure.ac"; \
	git-commit -e -a -m "Bump revision" || exit 1; \
	git-tag v$$newver.$$newrev || exit 1; \
	#

.PHONY: changelog
changelog:
	ver=`echo "@VERSION@" | sed -n -e 's/\([0-9]\+\)\.[0-9]\+/\1/p'`; \
	rev=`echo "@VERSION@" | sed -n -e 's/[0-9]\+\.\([0-9]\+\)/\1/p'`; \
	oldver="$$ver"; \
	oldrev="$$(($$rev - 1))"; \
	git-log v$$oldver.$$oldrev..v$$ver.$$rev \
		| git-shortlog \
		| sed -e "s/^\(.*\) ([0-9]\+):\$$/* `date '+%a %b %d %Y'` \1 <bernie@codewiz.org> - $$ver.$$rev-1/" \
		| sed -e 's/^[\t ]\{2,\}/- /' \
       		> changelog.tmp \
	#

.PHONY: push
push:
	git-push
	git-push --tags


#TODO: generate specfile
#$(SPECFILE): $(SPECFILE).in

.PHONY: srpm
srpm $(SRPM): distcheck $(SPECFILE) $(TARBALL)
	$(RPMBUILD) -bs $(SPECFILE)

.PHONY: rpm
rpm: distcheck $(SPECFILE) $(TARBALL)
	$(RPMBUILD) -bb $(SPECFILE)

.PHONY: import
import: $(SRPM)
	./cvs-import.sh -b $(CVS_BRANCH) -m "Import $(PKGNAME)-$(PKGVER)" $(SRPM)

.PHONY: build
build:
	koji build dist-$(RPMDIST) $(CVS_URL)

.PHONY: scratchbuild
scratchbuild:
	koji build --scratch dist_$(RPMDIST) $(SRPM)

.PHONY: release
release: distcheck changelog push import build
