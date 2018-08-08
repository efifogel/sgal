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
#include "SGAL/Tracer.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_parser.hpp"

DXF_BEGIN_NAMESPACE

extern "C" void BOOST_EXTENSION_EXPORT_DECL dxf_init()
{
  auto* tracer = SGAL::Tracer::get_instance();
  auto code = tracer->register_option("dxf-parsing");

  auto* loader = SGAL::Loader::get_instance();
  auto* parser = new Dxf_parser();
  parser->set_trace_code(code);
  loader->doregister_loader(".dxf", parser);

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
