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

#include <Magick++.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"

#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego.hpp"
#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels_tiler.hpp"
#include "SEGO/Ego_voxelizer.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego::s_tag = "Ego";
Container_proto* Ego::s_prototype = NULL;

const Float Ego::s_def_voxel_width(8.2);
const Float Ego::s_def_voxel_length(8.2);
const Float Ego::s_def_voxel_height(9.6);
const Float Ego::s_def_scale(1);
const Ego_voxels_tiler::First_tile_placement 
Ego::s_def_first_tile_placement(Ego_voxels_tiler::FIRST00);
const Ego_voxels_tiler::Strategy
Ego::s_def_tiling_strategy(Ego_voxels_tiler::NONGRID);
const Ego_voxels_tiler::Tiling_rows
Ego::s_def_tiling_rows_direction(Ego_voxels_tiler::YROWS);

REGISTER_TO_FACTORY(Ego, "Ego");

/*! Constructor */
Ego::Ego(Boolean proto) :
  Group(proto),
  m_scale(s_def_scale),
  m_voxel_width(s_def_voxel_width),
  m_voxel_length(s_def_voxel_length),
  m_voxel_height(s_def_voxel_height),  
  m_first_tile_placement(s_def_first_tile_placement),
  m_tiling_strategy(s_def_tiling_strategy),
  m_tiling_rows_direction(s_def_tiling_rows_direction),
  m_dirty_voxels(true),
  m_dirty_tiling(true),
  m_dirty_parts(true),
  m_dirty_sphere_bound(true),
  m_own_parts(false)
{
  // This is temp code until we have something that can visualize it better.  
  m_ego_brick.set_number_of_knobs1(2);
  m_ego_brick.set_number_of_knobs2(2);
  m_ego_brick.set_knobs_visible(true);

  m_ego_brick_without_knobs.set_number_of_knobs1(2);
  m_ego_brick_without_knobs.set_number_of_knobs2(2);
  m_ego_brick_without_knobs.set_knobs_visible(false);
}

/*! Destructor */
Ego::~Ego() { clear(); }

/*! \brief clear the parts */
void Ego::clear_parts()
{
  m_dirty_parts = true;
  Node_iterator it1 = m_childs.begin();
  while (it1 != m_childs.end()) {
    // Remove the transform (translation):
    Transform* transform = static_cast<Transform*>(*it1++);
    Node_iterator it2 = transform->children_begin();
    while (it2 != transform->children_end()) {
      // Remove the brick (Shape):
      Shape* brick = static_cast<Shape*>(*it2);
      delete brick;
    }
    delete transform;
  }
  m_own_parts = false;
}

/*! \brief clear the internal representation and auxiliary data structures
 */
void Ego::clear()
{
  if (m_own_parts) clear_parts();
  
  m_dirty_sphere_bound = true;
  m_dirty_voxels = true;
  m_dirty_tiling = true;

  m_appearances.clear();
  m_materials.clear();
}

/*! \brief initializes the container prototype */
void Ego::init_prototype()
{
    //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);
  
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());

  // We use SF_int (instead of SF_uint) to allow connecting the value
  // field of an Incrementor, which is of int type (and not Uint) to this
  // field.
  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Ego::scale_changed);
  s_prototype->add_field_info(new SF_float(SCALE, "scale",
                                           get_member_offset(&m_scale),
                                           exec_func));

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
  typedef Element::Multi_cont_attr_iter   Multi_cont_attr_iter;
  typedef Element::Cont_list              Cont_list;
  typedef Element::Cont_iter              Cont_iter;

  // Sets the multi-container attributes of this node:
  for (Multi_cont_attr_iter mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); mcai++)
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
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
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
      Geo_set* ifs = dynamic_cast<Geo_set*>(cont);
      set_model(ifs);
      elem->mark_delete(cai);
      continue;
    }
  }

  typedef Element::Str_attr_iter Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
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
    if (name == "scale") {
      set_scale(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "firstTilePlacement") {
      size_t val = boost::lexical_cast<size_t>(value);
      set_first_tile_placement(static_cast<Ego_voxels_tiler::First_tile_placement>(val));
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
      set_tiling_rows_direction(static_cast<Ego_voxels_tiler::Tiling_rows>(val));
      elem->mark_delete(ai);
      continue;
    }

  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief determines whether the representation empty */
Boolean Ego::is_empty() { return true; }

/*! Obtain the model.
 * \return the model.
 */
const Polyhedron_geo* Ego::get_polyhedron_model() const {
  return boost::get<Polyhedron_geo*>(m_model);
}

const Exact_polyhedron_geo* Ego::get_exact_polyhedron_model() const {
  return boost::get<Exact_polyhedron_geo*>(m_model);
}

const Geo_set* Ego::get_geo_set_model() const {
  return boost::get<Geo_set*>(m_model);
}

/*! \brief clean the voxels */
void Ego::clean_voxels()
{
  if (m_dirty_voxels) {
    m_dirty_voxels = false;
    m_dirty_tiling = true;

    const double dx = m_voxel_length;
    const double dy = m_voxel_width;
    const double dz = m_voxel_height;

    Ego_voxelizer voxelize (dx, dy, dz);
    Ego_voxels_filler fill;

    m_voxels = Ego_voxels(); // Clear - should we make a func?
    if (this->is_model_polyhedron())
      m_tiled_voxels_origin = 
        voxelize(this->get_polyhedron_model()->get_polyhedron(),
                 m_scale, &m_voxels);
    if (this->is_model_exact_polyhedron())
      m_tiled_voxels_origin = 
        voxelize(this->get_exact_polyhedron_model()->get_polyhedron(),
                 m_scale, &m_voxels);
    else if (this->is_model_geo_set())
      m_tiled_voxels_origin = 
        voxelize(*(this->get_geo_set_model()), m_scale, &m_voxels);
    
    fill(&m_voxels);
    adjust_voxels_for_tiling();
  }
}

/*! \brief clean the tiling */
void Ego::clean_tiling()
{
  m_dirty_tiling = false;
  m_dirty_parts = true;
    
  m_tiled_voxels = m_voxels;
  Ego_voxels_tiler tile(m_first_tile_placement,
                        m_tiling_strategy,
                        m_tiling_rows_direction);
  tile(&m_tiled_voxels);
}

/*! \brief (re)generate the parts. */
void Ego::clean_parts()
{
  m_dirty_parts = false;
  m_dirty_sphere_bound = true;
  m_own_parts = true;

  const double dx = m_voxel_length;
  const double dy = m_voxel_width;
  const double dz = m_voxel_height;

  Ego_voxels::size_type size = m_tiled_voxels.size();

  for (std::size_t i = 0; i < size.get<0>(); ++i) {
    for (std::size_t j = 0; j < size.get<1>(); ++j) {
      for (std::size_t k = 0; k < size.get<2>(); ++k) {
        if (m_tiled_voxels.is_filled(i, j, k) == false)
          continue;
        
        boost::optional<Ego_voxels::size_type> brick =
          m_tiled_voxels.get_brick(i, j, k);

        if (!brick) continue;

        SGAL_assertion(brick->get<0>() == 2 && brick->get<1>() == 2);

        Transform* transform = new Transform;
        add_child(transform);
        
        double x = CGAL::to_double(m_tiled_voxels_origin.x()) + i*dx;
        double y = CGAL::to_double(m_tiled_voxels_origin.y()) + j*dy;
        double z = CGAL::to_double(m_tiled_voxels_origin.z()) + k*dz;
        transform->set_translation(x+dx/2, y+dy/2, z+dz/2);

        Shape* shape = new Shape;
        transform->add_child(shape);

        Uint hue_key = std::rand() % 256;
        Uint saturation_key = std::rand() % 256;
        // Uint luminosity_key = std::rand() % 256;
        Uint luminosity_key = 128;
        Uint color_key =
          (((hue_key << 8) | saturation_key) << 8) | luminosity_key;
        Appearance* app;
        Appearance_iter ait = m_appearances.find(color_key);
        if (ait == m_appearances.end()) {
          app = new Appearance;
          Material* mat = new Material;
          app->set_material(mat);
          Float hue = (Float) hue_key / 255.0;
          Float saturation = (Float) saturation_key / 255.0;
          Float luminosity = (Float) luminosity_key / 255.0;
          Magick::ColorHSL color_hsl(hue, saturation, luminosity);
          Magick::ColorRGB color_rgb(color_hsl);
          Float red = color_rgb.red();
          Float green = color_rgb.green();
          Float blue = color_rgb.blue();
          mat->set_diffuse_color(red, green, blue);
          m_appearances[color_key] = app;
        }
        else 
          app = ait->second;
        
        shape->set_appearance(app);
        
        bool should_draw_knobs = false;
        for (size_t s = 0; s < brick->get<0>(); ++s) {
          for (size_t t = 0; t < brick->get<1>(); ++t) {
            if ((k == size.get<2>() - 1) ||
                (!m_tiled_voxels.is_filled(i+s, j+t, k+1)))
              should_draw_knobs = true;
          }
        }
        Ego_brick* ego_brick = (should_draw_knobs) ?
          &m_ego_brick : &m_ego_brick_without_knobs;
        shape->set_geometry(ego_brick);
      }
    }
  }  
}

/*! \brief draws the geometry */
Action::Trav_directive Ego::draw(Draw_action* action)
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();

  return Group::draw(action);
}

/*! \brief */
void Ego::cull(Cull_context& cull_context)
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();

  Group::cull(cull_context);
}

/*! \brief */
void Ego::isect(Isect_action* action)
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();

  Group::isect(action);
}

void Ego::scale_changed(Field_info* field_info)
{
  m_dirty_sphere_bound = true;
  voxels_changed(field_info);
}

void Ego::voxels_changed(Field_info* field_info)
{
  m_dirty_voxels = true;
  tiling_changed(field_info);
}

void Ego::tiling_changed(Field_info*) { m_dirty_tiling = true; }

/*! \brief calculate sphere bound of the node */
Boolean Ego::calculate_sphere_bound()
{
  if (m_dirty_voxels) clean_voxels();
  if (m_dirty_tiling) clean_tiling();
  if (m_dirty_parts) clean_parts();
  if (!m_dirty_sphere_bound) return false;

  Ego_voxels::size_type size = m_tiled_voxels.size();
  Vector3f offset(m_voxel_length * size.get<0>() / 2,
                  m_voxel_width * size.get<1>() / 2,
                  m_voxel_height * size.get<2>() / 2);
  Vector3f center(CGAL::to_double(m_tiled_voxels_origin.x()),
                  CGAL::to_double(m_tiled_voxels_origin.y()),
                  CGAL::to_double(m_tiled_voxels_origin.z()));
  center.add(offset);
  float radius = offset.length();

  m_sphere_bound.set_center(center);
  m_sphere_bound.set_radius(radius);
  m_dirty_sphere_bound = false;
  return true;
}

void Ego::adjust_voxels_for_tiling() {

  // The current tiling uses 2x2 bricks and covers all the voxels.
  // This means that the tiling can go about one square off the current
  // voxels.
  // Therefore we need to "offset" the voxels sturcture in 1 square in
  // the xy-plane.
  
  // First, we move the origin. Don't touch z.
  const double dx = m_voxel_length;
  const double dy = m_voxel_width;
  Exact_polyhedron_geo::Kernel::Vector_3 diff(-dx, -dy, 0);
  m_tiled_voxels_origin = m_tiled_voxels_origin + diff;
  
  // Now offset the voxels.
  m_voxels.offset_xy_layers(1);
}

SGAL_END_NAMESPACE
