/* idn-stub.c --- Stub to dlopen libcidn.so and invoke idna_to_ascii_lz.
 * Copyright (C) 2003, 2004  Simon Josefsson
 *
 * This file is part of GNU Libidn.
 *
 * GNU Libidn is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * GNU Libidn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GNU Libidn; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <dlfcn.h>

/* Get specification for idna_to_ascii_lz. */
#include "idna.h"

/* Stub to dlopen libcidn.so and invoke the real idna_to_ascii_lz, or
   return IDNA_DLOPEN_ERROR on failure.  */
int
idna_to_ascii_lz (const char *input, char **output, int flags)
{
  void *h;
  int (*to_ascii_lz) (const char *input, char **output, int flags);
  int rc;

  h = __libc_dlopen ("libcidn.so");

  if (!h)
    return IDNA_DLOPEN_ERROR;

  to_ascii_lz = __libc_dlsym (h, "idna_to_ascii_lz");

  rc = to_ascii_lz (input, output, flags);

  __libc_dlclose(h);

  return rc;
}
