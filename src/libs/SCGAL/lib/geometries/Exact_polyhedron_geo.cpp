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
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <CGAL/basic.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Stl_formatter.hpp"
#include "SGAL/Vector3f.hpp"

#include "SCGAL/Exact_polyhedron_geo.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Exact_polyhedron_geo::s_tag = "ExactPolyhedron";
Container_proto* Exact_polyhedron_geo::s_prototype(NULL);

REGISTER_TO_FACTORY(Exact_polyhedron_geo, "Exact_polyhedron_geo");

//! \brief constructor.
Exact_polyhedron_geo::Exact_polyhedron_geo(Boolean proto) :
  Mesh_set(proto),
  m_convex_hull(false),
  m_dirty_polyhedron(true),
  m_dirty_facets(true),
  m_time(0)
{
  if (proto) return;
  m_surface.set_mesh_set(this);
}

//! \brief destructor.
Exact_polyhedron_geo::~Exact_polyhedron_geo() {}

//! \brief computes the convex hull of the coordinate set.
void Exact_polyhedron_geo::convex_hull()
{
  if (!m_coord_array) return;

  boost::shared_ptr<Exact_coord_array_3d> exact_coord_array =
    boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);
  if (exact_coord_array) {
    if (exact_coord_array->size() > 0)
      CGAL::convex_hull_3(exact_coord_array->begin(),
                          exact_coord_array->end(), m_polyhedron);
  }
  else {
    boost::shared_ptr<Coord_array_3d> coord_array =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coord_array) {
      if (coord_array->size() > 0) {
        std::vector<Point_3> points;
        points.resize(coord_array->size());
        std::transform(coord_array->begin(), coord_array->end(),
                     points.begin(), Vector_to_point());

#if 0
        std::copy(points.begin(), points.end(),
                  std::ostream_iterator<Point_3>(std::cout, "\n"));
#endif

        CGAL::convex_hull_3(points.begin(), points.end(), m_polyhedron);
      }
    }
    else SGAL_error();
  }
  m_dirty_polyhedron = false;
}

//! \brief draws the geometry.
void Exact_polyhedron_geo::draw(Draw_action* action)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_facets) clean_facets();
  if (is_empty()) return;

  draw_mesh(action);
}

//! \brief writes this container.
void Exact_polyhedron_geo::write(Formatter* formatter)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (is_dirty_flat_normal_indices()) clean_flat_normal_indices();
  if (is_dirty_flat_color_indices()) clean_flat_color_indices();
  if (is_dirty_flat_tex_coord_indices()) clean_flat_tex_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_facets) clean_facets();
  if (is_empty()) return;

  Stl_formatter* stl_formatter = dynamic_cast<Stl_formatter*>(formatter);
  if (stl_formatter) {
    const Matrix4f& matrix = stl_formatter->top_matrix();
    Facet_iterator fit = m_polyhedron.facets_begin();
    for (; fit != m_polyhedron.facets_end(); ++fit) {
      Polyhedron::Halfedge_around_facet_circulator j = fit->facet_begin();
      // Facets in polyhedral surfaces are at least triangles.
      CGAL_assertion(CGAL::circulator_size(j) >= 3);
      if (CGAL::circulator_size(j) >= 3) {
        const Vector3f& local_p1 = j++->vertex()->m_vertex;
        const Vector3f& local_p2 = j++->vertex()->m_vertex;
        const Vector3f& local_p3 = j++->vertex()->m_vertex;
        Vector3f p1, p2, p3;
        p1.xform_pt(local_p1, matrix);
        p2.xform_pt(local_p2, matrix);
        p3.xform_pt(local_p3, matrix);
        write_facet(p1, p2, p3, stl_formatter);
      }
    }
    return;
  }
}

//! \brief cleans the data structure.
void Exact_polyhedron_geo::clean_polyhedron()
{
  clock_t start_time = clock();
  if (m_convex_hull) convex_hull();
  else {
    m_polyhedron.delegate(m_surface);
#if 0
    if (!m_polyhedron.normalized_border_is_valid())
      m_polyhedron.normalize_border();
#else
    m_polyhedron.normalize_border();
#endif
    m_dirty_polyhedron = false;
  }
  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  m_dirty_facets = true;
}

//! \brief cleans the facets.
void Exact_polyhedron_geo::clean_facets()
{
  // Compute the plane equations:
  std::transform(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                 m_polyhedron.planes_begin(), Plane_equation());

#if 0
  /* Is there a collision?
   * \todo this doeasn't belong here
   */
  CGAL::Oriented_side side = oriented_side(CGAL::ORIGIN);
  std::cout << ((side == CGAL::ON_ORIENTED_BOUNDARY) ? "On boundary" :
                ((side == CGAL::ON_POSITIVE_SIDE) ? "Outside" : "Inside"))
            << std::endl;
#endif

  // Compute the normal used only for drawing the polyhedron
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                Plane_to_normal());

  // Convert the exact points to approximate used for drawing the polyhedron
  std::for_each(m_polyhedron.vertices_begin(), m_polyhedron.vertices_end(),
                Point_to_vector());

  m_dirty_facets = false;
}

//! \brief clears the internal representation.
void Exact_polyhedron_geo::clear()
{
  SGAL_TRACE_MSG(Trace::POLYHEDRON,
                 "Exact_polyhedron_geo::clear() start\n");
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
  m_dirty_facets = true;
}

//! \brief
void Exact_polyhedron_geo::cull(Cull_context& /* cull_context */) {}

//! \brief draws the internal representation.
void Exact_polyhedron_geo::draw_geometry(Draw_action* /* action */)
{
  SGAL_TRACE_MSG(Trace::POLYHEDRON, "Exact_polyhedron_geo::draw ... ");
  Facet_iterator i;
  for (i = m_polyhedron.facets_begin(); i != m_polyhedron.facets_end(); ++i) {
    Polyhedron::Halfedge_around_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    const Vector3f& normal = i->m_normal;
    glNormal3fv((float*)&normal);
    do {
      const Vector3f& vertex = j->vertex()->m_vertex;
      glVertex3fv((float*)&vertex);
    } while (++j != i->facet_begin());
    glEnd();
  }
  SGAL_TRACE_MSG(Trace::POLYHEDRON, "completed\n");
}

//! \brief
void Exact_polyhedron_geo::isect(Isect_action* /* action */)
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_facets) clean_facets();
  if (is_empty()) return;

  Facet_iterator i;
  for (i = m_polyhedron.facets_begin(); i != m_polyhedron.facets_end(); ++i) {
    Polyhedron::Halfedge_around_facet_circulator j = i->facet_begin();
    // Facets in polyhedral surfaces are at least triangles.
    CGAL_assertion(CGAL::circulator_size(j) >= 3);
    glBegin(GL_POLYGON);
    do {
      const Vector3f& vertex = j->vertex()->m_vertex;
      glVertex3fv((float*)&vertex);
    } while (++j != i->facet_begin());
    glEnd();
  }
}

//! \brief
Boolean Exact_polyhedron_geo::clean_sphere_bound()
{
  m_dirty_sphere_bound = false;
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_bb_is_pre_set) return true;

  Approximate_sphere_vector spheres;
  if (!m_polyhedron.empty()) {
    spheres.resize(m_polyhedron.size_of_vertices());
    Convert_approximate_sphere convert;
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
  return true;
}

//! \brief sets the attributes of this object.
void Exact_polyhedron_geo::set_attributes(Element* elem)
{
  Mesh_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "convexHull") {
      m_convex_hull = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the container prototype.
void Exact_polyhedron_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Mesh_set::get_prototype());
}

//! \brief deletes the container prototype.
void Exact_polyhedron_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Exact_polyhedron_geo::get_prototype()
{
  if (!s_prototype) Exact_polyhedron_geo::init_prototype();
  return s_prototype;
}

//! \brief computes the orientation of a point relative to the polyhedron.
CGAL::Oriented_side Exact_polyhedron_geo::oriented_side(const Point_3& p)
{
  Facet_iterator fi;
  for (fi = m_polyhedron.facets_begin(); fi != m_polyhedron.facets_end(); ++fi)
  {
    const Plane_3& plane = fi->plane();
    CGAL::Oriented_side side =  plane.oriented_side(p);
    if (side == CGAL::ON_NEGATIVE_SIDE) continue;
    return side;
  }
  return CGAL::ON_NEGATIVE_SIDE;
}

//! \brief prints statistics.
void Exact_polyhedron_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_facets) clean_facets();

  std::cout << "Primal"
            << ", no. vertices: " << m_polyhedron.size_of_vertices()
            << ",  no. edges: " << m_polyhedron.size_of_halfedges()/2
            << ",  no. facets: " << m_polyhedron.size_of_facets()
            << std::endl;

  std::cout << "Convex hull took " << m_time << " seconds to compute"
            << std::endl;
}

//! \brief obtains the polyhedron data-structure.
Exact_polyhedron_geo::Polyhedron& Exact_polyhedron_geo::get_polyhedron()
{
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_polyhedron;
}

//! \brief sets the polyhedron data-structure.
void Exact_polyhedron_geo::set_polyhedron(Polyhedron& polyhedron)
{
  m_dirty_polyhedron = false;
  m_polyhedron = polyhedron;
}

/*! \brief Sets the flag that indicates whether to compute the convex hull
 * of the coordinate set.
 */
void Exact_polyhedron_geo::set_convex_hull(Boolean flag)
{
  if (m_convex_hull == flag) return;
  m_convex_hull = flag;
  m_polyhedron.clear();
  m_dirty_polyhedron = true;
}

//! \brief processes change of field.
void Exact_polyhedron_geo::field_changed(Field_info* field_info)
{
  switch (field_info->get_id()) {
   case COORD_ARRAY:
    m_dirty_polyhedron = true;
    m_dirty_facets = true;
    break;
   default: break;
  }
  Mesh_set::field_changed(field_info);
}

SGAL_END_NAMESPACE
