/* tldchk.c  Handle TLD restriction checking
 *
 * Author  Thomas Jacob, Internet24.de
 *
 * Copyright (C) 2003,2004   Free Software Foundation, Inc.
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

#include <tldchk.h>
#include <string.h>
#include <stringprep.h>

#define DOTP(c) ((c) == 0x002E || (c) == 0x3002 ||     \
                 (c) == 0xFF0E || (c) == 0xFF61)

/**
 * tld_gettld_4i:
 * @in: Array of unicode code points to process (Does not need to be
 * zero terminated).
 * @inlen: Number of unicode code points.
 * @out: Zero terminated ascii result string pointer.
 *
 * Isolate the top-level domain of @in and return it as
 * an ascii string in @out.
 *
 * Return value: Return %TLDCHK_SUCCESS on success, the corresponding
 * error code otherwise.
 */
int
tld_gettld_4i (const uint32_t * in, size_t inlen, char **out)
{
  const uint32_t *ipos;
  size_t olen;

  *out = NULL;
  if (!in || (inlen <= 0))
    return TLDCHK_NODATA;

  ipos = &in[inlen - 1];
  olen = 0;
  /* Scan backwards for non(latin)letters. */
  while (ipos >= in && ((*ipos >= 0x41 && *ipos <= 0x5A) ||
			(*ipos >= 0x61 && *ipos <= 0x7A)))
    ipos--, olen++;

  if (olen > 0 && DOTP (*ipos))	/* Found something that appears a TLD. */
    {
      char *out_s = malloc (sizeof (char) * (olen + 1));
      char *opos = out_s;

      if (!opos)
	return TLDCHK_MALLOC_ERROR;

      ipos++;
      /* Transcribe to lowercase ascii string. */
      for (; ipos < &in[inlen]; ipos++, opos++)
	*opos = *ipos > 0x5A ? *ipos : *ipos + 0x20;
      *opos = 0;
      *out = out_s;
	  return TLDCHK_SUCCESS;
    }

  return TLDCHK_NOTLD;
}


/**
 * tld_gettld_4z:
 * @in: Zero terminated array of unicode code points to process.
 * @out: Zero terminated ascii result string pointer.
 *
 * Isolate the top-level domain of @in and return it as
 * an ascii string in @out.
 *
 * Return value: Returns %TLDCHK_SUCCESS on success, the corresponding
 * error code otherwise.
 */
int
tld_gettld_4z (const uint32_t * in, char **out)
{
  const uint32_t *ipos = in;

  if (!in)
    return TLDCHK_NODATA;

  while (*ipos)
    ipos++;

  return tld_gettld_4i (in, ipos - in, out);
}


/**
 * tld_check_4it
 * @in: Array of unicode code points to process (Does not need to be
 * zero terminated).
 * @inlen: Number of unicode code points.
 * @errpos: Position of offending character is returned here.
 * @tld: Data structure representing the restrictions for
 * which the input should be tested.
 *
 * Test each of the code points in @in for whether or not
 * they are allowed by the data structure in @tld, return
 * the position of the first character for which this is not
 * the case in @errpos.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all code points
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_4it (const uint32_t * in, size_t inlen, size_t * errpos,
	       const TLDCHK_domain * tld)
{
  const uint32_t *ipos;
  int rc;

  if (!tld)			/* No data for TLD so everything is valid. */
    return TLDCHK_SUCCESS;

  ipos = in;
  while (ipos < &in[inlen])
    {
      rc = tld_checkchar (*ipos, tld);
      if (rc != TLDCHK_SUCCESS)
	{
	  if (errpos)
	    {
	      *errpos = ipos - in;
	    }
	  return rc;
	}
      ipos++;
    }
  return TLDCHK_SUCCESS;
}

/**
 * tld_check_4zt
 * @in: Zero terminated array of unicode code points to process.
 * @errpos: Position of offending character is returned here.
 * @tld: Data structure representing the restrictions for
 * which the input should be tested.
 *
 * Test each of the code points in @in for whether or not
 * they are allowed by the data structure in @tld, return
 * the position of the first character for which this is not
 * the case in @errpos.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all code points
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_4zt (const uint32_t * in, size_t * errpos,
	       const TLDCHK_domain * tld)
{
  const uint32_t *ipos = in;

  if (!ipos)
    return TLDCHK_NODATA;

  while (*ipos)
    ipos++;

  return tld_check_4it (in, ipos - in, errpos, tld);
}

/**
 * tld_check_4i
 * @in: Array of unicode code points to process (Does not need to be
 * zero terminated).
 * @inlen: Number of unicode code points.
 * @errpos: Position of offending character is returned here.
 * @xtra_tlds: An array of additional domain restriction structures
 *  that complement and supersede the built-in information.
 *
 * Test each of the code points in @in for whether or not
 * they are allowed by the information in @xtra_tlds or by the built-in
 * TLD restriction data. When data for the same TLD is available both internally
 * and in @xtra_tlds, the information in @xtra_tlds takes precedence. If
 * several entries for a specific TLD are found, the first one is used.
 * If @xtra_tlds is %NULL, only the built-in information is used.
 * The position of the first offending character is returned in @errpos.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all code points
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_4i (const uint32_t * in, size_t inlen, size_t * errpos,
	      const TLDCHK_domain ** xtra_tlds)
{
  const uint32_t *ipos;
  const TLDCHK_domain *tld;
  char *domain;
  int rc;

  *errpos = 0;
  /* Get TLD name. */
  rc = tld_gettld_4i (in, inlen, &domain);

  if (rc != TLDCHK_SUCCESS)
  {
    if (rc == TLDCHK_NOTLD) /* No TLD, say OK */
	  return TLDCHK_SUCCESS;
	else
      return rc;
  }

  /* Retrieve appropriate data structure. */
  tld = tld_finddomain (domain, xtra_tlds);
  free (domain);

  return tld_check_4it (in, inlen, errpos, tld);
}

/**
 * tld_check_4z
 * @in: Zero-terminated array of unicode code points to process.
 * @errpos: Position of offending character is returned here.
 * @xtra_tlds: An array of additional domain restriction structures
 * that complement and supersede the built-in information.
 *
 * Test each of the code points in @in for whether or not
 * they are allowed by the information in @xtra_tlds or by the built-in
 * TLD restriction data. When data for the same TLD is available both internally
 * and in @xtra_tlds, the information in @xtra_tlds takes precedence. If
 * several entries for a specific TLD are found, the first one is used.
 * If @xtra_tlds is %NULL, only the built-in information is used.
 * The position of the first offending character is returned in @errpos.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all code points
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_4z (const uint32_t * in, size_t * errpos,
	      const TLDCHK_domain ** xtra_tlds)
{
  const uint32_t *ipos = in;

  if (!ipos)
    return TLDCHK_NODATA;

  while (*ipos)
    ipos++;

  return tld_check_4i (in, ipos - in, errpos, xtra_tlds);
}

/**
 * tld_check_8z
 * @in: Zero-terminated UTF8 string to process.
 * @errpos: Position of offending character is returned here.
 * @xtra_tlds: An array of additional domain restriction structures
 * that complement and supersede the built-in information.
 *
 * Test each of the characters in @in for whether or not
 * they are allowed by the information in @xtra_tlds or by the built-in
 * TLD restriction data. When data for the same TLD is available both internally
 * and in @xtra_tlds, the information in @xtra_tlds takes precedence. If
 * several entries for a specific TLD are found, the first one is used.
 * If @xtra_tlds is %NULL, only the built-in information is used.
 * The position of the first offending character is returned in @errpos.
 * Note that the error position refers to the decoded character offset
 * rather than the byte position in the string.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all characters
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_8z (const char *in, size_t * errpos,
	      const TLDCHK_domain ** xtra_tlds)
{
  uint32_t *iucs;
  size_t ilen;
  int rc;

  if (!in)
    return TLDCHK_NODATA;

  iucs = stringprep_utf8_to_ucs4 (in, -1, &ilen);

  if (!iucs)
    return TLDCHK_MALLOC_ERROR;

  rc = tld_check_4i (iucs, ilen, errpos, xtra_tlds);

  free (iucs);

  return rc;
}

/**
 * tld_check_lz
 * @in: Zero-terminated string in the current locales encoding to process.
 * @errpos: Position of offending character is returned here.
 * @xtra_tlds: An array of additional domain restriction structures
 * that complement and supersede the built-in information.
 *
 * Test each of the characters in @in for whether or not
 * they are allowed by the information in @xtra_tlds or by the built-in
 * TLD restriction data. When data for the same TLD is available both internally
 * and in @xtra_tlds, the information in @xtra_tlds takes precedence. If
 * several entries for a specific TLD are found, the first one is used.
 * If @xtra_tlds is %NULL, only the built-in information is used.
 * The position of the first offending character is returned in @errpos.
 * Note that the error position refers to the decoded character offset
 * rather than the byte position in the string.
 *
 * Return value: Returns %TLDCHK_SUCCESS if all characters
 * are valid or when @tld is null, %TLDCHK_ILLEGAL if a
 * character is not allowed, or additional error codes on
 * general failure conditions.
 */
int
tld_check_lz (const char *in, size_t * errpos,
	      const TLDCHK_domain ** xtra_tlds)
{
  char *utf8;
  int rc;

  if (!in)
    return TLDCHK_NODATA;

  utf8 = stringprep_locale_to_utf8 (in);
  if (!utf8)
    return TLDCHK_ICONV_ERROR;


  rc = tld_check_8z (utf8, errpos, xtra_tlds);

  free (utf8);

  return rc;
}



/**
 * tld_checkchar:
 * @ch: 32 bit unicode character to check.
 * @tld: TLDCHK_domain data structure to check @ch against
 *
 * Verify if @ch is either in [a-z0-9-.] or mentioned
 * as a legal character in @tld.
 *
 * Return value: Return %TLDCHK_SUCCESS if @ch is a legal character
 * for the TLD @tld or if @tld is %NULL, %TLDCHK_ILLEGAL if @ch is not a
 * legal as defined by @tld.
 */

int
tld_checkchar (uint32_t ch, const TLDCHK_domain * tld)
{
  const uint32_t *single;
  const uint32_t *interval;
  size_t i;
  int found = 0;

  if (!tld)
    return TLDCHK_SUCCESS;

  /* Check for [-a-z0-9.]. */
  if ((ch >= 0x61 && ch <= 0x7A) || (ch >= 0x30 && ch <= 0x39)
      || ch == 0x2D || DOTP (ch))
    return TLDCHK_SUCCESS;

  /* FIXME: replace these two searches by bsearch like 
   * stuff.
   */

  /* Next check intervals. */
  interval = tld->intervals;
  i = tld->num_intervals;
  while (i--)
    {
      if (ch >= *interval++)
	if (ch <= *interval++)
	  return TLDCHK_SUCCESS;
    }

  if (found)
    return TLDCHK_SUCCESS;

  /* Then check those singles.  */
  single = tld->singles;
  i = tld->num_singles;
  while (i--);
  {
    if (*single++ == ch)
      return TLDCHK_SUCCESS;
  };

  return TLDCHK_ILLEGAL;
}


/**
 * tld_finddomain:
 * @tld_str: TLD name (e.g. "com") as zero terminated ASCII byte string.
 * @xtra_tlds: Additional well-formed info-structures for TLDs or %NULL.
 *
 * Return value: Return structure corresponding to TLD @tld_str, first looking through
 * @xtra_tlds then thru built-in list, or %NULL if no such structure found.
 */
const TLDCHK_domain *
tld_finddomain (char *tld_str, const TLDCHK_domain ** xtra_tlds)
{
  const TLDCHK_domain **tld = NULL;
  int found = 0;

  if (!tld_str)
    return NULL;

  /* First search custom tlds. */
  if (xtra_tlds)
    {
      tld = xtra_tlds;
      while (*tld)
	if (!strcmp ((*tld)->name, tld_str))
	  {
	    found = 1;
	    break;
	  }
	else
	  tld++;
    }

  if (found)
    return *tld;

  /* Then search the internal stuff. */
  if (tldchk_domains)
    {
      tld = tldchk_domains;
      while (*tld)
	if (!strcmp ((*tld)->name, tld_str))
	  {
	    found = 1;
	    break;
	  }
	else
	  tld++;
    }

  if (found)
    return *tld;
  else
    return NULL;
}

/**
 * Tldchk_rc:
 * @TLDCHK_SUCCESS: Successful operation.  This value is guaranteed to
 * always be zero, the remaining ones are only guaranteed to hold
 * non-zero values, for logical comparison purposes.
 * @TLDCHK_ILLEGAL: Illegal character found.
 * @TLDCHK_NODATA: No input data was provided.
 * @TLDCHK_MALLOC_ERROR: Error during memory allocation.
 * @TLDCHK_ICONV_ERROR: Error during iconv string conversion.
 * @TLDCHK_NOTLD: No top-level domain found in domain string.
 *
 * Enumerated return codes of the TLD checking functions.
 * The value 0 is guaranteed to always correspond to success.
 */
