/* tst_stringprep.c	libstringprep self tests for stringprep_stringprep()
 * Copyright (C) 2002  Simon Josefsson
 *
 * This file is part of libstringprep.
 *
 * Libstringprep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Libstringprep is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libstringprep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "internal.h"
#include <stringprep_nameprep.h>

static int debug = 0;
static int error_count = 0;
static int break_on_error = 0;

static void
fail (const char *format, ...)
{
  va_list arg_ptr;

  va_start (arg_ptr, format);
  vfprintf (stderr, format, arg_ptr);
  va_end (arg_ptr);
  error_count++;
  if (break_on_error)
    exit (1);
}

static void
escapeprint (char *str, int len)
{
  int i;

  printf ("\t;; `");
  for (i = 0; i < len; i++)
    {
      if (((str[i] & 0xFF) >= 'A' && (str[i] & 0xFF) <= 'Z') ||
	  ((str[i] & 0xFF) >= 'a' && (str[i] & 0xFF) <= 'z') ||
	  ((str[i] & 0xFF) >= '0' && (str[i] & 0xFF) <= '9')
	  || (str[i] & 0xFF) == '.')
	printf ("%c", (str[i] & 0xFF));
      else
	printf ("\\x%02x", (str[i] & 0xFF));
    }
  printf ("' (length %d bytes)\n", len);
}

static void
hexprint (char *str, int len)
{
  int i;

  printf ("\t;; ");
  for (i = 0; i < len; i++)
    {
      printf ("%02x ", (str[i] & 0xFF));
      if ((i + 1) % 8 == 0)
	printf (" ");
      if ((i + 1) % 16 == 0 && i + 1 < len)
	printf ("\n\t;; ");
    }
}

static void
binprint (char *str, int len)
{
  int i;

  printf ("\t;; ");
  for (i = 0; i < len; i++)
    {
      printf ("%d%d%d%d%d%d%d%d ",
	      (str[i] & 0xFF) & 0x80 ? 1 : 0,
	      (str[i] & 0xFF) & 0x40 ? 1 : 0,
	      (str[i] & 0xFF) & 0x20 ? 1 : 0,
	      (str[i] & 0xFF) & 0x10 ? 1 : 0,
	      (str[i] & 0xFF) & 0x08 ? 1 : 0,
	      (str[i] & 0xFF) & 0x04 ? 1 : 0,
	      (str[i] & 0xFF) & 0x02 ? 1 : 0, (str[i] & 0xFF) & 0x01 ? 1 : 0);
      if ((i + 1) % 3 == 0)
	printf (" ");
      if ((i + 1) % 6 == 0 && i + 1 < len)
	printf ("\n\t;; ");
    }
}

struct stringprep
{
  char *in;
  int flags;
  char *out;
  Stringprep_profile *profile;
  int rc;
}
strprep[] =
{
  /* map to nothing U+00AD */
  {
  "foo\xC2\xAD" "bar", 0, "foobar", stringprep_generic}
  ,
    /* map case_nfkc + normalization: */
  {
  "\xC2\xB5", 0, "\xCE\xBC", stringprep_generic}
  ,
    /* case_nonfkc: */
  {
  "\xC2\xB5", STRINGPREP_NO_NFKC, "\xCE\xBC", stringprep_generic}
  ,
  {
  "\xC2\xAA", 0, "\x61", stringprep_generic}
  ,
  /* nameprep, exposed a bug in libstringprep 0.0.5 */
  {
    "\xC2\xAA\x0A", 0, "\x61\x0A", stringprep_nameprep}
  ,
  /* unassigned code point U+0221: */
  {
    "\xC8\xA1", 0, "\xC8\xA1", stringprep_generic}
  ,
  /* unassigned code point U+0221: */
  {
    "\xC8\xA1", STRINGPREP_NO_UNASSIGNED, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_UNASSIGNED}
  ,
  /* unassigned code point U+0236: */
  {
    "\xC8\xB6", 0, "\xC8\xB6", stringprep_generic}
  ,
  /* unassigned code point U+0236: */
  {
    "\xC8\xB6", STRINGPREP_NO_UNASSIGNED, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_UNASSIGNED},
  /* prohibited ASCII character U+0020: */
  {
    "\x20", 0, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_PROHIBITED},
  /* prohibited character U+00A0: */
  {
    "\xC2\xA0", 0, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_PROHIBITED},
  /* prohibited non-character U+10FFFE: */
  {
    "\xF4\x8F\xBF\xBE", 0, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_PROHIBITED},
  /* prohibited surrogate character U+D801: */
  {
    "\xED\xA0\x81", 0, NULL, stringprep_generic,
    STRINGPREP_CONTAINS_PROHIBITED},
  /* bidi RandALCat without trailing RandALCat <U+0627><U+0031>: */
  {
    "\xD8\xA7\x31", 0, NULL, stringprep_generic,
    STRINGPREP_BIDI_LEADTRAIL_NOT_RAL},
  /* bidi RandALCat correct  <U+0627><U+0031><U+0628>: */
  {
    "\xD8\xA7\x31\xD8\xA8", 0, "\xD8\xA7\x31\xD8\xA8", stringprep_generic},
  /* bidi both RandALCat and LCat  <U+0627><U+00AA><U+0628>: */
  {
    "\xD8\xA7\xC2\xAA\xD8\xA8", 0, NULL, stringprep_generic,
    STRINGPREP_BIDI_BOTH_L_AND_RAL},
  /* case mapping (this triggered a bug in 0.0.5) */
  {
    "CAFE", 0, "cafe", stringprep_generic}
};

int
main (int argc, char *argv[])
{
  char *p;
  int rc, i;

  if (!stringprep_check_version (STRINGPREP_VERSION))
    fail("stringprep_check_version() failed\n");

  do
    if (strcmp (argv[argc - 1], "-v") == 0 ||
	strcmp (argv[argc - 1], "--verbose") == 0)
      debug = 1;
    else if (strcmp (argv[argc - 1], "-b") == 0 ||
	     strcmp (argv[argc - 1], "--break-on-error") == 0)
      break_on_error = 1;
    else if (strcmp (argv[argc - 1], "-h") == 0 ||
	     strcmp (argv[argc - 1], "-?") == 0 ||
	     strcmp (argv[argc - 1], "--help") == 0)
      {
	printf ("Usage: %s [-vbh?] [--verbose] [--break-on-error] [--help]\n",
		argv[0]);
	return 1;
      }
  while (argc-- > 1);

  p = malloc (BUFSIZ);
  if (p == NULL)
    fail ("malloc() returned NULL\n");

  for (i = 0; i < sizeof (strprep) / sizeof (strprep[0]); i++)
    {
      if (debug)
	printf ("STRINGPREP entry %d\n", i);

      strcpy (p, strprep[i].in);

      if (debug)
	{
	  printf ("flags: %d\n", strprep[i].flags);

	  printf ("in:\n");
	  escapeprint (strprep[i].in, strlen (strprep[i].in));
	  hexprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");
	  binprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");
	}

      rc = stringprep (p, BUFSIZ, strprep[i].flags, strprep[i].profile);
      if (rc != strprep[i].rc)
	{
	  fail ("stringprep() entry %d failed: %d\n", i, rc);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

      if (debug && rc == STRINGPREP_OK)
	{
	  printf ("out:\n");
	  escapeprint (p, strlen (p));
	  hexprint (p, strlen (p));
	  puts ("");
	  binprint (p, strlen (p));
	  puts ("");

	  printf ("expected out:\n");
	  escapeprint (strprep[i].out, strlen (strprep[i].out));
	  hexprint (strprep[i].out, strlen (strprep[i].out));
	  puts ("");
	  binprint (strprep[i].out, strlen (strprep[i].out));
	  puts ("");
	}
      else if (debug)
	printf("returned %d expected %d\n", rc, strprep[i].rc);

      if (rc == STRINGPREP_OK)
	{
	  if (strlen (strprep[i].out) != strlen (p) ||
	      memcmp (strprep[i].out, p, strlen (p)) != 0)
	    {
	      fail ("stringprep() entry %d failed\n", i);
	      if (debug)
		printf ("ERROR\n");
	    }
	  else if (debug)
	    printf ("OK\n\n");
	}
      else if (debug)
	printf ("OK\n\n");
    }

  free (p);

#if 0
  memset(p, 0, 10);
  stringprep_unichar_to_utf8 (0x0628, p);
  hexprint (p, strlen (p)); puts("");
#endif

  if (debug)
    printf ("Stringprep self tests done with %d errors\n", error_count);

  return error_count ? 1 : 0;
}
