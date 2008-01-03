Name:		olpc-utils
Version:	0.61
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

# for olpc-dm
BuildRequires:  pam-devel
Requires:       pam

# for olpc-netcapture
Requires:	tcpdump

# for olpc-netlog
Requires:	ethtool
Requires:	tar
Requires:	gzip

# for olpc-configure
Requires:	/usr/bin/find

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
%{_sbindir}/olpc-dm
%{_bindir}/olpc-logbat
%{_bindir}/olpc-netlog
%{_bindir}/olpc-netcapture
%{_bindir}/olpc-netstatus
%{_bindir}/olpc-session
%{_bindir}/olpc-pwr-prof
%{_bindir}/olpc-pwr-prof-send
%{_sysconfdir}/profile.d/zzz_olpc.sh
%{_sysconfdir}/cron.d/olpc-pwr-prof.cron
%{_sysconfdir}/motd.olpc
%{_sysconfdir}/X11/xorg-dcon.conf
%{_sysconfdir}/X11/xorg-emu.conf
%{_sysconfdir}/X11/xorg-vmware.conf
%{_sysconfdir}/X11/xorg-dcon-1.3.conf
%{_sysconfdir}/X11/xorg-emu-1.3.conf
%{_sysconfdir}/rc.d/init.d/olpc-configure
%{_sysconfdir}/skel/.xsession-example


%changelog
* Thu Jan 03 2008 Michael Stone <bernie@codewiz.org> - 0.61-1
- Construct Rainbow's spool dir if it doesn't exist - #5033
- Ensure /security has reasonable permissions.

* Wed Jan 02 2008 Bernardo Innocenti <bernie@codewiz.org> - 0.60-1
- Depend on /usr/bin/find
- Remove files in $OLPC_HOME before creating them.
- Add missing dependencies.
- Use /ofw/openprom/model instead of olpc-bios-sig
- Add more missing dependencies
- Remove stray reference to olpc-bios-sig.c.
- Pass absolute paths to rpmbuild
- Add back sbin dirs to unprivileged users PATH
- Invoke rainbow-replay-spool

* Thu Dec 20 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.59-1
- Remove stupid 'exit 0' in zzz_olpc.sh that makes bash *exit* rather than skip the scriptlet
- Depend on tcpdump for olpc-netcapture.

* Sun Dec 16 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.58-1
- Fix version replacement in spec file

* Sun Dec 16 2007 Giannis Galanis <bernie@codewiz.org> - 0.57-1
- Merge olpc-netstatus 0.2
- Merge olpc-netlog 0.2

* Fri Dec 14 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.56-1
- Really bump revision

* Fri Dec 14 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.55-1
- Add a couple of new languages
- Add missing files
- Ensure correct keyboard is loaded even on first boot
- Don't create /root/.i18n as it makes us loose the boot time optimization
- Add code to help us improve boot time

* Fri Dec 07 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.54-1
- Add VMware configuration.
- Fix http://dev.laptop.org/ticket/5320
- Display motd in profile, not through /bin/login

* Mon Dec 03 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.53-1
- Simplyfy setxkb invocation
- Add ASCII art for motd (need more translations)
- More languages for the motd
- Replace fake input driver hack with proper config option.

* Mon Dec 03 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.52-1
- Fix http://dev.laptop.org/ticket/5114
- Simplify test for Geode
- Reindent with TABs to match other init scripts
- Remove check for A-test boards (the following code is harmelss)
- Be a little more verbose on progress.
- Fix https://dev.laptop.org/ticket/5217: Update library index
- Only run checks on start
- Use $OLPC_HOME consistently
- Only run hardware configuration on startup.
- Fix numeric test on empty flag file.
- Bump revision

* Fri Nov 30 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.51-2
- Add olpc-netcapture to %files

* Fri Nov 30 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.51-1
- Fix olpc#5195: Console font too small when using pretty boot.
- Bump revision

* Fri Nov 30 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.50-1
- Add autoconf check for PAM
- Update spec file
- Merge branch 'master' of ssh://bernie@dev.laptop.org/git/projects/olpc-utils
- Automatically push to origin on bumprev
- Fix bumprev rule
- Bump revision

* Wed Nov 28 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.49-1
- Reorganize variables
- Fix http://dev.laptop.org/ticket/4928
- Fix permissions on /home/olpc
- Bump revision

* Mon Nov 26 2007 Bernardo Innocenti <bernie@codewiz.org> - 0.48-1
- Pacify automake's portability warnings
- Update spec file
- Even more aggressive packaging automation
- Add script to import srpms in Fedora.
- Merge commit 'cscott/master'
- Explicitly strip NUL from mfg tags
- Add cvs-import.sh to EXTRADIST
- Fix https://dev.laptop.org/ticket/4762
- Bump revision

* Mon Nov 26 2007 C. Scott Ananian <bernie@codewiz.org> - 0.47-1
- Separate out configuration done to /home and /.
- Create /home/devkey.html, which can be used to request a developer key.

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
