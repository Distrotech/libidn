/* profiles.c	definitions of stringprep profiles shipped with libstringprep
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

#include "stringprep_generic.h"
#include "stringprep_nameprep.h"
#include "stringprep_kerberos5.h"

struct stringprep_table stringprep_generic[] = {
  { 0, stringprep_generic_B_1 },
  { ~STRINGPREP_NO_NFKC, stringprep_generic_B_2 },
  { STRINGPREP_NO_NFKC, stringprep_generic_B_3 },
  { ~STRINGPREP_NO_NFKC },
  { 0, stringprep_generic_C_1_1 },
  { 0, stringprep_generic_C_1_2 },
  { 0, stringprep_generic_C_2_1 },
  { 0, stringprep_generic_C_2_2 },
  { 0, stringprep_generic_C_3 },
  { 0, stringprep_generic_C_4 },
  { 0, stringprep_generic_C_5 },
  { 0, stringprep_generic_C_6 },
  { 0, stringprep_generic_C_7 },
  { STRINGPREP_BIDI_PROHIBITED_MASK, stringprep_generic_C_8 },
  { 0, stringprep_generic_C_9 },
  { STRINGPREP_BIDI_RAL_MASK, stringprep_generic_D_1 },
  { STRINGPREP_BIDI_L_MASK, stringprep_generic_D_2 },
  { 0 }
};

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

struct stringprep_table stringprep_kerberos5[] = {
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
  { 0, stringprep_generic_C_9 },
  { 0 }
};
