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

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Field_infos.hpp"

#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego_geo.hpp"
#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels_tiler.hpp"
#include "SEGO/Ego_voxelizer.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego_geo::s_tag = "Ego";
Container_proto* Ego_geo::s_prototype = NULL;

const Float Ego_geo::s_def_voxel_width(8.2);
const Float Ego_geo::s_def_voxel_length(8.2);
const Float Ego_geo::s_def_voxel_height(9.6);
const Float Ego_geo::s_def_scale(1);
const Ego_voxels_tiler::First_tile_placement 
Ego_geo::s_def_first_tile_placement(Ego_voxels_tiler::FIRST00);
const Ego_voxels_tiler::Strategy
Ego_geo::s_def_tiling_strategy(Ego_voxels_tiler::NONGRID);
const Ego_voxels_tiler::Tiling_rows
Ego_geo::s_def_tiling_rows_direction(Ego_voxels_tiler::YROWS);


REGISTER_TO_FACTORY(Ego_geo, "Ego_geo");

/*! Constructor */
Ego_geo::Ego_geo(Boolean proto) :
  Geometry(proto),
  m_voxel_width(s_def_voxel_width),
  m_voxel_lebgth(s_def_voxel_length),
  m_voxel_height(s_def_voxel_height),  
  m_first_tile_placement(s_def_first_tile_placement),
  m_tiling_strategy(s_def_tiling_strategy),
  m_tiling_rows_direction(s_def_tiling_rows_direction),
  m_dirty(true),
  m_voxels_dirty(true),
  m_tiling_dirty(true),
  m_scale(s_def_scale)
{
}

/*! Destructor */
Ego_geo::~Ego_geo()
{
  clear();
}

/*! \brief clear the internal representation and auxiliary data structures
 */
void Ego_geo::clear()
{
  m_dirty = true;
}

/*! \brief initializes the container prototype */
void Ego_geo::init_prototype()
{
    //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);
  
  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Ego_geo::tiling_changed);

  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // We use SF_int (instead of SF_uint) to allow connecting the value
  // field of an Incrementor, which is of int type (and not Uint) to this
  // field.
  s_prototype->add_field_info(new SF_float(VOXEL_WIDTH, "voxelWidth",
                                           get_member_offset(&m_voxel_width),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(VOXEL_LENGTH, "voxelLength",
                                           get_member_offset(&m_voxel_length),
                                           exec_func));

  s_prototype->add_field_info(new SF_float(VOXEL_HEIGHT, "voxelHeight",
                                           get_member_offset(&m_voxel_height),
                                           exec_func));

  s_prototype->add_field_info(new SF_int(FIRST_TILE_PLACEMENT,
                                         "firstTilePlacement",
                                         get_member_offset(&m_first_tile_placement),
                                         exec_func));
  
  s_prototype->add_field_info(new SF_int(TILING_STRATEGY,
                                         "tilingStrategy",
                                         get_member_offset(&m_tiling_strategy),
                                         exec_func));
  
  s_prototype->add_field_info(new SF_int(TILING_ROWS_DIRECTION,
                                         "tilingRowsDirection",
                                         get_member_offset(&m_tiling_rows_direction),
                                         exec_func));
  
}

/*! \brief deletes the container prototype */
void Ego_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto* Ego_geo::get_prototype()
{
  if (!s_prototype) Ego_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this node */
void Ego_geo::set_attributes(Element* elem)
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
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
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
Boolean Ego_geo::is_empty() { return true; }

/*! Obtain the model.
 * \return the model.
 */
const Polyhedron_geo* Ego_geo::get_polyhedron_model() const {
  return boost::get<Polyhedron_geo*>(m_model);
}

const Exact_polyhedron_geo* Ego_geo::get_exact_polyhedron_model() const {
  return boost::get<Exact_polyhedron_geo*>(m_model);
}

const Geo_set* Ego_geo::get_geo_set_model() const {
  return boost::get<Geo_set*>(m_model);
}

/*! \brief clean the representation */
void Ego_geo::clean()
{
  m_dirty = false;

  // This is temp code until we have something that can visualize it better.
  const double dx = m_voxel_length;
  const double dy = m_voxel_width;
  const double dz = m_voxel_height;

  m_ego_brick.set_number_of_knobs1(2);
  m_ego_brick.set_number_of_knobs2(2);

  m_ego_brick_without_knobs.set_number_of_knobs1(2);
  m_ego_brick_without_knobs.set_number_of_knobs2(2);

  if (m_voxels_dirty) {
    m_voxels_dirty = false;
    m_tiling_dirty = true;
    
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

  if (m_tiling_dirty) {
    m_tiling_dirty = false;
    
    m_tiled_voxels = m_voxels;
    Ego_voxels_tiler tile(m_first_tile_placement,
                          m_tiling_strategy,
                          m_tiling_rows_direction);
    tile(&m_tiled_voxels);
  }
}

/*! \brief draws the geometry */
void Ego_geo::draw(Draw_action* action)
{
  if (m_dirty) clean();

  const double dx = m_voxel_length;
  const double dy = m_voxel_width;
  const double dz = m_voxel_height;

  Ego_voxels::size_type size = m_tiled_voxels.size();

  for (std::size_t i = 0; i < size.get<0>(); ++i) {
    for (std::size_t j = 0; j < size.get<1>(); ++j) {
      for (std::size_t k = 0; k < size.get<2>(); ++k) {
        if (m_tiled_voxels.is_filled(i, j, k) == false)
          continue;
        
        double x = CGAL::to_double(m_tiled_voxels_origin.x()) + i*dx;
        double y = CGAL::to_double(m_tiled_voxels_origin.y()) + j*dy;
        double z = CGAL::to_double(m_tiled_voxels_origin.z()) + k*dz;

        glPushMatrix();
        glTranslatef(x+dx/2, y+dy/2, z+dz/2);

        // I don't know what is the best way to check.
        // Draw alwyas without knobs as this is heavy.
        boost::optional<Ego_voxels::size_type> brick =
          m_tiled_voxels.get_brick(i, j, k);

        if (brick) {
          SGAL_assertion(brick->get<0>() == 2 && brick->get<1>() == 2);
          
          bool should_draw_knobs = false;
          
          for (size_t s = 0; s < brick->get<0>(); ++s) {
            for (size_t t = 0; t < brick->get<1>(); ++t) {
              if ((k == size.get<2>() - 1) ||
                  (!m_tiled_voxels.is_filled(i+s, j+t, k+1)))
                should_draw_knobs = true;
            }
          }
          
          if (should_draw_knobs)
            m_ego_brick.draw(action);
          else
            m_ego_brick_without_knobs.draw(action);
        }

        glPopMatrix();
      }
    }
  }  

  // Draw the output
  //Context* context = action->get_context();
  //Camera* camera = context->get_active_camera();

}

void Ego_geo::tiling_changed(Field_info * ) {
  // We should do an optimization so we won't compute all the voxels each
  // time. For now, we do.
  
  m_dirty = true;
  m_tiling_dirty = true;
}


/*! \brief */
void Ego_geo::cull(Cull_context& cull_context)
{
}

/*! \brief */
void Ego_geo::isect(Isect_action* action)
{
  //! \todo isect with a sphere
}

/*! \brief calculate sphere bound of the node */
Boolean Ego_geo::calculate_sphere_bound()
{
  if (!m_is_sphere_bound_dirty) return false;

  if (m_dirty) clean();

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
  m_is_sphere_bound_dirty = false;
  return true;
}

void Ego_geo::adjust_voxels_for_tiling() {

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
