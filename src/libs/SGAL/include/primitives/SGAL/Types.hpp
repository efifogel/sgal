// Copyright (c) 2004 Israel.
// All rights reserved.
//
// This file is part of SGAL; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the
// License. See the file LICENSE.LGPL distributed with SGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the
// software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TYPES_HPP
#define SGAL_TYPES_HPP

#if (defined _MSC_VER)
#pragma warning (disable: 4786)
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

// Defined to disable exception handling in STL.
// Even if compiler have exceptions enabled.
#define __STL_NO_EXCEPTIONS

// When defined SGI STL will skip string header and use our string.
#define __SGI_STL_STRING

#if defined(DEBUG)
#define SGAL_DBG(arg) arg
#define SGAL_ASSERT(Arg) assert(Arg)
#else
#define SGAL_DBG(arg)
#define SGAL_ASSERT(arg)
#endif

typedef char            Char;
typedef char            Byte;
typedef short           Short;
typedef int             Int;
typedef long            Long;
typedef unsigned char   Ubyte;
typedef unsigned char   Uchar;
typedef unsigned short  Ushort;
typedef unsigned int    Uint;
typedef unsigned long   Ulong;
typedef float           Float;
typedef double          Double;

typedef Double          Scene_time;

#if defined(_WIN32)
typedef __int64                 Double_word;
#else
typedef unsigned long long int  Double_word;
#endif

// Bool is already defined in X11/Xlib.h
typedef bool            Boolean;

SGAL_END_NAMESPACE

#endif
