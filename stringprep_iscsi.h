/* stringprep_iscsi.h	IPS iSCSI stringprep profile.
 * Copyright (C) 2002, 2003  Simon Josefsson
 *
 * This file is part of GNU Libidn.
 *
 * GNU Libidn is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * GNU Libidn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GNU Libidn; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _STRINGPREP_ISCSI_H
#define _STRINGPREP_ISCSI_H

#ifdef __cplusplus
extern "C"
{
#if 0				/* keep Emacsens's auto-indent happy */
}
#endif
#endif

#include <stringprep.h>

extern Stringprep_profile stringprep_iscsi[];

#define stringprep_iscsi(in, maxlen) \
	stringprep(in, maxlen, 0, stringprep_iscsi)

#ifdef __cplusplus
}
#endif
#endif /* _STRINGPREP_ISCSI_H */
