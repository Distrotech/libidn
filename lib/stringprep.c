/* stringprep.c		Core stringprep implementation.
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

static ssize_t
stringprep_find_character_in_table (uint32_t ucs4,
				    Stringprep_table_element * table)
{
  ssize_t i;

  for (i = 0; table[i].start; i++)
    if (ucs4 >= table[i].start &&
	ucs4 <= (table[i].end ? table[i].end : table[i].start))
      return i;

  return -1;
}

static ssize_t
stringprep_find_string_in_table (uint32_t * ucs4,
				 size_t ucs4len,
				 size_t * tablepos,
				 Stringprep_table_element * table)
{
  size_t j;
  ssize_t pos;

  for (j = 0; j < ucs4len; j++)
    if ((pos = stringprep_find_character_in_table (ucs4[j], table)) != -1)
      {
	if (tablepos)
	  *tablepos = pos;
	return j;
      }

  return -1;
}

static int
stringprep_apply_table_to_string (uint32_t * ucs4,
				  size_t * ucs4len,
				  size_t maxucs4len,
				  Stringprep_table_element * table,
				  const char *tablename)
{
  ssize_t pos;
  size_t i, maplen;

  while ((pos = stringprep_find_string_in_table (ucs4, *ucs4len,
						 &i, table)) != -1)
    {
      for (maplen = STRINGPREP_MAX_MAP_CHARS;
	   maplen > 0 && table[i].map[maplen - 1] == 0; maplen--)
	;

      if (*ucs4len - 1 + maplen >= maxucs4len)
	return STRINGPREP_TOO_SMALL_BUFFER;

      memmove (&ucs4[pos + maplen], &ucs4[pos + 1],
	       sizeof (uint32_t) * (*ucs4len - pos - 1));
      memcpy (&ucs4[pos], table[i].map, sizeof (uint32_t) * maplen);
      *ucs4len = *ucs4len - 1 + maplen;
    }

  return STRINGPREP_OK;
}

#define INVERTED(x) ((x) & ((~0UL) >> 1))
#define UNAPPLICAPLEFLAGS(flags, profileflags) \
  ((!INVERTED(profileflags) && !(profileflags & flags) && profileflags) || \
   ( INVERTED(profileflags) && (profileflags & flags)))

/**
 * stringprep_4i:
 * @ucs4: input/output array with string to prepare.
 * @len: on input, length of input array with Unicode code points,
 *          on exit, length of output array with Unicode code points.
 * @maxucs4len: maximum length of input/output array.
 * @flags: stringprep profile flags, or 0.
 * @profile: pointer to stringprep profile to use.
 *
 * Prepare the input UCS-4 string according to the stringprep profile,
 * and write back the result to the input string.
 *
 * The input is not required to be zero terminated (@ucs4[@len] = 0).
 * The output will not be zero terminated unless @ucs4[@len] = 0.
 * Instead, see stringprep_4zi() if your input is zero terminated or
 * if you want the output to be.
 *
 * Since the stringprep operation can expand the string, @maxucs4len
 * indicate how large the buffer holding the string is.  This function
 * will not read or write to code points outside that size.
 *
 * The @flags are one of Stringprep_profile_flags, or 0.
 *
 * The @profile contain the instructions to perform.  Your application
 * can define new profiles, possibly re-using the generic stringprep
 * tables that always will be part of the library, or use one of the
 * currently supported profiles.
 *
 * Return value: Returns %STRINGPREP_OK iff successful, or an error code.
 **/
int
stringprep_4i (uint32_t *ucs4, size_t *len, size_t maxucs4len,
	       Stringprep_profile_flags flags, Stringprep_profile * profile)
{
  size_t i, j;
  ssize_t k;
  size_t ucs4len = *len;
  int rc;

  for (i = 0; profile[i].operation; i++)
    {
      switch (profile[i].operation)
	{
	case STRINGPREP_NFKC:
	  {
	    uint32_t *q = 0;

	    if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	      break;

	    if (flags & STRINGPREP_NO_NFKC && !profile[i].flags)
	      /* Profile requires NFKC, but callee asked for no NFKC. */
	      return STRINGPREP_FLAG_ERROR;

	    q = stringprep_ucs4_nfkc_normalize (ucs4, ucs4len);
	    if (!q)
	      return STRINGPREP_NFKC_FAILED;

	    for (ucs4len = 0; q[ucs4len]; ucs4len++)
	      ;

	    if (ucs4len >= maxucs4len)
	      {
		free (q);
		return STRINGPREP_TOO_SMALL_BUFFER;
	      }

	    memcpy (ucs4, q, ucs4len * sizeof (ucs4[0]));

	    free (q);
	  }
	  break;

	case STRINGPREP_PROHIBIT_TABLE:
	  k = stringprep_find_string_in_table (ucs4, ucs4len,
					       NULL, profile[i].table);
	  if (k != -1)
	    return STRINGPREP_CONTAINS_PROHIBITED;
	  break;

	case STRINGPREP_UNASSIGNED_TABLE:
	  if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	    break;
	  if (flags & STRINGPREP_NO_UNASSIGNED)
	    {
	      k = stringprep_find_string_in_table
		(ucs4, ucs4len, NULL, profile[i].table);
	      if (k != -1)
		return STRINGPREP_CONTAINS_UNASSIGNED;
	    }
	  break;

	case STRINGPREP_MAP_TABLE:
	  if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	    break;
	  rc = stringprep_apply_table_to_string
	    (ucs4, &ucs4len, maxucs4len, profile[i].table, profile[i].name);
	  if (rc != STRINGPREP_OK)
	    return rc;
	  break;

	case STRINGPREP_BIDI_PROHIBIT_TABLE:
	case STRINGPREP_BIDI_RAL_TABLE:
	case STRINGPREP_BIDI_L_TABLE:
	  break;

	case STRINGPREP_BIDI:
	  {
	    int done_prohibited = 0;
	    int done_ral = 0;
	    int done_l = 0;
	    int contains_ral = -1;
	    int contains_l = -1;

	    for (j = 0; profile[j].operation; j++)
	      if (profile[j].operation == STRINGPREP_BIDI_PROHIBIT_TABLE)
		{
		  done_prohibited = 1;
		  k = stringprep_find_string_in_table (ucs4, ucs4len,
						       NULL,
						       profile[j].table);
		  if (k != -1)
		    return STRINGPREP_BIDI_CONTAINS_PROHIBITED;
		}
	      else if (profile[j].operation == STRINGPREP_BIDI_RAL_TABLE)
		{
		  done_ral = 1;
		  if (stringprep_find_string_in_table
		      (ucs4, ucs4len, NULL, profile[j].table) != -1)
		    contains_ral = j;
		}
	      else if (profile[j].operation == STRINGPREP_BIDI_L_TABLE)
		{
		  done_l = 1;
		  if (stringprep_find_string_in_table
		      (ucs4, ucs4len, NULL, profile[j].table) != -1)
		    contains_l = j;
		}

	    if (!done_prohibited || !done_ral || !done_l)
	      return STRINGPREP_PROFILE_ERROR;

	    if (contains_ral != -1 && contains_l != -1)
	      return STRINGPREP_BIDI_BOTH_L_AND_RAL;

	    if (contains_ral != -1)
	      {
		if (!(stringprep_find_character_in_table
		      (ucs4[0], profile[contains_ral].table) != -1 &&
		      stringprep_find_character_in_table
		      (ucs4[ucs4len - 1], profile[contains_ral].table) != -1))
		  return STRINGPREP_BIDI_LEADTRAIL_NOT_RAL;
	      }
	  }
	  break;

	default:
	  return STRINGPREP_PROFILE_ERROR;
	  break;
	}
    }

  *len = ucs4len;

  return STRINGPREP_OK;
}

static int
stringprep_4zi_1 (uint32_t *ucs4, size_t ucs4len, size_t maxucs4len,
		  Stringprep_profile_flags flags, Stringprep_profile * profile)
{
  int rc;

  rc = stringprep_4i (ucs4, &ucs4len, maxucs4len, flags, profile);
  if (rc != STRINGPREP_OK)
    return rc;

  if (ucs4len >= maxucs4len)
    return STRINGPREP_TOO_SMALL_BUFFER;

  ucs4[ucs4len] = 0;

  return STRINGPREP_OK;
}

/**
 * stringprep_4zi:
 * @ucs4: input/output array with zero terminated string to prepare.
 * @maxucs4len: maximum length of input/output array.
 * @flags: stringprep profile flags, or 0.
 * @profile: pointer to stringprep profile to use.
 *
 * Prepare the input zero terminated UCS-4 string according to the
 * stringprep profile, and write back the result to the input string.
 *
 * Since the stringprep operation can expand the string, @maxucs4len
 * indicate how large the buffer holding the string is.  This function
 * will not read or write to code points outside that size.
 *
 * The @flags are one of Stringprep_profile_flags, or 0.
 *
 * The @profile contain the instructions to perform.  Your application
 * can define new profiles, possibly re-using the generic stringprep
 * tables that always will be part of the library, or use one of the
 * currently supported profiles.
 *
 * Return value: Returns %STRINGPREP_OK iff successful, or an error code.
 **/
int
stringprep_4zi (uint32_t *ucs4, size_t maxucs4len,
		Stringprep_profile_flags flags, Stringprep_profile * profile)
{
  int rc;
  size_t ucs4len;

  for (ucs4len = 0; ucs4len < maxucs4len && ucs4[ucs4len] != 0; ucs4len++)
    ;

  return stringprep_4zi_1 (ucs4, ucs4len, maxucs4len, flags, profile);
}

/**
 * stringprep:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 * @flags: stringprep profile flags, or 0.
 * @profile: pointer to stringprep profile to use.
 *
 * Prepare the input zero terminated UTF-8 string according to the
 * stringprep profile, and write back the result to the input string.
 *
 * Note that you must convert strings entered in the systems locale
 * into UTF-8 before using this function, see
 * stringprep_locale_to_utf8().
 *
 * Since the stringprep operation can expand the string, @maxlen
 * indicate how large the buffer holding the string is.  This function
 * will not read or write to characters outside that size.
 *
 * The @flags are one of Stringprep_profile_flags, or 0.
 *
 * The @profile contain the instructions to perform.  Your application
 * can define new profiles, possibly re-using the generic stringprep
 * tables that always will be part of the library, or use one of the
 * currently supported profiles.
 *
 * Return value: Returns %STRINGPREP_OK iff successful, or an error code.
 **/
int
stringprep (char *in,
	    size_t maxlen,
	    Stringprep_profile_flags flags, Stringprep_profile * profile)
{
  size_t i, j;
  ssize_t k;
  int rc;
  char *utf8 = NULL;
  uint32_t *ucs4 = NULL;
  size_t ucs4len, maxucs4len, adducs4len = 50;

  do
    {
      if (ucs4)
	free (ucs4);
      ucs4 = stringprep_utf8_to_ucs4 (in, -1, &ucs4len);
      maxucs4len = ucs4len + adducs4len;
      ucs4 = realloc (ucs4, maxucs4len * sizeof (uint32_t));
      if (!ucs4)
	return STRINGPREP_MALLOC_ERROR;

      rc = stringprep_4i (ucs4, &ucs4len, maxucs4len, flags, profile);
      adducs4len += 50;
    }
  while (rc == STRINGPREP_TOO_SMALL_BUFFER);
  if (rc != STRINGPREP_OK)
    {
      free (ucs4);
      return rc;
    }

  utf8 = stringprep_ucs4_to_utf8 (ucs4, ucs4len, 0, 0);
  if (!utf8)
    return STRINGPREP_MALLOC_ERROR;

  if (strlen (utf8) >= maxlen)
    {
      free (utf8);
      free (ucs4);
      return STRINGPREP_TOO_SMALL_BUFFER;
    }

  strcpy (in, utf8);		/* flawfinder: ignore */

  return STRINGPREP_OK;
}

/**
 * stringprep_profile:
 * @in: input array with UTF-8 string to prepare.
 * @out: output variable with pointer to newly allocate string.
 * @profile: name of stringprep profile to use.
 * @flags: stringprep profile flags, or 0.
 *
 * Prepare the input zero terminated UTF-8 string according to the
 * stringprep profile, and return the result in a newly allocated
 * variable.
 *
 * Note that you must convert strings entered in the systems locale
 * into UTF-8 before using this function, see
 * stringprep_locale_to_utf8().
 *
 * The output @out variable must be deallocated by the caller.
 *
 * The @flags are one of Stringprep_profile_flags, or 0.
 *
 * The @profile specifies the name of the stringprep profile to use.
 * It must be one of the internally supported stringprep profiles.
 *
 * Return value: Returns %STRINGPREP_OK iff successful, or an error code.
 **/
int
stringprep_profile (const char *in,
		    char **out,
		    char *profile,
		    Stringprep_profile_flags flags)
{
  Stringprep_profiles *p;
  char *str = NULL;
  size_t len = strlen (in) + 1;
  int rc;

  for (p = &stringprep_profiles[0]; p->name; p++)
    if (strcmp (p->name, profile) == 0)
      break;

  if (!p || !p->name || !p->tables)
    return STRINGPREP_UNKNOWN_PROFILE;

  do
    {
      if (str)
	free (str);
      str = (char *) malloc (len);
      if (str == NULL)
	return STRINGPREP_MALLOC_ERROR;

      strcpy (str, in);

      rc = stringprep (str, len, flags, p->tables);
      len += 50;
    }
  while (rc == STRINGPREP_TOO_SMALL_BUFFER);

  if (rc == STRINGPREP_OK)
    *out = str;
  else
    free (str);

  return rc;
}

/**
 * STRINGPREP_VERSION
 *
 * String defined via CPP denoting the header file version number.
 * Used together with stringprep_check_version() to verify header file
 * and run-time library consistency.
 */

/**
 * STRINGPREP_MAX_MAP_CHARS
 *
 * Maximum number of code points that can replace a single code point,
 * during stringprep mapping.
 */

/**
 * Stringprep_rc
 *
 * Enumerated return codes of stringprep(), stringprep_profile()
 * functions (and macros using those functions).  The value 0 is
 * guaranteed to always correspond to success.
 */

/**
 * Stringprep_profile_flags:
 * @STRINGPREP_NO_NFKC: Disable the NFKC normalization, as well as
 *   selecting the non-NFKC case folding tables.  Usually the profile
 *   specifies BIDI and NFKC settings, and applications should not
 *   override it unless in special situations.
 * @STRINGPREP_NO_BIDI: Disable the BIDI step.  Usually the profile
 *   specifies BIDI and NFKC settings, and applications should not
 *   override it unless in special situations.
 * @STRINGPREP_NO_UNASSIGNED: Make the library return with an error if
 *   string contains unassigned characters according to profile.
 *
 * Stringprep profile flags.
 */

/**
 * Stringprep_profile_steps:
 *
 * Various steps in the stringprep algorithm.  You really want to
 * study the source code to understand this one.  Only useful if you
 * want to add another profile.
 */

/**
 * stringprep_nameprep:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the nameprep profile.
 * The AllowUnassigned flag is true, use
 * stringprep_nameprep_no_unassigned() for false AllowUnassigned.
 * Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_nameprep_no_unassigned:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the nameprep profile.
 * The AllowUnassigned flag is false, use stringprep_nameprep() for
 * true AllowUnassigned.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_iscsi:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft iSCSI
 * stringprep profile.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_kerberos5:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft Kerberos5
 * stringprep profile.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_plain:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft SASL
 * ANONYMOUS profile.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_xmpp_nodeprep:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft XMPP node
 * identifier profile.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_xmpp_resourceprep:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft XMPP resource
 * identifier profile.  Returns 0 iff successful, or an error code.
 **/

/**
 * stringprep_generic:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to a hypotetical "generic"
 * stringprep profile. This is mostly used for debugging or when
 * constructing new stringprep profiles. Returns 0 iff successful, or
 * an error code.
 **/
