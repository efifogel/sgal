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
#include "SEGO/Ego_voxelizer.hpp"

SGAL_BEGIN_NAMESPACE

std::string Ego_geo::s_tag = "Ego";
Container_proto* Ego_geo::s_prototype = NULL;

REGISTER_TO_FACTORY(Ego_geo, "Ego_geo");

/*! Constructor */
Ego_geo::Ego_geo(Boolean proto) :
  Geometry(proto),
  m_model(NULL),
  m_dirty(true)
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
        Container* cont = *ci;
        // Spherical_gaussian_map_geo* sgm =
        //   dynamic_cast<Spherical_gaussian_map_geo*>(cont);
        // if (sgm) insert_sgm(sgm);
        else {
          std::cerr << "Invalid " << s_tag << " geometry nodes!"
                    << std::endl;
        }
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
      Exact_polyhedron_geo* model = dynamic_cast<Exact_polyhedron_geo*>(cont);
      set_model(model);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief determines whether the representation empty */
Boolean Ego_geo::is_empty() { return true; }

/*! \brief clean the representation */
void Ego_geo::clean()
{
  m_dirty = false;
  
  // Ego_voxelizer::Voxels voxels;
  // Ego_voxelizer voxelize (0.1, 0.1, 0.1);
  // voxelize(this->get_model()->get_polyhedron(), &voxels);
}

/*! \brief draws the geometry */
void Ego_geo::draw(Draw_action* action)
{
  if (m_dirty) clean();
  if (m_dirty) return;

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
