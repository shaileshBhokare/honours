#!/bin/sh

libtoolize --copy --force \
  && aclocal \
  && autoheader \
  && automake --add-missing --foreign --copy

glib-gettextize --copy --force
autoconf

/bin/rm -rf autom4te.cache
