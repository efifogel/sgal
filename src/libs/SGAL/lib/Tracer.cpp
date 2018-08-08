// Copyright (c) 2004 Israel.
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

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Tracer.hpp"

SGAL_BEGIN_NAMESPACE

//! The trace singleton.
Tracer* Tracer::s_instance(nullptr);

//! \brief obtains a trace singleton.
Tracer* Tracer::get_instance()
{
  if (!s_instance) s_instance = new Tracer();
  return s_instance;
}

//! \brief constructs.
Tracer::Tracer() :
  m_signature(0x0)
{
  // Trace options.
  Map options = {
    {"", INVALID},
    {"graphics", GRAPHICS},
    {"vrml-parsing", VRML_PARSING},
    {"window-manager", WINDOW_MANAGER},
    {"events", EVENTS},
    {"script", SCRIPT},
    {"ifs", INDEXED_FACE_SET},
    {"ils", INDEXED_LINE_SET},
    {"polyhedron", POLYHEDRON},
    {"cgm", CUBICAL_GAUSSIAN_MAP},
    {"destructor", DESTRUCTOR},
    {"snapshot", SNAPSHOT},
    {"export", EXPORT},
    {"font", FONT},
    {"proto", PROTO}
  };
  set_options(std::move(options));
}

SGAL_END_NAMESPACE
