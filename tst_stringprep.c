/* tst_stringprep.c	Self tests for stringprep().
 * Copyright (C) 2002, 2003  Simon Josefsson
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

#include "internal.h"

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

  printf ("`");
  for (i = 0; i < len; i++)
    {
      if (((str[i] & 0xFF) >= 'A' && (str[i] & 0xFF) <= 'Z') ||
	  ((str[i] & 0xFF) >= 'a' && (str[i] & 0xFF) <= 'z') ||
	  ((str[i] & 0xFF) >= '0' && (str[i] & 0xFF) <= '9')
	  || (str[i] & 0xFF) == ' ' || (str[i] & 0xFF) == '.')
	printf ("%c", (str[i] & 0xFF));
      else
	printf ("\\x%02x", (str[i] & 0xFF));
      if ((i+1)%16 == 0 && (i+1) < len)
	printf("'\n\t'");
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
  char *comment;
  char *in;
  char *out;
  Stringprep_profile *profile;
  int flags;
  int rc;
}
strprep[] =
{
  {
    "Map to nothing",
    "foo\xC2\xAD\xCD\x8F\xE1\xA0\x86\xE1\xA0\x8B"
    "bar""\xE2\x80\x8B\xE2\x81\xA0""baz\xEF\xB8\x80\xEF\xB8\x88"
    "\xEF\xB8\x8F\xEF\xBB\xBF", "foobarbaz"
  },
  {
    "Case folding ASCII U+0043 U+0041 U+0046 U+0045",
    "CAFE", "cafe"
  },
  {
    "Case folding 8bit U+00DF (german sharp s)",
    "\xC3\xDF", "ss"
  },
  {
    "Case folding U+0130 (turkish capital I with dot)",
    "\xC4\xB0", "i\xcc\x87"
  },
  {
    "Case folding multibyte U+0143 U+037A",
    "\xC5\x83\xCD\xBA", "\xC5\x84 \xCE\xB9"
  },
  {
    "Case folding U+2121 U+33C6 U+1D7BB",
    "\xE2\x84\xA1\xE3\x8F\x86\xF0\x9D\x9E\xBB",
    "telc\xE2\x88\x95""kg\xCF\x83"
  },
  {
    "Normalization of U+006a U+030c U+00A0 U+00AA",
    "\x6A\xCC\x8C\xC2\xA0\xC2\xAA", "\xC7\xB0 a"
  },
  {
    "Case folding U+1FB7 and normalization",
    "\xE1\xBE\xB7", "\xE1\xBE\xB6\xCE\xB9"
  },
  {
    "Self-reverting case folding U+01F0 and normalization",
    "\xC7\xF0", "\xC7\xB0"
  },
  {
    "Self-reverting case folding U+0390 and normalization",
    "\xCE\x90", "\xCE\x90"
  },
  {
    "Self-reverting case folding U+03B0 and normalization",
    "\xCE\xB0", "\xCE\xB0"
  },
  {
    "Self-reverting case folding U+1E96 and normalization",
    "\xE1\xBA\x96", "\xE1\xBA\x96"
  },
  {
    "Self-reverting case folding U+1F56 and normalization",
    "\xE1\xBD\x96", "\xE1\xBD\x96"
  },
  {
    "ASCII space character U+0020",
    "\x20", "\x20"
  },
  {
    "Non-ASCII 8bit space character U+00A0",
    "\xC2\xA0", "\x20"
  },
  {
    "Non-ASCII multibyte space character U+1680",
    "\xE1\x9A\x80", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Non-ASCII multibyte space character U+2000",
    "\xE2\x80\x80", "\x20"
  },
  {
    "Zero Width Space U+200b",
    "\xE2\x80\x8b", ""
  },
  {
    "Non-ASCII multibyte space character U+3000",
    "\xE3\x80\x80", "\x20"
  },
  {
    "ASCII control characters U+0010 U+007F",
    "\x10\x7F", "\x10\x7F"
  },
  {
    "Non-ASCII 8bit control character U+0085",
    "\xC2\x85", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Non-ASCII multibyte control character U+180E",
    "\xE1\xA0\x8E", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Zero Width No-Break Space U+FEFF",
    "\xEF\xBB\xBF", ""
  },
  {
    "Non-ASCII control character U+1D175",
    "\xF0\x9D\x85\xB5", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Plane 0 private use character U+F123",
    "\xEF\x84\xA3", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Plane 15 private use character U+F1234",
    "\xF3\xB1\x88\xB4", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Plane 16 private use character U+10F234",
    "\xF4\x8F\x88\xB4", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Non-character code point U+8FFFE",
    "\xF2\x8F\xBF\xBE", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Non-character code point U+10FFFF",
    "\xF4\x8F\xBF\xBF", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Surrogate code U+DF42",
    "\xED\xBD\x82", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Non-plain text character U+FFFD",
    "\xEF\xBF\xBD", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Ideographic description character U+2FF5",
    "\xE2\xBF\xB5", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Display property character U+0341",
    "\xCD\x81", "\xCC\x81"
  },
  {
    "Left-to-right mark U+200E",
    "\xE2\x80\x8E", "\xCC\x81", stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Deprecated U+202A",
    "\xE2\x80\xAA", "\xCC\x81", stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Language tagging character U+E0001",
    "\xF3\xA0\x80\x81", "\xCC\x81", stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Language tagging character U+E0042",
    "\xF3\xA0\x81\x82", NULL, stringprep_nameprep, 0,
    STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Bidi: RandALCat character U+05BE and LCat characters",
    "foo\xD6\xBE""bar", NULL, stringprep_nameprep, 0,
    STRINGPREP_BIDI_BOTH_L_AND_RAL
  },
  {
    "Bidi: RandALCat character U+FD50 and LCat characters",
    "foo\xEF\xB5\x90""bar", NULL, stringprep_nameprep, 0,
    STRINGPREP_BIDI_BOTH_L_AND_RAL
  },
  {
    "Bidi: RandALCat character U+FB38 and LCat characters",
    "foo\xEF\xB9\xB6""bar", "foo \xd9\x8e""bar"
  },
  { "Bidi: RandALCat without trailing RandALCat U+0627 U+0031",
    "\xD8\xA7\x31", NULL, stringprep_nameprep, 0,
    STRINGPREP_BIDI_LEADTRAIL_NOT_RAL}
  ,
  {
    "Bidi: RandALCat character U+0627 U+0031 U+0628",
    "\xD8\xA7\x31\xD8\xA8", "\xD8\xA7\x31\xD8\xA8"
  },
  {
    "Unassigned code point U+E0002",
    "\xF3\xA0\x80\x82", NULL, stringprep_nameprep, STRINGPREP_NO_UNASSIGNED,
    STRINGPREP_CONTAINS_UNASSIGNED
  },
  {
    "Larger test (shrinking)",
    "X\xC2\xAD\xC3\xDF\xC4\xB0\xE2\x84\xA1\x6a\xcc\x8c\xc2\xa0\xc2"
    "\xaa\xce\xb0\xe2\x80\x80", "xssi\xcc\x87""tel\xc7\xb0 a\xce\xb0 ",
    stringprep_nameprep
  },
  {
    "Larger test (expanding)",
    "X\xC3\xDF\xe3\x8c\x96\xC4\xB0\xE2\x84\xA1\xE2\x92\x9F\xE3\x8c\x80",
    "xss\xe3\x82\xad\xe3\x83\xad\xe3\x83\xa1\xe3\x83\xbc\xe3\x83\x88"
    "\xe3\x83\xab""i\xcc\x87""tel\x28""d\x29\xe3\x82\xa2\xe3\x83\x91"
    "\xe3\x83\xbc\xe3\x83\x88"
  },
#if !defined(DRAFT)
  { "Test of prohibited ASCII character U+0020",
    "\x20", NULL, stringprep_generic, 0, STRINGPREP_CONTAINS_PROHIBITED
  },
  {
    "Test of NFKC U+00A0 and prohibited character U+0020",
    "\xC2\xA0", NULL, stringprep_generic, 0, STRINGPREP_CONTAINS_PROHIBITED
  },
  { "Case map + normalization",
    "\xC2\xB5", "\xCE\xBC", stringprep_generic},
  /* The rest are rather non-interesting, but no point in removing
     working test cases... */
  { "case_nonfkc",
    "\xC2\xB5", "\xCE\xBC", stringprep_generic, STRINGPREP_NO_NFKC}
  ,
  { "NFKC test",
  "\xC2\xAA", "\x61", stringprep_generic},
  { "nameprep, exposed a bug in libstringprep 0.0.5",
    "\xC2\xAA\x0A", "\x61\x0A"},
  { "unassigned code point U+0221",
    "\xC8\xA1", "\xC8\xA1", stringprep_generic},
  { "Unassigned code point U+0221",
    "\xC8\xA1", NULL, stringprep_generic, STRINGPREP_NO_UNASSIGNED,
    STRINGPREP_CONTAINS_UNASSIGNED},
  { "Unassigned code point U+0236",
    "\xC8\xB6", "\xC8\xB6", stringprep_generic},
  { "unassigned code point U+0236",
    "\xC8\xB6", NULL, stringprep_generic, STRINGPREP_NO_UNASSIGNED,
    STRINGPREP_CONTAINS_UNASSIGNED},
  { "bidi both RandALCat and LCat  U+0627 U+00AA U+0628",
    "\xD8\xA7\xC2\xAA\xD8\xA8", NULL, stringprep_generic, 0,
    STRINGPREP_BIDI_BOTH_L_AND_RAL}
  ,
  { "XMPP node profile prohibited output",
    "foo@bar", NULL, stringprep_xmpp_nodeprep, 0,
      STRINGPREP_CONTAINS_PROHIBITED},
  { "XMPP resource profile on same string should work though",
    "foo@bar", "foo@bar", stringprep_xmpp_resourceprep},
  { "SASL ANONYMOUS plain mechanism",
    "simon@josefsson.org", "simon@josefsson.org", stringprep_plain}
#endif
};

int
main (int argc, char *argv[])
{
  char *p;
  int rc;
  size_t i;

  if (!stringprep_check_version (STRINGPREP_VERSION))
    fail ("stringprep_check_version() failed\n");

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
#ifdef DRAFT
      printf("<section title=\"%s.\">\n", strprep[i].comment);
      printf("\n");
      printf("<figure>\n");
      printf("<artwork>\n");
      printf ("in: ");
      escapeprint (strprep[i].in, strlen (strprep[i].in));
#endif
      if (debug)
	printf ("STRINGPREP entry %d\n", i);

      strcpy (p, strprep[i].in);

      if (debug)
	{
	  printf ("flags: %d\n", strprep[i].flags);

	  printf ("in: ");
	  escapeprint (strprep[i].in, strlen (strprep[i].in));
	  hexprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");
	  binprint (strprep[i].in, strlen (strprep[i].in));
	  puts ("");
	}

      rc = stringprep (p, BUFSIZ, strprep[i].flags,
		       strprep[i].profile ?
		       strprep[i].profile :
		       stringprep_nameprep);
      if (rc != strprep[i].rc)
	{
	  fail ("stringprep() entry %d failed: %d\n", i, rc);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

#ifdef DRAFT
      printf ("out: ");
      escapeprint (p, strlen (p));
#endif

      if (debug && rc == STRINGPREP_OK)
	{
	  printf ("out: ");
	  escapeprint (p, strlen (p));
	  hexprint (p, strlen (p));
	  puts ("");
	  binprint (p, strlen (p));
	  puts ("");

	  printf ("expected out: ");
	  escapeprint (strprep[i].out, strlen (strprep[i].out));
	  hexprint (strprep[i].out, strlen (strprep[i].out));
	  puts ("");
	  binprint (strprep[i].out, strlen (strprep[i].out));
	  puts ("");
	}
      else if (debug)
	printf ("returned %d expected %d\n", rc, strprep[i].rc);

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

#ifdef DRAFT
      printf("</artwork>\n");
      printf("</figure>\n");
      printf("\n");
      printf("</section>\n");
#endif
    }

#if 0
  memset (p, 0, 10);
  stringprep_unichar_to_utf8 (0x3316, p);
  hexprint (p, strlen (p));
  puts ("");
#endif

  free (p);

  if (debug)
    printf ("Stringprep self tests done with %d errors\n", error_count);

  return error_count ? 1 : 0;
}
