/* stringprep_nameprep.h	nameprep stringprep profile    -*- c -*-
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

#ifndef _STRINGPREP_NAMEPREP_H
#define _STRINGPREP_NAMEPREP_H

#ifdef __cplusplus
extern "C"
{
#if 0 /* keep Emacsens's auto-indent happy */
}
#endif
#endif

#include <stringprep.h>

extern struct stringprep_table_element stringprep_generic_B_1[];
extern struct stringprep_table_element stringprep_generic_B_2[];
extern struct stringprep_table_element stringprep_generic_C_1_2[];
extern struct stringprep_table_element stringprep_generic_C_2_2[];
extern struct stringprep_table_element stringprep_generic_C_3[];
extern struct stringprep_table_element stringprep_generic_C_4[];
extern struct stringprep_table_element stringprep_generic_C_5[];
extern struct stringprep_table_element stringprep_generic_C_6[];
extern struct stringprep_table_element stringprep_generic_C_7[];
extern struct stringprep_table_element stringprep_generic_C_8[];
extern struct stringprep_table_element stringprep_generic_C_9[];
extern struct stringprep_table_element stringprep_generic_D_1[];
extern struct stringprep_table_element stringprep_generic_D_2[];

struct stringprep_table stringprep_nameprep[] = {
  { 0, stringprep_generic_B_1 },
  { ~STRINGPREP_NO_NFKC, stringprep_generic_B_2 },
  { ~STRINGPREP_NO_NFKC },
  { 0, stringprep_generic_C_1_2 },
  { 0, stringprep_generic_C_2_2 },
  { 0, stringprep_generic_C_3 },
  { 0, stringprep_generic_C_4 },
  { 0, stringprep_generic_C_5 },
  { 0, stringprep_generic_C_6 },
  { 0, stringprep_generic_C_7 },
  { ~STRINGPREP_NO_BIDI & STRINGPREP_BIDI_PROHIBITED, stringprep_generic_C_8 },
  { 0, stringprep_generic_C_9 },
  { ~STRINGPREP_NO_BIDI & STRINGPREP_BIDI_RAL, stringprep_generic_D_1 },
  { ~STRINGPREP_NO_BIDI & STRINGPREP_BIDI_L, stringprep_generic_D_2 },
  { 0 }
};

#define stringprep_nameprep(in, maxlen) stringprep(in, maxlen, 0, stringprep_nameprep)

#ifdef __cplusplus
}
#endif
#endif				/* _STRINGPREP_NAMEPREP_H */
