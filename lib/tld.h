/* tld.h --- Declarations for TLD restriction checking.
 * Copyright (C) 2004  Simon Josefsson.
 * Copyright (C) 2003, 2004  Free Software Foundation, Inc.
 *
 * Author: Thomas Jacob, Internet24.de
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

#ifndef _TLD_H
#define _TLD_H

#ifdef __cplusplus
extern "C"
{
#endif

  /* Get size_t. */
#include <stdlib.h>

  /* Get uint32_t. */
#include <idn-int.h>

  /* Interval of valid code points in the TLD. */
  struct Tld_table_element
  {
    uint32_t start;
    uint32_t end;
  };
  typedef struct Tld_table_element Tld_table_element;

  /* List valid code points in a TLD. */
  struct Tld_table
  {
    char *name;				/* TLD name, e.g., "no". */
    char *version;			/* Version string from TLD file. */
    size_t nvalid;			/* Number of entries in data. */
    const Tld_table_element *valid;	/* Sorted array of valid code points. */
  };
  typedef struct Tld_table Tld_table;

  /* Array of built-in domain restriction structures. */
  extern const Tld_table *tld_table[];

  /* Error codes. */
  typedef enum
    {
      TLD_SUCCESS = 0,
      TLD_ILLEGAL = 1,		/* Illegal character found. */
      TLD_NODATA = 2,		/* Char, domain or inlen = 0. */
      TLD_MALLOC_ERROR = 3,
      TLD_ICONV_ERROR = 4,
      TLD_NOTLD = 5
    } Tld_rc;

  /* Return structure corresponding to the named tld, first looking
   * thru xtra_tlds then thru built-in list, or NULL if not found. */
  const Tld_table *tld_finddomain (const char *tld_str,
				   const Tld_table ** xtra_tlds);

  /* Verify if ch is either in [a-z0-9-.] or mentioned as a legal
   * character in tld and return TLD_SUCCESS/ILLEGAL respectively. */
  int tld_checkchar (uint32_t ch, const Tld_table * tld);

  /* Determine TLD of UCS4 DNS name and return in out. */
  int tld_gettld_4i (const uint32_t * in, size_t inlen, char **out);
  int tld_gettld_4z (const uint32_t * in, char **out);

  /* Check NAMEPREPPED domain name for valid characters as defined by
   * the relevant registering body + [a-z0-9.-] return Tld_rc,
   * position of offending character in errpos if TLD_ILLEGAL. */
  int tld_check_4it (const uint32_t * in, size_t inlen, size_t * errpos,
		     const Tld_table * tld);
  int tld_check_4iz (const uint32_t * in, size_t * errpos,
		     const Tld_table * tld);

  int tld_check_4i (const uint32_t * in, size_t inlen, size_t * errpos,
		    const Tld_table ** xtra_tlds);
  int tld_check_4z (const uint32_t * in, size_t * errpos,
		    const Tld_table ** xtra_tlds);
  int tld_check_8z (const char *in, size_t * errpos,
		    const Tld_table ** xtra_tlds);
  int tld_check_lz (const char *in, size_t * errpos,
		    const Tld_table ** xtra_tlds);

#ifdef __cplusplus
}
#endif
#endif				/* _TLD_H */
