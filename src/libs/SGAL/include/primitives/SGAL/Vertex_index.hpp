// Copyright (c) 2018 Israel.
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

#ifndef SCGAL_VERTEX_INDEX_HPP
#define SCGAL_VERTEX_INDEX_HPP

#include "SGAL/basic.hpp"

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

struct Vertex_index {
  Vertex_index() : m_index(static_cast<size_t>(-1)) {}
  size_t m_index;
};

SGAL_END_NAMESPACE

#endif
