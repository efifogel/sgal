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

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_writer.hpp"

DXF_BEGIN_NAMESPACE

//! \brief writes a scene to an output stream.
void Dxf_writer::operator()(SGAL::Scene_graph* scene_graph,
                            std::ostream& os, const std::string& filename,
                            bool /* is_binary */)
{
  set_out(os);

  out() << "Writing " << filename << std::endl;

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_header();

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_classes();

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_tables();

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_blocks();

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_entities();

  out() << "0" << std::endl;
  out() << "SECTION" << std::endl;
  write_objects();
};

//! \brief writes the HEADER section.
void Dxf_writer::write_header() {}

//! \brief writes the CLASSES section.
void Dxf_writer::write_classes() {}

//! \brief writes the TABLES section.
void Dxf_writer::write_tables() {}

//! \brief writes the BLOCKS section.
void Dxf_writer::write_blocks() {}

//! \brief writes the ENTITIES section.
void Dxf_writer::write_entities() {}

//! \brief writes the OBJECTS section.
void Dxf_writer::write_objects() {}

DXF_END_NAMESPACE
