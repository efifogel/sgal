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
// $Id: $
// $Revision: 14183 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <CGAL/Cartesian.h>
#include <CGAL/Optimisation_d_traits_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "SGAL/Coord_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"

#include "SCGAL/Polyhedron_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Polyhedron_geo::s_tag = "Polyhedron";
SGAL::Container_proto* Polyhedron_geo::m_prototype = 0;

REGISTER_TO_FACTORY(Polyhedron_geo, "Polyhedron_geo");

/*! Constructor */
Polyhedron_geo::Polyhedron_geo(Boolean proto) :
  Mesh_set(proto)
{ m_surface.set_polyhedron(this); }

/*! Destructor */
Polyhedron_geo::~Polyhedron_geo()
{ m_polyhedron.clear(); }

/*! Clean the data structure */
void Polyhedron_geo::clean()
{
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON, "Polyhedron_geo::clean() start\n");
  m_polyhedron.delegate(m_surface);
#if 0
  if (!m_polyhedron.normalized_border_is_valid()) {
    m_polyhedron.normalize_border();
  }
#else
  m_polyhedron.normalize_border();
#endif
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Normal_vector());

  if (Mesh_set::is_dirty()) Mesh_set::clean();
}

/*! \brief clears the internal representation. */
void Polyhedron_geo::clear() { Mesh_set::clear(); }

/*! \brief */
void Polyhedron_geo::cull(SGAL::Cull_context& cull_context) {}

/*! \brief */
void Polyhedron_geo::draw_geometry(SGAL::Draw_action* /* action */)
{
  Facet_iterator i;
  for (i = m_polyhedron.facets_begin(); i != m_polyhedron.facets_end(); ++i) {
    Halfedge_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    Vector& normal = i->plane();
    glNormal3f(normal.x(), normal.y(), normal.z());
    do {
      Point& point = j->vertex()->point();
      glVertex3f(point.x(), point.y(), point.z());
    } while (++j != i->facet_begin());
    glEnd();
  }
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON, "completed\n");
}

/*! \brief */
void Polyhedron_geo::isect(SGAL::Isect_action* action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;

  Facet_iterator i;
  for (i = m_polyhedron.facets_begin(); i != m_polyhedron.facets_end(); ++i) {
    Halfedge_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    do {
      Point & point = j->vertex()->point();
      glVertex3f(point.x(), point.y(), point.z());
    } while (++j != i->facet_begin());
    glEnd();
  }
}

/*! \brief */
Boolean Polyhedron_geo::clean_sphere_bound()
{
  if (is_dirty()) clean();
  if (!m_bb_is_pre_set && !m_polyhedron.empty()) {
    Vector3f center_vec;

#if !defined(USE_MIN_SPHERE)
    // Work around: convert points to spheres with 0 radius:
    Sphere_vector spheres;
    spheres.resize(m_polyhedron.size_of_vertices());
    Extract_sphere extract;
    std::transform(m_polyhedron.points_begin(), m_polyhedron.points_end(),
                   spheres.begin(), extract);
    Min_sphere min_sphere(spheres.begin(), spheres.end());
    std::copy(min_sphere.center_cartesian_begin(),
              min_sphere.center_cartesian_end(),
              &center_vec[0]);
    m_sphere_bound.set_radius(min_sphere.radius());
#else
    Min_sphere min_sphere(m_polyhedron.points_begin(),
                          m_polyhedron.points_end());
    Point center = min_sphere.center();
    center_vec[0] = center.x();
    center_vec[1] = center.y();
    center_vec[2] = center.z();
    m_sphere_bound.set_radius(squarerootf(min_sphere.squared_radius()));
#endif

    m_sphere_bound.set_center(center_vec);
  }
  m_dirty_sphere_bound = false;
  return true;
}

/*! \brief sets the attributes of this object. */
void Polyhedron_geo::set_attributes(SGAL::Element* elem)
{
  SGAL::Mesh_set::set_attributes(elem);
}

/*! sets the attributes of this node */
void Polyhedron_geo::init_prototype()
{
  if (m_prototype) return;
  m_prototype = new SGAL::Container_proto(SGAL::Mesh_set::get_prototype());

  //! Container execution function
  typedef void (SGAL::Container::* Execution_function)(SGAL::Field_info*);
}

/*! \brief deletes the prototype. */
void Polyhedron_geo::delete_prototype()
{
  delete m_prototype;
  m_prototype = NULL;
}

/*! \brief obtaisn the prototype. */
SGAL::Container_proto* Polyhedron_geo::get_prototype() 
{  
  if (!m_prototype) Polyhedron_geo::init_prototype();
  return m_prototype;
}

/*! \brief obtains the polyhedron data-structure. */
Polyhedron_geo::Polyhedron& Polyhedron_geo::get_polyhedron()
{
  if (is_dirty()) clean();
  return m_polyhedron;
}

SGAL_END_NAMESPACE
