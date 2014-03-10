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

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure, where the surfaces are sphere.
 */

#if defined(_WIN32)
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <list>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>

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

#include "SCGAL/Lower_envelope_sphere_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Lower_envelope_sphere_geo::s_tag = "LowerEnvelopeSphere";
Container_proto* Lower_envelope_sphere_geo::s_prototype(NULL);

Uint Lower_envelope_sphere_geo::s_def_resolution(32);

REGISTER_TO_FACTORY(Lower_envelope_sphere_geo, "Lower_envelope_sphere_geo");

/*! Constructor. */
Lower_envelope_sphere_geo::Lower_envelope_sphere_geo(Boolean proto) :
  Lower_envelope_geo(proto),
  m_owned_envelope(false),
  m_envelope(NULL),
  m_resolution(s_def_resolution)
{}

/*! Destructor. */
Lower_envelope_sphere_geo::~Lower_envelope_sphere_geo()
{ if (m_envelope && m_owned_envelope) delete m_envelope; }

/*! \brief cleans the polyhedron data structure. */
void Lower_envelope_sphere_geo::clean()
{
  clock_t start_time = clock();

  if (!m_envelope) {
    m_envelope = new Envelope_diagram_2;
    SGAL_assertion(m_envelope);
    m_owned_envelope = true;
  }

  std::list<Sphere_3> spheres;
  get_spheres(std::back_inserter(spheres));
  CGAL::lower_envelope_3(spheres.begin(), spheres.end(), *m_envelope);

  // Subdivide the curves into piecewise linear segments:
  Envelope_diagram_2::Edge_iterator eit;
  for (eit = m_envelope->edges_begin(); eit != m_envelope->edges_end(); ++eit) {
    if (eit->get_points()) continue;
    Double_point_list* points = new Double_point_list;
    eit->curve().polyline_approximation(m_resolution,
                                        std::back_inserter(*points));
    eit->set_points(points);
    eit->twin()->set_points(points);
  }

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  Lower_envelope_geo::clean();
}

/*! \brief clears the internal representation. */
void Lower_envelope_sphere_geo::clear()
{
  Lower_envelope_geo::clear();
  if (m_envelope) m_envelope->clear();
}

/*! \brief sets the attributes of the object extracted from an input file. */
void Lower_envelope_sphere_geo::set_attributes(SGAL::Element* elem)
{ Lower_envelope_geo::set_attributes(elem); }

/*! \brief initializes the prototype of this container. */
void Lower_envelope_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Lower_envelope_geo::get_prototype());
}

/*! \brief deletes the prototype of this container. */
void Lower_envelope_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype of this container. */
Container_proto* Lower_envelope_sphere_geo::get_prototype()
{
  if (!s_prototype) Lower_envelope_sphere_geo::init_prototype();
  return s_prototype;
}

/*! \brief draws the envelope faces. */
void Lower_envelope_sphere_geo::draw_envelope_faces(Draw_action* action)
{
  Context* context = action->get_context();
  context->draw_material_mode_enable(SGAL::Gfx::COLOR_MATERIAL);
  context->draw_light_model_sides(SGAL::Gfx::TWO_SIDE);
  context->draw_cull_face(Gfx::NO_CULL);
  context->draw_transp_enable(true);

  Envelope_diagram_2::Face_const_iterator fit;
  for (fit = m_envelope->faces_begin(); fit != m_envelope->faces_end(); ++fit) {
    if (fit->number_of_surfaces() == 0) continue;

    Vector3f color;
    Envelope_diagram_2::Surface_const_iterator sit;
    for (sit = fit->surfaces_begin(); sit != fit->surfaces_end(); ++sit) {
      Shared_appearance surf_app = sit->data();
      boost::shared_ptr<Material> surf_mat = surf_app->get_material();
      const Vector3f& surf_color = surf_mat->get_diffuse_color();
      color.add(surf_color);
    }
    color.scale(1.0f / std::distance(fit->surfaces_begin(),
                                     fit->surfaces_end()));
    glColor4f(color[0], color[1], color[2], m_face_transparency);

    Envelope_diagram_2::Outer_ccb_const_iterator oit;
    for (oit = fit->outer_ccbs_begin(); oit != fit->outer_ccbs_end(); ++oit) {
      Envelope_diagram_2::Halfedge_const_iterator first = *oit;
      Envelope_diagram_2::Halfedge_const_iterator curr = first;
      glBegin(GL_POLYGON);
      do {
        const Rational& r = curr->curve().r();
        const Rational& s = curr->curve().s();
        if ((r == 0) && (s == 0)) {
          // Segment:
          const Vector2f& p = to_vector2f(curr->source()->point());
          Vector2f vec(p[0], p[1]);
          glVertex2fv((float*)&vec);
        } else {
          const Double_point_list* points = curr->get_points();
          SGAL_assertion(points);
          if (curr->direction() == CGAL::ARR_LEFT_TO_RIGHT) {
            Double_point_list::const_iterator it_next = points->begin();
            Double_point_list::const_iterator it = it_next++;
            for (; it_next != points->end(); it = it_next++)
              glVertex2d((*it).first, (*it).second);
          } else {
            Double_point_list::const_reverse_iterator it_next =
              points->rbegin();
            Double_point_list::const_reverse_iterator it = it_next++;
            for (; it_next != points->rend(); it = it_next++)
              glVertex2d((*it).first, (*it).second);
          }
        }
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
        const Rational& r = curr->curve().r();
        const Rational& s = curr->curve().s();
        if ((r == 0) && (s == 0)) {
          // Segment:
          const Vector2f& p = to_vector2f(curr->source()->point());
          glVertex2f(p[0], p[1]);
        } else {
          const Double_point_list* points = curr->get_points();
          SGAL_assertion(points);
          if (curr->direction() == CGAL::ARR_LEFT_TO_RIGHT) {
            Double_point_list::const_iterator it_next = points->begin();
            Double_point_list::const_iterator it = it_next++;
            for (; it_next != points->end(); it = it_next++)
              glVertex2d((*it).first, (*it).second);
          } else {
            Double_point_list::const_reverse_iterator it_next =
              points->rbegin();
            Double_point_list::const_reverse_iterator it = it_next++;
            for (; it_next != points->rend(); it = it_next++)
              glVertex2d((*it).first, (*it).second);
          }
        }
        curr = curr->next();
      } while (curr != first);
      glEnd();
    }
  }

  context->draw_transp_enable(false);
  context->draw_cull_face(Gfx::BACK_CULL);
  context->draw_light_model_sides(Gfx::ONE_SIDE);
  context->draw_material_mode_enable(SGAL::Gfx::NO_COLOR_MATERIAL);
}

/*! \brief draws the envelope edges. */
void Lower_envelope_sphere_geo::draw_envelope_edges(Draw_action* action)
{
  Envelope_diagram_2::Edge_const_iterator eit;
  for (eit = m_envelope->edges_begin(); eit != m_envelope->edges_end(); ++eit) {
    const Rational& r = eit->curve().r();
    const Rational& s = eit->curve().s();

    SGAL_assertion((eit->curve().t() == 0) && (r == s));
    Extrusion tube;
    std::vector<Vector3f>& spine = tube.get_spine();
    tube.set_cross_section_radius(m_edge_radius);
    if ((r == 0) && (s == 0)) {
      // Segment:
      spine.resize(2);
      Vector2f src = to_vector2f(eit->curve().source());
      Vector2f trg = to_vector2f(eit->curve().target());
      spine[0].set(src[0], src[1], 0);
      spine[1].set(trg[0], trg[1], 0);
      tube.draw(action);
      continue;
    }
    const Double_point_list* points = eit->get_points();
    SGAL_assertion(points);
    spine.resize(points->size());
    Double_point_list::const_iterator it;
    Uint i = 0;
    for (it = points->begin(); it != points->end(); ++it)
      spine[i++].set((*it).first, (*it).second, 0);
    tube.draw(action);
  }
}

/*! \brief draws the envelope vertices. */
void Lower_envelope_sphere_geo::draw_envelope_vertices(Draw_action* action)
{
  Envelope_diagram_2::Vertex_const_iterator vit;
  for (vit = m_envelope->vertices_begin(); vit != m_envelope->vertices_end();
       ++vit)
  {
    const Vector2f& p = to_vector2f(vit->point());
    Vector3f center(p[0], p[1], 0);
    Sphere sphere;
    sphere.set_radius(m_vertex_radius);
    sphere.set_center(center);
    sphere.set_stacks(64);
    sphere.set_slices(64);
    sphere.draw(action);
  }
}

SGAL_END_NAMESPACE
