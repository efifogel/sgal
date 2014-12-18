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

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4390 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#pragma warning ( disable : 4996 )
#endif

#include <time.h>
#include <vector>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"

#include "SCGAL/Exact_nef_polyhedron.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_nef_polyhedron::s_tag = "ExactNefPolyhedron";
Container_proto* Exact_nef_polyhedron::s_prototype(nullptr);

REGISTER_TO_FACTORY(Exact_nef_polyhedron, "Exact_nef_polyhedron");

//! \brief constructor.
Exact_nef_polyhedron::Exact_nef_polyhedron(Boolean proto) :
  Mesh_set(proto),
  m_dirty_polyhedron(true),
  m_dirty_nef_polyhedron(true),
  m_time(0)
{ m_surface.set_nef_polyhedron(this); }

//! \brief destructor.
Exact_nef_polyhedron::~Exact_nef_polyhedron() {}

//! \brief updates the polyhedron data structure.
void Exact_nef_polyhedron::clean_polyhedron()
{
  m_dirty_polyhedron = false;

  // Construct the polyhedron:
  m_polyhedron.delegate(m_surface);
#if 0
  if (!m_polyhedron.normalized_border_is_valid()) {
    m_polyhedron.normalize_border();
  }
#else
  m_polyhedron.normalize_border();
#endif

  // Compute the plane equations:
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Plane_equation());

#if 0
  std::for_each(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
                Convert_inexact_point());
#endif
}

//! \brief clears the internal representation.
void Exact_nef_polyhedron::clear_polyhedron()
{
  m_dirty_polyhedron = true;
  m_polyhedron.clear();
}

//! \brief cleans the data structure.
void Exact_nef_polyhedron::clean_nef_polyhedron()
{
  m_dirty_nef_polyhedron = false;
  m_nef_polyhedron = Nef_polyhedron_3(m_polyhedron);

  clock_t start_time = clock();
  if (m_dirty_polyhedron) clean_polyhedron();
  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
}

//! \brief clears the internal representation.
void Exact_nef_polyhedron::clear_nef_polyhedron()
{
  m_dirty_nef_polyhedron = true;
  m_nef_polyhedron.clear();
  clear_polyhedron();
}

//! \brief draws the intermediate polyhedron (for debugging purpose).
void Exact_nef_polyhedron::draw_polyhedron(Draw_action * action)
{
  if (m_dirty_polyhedron) clean_polyhedron();
  Polyhedron::Facet_const_iterator i;
  for (i = m_polyhedron.facets_begin(); i != m_polyhedron.facets_end(); ++i) {
    Polyhedron::Halfedge_around_facet_const_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);

    const Exact_vector_3& normal_3 = i->plane().orthogonal_vector();
    float x = static_cast<float>(CGAL::to_double(normal_3.x()));
    float y = static_cast<float>(CGAL::to_double(normal_3.y()));
    float z = static_cast<float>(CGAL::to_double(normal_3.z()));
    Vector3f normal(x, y, z);
    normal.normalize();
    glNormal3fv((float*)&normal);

    do {
      const Exact_point_3& point = j->vertex()->point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      glVertex3f(x, y, z);
    } while (++j != i->facet_begin());

    glEnd();
  }
}

//! \brief draws the nef polyhedron representation.
void Exact_nef_polyhedron::draw(Draw_action* action)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (is_dirty_nef_polyhedron()) clean_nef_polyhedron();
  if (is_nef_polyhedron_empty()) return;

  draw_mesh(action);
}

//! \brief draws the nef polyhedron geometry.
void Exact_nef_polyhedron::draw_geometry(Draw_action* action)
{
  if (is_nef_polyhedron_empty()) return;

  // draw_polyhedron(action);

  for (Halffacet_iterator f = m_nef_polyhedron.halffacets_begin();
       f != m_nef_polyhedron.halffacets_end(); ++f)
  {
    if (f->incident_volume() != m_nef_polyhedron.volumes_begin()) continue;

    // \todo do not ignore the holes!
    Nef_polyhedron_3::SHalfedge_const_handle se = f->facet_cycles_begin();
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j = se;
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j_begin = j;
    // Ensure facets are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);

    glBegin(GL_POLYGON);

    Exact_vector_3 normal = f->plane().orthogonal_vector();
    float x = static_cast<float>(CGAL::to_double(normal.x()));
    float y = static_cast<float>(CGAL::to_double(normal.y()));
    float z = static_cast<float>(CGAL::to_double(normal.z()));
    Vector3f normal3f(x,y,z);
    normal3f.normalize();
    glNormal3fv((float*)&normal3f);

    do {
      const Exact_point_3& point = j->source()->source()->point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      glVertex3f(x, y, z);
    } while (++j != j_begin);
    glEnd();
  }
}

/*! \brief */
void Exact_nef_polyhedron::isect(SGAL::Isect_action* action)
{
  if (is_dirty_nef_polyhedron()) clean_nef_polyhedron();
  if (is_nef_polyhedron_empty()) return;

  for (Halffacet_iterator f = m_nef_polyhedron.halffacets_begin();
       f != m_nef_polyhedron.halffacets_end(); ++f)
  {
    if (f->incident_volume() != m_nef_polyhedron.volumes_begin()) continue;

    // \todo do not ignore the holes!
    Nef_polyhedron_3::SHalfedge_const_handle se = f->facet_cycles_begin();
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j = se;
    Nef_polyhedron_3::SHalfedge_around_facet_const_circulator j_begin = j;
    // Ensure facets are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);

    glBegin(GL_POLYGON);
    do {
      const Exact_point_3& point = j->source()->source()->point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      glVertex3f(x, y, z);
    } while (++j != j_begin);
    glEnd();
  }
}

//! \brief cleans the bounding sphere of the nef polyhedron.
void Exact_nef_polyhedron::clean_sphere_bound_polyhedron()
{
  if (m_bb_is_pre_set) {
    m_dirty_sphere_bound = false;
    return;
  }

  Inexact_sphere_vector spheres;
  if (!m_polyhedron.empty()) {
    spheres.resize(m_polyhedron.size_of_vertices());
    Convert_inexact_sphere convert;
    std::transform(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
                   spheres.begin(), convert);
  }
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

//! \brief cleans the bounding sphere of the nef polyhedron.
void Exact_nef_polyhedron::clean_sphere_bound()
{
  if (m_bb_is_pre_set) {
    m_dirty_sphere_bound = false;
    return;
  }

  if (is_dirty_nef_polyhedron()) clean_nef_polyhedron();
  Inexact_sphere_vector spheres;
  if (!m_polyhedron.empty()) {
    spheres.resize(m_polyhedron.size_of_vertices());
    Convert_inexact_sphere convert;
    std::transform(m_nef_polyhedron.vertices_begin(),
                   m_nef_polyhedron.vertices_end(),
                   spheres.begin(), convert);
  }
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

//! \brief sets the attributes of this node.
void Exact_nef_polyhedron::set_attributes(SGAL::Element* elem)
{
  SGAL::Mesh_set::set_attributes(elem);

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the prototype of this container.
void Exact_nef_polyhedron::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new SGAL::Container_proto(Mesh_set::get_prototype());
}

//! \brief deletes the prototype of this container.
void Exact_nef_polyhedron::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype of this container.
SGAL::Container_proto* Exact_nef_polyhedron::get_prototype()
{
  if (!s_prototype) Exact_nef_polyhedron::init_prototype();
  return s_prototype;
}

//! \brief prints statistics.
void Exact_nef_polyhedron::print_stat()
{
  if (is_dirty_nef_polyhedron()) clean_nef_polyhedron();
  std::cout << "No statistics available!" << std::endl;
}

//! \brief obtains the Nef polyhedron.
Exact_nef_polyhedron::Nef_polyhedron_3&
Exact_nef_polyhedron::get_nef_polyhedron()
{
  if (is_dirty_nef_polyhedron()) clean_nef_polyhedron();
  return m_nef_polyhedron;
}

SGAL_END_NAMESPACE
