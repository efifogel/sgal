// Copyright (c) 2004  Israel.
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
// $Source$
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 */

#if defined(_WIN32)
#pragma warning(disable: 4146)
#pragma warning(disable: 4503)
#endif

#include <CGAL/basic.h>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "SCGAL/Cubical_gaussian_map_geo.hpp"
#include "SCGAL/Spherical_gaussian_map_base_geo.hpp"

#include "SGAL/Camera.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Redraw_event.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Switch.hpp"

#if (defined USE_GLUT)
#include "SGLUT/Glut_window_item.hpp"
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_item.hpp"
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_item.hpp"
#include "SGAL/X11_window_manager.hpp"
#endif

#include "Player2_scene.hpp"
#include "Player2_option_parser.hpp"

/*! Constructor */
Player2_scene::Player2_scene(Player2_option_parser * option_parser) :
  Player_scene(option_parser),
  m_option_parser(option_parser),
  m_cgm_geo(NULL),
  m_sgm_geo(NULL)
{}

/*! Destructor */
Player2_scene::~Player2_scene(void) {}

/*! \brief initializes the secene */
void Player2_scene::init_scene()
{
  Player_scene::init_scene();

  m_cgm_geo = dynamic_cast<SGAL::Cubical_gaussian_map_geo*>
    (m_scene_graph->get_container("GEOM"));

  m_sgm_geo = dynamic_cast<SGAL::Spherical_gaussian_map_base_geo*>
    (m_scene_graph->get_container("GEOM"));
}

/*! Draw the scene */
void Player2_scene::draw_window(SGAL::Window_item * window_item,
                                SGAL::Boolean dont_accumulate)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration * conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  
  SGAL::Draw_action draw_action(conf);
  draw_action.set_context(m_context);
  draw_action.set_snap(false);
  draw_action.set_clear(0);

  SGAL_assertion(conf);
  
  SGAL::Uint width = m_win_width;
  SGAL::Uint height = m_win_height;
  SGAL::Uint hwidth = width / 2;
  float scale = m_option_parser->get_viewport_scale();
  int sw = (int) (scale * hwidth);
  int sh = (int) (scale * height);
  int x = (int) ((scale - 1) / 2 * hwidth);
  int y = (int) ((scale - 1) / 2 * height);
  int margin = m_option_parser->get_viewport_margin();
  
  SGAL::Camera * camera = m_scene_graph->get_active_camera();
  SGAL_assertion(camera);

  // Clear the entire frame buffer:
  m_context->set_viewport(0, 0, width, height);  
  camera->init(m_context);
  SGAL::Background * bg = m_scene_graph->get_active_background();
  if (bg) {
    bg->draw(&draw_action);
    m_context->clear_stencil_buffer();
  } else {
    m_context->clear_color_depth_stencil_buffer();
  }

  SGAL::Accumulation * acc = conf->get_accumulation();
  bool enabled = false;
  if (acc) enabled = acc->is_enabled();
 
  // Draw 1st viewport
  if (acc) acc->set_enabled(false);
  m_context->set_viewport(-x + margin, -y, sw, sh);
  camera->init(m_context);
  if (m_cgm_geo) m_cgm_geo->set_draw_aos(false);
  if (m_sgm_geo) m_sgm_geo->set_draw_aos(false);
  m_scene_graph->draw(&draw_action);

  // Draw 2nd viewport
  if (acc) acc->set_enabled(enabled);
  m_context->set_viewport(hwidth - x - margin, -y, sw, sh);  
  camera->init(m_context);
  if (m_cgm_geo) m_cgm_geo->set_draw_aos(true);
  if (m_sgm_geo) m_sgm_geo->set_draw_aos(true);
  m_scene_graph->draw(&draw_action);

  // Draw grid
  m_context->set_viewport(0, 0, width, height);  
  if (m_option_parser->get_draw_grid()) draw_grid();
  
  if (!acc || !acc->is_enabled() || dont_accumulate) {
    m_scene_graph->process_snapshots(&draw_action);
    if (acc) acc->disactivate();
    window_item->set_accumulating(false);
    // m_context->swap_buffers();
    window_item->swap_buffers();
    return;
  }
  
  if (!acc->is_active()) {
    acc->enactivate();
    window_item->set_accumulating(SGAL_TRUE);
    window_item->set_redraw(SGAL_TRUE);
    return;
  }

  // Accumulation is active:
  if (acc->is_done()) {
    // Accumulation is done:
    acc->disactivate();
    m_scene_graph->process_snapshots(&draw_action);
    window_item->set_accumulating(SGAL_FALSE);
    window_item->swap_buffers();
    return;
  }

  // Accumulation is not done:
  window_item->set_redraw(SGAL_TRUE);
  if (acc->do_show()) window_item->swap_buffers();
}
