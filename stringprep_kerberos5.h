/* stringprep_kerberos5.h	kerberos5 stringprep profile    -*- c -*-
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

#ifndef _STRINGPREP_KERBEROS5_H
#define _STRINGPREP_KERBEROS5_H

#ifdef __cplusplus
extern "C"
{
#if 0 /* keep Emacsens's auto-indent happy */
}
#endif
#endif

#include <stringprep.h>

extern struct stringprep_table stringprep_kerberos5[];

#define stringprep_kerberos5(in, maxlen) \
	stringprep(in, maxlen, 0, stringprep_kerberos5)

#ifdef __cplusplus
}
#endif
#endif				/* _STRINGPREP_KERBEROS5_H */
