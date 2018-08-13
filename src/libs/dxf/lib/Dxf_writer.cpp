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

#include <iostream>
#include <iomanip>

#include "SGAL/basic.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_writer.hpp"
#include "dxf/Dxf_data.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_header_wrapper.hpp"

DXF_BEGIN_NAMESPACE

//! \brief constructs
Dxf_writer::Dxf_writer() :
  m_data(nullptr),
  m_owned(false)
{}

//! \brief destructs.
Dxf_writer::~Dxf_writer()
{
  if (m_owned) {
    delete m_data;
    m_data = nullptr;
    m_owned = false;
  }
}

//! \brief writes a scene to an output stream in the dxf format.
void Dxf_writer::operator()(SGAL::Scene_graph* scene_graph,
                            std::ostream& os, const std::string& filename,
                            bool /* is_binary */)
{
  set_out(os);

  out() << "Writing " << filename << std::endl;

  // Populate the Dxf_data with the minimal requirements
  m_data = new Dxf_data;
  init();

  // Export
  write_header();

  export_code(0);
  export_value("SECTION");
  write_classes();

  export_code(0);
  export_value("SECTION");
  write_tables();

  export_code(0);
  export_value("SECTION");
  write_blocks();

  export_code(0);
  export_value("SECTION");
  write_entities();

  export_code(0);
  export_value("SECTION");
  write_objects();
};

//! \brief exports a given code.
void Dxf_writer::export_code(int code)
{ out() << std::setw(3) << code << std::endl; }

//! \brief writes the HEADER section.
void Dxf_writer::write_header()
{
  SGAL_assertion(m_data);
  auto& vars = m_data->m_header.m_variables;
  SGAL_assertion(! vars.empty());

  export_code(0);
  export_value("SECTION");
  export_code(2);
  export_value("HEADER");
  // my_visitor visitor(m_data);
  const auto& members = Dxf_header_wrapper::s_header_members;

  for (const auto& var : vars) {
    export_code(9);
    export_value(var.m_name);
    auto it = members.find(var.m_name);
    SGAL_assertion(it != members.end());
    const auto& codes = it->second;
    // boost::apply_visitor([&](auto&& value) {
    //     for (auto code : codes) {
    //       export_code(code);
    //       export_value(*value++);
    //     }
    //   }, var.m_value);
  }
}

//! \brief writes the CLASSES section.
void Dxf_writer::write_classes()
{
}

//! \brief writes the TABLES section.
void Dxf_writer::write_tables()
{
}

//! \brief writes the BLOCKS section.
void Dxf_writer::write_blocks()
{
}

//! \brief writes the ENTITIES section.
void Dxf_writer::write_entities()
{
}

//! \brief writes the OBJECTS section.
void Dxf_writer::write_objects()
{
}

//! \brief writes the THUMBNAILIMAGE section.
void Dxf_writer::write_thumbnailimage()
{
}

//! \brief writes the ACDSDATA section.
void Dxf_writer::write_acdsdata()
{
}

//! \brief initializes with the minimal requirements.
void Dxf_writer::init()
{
  auto& vars = m_data->m_header.m_variables;
  vars.emplace_back("$ACADVER", new std::string("AC1009"));
  vars.emplace_back("$INSBASE", new double[3]{0, 0, 0});
  vars.emplace_back("$EXTMIN", new double[3]{0, 0, 0});
  vars.emplace_back("$EXTMAX", new double[3]{100, 100, 0});
  vars.emplace_back("$UNITMODE", new int16_t(0));
  vars.emplace_back("$AUNITS", new int16_t(0));
  vars.emplace_back("$ANGBASE", new double(0));
  vars.emplace_back("$ANGDIR", new int16_t(0));
}

DXF_END_NAMESPACE
