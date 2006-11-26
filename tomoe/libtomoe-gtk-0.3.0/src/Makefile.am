AM_CPPFLAGS =							\
	-DSYSCONFDIR=\""$(sysconfdir)"\"			\
	-DDATADIR=\""$(datadir)"\"				\
	-DLIBTOMOEGTKDATADIR=\""$(datadir)"/$(PACKAGE)\"

INCLUDES = @GTK2_CFLAGS@ @TOMOE_CFLAGS@
LIBS     = @GTK2_LIBS@   @TOMOE_LIBS@

lib_LTLIBRARIES = libtomoe-gtk.la
libtomoe_gtk_la_SOURCES = tomoe_canvas.c tomoe_canvas.h \
	tomoe_char_table.c tomoe_char_table.h \
	tomoe_window.c tomoe_window.h \
	tomoe_stroke_search.c tomoe_stroke_search.h \
	tomoe_dictionaries_page.c tomoe_dictionaries_page.h \
	tomoe_reading_search_page.c tomoe_reading_search_page.h \
	tomoe_dictionary_page.c tomoe_dictionary_page.h \
	tomoe_details.c tomoe_details.h \
	tomoe_edit_meta.c tomoe_edit_meta.h \
	tomoe_edit_char.c tomoe_edit_char.h \
	tomoe_edit_strokes.c tomoe_edit_strokes.h

libtomoe_gtk_la_LDFLAGS = -version-info 0:0:0

pkgincludedir = $(includedir)/tomoe
pkginclude_HEADERS = tomoe-gtk.h \
	tomoe_canvas.h tomoe_char_table.h tomoe_window.h tomoe_stroke_search.h \
	tomoe_dictionaries_page.h tomoe_reading_search_page.h tomoe_edit_strokes.h \
	tomoe_dictionary_page.h tomoe_details.h tomoe_edit_meta.h tomoe_edit_char.h
