prefix?=/usr/local

termbox_cflags:=-std=c99 -Wall -Wextra -pedantic -Wno-unused-result -g -O0 -D_XOPEN_SOURCE -D_DEFAULT_SOURCE $(CFLAGS)
termbox_demos:=$(patsubst demo/%.c,demo/%,$(wildcard demo/*.c))
termbox_h:=termbox2.h
termbox_h_lib:=termbox2.h.lib
termbox_ffi_h:=termbox2.ffi.h
termbox_ffi_macro:=termbox2.ffi.macro
termbox_o:=termbox2.o
termbox_so_version_abi:=2
termbox_so_version_minor_patch:=0.0
termbox_so:=libtermbox2.so
termbox_so_x:=$(termbox_so).$(termbox_so_version_abi)
termbox_so_x_y_z:=$(termbox_so_x).$(termbox_so_version_minor_patch)
termbox_ld_soname:=soname
termbox_a:=libtermbox2.a

DOCKER?=docker

ifeq ($(shell $(CC) -dumpmachine | grep -q apple && echo 1), 1)
    termbox_so:=libtermbox2.dylib
    termbox_so_x:=libtermbox2.$(termbox_so_version_abi).dylib
    termbox_so_x_y_z:=libtermbox2.$(termbox_so_version_abi).$(termbox_so_version_minor_patch).dylib
    termbox_ld_soname:=install_name
endif

all: $(termbox_demos) $(termbox_so) $(termbox_so_x) $(termbox_a)

$(termbox_demos): %: %.c
	$(CC) -DTB_IMPL -DTB_LIB_OPTS $(termbox_cflags) $^ -o $@

$(termbox_o): $(termbox_h)
	$(CC) -DTB_IMPL -DTB_LIB_OPTS -fPIC -xc -c $(termbox_cflags) $(termbox_h) -o $@

$(termbox_so_x_y_z): $(termbox_o)
	$(CC) -shared -Wl,-$(termbox_ld_soname),$(termbox_so_x) $(termbox_o) -o $@

$(termbox_so_x): $(termbox_so_x_y_z)
	ln -sf $(termbox_so_x_y_z) $@

$(termbox_so): $(termbox_so_x_y_z)
	ln -sf $(termbox_so_x_y_z) $@

$(termbox_a): $(termbox_o)
	$(AR) rcs $@ $(termbox_o)

$(termbox_ffi_h): $(termbox_h)
	awk '/__ffi_start/{p=1} p==1 || /TERMBOX_H_INCL/{print}' $^ | $(CC) -DTB_LIB_OPTS $(termbox_cflags) -P -E - >$@

$(termbox_ffi_macro): $(termbox_h)
	awk '/__ffi_start/{p=1} p==1 || /TERMBOX_H_INCL/{print}' $^ | $(CC) -DTB_LIB_OPTS $(termbox_cflags) -P -E -dM - >$@

$(termbox_h_lib): $(termbox_h)
	sed 's|0 // __tb_lib_opts|1 // __tb_lib_opts|' $(termbox_h) >$@

terminfo:
	awk -vg=0 'g==0{print} /BEGIN codegen h/{g=1; system("./codegen.sh h")} /END codegen h/{g=0; print} g==1{next}' termbox2.h >termbox2.h.tmp && mv -vf termbox2.h.tmp termbox2.h
	awk -vg=0 'g==0{print} /BEGIN codegen c/{g=1; system("./codegen.sh c")} /END codegen c/{g=0; print} g==1{next}' termbox2.h >termbox2.h.tmp && mv -vf termbox2.h.tmp termbox2.h

format:
	clang-format -i termbox2.h

test: $(termbox_so) $(termbox_ffi_h) $(termbox_ffi_macro)
	$(DOCKER) build -f tests/Dockerfile --build-arg=cflags="$(termbox_cflags)" .

test_local: $(termbox_so) $(termbox_ffi_h) $(termbox_ffi_macro)
	./tests/run.sh

install:
	$(MAKE) install_h

lib:
	$(MAKE) $(termbox_h_lib)
	$(MAKE) $(termbox_a)
	$(MAKE) $(termbox_so)

install_lib:
	$(MAKE) install_h_lib
	$(MAKE) install_a
	$(MAKE) install_so

install_h: $(termbox_h)
	install -d $(DESTDIR)$(prefix)/include
	install -p -m 644 $(termbox_h) $(DESTDIR)$(prefix)/include/$(termbox_h)

install_h_lib: $(termbox_h_lib)
	install -d $(DESTDIR)$(prefix)/include
	install -p -m 644 $(termbox_h_lib) $(DESTDIR)$(prefix)/include/$(termbox_h)

install_a: $(termbox_a)
	install -d $(DESTDIR)$(prefix)/lib
	install -p -m 644 $(termbox_a) $(DESTDIR)$(prefix)/lib/$(termbox_a)

install_so: $(termbox_so_x_y_z)
	install -d $(DESTDIR)$(prefix)/lib
	install -p -m 755 $(termbox_so_x_y_z) $(DESTDIR)$(prefix)/lib/$(termbox_so_x_y_z)
	ln -sf $(termbox_so_x_y_z) $(DESTDIR)$(prefix)/lib/$(termbox_so_x)
	ln -sf $(termbox_so_x_y_z) $(DESTDIR)$(prefix)/lib/$(termbox_so)

clean:
	rm -f $(termbox_demos) $(termbox_o) $(termbox_a) $(termbox_so) $(termbox_so_x) $(termbox_so_x_y_z) $(termbox_ffi_h) $(termbox_ffi_macro) $(termbox_h_lib) tests/**/observed.ansi

.PHONY: all lib terminfo format test test_local install install_lib install_h install_h_lib install_a install_so clean
