#!/bin/sh -x
autoreconf --install --force --verbose
./configure CFLAGS="-g -Wall -pedantic" --with-dmalloc "$@"
