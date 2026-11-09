VERSION_MAJOR := $(shell sed -n 's/^#define MUSLEXT_VERSION_MAJOR[ \t]*//p' include/muslext/muslext.h)
VERSION_MINOR := $(shell sed -n 's/^#define MUSLEXT_VERSION_MINOR[ \t]*//p' include/muslext/muslext.h)
VERSION_PATCH := $(shell sed -n 's/^#define MUSLEXT_VERSION_PATCH[ \t]*//p' include/muslext/muslext.h)
ABI_VERSION := $(shell sed -n 's/^#define MUSLEXT_ABI_VERSION[ \t]*//p' include/muslext/muslext.h)
VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
ifeq ($(strip $(VERSION_MAJOR)$(VERSION_MINOR)$(VERSION_PATCH)),)
$(error Cannot read muslext version from include/muslext/muslext.h)
endif
ifeq ($(strip $(ABI_VERSION)),)
$(error Cannot read muslext ABI version from include/muslext/muslext.h)
endif
# GNU make predefines CC=cc.  Only replace that default; an explicit
# CC=... on the command line or in the environment still wins.
ifeq ($(origin CC),default)
CC := $(shell command -v cc 2>/dev/null || command -v gcc 2>/dev/null || command -v clang 2>/dev/null)
endif
ifeq ($(strip $(CC)),)
$(error No C compiler found (tried cc, gcc and clang))
endif
CFLAGS ?= -O2 -g -Wall -Wextra
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?= -lm
PICFLAGS ?= -fPIC

PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
BUILDDIR ?= build
DESTDIR ?=

CPPFLAGS += -D_GNU_SOURCE=1 -Iinclude
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,$(BUILDDIR)/%.o,$(SOURCES))
SHARED := $(BUILDDIR)/libmuslext.so.$(VERSION)
SONAME := libmuslext.so.$(ABI_VERSION)
STATIC := $(BUILDDIR)/libmuslext.a
PC := $(BUILDDIR)/muslext.pc
TEST_SOURCES := $(wildcard tests/*.c)
TEST_BINS := $(patsubst tests/%.c,$(BUILDDIR)/tests/%,$(TEST_SOURCES))

.PHONY: all shared static check install clean FORCE

all: shared static $(PC)

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PICFLAGS) -c $< -o $@

shared: $(SHARED)

$(SHARED): $(OBJECTS)
	$(CC) -shared $(LDFLAGS) -Wl,-soname,$(SONAME) -o $@ $(OBJECTS) $(LDLIBS)
	ln -sf libmuslext.so.$(VERSION) $(BUILDDIR)/$(SONAME)
	ln -sf $(SONAME) $(BUILDDIR)/libmuslext.so

static: $(STATIC)

$(STATIC): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(PC): muslext.pc.in FORCE | $(BUILDDIR)
	sed -e 's|@PREFIX@|$(PREFIX)|g' \
	    -e 's|@LIBDIR@|$(LIBDIR)|g' \
	    -e 's|@INCLUDEDIR@|$(INCLUDEDIR)|g' \
	    -e 's|@VERSION@|$(VERSION)|g' $< > $@

$(BUILDDIR)/tests/%: tests/%.c shared | $(BUILDDIR)
	mkdir -p $(BUILDDIR)/tests
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -L$(BUILDDIR) \
	    -Wl,-rpath,$(abspath $(BUILDDIR)) -lmuslext $(LDLIBS) -o $@

check: $(TEST_BINS)
	@set -e; for test in $(TEST_BINS); do \
	    echo "== $$test"; $$test; \
	done

FORCE:

install: all
	install -d $(DESTDIR)$(LIBDIR) $(DESTDIR)$(INCLUDEDIR) \
	           $(DESTDIR)$(LIBDIR)/pkgconfig
	cp -R include/. $(DESTDIR)$(INCLUDEDIR)/
	install -m 644 $(SHARED) $(DESTDIR)$(LIBDIR)/libmuslext.so.$(VERSION)
	ln -sf libmuslext.so.$(VERSION) $(DESTDIR)$(LIBDIR)/$(SONAME)
	ln -sf $(SONAME) $(DESTDIR)$(LIBDIR)/libmuslext.so
	install -m 644 $(STATIC) $(DESTDIR)$(LIBDIR)/libmuslext.a
	install -m 644 $(PC) $(DESTDIR)$(LIBDIR)/pkgconfig/muslext.pc

clean:
	rm -rf $(BUILDDIR)
