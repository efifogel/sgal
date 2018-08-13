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

#ifndef DXF_HEADER_VARIABLE_HPP
#define DXF_HEADER_VARIABLE_HPP

#include <cstdint>
#include <list>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header_variable {
  typedef boost::variant<std::string*,
                         double*,
                         int8_t*,
                         int16_t*,
                         int32_t*,
                         SGAL::Uint*,
                         bool*>         Header_variable_value;

  /// Constructors
  //@{

  Dxf_header_variable(const std::string& name, std::string* value);
  Dxf_header_variable(const std::string& name, double* value);
  Dxf_header_variable(const std::string& name, int8_t* value);
  Dxf_header_variable(const std::string& name, int16_t* value);
  Dxf_header_variable(const std::string& name, int32_t* value);
  Dxf_header_variable(const std::string& name, SGAL::Uint* value);
  Dxf_header_variable(const std::string& name, bool* value);

  //@}

  std::string m_name;
  Header_variable_value m_value;
  size_t m_num;
};

//! \brief constructs from string
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                std::string* value) :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from double
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                double* value)  :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from int8_t
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                int8_t* value)  :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from int16_t
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                int16_t* value)  :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from int32_t
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                int32_t* value)  :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from Uint
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                SGAL::Uint* value)  :
  m_name(name),
  m_value(value)
{}

//! \brief constructs from bool
inline Dxf_header_variable::Dxf_header_variable(const std::string& name,
                                                bool* value)  :
  m_name(name),
  m_value(value)
{}

DXF_END_NAMESPACE

#endif
