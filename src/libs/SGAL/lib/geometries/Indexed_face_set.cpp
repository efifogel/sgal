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
#include <time.h>
#include <vector>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

#include <CGAL/basic.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/enum.h>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_polyhedron.hpp"
#include "SGAL/Epic_polyhedron.hpp"
#include "SGAL/Epec_polyhedron.hpp"
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
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/calculate_multiple_normals_per_vertex.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Modeling.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Hole_filler_visitor.hpp"
#include "SGAL/Clean_facet_indices_from_polyhedron_visitor.hpp"
#include "SGAL/Orient_polygon_soup_visitor.hpp"
#include "SGAL/Delegate_surface_visitor.hpp"
#include "SGAL/Connected_components_counter_visitor.hpp"
#include "SGAL/Reverse_facet_indices_visitor.hpp"
#include "SGAL/Clean_vertices_visitor.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Convex_hull_visitor.hpp"
#include "SGAL/Compute_coords_visitor.hpp"
#include "SGAL/Polyhedron_vertices_counter_visitor.hpp"
#include "SGAL/Polyhedron_halfedges_counter_visitor.hpp"
#include "SGAL/Polyhedron_facets_counter_visitor.hpp"
#include "SGAL/Faces_counter_visitor.hpp"
#include "SGAL/Border_edges_counter_visitor.hpp"
#include "SGAL/Empty_polyhedron_visitor.hpp"
#include "SGAL/Clear_polyhedron_visitor.hpp"
#include "SGAL/Border_normalizer_visitor.hpp"
#include "SGAL/Volume_calculator_visitor.hpp"
#include "SGAL/Surface_area_calculator_visitor.hpp"
// #include "SGAL/Collinear_facets_remesher_visitor.hpp"

SGAL_BEGIN_NAMESPACE

/*!
 * -----------------------------------------------------------------------------
 * Handling Fields
 * -----------------------------------------------------------------------------
 * We use a deferred response mechanism to handle the change of fields.
 * For every field f we define at least the following functions:
 * 1. set_f(value) -          sets a new value for f.
 * 2. value get_f() -         obtains the value of f.
 * 3. clean_f() -             cleans (or validates) the value of f.
 * 4. clear_f() -             clears (or invalidates) the value of f.
 * 5. f_changed(field-info) - responds to a change of the value of f.
 * 6. bool is_dirty_f() -     determines whether the field has been cleared.
 *
 * set_f() accepts a new value of f and stores it. Then, it calls f_changed().
 *
 * f_changed() propagates the change to other fields. It also reflects the
 *             change at derived classes if exist and perhaps even at the
 *             parent Shape node (which, in turn, reflects it further up in
 *             parent via polymorphism. (Notice that this function is virtual.)
 *
 * clear_f() clears the data structure storing f, and sets the corresponding
 *           m_dirty_f flag to indicate that the f is no longer valid. Then,
 *           it calls f_changed() of the base class if exists. It is imperative
 *           to call the base class function, so that the call to f_changed()
 *           is not captured by the derived classes if exist.
 *
 * f_clean() validates the value of f. Then, it calls f_changed() of the base
 *           class if exists; see clear_f() above.
 *
 * get_f() calls clean_f() if the m_dirty_f flag is set. Only then, it returns
 *         the current value of f.
 *
 * is_dirty_f() simply returns the value of the m_dirty_f flag.
 *
 * The above, essencially implies that the call to clear_f() clears all
 * dependent data structures of base classes and the call to f_changed() clears
 * all dependent data structures of derived classes.
 *
 * -----------------------------------------------------------------------------
 * Repairing
 * -----------------------------------------------------------------------------
 * It is assumed that the geometry represented by this data structure does
 * not need repairing at all, that is, it is consistent, has no singular
 * vertices, and does not need repairing (triangulation of holes and repairing
 * of orientation) at all, with the following exceptions:
 * a. If the global configuration indicates that hole-triangulation or
 *    orientation repairing is desired, then repairing is enabled.
 * b. lobal configuration indicates that consistency repairing is desired,
 *    then consistency enforcing is enabled.
 * In order to alter the defaults, e.g., in the case of polygon soup read from
 * an stl file, call the following corespnding member functions:
 *   set_consistent(false) - enables consistency enforcing.
 *   set_repaired(flase) - enables hole-triangulation and orientation-repairing.
 * \todo if (!closed) set_solid(false);
 */

const std::string Indexed_face_set::s_tag = "IndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype(nullptr);

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

//! \brief constructs from the prototype.
// By default, we assume that the mesh is inconsistent.
// If the mesh is consistent, there is no need to re-orient the polygon soup
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Boundary_set(proto),
  m_consistent(true),
  m_has_singular_vertices(false),
  m_repaired(true),
  m_normals_repaired(true),
  m_dirty_volume(true),
  m_dirty_surface_area(true),
  m_convex_hull(false),
  m_volume(0),
  m_surface_area(0),
  m_polyhedron_type(POLYHEDRON_INEXACT),
  m_dirty_coord_array(true),
  m_dirty_polyhedron(true),
  m_dirty_polyhedron_facet_normals(true),
  m_dirty_normal_attributes(true),
  m_triangulate_holes(Modeling::s_def_triangulate_holes),
  m_refine(Modeling::s_def_refine),
  m_fair(Modeling::s_def_refine),
  m_repair_orientation(Modeling::s_def_repair_orientation),
  m_repair_normals(Modeling::s_def_repair_normals),
  m_make_consistent(Modeling::s_def_make_consistent)
{
  if (proto) return;
  set_crease_angle(0);
  set_normal_per_vertex(true);
  set_color_per_vertex(true);
}

//! \brief destructs.
Indexed_face_set::~Indexed_face_set(){}

//! \brief sets the attributes of the object.
void Indexed_face_set::set_attributes(Element* elem)
{
  Boundary_set::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "convexHull") {
      set_convex_hull(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the container prototype.
void Indexed_face_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Boundary_set::get_prototype());

  // volume
  auto volume_func = static_cast<Float_handle_function>
    (&Indexed_face_set::volume_handle);
  s_prototype->add_field_info(new SF_float(VOLUME, "volume",
                                           Field_rule::RULE_OUT, volume_func));

  // surfaceArea
  auto surface_area_func = static_cast<Float_handle_function>
    (&Indexed_face_set::surface_area_handle);
  s_prototype->add_field_info(new SF_float(SURFACE_AREA, "surfaceArea",
                                           Field_rule::RULE_OUT,
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
  if (s_prototype == nullptr) Indexed_face_set::init_prototype();
  return s_prototype;
}

//! \brief draws the polygons.
void Indexed_face_set::draw(Draw_action* action)
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  else if (is_convex_hull() && is_dirty_polyhedron()) {
    clean_polyhedron();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  if (!m_normals_repaired && m_repair_normals) repair_normals();
  Boundary_set::draw(action);
}

//! \brief draws the polygons for selection.
void Indexed_face_set::isect(Isect_action* action)
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  else if (is_convex_hull() && is_dirty_polyhedron()) {
    clean_polyhedron();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  Boundary_set::isect(action);
}

//! \brief cleans the sphere bound.
void Indexed_face_set::clean_bounding_sphere()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clean_bounding_sphere(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }

  if (!m_coord_array) return;

  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (coords) {
    m_bounding_sphere.set_around(coords->begin(), coords->end());
    return;
  }

  auto epic_coords =
    boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coord_array);
  if (epic_coords) {
    const auto& vecs = epic_coords->get_inexact_coords();
    m_bounding_sphere.set_around(vecs.begin(), vecs.end());
    return;
  }

  auto epec_coords =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  if (epec_coords) {
    const auto& vecs = epec_coords->get_inexact_coords();
    m_bounding_sphere.set_around(vecs.begin(), vecs.end());
    return;
  }

  SGAL_error();
}

//! \brief cleans the coordinate array and coordinate indices.
void Indexed_face_set::clean_coords()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout
                  << "Indexed_face_set::clean_coords(): "
                  << "name: " << get_name() << std::endl;);

  m_dirty_coord_array = false;

  // If the polyhedron is empty, return.
  if (boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron)) return;

  Polyhedron_vertices_counter_visitor vertices_visitor;
  auto num_vertices = boost::apply_visitor(vertices_visitor, m_polyhedron);

  if (!m_coord_array) {
    m_coord_array.reset(new Coord_array_3d(num_vertices));
    SGAL_assertion(m_coord_array);
  }
  else m_coord_array->resize(num_vertices);

  /* Generate the coordinate array and assign the index into the coordinate
   * array of the vertex to the vertex.
   */
  Compute_coords_visitor coords_visitor(m_coord_array);
  boost::apply_visitor(coords_visitor, m_polyhedron);

  clean_facet_coord_indices_from_polyhedron();

  /* Notice that we call the function of the base calss.
   * In general when the coordinates change, we must invalidate the polyhedron
   * to force cleaning of the polyhedron, so that the change to the coordinates
   * is reflected in the polyhedron. However, clean_coords() could have beeen
   * invoked as a consequence of an update of the polyhedron. Naturally, in this
   * case we do not want to invalidate the polyhedron.
   */
  Boundary_set::coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief clears the coordinates.
void Indexed_face_set::clear_coord_array()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clear_coord_array(): "
                  << "name: " << get_name() << std::endl;);

  m_dirty_coord_array = true;

  if (m_coord_array) m_coord_array->clear();
  Boundary_set::coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief obtains the coordinate array.
Indexed_face_set::Shared_coord_array Indexed_face_set::get_coord_array()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::get_coord_array(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();

  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
    repair();
    if (is_dirty_coord_array()) clean_coords();
  }

  return m_coord_array;
}

//! \brief cleans the coordinate indices.
void Indexed_face_set::clean_facet_coord_indices_from_polyhedron()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clean_facet_coord_indices_from_polyhedron(): "
                  << "name: " << get_name() << std::endl;);

  // If the polyhedron is empty, return.
  if (boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron)) {
    m_dirty_coord_indices = false;
    m_dirty_facet_coord_indices = false;
    return;
  }

  auto num_primitives =
    boost::apply_visitor(Polyhedron_facets_counter_visitor(), m_polyhedron);
  set_num_primitives(num_primitives);

  auto primitive_type =
    boost::apply_visitor(Type_polyhedron_visitor(), m_polyhedron);
  set_primitive_type(primitive_type);

  init_facet_coord_indices();
  Clean_facet_indices_from_polyhedron_visitor visitor(num_primitives);
  boost::apply_visitor(visitor, m_polyhedron, m_facet_coord_indices);

  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;

  /* Notice that we call the function of the base calss.
   * In general when the coordinates change, we must invalidate the polyhedron
   * to force cleaning of the polyhedron, so that the change to the coordinates
   * is reflected in the polyhedron. However, clean_coords() could have beeen
   * invoked as a consequence of an update of the polyhedron. Naturally, in this
   * case we do not want to invalidate the polyhedron.
   */
  Boundary_set::coord_content_changed(get_field_info(COORD_INDEX_ARRAY));
}

//! \brief cleans (validate) the facet coordinate indices.
void Indexed_face_set::clean_facet_coord_indices()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clean_facet_coord_indices(): "
                  << "name: " << get_name() << std::endl;);

  if (! is_dirty_polyhedron()) clean_facet_coord_indices_from_polyhedron();
  else Boundary_set::clean_facet_coord_indices();
}

//! \brief cleans the normal array and the normal indices.
void Indexed_face_set::clean_normals()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clean_normals(): "
                  << "name: " << get_name() << std::endl;);

  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_polyhedron) clean_polyhedron();
    if (m_dirty_polyhedron_facet_normals) clean_polyhedron_facet_normals();
    if (m_dirty_normal_attributes) clean_normal_attributes();

    if (m_smooth) calculate_single_normal_per_vertex();
    else if (m_creased) calculate_normal_per_facet();
    else calculate_multiple_normals_per_vertex();
  }
  else if (m_crease_angle >= SGAL_PI) calculate_single_normal_per_vertex();
  else if (m_crease_angle == 0) calculate_normal_per_facet();
  else SGAL_error();
  m_dirty_normal_array = false;
  m_normal_array_cleaned = true;
  m_dirty_normal_buffer = true;
}

//! \brief repairs the data structures
void Indexed_face_set::repair()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::repair(): "
                  << get_name() << std::endl;);

  if (m_dirty_polyhedron) clean_polyhedron();

  Connected_components_counter_visitor ccs_counter_visitor;
  auto num_ccs = boost::apply_visitor(ccs_counter_visitor, m_polyhedron);
  Is_closed_polyhedron_visitor is_closed_visitor;
  auto closed = boost::apply_visitor(is_closed_visitor, m_polyhedron);
  if ((num_ccs == 1) && closed && m_repair_orientation) {
    clean_volume();
    // If the volume of the closed polyhedron is negative, all facets are
    // oriented clockwise. That is, the normal of every facet is directed
    // towards the interior of the polyhedron instead of the exterior.
    if (m_volume < 0) {
      // If the facet coord indices is dirty (invalid), clean (recompute) it.
      //! \todo Do the cleaning and reversing at once as an optimization.
      if (is_dirty_coord_array() || is_dirty_facet_coord_indices())
        clean_coords();

      // Do not call reverse_facet_coord_indices() of the base class.
      // Instead, reverse and set the appropriate flags.
      // Do not alter the following flags:
      // m_consistent, m_has_singular_vertices, and m_dirty_repaired_polyhedron
      Reverse_facet_indices_visitor visitor;
      boost::apply_visitor(visitor, m_facet_coord_indices);

      m_dirty_facet_coord_indices = false;
      Boundary_set::facet_coord_indices_changed();
      m_dirty_coord_indices = true;
      boost::apply_visitor(Clear_polyhedron_visitor(), m_polyhedron);
      m_dirty_polyhedron = true;

      //! \todo Instead of brutally clearing the normals, colors, and texture
      // coordinat, reorder them appropriately.
      m_dirty_polyhedron_facet_normals = true;
      m_dirty_normal_attributes = true;
      clear_normal_indices();
      clear_facet_normal_indices();
      set_normal_array(Shared_normal_array());

      // Clean the normals
      clear_color_indices();
      clear_facet_color_indices();
      set_color_array(Shared_color_array());

      // Clean the texture coordinate
      clear_tex_coord_indices();
      clear_facet_tex_coord_indices();
      set_tex_coord_array(Shared_tex_coord_array());

      m_dirty_volume = true;
      m_dirty_surface_area = true;
      m_repaired = true;
    }
  }

  m_repaired = true;
}

//! \brief repairs the data structures
void Indexed_face_set::repair_normals()
{
  clear_normal_indices();
  clear_facet_normal_indices();
  set_normal_array(Shared_normal_array());
  set_normal_per_vertex(false);
  m_normals_repaired = true;
}

//! \brief initializes the polyhedron.
void Indexed_face_set::init_polyhedron()
{
  switch (m_polyhedron_type) {
   case POLYHEDRON_INEXACT: m_polyhedron = Inexact_polyhedron(); break;
   case POLYHEDRON_EPIC: m_polyhedron = Epic_polyhedron(); break;
   case POLYHEDRON_EPEC: m_polyhedron = Epec_polyhedron(); break;
   default: SGAL_error();
  }
}

//! \brief responds to a change in the polyhedron.
void Indexed_face_set::polyhedron_changed()
{
  m_dirty_polyhedron = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;

  // Assume that the incoming polyhedron
  //   (i) the facets normal of which are cleaned,
  //  (ii) has no singular vertices,
  // (iii) is consistent, and
  // ((iv) repaired.
  m_dirty_polyhedron_facet_normals = false;

  m_consistent = true;
  m_has_singular_vertices = false;
  m_repaired = true;

  clear_coord_array();
  clear_coord_indices();
  clear_facet_coord_indices();
}

//! \brief computes the convex hull of the coordinate set.
void Indexed_face_set::convex_hull()
{
  if (!m_coord_array || m_coord_array->empty()) return;

  if (m_polyhedron_type != POLYHEDRON_EPEC) m_polyhedron_type = POLYHEDRON_EPEC;
  init_polyhedron();

  auto exact_coords =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  if (exact_coords) {
    if (exact_coords->size() > 0) {
      Convex_hull_visitor<Epec_coord_array_3d::Exact_point_const_iter>
        ch_visitor(exact_coords->begin(), exact_coords->end());
      boost::apply_visitor(ch_visitor, m_polyhedron);

      /* Compute the index of the vertex into the coordinate array and assign it
       * to the polyhedron-vertex record.
       * \todo make more efficient.
       */
      Clean_vertices_visitor<Epec_coord_array_3d::Exact_point_const_iter>
        cv_visitor(exact_coords->begin(), exact_coords->end());
      boost::apply_visitor(cv_visitor, m_polyhedron);
    }
  }
  else {
    auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    if (coords) {
      if (coords->size() > 0) {
        std::vector<Epec_point_3> points;
        points.resize(coords->size());
        std::transform(coords->begin(), coords->end(),
                       points.begin(), Vector_to_point());

        // std::copy(points.begin(), points.end(),
        //           std::ostream_iterator<Exact_point_3>(std::cout, "\n"));

        Convex_hull_visitor<std::vector<Epec_point_3>::const_iterator>
          ch_visitor(points.begin(), points.end());
        boost::apply_visitor(ch_visitor, m_polyhedron);

        /* Compute the index of the vertex into the coordinate array and assign
         * it to the polyhedron-vertex record.
         * \todo make more efficient.
         */
        Clean_vertices_visitor<std::vector<Epec_point_3>::const_iterator>
          cv_visitor(points.begin(), points.end());
        boost::apply_visitor(cv_visitor, m_polyhedron);
      }
    }
    else SGAL_error();
  }

  m_dirty_polyhedron = false;
  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = true;
}

//! \brief cleans the polyhedron data structure.
// Set (or reset) the dirty flags at the end.
// In fact, this should be applied for all cleaning functions.
// In some cases it doesn't matter, but here, we bail out if the coordinates
// or the coordinate indices are empty and the m_dirty_polyhedron must retain
// its (true) value; this is imperative as explained next.
// When we need the clean (valid) polyhedron (e.g., when we need to determine
// whether the polyhedron is closed) we first attempt to clean the
// coordinates & coordinate indices and immediately after we attempt to
// clean the polyhedron. If, on the other hand, we need the clean (valid)
// coordinates & coordinate indices, we attemp the cleaning the other way
// around; that is, we first attempt to clean the polyhedron, and immediately
// after we attempt to clean the coordinates & coordinate indices. In this
// case, when the polyhedron cleaning function is invoked, and indices are
// not cleaned, we must enable the re-invocation of the polyhedron cleaning
// function at a later point after the indices have been cleaned.
void Indexed_face_set::clean_polyhedron()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clean_polyhedron(): "
                  << "name: " << get_name() << std::endl;);

  // If there are no coordinates bail out.
  if (!m_coord_array || m_coord_array->empty()) return;

  clock_t start_time = clock();
  if (m_convex_hull) convex_hull();
  else {
    // If there are no coordinate indices bail out.
    if (empty_facet_indices(m_facet_coord_indices)) return;

    // We do not remove collinear facets now, cause it may create T verticees.

    // Make consistent.
    if (!m_consistent) {
      Orient_polygon_soup_visitor visitor(m_coord_array);
      m_has_singular_vertices =
        boost::apply_visitor(visitor, m_facet_coord_indices);
      clear_coord_indices();

      m_num_primitives =
        boost::apply_visitor(Faces_counter_visitor(), m_facet_coord_indices);
    }

    // Create the polyhedral surface
    if ((m_polyhedron_type == POLYHEDRON_INEXACT) && m_triangulate_holes)
      m_polyhedron_type = POLYHEDRON_EPIC;

    init_polyhedron();
    Delegate_surface_visitor visitor(m_primitive_type, m_num_primitives,
                                     m_coord_array, m_facet_coord_indices);
    m_consistent = boost::apply_visitor(visitor, m_polyhedron);

    // Normalize the border of the polyhedron.
    boost::apply_visitor(Border_normalizer_visitor(), m_polyhedron);

    auto closed =
      boost::apply_visitor(Is_closed_polyhedron_visitor(), m_polyhedron);

    // Border_edges_counter_visitor bec_visitor;
    // auto num_border_edges = boost::apply_visitor(bec_visitor, m_polyhedron);
    // std::cout << "# border edge: " << num_border_edges << std::endl;

    if (!closed && m_triangulate_holes) {
      Hole_filler_visitor visitor(m_refine, m_fair, m_coord_array);
      boost::apply_visitor(visitor, m_polyhedron, m_facet_coord_indices);
      auto new_coord_array = visitor.get_new_coord_array();
      if (new_coord_array) std::swap(m_coord_array, m_coord_array);

      // num_border_edges = boost::apply_visitor(bec_visitor, m_polyhedron);
      // std::cout << "# border edge: " << num_border_edges << std::endl;

      //! \todo instead of brutally clearing the indices arrays,
      // update these arrays based on the results of the hole-filler visitor.
      clear_coord_array();
      clear_coord_indices();
      clear_facet_coord_indices();
    }

    // Remesh collinear triangles.
    //! \todo Fix the remesher.
    // Collinear_facets_remesher_visitor remesher_visitor(m_primitive_type);
    // auto num_facets = boost::apply_visitor(remesher_visitor, m_polyhedron);
    // std::cout << "primitive type: " << m_primitive_type << std::endl;
    // std::cout << "# primitive: " << m_num_primitives << std::endl;
    // std::cout << "# facets: " << num_facets << std::endl;
    // if (num_facets != m_num_primitives) {
    //   m_num_primitives = num_facets;
    //   clear_coord_array();
    //   clear_coord_indices();
    //   clear_facet_coord_indices();
    // }
  }
  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;

  m_dirty_polyhedron = false;
  m_dirty_polyhedron_facet_normals = true;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
}

//! \brief clears the polyhedron.
void Indexed_face_set::clear_polyhedron()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::clear_polyhedron(): "
                  << "name: " << get_name() << std::endl;);

  m_dirty_polyhedron = true;
  m_dirty_polyhedron_facet_normals = true;
  m_dirty_volume = true;
  m_dirty_surface_area = true;

  // It is assumed by default that the polyhedron does not nead repairing at
  // all.
  m_consistent = true;
  m_has_singular_vertices = false;
  m_repaired = true;
  if (m_make_consistent) m_consistent = false;
  if (m_triangulate_holes || m_repair_orientation) m_repaired = false;

  boost::apply_visitor(Clear_polyhedron_visitor(), m_polyhedron);
}

//! \brief sets the polyhedron data-structure.
void Indexed_face_set::set_polyhedron(Polyhedron& polyhedron)
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::set_polyhedron(): "
                  << "name: " << get_name() << std::endl;);

  m_polyhedron = polyhedron;
  polyhedron_changed();
}

//! \brief obtains the representation mode.
Geo_set::Primitive_type Indexed_face_set::get_primitive_type()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::get_primitive_type(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  else if (is_convex_hull() && is_dirty_polyhedron()) {
    clean_polyhedron();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  return Boundary_set::get_primitive_type();
}

//! \brief obtains the number of primitives.
Size Indexed_face_set::get_num_primitives()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::get_num_primitives(): "
                  << "name: " << get_name() << std::endl;);
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  else if (is_convex_hull() && is_dirty_polyhedron()) {
    clean_polyhedron();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  return Boundary_set::get_num_primitives();
}

//! \brief obtains the polyhedron data-structure.
const Indexed_face_set::Polyhedron&
Indexed_face_set::get_polyhedron(Boolean clean_facet_normals)
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::get_polyhedron(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (clean_facet_normals) {
    if (m_dirty_polyhedron_facet_normals) clean_polyhedron_facet_normals();
  }
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

//! \brief obtains the ith 3D coordinate.
const Vector3f& Indexed_face_set::get_coord_3d(Uint i) const
{
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (coords) return (*coords)[i];

  auto epic_coords =
    boost::dynamic_pointer_cast<Epic_coord_array_3d>(m_coord_array);
  if (epic_coords) return epic_coords->get_inexact_coord(i);

  auto epec_coords =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  SGAL_assertion(epec_coords);
  return epec_coords->get_inexact_coord(i);
}

//! \brief calculates multiple normals per vertex for all vertices.
void Indexed_face_set::calculate_multiple_normals_per_vertex()
{
  resize_facet_indices(m_facet_normal_indices, m_facet_coord_indices);
  if (!m_normal_array) {
    m_normal_array.reset(new Normal_array());
    SGAL_assertion(m_normal_array);
  }
  else m_normal_array->clear();
  Calculate_multiple_normals_per_vertex_visitor visitor(m_normal_array,
                                                        m_facet_normal_indices);
  boost::apply_visitor(visitor, m_polyhedron);
  m_dirty_normal_indices = true;
  m_dirty_facet_normal_indices = false;
}

//! \brief prints statistics.
void Indexed_face_set::print_stat()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (is_dirty_polyhedron()) clean_polyhedron();

  std::cout << "Container name: " << get_name() << std::endl;
  std::cout << "Container tag: " << get_tag() << std::endl;
  std::cout << "Is closed: " << is_closed() << std::endl;
  // std::cout << "# vertices: " << m_polyhedron.size_of_vertices()
  //           << ", # edges: " << m_polyhedron.size_of_halfedges() / 2
  //           << ", # facets: " << m_polyhedron.size_of_facets()
  //           << std::endl;
  std::cout << "Volume of convex hull: " << volume_of_convex_hull()
            << std::endl;

  if (m_convex_hull)
    std::cout << "Convex hull took " << m_time << " seconds to compute"
              << std::endl;
}

//! Write this container.
void Indexed_face_set::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Indexed_face_set: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);

  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set:write(): "
                  << ", name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) {
    repair();
    if (is_dirty_coord_array()) clean_coords();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  else if (is_convex_hull() && is_dirty_polyhedron()) {
    clean_polyhedron();
    if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  }
  if (!m_normals_repaired && m_repair_normals) repair_normals();
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
  if (boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron)) return;
  m_volume = boost::apply_visitor(Volume_calculator_visitor(), m_polyhedron);
}

//! \brief cleans (compute) the surface area.
void Indexed_face_set::clean_surface_area()
{
  m_dirty_surface_area = false;

  m_surface_area = 0;
  if (boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron)) return;
  Surface_area_calculator_visitor visitor;
  m_surface_area = boost::apply_visitor(visitor, m_polyhedron);
}

//! \brief computes the volume of the polyhedron.
Float Indexed_face_set::volume()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::volume(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_volume) clean_volume();
  return m_volume;
}

//! \brief computes the surface area of the polyhedron.
Float Indexed_face_set::surface_area()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::surface_area(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  if (m_dirty_surface_area) clean_surface_area();
  return m_surface_area;
}

//! \brief determines wheather the mesh is consistent.
Boolean Indexed_face_set::is_consistent()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::is_consistent(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_consistent;
}

//! \brief determines whether the mesh has singular vertices.
Boolean Indexed_face_set::has_singular_vertices()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_has_singular_vertices;
}

//! \brief determines whether the polyhedron representation is empty.
bool Indexed_face_set::is_polyhedron_empty()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  return boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron);
}

//! \brief determines whether there are no border edges.
Boolean Indexed_face_set::is_closed()
{
  SGAL_TRACE_CODE(Trace::INDEXED_FACE_SET,
                  std::cout << "Indexed_face_set::is_closed(): "
                  << "name: " << get_name() << std::endl;);

  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  return boost::apply_visitor(Is_closed_polyhedron_visitor(), m_polyhedron);
}

//! \brief obtains the number of border edges.
size_t Indexed_face_set::get_number_of_border_edges()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  Border_edges_counter_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron);
}

//! \bried obtains the number of connected components.
Size Indexed_face_set::get_number_of_connected_components()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  Connected_components_counter_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron);
}

//! \brief obtains the number of vertices.
Size Indexed_face_set::get_number_of_vertices()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  Polyhedron_vertices_counter_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron);
}

//! \brief obtains the number of edges.
Size Indexed_face_set::get_number_of_edges()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  Polyhedron_halfedges_counter_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron) / 2;
}

//! \brief obtains the number of facets.
Size Indexed_face_set::get_number_of_facets()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();
  Polyhedron_facets_counter_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron);
}

//! \brief cleans the polyhedron facets.
void Indexed_face_set::clean_polyhedron_facet_normals()
{
  m_dirty_polyhedron_facet_normals = false;
  boost::apply_visitor(Clean_facet_normals_visitor(), m_polyhedron);
  m_dirty_normal_attributes = true;
}

//! \brief cleans the polyhedron edges.
void Indexed_face_set::clean_normal_attributes()
{
  m_dirty_normal_attributes = false;
  Clean_normal_attributes_visitor visitor(get_crease_angle());
  std::pair<Boolean, Boolean> res = boost::apply_visitor(visitor, m_polyhedron);
  m_smooth = res.first;
  m_creased = res.second;
}

//! brief sets the polyhedron type.
void Indexed_face_set::set_polyhedron_type(Polyhedron_type type)
{
  if (m_polyhedron_type == type) return;
  clear_polyhedron();
  m_polyhedron_type = type;
  init_polyhedron();
}

//! \brief adds the container to a given scene.
void Indexed_face_set::add_to_scene(Scene_graph* sg)
{
  Boundary_set::add_to_scene(sg);
  const auto* conf = sg->get_configuration();

  if (!conf) return;

  auto modeling = conf->get_modeling();
  if (!modeling) return;

  set_make_consistent(modeling->get_make_consistent());
  set_repair(modeling->get_triangulate_holes(),
             modeling->get_repair_orientation());
  m_refine = modeling->get_refine();
  m_fair = modeling->get_fair();
  m_repair_normals = modeling->get_repair_normals();
  if (m_repair_normals) m_normals_repaired = false;
}

//! \brief responds to a change in the coordinate-index array.
void Indexed_face_set::coord_indices_changed(const Field_info* field_info)
{
  clear_polyhedron();
  Boundary_set::coord_indices_changed(field_info);
}

//! \brief responds to a change in the normal-index array.
void Indexed_face_set::normal_indices_changed(const Field_info* field_info)
{ Boundary_set::normal_indices_changed(field_info); }

//! \brief responds to a change in the color-index array.
void Indexed_face_set::color_indices_changed(const Field_info* field_info)
{ Boundary_set::color_indices_changed(field_info); }

//! \brief responds to a change in the texture-coordinate index array.
void Indexed_face_set::tex_coord_indices_changed(const Field_info* field_info)
{ Boundary_set::tex_coord_indices_changed(field_info); }

//! \brief responds to a change in the facet coordinate-index array.
void Indexed_face_set::facet_coord_indices_changed()
{
  clear_polyhedron();
  Boundary_set::facet_coord_indices_changed();
}

//! \brief responds to a change in the facet normal-index array.
void Indexed_face_set::facet_normal_indices_changed()
{ Boundary_set::facet_normal_indices_changed(); }

//! \brief responds to a change in the facet color-index array.
void Indexed_face_set::facet_color_indices_changed()
{ Boundary_set::facet_color_indices_changed(); }

//! \brief responds to a change in the facet texture-coordinate index array.
void Indexed_face_set::facet_tex_coord_indices_changed()
{ Boundary_set::facet_tex_coord_indices_changed(); }

//! \brief obtains the empty polyhedron.
Indexed_face_set::Polyhedron& Indexed_face_set::get_empty_polyhedron()
{
  init_polyhedron();
  polyhedron_changed();
  return m_polyhedron;
}

//! \brief obtains the empty inexact polyhedron.
Inexact_polyhedron& Indexed_face_set::get_empty_inexact_polyhedron()
{
  m_polyhedron = Inexact_polyhedron();
  m_polyhedron_type = POLYHEDRON_INEXACT;
  polyhedron_changed();
  return boost::get<Inexact_polyhedron>(m_polyhedron);
}

//! \brief obtains the empty epic  polyhedron.
Epic_polyhedron& Indexed_face_set::get_empty_epic_polyhedron()
{
  m_polyhedron = Epic_polyhedron();
  m_polyhedron_type = POLYHEDRON_EPIC;
  polyhedron_changed();
  return boost::get<Epic_polyhedron>(m_polyhedron);
}

//! \brief obtains the empty epec  polyhedron.
Epec_polyhedron& Indexed_face_set::get_empty_epec_polyhedron()
{
  m_polyhedron = Epec_polyhedron();
  m_polyhedron_type = POLYHEDRON_EPEC;
  polyhedron_changed();
  return boost::get<Epec_polyhedron>(m_polyhedron);
}

/*! \brief Sets the flag that indicates whether to compute the convex hull
 * of the coordinate set.
 */
void Indexed_face_set::set_convex_hull(Boolean flag)
{
  if (m_convex_hull == flag) return;
  m_convex_hull = flag;
  clear_polyhedron();
}

//! \brief computes the volume of the convex hull of the polyhedron.
Float Indexed_face_set::volume_of_convex_hull()
{
  if (is_dirty_coord_array()) clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();
  if (!m_repaired && (m_triangulate_holes || m_repair_orientation)) repair();
  if (m_dirty_polyhedron) clean_polyhedron();

  Float volume = 0.0f;

  // If the polyhedron is empty, return.
  if (boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron))
    return volume;

  //! \todo replace with a visitor.
  const auto& polyhedron = boost::get<Epec_polyhedron>(m_polyhedron);

  if (is_convex_hull()) {
    typedef CGAL::Triangulation_3<Epec_kernel>                Triangulation;
    Triangulation tri(polyhedron.points_begin(), polyhedron.points_end());
    for (auto it = tri.finite_cells_begin(); it != tri.finite_cells_end(); ++it)
    {
      auto tetr = tri.tetrahedron(it);
      volume += CGAL::to_double(tetr.volume());
    }
  }
  else {
    Epec_polyhedron ch;
    CGAL::convex_hull_3(polyhedron.points_begin(), polyhedron.points_end(), ch);

    typedef CGAL::Triangulation_3<Epec_kernel>                Triangulation;
    Triangulation tri(ch.points_begin(), ch.points_end());
    for (auto it = tri.finite_cells_begin(); it != tri.finite_cells_end(); ++it)
    {
      auto tetr = tri.tetrahedron(it);
      volume += CGAL::to_double(tetr.volume());
    }
  }

  return volume;
}

//! \brief sets the flag that indicates whether to enable consistency repairing.
void Indexed_face_set::set_make_consistent(Boolean flag)
{
  m_make_consistent = flag;
  if (m_make_consistent) m_consistent = false;
}

//! \brief sets the flag that indicates whether to enable repairing procedures.
void Indexed_face_set::set_repair(Boolean triangulate_holes,
                                  Boolean repair_orientation)
{
  m_triangulate_holes = triangulate_holes;
  m_repair_orientation = repair_orientation;
  if (m_triangulate_holes || m_repair_orientation) m_repaired = false;
}

SGAL_END_NAMESPACE
