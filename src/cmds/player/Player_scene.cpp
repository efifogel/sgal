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
// $Id: Player_scene.cpp 13681 2012-07-04 15:39:29Z efif $
// $Revision: 13681 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(PLAYER_CONTROL)
#include "stdafx.h"
#endif

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4800 )
#pragma warning ( disable : 4503 )
#endif

#include <stdlib.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Event_handler.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Reshape_event.hpp"
#include "SGAL/Draw_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Simulate_event.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Loader.hpp"
#include "SGAL/Navigation_info.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Coord_transformer.hpp"
#include "SGAL/Route.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Single_key_sensor.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Context.hpp"

#if (defined USE_GLUT)
#include "SGLUT/Glut_window_item.hpp"
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#if defined(PLAYER_CONTROL)
#include "PlayerControlCtrl.h"
#include "SGAL/Windows_window_item.hpp"
#else
#include "SGAL/Windows_window_item.hpp"
#include "SGAL/Windows_window_manager.hpp"
#endif
#else
#include "SGAL/X11_window_item.hpp"
#include "SGAL/X11_window_manager.hpp"
#endif

#if defined(USE_CGAL)
#include "CGAL/basic.h"

#include "SCGAL/Exact_polyhedron_geo.hpp"
#if defined(USE_CGM)
#include "SCGAL/Cubical_gaussian_map_geo.hpp"
#endif
#if defined(USE_SGM)
#include "SCGAL/Spherical_gaussian_map_geo.hpp"
#endif
#if defined(USE_AOS)
#include "SCGAL/Arrangement_on_sphere_geo.hpp"
#endif
#if defined(USE_EOS)
#include "SCGAL/Envelope_on_sphere_geo.hpp"
#endif
#if defined(USE_NEF) && defined(USE_NGM)
#include "SCGAL/Nef_gaussian_map_geo.hpp"
#endif
#endif

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"

/*! Valid file format names */
const char* Player_scene::s_file_format_names[] = { "wrl" };

/*! Constructor */
Player_scene::Player_scene(Player_option_parser* option_parser) :
  m_window_manager(NULL),
  m_window_item(NULL),
  m_win_width(0), m_win_height(0),
  m_scene_graph(NULL),
  m_context(NULL),
  m_option_parser(option_parser),
  m_simulate(false)  
{ init(); }

/*! Constructor */
Player_scene::Player_scene() :
  m_window_manager(NULL),
  m_window_item(NULL),
  m_win_width(0), m_win_height(0),
  m_scene_graph(NULL),
  m_context(NULL),
  m_simulate(false)  
{ init(); }

/*! \brief initializes. */
void Player_scene::init()
{
  m_dirs.push_front(".");

  const char* root = getenv("ROOT");
  if (root) {
    std::string root_str(root);
#if (defined _MSC_VER)
    // Convert the ROOT from cygwin path to windows path, if relevant:
    std::string cygdrive = root_str.substr(0, 10);
    if (cygdrive == std::string("/cygdrive/")) {
      root_str.erase(0, 10);
      root_str.insert(1, ":");
    }
#endif
#if BOOST_VERSION >= 103400
    fi::path root_path(root_str);
#else
    fi::path root_path(root_str, fi::native);
#endif
    root_path /= "data";
    root_path /= "vrml";
    m_dirs.push_back(root_path);
  }

  SGAL::Tick_event::doregister(this);
  SGAL::Keyboard_event::doregister(this);
  SGAL::Draw_event::doregister(this);
  SGAL::Reshape_event::doregister(this);
  SGAL::Motion_event::doregister(this);
  SGAL::Passive_motion_event::doregister(this);
  SGAL::Mouse_event::doregister(this);
}

/*! Destructor */
Player_scene::~Player_scene(void)
{
  SGAL::Reshape_event::unregister(this);
  SGAL::Draw_event::unregister(this);
  SGAL::Keyboard_event::unregister(this);
  SGAL::Tick_event::unregister(this);
  SGAL::Motion_event::unregister(this);
  SGAL::Passive_motion_event::unregister(this);
  SGAL::Mouse_event::unregister(this);
  m_fullname.clear();
}

/*! \brief creates the scene. */
void Player_scene::create_scene()
{
  std::string filename;
  if (!m_option_parser->get_file_name(filename) || filename.empty()) {
    std::string str("input file missing!");
    throw Input_file_missing_error(str);
    return;
  }

#if (defined _MSC_VER)
  // Convert the ROOT from cygwin path to windows path, if relevant:
  std::string cygdrive = filename.substr(0, 10);
  if (cygdrive == std::string("/cygdrive/")) {
    filename.erase(0, 10);
    filename.insert(1, ":");
  }
#endif
  
#if BOOST_VERSION >= 103400
  fi::path file_path(filename);
#else
  fi::path file_path(filename, fi::native);
#endif  
  if (file_path.is_complete()) {
#if BOOST_VERSION >= 103400
    if (fi::exists(file_path)) m_fullname = file_path.string();
#else
    if (fi::exists(file_path)) m_fullname = file_path.native_file_string();
#endif
  } else {
    m_option_parser->for_each_dir(Add_dir(m_dirs));
    for (Path_iter pi = m_dirs.begin(); pi != m_dirs.end(); ++pi) {
      fi::path full_file_path = *pi / file_path;
      if (!fi::exists(full_file_path)) continue;
#if BOOST_VERSION >= 103400
      m_fullname = full_file_path.string();
#else
      m_fullname = full_file_path.native_file_string();
#endif
      break;
    }
  }
  if (m_fullname.empty()) {
    throw Illegal_input(FILE_NOT_FOUND, "cannot find file", filename);
    return;
  }
  
  std::cout << m_fullname.c_str() << std::endl; 

  // Construct a Scene_graph:
  m_scene_graph = new SGAL::Scene_graph;
  SGAL_assertion(m_scene_graph);
  
  update_data_dirs();
}

/*! \brief destroys the scene. */
void Player_scene::destroy_scene()
{
  if (m_context) {
    delete m_context;
    m_context = NULL;
  }

  if (m_scene_graph) {
    delete m_scene_graph;
    m_scene_graph = NULL;
  }
}
  
/*! \brief initializes the secene. */
void Player_scene::init_scene()
{
  // Obtain the input file full name.
  std::string filename;
  if (!m_option_parser->get_file_name(filename) || filename.empty()) {
    std::string str("input file missing!");
    throw Input_file_missing_error(str);
    return;
  }

  // Load the input file.
  SGAL::Loader loader;
  int rc = loader.load(m_fullname.c_str(), m_scene_graph);
  if (rc < 0) {
    throw Illegal_input(UNABLE_TO_LOAD, "Cannot load file", filename);
    return;
  }
  print_stat();

  // Create the missing nodes.
  m_scene_graph->create_defaults();
    
  // Prepare the window item.
  m_window_item = new Window_item;
  m_window_item->set_title(filename);
  m_window_item->set_number_of_stencil_bits(1);

  // Update the configuration node.
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Accumulation* acc = conf->get_accumulation();
  SGAL::Multisample* ms = conf->get_multisample();

  if (acc) {
    SGAL::Uint red_bits, green_bits, blue_bits, alpha_bits;
    acc->get_number_of_bits(red_bits, green_bits, blue_bits, alpha_bits);
    m_window_item->set_number_of_accumulation_bits(red_bits, green_bits,
                                                   blue_bits, alpha_bits);
  }
  if (ms) m_window_item->set_number_of_samples(ms->get_number_of_samples());
  m_window_item->set_number_of_stencil_bits(conf->get_number_of_stencil_bits());
  m_window_item->set_number_of_depth_bits(conf->get_number_of_depth_bits());
  m_window_manager->create_window(m_window_item);
  if (ms) ms->set_number_of_samples(m_window_item->get_number_of_samples());
  if (acc) {
    SGAL::Uint red_bits, green_bits, blue_bits, alpha_bits;
    m_window_item->get_number_of_accumulation_bits(red_bits, green_bits,
                                                   blue_bits, alpha_bits);
    acc->set_number_of_bits(red_bits, green_bits, blue_bits, alpha_bits);
  }

  indulge_user();

  // Construct the context.
  m_context = new SGAL::Context();
  SGAL_assertion(m_context);
  m_scene_graph->set_context(m_context);
  m_scene_graph->init_context();
  m_scene_graph->start_simulation();
  m_scene_graph->bind();
  m_window_item->show();
}

/*! \brief indulges user requests from the command line. */
void Player_scene::indulge_user()
{
  m_option_parser->configure(m_scene_graph);
  
  // Local options:
  if (m_option_parser->get_display_texture_info()) {
    SGAL_assertion(m_scene_graph);
    // Look for non instance containers:
    SGAL::Scene_graph::Container_vector_iter ci;
    for (ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      SGAL::Container* cont = *ci;
      SGAL::Texture_2d* texture = dynamic_cast<SGAL::Texture_2d*>(cont);
      if (texture) texture->print_info();
    }
    
    // Look for instance containers:
    SGAL::Scene_graph::Container_map_iter ii;
    for (ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      SGAL::Container* cont = (*ii).second;
      SGAL::Texture_2d* texture = dynamic_cast<SGAL::Texture_2d*>(cont);
      if (texture) texture->print_info();
    }
  }

  if (m_option_parser->get_display_geometry_info()) {
    // Look for non instance containers:
    SGAL::Scene_graph::Container_vector_iter ci;
    for (ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      SGAL::Container* cont = *ci;
      // std::cout << "Tag 1: " << cont->get_tag().c_str() << std::endl;
      SGAL::Indexed_face_set* ifs = dynamic_cast<SGAL::Indexed_face_set*>(cont);
      if (ifs) print_geometry_info(ifs);
      SGAL::Box* box = dynamic_cast<SGAL::Box*>(cont);
      if (box) print_geometry_info(box);
    }

    // Look for instance containers:
    SGAL::Scene_graph::Container_map_iter ii;
    for (ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      SGAL::Container* cont = (*ii).second;
      // std::cout << "Tag 2: " << cont->get_tag().c_str() << std::endl;
      SGAL::Indexed_face_set* ifs = dynamic_cast<SGAL::Indexed_face_set*>(cont);
      if (ifs) print_geometry_info(ifs);
      SGAL::Box* box = dynamic_cast<SGAL::Box*>(cont);
      if (box) print_geometry_info(box);
    }
  }

#if 0
  if (m_option_parser->get_bench()) {
    m_window_manager->set_benching(true);
    m_window_manager->set_num_samples(m_option_parser->get_samples());
  }
#endif
  
#if 0
  SGAL::Uint sub_index_buffer_size;
  if (m_option_parser->get_sub_index_buffer_size(sub_index_buffer_size)) {
    SGAL_assertion(m_scene_graph);
    // Look for non instance containers:
    SGAL::Scene_graph::Container_vector_iter ci;
    for (ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      SGAL::Container* cont = *ci;
      // std::cout << "Tag 1: " << cont->get_tag().c_str() << std::endl;
      SGAL::Geo_set* ifs = dynamic_cast<SGAL::Geo_set*>(cont);
      if (ifs) ifs->set_sub_index_array_size(sub_index_buffer_size);
    }

    // Look for instance containers:
    SGAL::Scene_graph::Container_map_iter ii;
    for (ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      SGAL::Container* cont = (*ii).second;
      // std::cout << "Tag 2: " << cont->get_tag().c_str() << std::endl;
      SGAL::Geo_set* ifs = dynamic_cast<SGAL::Geo_set*>(cont);
      if (ifs) ifs->set_sub_index_array_size(sub_index_buffer_size);
    }
  }
#endif
}

/*! \brief print geometry information of Box */
void Player_scene::print_geometry_info(SGAL::Box* box)
{
  std::cout << "Geometry: Box" << std::endl;  
}

/*! \brief prints texture information of Index_face_set. */
void Player_scene::print_geometry_info(SGAL::Indexed_face_set* ifs)
{
  static const char* primitive_types[] = {
    "TRIANGLE STRIP",
    "TRIANGLE FAN",
    "TRIANGLES",
    "QUAD STRIP",
    "QUADS",
    "POLYGONS"
  };
  
  std::cout << "Geometry: Indexed_face_set"
            << ", No. primitives: " << ifs->get_num_primitives()
            << ", Types: " << primitive_types[ifs->get_primitive_type()]
            << std::endl;
}

/*! \brief updates directory search. */
void Player_scene::update_data_dirs()
{
  // Add directories to search list
  m_scene_graph->add_data_dir(".");

  //! \todo use fi::path
  std::string path(m_fullname);
  size_t n = path.rfind('/');
  if (n < path.size()) {
    path.erase(n, path.size());
    if (path != ".") m_scene_graph->add_data_dir(path);
    path.clear();
  }
  
  const char* root = getenv("ROOT");
  if (root) {
#if BOOST_VERSION >= 103400
    fi::path dir(root);
#else
    fi::path dir(root, fi::native);
#endif    
    dir /= "/data/images";
    m_scene_graph->add_data_dir(dir);
  }
}

/*! \brief prints statistic information. */
void Player_scene::print_stat()
{
#if defined(USE_CGAL)
  static const char* names[] =
    {"GEOM", "ROBOT", "OBSTACLE", "MINK", "MINK_CH"};

  if (m_option_parser->get_verbosity_level() >= 1) {
    unsigned int size = sizeof(names) / sizeof(char*);
    for (const char** ni = names; ni != &names[size]; ++ni) {
      std::string name(*ni);
    
      SGAL::Exact_polyhedron_geo* ep =
        dynamic_cast<SGAL::Exact_polyhedron_geo*>
        (m_scene_graph->get_container(name));
      if (ep) ep->print_stat();
#if defined(USE_AOS)
      SGAL::Arrangement_on_sphere_geo* aos =
        dynamic_cast<SGAL::Arrangement_on_sphere_geo*>
        (m_scene_graph->get_container(name));
      if (aos) aos->print_stat();
#endif
#if defined(USE_SGM)
      SGAL::Spherical_gaussian_map_geo* sgm =
        dynamic_cast<SGAL::Spherical_gaussian_map_geo*>
        (m_scene_graph->get_container(name));
      if (sgm) sgm->print_stat();
#endif
#if defined(USE_CGM)
      SGAL::Cubical_gaussian_map_geo* cgm =
        dynamic_cast<SGAL::Cubical_gaussian_map_geo*>
        (m_scene_graph->get_container(name));
      if (cgm) cgm->print_stat();
#endif
#if defined(USE_NEF) && defined(USE_NGM)
      SGAL::Nef_gaussian_map_geo* ngm =
        dynamic_cast<SGAL::Nef_gaussian_map_geo*>
        (m_scene_graph->get_container(name));
      if (ngm) ngm->print_stat();
#endif
    }
  }
#endif
}

/*! \brief clears the scene. */
void Player_scene::clear_scene()
{
  m_scene_graph->release_context();
  m_scene_graph->destroy_defaults();
  delete m_window_item;
  m_window_item = NULL;
}

/*! \brief identifies the agent. */
void Player_scene::identify(void)
{ std::cout << "Agent: Player_scene" << std::endl; }

/*! \brief handles a mouse event. */
void Player_scene::handle(SGAL::Mouse_event* event)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Accumulation* acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

/*! \brief handles a motion event. */
void Player_scene::handle(SGAL::Motion_event* event)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Accumulation* acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

/*! \brief handles a passive motion event. */
void Player_scene::handle(SGAL::Passive_motion_event* event)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Accumulation* acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

/*! \brief handless a keyboard event. */
void Player_scene::handle(SGAL::Keyboard_event* keyboard_event)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Accumulation* acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();

  if (keyboard_event->get_pressed()) return;
  SGAL::Uint key = keyboard_event->get_key();
  if (key == 0x1b) m_window_manager->destroy_window(m_window_item); // escape
}

/*! \brief handless a tick event. */
void Player_scene::handle(SGAL::Tick_event* tick_event)
{
  if (m_option_parser->get_verbosity_level() >= 2)
    std::cout << "Handling Tick event: "
              << "Window_manager time: "
              << tick_event->get_sim_time() << std::endl;
}

/*! \brief handles a reshape event. */
void Player_scene::handle(SGAL::Reshape_event* event)
{
  SGAL::Window_item* window_item = event->get_window_item();
  SGAL::Uint width = event->get_width();
  SGAL::Uint height = event->get_height();
  reshape_window(window_item, width, height);  
}

/*! \brief handles a draw event */
void Player_scene::handle(SGAL::Draw_event* event)
{
  SGAL::Window_item* window_item = event->get_window_item();
  SGAL::Boolean dont_accumulate = event->get_suppress_accumulation();
  draw_window(window_item, dont_accumulate);
}

/*! \brief draws into a window of the scene. */
void Player_scene::draw_window(SGAL::Window_item* window_item,
                               SGAL::Boolean dont_accumulate)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);

  SGAL::Draw_action draw_action(conf);
  draw_action.set_context(m_context);
  draw_action.set_snap(false);
  m_scene_graph->draw(&draw_action);
  if (m_option_parser->get_draw_grid()) draw_grid();
  
  SGAL::Accumulation* acc = conf->get_accumulation();
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
    window_item->set_accumulating(true);
    window_item->set_redraw(true);
    return;
  }

  // Accumulation is active:
  if (acc->is_done()) {
    // Accumulation is done:
    acc->disactivate();
    m_scene_graph->process_snapshots(&draw_action);
    window_item->set_accumulating(false);
    window_item->swap_buffers();
    return;
  }

  // Accumulation is not done:
  window_item->set_redraw(true);
  if (acc->do_show()) window_item->swap_buffers();
}

/*! \brief draws guides that separate the window into 4x5 rectangles. */
void Player_scene::draw_grid()
{
  m_context->draw_light_enable(false);
  glViewport(0, 0, m_win_width, m_win_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, m_win_width - 0.5, -0.5, m_win_height - 0.5, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  int width = m_win_width / 5;
  int height = m_win_height / 4;
  // m_context->set_line_width(1);
  unsigned int i;
  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  for (i = 1; i < 4; ++i) {
    glVertex2i(0, height*i);
    glVertex2i(m_win_width, height*i);
  }
  for (i = 1; i < 5; ++i) {
    glVertex2i(width*i, 0);
    glVertex2i(width*i, m_win_height);
  }
  glEnd();
  m_context->draw_light_enable(true);
}

/*! \brief reshapes the viewport of a window of the scene. */
void Player_scene::reshape_window(SGAL::Window_item* window_item,
                                  SGAL::Uint width, SGAL::Uint height)
{
  m_win_width = width;
  m_win_height = height;
  SGAL::Context* context = m_scene_graph->get_context();
  context->set_viewport(0, 0, width, height);
  SGAL::Camera* camera = m_scene_graph->get_active_camera();
  SGAL_assertion(camera);
  camera->init(context);
}

/*! Return true iff the scene does simulate something. In other words,
 * return true iff tick evenets must be generated to perform the simulation.
 */
SGAL::Boolean Player_scene::is_simulating(void)
{
  SGAL::Scene_graph* sg = get_scene_graph();
  return sg && sg->has_time_sensors();
}
