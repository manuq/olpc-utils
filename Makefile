VERSION=0.2

all: setolpckeys olpc-bios-sig 

setolpckeys:
	gcc -o setolpckeys setolpckeys.c

olpc-bios-sig:
	gcc -o olpc-bios-sig olpc-bios-sig.c

install: all
	/usr/bin/install -d $(DESTDIR)/usr/share/olpc/keycodes
	/usr/bin/install -d $(DESTDIR)/usr/sbin
	/usr/bin/install olpc-evdev $(DESTDIR)/usr/share/olpc/keycodes/olpc-evdev
	/usr/bin/install setolpckeys $(DESTDIR)/usr/sbin
	/usr/bin/install olpc-bios-sig $(DESTDIR)/usr/sbin

dist/olpc-util-$(VERSION):
	mkdir -p dist/olpc-util-$(VERSION)
	cp Makefile COPYING setolpckeys.c olpc-bios-sig.c olpc-evdev dist/olpc-util-$(VERSION)/
	(cd dist; tar cvfjp olpc-util-$(VERSION).tar.bz2 olpc-util-$(VERSION); md5sum olpc-util-$(VERSION).tar.bz2 > olpc-util-$(VERSION).tar.bz2.md5)

dist: dist/olpc-util-$(VERSION)
