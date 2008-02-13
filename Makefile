# flags

WARNFLAGS = \
        -W -Wformat -Wall -Wundef -Wpointer-arith -Wcast-qual \
        -Wcast-align -Wwrite-strings -Wsign-compare \
        -Wmissing-noreturn \
        -Wextra -Wstrict-aliasing=2 \
        -Wunsafe-loop-optimizations


# files

SETOLPCKEYS_OBJS = src/setolpckeys.o
OLPC_DM_OBJS = src/olpc-dm.o

BINARIES = usr/sbin/setolpckeys usr/sbin/olpc-dm
OBJS = $(SETOLPCKEYS_OBJS) $(OLPC_DM_OBJS)
OVERLAYS = etc usr
BUILT_DIRS= usr/sbin


# default target

all: $(BINARIES)


# objects

%.o: %.c
	$(CC) $(WARNFLAGS) -c -o $@ $<


# built works

usr/sbin/setolpckeys: $(SETOLPCKEYS_OBJS)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(filter %.o,$^)

usr/sbin/olpc-dm: $(OLPC_DM_OBJS)
	mkdir -p $(dir $@)
	$(CC) -lpam_misc -lpam -o $@ $(filter %.o,$^)


# install targets

$(DESTDIR)/usr/%: usr/%
	install -D $< $@

$(DESTDIR)/etc/%: etc/%
	install -D -m 644 $< $@

$(DESTDIR)/etc/rc.d/init.d/%: etc/rc.d/init.d/%
	install -D $< $@

$(foreach d,$(shell find $(OVERLAYS) -type d),$(DESTDIR)/$d):
	install -D -d $@

install: $(BUILT_DIRS) $(BINARIES) $(foreach f,$(shell find $(OVERLAYS) -not -type d),$(DESTDIR)/$f)


# clean target

.PHONY: clean
clean:
	rm -f $(BINARIES) $(OBJS)
	rm -rf $(BUILT_DIRS)

# vim: noet sts=4 ts=4 sw=4 :
