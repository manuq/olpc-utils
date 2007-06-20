VERSION=0.10

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

dist/olpc-utils-$(VERSION):
	mkdir -p dist/olpc-utils-$(VERSION)
	cp Makefile COPYING setolpckeys.c olpc-bios-sig.c olpc-evdev dist/olpc-utils-$(VERSION)/
	(cd dist; tar cvfjp olpc-utils-$(VERSION).tar.bz2 olpc-utils-$(VERSION); md5sum olpc-utils-$(VERSION).tar.bz2 > olpc-utils-$(VERSION).tar.bz2.md5)

dist: dist/olpc-utils-$(VERSION)
