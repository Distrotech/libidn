/* idna.c	Convert to or from IDN strings.
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

#include "internal.h"

/**
 * idna_to_ascii
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output zero terminated string that must have room for at
 *       least 63 characters plus the terminating zero.
 * @allowunassigned: boolean value as per IDNA specification.
 * @usestd3asciirules: boolean value as per IDNA specification.
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
  size_t len, outlen;
  unsigned long *src;
  int rc;

  src = malloc (sizeof (in[0]) * inlen + 1);
  if (src == NULL)
    return IDNA_MALLOC_ERROR;

  memcpy (src, in, sizeof (in[0]) * inlen);
  src[inlen] = 0;

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
    for (i = 0; src[i]; i++)
      if (src[i] > 0x7F)
	inasciirange = 0;
    if (inasciirange)
      goto step3;
  }

  /*
   * 2. Perform the steps specified in [NAMEPREP] and fail if there is
   * an error. The AllowUnassigned flag is used in [NAMEPREP].
   */

  {
    char *p;

    p = stringprep_ucs4_to_utf8 (src, inlen, NULL, NULL);
    if (p == NULL)
      return IDNA_MALLOC_ERROR;

    p = realloc (p, BUFSIZ);
    if (p == NULL)
      return IDNA_MALLOC_ERROR;

    if (allowunassigned)
      rc = stringprep_nameprep (p, BUFSIZ);
    else
      rc = stringprep_nameprep_no_unassigned (p, BUFSIZ);

    if (rc != STRINGPREP_OK)
      return IDNA_STRINGPREP_ERROR;

    free (src);

    src = stringprep_utf8_to_ucs4 (p, -1, NULL);
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

  if (usestd3asciirules)
    {
      size_t i;

      for (i = 0; src[i]; i++)
	if (src[i] <= 0x2C || src[i] == 0x2E || src[i] == 0x2F ||
	    (src[i] >= 0x3A && src[i] <= 0x40) ||
	    (src[i] >= 0x5B && src[i] <= 0x60) ||
	    (src[i] >= 0x7B && src[i] <= 0x7F))
	  return IDNA_CONTAINS_LDH;

      if (src[0] == 0x002D || (i > 0 && src[i - 1] == 0x002D))
	return IDNA_CONTAINS_MINUS;
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
    /* XXX */
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
idna_to_unicode_internal (const unsigned long *in, size_t inlen,
			  unsigned long *out, size_t * outlen,
			  int allowunassigned, int usestd3asciirules,
			  char *utf8in, size_t utf8len)
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
    for (i = 0; in[i]; i++)
      if (in[i] > 0x7F)
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

  if (allowunassigned)
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

  rc = punycode_decode (strlen(utf8in), utf8in, outlen, out, NULL);
  if (rc != PUNYCODE_SUCCESS)
    return IDNA_PUNYCODE_ERROR;

  /* 6. Apply ToASCII.
   */

  rc = idna_to_ascii (out, *outlen, tmpout,
		      allowunassigned, usestd3asciirules);
  if (rc != IDNA_SUCCESS)
    return rc;

  /* 7. Verify that the result of step 6 matches the saved copy from
   * step 3, using a case-insensitive ASCII comparison.
   */

  if (strcasecmp(utf8in, tmpout + strlen(IDNA_ACE_PREFIX)) != 0)
    return IDNA_ROUNDTRIP_VERIFY_ERROR;

  /* 8. Return the saved copy from step 5.
   */

  return IDNA_SUCCESS;
}

/**
 * idna_to_unicode
 * @in: input array with unicode code points.
 * @inlen: length of input array with unicode code points.
 * @out: output array with unicode code points.
 * @outlen: on input, maximum size of output array with unicode code points,
 *          on exit, actual size of output array with unicode code points.
 * @allowunassigned: boolean value as per IDNA specification.
 * @usestd3asciirules: boolean value as per IDNA specification.
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
  int outlensave = *outlen;
  char *p;

  p = stringprep_ucs4_to_utf8 (in, inlen, NULL, NULL);
  if (p == NULL)
    return IDNA_MALLOC_ERROR;

  p = realloc (p, BUFSIZ);
  if (p == NULL)
    return IDNA_MALLOC_ERROR;

  rc = idna_to_unicode_internal (in, inlen, out, outlen,
				 allowunassigned, usestd3asciirules,
				 p, BUFSIZ);
  if (rc != IDNA_SUCCESS)
    {
      memcpy(out, in,
	     sizeof (in[0]) * (inlen < outlensave ? inlen : outlensave));
      *outlen = inlen;
    }

  free(p);

  return rc;
}

/**
 * idna_ucs4_to_ace:
 * @input: zero terminated input Unicode string.
 * @output: pointer to newly allocated output string.
 *
 * Convert UCS-4 domain name to ASCII string.  The AllowUnassigned
 * flag is false and std3asciirules flag is false.  The domain name
 * may contain several labels, separated by dots.  The output buffer
 * must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_ucs4_to_ace (const unsigned long *input, char **output)
{
  const unsigned long *start = input;
  const unsigned long *end = input;
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
	     *end != 0x3002 &&
	     *end != 0xFF0E &&
	     *end != 0xFF61; end++)
	;

      rc = idna_to_ascii (start, end-start, buf, 0, 0);
      if (rc != IDNA_SUCCESS)
	return rc;

      if (out)
	{
	  out = realloc(out, strlen(out) + 1 + strlen(buf) + 1);
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	  strcat(out, ".");
	  strcat(out, buf);
	}
      else
	{
	  out = strdup(buf);
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	}

      start = end+1;
    }
  while (*end);

  *output = out;

  return IDNA_SUCCESS;
}

/**
 * idna_utf8_to_ace:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 *
 * Convert UTF-8 domain name to ASCII string.  The AllowUnassigned
 * flag is false and std3asciirules flag is false.  The domain name
 * may contain several labels, separated by dots.  The output buffer
 * must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8_to_ace (const char *input, char **output)
{
  unsigned long *ucs4;
  int ucs4len;
  int rc;

  ucs4 = stringprep_utf8_to_ucs4 (input, -1, &ucs4len);
  rc = idna_ucs4_to_ace(ucs4, output);
  free(ucs4);

  return rc;
}

/**
 * idna_locale_to_ace:
 * @input: zero terminated input UTF-8 string.
 * @output: pointer to newly allocated output string.
 *
 * Convert domain name in the locale's encoding to ASCII string.  The
 * AllowUnassigned flag is false and std3asciirules flag is false.
 * The domain name may contain several labels, separated by dots.  The
 * output buffer must be deallocated by the caller.
 *
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_locale_to_ace (const char *input, char **output)
{
  char *utf8;
  int rc;

  utf8 = stringprep_locale_to_utf8 (input);
  rc = idna_utf8_to_ace(utf8, output);
  free(utf8);

  return rc;
}

/* Transforms an (possibly) ACE domain name into Unicode. Every label
   which is not ACE will be output inchanged so you can safely use
   this routine.  The output will be encoded in UTF-8. The output must
   be allocated and freed by you. The returned int is a status
   code. */

/**
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
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_ucs4ace_to_ucs4 (const unsigned long *input, unsigned long **output)
{
  const unsigned long *start = input;
  const unsigned long *end = input;
  unsigned long *buf;
  size_t buflen;
  unsigned long *out = NULL;
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
	     *end != 0x3002 &&
	     *end != 0xFF0E &&
	     *end != 0xFF61; end++)
	;

      buflen = end-start;
      buf = malloc(sizeof(buf[0]) * buflen);
      if (!buf)
	return IDNA_MALLOC_ERROR;

      rc = idna_to_unicode (start, end-start, buf, &buflen, 0, 0);
      /* don't check rc as per specification! */

      if (out)
	{
	  out = realloc(out, sizeof(out[0]) * (outlen + 1 + buflen + 1));
	  if (!out)
	    return IDNA_MALLOC_ERROR;
	  out[outlen++] = 0x002E; /* '.' (full stop) */
	  memcpy(out + outlen, buf, sizeof(buf[0]) * buflen);
	  outlen += buflen;
	  out[outlen] = 0x0;
	  free(buf);
	}
      else
	{
	  out = buf;
	  outlen = buflen;
	}

      start = end+1;
    }
  while (*end);

  *output = out;

  return IDNA_SUCCESS;
}

/**
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
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_ucs4 (const char *input, unsigned long **output)
{
  unsigned long *ucs4;
  int ucs4len;
  int rc;

  ucs4 = stringprep_utf8_to_ucs4 (input, -1, &ucs4len);
  rc = idna_ucs4ace_to_ucs4(ucs4, output);
  free(ucs4);

  return rc;
}

/**
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
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_utf8 (const char *input, char **output)
{
  unsigned long *ucs4;
  int rc;

  rc = idna_utf8ace_to_ucs4(input, &ucs4);
  *output = stringprep_ucs4_to_utf8(ucs4, -1, NULL, NULL);
  free(ucs4);

  return rc;
}

/**
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
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_utf8ace_to_locale (const char *input, char **output)
{
  char *utf8;
  int rc;

  rc = idna_utf8ace_to_utf8(input, &utf8);
  *output = stringprep_utf8_to_locale(utf8);
  free(utf8);

  return rc;
}

/**
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
 * Return value: Returns IDNA_SUCCESS on success, or error code.
 **/
int
idna_localeace_to_locale (const char *input, char **output)
{
  char *utf8;
  int rc;

  utf8 = stringprep_locale_to_utf8(input);
  rc = idna_utf8ace_to_locale(utf8, output);
  free(utf8);

  return rc;
}
