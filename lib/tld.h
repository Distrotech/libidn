/* tldchk.h·  Declarations for TLD restriction checking
 * 
 * Author  Thomas Jacob, Internet24.de
 * 
 * Copyright (C) 2003,2004  Free Software Foundation, Inc.
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

#ifndef _TLDCHK_H
#define _TLDCHK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <idn-int.h>

/* Domain restriction structure. */
  struct TLDCHK_domain
  {
    char *name;
	int vermaj;   /* Major and minor version number of information */
	int vermin;
    size_t num_singles;		/* Number of entries in singles */
    size_t num_intervals;	/* Number of double entries in intervals */
    const uint32_t *singles;	/* Sorted array of legal single chars. */
    const uint32_t *intervals;	/* Sorted array of legal char intervals. */
  };
  typedef struct TLDCHK_domain TLDCHK_domain;

/* Array of built-in domain restriction structures. */
  extern const TLDCHK_domain *tldchk_domains[];

/* Error codes. */
  typedef enum
  {
    TLDCHK_SUCCESS = 0,
    TLDCHK_ILLEGAL = 1,		/* Illegal character found. */
    TLDCHK_NODATA = 2,		/* Char, domain or inlen = 0. */
    TLDCHK_MALLOC_ERROR = 3,
    TLDCHK_ICONV_ERROR = 4,
	TLDCHK_NOTLD = 5
  } Tldchk_rc;

/* Determine TLD of UCS4 DNS name and return in out. */
  int tld_gettld_4i (const uint32_t * in, size_t inlen, char **out);
  int tld_gettld_4z (const uint32_t * in, char **out);

/* Check >!nameprepped!< domain name for valid characters as
 * defined by the relevant registering body + [a-z0-9.-]
 * return Tldchk_rc, position of offending character in
 * errpos if TLDCHK_ILLEGAL.
 */
  int tld_check_4it (const uint32_t * in, size_t inlen, size_t * errpos,
		     const TLDCHK_domain * tld);
  int tld_check_4iz (const uint32_t * in, size_t * errpos,
		     const TLDCHK_domain * tld);

  int tld_check_4i (const uint32_t * in, size_t inlen, size_t * errpos,
		    const TLDCHK_domain ** xtra_tlds);
  int tld_check_4z (const uint32_t * in, size_t * errpos,
		    const TLDCHK_domain ** xtra_tlds);
  int tld_check_8z (const char *in, size_t * errpos,
		    const TLDCHK_domain ** xtra_tlds);
  int tld_check_lz (const char *in, size_t * errpos,
		    const TLDCHK_domain ** xtra_tlds);

/* Verify if ch is either in [a-z0-9-.] or mentioned
 * as a legal character in tld and return TLDCHK_SUCCESS/ILLEGAL
 * respectively.
 */
  int tld_checkchar (uint32_t ch, const TLDCHK_domain * tld);

/* Return structure corresponding to the named tld, first looking thru
 * xtra_tlds then thru built-in list, or NULL if not found.
 */
  const TLDCHK_domain *tld_finddomain (char *tld_str,
				       const TLDCHK_domain ** xtra_tlds);


#ifdef __cplusplus
}
#endif
#endif				/* _TLDCHK_H */
