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
// $Id: Lower_envelope_plane_geo.cpp 7681 2009-06-08 16:52:09Z ophirset $
// $Revision: 7681 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure, where the surfaces are planes.
 */

#include <CGAL/Cartesian.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>

#include <boost/lexical_cast.hpp>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Sphere.hpp"

#include "SCGAL/Lower_envelope_plane_geo.hpp"

SGAL_BEGIN_NAMESPACE

std::string Lower_envelope_plane_geo::s_tag = "LowerEnvelopePlane";
Container_proto * Lower_envelope_plane_geo::s_prototype = NULL;

bool Lower_envelope_plane_geo::s_def_draw_patches(SGAL_TRUE);

REGISTER_TO_FACTORY(Lower_envelope_plane_geo, "Lower_envelope_plane_geo");

/*! Constructor */
Lower_envelope_plane_geo::Lower_envelope_plane_geo(Boolean proto) :
  Lower_envelope_geo(proto),
  m_own_envelope(SGAL_FALSE),
  m_envelope(NULL),
  m_plane_array(NULL),
  m_color_array(NULL),
  m_own_bounding_polygon(SGAL_FALSE),
  m_bounding_polygon(NULL),
  m_draw_patches(s_def_draw_patches)
{}

/*! Destructor */
Lower_envelope_plane_geo::~Lower_envelope_plane_geo()
{ if (m_envelope && m_own_envelope) delete m_envelope; }

/*! \brief cleans the polyhedron data structure */
void Lower_envelope_plane_geo::clean()
{
  clock_t start_time = clock();

  if (!m_envelope) {
    m_envelope = new Envelope_diagram_2;
    m_own_envelope = SGAL_TRUE;
  }

  std::list<Plane_3> planes;
  get_planes(std::back_inserter(planes));
  CGAL::lower_envelope_3(planes.begin(), planes.end(), *m_envelope);

  // Insert the bounding polygon:
  //! \todo compute a bouding squre if none is provided
  // if (!m_bounding_polygon) {
  //}
  My_observer obs (*m_envelope);

  Exact_coord2_array::Exact_point_const_iter it = m_bounding_polygon->begin();
  const Env_plane_traits_3::Point_2 & start_point = *it;
  const Env_plane_traits_3::Point_2 * prev_point = &(*it);
  for (++it; it != m_bounding_polygon->end(); ++it) {
    const Env_plane_traits_3::Point_2 & point = *it;
    Env_plane_traits_3::X_monotone_curve_2 curve(*prev_point, point);
    CGAL::insert(*m_envelope, curve);
    prev_point = &(*it);
  }
  Env_plane_traits_3::X_monotone_curve_2 curve(*prev_point, start_point);
  CGAL::insert(*m_envelope, curve);

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  Lower_envelope_geo::clean();
}

/*! \brief clears the internal representation */
void Lower_envelope_plane_geo::clear()
{
  Lower_envelope_geo::clear();
  if (m_envelope) m_envelope->clear();
}

/*! \brief sets the attributes of the object extracted from an input file */
void Lower_envelope_plane_geo::set_attributes(SGAL::Element * elem)
{
  Lower_envelope_geo::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "drawPatches") {
      m_draw_patches = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }
  
  typedef Element::Cont_attr_iter       Cont_attr_iter;
  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "plane") {
      Exact_plane_array * plane_array = dynamic_cast<Exact_plane_array*>(cont);
      set_plane_array(plane_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "color") {
      Color_array * color_array = dynamic_cast<Color_array*>(cont);
      set_color_array(color_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "boundingPolygon") {
      Exact_coord2_array * bounding_polygon =
        dynamic_cast<Exact_coord2_array*>(cont);
      set_bounding_polygon(bounding_polygon);
      elem->mark_delete(cai);
      continue;
    }
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief initializes the prototype of this container. */
void Lower_envelope_plane_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Lower_envelope_geo::get_prototype());
}

/*! \brief deletes the prototype of this container */
void Lower_envelope_plane_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype of this container */
Container_proto * Lower_envelope_plane_geo::get_prototype() 
{  
  if (!s_prototype) Lower_envelope_plane_geo::init_prototype();
  return s_prototype;
}

/*! \brief draws the envelope faces */
void Lower_envelope_plane_geo::draw_envelope_faces(Draw_action * action)
{
  Context * context = action->get_context();
  if (m_color_array) {
    context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
  }
  context->draw_cull_face(Gfx::NO_CULL);
  context->draw_transp_enable(true);
  
  Envelope_diagram_2::Face_const_iterator fit;
  for (fit = m_envelope->faces_begin(); fit != m_envelope->faces_end(); ++fit) {
    SGAL_assertion(fit->number_of_surfaces() > 0);
    if (fit->is_unbounded()) continue;

    if (m_color_array) {
      Vector3f color;
      Envelope_diagram_2::Surface_const_iterator sit;
      for (sit = fit->surfaces_begin(); sit != fit->surfaces_end(); ++sit) {
        Uint index = sit->data();
        const Vector3f & surf_color = (*m_color_array)[index];
        color.add(surf_color);
      }
      color.scale(1.0f / fit->number_of_surfaces());
      glColor4f(color[0], color[1], color[2], m_face_transparency);
    }

    Envelope_diagram_2::Outer_ccb_const_iterator oit;
    for (oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end(); ++oit) {
      Envelope_diagram_2::Halfedge_const_iterator first = *oit;
      Envelope_diagram_2::Halfedge_const_iterator curr = first;
      glBegin(GL_POLYGON);
      do {
        const Vector2f & vec = to_vector2f(curr->source()->point());
        glVertex2fv((float*)&vec);
        curr = curr->next();
      } while (curr != first);
      glEnd();
    }

    Envelope_diagram_2::Inner_ccb_const_iterator iit;
    for (iit = fit->inner_ccbs_begin(); iit != fit->inner_ccbs_end(); ++iit) {
      Envelope_diagram_2::Halfedge_const_iterator first = *iit;
      Envelope_diagram_2::Halfedge_const_iterator curr = first;
      glBegin(GL_POLYGON);
      do {
        const Vector2f & vec = to_vector2f(curr->source()->point());
        glVertex2fv((float*)&vec);
        curr = curr->next();
      } while (curr != first);
      glEnd();
    }
  }
  context->draw_transp_enable(false);
  context->draw_cull_face(Gfx::BACK_CULL);
  if (m_color_array) {
    context->draw_light_model_sides(Gfx::ONE_SIDE);
    context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
  }
}

/*! \brief draws the envelope edges */
void Lower_envelope_plane_geo::draw_envelope_edges(Draw_action * action)
{
  Envelope_diagram_2::Edge_const_iterator eit;
  for (eit = m_envelope->edges_begin(); eit != m_envelope->edges_end(); ++eit) {
    if (eit->source()->is_at_open_boundary() ||
        eit->target()->is_at_open_boundary())
      continue;
    Vector2f src = to_vector2f(eit->source()->point());
    Vector2f trg = to_vector2f(eit->target()->point());

    Extrusion tube;
    tube.set_cross_section_radius(m_edge_radius);
    SGAL::Array<Vector3f> & spine = tube.get_spine();
    spine.resize(2);
    spine[0].set(src[0], src[1], 0);
    spine[1].set(trg[0], trg[1], 0);
    tube.draw(action);
  }
}

/*! \brief draws the envelope vertices */
void Lower_envelope_plane_geo::draw_envelope_vertices(Draw_action * action)
{
  Envelope_diagram_2::Vertex_const_iterator vit;
  for (vit = m_envelope->vertices_begin(); vit != m_envelope->vertices_end();
       ++vit)
  {
    const Vector2f & p = to_vector2f(vit->point());
    Vector3f center(p[0], p[1], 0);
    Sphere sphere;
    sphere.set_radius(m_vertex_radius);
    sphere.set_center(center);
    sphere.set_stacks(64);
    sphere.set_slices(64);
    sphere.draw(action);
  }
}

/*! \brief draws the geometry of the lower envelope */
void Lower_envelope_plane_geo::draw(Draw_action * action)
{
  if (m_draw_patches) draw_patches(action);
  Lower_envelope_geo::draw(action);
}

/*! \brief draws the patches of the plane that induce the envelope */
void Lower_envelope_plane_geo::draw_patches(Draw_action * action)
{
  Context * context = action->get_context();
  if (m_color_array) {
    context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
    context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
  }
  context->draw_cull_face(Gfx::NO_CULL);
  
  Envelope_diagram_2::Face_const_iterator fit;
  for (fit = m_envelope->faces_begin(); fit != m_envelope->faces_end(); ++fit) {
    SGAL_assertion(fit->number_of_surfaces() > 0);
    if (fit->is_unbounded()) continue;
    const Base_plane_3 & env_plane = fit->surface();
    const Exact_plane_3 & plane = env_plane.plane();
    const Exact_number_type & a = plane.a();
    const Exact_number_type & b = plane.b();
    const Exact_number_type & c = plane.c();
    const Exact_number_type & d = plane.d();
    if (m_color_array) {
      Vector3f color;
      Envelope_diagram_2::Surface_const_iterator sit;
      for (sit = fit->surfaces_begin(); sit != fit->surfaces_end(); ++sit) {
        Uint index = sit->data();
        const Vector3f & surf_color = (*m_color_array)[index];
        color.add(surf_color);
      }
      color.scale(1.0f / fit->number_of_surfaces());
      glColor3fv((float*)&color);
    }

    Envelope_diagram_2::Outer_ccb_const_iterator oit;
    for (oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end(); ++oit) {
      Envelope_diagram_2::Halfedge_const_iterator first = *oit;
      Envelope_diagram_2::Halfedge_const_iterator curr = first;
      glBegin(GL_POLYGON);
      do {
        const Env_plane_traits_3::Point_2 & point2 = curr->source()->point();
        const Exact_number_type & x = point2.x();
        const Exact_number_type & y = point2.y();
        Exact_number_type z = (-a * x - b * y - d) / c;
        Exact_point_3 point3(x, y, z);
        Vector3f vec = to_vector3f(point3);
        glVertex3fv((float*)&vec);
        curr = curr->next();
      } while (curr != first);
      glEnd();
    }

    Envelope_diagram_2::Inner_ccb_const_iterator iit;
    for (iit = fit->inner_ccbs_begin(); iit != fit->inner_ccbs_end(); ++iit) {
      Envelope_diagram_2::Halfedge_const_iterator first = *iit;
      Envelope_diagram_2::Halfedge_const_iterator curr = first;
      glBegin(GL_POLYGON);
      do {
        const Env_plane_traits_3::Point_2 & point2 = curr->source()->point();
        const Exact_number_type & x = point2.x();
        const Exact_number_type & y = point2.y();
        Exact_number_type z = (-a * x - b * y - d) / c;
        Exact_point_3 point3(x, y, z);
        Vector3f vec = to_vector3f(point3);
        glVertex3fv((float*)&vec);
        curr = curr->next();
      } while (curr != first);
      glEnd();
    }
  }
  context->draw_cull_face(Gfx::BACK_CULL);
  if (m_color_array) {
    context->draw_light_model_sides(Gfx::ONE_SIDE);
    context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
  }
}

SGAL_END_NAMESPACE
