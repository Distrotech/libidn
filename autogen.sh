#!/bin/sh -x
autoreconf --install --force --verbose
: 'You can now run ./configure CFLAGS=-g --enable-maintainer-mode --disable-shared && make.'
