#!/bin/sh -x
autoreconf --install --force --verbose
: 'You can now run CFLAGS=-g ./configure --disable-shared and then make.'
