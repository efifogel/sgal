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

#include <boost/extension/extension.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Trace.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"

DXF_BEGIN_NAMESPACE

extern "C" void BOOST_EXTENSION_EXPORT_DECL dxf_init()
{
  auto* trace = SGAL::Trace::get_instance();
  auto code = trace->doregister_trace_opt("dxf-parsing");

  auto* loader = SGAL::Loader::get_instance();
  auto* parser = new Dxf_parser();
  parser->set_trace_code(code);
  loader->doregister_loader(".dxf", parser);
}

DXF_END_NAMESPACE
