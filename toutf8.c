/* toutf8.c	convert strings from system locale into UTF-8
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

#ifdef HAVE_ICONV

#include <iconv.h>

#if LOCALE_WORKS
#include <langinfo.h>
#include <locale.h>
#endif

static const char *
stringprep_locale_charset_slow ()
{
  const char *charset = getenv ("CHARSET");
  char *p;

  if (charset && *charset)
    return charset;

#if LOCALE_WORKS
  p = setlocale (LC_CTYPE, NULL);
  setlocale (LC_CTYPE, "");

  charset = nl_langinfo (CODESET);

  setlocale (LC_CTYPE, p);
#endif

  if (charset && *charset)
    return charset;

  return "ASCII";
}

static const char *stringprep_locale_charset_cache = NULL;

const char *
stringprep_locale_charset ()
{
  if (!stringprep_locale_charset_cache)
    stringprep_locale_charset_cache = stringprep_locale_charset_slow ();

  return stringprep_locale_charset_cache;
}

char *
stringprep_convert (const char *str,
		    const char *to_codeset, const char *from_codeset)
{
  iconv_t cd;
  char *dest;
  char *outp;
  char *p;
  size_t inbytes_remaining;
  size_t outbytes_remaining;
  size_t err;
  size_t outbuf_size;
  int have_error = 0;
  int len;

  if (strcmp (to_codeset, from_codeset) == 0)
    return strdup (str);

  cd = iconv_open (to_codeset, from_codeset);

  if (cd == (iconv_t) - 1)
    return NULL;

  len = strlen (str);

  p = (char *) str;
  inbytes_remaining = len;
  outbuf_size = len + 1;	/* + 1 for nul in case len == 1 */

  outbytes_remaining = outbuf_size - 1;	/* -1 for nul */
  outp = dest = malloc (outbuf_size);

again:

  err = iconv (cd, &p, &inbytes_remaining, &outp, &outbytes_remaining);

  if (err == (size_t) - 1)
    {
      switch (errno)
	{
	case EINVAL:
	  /* Incomplete text, do not report an error */
	  break;

	case E2BIG:
	  {
	    size_t used = outp - dest;

	    outbuf_size *= 2;
	    dest = realloc (dest, outbuf_size);

	    outp = dest + used;
	    outbytes_remaining = outbuf_size - used - 1;	/* -1 for nul */

	    goto again;
	  }
	  break;

	case EILSEQ:
	  have_error = 1;
	  break;

	default:
	  have_error = 1;
	  break;
	}
    }

  *outp = '\0';

  if ((p - str) != len)
    have_error = 1;

  iconv_close (cd);

  if (have_error)
    {
      free (dest);
      dest = NULL;
    }

  return dest;
}

#else

const char *
stringprep_locale_charset ()
{
  return NULL;
}

char *
stringprep_convert (const char *str,
		    const char *to_codeset, const char *from_codeset)
{
  return NULL;
}

#endif

char *
stringprep_locale_to_utf8 (const char *str)
{
  return stringprep_convert (str, "UTF-8", stringprep_locale_charset ());
}
