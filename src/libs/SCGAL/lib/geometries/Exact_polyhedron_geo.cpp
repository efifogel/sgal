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
// $Source$
// $Revision: 13487 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4503 )
#pragma warning ( disable : 4800 )
#endif

#include <CGAL/Cartesian.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "SCGAL/Exact_polyhedron_geo.hpp"
#include "SCGAL/Exact_coord_array.hpp"

#include "SGAL/Coord_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

std::string Exact_polyhedron_geo::s_tag = "ExactPolyhedron";
SGAL::Container_proto* Exact_polyhedron_geo::s_prototype = 0;

REGISTER_TO_FACTORY(Exact_polyhedron_geo, "Exact_polyhedron_geo");

/*! Constructor */
Exact_polyhedron_geo::Exact_polyhedron_geo(Boolean proto) :
  Mesh_set(proto),
  m_convex_hull(false),
  m_time(0)
{}

/*! Destructor */
Exact_polyhedron_geo::~Exact_polyhedron_geo() {}

/*! Compute the convex hull of the coordinate set */
void Exact_polyhedron_geo::convex_hull()
{
  std::cout << "Exact_polyhedron_geo::convex_hull" << std::endl;
  
  if (!m_coord_array) return;

  Exact_coord_array* exact_coord_array =
    dynamic_cast<Exact_coord_array *>(m_coord_array);
  if (exact_coord_array && (exact_coord_array->size() > 0)) {
    // std::cout << "Exact_polyhedron_geo::exact" << std::endl;
    CGAL::convex_hull_3(exact_coord_array->begin(),
                        exact_coord_array->end(), m_polyhedron);    
  } else {
    // std::cout << "Exact_polyhedron_geo::approximate" << std::endl;
    std::vector<Point_3> points;
    points.resize(m_coord_array->size());
    std::transform(m_coord_array->begin(), m_coord_array->end(),
                   points.begin(), Vector_to_point());

#if 0
    std::copy(points.begin(), points.end(),
              std::ostream_iterator<Point_3>(std::cout, "\n"));
#endif

    CGAL::convex_hull_3(points.begin(), points.end(), m_polyhedron);
  }
}

/*! Clean the data structure */
void Exact_polyhedron_geo::clean()
{
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON,
                 "Exact_polyhedron_geo::clean() start\n");
  clock_t start_time = clock();
  if (m_convex_hull) convex_hull();
  else {
    m_surface.set_geo_set(this);
    
    m_polyhedron.delegate(m_surface);
#if 0
    if (!m_polyhedron.normalized_border_is_valid())
      m_polyhedron.normalize_border();
#else
    m_polyhedron.normalize_border();
#endif
  }

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

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;

  Mesh_set::clean();
}

/*! Clear the internal representation */
void Exact_polyhedron_geo::clear()
{
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON,
                 "Exact_polyhedron_geo::clear() start\n");
  m_polyhedron.clear();
  Mesh_set::clear();
}

/*!
 */
void Exact_polyhedron_geo::cull(SGAL::Cull_context& cull_context) {}

/*! Draw the internal representation */
void Exact_polyhedron_geo::draw_geometry(SGAL::Draw_action * /* action */)
{
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON, "Exact_polyhedron_geo::draw ... ");
  for (Facet_iterator i = m_polyhedron.facets_begin();
       i != m_polyhedron.facets_end(); ++i)
  {
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
  SGAL_TRACE_MSG(SGAL::Trace::POLYHEDRON, "completed\n");
}

/*!
 */
void Exact_polyhedron_geo::isect(SGAL::Isect_action * action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;

  for (Facet_iterator i = m_polyhedron.facets_begin();
       i != m_polyhedron.facets_end(); ++i)
  {
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

/*!
 */
Boolean Exact_polyhedron_geo::clean_sphere_bound()
{
  if (is_dirty()) clean();
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
  m_dirty_sphere_bound = false;
  return true;
}

/*! Set the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Exact_polyhedron_geo::set_attributes(SGAL::Element * elem)
{
  SGAL::Mesh_set::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "convexHull") {
      m_convex_hull = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! Initialize the container prototype */
void Exact_polyhedron_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new SGAL::Container_proto(Mesh_set::get_prototype());

  //! Container execution function
  // typedef void (SGAL::Container::* Execution_function)(SGAL::Field_info*);
  // Execution_function exec_func;
}

/*! Delete the container prototype */
void Exact_polyhedron_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the container prototype */
SGAL::Container_proto* Exact_polyhedron_geo::get_prototype() 
{  
  if (!s_prototype) Exact_polyhedron_geo::init_prototype();
  return s_prototype;
}

/*! Compute the orientation of a point relative to the polyhedron */
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

/*! Print statistics */
void Exact_polyhedron_geo::print_stat()
{
  std::cout << "Information for " << get_name() << ":\n";
  if (is_dirty()) clean();

  std::cout << "Primal"
            << ", no. vertices: " << m_polyhedron.size_of_vertices()
            << ",  no. edges: " << m_polyhedron.size_of_halfedges()/2
            << ",  no. facets: " << m_polyhedron.size_of_facets()
            << std::endl;

  std::cout << "Convex hull took " << m_time << " seconds to compute"
            << std::endl;
}

/*! \brief obtains the polyhedron data-structure. */
Exact_polyhedron_geo::Polyhedron& Exact_polyhedron_geo::get_polyhedron()
{
  if (is_dirty()) clean();
  return m_polyhedron;
}

SGAL_END_NAMESPACE
