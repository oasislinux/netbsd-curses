.POSIX:

-include config.mk

PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
LIBDIR?=$(PREFIX)/lib
INCDIR?=$(PREFIX)/include
DATADIR?=$(PREFIX)/share
MANDIR?=$(DATADIR)/man
MAN1DIR?=$(MANDIR)/man1
MAN3DIR?=$(MANDIR)/man3

ARFLAGS?=-cr
CFLAGS+=\
	-Wall\
	-Wpedantic\
	-D _PATH_TERMINFO=\"$(DATADIR)/misc/terminfo\"\
	-D TERMINFO_COMPILE\
	-D TERMINFO_COMPAT\
	-D TERMINFO_DB\
	-D _XOPEN_SOURCE=700\
	-I compat\
	-I lib/libcurses\
	-I lib/libterminfo\
	-I lib/libform\
	-I lib/libmenu\
	-I lib/libpanel
YFLAGS+=-d
HOSTCC?=$(CC)
HOSTCFLAGS?=$(CFLAGS)
HOSTLDFLAGS?=$(LDFLAGS)

TERM_BUILTIN?=ansi dumb vt100 vt220 xterm xterm-256color
ENABLE_WCHAR?=1

LIBCURSES_SRC=\
	acs.c addbytes.c addch.c addchnstr.c addnstr.c attributes.c\
	background.c bell.c border.c box.c chgat.c clear.c clearok.c\
	clrtobot.c clrtoeol.c color.c copywin.c cr_put.c\
	ctrace.c cur_hash.c curs_set.c\
	curses.c delch.c deleteln.c delwin.c echochar.c erase.c fileio.c\
	flushok.c fullname.c getch.c getstr.c getyx.c id_subwins.c idlok.c\
	idcok.c immedok.c inch.c inchstr.c initscr.c insch.c insdelln.c\
	insertln.c insstr.c instr.c keypad.c keyname.c leaveok.c line.c\
	meta.c mouse.c move.c\
	mvwin.c newwin.c nodelay.c notimeout.c overlay.c overwrite.c pause.c\
	printw.c putchar.c refresh.c resize.c ripoffline.c scanw.c screen.c\
	scroll.c scrollok.c setterm.c slk.c standout.c syncok.c timeout.c\
	toucholap.c touchwin.c tstp.c tty.c unctrl.c underscore.c version.c\
	$(LIBCURSES_SRC-1)
LIBCURSES_SRC-$(ENABLE_WCHAR)=\
	cchar.c add_wch.c add_wchstr.c addwstr.c echo_wchar.c ins_wch.c\
	ins_wstr.c get_wch.c get_wstr.c in_wch.c in_wchstr.c\
	inwstr.c
LIBCURSES_OBJ=\
	$(LIBCURSES_SRC:%.c=lib/libcurses/%.o)
LIBCURSES_HDR=\
	curses.h unctrl.h
LIBCURSES_MAN3=\
	curses.3 curses_addch.3 curses_addchstr.3 curses_addstr.3\
	curses_attributes.3 curses_background.3 curses_border.3\
	curses_cchar.3\
	curses_chgat.3 curses_clear.3 curses_color.3\
	curses_cursor.3 curses_default_colors.3\
	curses_delch.3 curses_deleteln.3 curses_echochar.3 curses_fileio.3\
	curses_inch.3 curses_input.3 curses_insch.3 curses_insertln.3\
	curses_insdelln.3 curses_keyname.3 curses_line.3\
	curses_mouse.3 curses_pad.3\
	curses_print.3 curses_refresh.3 curses_scanw.3 curses_screen.3\
	curses_scroll.3 curses_slk.3 curses_standout.3 curses_termcap.3\
	curses_touch.3 curses_tty.3 curses_underscore.3 curses_version.3\
	curses_window.3

LIBFORM_SRC=\
	driver.c field_types.c internals.c field.c form.c post.c type_alnum.c\
        type_alpha.c type_integer.c type_numeric.c type_enum.c type_regex.c\
	type_ipv4.c type_ipv6.c
LIBFORM_OBJ=\
	$(LIBFORM_SRC:%.c=lib/libform/%.o)
LIBFORM_HDR=\
	form.h
LIBFORM_MAN3=\
	form_cursor.3 form_data.3 form_driver.3 form_field.3\
	form_field_attributes.3 form_field_buffer.3 form_field_info.3\
	form_field_just.3 form_field_new.3 form_field_opts.3\
	form_field_userptr.3 form_field_validation.3 form_fieldtype.3\
	form_hook.3 form_new.3 form_new_page.3 form_opts.3 form_page.3\
	form_post.3 form_userptr.3 form_win.3 forms.3

LIBMENU_SRC=\
	menu.c item.c userptr.c internals.c driver.c post.c attributes.c
LIBMENU_OBJ=\
	$(LIBMENU_SRC:%.c=lib/libmenu/%.o)
LIBMENU_HDR=\
	menu.h eti.h
LIBMENU_MAN3=\
	menu_attributes.3 menu_item_name.3 menu_items.3 menu_userptr.3\
	menu_cursor.3 menu_item_new.3 menu_mark.3 menu_win.3 menu_driver.3\
	menu_item_opts.3 menu_new.3 menus.3 menu_format.3 menu_item_userptr.3\
	menu_opts.3 menu_hook.3 menu_item_value.3 menu_pattern.3\
	menu_item_current.3 menu_item_visible.3 menu_post.3

LIBPANEL_SRC=\
	_deck.c above.c below.c bottom.c del.c getuser.c hidden.c hide.c move.c\
	new.c replace.c setuser.c show.c top.c update.c window.c
LIBPANEL_OBJ=\
	$(LIBPANEL_SRC:%.c=lib/libpanel/%.o)
LIBPANEL_HDR=\
	panel.h
LIBPANEL_MAN3=\
	move_panel.3 new_panel.3 panel.3 panel_above.3 panel_hidden.3 \
	panel_userptr.3 update_panels.3

LIBTERMINFO_SRC=\
	term.c ti.c setupterm.c curterm.c tparm.c tputs.c\
	hash.c\
	compile.c\
	termcap.c
LIBTERMINFO_OBJ=\
	$(LIBTERMINFO_SRC:%.c=lib/libterminfo/%.o)\
	compat/cdbr.o\
	compat/mi_vector_hash.o
LIBTERMINFO_HDR=\
	term.h termcap.h
LIBTERMINFO_MAN3=\
	terminfo.3

NBPERF_SRC=\
	nbperf.c\
	nbperf-bdz.c nbperf-chm.c nbperf-chm3.c\
	graph2.c graph3.c
HOST_NBPERF_OBJ=\
	$(NBPERF_SRC:%.c=host-%.o)\
	host-mi_vector_hash.o

INFOCMP_SRC=\
	infocmp.c
INFOCMP_OBJ=\
	$(INFOCMP_SRC:%.c=usr.bin/infocmp/%.o)\
	libterminfo.a
INFOCMP_MAN1=\
	infocmp.1

TABS_SRC=\
	tabs.c
TABS_OBJ=\
	$(TABS_SRC:%.c=usr.bin/tabs/%.o)\
	libterminfo.a
TABS_MAN1=\
	tabs.1

TIC_SRC=\
	tic.c
TIC_OBJ=\
	$(TIC_SRC:%.c=usr.bin/tic/%.o)\
	compat/cdbw.o\
	libterminfo.a
TIC_MAN1=\
	tic.1
HOST_TIC_OBJ=\
	$(TIC_SRC:%.c=host-%.o)\
	host-compile.o\
	host-hash.o\
	host-cdbw.o\
	host-mi_vector_hash.o

TPUT_SRC=\
	tput.c
TPUT_OBJ=\
	$(TPUT_SRC:%.c=usr.bin/tput/%.o)\
	libterminfo.a
TPUT_MAN1=\
	tput.1

TSET_SRC=\
	map.c misc.c set.c term.c tset.c
TSET_OBJ=\
	$(TSET_SRC:%.c=usr.bin/tset/%.o)\
	libterminfo.a
TSET_MAN1=\
	tset.1

LIBS=libcurses.a libform.a libmenu.a libpanel.a libterminfo.a
BINS=infocmp tabs tic tput tset

all: $(LIBS) $(BINS)

host-cdbw.o: compat/cdbw.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ compat/cdbw.c
host-mi_vector_hash.o: compat/mi_vector_hash.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ compat/mi_vector_hash.c

host-nbperf.o: usr.bin/nbperf/nbperf.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/nbperf.c
host-nbperf-bdz.o: usr.bin/nbperf/nbperf-bdz.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/nbperf-bdz.c
host-nbperf-chm.o: usr.bin/nbperf/nbperf-chm.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/nbperf-chm.c
host-nbperf-chm3.o: usr.bin/nbperf/nbperf-chm3.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/nbperf-chm3.c
host-graph2.o: usr.bin/nbperf/graph2.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/graph2.c
host-graph3.o: usr.bin/nbperf/graph3.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/nbperf/graph3.c
host-nbperf: $(HOST_NBPERF_OBJ)
	$(HOSTCC) $(HOSTLDFLAGS) -o $@ $(HOST_NBPERF_OBJ)

host-tic.o: usr.bin/tic/tic.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ usr.bin/tic/tic.c
host-compile.o: lib/libterminfo/compile.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ lib/libterminfo/compile.c
host-hash.o: lib/libterminfo/hash.c
	$(HOSTCC) $(HOSTCFLAGS) -c -o $@ lib/libterminfo/hash.c
host-tic: $(HOST_TIC_OBJ)
	$(HOSTCC) $(HOSTLDFLAGS) -o $@ $(HOST_TIC_OBJ)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

libcurses.a: $(LIBCURSES_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBCURSES_OBJ)

libform.a: $(LIBFORM_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBFORM_OBJ)

libmenu.a: $(LIBMENU_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBMENU_OBJ)

libpanel.a: $(LIBPANEL_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBPANEL_OBJ)

lib/libterminfo/compiled_terms.c: share/terminfo/terminfo host-tic
	./host-tic -Sx share/terminfo/terminfo $(TERM_BUILTIN) >$@.tmp && mv $@.tmp $@
terminfo.cdb: share/terminfo/terminfo host-tic
	./host-tic -x -o $@.tmp share/terminfo/terminfo && mv $@.tmp $@
lib/libterminfo/hash.c: lib/libterminfo/genhash lib/libterminfo/term.h host-nbperf
	TOOL_NBPERF=./host-nbperf lib/libterminfo/genhash\
		lib/libterminfo/term.h >$@.tmp && mv $@.tmp $@
lib/libterminfo/termcap_hash.c: lib/libterminfo/genthash lib/libterminfo/termcap_map.c host-nbperf
	TOOL_NBPERF=./host-nbperf lib/libterminfo/genthash\
		lib/libterminfo/termcap_map.c >$@.tmp && mv $@.tmp $@
lib/libterminfo/term.o: lib/libterminfo/compiled_terms.c
lib/libterminfo/termcap.o: lib/libterminfo/termcap_hash.c
libterminfo.a: $(LIBTERMINFO_OBJ)
	$(AR) $(ARFLAGS) $@ $(LIBTERMINFO_OBJ)

infocmp: $(INFOCMP_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(INFOCMP_OBJ)

tabs: $(TABS_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TABS_OBJ)

tic: $(TIC_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TIC_OBJ)

tput: $(TPUT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TPUT_OBJ)

tset: $(TSET_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TSET_OBJ)

tests/lib/libcurses/director/testlang_parse.h: tests/lib/libcurses/director/testlang_parse.c
	mv -f y.tab.h $@
tests/lib/libcurses/director/testlang_parse.o: tests/lib/libcurses/director/testlang_parse.c
tests/lib/libcurses/director/testlang_conf.o: tests/lib/libcurses/director/testlang_parse.h

TEST_DIRECTOR_OBJ=\
	tests/lib/libcurses/director/testlang_parse.o\
	tests/lib/libcurses/director/testlang_conf.o\
	tests/lib/libcurses/director/director.o

TEST_SLAVE_OBJ=\
	tests/lib/libcurses/slave/slave.o\
	tests/lib/libcurses/slave/commands.o\
	tests/lib/libcurses/slave/curses_commands.o

tests/lib/libcurses/t_curses: tests/lib/libcurses/t_curses.sh
	{ echo '#!/usr/bin/env atf-sh'; cat tests/lib/libcurses/t_curses.sh; } >$@
	chmod +x $@

tests/lib/libcurses/slave/slave.o: tests/lib/libcurses/slave/slave.c
	$(CC) $(CFLAGS) -I tests/lib/libcurses/director -c -o $@ tests/lib/libcurses/slave/slave.c

tests/lib/libcurses/slave/commands.o: tests/lib/libcurses/slave/commands.c
	$(CC) $(CFLAGS) -I tests/lib/libcurses/director -c -o $@ tests/lib/libcurses/slave/commands.c

tests/lib/libcurses/terminfo.cdb: host-tic tests/lib/libcurses/atf.terminfo
	./host-tic -o $@ tests/lib/libcurses/atf.terminfo

tests/lib/libcurses/director/director: $(TEST_DIRECTOR_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(TEST_DIRECTOR_OBJ)

tests/lib/libcurses/slave/slave: $(TEST_SLAVE_OBJ) libcurses.a libterminfo.a
	$(CC) $(LDFLAGS) -o $@ $(TEST_SLAVE_OBJ) libcurses.a libterminfo.a

.PHONY: check
check: tests/lib/libcurses/t_curses tests/lib/libcurses/director/director tests/lib/libcurses/slave/slave tests/lib/libcurses/terminfo.cdb
	kyua test -k tests/lib/libcurses/Kyuafile

.PHONY: install
install: $(LIBS) $(BINS) terminfo.cdb
	mkdir -p\
		$(DESTDIR)$(LIBDIR)\
		$(DESTDIR)$(BINDIR)\
		$(DESTDIR)$(DATADIR)/misc\
		$(DESTDIR)$(INCDIR)\
		$(DESTDIR)$(MAN1DIR)\
		$(DESTDIR)$(MAN3DIR)
	cp $(LIBS) $(DESTDIR)$(LIBDIR)/
	cp $(BINS) $(DESTDIR)$(BINDIR)/
	cp share/terminfo/terminfo terminfo.cdb $(DESTDIR)$(DATADIR)/misc/
	cp\
		$(LIBCURSES_HDR:%=lib/libcurses/%)\
		$(LIBFORM_HDR:%=lib/libform/%)\
		$(LIBMENU_HDR:%=lib/libmenu/%)\
		$(LIBPANEL_HDR:%=lib/libpanel/%)\
		$(LIBTERMINFO_HDR:%=lib/libterminfo/%)\
		$(DESTDIR)$(INCDIR)/
	cp\
		$(INFOCMP_MAN1:%=usr.bin/infocmp/%)\
		$(TABS_MAN1:%=usr.bin/tabs/%)\
		$(TIC_MAN1:%=usr.bin/tic/%)\
		$(TPUT_MAN1:%=usr.bin/tput/%)\
		$(TSET_MAN1:%=usr.bin/tset/%)\
		$(DESTDIR)$(MAN1DIR)/
	cp\
		$(LIBCURSES_MAN3:%=lib/libcurses/%)\
		$(LIBFORM_MAN3:%=lib/libform/%)\
		$(LIBMENU_MAN3:%=lib/libmenu/%)\
		$(LIBPANEL_MAN3:%=lib/libpanel/%)\
		$(LIBTERMINFO_MAN3:%=lib/libterminfo/%)\
		$(DESTDIR)$(MAN3DIR)/

.PHONY: clean
clean:
	rm -f\
		host-nbperf $(HOST_NBPERF_OBJ)\
		host-tic $(HOST_TIC_OBJ)\
		libcurses.a $(LIBCURSES_OBJ)\
		libform.a $(LIBFORM_OBJ)\
		libmenu.a $(LIBMENU_OBJ)\
		libpanel.a $(LIBPANEL_OBJ)\
		libterminfo.a $(LIBTERMINFO_OBJ)\
		infocmp $(INFOCMP_OBJ)\
		tabs $(TABS_OBJ)\
		tic $(TIC_OBJ)\
		tput $(TPUT_OBJ)\
		tset $(TSET_OBJ)\
		lib/libterminfo/compiled_terms.c\
		lib/libterminfo/hash.c\
		lib/libterminfo/termcap_hash.c\
		terminfo.cdb\
		tests/lib/libcurses/t_curses\
		tests/lib/libcurses/terminfo.cdb\
		tests/lib/libcurses/director/director $(TEST_DIRECTOR_OBJ)\
		tests/lib/libcurses/director/testlang_conf.c\
		tests/lib/libcurses/director/testlang_parse.c\
		tests/lib/libcurses/director/testlang_parse.h\
		tests/lib/libcurses/slave/slave $(TEST_SLAVE_OBJ)
