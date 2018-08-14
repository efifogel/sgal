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

#ifndef DXF_HEADER_HPP
#define DXF_HEADER_HPP

#include <list>
#include <map>
#include <string>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header_variable.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header {
  //! The DXF version names
  static const std::map<size_t, std::string> s_version_names;

  /*! Determine whether the header is empty.
   */
  bool empty() const;

  /*! Clear the header.
   */
  void clear();

  std::list<Dxf_header_variable> m_variables;
};

//! \brief determines whether the header is empty.
inline bool Dxf_header::empty() const { return m_variables.empty(); }

//! \brief clears the header.
inline void Dxf_header::clear() { m_variables.clear(); }

DXF_END_NAMESPACE

#endif
