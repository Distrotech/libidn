/* idna.c	Convert to or from IDN strings.
 * Copyright (C) 2002, 2003  Simon Josefsson
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

#include "internal.h"

/* Core functions */

/**
 * idna_to_ascii_4i
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output zero terminated string that must have room for at
 *       least 63 characters plus the terminating zero.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * The ToASCII operation takes a sequence of Unicode code points that make
 * up one label and transforms it into a sequence of code points in the
 * ASCII range (0..7F). If ToASCII succeeds, the original sequence and the
 * resulting sequence are equivalent labels.
 *
 * It is important to note that the ToASCII operation can fail. ToASCII
 * fails if any step of it fails. If any step of the ToASCII operation
 * fails on any label in a domain name, that domain name MUST NOT be used
 * as an internationalized domain name. The method for deadling with this
 * failure is application-specific.
 *
 * The inputs to ToASCII are a sequence of code points, the AllowUnassigned
 * flag, and the UseSTD3ASCIIRules flag. The output of ToASCII is either a
 * sequence of ASCII code points or a failure condition.
 *
 * ToASCII never alters a sequence of code points that are all in the ASCII
 * range to begin with (although it could fail). Applying the ToASCII
 * operation multiple times has exactly the same effect as applying it just
 * once.
 *
 * Return value: Returns 0 on success, or an error code.
 */
int
idna_to_ascii_4i (const uint32_t * in, size_t inlen, char *out, int flags)
{
  size_t len, outlen;
  uint32_t *src;		/* XXX don't need to copy data? */
  int rc;


  /*
   * ToASCII consists of the following steps:
   *
   * 1. If all code points in the sequence are in the ASCII range (0..7F)
   * then skip to step 3.
   */

  {
    size_t i;
    int inasciirange;

    inasciirange = 1;
    for (i = 0; i < inlen; i++)
      if (in[i] > 0x7F)
	inasciirange = 0;
    if (inasciirange)
      {
	src = malloc (sizeof (in[0]) * (inlen + 1));
	if (src == NULL)
	  return IDNA_MALLOC_ERROR;

	memcpy (src, in, sizeof (in[0]) * inlen);
	src[inlen] = 0;

	goto step3;
      }
  }

  /*
   * 2. Perform the steps specified in [NAMEPREP] and fail if there is
   * an error. The AllowUnassigned flag is used in [NAMEPREP].
   */

  {
    char *p;

    p = stringprep_ucs4_to_utf8 (in, inlen, NULL, NULL);
    if (p == NULL)
      return IDNA_MALLOC_ERROR;

    len = strlen (p);
    do
      {
	len = 2 * len + 10;	/* XXX better guess? */
	p = realloc (p, len);
	if (p == NULL)
	  return IDNA_MALLOC_ERROR;

	if (flags & IDNA_ALLOW_UNASSIGNED)
	  rc = stringprep_nameprep (p, len);
	else
	  rc = stringprep_nameprep_no_unassigned (p, len);
      }
    while (rc == STRINGPREP_TOO_SMALL_BUFFER);

    if (rc != STRINGPREP_OK)
      {
	free (p);
	return IDNA_STRINGPREP_ERROR;
      }

    src = stringprep_utf8_to_ucs4 (p, -1, NULL);

    free (p);
  }

step3:
  /*
   * 3. If the UseSTD3ASCIIRules flag is set, then perform these checks:
   *
   * (a) Verify the absence of non-LDH ASCII code points; that is,
   * the absence of 0..2C, 2E..2F, 3A..40, 5B..60, and 7B..7F.
   *
   * (b) Verify the absence of leading and trailing hyphen-minus;
   * that is, the absence of U+002D at the beginning and end of
   * the sequence.
   */

  if (flags & IDNA_USE_STD3_ASCII_RULES)
    {
      size_t i;

      for (i = 0; src[i]; i++)
	if (src[i] <= 0x2C || src[i] == 0x2E || src[i] == 0x2F ||
	    (src[i] >= 0x3A && src[i] <= 0x40) ||
	    (src[i] >= 0x5B && src[i] <= 0x60) ||
	    (src[i] >= 0x7B && src[i] <= 0x7F))
	  {
	    free (src);
	    return IDNA_CONTAINS_LDH;
	  }

      if (src[0] == 0x002D || (i > 0 && src[i - 1] == 0x002D))
	{
	  free (src);
	  return IDNA_CONTAINS_MINUS;
	}
    }

  /*
   * 4. If all code points in the sequence are in the ASCII range
   * (0..7F), then skip to step 8.
   */

  {
    size_t i;
    int inasciirange;

    inasciirange = 1;
    for (i = 0; src[i]; i++)
      {
	if (src[i] > 0x7F)
	  inasciirange = 0;
	/* copy string to output buffer if we are about to skip to step8 */
	if (i < 64)
	  out[i] = src[i];
      }
    if (i < 64)
      out[i] = '\0';
    if (inasciirange)
      goto step8;
  }

  /*
   * 5. Verify that the sequence does NOT begin with the ACE prefix.
   *
   */

  {
    size_t i;
    int match;

    match = 1;
    for (i = 0; match && i < strlen (IDNA_ACE_PREFIX); i++)
      if (((uint32_t) IDNA_ACE_PREFIX[i] & 0xFF) != src[i])
	match = 0;
    if (match)
      {
	free (src);
	return IDNA_CONTAINS_ACE_PREFIX;
      }
  }

  /*
   * 6. Encode the sequence using the encoding algorithm in [PUNYCODE]
   * and fail if there is an error.
   */
  for (len = 0; src[len]; len++)
    ;
  src[len] = '\0';
  outlen = 63 - strlen (IDNA_ACE_PREFIX);
  rc = punycode_encode (len, src, NULL,
			&outlen, &out[strlen (IDNA_ACE_PREFIX)]);
  free (src);
  if (rc != PUNYCODE_SUCCESS)
    return IDNA_PUNYCODE_ERROR;
  out[strlen (IDNA_ACE_PREFIX) + outlen] = '\0';

  /*
   * 7. Prepend the ACE prefix.
   */

  memcpy (out, IDNA_ACE_PREFIX, strlen (IDNA_ACE_PREFIX));

  /*
   * 8. Verify that the number of code points is in the range 1 to 63
   * inclusive.
   */

step8:
  if (strlen (out) < 1 || strlen (out) > 63)
    return IDNA_INVALID_LENGTH;

  return IDNA_SUCCESS;
}

static int
idna_to_unicode_internal (char *utf8in, size_t utf8len,
			  uint32_t * out, size_t * outlen, int flags)
{
  int rc;
  char tmpout[64];

  /*
   * 1. If all code points in the sequence are in the ASCII range (0..7F)
   * then skip to step 3.
   */

  {
    size_t i;
    int inasciirange;

    inasciirange = 1;
    for (i = 0; utf8in[i]; i++)
      if (utf8in[i] & ~0x7F)
	inasciirange = 0;
    if (inasciirange)
      goto step3;
  }

  /*
   * 2. Perform the steps specified in [NAMEPREP] and fail if there is an
   * error. (If step 3 of ToASCII is also performed here, it will not
   * affect the overall behavior of ToUnicode, but it is not
   * necessary.) The AllowUnassigned flag is used in [NAMEPREP].
   */

  if (flags & IDNA_ALLOW_UNASSIGNED)
    rc = stringprep_nameprep (utf8in, utf8len);
  else
    rc = stringprep_nameprep_no_unassigned (utf8in, utf8len);

  if (rc != STRINGPREP_OK)
    return IDNA_STRINGPREP_ERROR;

  /* 3. Verify that the sequence begins with the ACE prefix, and save a
   * copy of the sequence.
   */

step3:
  if (memcmp (IDNA_ACE_PREFIX, utf8in, strlen (IDNA_ACE_PREFIX)) != 0)
    return IDNA_NO_ACE_PREFIX;

  /* 4. Remove the ACE prefix.
   */

  memmove (utf8in, &utf8in[strlen (IDNA_ACE_PREFIX)],
	   strlen (utf8in) - strlen (IDNA_ACE_PREFIX) + 1);

  /* 5. Decode the sequence using the decoding algorithm in [PUNYCODE]
   * and fail if there is an error. Save a copy of the result of
   * this step.
   */

  (*outlen)--;			/* reserve one for the zero */

  rc = punycode_decode (strlen (utf8in), utf8in, outlen, out, NULL);
  if (rc != PUNYCODE_SUCCESS)
    return IDNA_PUNYCODE_ERROR;

  out[*outlen] = 0;		/* add zero */

  /* 6. Apply ToASCII.
   */

  rc = idna_to_ascii_4i (out, *outlen, tmpout, flags);
  if (rc != IDNA_SUCCESS)
    return rc;

  /* 7. Verify that the result of step 6 matches the saved copy from
   * step 3, using a case-insensitive ASCII comparison.
   */

  if (strcasecmp (utf8in, tmpout + strlen (IDNA_ACE_PREFIX)) != 0)
    return IDNA_ROUNDTRIP_VERIFY_ERROR;

  /* 8. Return the saved copy from step 5.
   */

  return IDNA_SUCCESS;
}

/**
 * idna_to_unicode_44i
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output array with unicode code points.
 * @outlen: on input, maximum size of output array with unicode code points,
 *          on exit, actual size of output array with unicode code points.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * The ToUnicode operation takes a sequence of Unicode code points
 * that make up one label and returns a sequence of Unicode code
 * points. If the input sequence is a label in ACE form, then the
 * result is an equivalent internationalized label that is not in ACE
 * form, otherwise the original sequence is returned unaltered.
 *
 * ToUnicode never fails. If any step fails, then the original input
 * sequence is returned immediately in that step.
 *
 * The ToUnicode output never contains more code points than its
 * input.  Note that the number of octets needed to represent a
 * sequence of code points depends on the particular character
 * encoding used.
 *
 * The inputs to ToUnicode are a sequence of code points, the
 * AllowUnassigned flag, and the UseSTD3ASCIIRules flag. The output of
 * ToUnicode is always a sequence of Unicode code points.
 *
 * Return value: Returns error condition, but it must only be used for
 *               debugging purposes.  The output buffer is always
 *               guaranteed to contain the correct data according to
 *               the specification (sans malloc induced errors).  NB!
 *               This means that you normally ignore the return code
 *               from this function, as checking it means breaking the
 *               standard.
 */
int
idna_to_unicode_44i (const uint32_t * in, size_t inlen,
		     uint32_t * out, size_t * outlen, int flags)
{
  int rc;
  size_t outlensave = *outlen;
  char *p;

  p = stringprep_ucs4_to_utf8 (in, inlen, NULL, NULL);
  if (p == NULL)
    return IDNA_MALLOC_ERROR;

  p = realloc (p, BUFSIZ);
  if (p == NULL)
    return IDNA_MALLOC_ERROR;

  rc = idna_to_unicode_internal (p, BUFSIZ, out, outlen, flags);
  if (rc != IDNA_SUCCESS)
    {
      memcpy (out, in, sizeof (in[0]) * (inlen < outlensave ?
					 inlen : outlensave));
      *outlen = inlen;
    }

  free (p);

  return rc;
}

/* Wrappers that handle several labels */

/**
 * idna_to_ascii_4z:
 * @input: zero terminated input Unicode string.
 * @output: pointer to newly allocated output string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert UCS-4 domain name to ASCII string.  The domain name may
 * contain several labels, separated by dots.  The output buffer must
 * be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_4z (const uint32_t * input, char **output, int flags)
{
  const uint32_t *start = input;
  const uint32_t *end = input;
  char buf[64];
  char *out = NULL;
  int rc;

  *output = NULL;

  do
    {
      end = start;

      /* 1) Whenever dots are used as label separators, the following
         characters MUST be recognized as dots: U+002E (full stop),
         U+3002 (ideographic full stop), U+FF0E (fullwidth full stop),
         U+FF61 (halfwidth ideographic full stop). */
      for (; *end &&
	   *end != 0x002E &&
	   *end != 0x3002 && *end != 0xFF0E && *end != 0xFF61; end++)
	;

      /* Handle empty trailing labels. The RFC is not clear on this,
	 the text that mandate this behaviour inside a parenthesis in
	 the terminology section. */
      if (end == start && *end == '\0')
	{
	  strcpy(buf, out ? "" : ".");
	}
      else
	{
	  rc = idna_to_ascii_4i (start, end - start, buf, flags);
	  if (rc != IDNA_SUCCESS)
	    return rc;
	}

      if (out)
	{
	  out = realloc (out, strlen (out) + 1 + strlen (buf) + 1);
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	  strcat (out, ".");
	  strcat (out, buf);
	}
      else
	{
	  out = (char *) strdup (buf);
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	}

      start = end + 1;
    }
  while (*end);

  *output = out;

  return IDNA_SUCCESS;
}

/**
 * idna_to_ascii_8z:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert UTF-8 domain name to ASCII string.  The domain name may
 * contain several labels, separated by dots.  The output buffer must
 * be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_8z (const char *input, char **output, int flags)
{
  uint32_t *ucs4;
  size_t ucs4len;
  int rc;

  ucs4 = stringprep_utf8_to_ucs4 (input, -1, &ucs4len);
  if (!ucs4)
    return IDNA_ICONV_ERROR;

  rc = idna_to_ascii_4z (ucs4, output, flags);

  free (ucs4);

  return rc;

}

/**
 * idna_to_ascii_lz:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert domain name in the locale's encoding to ASCII string.  The
 * domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_lz (const char *input, char **output, int flags)
{
  char *utf8;
  int rc;

  utf8 = stringprep_locale_to_utf8 (input);
  if (!utf8)
    return IDNA_ICONV_ERROR;

  rc = idna_to_ascii_8z (utf8, output, flags);

  free (utf8);

  return rc;
}

/**
 * idna_to_unicode_4z4z:
 * @input: zero-terminated Unicode string.
 * @output: pointer to newly allocated output Unicode string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert possibly ACE encoded domain name in UCS-4 format into a
 * UCS-4 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_4z4z (const uint32_t * input, uint32_t ** output, int flags)
{
  const uint32_t *start = input;
  const uint32_t *end = input;
  uint32_t *buf;
  size_t buflen;
  uint32_t *out = NULL;
  size_t outlen = 0;
  int rc;

  *output = NULL;

  do
    {
      end = start;

      /* 1) Whenever dots are used as label separators, the following
         characters MUST be recognized as dots: U+002E (full stop),
         U+3002 (ideographic full stop), U+FF0E (fullwidth full stop),
         U+FF61 (halfwidth ideographic full stop). */
      for (; *end &&
	   *end != 0x002E &&
	   *end != 0x3002 && *end != 0xFF0E && *end != 0xFF61; end++)
	;

      buflen = end - start;
      buf = malloc (sizeof (buf[0]) * (buflen + 1));
      if (!buf)
	return IDNA_MALLOC_ERROR;

      rc = idna_to_unicode_44i (start, end - start, buf, &buflen, flags);
      /* don't check rc as per specification! */

      if (out)
	{
	  out = realloc (out, sizeof (out[0]) * (outlen + 1 + buflen + 1));
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	  out[outlen++] = 0x002E;	/* '.' (full stop) */
	  memcpy (out + outlen, buf, sizeof (buf[0]) * buflen);
	  outlen += buflen;
	  out[outlen] = 0x0;
	  free (buf);
	}
      else
	{
	  out = buf;
	  outlen = buflen;
	  out[outlen] = 0x0;
	}

      start = end + 1;
    }
  while (*end);

  *output = out;

  return IDNA_SUCCESS;
}

/**
 * idna_to_unicode_8z4z:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output Unicode string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UCS-4 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_8z4z (const char *input, uint32_t ** output, int flags)
{
  uint32_t *ucs4;
  size_t ucs4len;
  int rc;

  ucs4 = stringprep_utf8_to_ucs4 (input, -1, &ucs4len);
  if (!ucs4)
    return IDNA_ICONV_ERROR;

  rc = idna_to_unicode_4z4z (ucs4, output, flags);
  free (ucs4);

  return rc;
}

/**
 * idna_to_unicode_8z8z:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output UTF-8 string.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UTF-8 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_8z8z (const char *input, char **output, int flags)
{
  uint32_t *ucs4;
  int rc;

  rc = idna_to_unicode_8z4z (input, &ucs4, flags);
  *output = stringprep_ucs4_to_utf8 (ucs4, -1, NULL, NULL);
  free (ucs4);

  if (!*output)
    return IDNA_ICONV_ERROR;

  return rc;
}

/**
 * idna_to_unicode_8zlz:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * string encoded in the current locale's character set.  The domain
 * name may contain several labels, separated by dots.  The output
 * buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_8zlz (const char *input, char **output, int flags)
{
  char *utf8;
  int rc;

  rc = idna_to_unicode_8z8z (input, &utf8, flags);
  *output = stringprep_utf8_to_locale (utf8);
  free (utf8);

  if (!*output)
    return IDNA_ICONV_ERROR;

  return rc;
}

/**
 * idna_to_unicode_lzlz:
 * @input: zero-terminated string encoded in the current locale's
 *   character set.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 * @flags: IDNA flags, e.g. IDNA_ALLOW_UNASSIGNED or IDNA_USE_STD3_ASCII_RULES.
 *
 * Convert possibly ACE encoded domain name in the locale's character
 * set into a string encoded in the current locale's character set.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_lzlz (const char *input, char **output, int flags)
{
  char *utf8;
  int rc;

  utf8 = stringprep_locale_to_utf8 (input);
  if (!utf8)
    return IDNA_ICONV_ERROR;

  rc = idna_to_unicode_8zlz (utf8, output, flags);
  free (utf8);

  return rc;
}


/* Deprecated interfaces */

/*
 * idna_to_ascii
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output zero terminated string that must have room for at
 *       least 63 characters plus the terminating zero.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * The ToASCII operation takes a sequence of Unicode code points that make
 * up one label and transforms it into a sequence of code points in the
 * ASCII range (0..7F). If ToASCII succeeds, the original sequence and the
 * resulting sequence are equivalent labels.
 *
 * It is important to note that the ToASCII operation can fail. ToASCII
 * fails if any step of it fails. If any step of the ToASCII operation
 * fails on any label in a domain name, that domain name MUST NOT be used
 * as an internationalized domain name. The method for deadling with this
 * failure is application-specific.
 *
 * The inputs to ToASCII are a sequence of code points, the AllowUnassigned
 * flag, and the UseSTD3ASCIIRules flag. The output of ToASCII is either a
 * sequence of ASCII code points or a failure condition.
 *
 * ToASCII never alters a sequence of code points that are all in the ASCII
 * range to begin with (although it could fail). Applying the ToASCII
 * operation multiple times has exactly the same effect as applying it just
 * once.
 *
 * Return value: Returns 0 on success, or an error code.
 */
int
idna_to_ascii (const unsigned long *in, size_t inlen,
	       char *out, int allowunassigned, int usestd3asciirules)
{
  int rc;
  int flags = 0;
  uint32_t *tmp;
  size_t i;

  tmp = malloc (sizeof (tmp[0]) * inlen);
  if (!tmp)
    return IDNA_MALLOC_ERROR;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  for (i = 0; i < inlen; i++)
    tmp[i] = in[i];
  rc = idna_to_ascii_4i (tmp, inlen, out, flags);
  free (tmp);

  return rc;
}

/*
 * idna_to_unicode
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output array with unicode code points.
 * @outlen: on input, maximum size of output array with unicode code points,
 *          on exit, actual size of output array with unicode code points.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * The ToUnicode operation takes a sequence of Unicode code points
 * that make up one label and returns a sequence of Unicode code
 * points. If the input sequence is a label in ACE form, then the
 * result is an equivalent internationalized label that is not in ACE
 * form, otherwise the original sequence is returned unaltered.
 *
 * ToUnicode never fails. If any step fails, then the original input
 * sequence is returned immediately in that step.
 *
 * The ToUnicode output never contains more code points than its
 * input.  Note that the number of octets needed to represent a
 * sequence of code points depends on the particular character
 * encoding used.
 *
 * The inputs to ToUnicode are a sequence of code points, the
 * AllowUnassigned flag, and the UseSTD3ASCIIRules flag. The output of
 * ToUnicode is always a sequence of Unicode code points.
 *
 * Return value: Returns error condition, but it must only be used for
 *               debugging purposes.  The output buffer is always
 *               guaranteed to contain the correct data according to
 *               the specification (sans malloc induced errors).  NB!
 *               This means that you normally ignore the return code
 *               from this function, as checking it means breaking the
 *               standard.
 */
int
idna_to_unicode (const unsigned long *in, size_t inlen,
		 unsigned long *out, size_t * outlen,
		 int allowunassigned, int usestd3asciirules)
{
  int rc;
  int flags = 0;
  uint32_t *tmpin;
  uint32_t *tmpout;
  size_t i;

  tmpin = malloc (sizeof (tmpin[0]) * inlen);
  if (!tmpin)
    return IDNA_MALLOC_ERROR;
  tmpout = malloc (sizeof (tmpout[0]) * *outlen);
  if (!tmpout)
    return IDNA_MALLOC_ERROR;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  for (i = 0; i < inlen; i++)
    tmpin[i] = in[i];
  rc = idna_to_unicode_44i (tmpin, inlen, tmpout, outlen, flags);
  free (tmpin);

  for (i = 0; i < *outlen; i++)
    out[i] = tmpout[i];

  free (tmpout);

  return rc;

}

/*
 * idna_to_ascii_from_ucs4:
 * @input: zero terminated input Unicode string.
 * @output: pointer to newly allocated output string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert UCS-4 domain name to ASCII string.  The domain name may
 * contain several labels, separated by dots.  The output buffer must
 * be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_from_ucs4 (const unsigned long *input, char **output,
			 int allowunassigned, int usestd3asciirules)
{
  size_t inlen;
  int rc;
  int flags = 0;
  uint32_t *tmp;
  size_t i;

  for (inlen = 0; input[inlen]; inlen++)
    ;
  tmp = malloc (sizeof (tmp[0]) * (inlen + 1));
  if (!tmp)
    return IDNA_MALLOC_ERROR;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  for (i = 0; i < inlen; i++)
    tmp[i] = input[i];
  tmp[i] = 0;
  rc = idna_to_ascii_4z (tmp, output, flags);
  free (tmp);

  return rc;
}

/*
 * idna_to_ascii_from_utf8:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert UTF-8 domain name to ASCII string.  The domain name may
 * contain several labels, separated by dots.  The output buffer must
 * be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_from_utf8 (const char *input, char **output,
			 int allowunassigned, int usestd3asciirules)
{
  int rc;
  int flags = 0;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_ascii_8z (input, output, flags);

  return rc;
}

/*
 * idna_to_ascii_from_locale:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert domain name in the locale's encoding to ASCII string.  The
 * domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_ascii_from_locale (const char *input, char **output,
			   int allowunassigned, int usestd3asciirules)
{
  int rc;
  int flags = 0;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_ascii_lz (input, output, flags);

  return rc;
}

/*
 * idna_to_unicode_ucs4_from_ucs4:
 * @input: zero-terminated Unicode string.
 * @output: pointer to newly allocated output Unicode string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert possibly ACE encoded domain name in UCS-4 format into a
 * UCS-4 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_ucs4_from_ucs4 (const unsigned long *input,
				unsigned long **output,
				int allowunassigned, int usestd3asciirules)
{
  size_t inlen, tmpoutlen;
  int rc;
  int flags = 0;
  uint32_t *tmpin;
  uint32_t *tmpout;
  size_t i;

  for (inlen = 0; input[inlen]; inlen++)
    ;
  tmpin = malloc (sizeof (tmpin[0]) * (inlen + 1));
  if (!tmpin)
    return IDNA_MALLOC_ERROR;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  for (i = 0; i < inlen; i++)
    tmpin[i] = input[i];
  tmpin[i] = 0;
  rc = idna_to_unicode_4z4z (tmpin, &tmpout, flags);
  free (tmpin);

  for (tmpoutlen = 0; tmpout[tmpoutlen]; tmpoutlen++)
    ;

  *output = malloc (sizeof (output[0]) * (tmpoutlen + 1));
  if (!*output)
    return IDNA_MALLOC_ERROR;

  for (i = 0; i < tmpoutlen; i++)
    (*output)[i] = tmpout[i];
  (*output)[i] = 0;

  return rc;
}

/*
 * idna_to_unicode_ucs4_from_utf8:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output Unicode string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UCS-4 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_ucs4_from_utf8 (const char *input, unsigned long **output,
				int allowunassigned, int usestd3asciirules)
{
  size_t tmpinlen, tmpoutlen;
  int rc;
  int flags = 0;
  uint32_t *tmpin;
  uint32_t *tmpout;
  size_t i;

  tmpin = stringprep_utf8_to_ucs4 (input, -1, &tmpinlen);
  if (!tmpin)
    return IDNA_ICONV_ERROR;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_unicode_4z4z (tmpin, &tmpout, flags);
  free (tmpin);

  for (tmpoutlen = 0; tmpout[tmpoutlen]; tmpoutlen++)
    ;

  *output = malloc (sizeof (output[0]) * (tmpoutlen + 1));
  if (!*output)
    return IDNA_MALLOC_ERROR;

  for (i = 0; i < tmpoutlen; i++)
    (*output)[i] = tmpout[i];
  (*output)[i] = 0;

  return rc;
}

/*
 * idna_to_unicode_utf8_from_utf8:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output UTF-8 string.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UTF-8 string.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_utf8_from_utf8 (const char *input, char **output,
				int allowunassigned, int usestd3asciirules)
{
  int flags = 0;
  int rc;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_unicode_8z8z (input, output, flags);

  return rc;
}

/*
 * idna_to_unicode_locale_from_utf8:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * string encoded in the current locale's character set.  The
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_locale_from_utf8 (const char *input, char **output,
				  int allowunassigned, int usestd3asciirules)
{
  int flags = 0;
  int rc;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_unicode_8zlz (input, output, flags);

  return rc;
}

/*
 * idna_to_unicode_locale_from_locale:
 * @input: zero-terminated string encoded in the current locale's
 *   character set.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 * @allowunassigned: whether to allow unassigned code points.
 * @usestd3asciirules: whether to check input for STD3 compliance.
 *
 * Convert possibly ACE encoded domain name in the locale's character
 * set into a string encoded in the current locale's character set.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_to_unicode_locale_from_locale (const char *input, char **output,
				    int allowunassigned,
				    int usestd3asciirules)
{
  int flags = 0;
  int rc;

  if (allowunassigned)
    flags |= IDNA_ALLOW_UNASSIGNED;
  if (usestd3asciirules)
    flags |= IDNA_USE_STD3_ASCII_RULES;

  rc = idna_to_unicode_lzlz (input, output, flags);

  return rc;
}


/* Deprecated interfaces (even older) */


/*
 * idna_ucs4_to_ace:
 * @input: zero terminated input Unicode string.
 * @output: pointer to newly allocated output string.
 *
 * Convert UCS-4 domain name to ASCII string.  The AllowUnassigned
 * flag is false and std3asciirules flag is false.  The domain name
 * may contain several labels, separated by dots.  The output buffer
 * must be deallocated by the caller.
 *
 * This function is deprecated in favor of idna_to_ascii_from_ucs4()
 * and will be removed in future versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_ucs4_to_ace (const unsigned long *input, char **output)
{
  return idna_to_ascii_from_ucs4 (input, output, 0, 0);
}

/*
 * idna_utf8_to_ace:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 *
 * Convert UTF-8 domain name to ASCII string.  The AllowUnassigned
 * flag is false and std3asciirules flag is false.  The domain name
 * may contain several labels, separated by dots.  The output buffer
 * must be deallocated by the caller.
 *
 * This function is deprecated in favor of idna_to_ascii_from_utf8()
 * and will be removed in future versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8_to_ace (const char *input, char **output)
{
  return idna_to_ascii_from_utf8 (input, output, 0, 0);
}

/*
 * idna_locale_to_ace:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 *
 * Convert domain name in the locale's encoding to ASCII string.  The
 * AllowUnassigned flag is false and std3asciirules flag is false.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * This function is deprecated in favor of idna_to_ascii_from_locale()
 * and will be removed in future versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_locale_to_ace (const char *input, char **output)
{
  return idna_to_ascii_from_locale (input, output, 0, 0);
}

/*
 * idna_ucs4ace_to_ucs4:
 * @input: zero-terminated Unicode string.
 * @output: pointer to newly allocated output Unicode string.
 *
 * Convert possibly ACE encoded domain name in UCS-4 format into a
 * UCS-4 string.  The AllowUnassigned flag is false and std3asciirules
 * flag is false.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * This function is deprecated in favor of
 * idna_to_unicode_ucs4_from_ucs4() and will be removed in future
 * versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_ucs4ace_to_ucs4 (const unsigned long *input, unsigned long **output)
{
  return idna_to_unicode_ucs4_from_ucs4 (input, output, 0, 0);
}

/*
 * idna_utf8ace_to_ucs4:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output Unicode string.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UCS-4 string.  The AllowUnassigned flag is false and std3asciirules
 * flag is false.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * This function is deprecated in favor of
 * idna_to_unicode_ucs4_from_utf8() and will be removed in future
 * versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_ucs4 (const char *input, unsigned long **output)
{
  return idna_to_unicode_ucs4_from_utf8 (input, output, 0, 0);
}

/*
 * idna_utf8ace_to_utf8:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output UTF-8 string.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * UTF-8 string.  The AllowUnassigned flag is false and std3asciirules
 * flag is false.  The domain name may contain several labels,
 * separated by dots.  The output buffer must be deallocated by the
 * caller.
 *
 * This function is deprecated in favor of
 * idna_to_unicode_utf8_from_utf8() and will be removed in future
 * versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_utf8 (const char *input, char **output)
{
  return idna_to_unicode_utf8_from_utf8 (input, output, 0, 0);
}

/*
 * idna_utf8ace_to_locale:
 * @input: zero-terminated UTF-8 string.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 *
 * Convert possibly ACE encoded domain name in UTF-8 format into a
 * string encoded in the current locale's character set.  The
 * AllowUnassigned flag is false and std3asciirules flag is false.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * This function is deprecated in favor of
 * idna_to_unicode_locale_from_utf8() and will be removed in future
 * versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_locale (const char *input, char **output)
{
  return idna_to_unicode_locale_from_utf8 (input, output, 0, 0);
}

/*
 * idna_localeace_to_locale:
 * @input: zero-terminated string encoded in the current locale's
 *   character set.
 * @output: pointer to newly allocated output string encoded in the
 *   current locale's character set.
 *
 * Convert possibly ACE encoded domain name in the locale's character
 * set into a string encoded in the current locale's character set.
 * The AllowUnassigned flag is false and std3asciirules flag is false.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * This function is deprecated in favor of
 * idna_to_unicode_locale_from_locale() and will be removed in future
 * versions.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_localeace_to_locale (const char *input, char **output)
{
  return idna_to_unicode_locale_from_locale (input, output, 0, 0);
}
