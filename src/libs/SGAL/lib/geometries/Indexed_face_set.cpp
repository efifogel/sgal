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

#include <iostream>
#include <sstream>
#include <algorithm>
#include <tuple>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Tex_coord_array_3d.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/calculate_multiple_normals_per_vertex.hpp"

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

const std::string Indexed_face_set::s_tag = "IndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype(nullptr);

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

//! \brief constructor.
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Boundary_set(proto),
  m_dirty_polyhedron(true)
{
  if (proto) return;
  m_surface.set_mesh_set(this);
  //! \todo move crease_angle to here.
  set_crease_angle(0);
  set_normal_per_vertex(true);
  set_color_per_vertex(true);
}

//! \brief destructor.
Indexed_face_set::~Indexed_face_set(){}

//! Determine whether the representation is empty.
Boolean Indexed_face_set::is_empty() const
{
  return m_coord_indices.empty() && m_flat_coord_indices.empty() &&
    m_polyhedron.empty();
}

//! \brief claculates the normals in case they are invalidated.
void Indexed_face_set::clean_normals()
{
  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_polyhedron) clean_polyhedron();
    if (m_smooth) calculate_single_normal_per_vertex();
    else if (m_creased) calculate_normal_per_polygon();
    else calculate_multiple_normals_per_vertex();
  }
  else if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle == 0) calculate_normal_per_polygon();
  else SGAL_error();
  m_dirty_normals = false;
  m_normals_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief determines whether the surface is smooth.
Boolean Indexed_face_set::is_smooth(const Vector3f& normal1,
                                    const Vector3f& normal2) const
{
  float angle = acosf(normal1.dot(normal2));
  return (angle > m_crease_angle);
}

//! \brief calculates multiple normals per vertex for all vertices.
void Indexed_face_set::calculate_multiple_normals_per_vertex()
{
  m_flat_normal_indices.resize(m_flat_coord_indices.size());
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array());
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->clear();

  SGAL::calculate_multiple_normals_per_vertex(m_polyhedron,
                                              m_normal_array,
                                              m_flat_normal_indices);
  m_dirty_normal_indices = true;
  m_dirty_flat_normal_indices = false;
}

//! \brief sets the attributes of the object.
void Indexed_face_set::set_attributes(Element* elem)
{ Boundary_set::set_attributes(elem); }

#if 0
/*! Obtain a list of atytributes in this object. This method is called only
 * from the Builder side.
 * An Indexed_face_set is always converted to an ProgIndexedTriSet
 * and therefore any information regarding compressed data is not
 * written out (e.g., coordIndex).
 *
 * \return a list of attributes
 */
Attribute_list Indexed_face_set::get_attributes()
{
  Attribute_list attribs;
  Attribute attrib;
  char buf[32];
  Vector3f col;

  attribs = Boundary_set::get_attributes();
  return attribs;
}

#endif

//! \brief initializes the container prototype.
void Indexed_face_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Boundary_set::get_prototype());
}

//! \brief deletes the container prototype.
void Indexed_face_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Indexed_face_set::get_prototype()
{
  if (s_prototype == NULL) Indexed_face_set::init_prototype();
  return s_prototype;
}

//! \brief cleans the data structure.
void Indexed_face_set::clean_polyhedron()
{
  if (!m_coord_array || m_coord_array->size() == 0) return;
  m_polyhedron.delegate(m_surface);           // create the polyhedral surface
#if 0
  if (!m_polyhedron.normalized_border_is_valid()) {
    m_polyhedron.normalize_border();
  }
#else
  m_polyhedron.normalize_border();
#endif

  // Clean the facets
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Facet_normal_calculator());

  // Clean the halfedges
  Edge_normal_calculator edge_normal_calculator(get_crease_angle());
  edge_normal_calculator =
    std::for_each(m_polyhedron.edges_begin(), m_polyhedron.edges_end(),
                  edge_normal_calculator);
  m_smooth = edge_normal_calculator.m_smooth;
  m_creased = edge_normal_calculator.m_creased;

  m_dirty_polyhedron = false;
}

//! \brief sets the polyhedron data-structure.
void Indexed_face_set::set_polyhedron(Polyhedron& polyhedron)
{
  m_polyhedron = polyhedron;
  m_dirty_polyhedron = false;
}

//! \brief obtains the polyhedron data-structure.
const Indexed_face_set::Polyhedron& Indexed_face_set::get_polyhedron()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_polyhedron;
}

SGAL_END_NAMESPACE
