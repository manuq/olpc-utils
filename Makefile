VERSION=0.12

prefix=/usr/local
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
sbindir=${exec_prefix}/sbin
mandir=${prefix}/man
datadir=${prefix}/share

all: setolpckeys olpc-bios-sig 

setolpckeys:
	gcc -o setolpckeys setolpckeys.c

olpc-bios-sig:
	gcc -o olpc-bios-sig olpc-bios-sig.c

install: all
	/usr/bin/install -p -d $(DESTDIR)/$(datadir)/olpc/keycodes
	/usr/bin/install -p -d $(DESTDIR)/$(sbindir)
	/usr/bin/install -p --mode=0664 olpc-evdev $(DESTDIR)/$(datadir)/olpc/keycodes/olpc-evdev
	/usr/bin/install -p setolpckeys $(DESTDIR)/$(sbindir)
	/usr/bin/install -p olpc-bios-sig $(DESTDIR)/$(sbindir)

dist/olpc-utils-$(VERSION):
	mkdir -p dist/olpc-utils-$(VERSION)
	cp Makefile COPYING setolpckeys.c olpc-bios-sig.c olpc-evdev dist/olpc-utils-$(VERSION)/
	(cd dist; tar cvfjp olpc-utils-$(VERSION).tar.bz2 olpc-utils-$(VERSION); md5sum olpc-utils-$(VERSION).tar.bz2 > olpc-utils-$(VERSION).tar.bz2.md5)

dist: dist/olpc-utils-$(VERSION)
