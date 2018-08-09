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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef DXF_HEADER_VARIABLE_HPP
#define DXF_HEADER_VARIABLE_HPP

#include <cstdint>
#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header_variable {
  typedef boost::variant<std::string*,
                         double*,
                         //double[2]*,
                         //double[3]*,
                         int8_t*,
                         int16_t*,
                         int32_t*,
                         SGAL::Uint*,
                         bool*>         Header_variable_data;

  std::string m_name;
  Header_variable_data m_data;
};

DXF_END_NAMESPACE

#endif
