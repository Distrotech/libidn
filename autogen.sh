#!/bin/sh -x
autoreconf --install --force --verbose
: 'You can now run ./configure CFLAGS=-g --disable-shared and then make.'
