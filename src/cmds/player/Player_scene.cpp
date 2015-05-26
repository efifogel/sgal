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
#include <iostream>
#include <fstream>
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/shared_ptr.hpp>

#if defined(SGAL_USE_V8)
#include <v8.h>
#endif

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

////
// Until the bug is fixed.
#include <CGAL/boost/graph/iterator.h>
////

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/errors.hpp"
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
#include "SGAL/Frustum.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Image_writer.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Snapshotter.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Camera.hpp"

#if (defined SGAL_USE_GLUT)
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

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"

#if 0
#include "SCGAL/basic.h"

#include "SCGAL/Exact_polyhedron_geo.hpp"
#if defined(SGAL_USE_CGM)
#include "SCGAL/Cubical_gaussian_map_geo.hpp"
#endif
#if defined(SGAL_USE_SGM)
#include "SCGAL/Spherical_gaussian_map_geo.hpp"
#endif
#if defined(SGAL_USE_AOS)
#include "SCGAL/Arrangement_on_sphere_geo.hpp"
#endif
#if defined(SGAL_USE_EOS)
#include "SCGAL/Envelope_on_sphere_geo.hpp"
#endif
#if defined(SGAL_USE_NEF) && defined(SGAL_USE_NGM)
#include "SCGAL/Nef_gaussian_map_geo.hpp"
#endif
#endif

//! \brief constructor.
Player_scene::Player_scene(Player_option_parser* option_parser) :
  m_window_manager(nullptr),
  m_window_item(nullptr),
  m_win_width(0), m_win_height(0),
  m_offscreen_width(0), m_offscreen_height(0),
  m_isolate(nullptr),
  m_scene_graph(nullptr),
  m_context(nullptr),
  m_option_parser(option_parser),
  m_simulate(false)
{ init(); }

//! \brief constructor.
Player_scene::Player_scene() :
  m_window_manager(nullptr),
  m_window_item(nullptr),
  m_win_width(0), m_win_height(0),
  m_scene_graph(nullptr),
  m_context(nullptr),
  m_simulate(false)
{ init(); }

//! \brief initializes.
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

//! \brief destructor.
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

//! \brief finds the input file.
void Player_scene::find_input_file(const std::string& filename)
{
  SGAL_assertion(!filename.empty());

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
    return;
  }

  m_option_parser->for_each_dir(Add_dir(m_dirs));
  for (Path_iter pi = m_dirs.begin(); pi != m_dirs.end(); ++pi) {
    fi::path full_file_path = *pi / file_path;
    if (!fi::exists(full_file_path)) continue;
#if BOOST_VERSION >= 103400
    m_fullname = full_file_path.string();
#else
    m_fullname = full_file_path.native_file_string();
#endif
    return;
  }
}

//! \brief creates the scene.
void Player_scene::create_scene(char* data, int size)
{
  // Construct a Scene_graph:
  m_scene_graph = new SGAL::Scene_graph;
  SGAL_assertion(m_scene_graph);

#if defined(SGAL_USE_V8)
  m_isolate = v8::Isolate::New();
  SGAL_assertion(m_isolate);
  m_scene_graph->set_isolate(m_isolate);
#endif

  update_data_dirs();

  // Load the buffer.
  if (!data || (size == 0)) {
    //! \todo introduce a new error.
    throw Illegal_input(FILE_NOT_FOUND, "Data is nil", "");
    return;
  }

  SGAL::Loader loader;
  if (0 < m_option_parser->get_num_input_files()) {
    const auto& filename = m_option_parser->get_input_file(0);
    auto rc = loader.load(data, size, filename.c_str(), m_scene_graph);
    if (rc < 0) {
      throw Illegal_input(UNABLE_TO_LOAD, "Cannot load file", filename);
      return;
    }
  }
  else {
    auto rc = loader.load(data, size, m_scene_graph);
    if (rc < 0) {
      throw Illegal_input(UNABLE_TO_LOAD, "Cannot load buffer");
      return;
    }
  }
  print_stat();

  create_defaults();
  if (m_option_parser->do_snapshot()) snapshot_scene();
  if (m_option_parser->do_export()) export_scene();
}

//! \brief creates the scene.
void Player_scene::create_scene()
{
  // Construct a Scene_graph:
  m_scene_graph = new SGAL::Scene_graph;
  SGAL_assertion(m_scene_graph);

#if defined(SGAL_USE_V8)
  m_isolate = v8::Isolate::New();
  SGAL_assertion(m_isolate);
  m_scene_graph->set_isolate(m_isolate);
#endif

  update_data_dirs();

  // Find the input file full name.
  if (0 == m_option_parser->get_num_input_files()) {
    std::string str("input file missing!");
    throw Input_file_missing_error(str);
    return;
  }
  const auto& filename = m_option_parser->get_input_file(0);
  find_input_file(filename);
  if (m_fullname.empty()) {
    throw Illegal_input(FILE_NOT_FOUND, "cannot find file", filename);
    return;
  }

  // Load the input file.
  SGAL::Loader loader;
  auto rc = loader.load(m_fullname.c_str(), m_scene_graph);
  if (rc < 0) {
    throw Illegal_input(UNABLE_TO_LOAD, "Cannot load file", filename);
    return;
  }
  print_stat();

  create_defaults();
  if (m_option_parser->do_snapshot()) snapshot_scene();
  if (m_option_parser->do_export()) export_scene();
}

//! \brief takes a snapshot of the scene.
void Player_scene::snapshot_scene()
{
  const auto& output_filename = m_option_parser->get_output_file();
  const auto& output_pathname = m_option_parser->get_output_path();
  boost::shared_ptr<SGAL::Snapshotter> snapshotter(new SGAL::Snapshotter);
  SGAL_assertion(snapshotter);
  m_scene_graph->get_root()->add_child(snapshotter);
  if (!m_image) m_image.reset(new SGAL::Image);
  SGAL_assertion(m_image);
  snapshotter->set_image(m_image);
  snapshotter->add_to_scene(m_scene_graph);
  auto mode = (do_render_offscreen()) ?
    SGAL::Snapshotter::MODE_COLOR_ATTACHMENT : SGAL::Snapshotter::MODE_BACK;
  snapshotter->set_mode(mode);
  snapshotter->trigger();
  auto* src_field = snapshotter->get_source_field("image");
  SGAL_assertion(src_field);
  typedef boost::shared_ptr<SGAL::Image_writer>         Shared_image_writer;
  if (0 == m_option_parser->formats_2d_size()) {
    Shared_image_writer image_writer(new SGAL::Image_writer);
    SGAL_assertion(image_writer);
    m_scene_graph->get_root()->add_child(image_writer);

    image_writer->set_image(m_image);
    if (!output_pathname.empty()) image_writer->set_dir_name(output_pathname);
    auto lastdot = output_filename.find_last_of(".");
    auto filename = (lastdot == std::string::npos) ? output_filename :
      output_filename.substr(0, lastdot);
    if (!filename.empty()) image_writer->set_file_name(filename);

    auto* dst_field = image_writer->get_destination_field("trigger");
    SGAL_assertion(dst_field);

    src_field->disconnect(dst_field); // disconnect old connections if exists
    src_field->connect(dst_field);

    return;
  }

  // Iterate over all requested formats.
  for (auto it = m_option_parser->formats_2d_begin();
       it != m_option_parser->formats_2d_end(); ++it)
  {
    Shared_image_writer image_writer(new SGAL::Image_writer);
    SGAL_assertion(image_writer);
    m_scene_graph->get_root()->add_child(image_writer);

    image_writer->set_image(m_image);
    if (!output_pathname.empty()) image_writer->set_dir_name(output_pathname);
    auto lastdot = output_filename.find_last_of(".");
    auto filename = (lastdot == std::string::npos) ? output_filename :
      output_filename.substr(0, lastdot);
    if (!filename.empty()) image_writer->set_file_name(filename);
    SGAL::File_format_2d::Id format_id = *it;
    image_writer->set_file_format(format_id);

    auto* dst_field = image_writer->get_destination_field("trigger");
    SGAL_assertion(dst_field);

    src_field->disconnect(dst_field); // disconnect old connections if exists
    src_field->connect(dst_field);
  }
}

//! \brief exports the scene.
void Player_scene::export_scene()
{
  const auto& output_filename = m_option_parser->get_output_file();
  const auto& output_pathname = m_option_parser->get_output_path();
  fi::path output_path(output_pathname);
  auto input_format_id = m_scene_graph->get_input_format_id();

  if (0 == m_option_parser->formats_3d_size()) {
    SGAL_assertion(!output_filename.empty());
    fi::path output_filename_path(output_filename);
    if (! output_filename_path.has_extension()) {
      const auto& new_extension =
        SGAL::File_format_3d::get_name(input_format_id);
      output_filename_path.replace_extension(new_extension);
    }
    output_path /= output_filename_path;
    m_scene_graph->write(output_path.string(), input_format_id);
    return;
  }

  // Iterate over all requested formats.
  for (auto it = m_option_parser->formats_3d_begin();
       it != m_option_parser->formats_3d_end(); ++it)
  {
    SGAL::File_format_3d::Id format_id = *it;
    const auto& new_extension = SGAL::File_format_3d::get_name(format_id);
    fi::path output_filename_path(output_filename);
    output_filename_path.replace_extension(new_extension);
    output_path /= output_filename_path;
    m_scene_graph->write(output_path.string(), format_id);

    // #if BOOST_VERSION <= 104800
    // // Workaround a bug in boost version 1.48 and lower:
    // fi::path tmp_path(output_filename);
    // fi::path output_path = tmp_path.parent_path();
    // output_path /= tmp_path.stem();
    // std::string final = output_path.string() + "." + new_extension;
    // m_scene_graph->write(final, format_id);
    // #else
    // fi::path output_filename_path(output_filename);
    // output_filename_path.replace_extension(new_extension);
    // m_scene_graph->write(output_filename_path.string(), format_id);
    // #endif
  }
}

//! \brief destroys the scene.
void Player_scene::destroy_scene()
{
  m_scene_graph->destroy_defaults();
  if (m_scene_graph) {
    delete m_scene_graph;
    m_scene_graph = nullptr;
  }

#if defined(SGAL_USE_V8)
  if (m_isolate) {
    m_isolate->Dispose();
    m_isolate = nullptr;
  }
#endif
}

//! \brief sets preferred window attributes.
void Player_scene::set_preferred_window_attributes()
{
  // If offscreen rendering is in effect, set the offsecreen width and height
  // to the prferred ones, respectively, and the window width and height to 1x1.
  if (do_render_offscreen()) {
    m_offscreen_width = m_window_item->get_width();
    m_offscreen_height = m_window_item->get_height();
    m_window_item->set_size(1, 1);
    m_window_item->set_double_buffer(false);
  }
  else m_window_item->set_double_buffer(true);

  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto acc = conf->get_accumulation();
  auto ms = conf->get_multisample();

  if (acc) {
    SGAL::Uint red_bits, green_bits, blue_bits, alpha_bits;
    acc->get_number_of_bits(red_bits, green_bits, blue_bits, alpha_bits);
    m_window_item->set_number_of_accumulation_bits(red_bits, green_bits,
                                                   blue_bits, alpha_bits);
  }
  if (ms) m_window_item->set_number_of_samples(ms->get_number_of_samples());
  m_window_item->set_number_of_stencil_bits(conf->get_number_of_stencil_bits());
  m_window_item->set_number_of_depth_bits(conf->get_number_of_depth_bits());
}

//! \brief sets actual window attributes.
void Player_scene::set_actual_window_attributes()
{
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);

  conf->set_number_of_stencil_bits(m_window_item->get_number_of_stencil_bits());
  conf->set_number_of_depth_bits(m_window_item->get_number_of_depth_bits());

  auto ms = conf->get_multisample();
  if (ms) ms->set_number_of_samples(m_window_item->get_number_of_samples());

  auto acc = conf->get_accumulation();
  if (acc) {
    SGAL::Uint red_bits, green_bits, blue_bits, alpha_bits;
    m_window_item->get_number_of_accumulation_bits(red_bits, green_bits,
                                                   blue_bits, alpha_bits);
    acc->set_number_of_bits(red_bits, green_bits, blue_bits, alpha_bits);
  }

  if (m_image) {
    auto alpha_bits = m_window_item->get_number_of_alpha_bits();
    //! \todo add an option to have alpha bits. If alpha bits are present
    // use SGAL::Image::kRGBA8_8_8_8 instead of SGAL::Image::kRGB8_8_8
    m_image->set_format(SGAL::Image::kRGB8_8_8);
    if (do_render_offscreen()) {
      m_image->set_width(m_offscreen_width);
      m_image->set_height(m_offscreen_height);
    }
    else {
      m_image->set_width(m_win_width);
      m_image->set_height(m_win_height);
    }
  }
}

//! \brief creates the visual.
void Player_scene::create_visual()
{
  SGAL::Uint sample_bits = m_window_item->get_number_of_samples();
  SGAL::Uint red_bits, green_bits, blue_bits, alpha_bits;
  m_window_item->get_number_of_accumulation_bits(red_bits, green_bits,
                                                 blue_bits, alpha_bits);
  SGAL::Boolean retry =
    ((red_bits + green_bits + blue_bits + alpha_bits + sample_bits) > 0);
  SGAL::Boolean visual_chosen = false;
  while (retry && !visual_chosen) {
    try {
      m_window_manager->create_window(m_window_item);
    }
    catch(SGAL::Visual_selection_error& e) {
      std::cerr << e.what() << std::endl;
      if (sample_bits > 0) {
        // Try with less sample bits:
        sample_bits = 0;
        std::cerr << "Retrying with 0 sample bits." << std::endl;
        m_window_item->set_number_of_samples(sample_bits);
        retry =
          ((red_bits + green_bits + blue_bits + alpha_bits + sample_bits) > 0);
        continue;
      }

      if ((red_bits + green_bits + blue_bits + alpha_bits) > 0) {
        // Try with less accumulation bits:
        red_bits = green_bits = blue_bits = alpha_bits = 0;
        std::cerr << "Retrying with 0 accumulation bits." << std::endl;
        m_window_item->set_number_of_accumulation_bits(red_bits, green_bits,
                                                       blue_bits, alpha_bits);
        retry =
          ((red_bits + green_bits + blue_bits + alpha_bits + sample_bits) > 0);
        continue;
      }
      SGAL_error();
    }
    visual_chosen = true;
  };

  if (!visual_chosen) m_window_manager->create_window(m_window_item);
}

//! \brief creates a window.
void Player_scene::create_window()
{
  // Prepare the window item.
  if (0 == m_option_parser->get_num_input_files()) {
    std::string str("input file missing!");
    throw Input_file_missing_error(str);
    return;
  }
  const auto& filename = m_option_parser->get_input_file(0);

  m_window_item = new Window_item;
  m_window_item->set_title(filename);
  set_preferred_window_attributes();
  create_visual();
  set_actual_window_attributes();
}

//! \brief creates default nodes in the scene graph.
void Player_scene::create_defaults()
{
  m_scene_graph->create_defaults();     // create missing nodes.

  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);

  auto acc = conf->get_accumulation();
  if (!acc && m_option_parser->get_accumulate()) {
    auto* tmp = new SGAL::Accumulation;
    acc.reset(tmp);
    conf->set_accumulation(acc);
  }

  //! \todo Introduce a command line option to use multisamples
  // auto ms = conf->get_multisample();
  // if (!ms && m_option_parser->get_multisample()) {
  //   ...;
  // }
}

//! \brief initializes the secene.
// Creates all windows; in this case only one.
void Player_scene::init_scene()
{
  // Configure the window manager and the scene graph.
  m_option_parser->configure(m_window_manager, m_scene_graph);
  create_window();                      // create a window
  indulge_user();
}

//! \brief indulges user requests from the command line.
void Player_scene::indulge_user()
{
  // Local options:
  if (m_option_parser->get_display_texture_info()) {
    SGAL_assertion(m_scene_graph);
    // Look for non instance containers:
    for (auto ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      auto cont = *ci;
      auto texture = boost::dynamic_pointer_cast<SGAL::Texture_2d>(cont);
      if (texture) texture->print_info();
    }

    // Look for instance containers:
    for (auto ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      auto cont = (*ii).second;
      auto texture = boost::dynamic_pointer_cast<SGAL::Texture_2d>(cont);
      if (texture) texture->print_info();
    }
  }

  if (m_option_parser->get_display_geometry_info()) {
    // Look for non instance containers:
    for (auto ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      auto cont = *ci;
      auto ifs = boost::dynamic_pointer_cast<SGAL::Indexed_face_set>(cont);
      if (ifs) print_geometry_info(&*ifs);
      auto box = boost::dynamic_pointer_cast<SGAL::Box>(cont);
      if (box) print_geometry_info(&*box);
    }

    // Look for instance containers:
    for (auto ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      auto cont = (*ii).second;
      auto ifs = boost::dynamic_pointer_cast<SGAL::Indexed_face_set>(cont);
      if (ifs) print_geometry_info(&*ifs);
      auto box = boost::dynamic_pointer_cast<SGAL::Box>(cont);
      if (box) print_geometry_info(&*box);
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
    for (auto ci = m_scene_graph->containers_begin();
         ci != m_scene_graph->containers_end(); ++ci)
    {
      auto* cont = *ci;
      auto* ifs = dynamic_cast<SGAL::Geo_set*>(cont);
      if (ifs) ifs->set_sub_index_array_size(sub_index_buffer_size);
    }

    // Look for instance containers:
    for (auto ii = m_scene_graph->instances_begin();
         ii != m_scene_graph->instances_end(); ++ii)
    {
      auto* cont = (*ii).second;
      auto* ifs = dynamic_cast<SGAL::Geo_set*>(cont);
      if (ifs) ifs->set_sub_index_array_size(sub_index_buffer_size);
    }
  }
#endif
}

//! \brief print geometry information of Box.
void Player_scene::print_geometry_info(SGAL::Box* /* box */)
{
  std::cout << "Geometry: Box" << std::endl;
}

//! \brief prints texture information of Index_face_set.
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

//! \brief updates directory search.
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

//! \brief prints statistic information.
void Player_scene::print_stat()
{
#if 0
  static const char* names[] =
    {"GEOM", "ROBOT", "OBSTACLE", "MINK", "MINK_CH"};

  if (m_option_parser->get_verbosity_level() >= 1) {
    unsigned int size = sizeof(names) / sizeof(char*);
    for (const char** ni = names; ni != &names[size]; ++ni) {
      std::string name(*ni);
      boost::shared_ptr<SGAL::Container> cont =
        m_scene_graph->get_container(name);
      boost::shared_ptr<SGAL::Exact_polyhedron_geo> ep =
        boost::dynamic_pointer_cast<SGAL::Exact_polyhedron_geo>(cont);
      if (ep) ep->print_stat();
#if defined(SGAL_USE_AOS)
      boost::shared_ptr<SGAL::Arrangement_on_sphere_geo> aos =
        boost::dynamic_pointer_cast<SGAL::Arrangement_on_sphere_geo>(cont);
      if (aos) aos->print_stat();
#if defined(SGAL_USE_SGM)
      boost::shared_ptr<SGAL::Spherical_gaussian_map_geo> sgm =
        boost::dynamic_pointer_cast<SGAL::Spherical_gaussian_map_geo>(cont);
      if (sgm) sgm->print_stat();
#endif
#if defined(SGAL_USE_CGM)
      boost::shared_ptr<SGAL::Cubical_gaussian_map_geo> cgm =
        boost::dynamic_pointer_cast<SGAL::Cubical_gaussian_map_geo>(cont);
      if (cgm) cgm->print_stat();
#endif
#if defined(SGAL_USE_NEF) && defined(SGAL_USE_NGM)
      boost::shared_ptr<SGAL::Nef_gaussian_map_geo> ngm =
        boost::dynamic_pointer_cast<SGAL::Nef_gaussian_map_geo>(cont);
      if (ngm) ngm->print_stat();
#endif
    }
  }
#endif
#endif
}

//! \brief clears the scene.
void Player_scene::clear_scene()
{
  // delete all windows.
  if (m_window_item) {
    delete m_window_item;
    m_window_item = nullptr;
  }
}

//! \brief identifies the agent.
void Player_scene::identify(void)
{ std::cout << "Agent: Player_scene" << std::endl; }

//! \brief handles a mouse event.
void Player_scene::handle(SGAL::Mouse_event* /* event */)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  boost::shared_ptr<SGAL::Accumulation> acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

//! \brief handles a motion event.
void Player_scene::handle(SGAL::Motion_event* /* event */)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  boost::shared_ptr<SGAL::Accumulation> acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

//! \brief handles a passive motion event.
void Player_scene::handle(SGAL::Passive_motion_event* /* event */)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  boost::shared_ptr<SGAL::Accumulation> acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();
}

//! \brief handless a keyboard event.
void Player_scene::handle(SGAL::Keyboard_event* keyboard_event)
{
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  auto acc = conf->get_accumulation();
  if (acc && acc->is_enabled() && !acc->is_done()) acc->enactivate();

  if (keyboard_event->get_pressed()) return;
  SGAL::Uint key = keyboard_event->get_key();
  if (key == 0x1b) m_window_manager->destroy_window(m_window_item); // escape
}

//! \brief handless a tick event.
void Player_scene::handle(SGAL::Tick_event* tick_event)
{
  if (m_option_parser->get_verbosity_level() >= 2)
    std::cout << "Handling Tick event: "
              << "Window_manager time: "
              << tick_event->get_sim_time() << std::endl;
}

//! \brief handles a reshape event.
void Player_scene::handle(SGAL::Reshape_event* event)
{
  auto* window_item = event->get_window_item();
  auto width = event->get_width();
  auto height = event->get_height();
  reshape_window(window_item, width, height);
}

//! \brief handles a draw event.
void Player_scene::handle(SGAL::Draw_event* event)
{
  std::cout << "Player_scene::handle()" << std::endl;
  auto* window_item = event->get_window_item();
  SGAL::Boolean dont_accumulate = event->get_suppress_accumulation();
  draw_window(window_item, dont_accumulate);
}

//! \brief initializes offscreen rendering.
void Player_scene::init_offscreen_rendering()
{
  auto width = m_offscreen_width;
  auto height = m_offscreen_height;
  m_render_buffers[0] = 0;
  m_render_buffers[1] = 0;
  SGAL::glGenRenderbuffers(2, m_render_buffers);

  SGAL::glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffers[0]);
  auto gl_format =
    SGAL::Image::get_format_internal_format(m_image->get_format());
  SGAL::glRenderbufferStorage(GL_RENDERBUFFER, gl_format,
                              width, height);
  SGAL::glBindRenderbuffer(GL_RENDERBUFFER, 0);

  SGAL::glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffers[1]);
  SGAL::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                              width, height);
  SGAL::glBindRenderbuffer(GL_RENDERBUFFER, 0);

  SGAL::glGenFramebuffers(1, &m_frame_buffer);
  SGAL::glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
  SGAL::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, m_render_buffers[0]);
  SGAL::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_render_buffers[1]);
}

//! \brief clears offscreen rendering.
void Player_scene::clear_offscreen_rendering()
{
  SGAL::glDeleteRenderbuffers(2, m_render_buffers);
  m_render_buffers[0] = 0;
  m_render_buffers[1] = 0;
  SGAL::glDeleteFramebuffers(1, &m_frame_buffer);
  m_frame_buffer = 0;
}

//! \brief initializes the window.
// creates the context.
void Player_scene::init_window(SGAL::Window_item* window_item,
                               SGAL::Uint width, SGAL::Uint height)
{
  m_context = new SGAL::Context();
  SGAL_assertion(m_context);

  m_win_width = width;
  m_win_height = height;
  if (do_render_offscreen()) {
    init_offscreen_rendering();
    m_context->set_viewport(0, 0, m_offscreen_width, m_offscreen_height);
  }
  else {
    if (m_image) {
      m_image->set_width(width);
      m_image->set_height(height);
    }
    m_context->set_viewport(0, 0, m_win_width, m_win_height);
  }

  m_scene_graph->start_simulation();
  m_scene_graph->bind();
  window_item->show();

  m_scene_graph->set_context(m_context);
  m_scene_graph->init_context();
}

//! \brief clears a window.
// destroys the context.
void Player_scene::clear_window(SGAL::Window_item* window_item)
{
  if (do_render_offscreen()) clear_offscreen_rendering();
  m_scene_graph->release_context();
  m_scene_graph->set_context(nullptr);
  if (m_context) {
    delete m_context;
    m_context = nullptr;
  }
}

//! \brief draws into a window of the scene.
// \todo Replace accumulation with rendering to texture and blending.
void Player_scene::draw_window(SGAL::Window_item* window_item,
                               SGAL::Boolean dont_accumulate)
{
  if (!m_context) return;
  SGAL_assertion(m_scene_graph);
  auto* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);

  SGAL::Draw_action draw_action(conf);
  draw_action.set_context(m_context);
  draw_action.set_snap(false);

  m_scene_graph->draw(&draw_action);
  if (m_option_parser->get_draw_grid()) draw_grid();

  auto acc = conf->get_accumulation();
  if (!acc || !acc->is_enabled() || dont_accumulate) {
    m_scene_graph->process_snapshotters(&draw_action);
    if (acc) acc->disactivate();
    window_item->set_accumulating(false);
    if (!do_render_offscreen()) window_item->swap_buffers();
    if (!is_interactive()) m_window_manager->destroy_window(m_window_item);
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
    m_scene_graph->process_snapshotters(&draw_action);
    window_item->set_accumulating(false);
    if (!do_render_offscreen()) window_item->swap_buffers();
    if (!is_interactive()) m_window_manager->destroy_window(m_window_item);
    return;
  }

  // Accumulation is not done:
  window_item->set_redraw(true);
  if (acc->do_show()) {
    if (!do_render_offscreen()) window_item->swap_buffers();
  }
}

//! \brief draws guides that separate the window into 4x5 rectangles.
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

//! \brief reshapes the viewport of a window of the scene.
void Player_scene::reshape_window(SGAL::Window_item* /* window_item */,
                                  SGAL::Uint width, SGAL::Uint height)
{
  if (!m_context) return;
  if ((m_win_width == width) && (m_win_height == height)) return;
  m_win_width = width;
  m_win_height = height;
  if (do_render_offscreen()) {
    // I think that this case cannot take place.
    m_context->set_viewport(0, 0, m_offscreen_width, m_offscreen_height);
  }
  else {
    m_context->set_viewport(0, 0, width, height);
    if (m_image) {
      m_image->set_width(width);
      m_image->set_height(height);
    }
  }
}

/*! \brief returns true iff the scene does simulate something. In other words,
 * return true iff tick evenets must be generated to perform the simulation.
 */
SGAL::Boolean Player_scene::is_simulating(void) const
{
  const auto* sg = get_scene_graph();
  return sg && sg->has_time_sensors();
}

//! \brief determines whether the operation is interactive.
SGAL::Boolean Player_scene::do_have_visual() const
{
  return (is_simulating() ||
          (!m_option_parser->is_interactive_defaulted() &&
           m_option_parser->is_interactive()) ||
          (m_option_parser->is_interactive() && !m_option_parser->do_export()));
}

//! \brief determines whether the application renders off screen.
SGAL::Boolean Player_scene::do_render_offscreen() const
{
  //! \todo Fix antialiasing to render to texture and then blend (instead of
  //        using the obsolete accumatayion buffer. Untill then supress the use
  //        of the offscreen rendering, cause it cannot be used with
  //        accumulation. Simply render to the back buffer meanwhile.
  return false;
  // return ((m_option_parser->is_interactive_defaulted() ||
  //          !m_option_parser->is_interactive()) &&
  //         (!m_option_parser->is_interactive() ||
  //          m_option_parser->do_snapshot()));
}

//! \brief determines whether the scene is interactive.
SGAL::Boolean Player_scene::is_interactive() const
{
  return ((!m_option_parser->is_interactive_defaulted() ||
           (!m_option_parser->do_snapshot() && !m_option_parser->do_export())) &&
          (m_option_parser->is_interactive_defaulted() ||
           m_option_parser->is_interactive()));
}

//! \brief obtains the camera projection matrix.
void Player_scene::get_proj_mat(SGAL::Matrix4f& mat)
{
  auto* camera = m_scene_graph->get_active_camera();
  auto& frustum = camera->get_frustum();
  frustum.get_proj_mat(mat);
}

//! \brief obtains the camera viewing matrix.
void Player_scene::get_view_mat(SGAL::Matrix4f& mat)
{
  auto* camera = m_scene_graph->get_active_camera();
  const auto& tmp = camera->get_view_mat();
  mat.set(tmp);
}
