// Copyright (c) 2015 Israel.
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

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include <vector>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_option_parser.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Dxf_configuration.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/io_vector4f.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs default.
Dxf_option_parser::Dxf_option_parser() :
  m_dxf_opts("Dxf options")
{
  m_dxf_opts.add_options()
    ("dxf-palette-file",
     po::value<String>()->
     default_value(Dxf_configuration::s_def_palette_file_name),
     "DXF color palette file name")
    ("dxf-background-color", po::value<Vector4f>()->
     default_value(Dxf_configuration::s_def_background_color),
     "Background color")
    ("dxf-min-bulge", po::value<Float>()->
     default_value(Dxf_configuration::s_def_min_bulge),
     "DXF Minimum bulge value (used for approximating a circular arc)")
    ("dxf-refinement-arcs-num", po::value<Uint>()->
     default_value(Dxf_configuration::s_def_refinement_arcs_num),
     "DXF number of arcs used to represent a circle or an ellipsoid")
   ;
}

//! \brief destructs.
Dxf_option_parser::~Dxf_option_parser() {}

//! \brief applies the options.
void Dxf_option_parser::apply() {}

//! \brief sets the Configuration node.
void Dxf_option_parser::configure(Configuration* conf)
{
  if (!conf) return;

  const auto& var_map = get_variable_map();

  auto dxf = conf->get_dxf_configuration();
  if (!dxf) {
    dxf.reset(new Dxf_configuration);
    SGAL_assertion(dxf);
    conf->set_dxf_configuration(dxf);
  }

  dxf->set_palette_file_name(var_map["dxf-palette-file"].as<String>());
  dxf->set_background_color(var_map["dxf-background-color"].as<Vector4f>());
  dxf->set_min_bulge(var_map["dxf-min-bulge"].as<Float>());
  dxf->set_refinement_arcs_num(var_map["dxf-refinement-arcs-num"].as<Uint>());
}

SGAL_END_NAMESPACE
