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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GL_WRAPPER_HPP
#define SGAL_GL_WRAPPER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_mapper.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(NDEBUG)
#define SGAL_CHECK_GL()
#else
#define SGAL_CHECK_GL() Gl_mapper::check()
#endif

#include "SGAL/gl_trapper.hpp"

SGAL_END_NAMESPACE

#endif
