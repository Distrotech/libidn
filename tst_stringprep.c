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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#endif
#if HAVE_STRINGS_H
# include <strings.h>
#endif
#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <stringprep.h>
#include <stringprep_generic.h>

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

  printf ("\t ;; `");
  for (i = 0; i < len; i++)
    {
      str[i] = str[i] & 0xFF;
      if ((str[i] >= 'A' && str[i] <= 'Z') ||
	  (str[i] >= 'a' && str[i] <= 'z') ||
	  (str[i] >= '0' && str[i] <= '9') || str[i] == '.')
	printf ("%c", str[i]);
      else
	printf ("\\x%02x", str[i]);
    }
  printf ("' (length %d bytes)\n", len);
}

static void
hexprint (char *str, int len)
{
  int i;

  printf ("\t ;; ");
  for (i = 0; i < len; i++)
    {
      str[i] = str[i] & 0xFF;
      printf ("%02x ", str[i]);
      if ((i + 1) % 8 == 0)
	printf (" ");
      if ((i + 1) % 16 == 0 && i + 1 < len)
	printf ("\n\t ;; ");
    }
}

static void
binprint (char *str, int len)
{
  int i;

  printf ("\t ;; ");
  for (i = 0; i < len; i++)
    {
      str[i] = str[i] & 0xFF;
      printf ("%d%d%d%d%d%d%d%d ",
	      str[i] & 0x80 ? 1 : 0,
	      str[i] & 0x40 ? 1 : 0,
	      str[i] & 0x20 ? 1 : 0,
	      str[i] & 0x10 ? 1 : 0,
	      str[i] & 0x08 ? 1 : 0,
	      str[i] & 0x04 ? 1 : 0,
	      str[i] & 0x02 ? 1 : 0, str[i] & 0x01 ? 1 : 0);
      if ((i + 1) % 3 == 0)
	printf (" ");
      if ((i + 1) % 6 == 0 && i + 1 < len)
	printf ("\n\t ;; ");
    }
}

struct stringprep
{
  char *in;
  int flags;
  char *out;
  stringprep_profile *profile;
}
strprep[] =
{
  /* nothing */
  {
  "foo\xC2\xAD" "bar", 0, "foobar", stringprep_generic}
  ,
    /* case_nfkc + normalization: */
  {
  "\xC2\xB5", 0, "\xCE\xBC", stringprep_generic}
  ,
    /* case_nonfkc: */
  {
  "\xC2\xB5", STRINGPREP_NO_NFKC, "\xCE\xBC", stringprep_generic}
  ,
  {
  "\xC2\xAA", 0, "\x61", stringprep_generic}
};

int
main (int argc, char *argv[])
{
  char *p;
  int rc, i;

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

      rc = stringprep (p, BUFSIZ, strprep[i].flags, strprep[i].profile);
      if (rc != STRINGPREP_OK)
	{
	  fail ("stringprep() entry %d failed: %d\n", i, rc);
	  continue;
	}

      if (debug)
	{
	  printf ("in:\n");
	  escapeprint (strprep[i].in, strlen (strprep[i].in));
	  hexprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");
	  binprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");

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

      if (strlen (strprep[i].out) != strlen (p) ||
	  memcmp (strprep[i].out, p, strlen (p)) != 0)
	{
	  fail ("stringprep() entry %d failed\n", i);
	  if (debug)
	    printf ("ERROR\n");
	}
      else if (debug)
	printf ("OK\n");
    }

  free (p);

  if (debug)
    printf ("Stringprep self tests done with %d errors\n", error_count);

  return error_count ? 1 : 0;
}
