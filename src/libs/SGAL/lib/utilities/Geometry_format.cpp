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

#include "SGAL/basic.hpp"
#include "SGAL/Geometry_format.hpp"

SGAL_BEGIN_NAMESPACE

//! The singleton.
Geometry_format* Geometry_format::s_instance(nullptr);

//! \brief obtains the singleton.
Geometry_format* Geometry_format::get_instance()
{
  if (!s_instance) s_instance = new Geometry_format();
  return s_instance;
}

//! \brief constructs.
Geometry_format::Geometry_format()
{
  // Geometry format options.
  Map options = {
    {"", INVALID},
    {"wrl", WRL},
#if 0
    {"x3d", X3D},
#endif
    {"off", OFF},
    {"stl", STL},
    {"obj", OBJ},
    {"json", JSON}
  };
  set_options(std::move(options));
};

SGAL_END_NAMESPACE
