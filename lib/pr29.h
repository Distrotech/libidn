/* pr29.h --- Detect strings which are non-idempotent under NFKC.
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009  Simon Josefsson.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#ifndef PR29_H
# define PR29_H

# ifndef IDNAPI
#  if defined LIBIDN_BUILDING && defined HAVE_VISIBILITY && HAVE_VISIBILITY
#   define IDNAPI __attribute__((__visibility__("default")))
#  elif defined LIBIDN_BUILDING && defined _MSC_VER && ! defined LIBIDN_STATIC
#   define IDNAPI __declspec(dllexport)
#  elif defined _MSC_VER && ! defined LIBIDN_STATIC
#   define IDNAPI __declspec(dllimport)
#  else
#   define IDNAPI
#  endif
# endif

# ifdef __cplusplus
extern "C"
{
# endif

  /* Get size_t. */
# include <stdlib.h>

  /* Get uint32_t. */
# include <idn-int.h>

  /* Error codes. */
  typedef enum
  {
    PR29_SUCCESS = 0,
    PR29_PROBLEM = 1,		/* String is a problem sequence. */
    PR29_STRINGPREP_ERROR = 2	/* Charset conversion failed (p29_8*). */
  } Pr29_rc;

  extern IDNAPI const char *pr29_strerror (Pr29_rc rc);

  extern IDNAPI int pr29_4 (const uint32_t * in, size_t len);
  extern IDNAPI int pr29_4z (const uint32_t * in);
  extern IDNAPI int pr29_8z (const char *in);

# ifdef __cplusplus
}
# endif
#endif				/* PR29_H */
