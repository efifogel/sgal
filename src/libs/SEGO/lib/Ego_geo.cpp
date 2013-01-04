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

#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego_geo.hpp"
#include "SEGO/Ego_voxels_filler.hpp"
#include "SEGO/Ego_brick.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego_geo::s_tag = "Ego";
Container_proto* Ego_geo::s_prototype = NULL;

const Float Ego_geo::s_def_scale(1);

REGISTER_TO_FACTORY(Ego_geo, "Ego_geo");

/*! Constructor */
Ego_geo::Ego_geo(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
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
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

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
      Exact_polyhedron_geo* poly = dynamic_cast<Exact_polyhedron_geo*>(cont);
      if (poly != NULL) {
        set_model(poly);
        elem->mark_delete(cai);
        continue;
      } else {
        Geo_set* ifs = dynamic_cast<Geo_set*>(cont);
        set_model(ifs);
        elem->mark_delete(cai);
        continue;
      }
    }
  }

  typedef Element::Str_attr_iter Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "scale") {
      set_scale(boost::lexical_cast<Float>(value));
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
const Exact_polyhedron_geo* Ego_geo::get_polyhedron_model() const {
  return boost::get<Exact_polyhedron_geo*>(m_model);
}

const Geo_set* Ego_geo::get_geo_set_model() const {
  return boost::get<Geo_set*>(m_model);
}

/*! \brief clean the representation */
void Ego_geo::clean()
{
  // This is temp code until we have something that can visualize it better.
  const double dx = 0.1 / m_scale;
  const double dy = 0.1 / m_scale;
  const double dz = 0.1 / m_scale;

  m_ego_brick.set_number_of_knobs1(1);
  m_ego_brick.set_number_of_knobs2(1);

  m_ego_brick.set_pitch(dx - 0.2 * dx);
  m_ego_brick.set_height(dz - 0.01 * dz);
  m_ego_brick.set_knob_radius(0.25 * dx);
  m_ego_brick.set_knob_height(0.25 * dx);
  m_ego_brick.set_tolerance(0.1 * dx);

  m_ego_brick_without_knobs.set_number_of_knobs1(1);
  m_ego_brick_without_knobs.set_number_of_knobs2(1);

  m_ego_brick_without_knobs.set_pitch(dx - 0.2 * dx);
  m_ego_brick_without_knobs.set_height(dz - 0.01 * dz);
  m_ego_brick_without_knobs.set_knob_radius(0.25 * dx);
  m_ego_brick_without_knobs.set_knob_height(0.25 * dx);
  m_ego_brick_without_knobs.set_tolerance(0.1 * dx);
  m_ego_brick_without_knobs.set_knobs_visible(false);
  
  Ego_voxelizer voxelize (dx, dy, dz);
  Ego_voxels_filler fill;

  if (this->is_model_polyhedron())
    voxelize(this->get_polyhedron_model()->get_polyhedron(), &m_voxels);
  else if (this->is_model_geo_set())
    voxelize(*(this->get_geo_set_model()), &m_voxels);

  fill(&m_voxels);

  

  m_dirty = false;
}

/*! \brief draws the geometry */
void Ego_geo::draw(Draw_action* action)
{
  if (m_dirty) clean();

  const double dx = 0.1 / m_scale;
  const double dy = 0.1 / m_scale;
  const double dz = 0.1 / m_scale;

  Ego_voxels::size_type size = m_voxels.size();

  for (std::size_t i = 0; i < size.get<0>(); ++i) {
    for (std::size_t j = 0; j < size.get<1>(); ++j) {
      for (std::size_t k = 0; k < size.get<2>(); ++k) {
        if (m_voxels.is_filled(i, j, k) == false)
          continue;
        
        double x = CGAL::to_double(m_voxels.origin().x()) + i*dx;
        double y = CGAL::to_double(m_voxels.origin().y()) + j*dy;
        double z = CGAL::to_double(m_voxels.origin().z()) + k*dz;

        glPushMatrix();
        glTranslatef(x+dx/2, y+dy/2, z+dz/2);
        if ((k == size.get<2>() - 1) ||
            (!m_voxels.is_filled(i, j, k+1)))
          m_ego_brick.draw(action);
        else
          m_ego_brick_without_knobs.draw(action);
        glPopMatrix();
      }
    }
  }  

  // Draw the output
  //Context* context = action->get_context();
  //Camera* camera = context->get_active_camera();

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

/*! \brief */
Boolean Ego_geo::calculate_sphere_bound()
{
  return true;
}

SGAL_END_NAMESPACE
