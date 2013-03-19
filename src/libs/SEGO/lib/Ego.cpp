// Copyright (c) 2012 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4503)
#endif

/*! \file
 * A geometry container that represents a model decomposed into ego bricks.
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <Magick++.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Image_base.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Route.hpp"

#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego.hpp"
#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels_tiler.hpp"
#include "SEGO/Ego_voxelizer.hpp"

// #include <boost/tuple/tuple_io.hpp>

// #define SGAL_EGO_VERBOSE 1
#ifdef SGAL_EGO_VERBOSE
#define SGAL_EGO_VAR(x) #x << " " << x
#endif

SGAL_BEGIN_NAMESPACE

class Configuration;
  
std::string Ego::s_tag = "Ego";
Container_proto* Ego::s_prototype = NULL;

const Float Ego::s_def_voxel_width(8.2f);
const Float Ego::s_def_voxel_length(8.2f);
const Float Ego::s_def_voxel_height(9.6f);
const Ego_voxels_tiler::First_tile_placement 
Ego::s_def_first_tile_placement(Ego_voxels_tiler::FIRST00);
const Ego_voxels_tiler::Strategy
Ego::s_def_tiling_strategy(Ego_voxels_tiler::NONGRID);
const Ego_voxels_tiler::Tiling_rows
Ego::s_def_tiling_rows_direction(Ego_voxels_tiler::YROWS);
const Ego::Style Ego::s_def_style(STYLE_RANDOM_COLORS);
const Ego::Color_space Ego::s_def_color_space(COLOR_SPACE_RGB);
const Boolean Ego::s_def_space_filling(false);
const Ego::Layer_visibility Ego::s_layer_x_visibility(LV_ALL);
const Ego::Layer_visibility Ego::s_layer_y_visibility(LV_ALL);
const Ego::Layer_visibility Ego::s_layer_z_visibility(LV_ALL);

/*! Style names */
const char* Ego::s_style_names[] =
  { "randomColors", "appearance", "discreteCubeMap" };

/*! Color space names */
const char* Ego::s_color_space_names[] = {"RGB", "HSL"};

/*! Layer visibility names */
const char* Ego::s_layer_visibility_names[] = {
  "all", "above", "notAbove", "below", "notBelow", "only", "notOnly"
};

REGISTER_TO_FACTORY(Ego, "Ego");

/*! Constructor */
Ego::Ego(Boolean proto) :
  Transform(proto),
  m_voxel_width(s_def_voxel_width),
  m_voxel_length(s_def_voxel_length),
  m_voxel_height(s_def_voxel_height),
  m_style(s_def_style),
  m_touch_sensor(NULL),
  m_appearance(NULL),
  m_space_filling(s_def_space_filling),
  m_appearance_prev(NULL),
  m_first_tile_placement(s_def_first_tile_placement),
  m_tiling_strategy(s_def_tiling_strategy),
  m_tiling_rows_direction(s_def_tiling_rows_direction),
  m_dirty_appearance(true),
  m_dirty_voxels(true),
  m_dirty_tiling(true),
  m_dirty_parts(true),
  m_dirty_colors(true),
  m_dirty_visibility(true),
  m_owned_parts(false),
  m_scene_graph(NULL),
  m_knob_slices(Ego_brick::s_def_knob_slices),
  m_color_space(s_def_color_space),
  m_layer_x(0),
  m_layer_y(0),
  m_layer_z(0),
  m_layer_x_visibility(LV_ALL),
  m_layer_y_visibility(LV_ALL),
  m_layer_z_visibility(LV_ALL),
  m_selected(false),
  m_owned_appearance(false),
  m_clean_colors_in_progress(false)
{ if (m_style == STYLE_RANDOM_COLORS) m_dirty_appearance = false; }

/*! Destructor */
Ego::~Ego() { clear(); }

/*! \brief obtains the (const) polyhedron model. */
const Polyhedron_geo* Ego::get_polyhedron_model() const
{ return boost::get<Polyhedron_geo*>(m_model); }

/*! \brief obtains the (const) exact polyhedron model. */
const Exact_polyhedron_geo* Ego::get_exact_polyhedron_model() const
{ return boost::get<Exact_polyhedron_geo*>(m_model); }

/*! \brief obtains the (const) geometry set model. */
const Geo_set* Ego::get_geo_set_model() const
{ return boost::get<Geo_set*>(m_model); }

/*! \brief obtains the (non-const) polyhedron model. */
Polyhedron_geo* Ego::get_polyhedron_model()
{ return boost::get<Polyhedron_geo*>(m_model); }

/*! \brief obtains the (non-const) exact polyhedron model. */
Exact_polyhedron_geo* Ego::get_exact_polyhedron_model()
{ return boost::get<Exact_polyhedron_geo*>(m_model); }

/*! \brief obtains the (non-const) geometry set model. */
Geo_set* Ego::get_geo_set_model()
{ return boost::get<Geo_set*>(m_model); }

/*! \brief clear the parts */
void Ego::clear_parts()
{
  m_dirty_parts = true;
  Node_iterator it1 = m_childs.begin();
  while (it1 != m_childs.end()) {
    Node* node1 = *it1++;
    // Free the transform:
    Transform* transform = dynamic_cast<Transform*>(node1);
    if (transform) {
      Node_iterator it2 = transform->children_begin();
      while (it2 != transform->children_end()) {
        Node* node2 = *it2++;
        // Remove the brick (Shape):
        Shape* brick_shape = dynamic_cast<Shape*>(node2);
        if (brick_shape) delete brick_shape;
        transform->remove_child(node2);
      }
    }

    delete node1;
    remove_child(node1);
  }

  // Delete all appearances:
  Appearance_iter ait;
  for (ait = m_appearances.begin(); ait != m_appearances.end(); ++ait)
    delete ait->second;
  m_appearances.clear();

  // Delete all materials:
  Material_iter mit;
  for (mit = m_materials.begin(); mit != m_materials.end(); ++mit)
    delete *mit;
  m_materials.clear();

  // Delete all Ego bricks:
  Ego_brick_iter bit;
  for (bit = m_bricks.begin(); bit != m_bricks.end(); ++bit)
    delete (*bit).second;
  m_bricks.clear();
  for (bit = m_knobless_bricks.begin(); bit != m_knobless_bricks.end(); ++bit)
    delete (*bit).second;
  m_knobless_bricks.clear();

  // Clean the voxel signature array. */
  m_voxel_signatures.clear();

  m_owned_parts = false;
}

/*! \brief clear the internal representation and auxiliary data structures
 */
void Ego::clear()
{
  if (m_owned_parts) clear_parts();
  
  m_dirty_sphere_bound = true;
  m_dirty_voxels = true;
  m_dirty_tiling = true;
  m_dirty_visibility = true;
  
  if (m_owned_appearance) {
    if (m_appearance) {
      delete m_appearance;
      m_appearance = NULL;
    }
    m_owned_appearance = false;
  }
}

/*! \brief initializes the container prototype */
void Ego::init_prototype()
{
    //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);
  
  if (s_prototype) return;
  s_prototype = new Container_proto(Transform::get_prototype());

  // We use SF_int (instead of SF_uint) to allow connecting the value
  // field of an Incrementor, which is of int type (and not Uint) to this
  // field.
  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Ego::model_changed);

  exec_func = static_cast<Execution_function>(&Ego::voxels_changed);  
  s_prototype->add_field_info(new SF_float(VOXEL_WIDTH, "voxelWidth",
                                           get_member_offset(&m_voxel_width),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(VOXEL_LENGTH, "voxelLength",
                                           get_member_offset(&m_voxel_length),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(VOXEL_HEIGHT, "voxelHeight",
                                           get_member_offset(&m_voxel_height),
                                           exec_func));

  exec_func = static_cast<Execution_function>(&Ego::tiling_changed);
  SF_int* sf_int =
    new SF_int(FIRST_TILE_PLACEMENT, "firstTilePlacement",
               get_member_offset(&m_first_tile_placement), exec_func);
  s_prototype->add_field_info(sf_int);
  
  s_prototype->add_field_info(new SF_int(TILING_STRATEGY,
                                         "tilingStrategy",
                                         get_member_offset(&m_tiling_strategy),
                                         exec_func));

  sf_int = new SF_int(TILING_ROWS_DIRECTION, "tilingRowsDirection",
                      get_member_offset(&m_tiling_rows_direction), exec_func);
  s_prototype->add_field_info(sf_int);
  
  exec_func = static_cast<Execution_function>(&Shape::appearance_changed);
  s_prototype->add_field_info(new SF_container(APPEARANCE, "appearance",
                                               get_member_offset(&m_appearance),
                                               exec_func));
  exec_func = static_cast<Execution_function>(&Ego::visibility_changed);
  sf_int = new SF_int(LAYER_X_VISIBILITY, "layerXVisibility",
                      get_member_offset(&m_layer_x_visibility), exec_func);
  s_prototype->add_field_info(sf_int);

  sf_int = new SF_int(LAYER_Y_VISIBILITY, "layerYVisibility",
                      get_member_offset(&m_layer_y_visibility), exec_func);
  s_prototype->add_field_info(sf_int);

  sf_int = new SF_int(LAYER_Z_VISIBILITY, "layerZVisibility",
                      get_member_offset(&m_layer_z_visibility), exec_func);
  s_prototype->add_field_info(sf_int);  

  exec_func = static_cast<Execution_function>(&Ego::selected_changed);
  s_prototype->add_field_info(new SF_bool(SELECTED, "selected",
                                          get_member_offset(&m_selected),
                                          exec_func));  
}

/*! \brief deletes the container prototype */
void Ego::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto* Ego::get_prototype()
{
  if (!s_prototype) Ego::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this node */
void Ego::set_attributes(Element* elem)
{
  Transform::set_attributes(elem);
  
  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  Multi_cont_attr_iter mcai;
  for (mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const std::string& name = elem->get_name(mcai);
    Cont_list& cont_list = elem->get_value(mcai);
    if (name == "parts") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        // Container* cont = *ci;
        // Spherical_gaussian_map_geo* sgm =
        //   dynamic_cast<Spherical_gaussian_map_geo*>(cont);
        // if (sgm) insert_sgm(sgm);
        // else {
        //   std::cerr << "Invalid " << s_tag << " geometry nodes!"
        //             << std::endl;
        // }
      }
      elem->mark_delete(mcai);
    }
    continue;
  }
  
  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "model") {
      Exact_polyhedron_geo* epoly = dynamic_cast<Exact_polyhedron_geo*>(cont);
      if (epoly != NULL) {
        set_model(epoly);
        elem->mark_delete(cai);
        continue;
      }
      Polyhedron_geo* poly = dynamic_cast<Polyhedron_geo*>(cont);
      if (poly != NULL) {
        set_model(poly);
        elem->mark_delete(cai);
        continue;
      }
      Mesh_set* mesh = dynamic_cast<Mesh_set*>(cont);
      if (mesh->is_dirty()) mesh->clean();
      set_model(mesh);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "appearance") {
      Appearance* app = dynamic_cast<Appearance*>(cont);
      set_appearance(app);
      elem->mark_delete(cai);
      continue;
    }
  }

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "spaceFilling") {
      set_space_filling(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    } 
    if (name == "voxelWidth") {
      set_voxel_width(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "voxelLength") {
      set_voxel_length(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
     if (name == "voxelheight") {
      set_voxel_height(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "firstTilePlacement") {
      size_t val = boost::lexical_cast<size_t>(value);
      set_first_tile_placement
        (static_cast<Ego_voxels_tiler::First_tile_placement>(val));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "tilingStrategy") {
      size_t val = boost::lexical_cast<size_t>(value);
      set_tiling_strategy(static_cast<Ego_voxels_tiler::Strategy>(val));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "tilingRowsDirection") {
      size_t val = boost::lexical_cast<size_t>(value);
      set_tiling_rows_direction
        (static_cast<Ego_voxels_tiler::Tiling_rows>(val));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "style") {
      Uint num = sizeof(s_style_names) / sizeof(char*);
      const char** found = std::find(s_style_names,
                                     &s_style_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_style_names;
      if (index < num) set_style(static_cast<Style>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "knobSlices") {
      set_knob_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "colorSpace") {
      Uint num = sizeof(s_color_space_names) / sizeof(char*);
      const char** found = std::find(s_color_space_names,
                                     &s_color_space_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_color_space_names;
      if (index < num) set_color_space(static_cast<Color_space>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerX") {
      set_layer_x(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerY") {
      set_layer_y(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerZ") {
      set_layer_z(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerXVisibility") {
      Uint num = sizeof(s_layer_visibility_names) / sizeof(char*);
      const char** found = std::find(s_layer_visibility_names,
                                     &s_layer_visibility_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_layer_visibility_names;
      if (index < num)
        set_layer_x_visibility(static_cast<Layer_visibility>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerYVisibility") {
      Uint num = sizeof(s_layer_visibility_names) / sizeof(char*);
      const char** found = std::find(s_layer_visibility_names,
                                     &s_layer_visibility_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_layer_visibility_names;
      if (index < num)
        set_layer_y_visibility(static_cast<Layer_visibility>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "layerZVisibility") {
      Uint num = sizeof(s_layer_visibility_names) / sizeof(char*);
      const char** found = std::find(s_layer_visibility_names,
                                     &s_layer_visibility_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_layer_visibility_names;
      if (index < num)
        set_layer_z_visibility(static_cast<Layer_visibility>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "brickTypes") {
      Uint num_values = get_num_tokens(value);
      Uint size = num_values / 3;
      m_brick_types.resize(size);

      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0 ; i < size ; i++) {
        svalue >> m_brick_types[i][0] >> m_brick_types[i][1] >> m_brick_types[i][2];
      }
      elem->mark_delete(ai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief determines whether the representation empty */
Boolean Ego::is_empty() { return true; }

/*! \brief clean the voxels */
void Ego::clean_voxels()
{
  if (m_dirty_voxels) {
    m_dirty_voxels = false;
    m_dirty_tiling = true;

    Ego_voxelizer voxelize(m_voxel_length, m_voxel_width, m_voxel_height);
    Ego_voxels_filler fill;

    m_voxels = Ego_voxels(); // Clear - should we make a func?
    if (this->is_model_polyhedron())
      m_voxels_center = 
        voxelize(this->get_polyhedron_model()->get_polyhedron(),
                 get_matrix(), &m_voxels);
    else if (this->is_model_exact_polyhedron())
      m_voxels_center = 
        voxelize(this->get_exact_polyhedron_model()->get_polyhedron(),
                 get_matrix(), &m_voxels);
    else if (this->is_model_geo_set())
      m_voxels_center = 
        voxelize(*(this->get_geo_set_model()), get_matrix(), &m_voxels);

    fill(&m_voxels);
  }
}

/*! \brief clean the tiling. */
void Ego::clean_tiling()
{
  m_dirty_tiling = false;
  clear_parts();
    
  Ego_voxels_tiler tile(m_first_tile_placement,
                        m_tiling_strategy,
                        m_tiling_rows_direction,
                        convert_types(m_brick_types));
  tile(&m_voxels);
}

/*! \brief (re)generate the parts. */
void Ego::clean_parts()
{
  m_owned_parts = true;

  // Add the parts:
  Vector3f center(CGAL::to_double(m_voxels_center.x()),
                  CGAL::to_double(m_voxels_center.y()),
                  CGAL::to_double(m_voxels_center.z()));
  Ego_voxels::size_type size = m_voxels.size();
  Vector3f box(m_voxel_length * size.get<0>(),
               m_voxel_width * size.get<1>(),
               m_voxel_height * size.get<2>());
  Vector3f origin;
  origin.add_scaled(center, -0.5f, box);

  for (std::size_t i = 0; i < size.get<0>(); ++i) {
    for (std::size_t j = 0; j < size.get<1>(); ++j) {
      for (std::size_t k = 0; k < size.get<2>(); ++k) {
        if (m_voxels.is_filled(i, j, k) == false)
          continue;

        boost::optional<Ego_voxels::size_type> brick =
          m_voxels.get_brick(i, j, k);
        if (!brick) continue;

        // Continue, if the visibility scheme implies invisibility:
        
        // Continue, if the brick is completely obscured:
        std::size_t num0(brick->get<0>());
        std::size_t num1(brick->get<1>());

        //! \todo In the following we check whether the brick is apparent.
        // A brick is not apparent if it is obscured from all directions
        // by opaque bricks. Currently we assume that all bricks are opaque. 
        Boolean apparent = false;
        if ((i == 0) || (j == 0) || (k == 0) ||
            (i >= size.get<0>()-num0) || (j >= size.get<1>()-num1) ||
            (k == size.get<2>()-1))
          apparent = true;
        if (!apparent) {
          for (std::size_t t0 = 0; t0 < num0; ++t0) {
            for (std::size_t t1 = 0; t1 < num1; ++t1) {
              if (!m_voxels.is_filled(i+t0, j+t1, k-1) ||
                  !m_voxels.is_filled(i+t0, j+t1, k+1))
              {
                apparent = true;
                break;
              }
            }
            if (apparent) break;
          }
        }
        if (!apparent) {
          for (std::size_t t0 = 0; t0 < num0; ++t0) {
            if (!m_voxels.is_filled(i+t0, j-1, k) ||
                !m_voxels.is_filled(i+t0, j+num1, k))
            {
              apparent = true;
              break;
            }
          }
        }
        if (!apparent) {
          for (std::size_t t1 = 0; t1 < num1; ++t1) {
            if (!m_voxels.is_filled(i-1, j+t1, k) ||
                !m_voxels.is_filled(i+num0, j+t1, k))
            {
              apparent = true;
              break;
            }
          }
        }
        if (!m_space_filling && !apparent) continue;

        m_voxel_signatures.push_back(boost::make_tuple(i, j, k));
        Transform* transform = new Transform;
        add_child(transform);

        Vector3f displacement(i*m_voxel_length, j*m_voxel_width,
                              k*m_voxel_height);
        Vector3f brick_center(origin);
        brick_center.add(displacement);

        // The offset is half a voxel for each knob.
        Vector3f offset(m_voxel_length * 0.5f * num0,
                        m_voxel_width * 0.5f * num1, m_voxel_height);

        brick_center.add(offset);
        transform->set_translation(brick_center);

        Shape* shape = new Shape;
        transform->add_child(shape);

       // Determine whether to draw the knobs:
        bool should_draw_knobs = false;
        for (size_t s = 0; s < brick->get<0>(); ++s) {
          for (size_t t = 0; t < brick->get<1>(); ++t) {
            if ((k == size.get<2>() - 1) ||
                (!m_voxels.is_filled(i+s, j+t, k+1)) ||
                (!m_voxels.is_placed(i+s, j+t, k+1)))
              should_draw_knobs = true;
          }
        }
        Appearance* app = NULL;
        Geometry* ego_brick = NULL;
        Vector3f tmp;
        switch (m_style) {
         case STYLE_APPEARANCE:
          m_dirty_colors = false;
          shape->set_override_blend_func(false);
          app = m_appearance;
          tmp.sub(center, brick_center);
          ego_brick = create_geometry(num0, num1, should_draw_knobs, tmp);
          break;

         case STYLE_RANDOM_COLORS:
          m_dirty_colors = false;
          app = create_random_appearance();
          ego_brick = create_geometry(num0, num1, should_draw_knobs);
          break;

         case STYLE_DISCRETE_CUBE_MAP:
          shape->set_override_tex_enable(false);
          shape->set_override_tex_env(false);
          shape->set_override_blend_func(false);
          shape->set_override_light_model(false);
          shape->set_override_tex_gen(false);
          shape->set_override_light_enable(false);
          m_dirty_colors = true;
          app = m_appearance;
          tmp.sub(center, brick_center);
          ego_brick = create_geometry(num0, num1, should_draw_knobs, tmp);
          break;
          
         default: std::cerr << "Invalid style!" << std::endl;
        }
        shape->set_appearance(app);
        shape->set_geometry(ego_brick);
      }
    }
  }
  Uint num_childs = get_child_count();
  std::cout << "# of children: " << num_childs << std::endl;

  // Add a touch sensor:
  m_touch_sensor = new Touch_sensor;
  m_touch_sensor->set_enabled((m_layer_x_visibility != LV_ALL) ||
                              (m_layer_y_visibility != LV_ALL) ||
                              (m_layer_z_visibility != LV_ALL));
  m_touch_sensor->set_num_selection_ids(num_childs);
  m_touch_sensor->add_to_scene(m_scene_graph);
  Field* src_field = m_touch_sensor->add_field(Touch_sensor::ISOVER);
  SGAL_assertion(src_field);
  Field* dst_field = add_field(SELECTED);
  SGAL_assertion(dst_field);
  src_field->connect(dst_field);
  add_child(m_touch_sensor);

  m_dirty_parts = false;
  m_dirty_visibility = true;
  m_dirty_sphere_bound = true;
}

/*! \brief */
void Ego::clean_colors()
{
  if (m_clean_colors_in_progress) return;
  
  //! \todo create a new context? and remove the (action) argument.
  Context* context = m_scene_graph->get_context();
  SGAL_assertion(context);
  Uint x0 = 0, y0 = 0, width = 0, height = 0;
  context->get_viewport(x0, y0, width, height);

  // Prepare draw action:
  Configuration* conf = m_scene_graph->get_configuration();
  SGAL_assertion(conf);
  SGAL::Draw_action draw_action(conf);
  draw_action.set_context(context);
  draw_action.set_snap(false);

  // Prepare isect action:
  SGAL::Isect_action isect_action;
  isect_action.set_context(context);
  m_selection_id = m_scene_graph->reserve_selection_ids(get_child_count());
  
  // Prepare color image:
  Image_base::Format format = Image_base::kRGB8_8_8;
  GLenum gl_format = Image_base::get_format_format(format);
  GLenum gl_type = Image_base::get_format_type(format);
  Uint size = Image_base::get_size(width, height, format);
  Uint num_components = Image_base::get_format_components(format);
  Uchar* pixels = new Uchar[size];

  // Prepare selection image:
  Image_base::Format format_select = Image_base::kRGB8_8_8;
  GLenum gl_format_select = Image_base::get_format_format(format_select);
  GLenum gl_type_select = Image_base::get_format_type(format_select);
  Uint size_select = Image_base::get_size(width, height, format_select);
  Uint num_components_select = Image_base::get_format_components(format_select);
  SGAL_assertion_code(Uint bits = Image_base::get_format_size(format_select));
  SGAL_assertion(m_scene_graph->get_num_selection_ids() < (0x1 << bits));
  Uchar* selections = new Uchar[size_select];
  
  // Set the clear color:
  Vector3f color(0, 0, 0);

  // Prepare the openGl state
  // - Adjust texture environment
  // - Disable lighting
  // - Set read and draw buffers
  m_appearance->set_tex_env(Gfx::REPLACE_TENV);
  m_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
  m_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
  m_appearance->set_light_enable(false);
  m_appearance->set_poly_mode(Gfx::FILL_PMODE);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  GLint val;
  glGetIntegerv(GL_READ_BUFFER, &val);
  GLenum read_buffer_mode = (GLenum)val;
  glReadBuffer(GL_FRONT);
  glGetIntegerv(GL_DRAW_BUFFER, &val);
  GLenum draw_buffer_mode = (GLenum)val;
  glDrawBuffer(GL_FRONT);

  // Create the cameras:
  Camera cameras[6];
  Ego_voxels::size_type voxel_size = m_voxels.size();
  Vector3f center(CGAL::to_double(m_voxels_center.x()),
                  CGAL::to_double(m_voxels_center.y()),
                  CGAL::to_double(m_voxels_center.z()));
  Vector3f box(m_voxel_length * voxel_size.get<0>(),
               m_voxel_width * voxel_size.get<1>(),
               m_voxel_height * voxel_size.get<2>());
  Vector3f disp(box);
  disp.scale(0.5);
  
  // -1,0,0
  cameras[0].set_position(center[0]-box[0], center[1], center[2]);
  cameras[0].set_orientation(-1, 0, 0, 0);
  cameras[0].get_frustum().make_ortho(center[2]-disp[2], center[2]+disp[2],
                                      center[1]-disp[1], center[1]+disp[1],
                                      0.1, box[0]+0.4);

  // +1,0,0
  cameras[1].set_position(center[0]+box[0], center[1], center[2]);
  cameras[1].set_orientation(1, 0, 0, 0);
  cameras[1].get_frustum().make_ortho(center[1]-disp[1], center[1]+disp[1],
                                      center[2]-disp[2], center[2]+disp[2],
                                      0.1, box[0]+0.4);
  
  // 0,-1,0
  cameras[2].set_position(center[0], center[1]-box[1], center[2]);
  cameras[2].set_orientation(0, -1, 0, 0);
  cameras[2].get_frustum().make_ortho(center[0]-disp[0], center[0]+disp[0],
                                      center[2]-disp[2], center[2]+disp[2],
                                      0.1, box[1]+0.4);

  // 0,+1,0
  cameras[3].set_position(center[0], center[1]+box[1], center[2]);
  cameras[3].set_orientation(0, 1, 0, 0);
  cameras[3].get_frustum().make_ortho(center[2]-disp[2], center[2]+disp[2],
                                      center[0]-disp[0], center[0]+disp[0],
                                      0.1, box[1]+0.4);

  // 0,0,-1
  cameras[4].set_position(center[0], center[1], center[2]-box[2]);
  cameras[4].set_orientation(0, 0, -1, 0);
  cameras[4].get_frustum().make_ortho(center[1]-disp[1], center[1]+disp[1],
                                      center[0]-disp[0], center[0]+disp[0],
                                      0.1, box[2]+0.4);

  // 0,0,+1
  cameras[5].set_position(center[0], center[1], center[2]+box[2]);
  cameras[5].set_orientation(0, 0, 1, 0);
  cameras[5].get_frustum().make_ortho(center[0]-disp[0], center[0]+disp[0],
                                      center[1]-disp[1], center[1]+disp[1],
                                      0.1, box[2]+0.4);

  // Draw
  m_clean_colors_in_progress = true;
  typedef std::pair<Vector3f,Uint>              Weighted_color;
  typedef std::vector<Weighted_color>           Weighted_color_vector;
  typedef Weighted_color_vector::iterator       Weighted_color_iter;
  Weighted_color_vector colors(get_child_count());
  std::fill(colors.begin(), colors.end(), Weighted_color(Vector3f(), 0));
  typedef std::vector<Uint>                     Weight_vector;
  typedef Weight_vector::iterator               Weight_iter;
  Weight_vector weights(get_child_count());
  for (Uint i = 0; i < 6; ++ i) {
    m_appearance->set_tex_enable(true);
    m_appearance->set_shade_model(Gfx::SMOOTH_SHADE);
    context->clear_color_depth_buffer(color);
    cameras[i].draw(&draw_action);

    Cull_context cull_context;
    cull_context.cull(this, &cameras[i]);
    cull_context.draw(&draw_action);
    glReadPixels(0, 0, width, height, gl_format, gl_type, pixels);

#if defined(EGO_VERBOSE)
    {
      std::string ext = ".jpg";
      std::ostringstream oss;
      oss << i;
      std::string file_name = "pixels_" + oss.str() + ext;
      Magick::Image image(width, height, "RGB", Magick::CharPixel, pixels);
      image.flip();
      image.magick(ext);
      image.write(file_name);
    }
#endif
    
    context->draw_shade_model(Gfx::FLAT_SHADE);
    context->draw_tex_enable(false);
    context->clear_color_depth_buffer(color);
    isect(&isect_action);
    glReadPixels(0, 0, width, height, gl_format_select, gl_type_select,
                 selections);

#if defined(EGO_VERBOSE)
    {
      std::string ext = ".png";
      std::ostringstream oss;
      oss << i;
      std::string file_name = "selections_" + oss.str() + ext;
      const char* type = (num_components_select = 3) ? "RGB" : "RGBA";
      Magick::Image image(width, height, type, Magick::CharPixel, selections);
      image.flip();
      image.magick(ext);
      image.write(file_name);
    }
#endif
    
    // process pixels & selections
    std::fill(weights.begin(), weights.end(), 0);
    Uint j;
    
    // Pass 1:
    for (j = 0; j < height; ++j) {
      for (Uint k = 0; k < width; ++k) {
        Uint pixel_id = j * width + k;
        Uint component_id = pixel_id * num_components_select;
        SGAL_assertion(component_id < size_select);
        Uint brick_id = isect_action.get_index(&selections[component_id]);
        if (brick_id == 0) continue;
        brick_id -= get_selection_id();
        SGAL_assertion(brick_id < get_child_count());
        ++(weights[brick_id]);
      }
    }

    // Pass 2:
    Weighted_color_iter cit;
    Weight_iter wit = weights.begin();
    for (cit = colors.begin(); cit < colors.end(); ++cit, ++wit) {
      if ((*wit == 0) || (cit->second == 0)) continue;
      Float factor = weights[j] / (cit->second + *wit);
      cit->first.scale(factor);
    }
    
    // Pass 3:
    for (j = 0; j < height; ++j) {
      for (Uint k = 0; k < width; ++k) {
        Uint pixel_id = j * width + k;
        Uint component_id = pixel_id * num_components_select;
        SGAL_assertion(component_id < size_select);
        Uint brick_id = isect_action.get_index(&selections[component_id]);
        if (brick_id == 0) continue;
        brick_id -= get_selection_id();
        SGAL_assertion(brick_id < get_child_count());
        component_id = pixel_id * num_components;
        SGAL_assertion(component_id < size);
        Vector3f new_color;
        if (m_color_space == COLOR_SPACE_HSL) {
          Magick::ColorRGB color_rgb(((Float) pixels[component_id]) / 255.0f,
                                     ((Float) pixels[component_id+1]) / 255.0f,
                                     ((Float) pixels[component_id+2]) / 255.0f);
          Magick::ColorHSL color_hsl(color_rgb);
          new_color.set(color_hsl.hue(), color_hsl.saturation(),
                        color_hsl.luminosity());
        }
        else
          new_color.set(((Float) pixels[component_id]) / 255.0f,
                        ((Float) pixels[component_id+1]) / 255.0f,
                        ((Float) pixels[component_id+2]) / 255.0f);
        SGAL_assertion(weights[brick_id] != 0);
        Float factor = 1.0f / weights[brick_id];
        colors[brick_id].first.add_scaled(factor, new_color);
      }
    }

    // Pass 4:
    wit = weights.begin();
    for (cit = colors.begin(); cit < colors.end(); ++cit, ++wit)
      cit->second += *wit;
  }
  m_clean_colors_in_progress = false;

  // Clean & Recover 
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  //! \todo traverse the shapes and reset the overrid_* flags and turn on the
  // dirty-appearance flag instead of the following. The first 3 should not
  // even be set, cause we are using the material and not the texture!
  m_appearance->set_shade_model(Gfx::SMOOTH_SHADE);
  m_appearance->clean_tex_enable();
  if (!conf->is_texture_map()) m_appearance->set_tex_enable(false);
  m_appearance->clean_tex_env();
  m_appearance->clean_blend_func();
  m_appearance->clean_light_model();
  m_appearance->set_light_enable(true);

  glReadBuffer(read_buffer_mode);
  glDrawBuffer(draw_buffer_mode);

  // Clear space:
  delete [] pixels;
  delete [] selections;
  weights.clear();

  // Update the colors:
  Node_iterator nit;
  Weighted_color_iter cit = colors.begin();
  for (nit = m_childs.begin(); nit != m_childs.end(); ++nit, ++cit) {
    Transform* transform = dynamic_cast<Transform*>(*nit);
    if (!transform) continue;

    Shape* shape = dynamic_cast<Shape*>(*(transform->children_begin()));
    shape->set_override_tex_enable(true);
    shape->set_override_tex_env(true);
    shape->set_override_blend_func(true);
    shape->set_override_light_model(true);
    shape->set_override_tex_gen(true);
    shape->set_override_light_enable(true);
    SGAL_assertion(shape);

#ifdef SGAL_EGO_VERBOSE
    std::cout << SGAL_EGO_VAR(cit->first) << std::endl;
#endif

    Appearance* app;
    if (m_color_space == COLOR_SPACE_HSL)
      app = create_appearance((Uint) (cit->first[0] * 255.0f),
                              (Uint) (cit->first[1] * 255.0f),
                              (Uint) (cit->first[2] * 255.0f));
    else {
      Magick::ColorRGB color_rgb(cit->first[0], cit->first[1], cit->first[2]);
      Magick::ColorHSL color_hsl(color_rgb);
      app = create_appearance((Uint) (color_hsl.hue() * 255.0f),
                              (Uint) (color_hsl.saturation() * 255.0f),
                              (Uint) (color_hsl.luminosity() * 255.0f));
    }
    shape->set_appearance(app);
  }
  colors.clear();
  m_dirty_colors = false;
}

/*! \brief creates the geometry of a brick. */
Geometry* Ego::create_geometry(Uint num0, Uint num1, Boolean draw_knobs)
{
  Ego_brick* ego_brick;
  if (draw_knobs) {
    Ego_brick_iter it = m_bricks.find(std::make_pair(num0, num1));
    if (it == m_bricks.end()) {
      ego_brick = new Ego_brick;
      ego_brick->set_knob_slices(m_knob_slices);
      ego_brick->set_number_of_knobs1(num0);
      ego_brick->set_number_of_knobs2(num1);
      ego_brick->set_knobs_visible(true);
      m_bricks.insert(std::make_pair(std::make_pair(num0, num1), ego_brick));
    }
    else ego_brick = (*it).second;
  }
  else {
    Ego_brick_iter it = m_knobless_bricks.find(std::make_pair(num0, num1));
    if (it == m_knobless_bricks.end()) {
      ego_brick = new Ego_brick;
      ego_brick->set_knob_slices(m_knob_slices);
      ego_brick->set_number_of_knobs1(num0);
      ego_brick->set_number_of_knobs2(num1);
      ego_brick->set_knobs_visible(false);
      m_knobless_bricks.insert(std::make_pair(std::make_pair(num0, num1),
                                              ego_brick));
    }
    else ego_brick = (*it).second;
  }

  return ego_brick;
}

/*! \brief creates the geometry of a brick. */
Geometry* Ego::create_geometry(Uint num0, Uint num1, Boolean draw_knobs,
                               Vector3f& center)
{
  Ego_brick* ego_brick = new Ego_brick;
  ego_brick->set_knob_slices(m_knob_slices);
  ego_brick->set_number_of_knobs1(num0);
  ego_brick->set_number_of_knobs2(num1);
  ego_brick->set_center(center);

  if (draw_knobs) {
    ego_brick->set_knobs_visible(true);
    Ego_brick_iter it = m_bricks.find(std::make_pair(num0, num1));
    if (it != m_bricks.end()) {
      Ego_brick* ref_ego_brick = (*it).second;
      ego_brick->set_coord_array(ref_ego_brick->get_coord_array());
      ego_brick->set_normal_array(ref_ego_brick->get_normal_array());
      ego_brick->set_coord_indices(ref_ego_brick->get_coord_indices());
      ego_brick->set_indices_flat(true);
    }
    m_bricks.insert(std::make_pair(std::make_pair(num0, num1), ego_brick));
  }
  else {
    Ego_brick_iter it = m_knobless_bricks.find(std::make_pair(num0, num1));
    ego_brick->set_knobs_visible(false);
    if (it != m_knobless_bricks.end()) {
      Ego_brick* ref_ego_brick = (*it).second;
      ego_brick->set_coord_array(ref_ego_brick->get_coord_array());
      ego_brick->set_normal_array(ref_ego_brick->get_normal_array());
      ego_brick->set_coord_indices(ref_ego_brick->get_coord_indices());
      ego_brick->set_indices_flat(true);
    }
    m_knobless_bricks.insert(std::make_pair(std::make_pair(num0, num1),
                                            ego_brick));
  }

  return ego_brick;
}

/*! \brief draws the geometry. */
Action::Trav_directive Ego::draw(Draw_action* action)
{
  if (m_dirty_appearance) clean_appearance();
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();
  if (m_dirty_colors) clean_colors();
  if (m_dirty_visibility) clean_visibility();
  return Group::draw(action);
}

/*! \brief */
void Ego::cull(Cull_context& cull_context)
{
  if (m_dirty_appearance) clean_appearance();
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();
  if (m_dirty_colors) clean_colors();
  if (m_dirty_visibility) clean_visibility();

  // We deliberately call the cull() member of the Group and not of the
  // Transform to avoid duplicate application of the transformations.
  Group::cull(cull_context);
}

/*! \brief draws the Ego object in selection mode. */
void Ego::isect(Isect_action* action)
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();
  reset_visibility();
  Group::isect(action);
}

/*! \brief */
void Ego::model_changed(Field_info* field_info)
{
  m_dirty_sphere_bound = true;
  voxels_changed(field_info);
}

/*! \brief */
void Ego::voxels_changed(Field_info* field_info)
{
  m_dirty_voxels = true;
  tiling_changed(field_info);
}

/*! \brief Process change of tiling scheme. */
void Ego::tiling_changed(Field_info*) { m_dirty_tiling = true; }

/*! \brief Process change of visibility scheme. */
void Ego::visibility_changed(Field_info*)
{
  m_touch_sensor->set_enabled((m_layer_x_visibility != LV_ALL) ||
                              (m_layer_y_visibility != LV_ALL) ||
                              (m_layer_z_visibility != LV_ALL));
  m_dirty_visibility = true;
}

/*! \brief Process change of visibility scheme. */
void Ego::clean_visibility()
{
  std::size_t index = 0;
  Node_iterator it = m_childs.begin();
  while (it != m_childs.end()) {
    Node* node = *it++;
    Transform* transform = dynamic_cast<Transform*>(node);
    if (!transform) break;
    std::size_t i, j, k;
    boost::tie(i, j, k) = m_voxel_signatures[index++];
    transform->set_visible(is_visible(m_layer_x_visibility, m_layer_x, i) &&
                           is_visible(m_layer_y_visibility, m_layer_y, j) &&
                           is_visible(m_layer_z_visibility, m_layer_z, k));
  }
  m_dirty_visibility = false;
}

/*! \brief Process change of visibility scheme. */
void Ego::reset_visibility()
{
  Node_iterator it = m_childs.begin();
  while (it != m_childs.end()) {
    Node* node = *it++;
    Transform* transform = dynamic_cast<Transform*>(node);
    if (!transform) break;
    transform->set_visible(true);
  }
  m_dirty_visibility = true;
}

/*! \brief calculate sphere bound of the node. */
Boolean Ego::clean_sphere_bound()
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();
  if (m_dirty_visibility) clean_visibility();

  Ego_voxels::size_type size = m_voxels.size();
  Vector3f box(m_voxel_length * size.get<0>(),
               m_voxel_width * size.get<1>(),
               m_voxel_height * size.get<2>());
  Vector3f center(CGAL::to_double(m_voxels_center.x()),
                  CGAL::to_double(m_voxels_center.y()),
                  CGAL::to_double(m_voxels_center.z()));
  float radius = box.length() * 0.5f;
  m_sphere_bound.set_center(center);
  m_sphere_bound.set_radius(radius);
  m_dirty_sphere_bound = false;
  return true;
}

/*! \brief converts m_brick_types to the input received by Ego_voxels_tiler. */  
Ego_voxels_tiler::Brick_types
Ego::convert_types(const SGAL::Array<Vector3sh>& types)
{
  Ego_voxels_tiler::Brick_types ret;
  BOOST_FOREACH(const Vector3sh& vec, types) {
    ret.push_back(boost::make_tuple(vec[0], vec[1], vec[2]));
  }
  return ret;
}

/*! \brief adds the container to a given scene. */  
void Ego::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

/*! \brief sets the appearance of the object. */
void Ego::set_appearance(Appearance* app)
{
  m_appearance = app;
  if (m_style != STYLE_RANDOM_COLORS) m_dirty_appearance = true;
}

/*! \brief processes change of appearance. */
void Ego::appearance_changed(Field_info* /* field_info. */)
{ if (m_style != STYLE_RANDOM_COLORS) m_dirty_appearance = true; }

/*! breif cleans the apperance. */
void Ego::clean_appearance()
{
  // Construct a new owned appearance if needed, and delete the previously
  // constructed owned appearance if not needed any more.
  if (m_owned_appearance) {
    if (!m_appearance) m_appearance = m_appearance_prev;
    else if (m_appearance != m_appearance_prev) {
      delete m_appearance_prev;
      m_appearance_prev = NULL;
      m_owned_appearance = false;
    }
  }
  else {
    if (!m_appearance) {
      m_appearance = new Appearance;
      SGAL_assertion(m_appearance);
      m_owned_appearance = true;
    }
  }
  m_appearance_prev = m_appearance;
  m_dirty_appearance = false;
}

/*! \brief creates an appearance. */
Appearance* Ego::create_appearance(Uint hue_key, Uint saturation_key,
                                   Uint luminosity_key)
{
  Uint color_key =
    (((hue_key << 8) | saturation_key) << 8) | luminosity_key;
  Appearance_iter ait = m_appearances.find(color_key);
  if (ait != m_appearances.end()) return ait->second;

  Float hue = (Float) hue_key / 255.0f;
  Float saturation = (Float) saturation_key / 255.0f;
  Float luminosity = (Float) luminosity_key / 255.0f;

  Appearance* app = new Appearance;
  Material* mat = new Material;
  m_materials.push_back(mat);
  Magick::ColorHSL color_hsl(hue, saturation, luminosity);
  Magick::ColorRGB color_rgb(color_hsl);
  Float red = color_rgb.red();
  Float green = color_rgb.green();
  Float blue = color_rgb.blue();
  mat->set_diffuse_color(red, green, blue);
  if (m_appearance && m_appearance->get_material())
    mat->set_transparency(m_appearance->get_material()->get_transparency());
  app->set_material(mat);
  m_appearances[color_key] = app;

  return app;
}

/*! \brief creates a random appearance. */
Appearance* Ego::create_random_appearance()
{
  Uint hue_key = std::rand() % 256;
  Uint saturation_key = std::rand() % 256;
  Uint luminosity_key = 128;
  return create_appearance(hue_key, saturation_key, luminosity_key);
}

/*! \brief sets the style. */
void Ego::set_style(Style style)
{
  m_style = style;
  if (m_style == STYLE_APPEARANCE) m_dirty_appearance = true;
}

/*! \brief sets the color space. */
void Ego::set_color_space(Color_space color_space)
{
  m_color_space = color_space;
  // set something to clean.
}
  
/*! \brief sets the knob slicess number. */
void Ego::set_knob_slices(Uint slices) { m_knob_slices = slices; }

/*! \brief sets the flag that indicates whether the parts are space filling. */
void Ego::set_space_filling(Boolean flag) { m_space_filling = flag; }

/*! \brief determines whether a given brick is visible with respect to a 
 * specific layer.
 */
Boolean Ego::is_visible(Layer_visibility lv, Uint layer_index, Uint brick_index)
{
  switch (lv) {
   case LV_ALL: return true;
   case LV_ABOVE: return (brick_index > layer_index);
   case LV_NOT_ABOVE: return (brick_index <= layer_index);
   case LV_BELOW: return (brick_index < layer_index);
   case LV_NOT_BELOW: return (brick_index >= layer_index);
   case LV_ONLY: return (brick_index == layer_index);
   case LV_NOT_ONLY: return (brick_index != layer_index);
  }
  SGAL_error();
  return false;
}

/*! \brief processes change of selected brick. */
void Ego::selected_changed(Field_info* /* field_info. */)
{
  Uint id = m_touch_sensor->get_selection_id();
  if (id == 0) return;
  id -= m_touch_sensor->get_first_selection_id();
  SGAL_assertion(id < m_voxel_signatures.size());
  boost::tie(m_layer_x, m_layer_y, m_layer_z) = m_voxel_signatures[id];
  m_dirty_visibility = true;
}

SGAL_END_NAMESPACE
