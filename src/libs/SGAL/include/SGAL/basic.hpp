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

#ifndef SGAL_BASIC_HPP
#define SGAL_BASIC_HPP

#include "SGAL/config.hpp"

#include <iostream>
#include <cstdlib>

// Big endian or little endian machine.
// ====================================
#ifdef SGAL_CFG_NO_BIG_ENDIAN
#  define SGAL_LITTLE_ENDIAN 1
#else
#  define SGAL_BIG_ENDIAN 1
#endif

#include "SGAL/config.hpp"
#include "SGAL/assertions.hpp"

// Symbolic constants to tailor inlining. Inlining Policy.
// =======================================================
#ifndef SGAL_MEDIUM_INLINE
#  define SGAL_MEDIUM_INLINE inline
#endif

#ifndef SGAL_LARGE_INLINE
#  define SGAL_LARGE_INLINE
#endif

#ifndef SGAL_HUGE_INLINE
#  define SGAL_HUGE_INLINE
#endif

#endif
