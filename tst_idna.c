/* tst_idna.c	Self tests for idna_to_ascii().
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
ucs4print (unsigned long *str, ssize_t len)
{
  int i;

  printf ("\t;; ");
  for (i = 0; len >= 0 ? i < len : str[i]; i++)
    {
      printf ("U+%04lux ", str[i]);
      if ((i + 1) % 4 == 0)
	printf (" ");
      if ((i + 1) % 8 == 0 && i + 1 < len)
	printf ("\n\t;; ");
    }
  puts ("");
}

struct idna
{
  char *name;
  size_t inlen;
  unsigned long in[100];
  char *out;
  int allowunassigned;
  int usestd3asciirules;
  int toasciirc;
  int tounicoderc;
} idna[] =
{
  {
    "(A) Arabic (Egyptian)", 17,
    {
  0x0644, 0x064A, 0x0647, 0x0645, 0x0627, 0x0628, 0x062A, 0x0643,
	0x0644, 0x0645, 0x0648, 0x0634, 0x0639, 0x0631, 0x0628, 0x064A,
	0x061F},
      IDNA_ACE_PREFIX "egbpdaj6bu4bxfgehfvwxn", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(B) Chinese (simplified)", 9,
    {
  0x4ED6, 0x4EEC, 0x4E3A, 0x4EC0, 0x4E48, 0x4E0D, 0x8BF4, 0x4E2D, 0x6587},
      IDNA_ACE_PREFIX "ihqwcrb4cv8a8dqg056pqjye", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(C) Chinese (traditional)", 9,
    {
  0x4ED6, 0x5011, 0x7232, 0x4EC0, 0x9EBD, 0x4E0D, 0x8AAA, 0x4E2D, 0x6587},
      IDNA_ACE_PREFIX "ihqwctvzc91f659drss3x8bo0yb", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(D) Czech: Pro<ccaron>prost<ecaron>nemluv<iacute><ccaron>esky", 22,
    {
  0x0050, 0x0072, 0x006F, 0x010D, 0x0070, 0x0072, 0x006F, 0x0073,
	0x0074, 0x011B, 0x006E, 0x0065, 0x006D, 0x006C, 0x0075, 0x0076,
	0x00ED, 0x010D, 0x0065, 0x0073, 0x006B, 0x0079},
      IDNA_ACE_PREFIX "Proprostnemluvesky-uyb24dma41a", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(E) Hebrew:", 22,
    {
  0x05DC, 0x05DE, 0x05D4, 0x05D4, 0x05DD, 0x05E4, 0x05E9, 0x05D5,
	0x05D8, 0x05DC, 0x05D0, 0x05DE, 0x05D3, 0x05D1, 0x05E8, 0x05D9,
	0x05DD, 0x05E2, 0x05D1, 0x05E8, 0x05D9, 0x05EA},
      IDNA_ACE_PREFIX "4dbcagdahymbxekheh6e0a7fei0b", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(F) Hindi (Devanagari):", 30,
    {
  0x092F, 0x0939, 0x0932, 0x094B, 0x0917, 0x0939, 0x093F, 0x0928,
	0x094D, 0x0926, 0x0940, 0x0915, 0x094D, 0x092F, 0x094B, 0x0902,
	0x0928, 0x0939, 0x0940, 0x0902, 0x092C, 0x094B, 0x0932, 0x0938,
	0x0915, 0x0924, 0x0947, 0x0939, 0x0948, 0x0902},
      IDNA_ACE_PREFIX "i1baa7eci9glrd9b2ae1bj0hfcgg6iyaf8o0a1dig0cd", 0, 0,
      IDNA_SUCCESS},
  {
    "(G) Japanese (kanji and hiragana):", 18,
    {
  0x306A, 0x305C, 0x307F, 0x3093, 0x306A, 0x65E5, 0x672C, 0x8A9E,
	0x3092, 0x8A71, 0x3057, 0x3066, 0x304F, 0x308C, 0x306A, 0x3044,
	0x306E, 0x304B},
      IDNA_ACE_PREFIX "n8jok5ay5dzabd5bym9f0cm5685rrjetr6pdxa", 0, 0,
      IDNA_SUCCESS},
  {
    "(H) Korean (Hangul syllables):", 24,
    {
  0xC138, 0xACC4, 0xC758, 0xBAA8, 0xB4E0, 0xC0AC, 0xB78C, 0xB4E4,
	0xC774, 0xD55C, 0xAD6D, 0xC5B4, 0xB97C, 0xC774, 0xD574, 0xD55C,
	0xB2E4, 0xBA74, 0xC5BC, 0xB9C8, 0xB098, 0xC88B, 0xC744, 0xAE4C},
      IDNA_ACE_PREFIX "989aomsvi5e83db1d2a355cv1e0vak1dwrv93d5xbh15a0dt"
      "30a5jpsd879ccm6fea98c", 0, 0, IDNA_PUNYCODE_ERROR,
      IDNA_PUNYCODE_ERROR},
    /* too long output */
  {
    "(I) Russian (Cyrillic):", 28,
    {
  0x043F, 0x043E, 0x0447, 0x0435, 0x043C, 0x0443, 0x0436, 0x0435,
	0x043E, 0x043D, 0x0438, 0x043D, 0x0435, 0x0433, 0x043E, 0x0432,
	0x043E, 0x0440, 0x044F, 0x0442, 0x043F, 0x043E, 0x0440, 0x0443,
	0x0441, 0x0441, 0x043A, 0x0438},
      IDNA_ACE_PREFIX "b1abfaaepdrnnbgefbadotcwatmq2g4l", 0, 0,
      IDNA_SUCCESS, IDNA_SUCCESS},
  {
    "(J) Spanish: Porqu<eacute>nopuedensimplementehablarenEspa<ntilde>ol", 40,
    {
  0x0050, 0x006F, 0x0072, 0x0071, 0x0075, 0x00E9, 0x006E, 0x006F,
	0x0070, 0x0075, 0x0065, 0x0064, 0x0065, 0x006E, 0x0073, 0x0069,
	0x006D, 0x0070, 0x006C, 0x0065, 0x006D, 0x0065, 0x006E, 0x0074,
	0x0065, 0x0068, 0x0061, 0x0062, 0x006C, 0x0061, 0x0072, 0x0065,
	0x006E, 0x0045, 0x0073, 0x0070, 0x0061, 0x00F1, 0x006F, 0x006C},
      IDNA_ACE_PREFIX "PorqunopuedensimplementehablarenEspaol-fmd56a", 0, 0,
      IDNA_SUCCESS},
  {
    "(K) Vietnamese:", 31,
    {
  0x0054, 0x1EA1, 0x0069, 0x0073, 0x0061, 0x006F, 0x0068, 0x1ECD,
	0x006B, 0x0068, 0x00F4, 0x006E, 0x0067, 0x0074, 0x0068, 0x1EC3,
	0x0063, 0x0068, 0x1EC9, 0x006E, 0x00F3, 0x0069, 0x0074, 0x0069,
	0x1EBF, 0x006E, 0x0067, 0x0056, 0x0069, 0x1EC7, 0x0074},
      IDNA_ACE_PREFIX "TisaohkhngthchnitingVit-kjcr8268qyxafd2f1b9g", 0, 0,
      IDNA_SUCCESS},
  {
    "(L) 3<nen>B<gumi><kinpachi><sensei>", 8,
    {
  0x0033, 0x5E74, 0x0042, 0x7D44, 0x91D1, 0x516B, 0x5148, 0x751F},
      IDNA_ACE_PREFIX "3B-ww4c5e180e575a65lsy2b", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(M) <amuro><namie>-with-SUPER-MONKEYS", 24,
    {
  0x5B89, 0x5BA4, 0x5948, 0x7F8E, 0x6075, 0x002D, 0x0077, 0x0069,
	0x0074, 0x0068, 0x002D, 0x0053, 0x0055, 0x0050, 0x0045, 0x0052,
	0x002D, 0x004D, 0x004F, 0x004E, 0x004B, 0x0045, 0x0059, 0x0053},
      IDNA_ACE_PREFIX "-with-SUPER-MONKEYS-pc58ag80a8qai00g7n9n", 0, 0,
      IDNA_SUCCESS},
  {
    "(N) Hello-Another-Way-<sorezore><no><basho>", 25,
    {
  0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0x002D, 0x0041, 0x006E,
	0x006F, 0x0074, 0x0068, 0x0065, 0x0072, 0x002D, 0x0057, 0x0061,
	0x0079, 0x002D, 0x305D, 0x308C, 0x305E, 0x308C, 0x306E, 0x5834,
	0x6240},
      IDNA_ACE_PREFIX "Hello-Another-Way--fc4qua05auwb3674vfr0b", 0, 0,
      IDNA_SUCCESS},
  {
    "(O) <hitotsu><yane><no><shita>2", 8,
    {
  0x3072, 0x3068, 0x3064, 0x5C4B, 0x6839, 0x306E, 0x4E0B, 0x0032},
      IDNA_ACE_PREFIX "2-u9tlzr9756bt3uc0v", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(P) Maji<de>Koi<suru>5<byou><mae>", 13,
    {
  0x004D, 0x0061, 0x006A, 0x0069, 0x3067, 0x004B, 0x006F, 0x0069,
	0x3059, 0x308B, 0x0035, 0x79D2, 0x524D},
      IDNA_ACE_PREFIX "MajiKoi5-783gue6qz075azm5e", 0, 0, IDNA_SUCCESS,
      IDNA_SUCCESS},
  {
    "(Q) <pafii>de<runba>", 9,
    {
  0x30D1, 0x30D5, 0x30A3, 0x30FC, 0x0064, 0x0065, 0x30EB, 0x30F3, 0x30D0},
      IDNA_ACE_PREFIX "de-jg4avhby1noc0d", 0, 0, IDNA_SUCCESS, IDNA_SUCCESS},
  {
    "(R) <sono><supiido><de>", 7,
    {
  0x305D, 0x306E, 0x30B9, 0x30D4, 0x30FC, 0x30C9, 0x3067},
      IDNA_ACE_PREFIX "d9juau41awczczp", 0, 0, IDNA_SUCCESS, IDNA_SUCCESS},
#if 0
  {
    "(S) -> $1.00 <-", 11,
    {
  0x002D, 0x003E, 0x0020, 0x0024, 0x0031, 0x002E, 0x0030, 0x0030,
	0x0020, 0x003C, 0x002D},
      IDNA_ACE_PREFIX "-> $1.00 <--", 0, 0, IDNA_SUCCESS, IDNA_SUCCESS},
#endif
  {				/* XXX depends on IDNA_ACE_PREFIX */
    "ToASCII() with ACE prefix", 4 + 3,
    { 'x', 'k', '-', '-', 'f', 'o', 0x3067},
      IDNA_ACE_PREFIX "too long too long too long too long too long too "
      "long too long too long too long too long ", 0, 0,
      IDNA_CONTAINS_ACE_PREFIX, IDNA_PUNYCODE_ERROR}
};

int
main (int argc, char *argv[])
{
  char label[100];
  unsigned long *ucs4label;
  unsigned long tmp[100];
  ssize_t len;
  size_t len2;
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

  for (i = 0; i < sizeof (idna) / sizeof (idna[0]); i++)
    {
      if (debug)
	printf ("IDNA entry %d: %s\n", i, idna[i].name);

      if (debug)
	{
	  printf ("in:\n");
	  ucs4print (idna[i].in, idna[i].inlen);
	}

      rc = idna_to_ascii (idna[i].in, idna[i].inlen, label,
			  idna[i].allowunassigned, idna[i].usestd3asciirules);
      if (rc != idna[i].toasciirc)
	{
	  fail ("IDNA entry %d failed: %d\n", i, rc);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

      if (debug && rc == IDNA_SUCCESS)
	{
	  printf ("computed out: %s\n", label);
	  printf ("expected out: %s\n", idna[i].out);
	}
      else if (debug)
	printf ("returned %d expected %d\n", rc, idna[i].toasciirc);

      if (rc == IDNA_SUCCESS)
	{
	  if (strlen (idna[i].out) != strlen (label) ||
	      strcasecmp (idna[i].out, label) != 0)
	    {
	      fail ("IDNA entry %d failed\n", i);
	      if (debug)
		printf ("ERROR\n");
	    }
	  else if (debug)
	    printf ("OK\n");
	}
      else if (debug)
	printf ("OK\n");

      ucs4label = stringprep_utf8_to_ucs4 (idna[i].out, -1, &len);

      if (debug)
	{
	  printf ("in: %s (%d==%d)\n", idna[i].out, strlen (idna[i].out),
		  len);
	  ucs4print (ucs4label, len);
	}

      len2 = sizeof (tmp);
      rc = idna_to_unicode (ucs4label, len, tmp, &len2,
			    idna[i].allowunassigned,
			    idna[i].usestd3asciirules);
      if (debug)
	{
	  printf ("expected out:\n");
	  if (rc == IDNA_SUCCESS)
	    ucs4print (idna[i].in, idna[i].inlen);
	  else
	    ucs4print (ucs4label, len);

	  printf ("computed out:\n");
	  ucs4print (tmp, len2);
	}

      if (rc != idna[i].tounicoderc)
	{
	  fail ("IDNA entry %d failed: %d\n", i, rc);
	  if (debug)
	    printf ("FATAL\n");
	  continue;
	}

      if ((rc == IDNA_SUCCESS && (len2 != idna[i].inlen ||
				  memcmp (idna[i].in, tmp, len2) != 0)) ||
	  (rc != IDNA_SUCCESS && (len2 != len ||
				  memcmp (ucs4label, tmp, len) != 0)))
	{
	  fail ("IDNA entry %d failed\n", i);
	  if (debug)
	    printf ("ERROR\n");
	}
      else if (debug)
	printf ("OK\n\n");

    }

  if (debug)
    printf ("IDNA self tests done with %d errors\n", error_count);

  return error_count ? 1 : 0;
}
