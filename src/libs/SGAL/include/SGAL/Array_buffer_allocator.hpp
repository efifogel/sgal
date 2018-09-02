// Copyright (c) 2014 Israel.
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

#ifndef SGAL_ARRAY_BUFFER_ALLOCATOR_HPP
#define SGAL_ARRAY_BUFFER_ALLOCATOR_HPP

// This is an obsolete file that is no longer in use. It stopped being in use
// since the switch to V8 6.6. The new version provides a default allocator.

#if defined(SGAL_USE_V8)
#include <v8.h>
#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! The allocator below used to be used by a certain version of V8.
 * Why doesn't it have a default? beats me.
 */
class Array_buffer_allocator : public v8::ArrayBuffer::Allocator {
public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

SGAL_END_NAMESPACE

#endif
