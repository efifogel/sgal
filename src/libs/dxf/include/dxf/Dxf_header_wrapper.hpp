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

#ifndef DXF_HEADER_WRAPPER_HPP
#define DXF_HEADER_WRAPPER_HPP

#include <list>
#include <map>
#include <string>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header_full.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header_wrapper {
  static const std::map<std::string, std::list<int> > s_header_members;
};

DXF_END_NAMESPACE

#endif
