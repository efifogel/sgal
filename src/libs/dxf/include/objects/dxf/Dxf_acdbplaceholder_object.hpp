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

#ifndef DXF_ACDBPLACEHOLDER_OBJECT_HPP
#define DXF_ACDBPLACEHOLDER_OBJECT_HPP

#include <map>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_base_object.hpp"

DXF_BEGIN_NAMESPACE

struct Dxf_acdbplaceholder_object : public Dxf_base_object {
  typedef Dxf_base_object                       Base;

  std::map<SGAL::String, std::vector<SGAL::String> > m_xdata;
};

DXF_END_NAMESPACE

#endif
