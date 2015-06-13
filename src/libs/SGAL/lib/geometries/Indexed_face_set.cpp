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
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include <CGAL/basic.h>

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
#include "SGAL/Vrml_formatter.hpp"

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

const std::string Indexed_face_set::s_tag = "IndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype(nullptr);

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

//! \brief constructor.
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Boundary_set(proto),
  m_dirty_volume(true),
  m_dirty_surface_area(true),
  m_dirty_coord_array(true),
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

  // volume
  Float_handle_function volume_func = static_cast<Float_handle_function>
    (&Indexed_face_set::volume_handle);
  s_prototype->add_field_info(new SF_float(VOLUME, "volume",
                                           Field_info::RULE_OUT, volume_func));

  // surfaceArea
  Float_handle_function surface_area_func = static_cast<Float_handle_function>
    (&Indexed_face_set::surface_area_handle);
  s_prototype->add_field_info(new SF_float(SURFACE_AREA, "surfaceArea",
                                           Field_info::RULE_OUT,
                                           surface_area_func));
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

//! \brief draws the polygons.
void Indexed_face_set::draw(Draw_action* action)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_flat_coord_indices()))
    clean_coords();
  Boundary_set::draw(action);
}

//! \brief draws the polygons for selection.
void Indexed_face_set::isect(Isect_action* action)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_flat_coord_indices()))
    clean_coords();
  Boundary_set::isect(action);
}

//! \brief cleans the sphere bound.
void Indexed_face_set::clean_sphere_bound()
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_flat_coord_indices()))
    clean_coords();
  Boundary_set::clean_sphere_bound();
}

//! \brief cleans the coordinate array and coordinate indices.
void Indexed_face_set::clean_coords()
{
  m_dirty_coord_array = false;

  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_polyhedron.empty()) return;

  if (!m_coord_array) {
    Uint size = m_polyhedron.size_of_vertices();
    m_coord_array.reset(new Coord_array_3d(size));
    SGAL_assertion(m_coord_array);
  }
  else m_coord_array->resize(m_polyhedron.size_of_vertices());

  boost::shared_ptr<Coord_array_3d> coords =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);

  /* Generate the coordinate array and assign the index into the coordinate
   * array of the vertex to the vertex.
   * \todo either remove index from Exact_polyhedron_geo or introduce here.
   */
  // Uint index = 0;
  auto cit = coords->begin();
  for (auto vit = m_polyhedron.vertices_begin();
       vit != m_polyhedron.vertices_end(); ++vit)
  {
    // vit->m_index = index++;
    auto& p = vit->point();
    cit->set(p.x(), p.y(), p.z());
    ++cit;
  }

  clean_coord_indices();

  /* Notice that we call the function of the base calss.
   * In general when the coordinates change, we must invalidate the polyhedron
   * to force cleaning of the polyhedron, so that the change to the coordinates
   * is reflected in the polyhedron. However, clean_coords() could have beeen
   * invoked as a consequence to an update of the polyhedron. Naturally, in this
   * case we do not want to invalidate the polyhedron.
   */
  Boundary_set::coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief clears the coordinates.
void Indexed_face_set::clear_coord_array()
{
  m_dirty_coord_array = true;
  if (m_coord_array) m_coord_array->clear();
}

//! \brief obtains the coordinate array.
Indexed_face_set::Shared_coord_array Indexed_face_set::get_coord_array()
{
  if (is_dirty_coord_array()) clean_coords();
  return m_coord_array;
}

//! \brief cleans the coordinate indices.
void Indexed_face_set::clean_coord_indices()
{
  if (m_polyhedron.empty()) {
    m_dirty_coord_indices = false;
    m_dirty_flat_coord_indices = false;
    return;
  }

  set_num_primitives(m_polyhedron.size_of_facets());

  bool triangles(true);
  bool quads(true);
  Uint size = 0;
  for (auto fit = m_polyhedron.facets_begin();
       fit != m_polyhedron.facets_end(); ++fit)
  {
    Polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
    size_t circ_size = CGAL::circulator_size(hh);
    size += circ_size;
    if (circ_size != 3) triangles = false;
    if (circ_size != 4) quads = false;
  }
  SGAL_assertion(triangles && quads);

  Uint index = 0;
  if (triangles || quads) {
    m_flat_coord_indices.resize(size);
    set_primitive_type(quads ? PT_QUADS : PT_TRIANGLES);
    auto iit = m_flat_coord_indices.begin();
    for (auto fit = m_polyhedron.facets_begin();
         fit != m_polyhedron.facets_end(); ++fit)
    {
      Polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
      do {
        // *iit++ = hh->vertex()->m_index;
        hh->m_index = index++;
      } while (++hh != fit->facet_begin());
    }
    m_dirty_coord_indices = true;
    m_dirty_flat_coord_indices = false;
    m_normal_indices_flat = true;
  }
  else {
    size += m_polyhedron.size_of_facets();
    m_coord_indices.resize(size);
    set_primitive_type(PT_POLYGONS);
    auto iit = m_coord_indices.begin();
    for (auto fit = m_polyhedron.facets_begin();
         fit != m_polyhedron.facets_end(); ++fit)
    {
      Polyhedron::Halfedge_around_facet_circulator hh = fit->facet_begin();
      do {
        // *iit++ = hh->vertex()->m_index;
        hh->m_index = index++;
      } while (++hh != fit->facet_begin());
      *iit++ = (Uint) -1;
      ++index;
    }
    m_dirty_coord_indices = false;
    m_dirty_flat_coord_indices = true;
    m_normal_indices_flat = false;
  }
}

//! \brief cleans the normal array and the normal indices.
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
  m_dirty_normal_array = false;
  m_normal_array_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief cleans the polyhedron data structure.
void Indexed_face_set::clean_polyhedron()
{
  m_dirty_polyhedron = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;

  auto coords = get_coord_array();
  if (!coords || coords->empty()) return;
  if (is_dirty_flat_coord_indices()) clean_flat_coord_indices();

  //! \todo temporary solution
//   if (false) {
//     typedef Inexact_kernel::Point_3     Inexact_point_3;
//     boost::shared_ptr<Coord_array_3d> coord_array =
//       boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
//     std::vector<Inexact_point_3> points(coord_array->size());
//     std::transform(coord_array->begin(), coord_array->end(),
//                    points.begin(),
//                    [](const Vector3f& vec)
//                    { return Inexact_point_3(vec[0], vec[1], vec[2]); });
//     std::vector<std::vector<std::size_t> > polygons(get_num_primitives());
//     size_t i = 0;
//     for (auto it = polygons.begin(); it != polygons.end(); ++it) {
//       auto& polygon = *it;
//       polygon.resize(3);
//       polygon[0] = m_flat_coord_indices[i++];
//       polygon[1] = m_flat_coord_indices[i++];
//       polygon[2] = m_flat_coord_indices[i++];
//     }

//     bool oriented = CGAL::orient_polygon_soup(points, polygons);
//     std::cout << (oriented ? "Oriented." : "Not orientabled.") << std::endl;
//     std::cout << points.size() << ", " << coord_array->size() << std::endl;
//     std::cout << polygons.size() << ", " << get_num_primitives() << std::endl;

//     polygons.clear();
//     i = 0;
//     for (auto it = polygons.begin(); it != polygons.end(); ++it) {
//       auto& polygon = *it;
//       m_flat_coord_indices[i++] = polygon[0];
//       m_flat_coord_indices[i++] = polygon[1];
//       m_flat_coord_indices[i++] = polygon[2];
//     }
//     for (auto it = polygons.begin(); it != polygons.end(); ++it) it->clear();
//     points.clear();
//   }
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
}

//! \brief clears the polyhedron.
void Indexed_face_set::clear_polyhedron()
{
  m_dirty_polyhedron = true;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  m_polyhedron.clear();
}

//! \brief sets the polyhedron data-structure.
void Indexed_face_set::set_polyhedron(Polyhedron& polyhedron)
{
  m_polyhedron = polyhedron;
  m_dirty_polyhedron = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  clear_coord_array();
  clear_coord_indices();
  clear_flat_coord_indices();
}

//! \brief obtains the polyhedron data-structure.
const Indexed_face_set::Polyhedron&
Indexed_face_set::get_polyhedron(Boolean /* with_planes */)
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_polyhedron;
}

//! \brief responds to a change in the coordinate array.
void Indexed_face_set::coord_content_changed(const Field_info* field_info)
{
  m_dirty_coord_array = false;
  clear_polyhedron();
  Boundary_set::coord_content_changed(field_info);
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

//! Write this container.
void Indexed_face_set::write(Formatter* formatter)
{
  SGAL_assertion(is_dirty_polyhedron() && is_dirty_coord_array());
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_flat_coord_indices()))
    clean_coords();
  Boundary_set::write(formatter);
}

//! \brief writes a field of this container.
void Indexed_face_set::write_field(const Field_info* field_info,
                                   Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (vrml_formatter) {
    if (COLOR_PER_VERTEX == field_info->get_id()) {
      const auto & name = field_info->get_name();
      vrml_formatter->single_boolean(name, get_color_per_vertex(), true);
      return;
    }

    if (NORMAL_PER_VERTEX == field_info->get_id()) {
      const auto & name = field_info->get_name();
      vrml_formatter->single_boolean(name, get_normal_per_vertex(), true);
      return;
    }
  }
  Boundary_set::write_field(field_info, formatter);
}

//! \brief cleans (compute) the volume.
void Indexed_face_set::clean_volume()
{
  m_dirty_volume = false;

  m_volume = 0;
  if (is_polyhedron_empty()) return;

  Inexact_point_3 origin(CGAL::ORIGIN);
  //! \todo Fix CGAL::volume() to accept CGAL::ORIGIN as an argument.
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                [&](Polyhedron::Facet& facet)
                {
                  SGAL_assertion(3 == CGAL::circulator_size(fit->facet_begin()));
                  auto h = facet.halfedge();
                  m_volume += CGAL::volume(origin,
                                           h->vertex()->point(),
                                           h->next()->vertex()->point(),
                                           h->next()->next()->vertex()->point());
                });
}

//! \brief cleans (compute) the surface area.
void Indexed_face_set::clean_surface_area()
{
  m_dirty_surface_area = false;

  m_surface_area = 0;
  if (is_polyhedron_empty()) return;

  m_surface_area = 0;
  std::for_each(m_polyhedron.facets_begin(), m_polyhedron.facets_end(),
                [&](Polyhedron::Facet& facet)
                {
                  SGAL_assertion(3 == CGAL::circulator_size(fit->facet_begin()));
                  auto h = facet.halfedge();
                  const auto& p1 = h->vertex()->point();
                  const auto& p2 = h->next()->vertex()->point();
                  const auto& p3 = h->next()->next()->vertex()->point();
                  // m_surface_area += CGAL::area(p1, p2, p3);
                  Kernel::Triangle_3 tri(p1, p2, p3);
                  m_surface_area += sqrtf(tri.squared_area());
                });
}

//! \brief computes the volume of the polyhedron.
Float Indexed_face_set::volume()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_volume) clean_volume();
  return m_volume;
}

//! \brief computes the surface area of the polyhedron.
Float Indexed_face_set::surface_area()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_surface_area) clean_surface_area();
  return m_surface_area;
}

SGAL_END_NAMESPACE
