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

#include <boost/extension/extension.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Writer.hpp"
#include "SGAL/Tracer.hpp"
#include "SGAL/Geometry_format.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"
#include "dxf/Dxf_writer.hpp"

DXF_BEGIN_NAMESPACE

extern "C" void BOOST_EXTENSION_EXPORT_DECL dxf_init()
{
  auto* tracer = SGAL::Tracer::get_instance();
  auto trace_code_parsing = tracer->register_option("dxf-parsing");
  auto trace_code_building = tracer->register_option("dxf-building");

  auto* geom_format = SGAL::Geometry_format::get_instance();
  auto format_code = geom_format->register_option("dxf");

  auto* loader = SGAL::Loader::get_instance();
  auto* dxf_parser = new Dxf_parser();
  dxf_parser->set_trace_code_parsing(trace_code_parsing);
  dxf_parser->set_trace_code_building(trace_code_building);
  loader->doregister(".dxf", dxf_parser);

  auto* writer = SGAL::Writer::get_instance();
  auto* dxf_writter = new Dxf_writer;
  writer->doregister(format_code, dxf_writter);

  //! \todo Need a method (API) to add stuff to the command-line options by a
  // Dynamically Loaded Library (DLL).
  // In particular, add the "dxf-parsing" trace option.
  // Observe, that in the current flow, the DLLs are loaded only after the
  // command-line options are processed. If the '--help' option is entered by
  // the user, an exception is thrown and the DLLs are not loaded at all. This
  // need to change to enable loading first, then adding the command-line
  // options of the DLLs, and only then print the help message and exit.
}

DXF_END_NAMESPACE
