/* idna.h	Declarations for IDNA.
 * Copyright (C) 2002  Simon Josefsson
 *
 * This file is part of Libstringprep.
 *
 * Libstringprep is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libstringprep is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libstringprep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _IDNA_H
#define _IDNA_H

#ifdef __cplusplus
extern "C"
{
#endif

  /* Error codes. */
  enum
  {
    IDNA_SUCCESS = 0,
    IDNA_STRINGPREP_ERROR = 1,
    IDNA_PUNYCODE_ERROR = 2,
    IDNA_CONTAINS_LDH = 3,
    IDNA_CONTAINS_MINUS = 4,
    IDNA_INVALID_LENGTH = 5,
    IDNA_NO_ACE_PREFIX = 6,
    /* Internal errors. */
    IDNA_MALLOC_ERROR = 201
  };

#ifndef IDNA_ACE_PREFIX
#define IDNA_ACE_PREFIX "iesg--"
#endif

  int idna_to_ascii (const unsigned long *in, size_t inlen,
		     char *out, int allowunassigned, int usestd3asciirules);

  int idna_to_unicode (const unsigned long *in, size_t inlen,
		       unsigned long *out, size_t *outlen,
		       int allowunassigned, int usestd3asciirules);

#ifdef __cplusplus
}
#endif
#endif				/* _PUNYCODE_H */
