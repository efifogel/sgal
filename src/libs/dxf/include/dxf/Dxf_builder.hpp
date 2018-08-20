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

#ifndef DXF_BUILDER_HPP
#define DXF_BUILDER_HPP

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Group;

SGAL_END_NAMESPACE

DXF_BEGIN_NAMESPACE

class Dxf_data;

class SGAL_SGAL_DECL Dxf_builder {
public:
  /*! Construct.
   */
  Dxf_builder(Dxf_data& data);

  /*! Build from the root.
   */
  void operator()(SGAL::Group* root);

protected:
  //! The trace code.
  size_t m_trace_code_building;
};

DXF_END_NAMESPACE

#endif
