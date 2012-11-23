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
// $Id: Arrangement_on_sphere_sim_geo.cpp 7793 2009-07-19 13:31:22Z efif $
// $Revision: 7793 $
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
 * A geometry container that simulates the construction of an arrangement
 * induced by arcs of great circles embeded on a sphere.
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Tick_event.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Field_infos.hpp"

#include "SCGAL/Arrangement_on_sphere_sim_geo.hpp"

SGAL_BEGIN_NAMESPACE

std::string Arrangement_on_sphere_sim_geo::s_tag = "ArrangementOnSphereSim";
Container_proto * Arrangement_on_sphere_sim_geo::s_prototype = NULL;

REGISTER_TO_FACTORY(Arrangement_on_sphere_sim_geo,
                    "Arrangement_on_sphere_sim_geo");

/*! Default values */
const Boolean
Arrangement_on_sphere_sim_geo::s_def_draw_labeled_vertex(SGAL_FALSE);
const Boolean
Arrangement_on_sphere_sim_geo::s_def_draw_labeled_edge(SGAL_FALSE);
const Boolean
Arrangement_on_sphere_sim_geo::s_def_draw_labeled_face(SGAL_FALSE);

const Vector3f
Arrangement_on_sphere_sim_geo::s_def_labeled_vertex_color(0.5f, 0, 0);
const Vector3f
Arrangement_on_sphere_sim_geo::s_def_labeled_edge_color(0, 0.5f, 0);
const Vector3f
Arrangement_on_sphere_sim_geo::s_def_labeled_face_color(0, 0, 0.5f);

/*! Constructor */
Arrangement_on_sphere_sim_geo::
Arrangement_on_sphere_sim_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_own_aos(SGAL_FALSE),
  m_aos(NULL),
  m_time(0),
  m_resume(SGAL_FALSE),
  m_suspend(SGAL_FALSE),
  m_draw_labeled_vertex(s_def_draw_labeled_vertex),
  m_draw_labeled_edge(s_def_draw_labeled_edge),
  m_draw_labeled_face(s_def_draw_labeled_face),
  m_label(0xffffffff),
  m_vertex_label(0xffffffff),
  m_edge_label(0xffffffff),
  m_face_label(0xffffffff),
  m_labeled_vertex_color(s_def_labeled_vertex_color),
  m_labeled_edge_color(s_def_labeled_edge_color),
  m_labeled_face_color(s_def_labeled_face_color)
{
  if (!proto) create_renderers();
}

/*! Destructor */
Arrangement_on_sphere_sim_geo::~Arrangement_on_sphere_sim_geo()
{
  Tick_event::unregister(this);
  clear();
  if (m_aos && m_own_aos) delete m_aos;
}

/*! \brief initializes the container prototype */
void Arrangement_on_sphere_sim_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);
  
  Execution_function exec_func =
    static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::
                                    time_changed);
  s_prototype->add_field_info(new SF_float(TIME, "time",
                                           get_member_offset(&m_time),
                                           exec_func));

  exec_func =
    static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::resume);
  s_prototype->add_field_info(new SF_bool(RESUME, "resume",
                                          get_member_offset(&m_resume),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::suspend);
  s_prototype->add_field_info(new SF_bool(SUSPEND, "suspend",
                                          get_member_offset(&m_suspend),
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::
                                              increase_vertex_label);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_VERTEX_LABEL, "increaseVertexLabel",
                               get_member_offset(&m_increase_vertex_label),
                               exec_func));
  exec_func = static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::
                                              increase_edge_label);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_EDGE_LABEL, "increaseEdgeLabel",
                               get_member_offset(&m_increase_edge_label),
                               exec_func));
  exec_func = static_cast<Execution_function>(&Arrangement_on_sphere_sim_geo::
                                              increase_face_label);
  s_prototype->
    add_field_info(new SF_bool(INCREASE_FACE_LABEL, "increaseFaceLabel",
                               get_member_offset(&m_increase_face_label),
                               exec_func));  
}

/*! \brief deletes the container prototype */
void Arrangement_on_sphere_sim_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto * Arrangement_on_sphere_sim_geo::get_prototype()
{
  if (!s_prototype) Arrangement_on_sphere_sim_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the container attributes */
void Arrangement_on_sphere_sim_geo::set_attributes(Element * elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);

    if (name == "label") {
      m_label = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "vertexLabel") {
      m_vertex_label = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "edgeLabel") {
      m_edge_label = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "faceLabel") {
      m_face_label = boost::lexical_cast<Uint>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawLabeledVertex") {
      m_draw_labeled_vertex = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawLabeledEdge") {
      m_draw_labeled_edge = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawLabeledFace") {
      m_draw_labeled_face = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "vertexLabel") {
      set_vertex_label(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "edgeLabel") {
      set_edge_label(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "faceLabel") {
      set_face_label(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! Clean the representation */
void Arrangement_on_sphere_sim_geo::clean()
{
  m_dirty = false;
  if (!m_aos) {
    m_aos = new Aos_labeled;
    m_observer.attach(*m_aos);
    m_own_aos = SGAL_TRUE;
  }
  insert_all(m_aos);
  m_num_labels = m_aos->number_of_vertices() + m_aos->number_of_edges();
}

/*! \brief clears the internal representation and auxiliary data structures */
void Arrangement_on_sphere_sim_geo::clear()
{
  if (m_aos) m_aos->clear();
  m_dirty = true;
}

/*! \brief draws the marked vertex */
void Arrangement_on_sphere_sim_geo::draw_labeled_vertex()
{
  Aos_labeled::Vertex_const_iterator vi;
  for (vi = m_aos->vertices_begin(); vi != m_aos->vertices_end(); ++vi) {
    // if (!vi->labeled()) continue;
    Vector3f center = to_vector3f(vi->point());
    center.normalize();
    Sphere sphere;
    sphere.set_center(center);
    sphere.set_radius(m_aos_vertex_radius);
    Draw_action action;
    sphere.draw(&action);
  }
}

/*! \brief draws the labeled edge */
void Arrangement_on_sphere_sim_geo::draw_labeled_edge()
{
#if 0
  float radius = m_edge_radius;
  Aos_labeled::Edge_const_iterator ei;
  for (ei = m_aos->edges_begin(); ei != m_aos->edges_end(); ++ei) {
    if (!ei->labeled()) continue;
    Aos_labeled::Vertex_const_handle source_vertex = ei->source();
    Aos_labeled::Vertex_const_handle target_vertex = ei->target();
    Vector3f src = to_vector3f(source_vertex->point());
    Vector3f trg = to_vector3f(target_vertex->point());
    src.normalize();
    trg.normalize();
    // Draw a tube:
    //! \todo draw a tube:
  }
#endif
}

/*! \brief draws the labeled face */
void Arrangement_on_sphere_sim_geo::draw_labeled_face()
{
#if 0
  Aos_labeled::Face_const_iterator fi;
  for (fi = m_aos->faces_begin(); fi != m_aos->faces_end(); ++fi) {
    if (!fi->labeled()) continue;

    // Draw a patch:
    //! \todo draw a patch
  }
#endif
}

/*! \brief draws the arrangement vertices with color */
void Arrangement_on_sphere_sim_geo::Sphere_sim_colored_vertices_renderer::
operator()(Draw_action * action)
{
  Context * context = action->get_context();
  context->draw_transp_enable(true);

  Aos_labeled::Vertex_const_iterator vi;
  for (vi = m_geo.m_aos->vertices_begin(); vi != m_geo.m_aos->vertices_end();
       ++vi)
  {
    Vector3f center = to_vector3f(vi->point());
    center.normalize();

    float transparency =
      (vi->label() > m_geo.m_label) || (m_geo.m_label == 0xffffffff) ? 0.2f :
      ((vi->label() == m_geo.m_label) ? 1.0f : 0.8f);
    if (vi->point().is_no_boundary()) {
      const Vector3f & color = m_geo.get_aos_vertex_color();
      glColor4f(color[0], color[1], color[2], transparency);
      m_geo.draw_aos_vertex(action, center);
    } else {
      if (m_geo.m_vertex_label == vi->label()) {
        const Vector3f & color = m_geo.get_aos_isolated_vertex_color();
        glColor4f(color[0], color[1], color[2], transparency);
      } else {
        const Vector3f & color = m_geo.get_aos_boundary_vertex_color();
        glColor4f(color[0], color[1], color[2], transparency);
      }
      m_geo.draw_aos_boundary_vertex(action, center);
    }
  }
  context->draw_transp_enable(false);
}

/*! \brief draws the arrangement edges with color */
void Arrangement_on_sphere_sim_geo::Sphere_sim_colored_edges_renderer::
operator()(Draw_action * action)
{
  Context * context = action->get_context();
  context->draw_transp_enable(true);

  Vector3f src;
  Vector3f trg;
  Aos_labeled::Edge_iterator hei;
  for (hei = m_geo.m_aos->edges_begin(); hei != m_geo.m_aos->edges_end(); ++hei)
  {
    const Geometry_traits::X_monotone_curve_2 & curve = hei->curve();
    src = to_vector3f(curve.source());
    trg = to_vector3f(curve.target());
    src.normalize();
    trg.normalize();
    float transparency =
      (hei->label() > m_geo.m_label) || (m_geo.m_label == 0xffffffff) ? 0.2f :
      ((hei->label() == m_geo.m_label) ? 1.0f : 0.8f);
    const Vector3f & color = m_geo.get_aos_edge_color();
    glColor4f(color[0], color[1], color[2], transparency);
    m_geo.draw_aos_edge(action, src, trg);
    
    if (hei->label() == m_geo.m_label) {
      const Geometry_traits::Point_2 & src = hei->source()->point();
      if (!(src.is_no_boundary()))
        m_geo.m_vertex_label = hei->source()->label();
      const Geometry_traits::Point_2 & trg = hei->target()->point();
      if (!(trg.is_no_boundary()))
        m_geo.m_vertex_label = hei->target()->label();
    }
  }
  context->draw_transp_enable(false);
}

/*! \brief handles tick events */
void Arrangement_on_sphere_sim_geo::handle(Tick_event * event)
{
  static Uint cycle = 0;
  if (10 == cycle++) {
    cycle = 0;
    ++m_label;
    // std::cout << "handle label " << m_label << std::endl;
  }
}

/*! \brief prints out the name of this agent (for debugging purposes) */
void Arrangement_on_sphere_sim_geo::identify()
{
  std::cout << "Agent: Arrangement_on_sphere_sim_geo" << std::endl;
}

/*! Resume the simulation */
void Arrangement_on_sphere_sim_geo::resume(Field_info * /* field_info */)
{
  Tick_event::doregister(this);
}
  
/*! Suspend the simulation */
void Arrangement_on_sphere_sim_geo::suspend(Field_info * /* field_info */)
{
  Tick_event::unregister(this);
} 

/*! Process change of simulation time */
void Arrangement_on_sphere_sim_geo::time_changed(Field_info * /* field_info */)
{ m_label = static_cast<Uint>(m_time * (m_num_labels - 1)); }

/*! Increas the vertex index */
void
Arrangement_on_sphere_sim_geo::increase_vertex_label(Field_info * field_info)
{ ++m_vertex_label; }

/*! Increas the face index */
void
Arrangement_on_sphere_sim_geo::increase_edge_label(Field_info * field_info)
{ ++m_edge_label; }

/*! Increas the face index */
void
Arrangement_on_sphere_sim_geo::increase_face_label(Field_info * field_info)
{ ++m_face_label; }

/*! \brief creates the renderers */
void Arrangement_on_sphere_sim_geo::create_renderers()
{
  m_edges_renderer = new Sphere_sim_edges_renderer(*this);
  m_vertices_renderer = new Sphere_sim_vertices_renderer(*this);
  m_isolated_vertices_renderer =
    new Sphere_sim_isolated_vertices_renderer(*this);
  m_colored_edges_renderer = new Sphere_sim_colored_edges_renderer(*this);
  m_colored_vertices_renderer =
    new Sphere_sim_colored_vertices_renderer(*this);
  m_colored_isolated_vertices_renderer =
    new Sphere_sim_colored_isolated_vertices_renderer(*this);

  m_line_colored_edges_renderer =
    new Sphere_sim_line_colored_edges_renderer(*this);
  m_point_colored_vertices_renderer =
    new Sphere_sim_point_colored_vertices_renderer(*this);
  m_ring_colored_vertices_renderer =
    new Sphere_sim_ring_colored_vertices_renderer(*this);
  m_point_colored_isolated_vertices_renderer =
    new Sphere_sim_point_colored_isolated_vertices_renderer(*this);
  m_ring_colored_isolated_vertices_renderer =
    new Sphere_sim_ring_colored_isolated_vertices_renderer(*this);

  m_inflated_line_edges_renderer =
    new Sphere_sim_inflated_line_edges_renderer(*this);
  m_inflated_strip_edges_renderer =
    new Sphere_sim_inflated_strip_edges_renderer(*this);
  m_inflated_tube_edges_renderer =
    new Sphere_sim_inflated_tube_edges_renderer(*this);
}

/*! \brief destroys the renderers */
void Arrangement_on_sphere_sim_geo::destroy_renderers()
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
}

/*! \brief obtains the arrangement */
Arrangement_on_sphere_labeled * Arrangement_on_sphere_sim_geo::get_aos()
{
  if (m_dirty) clean();
  return m_aos;
}

/*! \brief sets the arrangement */
void Arrangement_on_sphere_sim_geo::set_aos(Arrangement_on_sphere_labeled * aos)
{
  m_dirty = false;
  m_aos = aos;
}
  
SGAL_END_NAMESPACE
