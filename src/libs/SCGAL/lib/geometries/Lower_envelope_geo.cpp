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

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure used as a base class to all lower envelope nodes.
 */

#include <CGAL/Cartesian.h>

#include <time.h>
#include <list>

#include <boost/lexical_cast.hpp>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Node.hpp"

#include "SCGAL/Lower_envelope_geo.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Lower_envelope_geo::s_prototype(nullptr);

// Default Values:
Float Lower_envelope_geo::s_def_vertex_radius(0.2f);
Float Lower_envelope_geo::s_def_edge_radius(0.1f);
Float Lower_envelope_geo::s_def_face_transparency(0.8f);

//! \brief constructor.
Lower_envelope_geo::Lower_envelope_geo(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_time(0),
  m_vertex_radius(s_def_vertex_radius),
  m_edge_radius(s_def_edge_radius),
  m_face_transparency(s_def_face_transparency)
{}

//! \brief destructor.
Lower_envelope_geo::~Lower_envelope_geo() { clear(); }

//! \brief draws the envelope.
void Lower_envelope_geo::draw_envelope(Draw_action* action)
{
  draw_envelope_faces(action);
  draw_envelope_edges(action);
  draw_envelope_vertices(action);
}

//! \brief draws the geometry of the Mesh.
void Lower_envelope_geo::draw(Draw_action* action)
{
  if (m_dirty) clean();
  if (is_empty()) return;
  draw_envelope(action);
}

//! \brief cleans the bounding sphere of the lower envelope.
void Lower_envelope_geo::clean_sphere_bound()
{
  if (m_dirty) clean();
  if (is_empty()) return;

  Inexact_sphere_vector spheres;
  transform_coords(spheres);
  if (!spheres.empty()) {
    Min_sphere min_sphere(spheres.begin(), spheres.end());
    Vector3f center_vec;
    std::copy(min_sphere.center_cartesian_begin(),
              min_sphere.center_cartesian_end(),
              &center_vec[0]);
    m_sphere_bound.set_center(center_vec);
    m_sphere_bound.set_radius(min_sphere.radius());
  }

  m_dirty_sphere_bound = false;
}

//! \brief sets the attributes of the object extracted from an input file.
void Lower_envelope_geo::set_attributes(SGAL::Element* elem)
{
  Geometry::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "vertexRadius") {
      m_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "edgeRadius") {
      m_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "faceTransparency") {
      m_face_transparency = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "surfaces") {
      Shared_node node = boost::dynamic_pointer_cast<Node>(cont);
      if (node) add_surface(node);
      elem->mark_delete(cai);
      continue;
    }
  }

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
    if (name == "surfaces") {
      for (Cont_iter ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        Shared_node node = boost::dynamic_pointer_cast<Node>(cont);
        if (node) add_surface(node);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the prototype of this container.
void Lower_envelope_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // vertexRadius
  Float_handle_function vertex_radius_func =
    static_cast<Float_handle_function>
    (&Lower_envelope_geo::vertex_radius_handle);
  s_prototype->add_field_info(new SF_float(VERTEX_RADIUS, "vertexRadius",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           vertex_radius_func));

  // edgeRadius
  Float_handle_function edge_radius_func =
    static_cast<Float_handle_function>
    (&Lower_envelope_geo::edge_radius_handle);
  s_prototype->add_field_info(new SF_float(EDGE_RADIUS, "edgeRadius",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           edge_radius_func));

  // faceTransparency
  Float_handle_function face_transparency_func =
    static_cast<Float_handle_function>
    (&Lower_envelope_geo::face_transparency_handle);
  s_prototype->add_field_info(new SF_float(FACE_TRANSPARENCY,
                                           "faceTransparency",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           face_transparency_func));
}

//! \brief deletes the prototype of this container.
void Lower_envelope_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype of this container.
Container_proto* Lower_envelope_geo::get_prototype()
{
  if (!s_prototype) Lower_envelope_geo::init_prototype();
  return s_prototype;
}

//! \brief prints statistics.
void Lower_envelope_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (m_dirty) clean();

  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty()) clean();
  std::cout << "Construction took " << m_time << " seconds." << std::endl;

  std::cout << "Construction took " << m_time << " seconds.\n";
}

SGAL_END_NAMESPACE
