/* stringprep.c	library implementing IETF stringprep specification
 * Copyright (C) 2002  Simon Josefsson
 *
 * This file is part of libstringprep.
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
 * License along with libstringprep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "internal.h"

#define DBG 0

static int
stringprep_find_character_in_table (long ucs4,
				    struct stringprep_table_element *table)
{
  int i;

  for (i = 0; table[i].start; i++)
    if (ucs4 >= table[i].start &&
	ucs4 <= (table[i].end ? table[i].end : table[i].start))
      return i;

  return -1;
}

static int
stringprep_find_string_in_table (long *ucs4,
				 int ucs4len,
				 int *tablepos,
				 struct stringprep_table_element *table)
{
  int j, pos;

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
stringprep_apply_table_to_string (long *ucs4,
				  int *ucs4len,
				  int maxucs4len,
				  struct stringprep_table_element *table)
{
  int i;
  int pos, maplen;

#if DBG
  printf ("in:  ");
  for (i = 0; i < *ucs4len; i++)
    printf ("U+%06x ", ucs4[i]);
  printf ("\n");
#endif

  if ((pos =
       stringprep_find_string_in_table (ucs4, *ucs4len, &i, table)) != -1)
    {
#if DBG
      printf ("hit %06x %06x-%06x\n", ucs4[pos], table[i].start,
	      table[i].end);
#endif

      for (maplen = 0; table[i].map[maplen]; maplen++)
	;

      if (*ucs4len - 1 + maplen >= maxucs4len)
	return STRINGPREP_TOO_SMALL_BUFFER;

      memmove (&ucs4[pos + maplen], &ucs4[pos + 1],
	       *ucs4len * sizeof (uint32_t) - (&ucs4[pos + 1] - ucs4));
      memcpy (&ucs4[pos], table[i].map, sizeof (uint32_t) * maplen);
      *ucs4len = *ucs4len - 1 + maplen;
    }

#if DBG
  printf ("out: ");
  for (i = 0; i < *ucs4len; i++)
    printf ("U+%06x ", ucs4[i]);
  printf ("\n");
#endif

  return STRINGPREP_OK;
}

int
stringprep (char *in, int maxlen, int flags, stringprep_profile * profile)
{
  int i;
  int rc;
  char *p = 0;
  long *q = 0;
  long *ucs4;
  int ucs4len;
  int maxucs4len;

  ucs4 = stringprep_utf8_to_ucs4 (in, -1, &ucs4len);
  maxucs4len = 4 * ucs4len + 10;
  ucs4 = realloc (ucs4, 1 + maxucs4len * sizeof (long));
  if (!ucs4)
    {
      rc = STRINGPREP_MALLOC_ERROR;
      goto done;
    }

  for (i = 0; profile[i].flags || profile[i].table; i++)
    {
      if (!profile[i].table && profile[i].flags == ~STRINGPREP_NO_NFKC &&
	  !(flags & STRINGPREP_NO_NFKC))
	{

	  q = stringprep_ucs4_nfkc_normalize (ucs4, ucs4len);

	  if (!q)
	    {
	      rc = STRINGPREP_NFKC_FAILED;
	      goto done;
	    }

	  for (ucs4len = 0; ucs4[ucs4len]; ucs4len++)
	    ;

	  free (ucs4);
	  ucs4 = q;
	  q = 0;

	  continue;
	}
      else if (profile[i].flags & STRINGPREP_INVERT_MAGIC)
	{
	  if (flags & (~profile[i].flags))
	    continue;
	}
      else
	{
	  if (profile[i].flags != 0 && !(flags & profile[i].flags))
	    continue;
	}

      if (!profile[i].table)
	continue;		/* this indicates an error in the profile ... */

      rc = stringprep_apply_table_to_string
	(ucs4, &ucs4len, maxucs4len, profile[i].table);
      if (rc != STRINGPREP_OK)
	goto done;
    }

  if (!(flags & STRINGPREP_NO_BIDI))
    {
      int done_prohibited = 0;
      int done_ral = 0;
      int done_l = 0;
      int contains_ral = 0;
      int contains_l = 0;

      for (i = 0; profile[i].flags || profile[i].table; i++)
	if (profile[i].flags == STRINGPREP_BIDI_PROHIBITED_MASK)
	  {
	    done_prohibited = 1;
	    rc = stringprep_apply_table_to_string
	      (ucs4, &ucs4len, maxucs4len, profile[i].table);
	    if (rc != STRINGPREP_OK)
	      goto done;
	  }
	else if (profile[i].flags == STRINGPREP_BIDI_RAL_MASK)
	  {
	    done_ral = 1;
	    contains_ral = stringprep_find_string_in_table
	      (ucs4, ucs4len, 0, profile[i].table) != -1;
	  }
	else if (profile[i].flags == STRINGPREP_BIDI_L_MASK)
	  {
	    done_l = 1;
	    contains_l = stringprep_find_string_in_table
	      (ucs4, ucs4len, 0, profile[i].table) != -1;
	  }

      if (!done_prohibited || !done_ral || !done_l)
	{
	  rc = STRINGPREP_BIDI_NOT_IN_PROFILE;
	  goto done;
	}

      if (contains_ral && contains_l)
	{
	  rc = STRINGPREP_BIDI_BOTH_L_AND_RAL;
	  goto done;
	}

      if (contains_ral &&
	  (!stringprep_find_character_in_table (ucs4[0],
						profile[i].table) ||
	   !stringprep_find_character_in_table (ucs4[ucs4len],
						profile[i].table)))
	{
	  rc = STRINGPREP_BIDI_LEADTRAIL_NOT_RAL;
	  goto done;
	}
    }

  p = stringprep_ucs4_to_utf8 (ucs4, ucs4len, 0, 0);

  if (strlen (p) >= maxlen)
    {
      rc = STRINGPREP_TOO_SMALL_BUFFER;
      goto done;
    }

  strcpy (in, p);

  rc = STRINGPREP_OK;

done:
  if (p)
    free (p);
  if (q)
    free (q);
  if (ucs4)
    free (ucs4);
  return rc;
}
