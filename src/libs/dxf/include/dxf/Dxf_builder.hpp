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

#ifndef DXF_BUILDER_HPP
#define DXF_BUILDER_HPP

#include <list>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Group;
class Appearance;
class Color_array;

SGAL_END_NAMESPACE

DXF_BEGIN_NAMESPACE

class Dxf_data;
class Dxf_base_entity;
class Dxf_polyline_boundary_path;

// Entities
class Dxf_line_entity;
class Dxf_circle_entity;
class Dxf_arc_entity;
class Dxf_hatch_entity;
class Dxf_polyline_entity;
class Dxf_lwpolyline_entity;
class Dxf_spline_entity;
class Dxf_solid_entity;
class Dxf_insert_entity;

class SGAL_SGAL_DECL Dxf_builder {
public:
  typedef boost::shared_ptr<SGAL::Appearance>         Shared_appearance;
  typedef boost::shared_ptr<SGAL::Color_array>        Shared_color_array;

  /*! Construct.
   */
  Dxf_builder(Dxf_data& data, SGAL::Scene_graph* scene_graph);

  /*! Destruct.
   */
  ~Dxf_builder();

  /*! Build from the root.
   */
  void operator()(SGAL::Group* root);

protected:
  /*! Process all layers. Create a color array for each.
   */
  void process_layers();

  /*! Initialize the pallete.
   */
  static void init_palette(const SGAL::String& file_name);

  /*! Add a default background color.
   */
  void add_background(SGAL::Group* root);

  /// \name General Processors
  //@{

  /*! Dispatch the processing of all entities.
   */
  void process_entities(std::vector<Dxf_base_entity*>& entities,
                        SGAL::Group* root);

  //@}

  /// Entity processors
  //@{

  /*! Process a line entity.
   */
  void process_line_entity(const Dxf_line_entity& line, SGAL::Group* root);

  /*! Process a circle entity.
   */
  void process_circle_entity(const Dxf_circle_entity& circle, SGAL::Group* root);

  /*! Process an arc entity.
   */
  void process_arc_entity(const Dxf_arc_entity& arc, SGAL::Group* root);

  /*! Process a hatch entity.
   */
  void process_hatch_entity(const Dxf_hatch_entity& hatch, SGAL::Group* root);

  /*! Process a polyline entity.
   */
  void process_polyline_entity(const Dxf_polyline_entity& polyline,
                               SGAL::Group* root);

  /*! Process a light weight polyline entity.
   */
  void process_lwpolyline_entity(const Dxf_lwpolyline_entity& polyline,
                                 SGAL::Group* root);

  /*! Process a spline entity.
   */
  void process_spline_entity(const Dxf_spline_entity& spline, SGAL::Group* root);

  /*! Process an insert entity.
   */
  void process_insert_entity(const Dxf_insert_entity& insert, SGAL::Group* root);

  /*! Process a solid entity.
   */
  void process_solid_entity(const Dxf_solid_entity& solid, SGAL::Group* root);

  //@}

  /*! Obtain the color array of an entity.
   */
  Shared_color_array get_color_array(int32_t color, int16_t color_index,
                                     const SGAL::String& layer_name);

  /*! Obtain the light-disabled appearance.
   */
  Shared_appearance get_fill_appearance();

  /*! Obtain a light-disabled pattern appearance.
   */
  Shared_appearance get_pattern_appearance();

  /*! Add polylines (without bulge).
   * \param[in] polylines
   * \param[in] root
   * \param[in] closed
   */
  void process_polyline_boundaries
    (const Dxf_hatch_entity& hatch_entity,
     const std::list<Dxf_polyline_boundary_path*>& polylines,
     SGAL::Group* root);

  /*! Print out hatch information.
   */
  void print_hatch_information(const Dxf_hatch_entity& hatch);

  //! The DXF data
  Dxf_data& m_data;

  //! The scene graph
  SGAL::Scene_graph* m_scene_graph;

  //! The trace code.
  size_t m_trace_code;

private:
  //! A light-disabled appearance.
  Shared_appearance m_fill_appearance;

  //! A light-disabled pattern enabled appearance.
  std::list<Shared_appearance> m_pattern_appearances;

  //! Color arrays that match the default color palette
  std::map<size_t, Shared_color_array> m_color_arrays;

  /// Counters for statistics. \todo Move out with Builder.
  //@{
  size_t m_lines_num;
  size_t m_polylines_num;
  size_t m_lwpolylines_num;
  size_t m_circles_num;
  size_t m_arcs_num;
  size_t m_hatches_num;
  size_t m_splines_num;
  size_t m_solids_num;

  size_t m_inserts_num;
  //@}

  //! Default color palette
  static std::vector<SGAL::Vector3f> s_palette;
};

DXF_END_NAMESPACE

#endif
