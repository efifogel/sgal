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
// $Id: $
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_MSC_VER)
#pragma warning ( disable : 4512 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Conf_option_parser.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Gfx_conf.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Conf_option_parser::Conf_option_parser() :
  m_conf_opts("SGAL configuration options"),
  m_accumulate(false),
  m_use_vertex_array(true),
  m_use_vertex_buffer_object(true),
  m_map_texture(true),
  m_display_fps(false)
{
  /* If the default of a specific option is set, the counter of that option
   * is set to 1
   * Don't set the default unless you realy want this default to take effect
   */
  m_conf_opts.add_options()
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

/*! Destructor */
Conf_option_parser::~Conf_option_parser() {}

/*! \brief applies the options */
void Conf_option_parser::apply(po::variables_map& /* variable_map */)
{
}

/*! \brief sets the Configuration node */
void Conf_option_parser::configure(po::variables_map& variable_map,
                                   Configuration* conf)
{
  if (!conf) return;

  if (variable_map.count("accumulate")) {
    Configuration::Shared_accumulation acc = conf->get_accumulation();
    if (acc) acc->set_enabled(variable_map["accumulate"].as<Boolean>());
  }

  if (variable_map.count("map-texture")) {
    conf->set_texture_map(variable_map["map-texture"].as<Boolean>());
  }

  if (variable_map.count("use-vertex-array")) {
    conf->set_display_fps(variable_map["use-vertex-array"].as<Boolean>());
  }

  if (variable_map.count("use-vertex-buffer-object")) {
    SGAL::Gfx_conf* gfx_conf = SGAL::Gfx_conf::get_instance();
    SGAL_assertion(gfx_conf);
    if (!variable_map["use-vertex-buffer-object"].as<Boolean>())
      gfx_conf->disable_vertex_buffer_object_support();
  }

  if (variable_map.count("fps-display")) {
    conf->set_display_fps(variable_map["fps-display"].as<Boolean>());
  }
}

SGAL_END_NAMESPACE
