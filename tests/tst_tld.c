/* tst_tld.c --- Self tests for tld_*().
 * Copyright (C) 2004  Simon Josefsson.
 *
 * This file is part of GNU Libidn.
 *
 * GNU Libidn is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GNU Libidn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Libidn; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <stringprep.h>
#include <tld.h>

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
ucs4print (const uint32_t * str, size_t len)
{
  size_t i;

  printf ("\t;; ");
  for (i = 0; i < len; i++)
    {
      printf ("U+%04ux ", str[i]);
      if ((i + 1) % 4 == 0)
	printf (" ");
      if ((i + 1) % 8 == 0 && i + 1 < len)
	printf ("\n\t;; ");
    }
  puts ("");
}

struct tld
{
  char *name;
  char *tld;
  size_t inlen;
  uint32_t in[100];
  int rc;
  int errpos;
};

const struct tld tld[] = {
  {
    "Simple valid French domain",
    "fr",
    3,
    { 0x00E0, 0x00E2, 0x00E6 },
    TLD_SUCCESS },
  {
    "Simple invalid French domain",
    "fr",
    5,
    { 0x00E0, 0x00E2, 0x00E6, 0x4711, 0x0042 },
    TLD_ILLEGAL,
    3
  }
};

int
main (int argc, char *argv[])
{
  size_t i;
  const Tld_table *tldtable;
  int errpos;
  int rc;

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

  for (i = 0; i < sizeof (tld) / sizeof (tld[0]); i++)
    {
      if (debug)
	printf ("TLD entry %d: %s\n", i, tld[i].name);

      if (debug)
	{
	  printf ("in:\n");
	  ucs4print (tld[i].in, tld[i].inlen);
	}

      tldtable = tld_finddomain (tld[i].tld, NULL);
      if (tldtable == NULL)
	{
	  fail ("TLD entry %d tld_finddomain (%s)\n", i, tld[i].tld);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

      rc = tld_check_4it (tld[i].in, tld[i].inlen, &errpos, tldtable);
      if (rc != tld[i].rc)
	{
	  fail ("TLD entry %d failed: %d\n", i, rc);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

      if (debug)
	printf ("returned %d expected %d\n", rc, tld[i].rc);

      if (rc != tld[i].rc)
	{
	  fail ("TLD entry %d failed\n", i);
	  if (debug)
	    printf ("ERROR\n");
	}
      else if (rc == TLD_ILLEGAL)
	{
	  if (debug)
	    printf ("returned errpos %d expected errpos %d\n",
		    errpos, tld[i].errpos);

	  if (tld[i].errpos != errpos)
	    {
	      fail ("TLD entry %d failed because errpos %d != %d\n", i,
		    tld[i].errpos, errpos);
	      if (debug)
		printf ("ERROR\n");
	    }
	}
      else if (debug)
	printf ("OK\n");
    }

  if (debug)
    printf ("TLD self tests done with %d errors\n", error_count);

  return error_count ? 1 : 0;
}
