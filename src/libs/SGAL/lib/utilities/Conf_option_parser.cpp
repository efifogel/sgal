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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Conf_option_parser.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Conf_option_parser::Conf_option_parser() :
  m_conf_opts("Viewing options"),
  m_accumulate(false),
  m_use_vertex_array(true),
  m_use_vertex_buffer_object(true),
  m_map_texture(true),
  m_display_fps(false)
{
  typedef std::vector<std::string> vs;

  /* If the default of a specific option is set, the counter of that option
   * is set to 1
   * Don't set the default unless you realy want this default to take effect
   */
  m_conf_opts.add_options()
    ("viewpoint", po::value<Configuration::Viewpoint_mode>(),
     "viewpoint options\n"
     "  viewing\n"
     "  modeling\n"
     )
    ("accumulate,A", po::value<Boolean>(&m_accumulate), "accumulate")
    //("accumulation-delay")
    //("ad")
    //("accumulation-quality")
    //("aq")
    ("use-vertex-array", po::value<Boolean>(&m_use_vertex_array),
     "use vertex array")
    ("va", po::value<Boolean>(&m_use_vertex_array), "use vertex array")
    ("use-vertex-buffer-object",
     po::value<Boolean>(&m_use_vertex_buffer_object),
     "use vertex buffer object")
    ("vbo", po::value<Boolean>(&m_use_vertex_buffer_object),
     "use vertex buffer object")
    ("map-texture", po::value<Boolean>(&m_map_texture), "map texture")
    ("tm", po::value<Boolean>(&m_map_texture),"map texture")
    ("fps-display", po::value<Boolean>(&m_display_fps), "display FPS")
    ("dfps", po::value<Boolean>(&m_display_fps), "display FPS")
    ;
}

//! \brief destructs.
Conf_option_parser::~Conf_option_parser() {}

//! \brief applies the options.
void Conf_option_parser::apply() {}

//! \brief sets the Configuration node.
void Conf_option_parser::configure(Configuration* conf)
{
  if (!conf) return;

  const auto& var_map = get_variable_map();

  if (var_map.count("accumulate")) {
    auto acc = conf->get_accumulation();
    if (!acc) {
      acc.reset(new Accumulation);
      SGAL_assertion(acc);
      conf->set_accumulation(acc);
    }
    acc->set_enabled(var_map["accumulate"].as<Boolean>());
  }

  if (var_map.count("map-texture"))
    conf->set_texture_map(var_map["map-texture"].as<Boolean>());

  if (var_map.count("use-vertex-array"))
    if (!var_map["use-vertex-array"].as<Boolean>())
      conf->set_geometry_drawing_mode(Configuration::GDM_DIRECT);

  if (var_map.count("use-vertex-buffer-object"))
    conf->set_use_vertex_buffer_object
      (var_map["use-vertex-buffer-object"].as<Boolean>());

  if (var_map.count("fps-display"))
    conf->set_display_fps(var_map["fps-display"].as<Boolean>());

  if (var_map.count("viewpoint"))
    conf->set_viewpoint_mode(var_map["viewpoint"].
                             as<Configuration::Viewpoint_mode>());
}

SGAL_END_NAMESPACE
