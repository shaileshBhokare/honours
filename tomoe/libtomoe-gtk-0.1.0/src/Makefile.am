lib_LTLIBRARIES = libtomoe-gtk.la
libtomoe_gtk_la_SOURCES = tomoe_canvas.c tomoe_canvas.h \
	tomoe_candidates_view.c tomoe_candidates_view.h \
	tomoe_window.c tomoe_window.h

INCLUDES = @GTK2_CFLAGS@ @TOMOE_CFLAGS@
LIBS     = @GTK2_LIBS@   @TOMOE_LIBS@

pkgincludedir = $(includedir)/tomoe
pkginclude_HEADERS = tomoe-gtk.h \
	tomoe_canvas.h tomoe_candidates_view.h tomoe_window.h
