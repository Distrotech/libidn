/* obsolete.c	Obsolete bindings for backwards compatibility.
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
enum Idna_rc
idna_to_ascii (const unsigned long *in, size_t inlen,
	       char *out, int allowunassigned, int usestd3asciirules)
{
  enum Idna_rc rc;
  enum Idna_flags flags = 0;
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
enum Idna_rc
idna_to_unicode (const unsigned long *in, size_t inlen,
		 unsigned long *out, size_t * outlen,
		 int allowunassigned, int usestd3asciirules)
{
  enum Idna_rc rc;
  enum Idna_flags flags = 0;
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
enum Idna_rc
idna_to_ascii_from_ucs4 (const unsigned long *input, char **output,
			 int allowunassigned, int usestd3asciirules)
{
  size_t inlen;
  enum Idna_rc rc;
  enum Idna_flags flags = 0;
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
enum Idna_rc
idna_to_ascii_from_utf8 (const char *input, char **output,
			 int allowunassigned, int usestd3asciirules)
{
  enum Idna_rc rc;
  enum Idna_flags flags = 0;

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
enum Idna_rc
idna_to_ascii_from_locale (const char *input, char **output,
			   int allowunassigned, int usestd3asciirules)
{
  enum Idna_rc rc;
  enum Idna_flags flags = 0;

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
enum Idna_rc
idna_to_unicode_ucs4_from_ucs4 (const unsigned long *input,
				unsigned long **output,
				int allowunassigned, int usestd3asciirules)
{
  size_t inlen, tmpoutlen;
  enum Idna_rc rc;
  enum Idna_flags flags = 0;
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
enum Idna_rc
idna_to_unicode_ucs4_from_utf8 (const char *input, unsigned long **output,
				int allowunassigned, int usestd3asciirules)
{
  size_t tmpinlen, tmpoutlen;
  enum Idna_rc rc;
  enum Idna_flags flags = 0;
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
enum Idna_rc
idna_to_unicode_utf8_from_utf8 (const char *input, char **output,
				int allowunassigned, int usestd3asciirules)
{
  enum Idna_flags flags = 0;
  enum Idna_rc rc;

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
enum Idna_rc
idna_to_unicode_locale_from_utf8 (const char *input, char **output,
				  int allowunassigned, int usestd3asciirules)
{
  enum Idna_flags flags = 0;
  enum Idna_rc rc;

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
enum Idna_rc
idna_to_unicode_locale_from_locale (const char *input, char **output,
				    int allowunassigned,
				    int usestd3asciirules)
{
  enum Idna_flags flags = 0;
  enum Idna_rc rc;

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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
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
enum Idna_rc
idna_localeace_to_locale (const char *input, char **output)
{
  return idna_to_unicode_locale_from_locale (input, output, 0, 0);
}
