Name:		olpc-utils
Version:	0.46
Release:	1%{?dist}
Summary:	OLPC utilities
URL:		http://dev.laptop.org/git?p=projects/olpc-utils;a=summary
Group:		System Environment/Base
License:	GPL
# The source for this package was pulled from upstream's vcs.  Use the
# following commands to generate the tarball:
#  git clone git://dev.laptop.org/projects/olpc-utils;
#  cd olpc-utils
#  git checkout v%{version}
#  ./autoconf.sh
#  make dist
Source0:	olpc-utils-%{version}.tar.bz2
Source100:	dot-xsession-example
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  pam-devel
Requires:       pam
  
%description

Tools for starting an X session, mapping keys on the OLPC keyboards
and checking BIOS signature.

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

install -D -m 0644 %{SOURCE100} $RPM_BUILD_ROOT/etc/skel/.xsession-example


%post
/sbin/chkconfig --add olpc-configure


%preun
if [ $1 = 0 ]; then
  /sbin/chkconfig --del olpc-configure
fi


%clean
rm -rf %{buildroot}


%files

%doc COPYING

%defattr(-,root,root,-)

%{_sbindir}/setolpckeys
%{_sbindir}/olpc-bios-sig
%{_sbindir}/olpc-dm
%{_bindir}/olpc-logbat
%{_bindir}/olpc-netlog
%{_bindir}/olpc-netstatus
%{_bindir}/olpc-session
%{_sysconfdir}/X11/xorg-dcon.conf
%{_sysconfdir}/X11/xorg-emu.conf
%{_sysconfdir}/rc.d/init.d/olpc-configure
%{_sysconfdir}/skel/.xsession-example


%changelog
* Wed Nov 21 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.46-1
- Automate the release process a bit more.
- Approximate XOs DPI on emulators.
- ReTAB.
- Automate specfile generation some more
- Ignore a few more generated files.
- Set i18n settings from the new manufacturing data tags
- Go back to starting sugar with /usr/bin/sugar.
- Bump revision

* Sat Nov 17 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.45-1
- Add bumprev rule
- Merge branch 'master' of ssh://bernie@dev.laptop.org/git/projects/olpc-utils
- Add rule to generate RPM changelog.
- Add support for X 1.3
- Bump revision

* Wed Nov 14 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.44-1
- Don't specify the (olpc) XKB variant esplicitly when not needed
- Fix http://dev.laptop.org/ticket/470
- Bump revision

* Mon Nov 13 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.43-2
- Typo: /etc/skel/.xession-example -> /etc/skel/.xsession-example
- Add "ulimit -c unlimited" example in .xsession-example

* Mon Nov 06 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.43-1
- Reverse check for A-test (bad monkey no bananas)
- Restore i18n, integrate /usr/bin/sugar.
- Disable .tar.gz and bump revision
- Fix check for sugar debug for new scheme.

* Mon Nov 06 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.41-3
- Be more specific in instructions on how to generate tarball
- Install .xession-example in /etc/skel
- REALLY drop obsolete olpc-register

* Mon Nov 05 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.41-2
- Drop obsolete olpc-register

* Mon Nov 05 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.41-1
- Source custom user session last, so they can override everything we did

* Mon Nov 05 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.40-1
- Bump revision to 0.40
- Rename user session to .xsession so it sounds familiar
- Make .i18n owned by user olpc and simplify script
- Fix X config file on qemu (untested)
- Improve override mechanism and make XKB_VARIANT optional
- Simplify by using extended XKB syntax instead of a separate XKB_VARIANT
- Remove now useless nvram code for DCON detection
- Add support for /etc/sysconfig/keyboard

* Mon Nov 03 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.33-1
- Bump revision
- Rename custom user session to ~/.olpcinit to avoid sourcing stale .xinitrc file on updates.
- Delete stray xorg.conf left by old versions of pilgrim.
- Make olpc-configure executable

* Mon Nov 02 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.32-1
- Bump revision to 0.32
- Juggle keyboard and language configuration stuff between olpc-configure
  and olpc-session

* Mon Nov 01 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.31-1
- Bump revision to 0.31
- Make olpc-configure pkgconfig compliant

* Mon Oct 31 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.30-1
- Bump revision to 0.30
- Whitespace cleanup
- Fix compiler warnings
- Add xorg.conf files
- Add olpc-netlog and olpc-netstatus
- Add olpc-session (replaces .xinitrc)
- Add olpc-logbat from Richard Smith
- Add olpc-configure

* Mon Oct 15 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.20-1
- Add olpc-dm
- Switch to automake
- Drop olpc-evdev
- Temporarily disable olpc-register because nobody seems to know what it was for

* Tue Jul 31 2007 Dan Williams <dcbw@redhat.com> - 0.15-1.1
- Add registration utility

* Sat Jun 23 2007 Rahul Sundaram <sundaram@redhat.com> 0.15
- Upstream pull, more spec file cleanup
* Thu Jun 21 2007 Rahul Sundaram <sundaram@redhat.com> 0.11-2
- Spec file cleanup as per review
* Wed Jun 20 2007 Rahul Sundaram <sundaram@redhat.com 0.11-1
- Newer source from J5 which fixes a permission issue. Fix build root cleanup.
* Wed Jun 20 2007 Rahul Sundaram <sundaram@redhat.com 0.10-1
- Newer source and spec cleanups from J5 
* Wed Jun 20 2007 Rahul Sundaram <sundaram@redhat.com 0.1-3
- Split off dbench. Added a description for bios signature tool. 
* Wed Jun 20 2007 Rahul Sundaram <sundaram@redhat.com> - 0.1-2
- Submit for review in Fedora
* Fri Nov 10 2006 John (J5) Palmieri <johnp@redhat.com> - 0.1-1
- initial package
