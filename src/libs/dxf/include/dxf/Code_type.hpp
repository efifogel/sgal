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

#ifndef DXF_CODE_TYPE_HPP
#define DXF_CODE_TYPE_HPP

#include <string>
#include <array>
#include <vector>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

//! Enumerations of types of code numbers.
enum class Code_type { STRING, DOUBLE, INT8, INT16, INT32, UINT, BOOL };

//! Represents a range of code numbers associated with a particular type.
struct Code_range {
  Code_range(int min, int max, Code_type type) :
    m_min(min), m_max(max), m_type(type)
  {}

  int m_min;
  int m_max;
  Code_type m_type;
};

/*! Obtain the type of a code
 * \param code the given code.
 */
Code_type code_type(int code);

/*! Obtain the name of a code.
 */
const std::string& code_type_name(Code_type code_type);

DXF_END_NAMESPACE

#endif
