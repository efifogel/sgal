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

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

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
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/calculate_multiple_normals_per_vertex.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Modeling.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Hole_filler_visitor.hpp"
#include "SGAL/Clean_facet_indices_from_polyhedron_visitor.hpp"
#include "SGAL/Orient_polygon_soup_visitor.hpp"
#include "SGAL/Number_of_connected_components_polyhedron_visitor.hpp"

SGAL_BEGIN_NAMESPACE

//! \todo #include "Model_stats.h"

const std::string Indexed_face_set::s_tag = "IndexedFaceSet";
Container_proto* Indexed_face_set::s_prototype(nullptr);

REGISTER_TO_FACTORY(Indexed_face_set, "Indexed_face_set");

//! \brief constructs from the prototype.
Indexed_face_set::Indexed_face_set(Boolean proto) :
  Boundary_set(proto),
  m_repaired(true),
  m_dirty_volume(true),
  m_dirty_surface_area(true),
  m_polyhedron_type(POLYHEDRON_INEXACT),
  m_dirty_coord_array(true),
  m_dirty_polyhedron(true),
  m_dirty_repaired_polyhedron(true),
  m_dirty_repaired_coords(true),
  m_dirty_polyhedron_facet_normals(true),
  m_dirty_normal_attributes(true),
  m_consistent(true),
  m_has_singular_vertices(false),
  m_triangulate(Modeling::s_def_triangulate),
  m_refine(Modeling::s_def_refine),
  m_fair(Modeling::s_def_refine),
  m_repair_orientation(Modeling::s_def_repair_orientation)
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
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (!coords) m_coord_array.reset();
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
                                           Field_info::RULE_OUT, volume_func));

  // surfaceArea
  auto surface_area_func = static_cast<Float_handle_function>
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
  if (s_prototype == nullptr) Indexed_face_set::init_prototype();
  return s_prototype;
}

//! \brief cleans the repaired coordinates and the coordinate indices.
void Indexed_face_set::clean_repaired_coords()
{
  // If triangulation of holes or orientation repairing is required,
  // first clean the repaired data structure. This either leaves the polyhedron
  // clean of the coordinate and the coordinate-indices clean; then, re-clean
  // the coordinates and the coordinate indices.
  if (!m_repaired && (m_triangulate || m_repair_orientation)) repair();

  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();

  m_dirty_repaired_coords = false;
}

//! \brief draws the polygons.
void Indexed_face_set::draw(Draw_action* action)
{
  if (m_dirty_repaired_coords) clean_repaired_coords();
  Boundary_set::draw(action);
}

//! \brief draws the polygons for selection.
void Indexed_face_set::isect(Isect_action* action)
{
  if (m_dirty_repaired_coords) clean_repaired_coords();
  Boundary_set::isect(action);
}

//! \brief cleans the sphere bound.
void Indexed_face_set::clean_bounding_sphere()
{
  if (m_dirty_repaired_coords) clean_repaired_coords();
  Boundary_set::clean_bounding_sphere();
}

//! \brief cleans the coordinate array and coordinate indices.
void Indexed_face_set::clean_coords()
{
  m_dirty_coord_array = false;

  if (m_dirty_polyhedron) clean_polyhedron();
  if (is_polyhedron_empty()) return;

  Size_of_vertices_visitor indices_visitor;
  auto size_of_vertices = boost::apply_visitor(indices_visitor, m_polyhedron);

  if (!m_coord_array) {
    m_coord_array.reset(new Coord_array_3d(size_of_vertices));
    SGAL_assertion(m_coord_array);
  }
  else m_coord_array->resize(size_of_vertices);

  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);

  /* Generate the coordinate array and assign the index into the coordinate
   * array of the vertex to the vertex.
   */
  Compute_coords_visitor<Coord_array_3d::iterator>
    coords_visitor(coords->begin());
  boost::apply_visitor(coords_visitor, m_polyhedron);

  clean_coord_indices();

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
  m_dirty_coord_array = true;
  if (m_coord_array) m_coord_array->clear();
}

//! \brief obtains the coordinate array.
Indexed_face_set::Shared_coord_array Indexed_face_set::get_coord_array()
{
  if (m_dirty_repaired_coords) clean_repaired_coords();
  return m_coord_array;
}

//! \brief cleans the coordinate indices.
void Indexed_face_set::clean_coord_indices()
{
  if (is_polyhedron_empty()) {
    m_dirty_coord_indices = false;
    m_dirty_facet_coord_indices = false;
    return;
  }

  auto num_primitives =
    boost::apply_visitor(Size_of_facets_visitor(), m_polyhedron);
  set_num_primitives(num_primitives);

  auto primitive_type =
    boost::apply_visitor(Type_polyhedron_visitor(), m_polyhedron);
  set_primitive_type(primitive_type);

  init_facet_coord_indices();
  Clean_facet_indices_from_polyhedron_visitor visitor(num_primitives);
  boost::apply_visitor(visitor, m_polyhedron, m_facet_coord_indices);

  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;
}

//! \brief cleans the normal array and the normal indices.
void Indexed_face_set::clean_normals()
{
  if ((0 < m_crease_angle) && (m_crease_angle < SGAL_PI)) {
    if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
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
  if (m_dirty_polyhedron) clean_polyhedron();

  Number_of_connected_components_polyhedron_visitor num_ccs_visitor;
  auto num_ccs = boost::apply_visitor(num_ccs_visitor, m_polyhedron);
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
      if (is_dirty_coord_array() ||
          (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
        clean_coords();
      reverse_facet_coord_indices();
    }
  }

  m_repaired = true;
}

//! \brief cleans the polyhedron data structure.
void Indexed_face_set::clean_repaired_polyhedron()
{
  // If triangulation of holes or orientation repairing is required,
  // first clean the repaired data structure. This either leaves the polyhedron
  // clean of the coordinate and the coordinate-indices clean; then, re-clean
  // the polyhedron.
  if (!m_repaired && (m_triangulate || m_repair_orientation)) repair();

  //! \todo Do not attempt to orient the polygon in the 2nd call.
  if (m_dirty_polyhedron) clean_polyhedron();

  m_dirty_repaired_polyhedron = false;
}

//! \brief cleans the polyhedron data structure.
void Indexed_face_set::clean_polyhedron()
{
  // Set the flags at the beginning in case of an earlt return.
  m_dirty_polyhedron = false;
  m_consistent = true;
  m_has_singular_vertices = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  m_dirty_polyhedron_facet_normals = true;
  m_dirty_normal_attributes = true;

  // Clean the coordinates and the coordinate indices (without repairing).
  if (is_dirty_coord_array() ||
      (is_dirty_coord_indices() && is_dirty_facet_coord_indices()))
    clean_coords();
  if (is_dirty_facet_coord_indices()) clean_facet_coord_indices();

  // If there are no coordinates bail out.
  if (!m_coord_array || m_coord_array->empty()) return;
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (!coords) return;

  // If there are no coordinate indices bail out.
  if (empty_facet_indices(m_facet_coord_indices)) return;

  Orient_polygon_soup_visitor visitor(coords);
  m_has_singular_vertices = boost::apply_visitor(visitor, m_facet_coord_indices);

  // create the polyhedral surface
  if ((m_polyhedron_type == POLYHEDRON_INEXACT) && m_triangulate)
    m_polyhedron_type = POLYHEDRON_EPIC;

  switch (m_polyhedron_type) {
   case POLYHEDRON_INEXACT:
    {
     auto& polyhedron = empty_inexact_polyhedron();
     Polyhedron_geo_builder<Inexact_polyhedron::HalfedgeDS> surface(this);
     polyhedron.delegate(surface);
     m_consistent = surface.is_consistent();
    }
    break;

   case POLYHEDRON_EPIC:
    {
     auto& polyhedron = empty_epic_polyhedron();
     Polyhedron_geo_builder<Epic_polyhedron::HalfedgeDS> surface(this);
     polyhedron.delegate(surface);
     m_consistent = surface.is_consistent();
    }
    break;

   case POLYHEDRON_EPEC:
    {
     auto& polyhedron = empty_epec_polyhedron();
     Polyhedron_geo_builder<Epec_polyhedron::HalfedgeDS> surface(this);
     polyhedron.delegate(surface);
     m_consistent = surface.is_consistent();
    }
    break;

   default: SGAL_error();
  }

  // Normalize the border of the polyhedron.
  boost::apply_visitor(Normalize_border_visitor(), m_polyhedron);

  auto closed =
    boost::apply_visitor(Is_closed_polyhedron_visitor(), m_polyhedron);
  if (!closed && m_triangulate) {
    Hole_filler_visitor visitor(m_refine, m_fair);
    boost::apply_visitor(visitor, m_polyhedron);

    //! \todo instead of brutally clearing the coords and indices arrays,
    // update these arrays based on the results of the hole-filler visitor.
    clear_coord_array();
    clear_coord_indices();
    clear_facet_coord_indices();
  }
}

//! \brief clears the polyhedron.
void Indexed_face_set::clear_polyhedron()
{
  m_dirty_polyhedron = true;
  m_dirty_polyhedron_facet_normals = true;
  m_dirty_normal_attributes = true;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  boost::apply_visitor(Clear_polyhedron_visitor(), m_polyhedron);
}

//! \brief sets the polyhedron data-structure.
void Indexed_face_set::set_polyhedron(Polyhedron& polyhedron)
{
  m_polyhedron = polyhedron;
  m_dirty_polyhedron = false;
  m_dirty_polyhedron_facet_normals = false;
  m_dirty_normal_attributes = false;
  m_dirty_volume = true;
  m_dirty_surface_area = true;
  clear_coord_array();
  clear_coord_indices();
  clear_facet_coord_indices();

  // Assume that the incoming polyhedron has no singular vertices, is
  // consistent, and repaired.
  m_consistent = true;
  m_has_singular_vertices = false;
  m_repaired = true;
}

//! \brief obtains the polyhedron data-structure.
const Indexed_face_set::Polyhedron&
Indexed_face_set::get_polyhedron(Boolean clean_facet_normals)
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
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

//! Write this container.
void Indexed_face_set::write(Formatter* formatter)
{
  SGAL_TRACE_CODE(Trace::EXPORT,
                  std::cout << "Indexed_face_set: " << "Tag: " << get_tag()
                  << ", name: " << get_name()
                  << std::endl;);

  if (m_dirty_repaired_coords) clean_repaired_coords();
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
  m_volume = boost::apply_visitor(Volume_visitor(), m_polyhedron);
}

//! \brief cleans (compute) the surface area.
void Indexed_face_set::clean_surface_area()
{
  m_dirty_surface_area = false;

  m_surface_area = 0;
  if (is_polyhedron_empty()) return;
   m_surface_area = boost::apply_visitor(Surface_area_visitor(), m_polyhedron);
}

//! \brief computes the volume of the polyhedron.
Float Indexed_face_set::volume()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  if (m_dirty_volume) clean_volume();
  return m_volume;
}

//! \brief computes the surface area of the polyhedron.
Float Indexed_face_set::surface_area()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  if (m_dirty_surface_area) clean_surface_area();
  return m_surface_area;
}

//! \brief determines wheather the mesh is consistent.
Boolean Indexed_face_set::is_consistent()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  return m_consistent;
}

//! \brief determines whether the mesh has singular vertices.
Boolean Indexed_face_set::has_singular_vertices()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return m_has_singular_vertices;
}

//! \brief determines whether the polyhedron representation is empty.
bool Indexed_face_set::is_polyhedron_empty()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return boost::apply_visitor(Empty_polyhedron_visitor(), m_polyhedron);
}

//! \brief determines whether there are no border edges.
Boolean Indexed_face_set::is_closed()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  return boost::apply_visitor(Is_closed_polyhedron_visitor(), m_polyhedron);
}

//! \brief obtains the number of border edges.
size_t Indexed_face_set::get_number_of_border_edges()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  return boost::apply_visitor(Size_of_border_edges_polyhedron_visitor(),
                              m_polyhedron);
}

//! \bried obtains the number of connected components.
Size Indexed_face_set::get_number_of_connected_components()
{
  if (m_dirty_polyhedron) clean_polyhedron();
  Number_of_connected_components_polyhedron_visitor visitor;
  return boost::apply_visitor(visitor, m_polyhedron);
}

//! \brief obtains the number of vertices.
Size Indexed_face_set::get_number_of_vertices()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  return boost::apply_visitor(Size_of_vertices_visitor(), m_polyhedron);
}

//! \brief obtains the number of edges.
Size Indexed_face_set::get_number_of_edges()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  return boost::apply_visitor(Size_of_halfedges_visitor(), m_polyhedron) / 2;
}

//! \brief obtains the number of facets.
Size Indexed_face_set::get_number_of_facets()
{
  if (m_dirty_repaired_polyhedron) clean_repaired_polyhedron();
  return boost::apply_visitor(Size_of_facets_visitor(), m_polyhedron) / 2;
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

//! \brief obtains an empty inexact polyhedron.
Inexact_polyhedron& Indexed_face_set::empty_inexact_polyhedron()
{
  m_polyhedron = Inexact_polyhedron();
  return boost::get<Inexact_polyhedron>(m_polyhedron);
}

//! \brief obtains an empty epic polyhedron.
Epic_polyhedron& Indexed_face_set::empty_epic_polyhedron()
{
  m_polyhedron = Epic_polyhedron();
  return boost::get<Epic_polyhedron>(m_polyhedron);
}

//! \brief obtains an empty eprc polyhedron.
Epec_polyhedron& Indexed_face_set::empty_epec_polyhedron()
{
  m_polyhedron = Epec_polyhedron();
  return boost::get<Epec_polyhedron>(m_polyhedron);
}

//! brief sets the polyhedron type.
void Indexed_face_set::set_polyhedron_type(Polyhedron_type type)
{
  if (m_polyhedron_type != type) clear_polyhedron();
  m_polyhedron_type = type;
}

//! \brief configures.
void Indexed_face_set::configure(const Configuration* conf)
{
  if (!conf) return;

  auto modeling = conf->get_modeling();
  if (!modeling) return;

  m_triangulate = modeling->get_triangulate();
  m_refine = modeling->get_refine();
  m_fair = modeling->get_fair();
  m_repair_orientation = modeling->get_repair_orientation();
  if (m_triangulate || m_repair_orientation) m_repaired = false;
}

//! \brief adds the container to a given scene.
void Indexed_face_set::add_to_scene(Scene_graph* sg)
{
  const auto* conf = sg->get_configuration();
  configure(conf);
}

//! \brief responds to a change in the coordinate-index array.
void Indexed_face_set::coord_indices_changed(const Field_info* field_info)
{
  m_dirty_polyhedron = true;
  Boundary_set::coord_indices_changed(field_info);
}

//! \brief responds to a change in the normal-index array.
void Indexed_face_set::normal_indices_changed(const Field_info* field_info)
{
  m_dirty_polyhedron = true;
  Boundary_set::normal_indices_changed(field_info);
}

//! \brief responds to a change in the color-index array.
void Indexed_face_set::color_indices_changed(const Field_info* field_info)
{
  m_dirty_polyhedron = true;
  Boundary_set::color_indices_changed(field_info);
}

//! \brief responds to a change in the texture-coordinate index array.
void Indexed_face_set::tex_coord_indices_changed(const Field_info* field_info)
{
  m_dirty_polyhedron = true;
  Boundary_set::tex_coord_indices_changed(field_info);
}

//! \brief responds to a change in the facet coordinate-index array.
void Indexed_face_set::facet_coord_indices_changed()
{
  m_dirty_polyhedron = true;
  Boundary_set::facet_coord_indices_changed();
}

//! \brief responds to a change in the facet normal-index array.
void Indexed_face_set::facet_normal_indices_changed()
{
  m_dirty_polyhedron = true;
  Boundary_set::facet_normal_indices_changed();
}

//! \brief responds to a change in the facet color-index array.
void Indexed_face_set::facet_color_indices_changed()
{
  m_dirty_polyhedron = true;
  Boundary_set::facet_color_indices_changed();
}

//! \brief responds to a change in the facet texture-coordinate index array.
void Indexed_face_set::facet_tex_coord_indices_changed()
{
  m_dirty_polyhedron = true;
  Boundary_set::facet_tex_coord_indices_changed();
}

SGAL_END_NAMESPACE
