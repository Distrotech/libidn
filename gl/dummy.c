/* A dummy file, to prevent empty libraries from breaking builds.
   Copyright (C) 2004 Simon Josefsson

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License License along with this file; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA */

/* Some systems, reportedly OpenBSD and Mac OS X, refuse to create
   libraries without any object files.  You might get an error like:

   > ar cru .libs/libgl.a
   > ar: no archive members specified

   Compiling this file, and adding its object file to the library, will
   prevent the library from being empty.  */

/* This declaration is solely to ensure that after preprocessing
   this file is never empty.  */
typedef int dummy;
