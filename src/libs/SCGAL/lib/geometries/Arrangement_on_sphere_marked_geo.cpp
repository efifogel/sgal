// Copyright (c) 2007 Israel.
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
#pragma warning( disable : 4146 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4390 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a spherewith an extended dcel. Every vertex,
 * halfedge, and face is extended with a Boolean mark.
 */

#include <list>
#include <boost/lexical_cast.hpp>

#include <CGAL/basic.h>
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_partition_traits_2.h>
#include <CGAL/partition_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Epec_coord_array_3d.hpp"
#include "SGAL/Utilities.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_marked_geo.hpp"
#include "SCGAL/Arrangement_on_surface_geo_overlay.hpp"
#include "SCGAL/Arrangement_on_sphere_renderers.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Arrangement_on_sphere_marked_geo::s_tag =
  "ArrangementOnSphereMarked";
Container_proto* Arrangement_on_sphere_marked_geo::s_prototype(NULL);

// Default values
const Arrangement_on_sphere_marked_geo::Vertex_style
  Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_vertex_style(Vertex_shape::BALL);
const Float Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_vertex_radius(0.06f);
const Float Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_vertex_point_size(2);
const Vector3f Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_vertex_color(0.4f, 0.4f, 0.2f);

const Boolean Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_enabled(true);
const Arrangement_on_sphere_marked_geo::Edge_style
  Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_style(Edge_shape::TUBE);
const Uint Arrangement_on_sphere_marked_geo::s_def_aos_marked_edge_count(1);
const Boolean Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_directed(false);
const Float Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_radius(0.03f);
const Float Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_line_width(1);
const Vector3f Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_edge_color(.4f, .4f, .2f);

const Vector3f Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_face_color(.2f, .4f, .8f);
const Float Arrangement_on_sphere_marked_geo::
  s_def_aos_marked_face_transparency(0.4f);

const Boolean Arrangement_on_sphere_marked_geo::
  s_def_draw_aos_marked_vertex(true);
const Boolean Arrangement_on_sphere_marked_geo::
  s_def_draw_aos_marked_edge(true);
const Boolean Arrangement_on_sphere_marked_geo::
  s_def_draw_aos_marked_face(true);

REGISTER_TO_FACTORY(Arrangement_on_sphere_marked_geo,
                    "Arrangement_on_sphere_marked_geo");

  /*! Constructor */
Arrangement_on_sphere_marked_geo::
Arrangement_on_sphere_marked_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_owned_aos(false),
  m_aos(nullptr),
  m_aos_marked_vertex_style(s_def_aos_marked_vertex_style),
  m_aos_marked_vertex_radius(s_def_aos_marked_vertex_radius),
  m_aos_marked_vertex_point_size(s_def_aos_marked_vertex_point_size),
  m_aos_marked_vertex_color(s_def_aos_marked_vertex_color),
  m_aos_marked_edge_enabled(s_def_aos_marked_edge_enabled),
  m_aos_marked_edge_style(s_def_aos_marked_edge_style),
  m_aos_marked_edge_count(s_def_aos_marked_edge_count),
  m_aos_marked_edge_directed(s_def_aos_marked_edge_directed),
  m_aos_marked_edge_radius(s_def_aos_marked_edge_radius),
  m_aos_marked_edge_line_width(s_def_aos_marked_edge_line_width),
  m_aos_marked_edge_color(s_def_aos_marked_edge_color),
  m_aos_marked_face_color(s_def_aos_marked_face_color),
  m_aos_marked_face_transparency(s_def_aos_marked_face_transparency),
  m_marked_face_renderer(nullptr)
{ if (!proto) create_renderers(); }

/*! Destructor */
Arrangement_on_sphere_marked_geo::~Arrangement_on_sphere_marked_geo()
{
  clear();
  if (m_owned_aos) {
    if (m_aos) {
      delete m_aos;
      m_aos = nullptr;
    }
    m_owned_aos = false;
  }
}

/*! \brief initializes the container prototype. */
void Arrangement_on_sphere_marked_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());

  // aosMarkedVertexRadius
  auto aos_marked_vertex_radius_func =
    static_cast<Float_handle_function>
    (&Arrangement_on_sphere_marked_geo::aos_marked_vertex_radius_handle);
  s_prototype->add_field_info(new SF_float(AOS_MARKED_VERTEX_RADIUS,
                                           "aosMarkedVertexRadius",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           aos_marked_vertex_radius_func));

  // aosMarkedEdgeRadius
  auto aos_marked_edge_radius_func =
    static_cast<Float_handle_function>
    (&Arrangement_on_sphere_marked_geo::aos_marked_edge_radius_handle);
  s_prototype->add_field_info(new SF_float(AOS_MARKED_EDGE_RADIUS,
                                           "aosMarkedEdgeRadius",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           aos_marked_edge_radius_func));
}

/*! \brief deletes the container prototype. */
void Arrangement_on_sphere_marked_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Arrangement_on_sphere_marked_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_marked_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes. */
void Arrangement_on_sphere_marked_geo::
set_attributes(Element* elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "aosMarkedVertexStyle") {
      m_aos_marked_vertex_style = Vertex_shape::style(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedVertexRadius") {
      m_aos_marked_vertex_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedVertexPointSize") {
      m_aos_marked_vertex_point_size = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedVertexColor") {
      Vector3f col(value);
      m_aos_marked_vertex_color = col;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeEnabled") {
      m_aos_marked_edge_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
     if (name == "aosMarkedEdgeStyle") {
      m_aos_marked_edge_style = Edge_shape::style(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeCount") {
      m_aos_marked_edge_count = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeDirected") {
      m_aos_marked_edge_directed = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeRadius") {
      m_aos_marked_edge_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeLineWidth") {
      m_aos_marked_edge_line_width = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedEdgeColor") {
      Vector3f col(value);
      m_aos_marked_edge_color = col;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "aosMarkedFaceColor") {
      Vector3f col(value);
      m_aos_marked_face_color = col;
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosMarkedFaceTransparency") {
      m_aos_marked_face_transparency = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }

    if (name == "aosMarkedFaceIndex") {
      tokenizer tokens(value, sep);
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_marked_faces_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_marked_faces_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_marked_faces_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }

    if (name == "aosMarkedEdgeIndex") {
      tokenizer tokens(value, sep);
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_marked_edges_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_marked_edges_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_marked_edges_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }

    if (name == "aosMarkedVertexIndex") {
      tokenizer tokens(value, sep);
      auto size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_marked_vertices_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_marked_vertices_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_marked_vertices_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "overlay") {
      auto aos_geo =
        boost::dynamic_pointer_cast<Arrangement_on_sphere_marked_geo>(cont);
      if (aos_geo) add_aos_geo(aos_geo);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const auto& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "overlay") {
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        auto cont = *ci;
        auto aos_geo =
          boost::dynamic_pointer_cast<Arrangement_on_sphere_marked_geo>(cont);
        if (aos_geo) add_aos_geo(aos_geo);
      }
      elem->mark_delete(mcai);
    }
    continue;
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  std::sort(m_marked_faces_indices.begin(), m_marked_faces_indices.end());
  std::sort(m_marked_edges_indices.begin(), m_marked_edges_indices.end());
  std::sort(m_marked_vertices_indices.begin(), m_marked_vertices_indices.end());
}

/*! \brief cleans the representation. */
void Arrangement_on_sphere_marked_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos_marked;
    SGAL_assertion(m_aos);
    m_owned_aos = true;
  }
  insert_all(m_aos);
  overlay_all(m_aoses.begin(), m_aoses.end(),
              std::distance(m_aoses.begin(), m_aoses.end()), this);

  // Locate points:
  auto exact_coord_array =
    boost::dynamic_pointer_cast<Epec_coord_array_3d>(m_coord_array);
  if (exact_coord_array && (exact_coord_array->size() > 0)) {
    Aos_point_location_strategy naive_pl(*m_aos);
    Epec_kernel kernel;
    for (auto it = m_point_location_indices.begin();
         it != m_point_location_indices.end(); ++it)
    {
      auto& point = (*exact_coord_array)[*it];
      Epec_vector_3 vec =
        kernel.construct_vector_3_object()(CGAL::ORIGIN, point);
      Epec_direction_3 dir = kernel.construct_direction_3_object()(vec);
      // Perform the point-location query.
      CGAL::Object obj = naive_pl.locate(dir);
      Aos_marked::Vertex_const_handle v;
      Aos_marked::Halfedge_const_handle e;
      Aos_marked::Face_const_handle f;
      if (CGAL::assign(f, obj)) {
        // std::cout << "face" << std::endl;
        m_aos->non_const_handle(f)->set_mark(true);
        // Mark the boundaries, since we cannot draw the face (yet):
        for (auto oit = f->outer_ccbs_begin(); oit != f->outer_ccbs_end();
             ++oit)
        {
          auto first = *oit;
          auto curr = first;
          do {
            m_aos->non_const_handle(curr)->set_mark(true);
            m_aos->non_const_handle(curr)->twin()->set_mark(true);
            m_aos->non_const_handle(curr->target())->set_mark(true);
            curr = curr->next();
          } while (curr != first);
        }
        for (auto iit = f->inner_ccbs_begin(); iit != f->inner_ccbs_end();
             ++iit)
        {
          auto first = *iit;
          auto curr = first;
          do {
            m_aos->non_const_handle(curr)->set_mark(true);
            m_aos->non_const_handle(curr)->twin()->set_mark(true);
            m_aos->non_const_handle(curr->target())->set_mark(true);
            curr = curr->next();
          } while (curr != first);
        }
      }
      else if (CGAL::assign(e, obj)) {
        // std::cout << "edge" << std::endl;
        m_aos->non_const_handle(e)->set_mark(true);
        m_aos->non_const_handle(e)->twin()->set_mark(true);
      }
      else if (CGAL::assign(v, obj)) {
        // std::cout << "vertex" << std::endl;
        m_aos->non_const_handle(v)->set_mark(true);
      }
      else {
        // std::cout << "none" << std::endl;
        SGAL_assertion(0);
      }
    }
  }

  if (!m_marked_faces_indices.empty()) {
    Uint i = 0;
    auto fit = m_aos->faces_begin();
    for (auto fiit = m_marked_faces_indices.begin();
         fiit != m_marked_faces_indices.end(); ++fiit)
    {
      auto index = *fiit;
      if (index >= m_aos->number_of_faces()) break;
      std::advance(fit, index - i);
      i = index;
      fit->set_mark(true);
    }
  }

  if (!m_marked_edges_indices.empty()) {
    Uint i = 0;
    auto eit = m_aos->edges_begin();
    for (auto eiit = m_marked_edges_indices.begin();
         eiit != m_marked_edges_indices.end(); ++eiit)
    {
      auto index = *eiit;
      if (index >= m_aos->number_of_edges()) break;
      std::advance(eit, index - i);
      i = index;
      eit->set_mark(true);
    }
  }

  if (!m_marked_vertices_indices.empty()) {
    Uint i = 0;
    auto vit = m_aos->vertices_begin();
    for (auto viit = m_marked_vertices_indices.begin();
         viit != m_marked_vertices_indices.end(); ++viit)
    {
      auto index = *viit;
      if (index >= m_aos->number_of_vertices()) break;
      std::advance(vit, index - i);
      i = index;
      vit->set_mark(true);
    }
  }

  {
    std::cout << "Faces:" << std::endl;
    Aos_marked::Face_const_iterator fit;
    for (fit = m_aos->faces_begin(); fit != m_aos->faces_end(); ++fit) {
      std::cout << "  Outer CCBs: "
                << std::distance(fit->outer_ccbs_begin(), fit->outer_ccbs_end())
                << std::endl;
      Aos_marked::Outer_ccb_const_iterator oit;
      for (oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end(); ++oit) {
        Aos_marked::Halfedge_const_iterator first = *oit;
        Aos_marked::Halfedge_const_iterator curr = first;
        do {
          std::cout << "  " << curr->curve() << std::endl;
          curr = curr->next();
        } while (curr != first);
      }

      std::cout << "  Inner CCBs: "
                << std::distance(fit->inner_ccbs_begin(), fit->inner_ccbs_end())
                << std::endl;
      Aos_marked::Inner_ccb_const_iterator iit;
      for (iit = fit->inner_ccbs_begin(); iit != fit->inner_ccbs_end(); ++iit) {
        Aos_marked::Halfedge_const_iterator first = *iit;
        Aos_marked::Halfedge_const_iterator curr = first;
        do {
          std::cout << "  " << curr->curve() << std::endl;
          curr = curr->next();
        } while (curr != first);
      }
    }
    std::cout << std::endl;
  }
}

/*! \brief clears the internal representation and auxiliary data structures. */
void Arrangement_on_sphere_marked_geo::clear()
{
  if (m_aos) m_aos->clear();
  m_aoses.clear();
  m_dirty = true;
}

/*! \brief draws the arrangement on surface vertices with color. */
void Arrangement_on_sphere_marked_geo::
Sphere_marked_colored_vertices_renderer::operator()(Draw_action* action)
{
  for (auto vi = m_geo.m_aos->vertices_begin();
       vi != m_geo.m_aos->vertices_end(); ++vi)
  {
    if (vi->is_isolated()) continue;
    auto center = to_vector3f(vi->point());
    center.normalize();
    if (vi->mark()) {
      const auto& color = m_geo.get_aos_marked_vertex_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_marked_vertex(action, center);
    }
    else {
      const auto& color = m_geo.get_aos_vertex_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_vertex(action, center);
    }
  }
}

/*! \brief draws the arrangement on surface isolated vertices with color. */
void
Arrangement_on_sphere_marked_geo::
Sphere_marked_colored_isolated_vertices_renderer::
operator()(Draw_action* action)
{
  for (auto vi = m_geo.m_aos->vertices_begin();
       vi != m_geo.m_aos->vertices_end(); ++vi)
  {
    if (!(vi->is_isolated())) continue;
    auto center = to_vector3f(vi->point());
    center.normalize();
    if (vi->mark()) {
      const auto& color = m_geo.get_aos_marked_vertex_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_marked_vertex(action, center);
    }
    else {
      const auto& color = m_geo.get_aos_isolated_vertex_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_isolated_vertex(action, center);
    }
  }
}

/*! \brief draws the arrangement on surface edges with color. */
void Arrangement_on_sphere_marked_geo::
Sphere_marked_colored_edges_renderer::operator()(Draw_action* action)
{
  for (auto hei = m_geo.m_aos->edges_begin(); hei != m_geo.m_aos->edges_end();
       ++hei)
  {
    const auto& curve = hei->curve();
    auto src = to_vector3f(curve.source());
    auto trg = to_vector3f(curve.target());
    auto normal = to_vector3f(curve.normal());
    src.normalize();
    trg.normalize();

    if (hei->mark() && m_geo.is_aos_marked_edge_enabled()) {
      const auto& color = m_geo.get_aos_marked_edge_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_marked_edge(action, src, trg, normal);
    }
    else {
      const auto& color = m_geo.get_aos_edge_color();
      glColor3fv((float*)&color);
      m_geo.draw_aos_edge(action, src, trg, normal);
    }
  }
}

/*! \brief draws the marked face (primal vertex) */
void Arrangement_on_sphere_marked_geo::
draw_aos_marked_face(Draw_action* action)
{
  // typedefs for polygon partitioning.
  typedef Aos_marked::Geometry_traits_2::Kernel           Kernel;
  typedef CGAL::Arr_geodesic_arc_on_sphere_partition_traits_2<Kernel>
                                                          Partition_traits_2;
  typedef Partition_traits_2::Polygon_2                   Polygon_2;
  typedef std::list<Polygon_2>                            Polygons_container;

  for (auto fi = m_aos->faces_begin(); fi != m_aos->faces_end(); ++fi) {
    if (!fi->mark()) continue;

    // First, if the face is non-convex then we assume that it does no
    // cross any of the boundaries. We partition the face into convex pieces
    // and then  draw them. (Pay attention to the fact that, for NOW, we
    // assume that the face does not cross any boundaries of the parameter
    // space.)
    Polygon_2 polygon;
    Polygons_container polys;
    for (auto oit = fi->outer_ccbs_begin(); oit != fi->outer_ccbs_end(); ++oit) {
      auto first = *oit;
      auto curr = first;
      do {
        polygon.push_back(curr->target()->point());
        curr = curr->next();
      } while (curr != first);
      SGAL_assertion(polygon.size() >= 3);
      CGAL::approx_convex_partition_2(polygon.vertices_begin(),
                                      polygon.vertices_end(),
                                      std::back_inserter(polys),
                                      Partition_traits_2());
      polygon.clear();
    }

    /* Collect the points on the boundary. Handle the special case, where
     * all curves have the same normal:
     */
    std::list<Vector3f> points;
    for (auto pit = polys.begin(); pit != polys.end(); ++pit) {
      for (auto vit = pit->vertices_begin(); vit != pit->vertices_end(); ++vit) {
        Vector3f vec = to_vector3f(*vit);
        vec.normalize();
        points.push_back(vec);
      }
      SGAL_assertion(points.size() >= 3);
      draw_aos_convex_face(action, points.begin(), points.end());
      points.clear();
    }


//// ophir: why do we paint the holes? Ask Efi about this.
//     for (oit = fi->inner_ccbs_begin(); oit != fi->inner_ccbs_end(); ++oit) {
//       Aos_marked::Halfedge_const_iterator first = *oit;
//       Aos_marked::Halfedge_const_iterator curr = first;
//       do {
//         Vector3f vec = to_vector3f(curr->target()->point());
//         vec.normalize();
//         points.push_back(vec);
//         curr = curr->next();
//       } while (curr != first);
//       SGAL_assertion(points.size() >= 3);
//       draw_aos_convex_face(action, points.begin(), points.end());
//       points.clear();
//     }
  }
}

/*! \brief renders the marked primal vertex. */
void Arrangement_on_sphere_marked_geo::Marked_face_renderer::
operator()(Draw_action* action)
{
  const auto& color = m_geo.get_aos_marked_face_color();
  auto* context = action->get_context();
  Gfx::Cull_face cull_save = Gfx::BACK_CULL;
  Gfx::Light_model_sides lms_save = Gfx::ONE_SIDE;
  if (!m_geo.m_draw_aos_surface) {
    cull_save = context->get_cull_face();
    lms_save = context->get_light_model_sides();
  }
  context->draw_cull_face(Gfx::NO_CULL);
  context->draw_light_model_sides(Gfx::TWO_SIDE);
  glColor4f(color[0], color[1], color[2],
            1 - m_geo.get_aos_marked_face_transparency());
  m_geo.draw_aos_marked_face(action);
  if (!m_geo.m_draw_aos_surface) {
    context->draw_cull_face(cull_save);
    context->draw_light_model_sides(lms_save);
  }
}

/*! \brief creates the renderers. */
void Arrangement_on_sphere_marked_geo::create_renderers()
{
  m_edges_renderer = new Sphere_marked_edges_renderer(*this);
  m_vertices_renderer = new Sphere_marked_vertices_renderer(*this);
  m_isolated_vertices_renderer =
    new Sphere_marked_isolated_vertices_renderer(*this);
  m_colored_edges_renderer = new Sphere_marked_colored_edges_renderer(*this);
  m_colored_vertices_renderer =
    new Sphere_marked_colored_vertices_renderer(*this);
  m_colored_isolated_vertices_renderer =
    new Sphere_marked_colored_isolated_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Sphere_marked_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Sphere_marked_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Sphere_marked_ring_colored_vertices_renderer(*this);
  m_point_colored_isolated_vertices_renderer =
    new Sphere_marked_point_colored_isolated_vertices_renderer(*this);
  m_ring_colored_isolated_vertices_renderer =
    new Sphere_marked_ring_colored_isolated_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Sphere_marked_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Sphere_marked_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Sphere_marked_inflated_tube_edges_renderer(*this);

  m_marked_face_renderer = new Marked_face_renderer(*this);
}

/*! \brief destroys the renderers. */
void Arrangement_on_sphere_marked_geo::destroy_renderers()
{
  if (m_edges_renderer) delete m_edges_renderer;
  if (m_vertices_renderer) delete m_vertices_renderer;
  if (m_isolated_vertices_renderer) delete m_isolated_vertices_renderer;
  if (m_colored_edges_renderer) delete m_colored_edges_renderer;
  if (m_colored_vertices_renderer) delete m_colored_vertices_renderer;
  if (m_colored_isolated_vertices_renderer)
    delete m_colored_isolated_vertices_renderer;
  if (m_line_colored_edges_renderer) delete m_line_colored_edges_renderer;
  if (m_point_colored_vertices_renderer)
    delete m_point_colored_vertices_renderer;
  if (m_ring_colored_vertices_renderer)
    delete m_ring_colored_vertices_renderer;
  if (m_point_colored_isolated_vertices_renderer)
    delete m_point_colored_isolated_vertices_renderer;
  if (m_ring_colored_isolated_vertices_renderer)
    delete m_ring_colored_isolated_vertices_renderer;
  if (m_inflated_line_edges_renderer) delete m_inflated_line_edges_renderer;
  if (m_inflated_strip_edges_renderer) delete m_inflated_strip_edges_renderer;
  if (m_inflated_tube_edges_renderer) delete m_inflated_tube_edges_renderer;

  if (m_marked_face_renderer) delete m_marked_face_renderer;
}

/*! \brief cleans the renderer. */
void Arrangement_on_sphere_marked_geo::clean_renderer()
{
  Arrangement_on_surface_geo::clean_renderer();

  // Surface and faces:
  m_renderer.push_front(m_marked_face_renderer,
                        Arrangement_renderer::SURFACE);
}

/*! \brief draws an arrangement on sphere marked vertex. */
void Arrangement_on_sphere_marked_geo::
draw_aos_marked_vertex(Draw_action* action, Vector3f& center)
{
  draw_vertex_on_sphere(action, center, m_aos_marked_vertex_style,
                        m_aos_marked_vertex_radius,
                        m_aos_delta_angle);
}

/*! \brief Draw an arrangement on sphere marked edge. */
void Arrangement_on_sphere_marked_geo::
draw_aos_marked_edge(Draw_action* action,
                     Vector3f& source, Vector3f& target, Vector3f& normal)
{
  draw_edge_on_sphere(action, source, target, normal,
                      m_aos_marked_edge_style,
                      m_aos_marked_edge_count,
                      m_aos_marked_edge_directed,
                      m_aos_marked_edge_radius,
                      m_aos_delta_angle,
                      m_aos_vertex_radius, m_aos_vertex_radius);
}

/*! \brief obrains the arrangement. */
Arrangement_on_sphere_marked* Arrangement_on_sphere_marked_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

/*! \brief sets the arrangement. */
void
Arrangement_on_sphere_marked_geo::set_aos(Arrangement_on_sphere_marked* aos)
{
  m_dirty = false;
  m_aos = aos;
}

SGAL_END_NAMESPACE
