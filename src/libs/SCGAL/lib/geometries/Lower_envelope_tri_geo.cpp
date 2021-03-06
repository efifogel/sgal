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
 * data structure, where the surfaces are triangles.
 */

#include <CGAL/Cartesian.h>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <time.h>
#include <GL/gl.h>
#include <list>

#include <boost/lexical_cast.hpp>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Sphere.hpp"

#include "SCGAL/Lower_envelope_tri_geo.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Lower_envelope_tri_geo::s_tag = "LowerEnvelopeTri";
Container_proto* Lower_envelope_tri_geo::s_prototype(nullptr);

REGISTER_TO_FACTORY(Lower_envelope_tri_geo, "Lower_envelope_tri_geo");

//! \brief constructor.
Lower_envelope_tri_geo::Lower_envelope_tri_geo(Boolean proto) :
  Lower_envelope_geo(proto),
  m_owned_envelope(false),
  m_envelope(nullptr)
{}

//! \brief destructor.
Lower_envelope_tri_geo::~Lower_envelope_tri_geo()
{ if (m_envelope && m_owned_envelope) delete m_envelope; }

//! \brief cleans the polyhedron data structure.
void Lower_envelope_tri_geo::clean()
{
  clock_t start_time = clock();

  if (!m_envelope) {
    m_envelope = new Envelope_diagram_2;
    SGAL_assertion(m_envelope);
    m_owned_envelope = true;
  }

  std::list<Triangle_3> triangles;
  get_triangles(std::back_inserter(triangles));
  CGAL::lower_envelope_3(triangles.begin(), triangles.end(), *m_envelope);

  clock_t end_time = clock();
  m_time = (float) (end_time - start_time) / (float) CLOCKS_PER_SEC;
  Lower_envelope_geo::clean();
}

//! \brief clears the internal representation.
void Lower_envelope_tri_geo::clear()
{
  Lower_envelope_geo::clear();
  if (m_envelope) m_envelope->clear();
}

//! \brief sets the attributes of the object extracted from an input file.
void Lower_envelope_tri_geo::set_attributes(SGAL::Element* elem)
{ Lower_envelope_geo::set_attributes(elem); }

//! \brief initializes the prototype of this container.
void Lower_envelope_tri_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Lower_envelope_geo::get_prototype());
}

//! \brief deletes the prototype of this container.
void Lower_envelope_tri_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype of this container.
Container_proto* Lower_envelope_tri_geo::get_prototype()
{
  if (!s_prototype) Lower_envelope_tri_geo::init_prototype();
  return s_prototype;
}

//! \brief draws the envelope faces.
void Lower_envelope_tri_geo::draw_envelope_faces(Draw_action* action)
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
        const Vector2f& vec = to_vector2f(curr->source()->point());
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
        const Vector2f& vec = to_vector2f(curr->source()->point());
        glVertex2fv((float*)&vec);
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

//! \brief draws the envelope edges.
void Lower_envelope_tri_geo::draw_envelope_edges(Draw_action* action)
{
  Envelope_diagram_2::Edge_const_iterator eit;
  for (eit = m_envelope->edges_begin(); eit != m_envelope->edges_end(); ++eit) {
    Vector2f src = to_vector2f(eit->source()->point());
    Vector2f trg = to_vector2f(eit->target()->point());

    Extrusion tube;
    tube.set_cross_section_radius(m_edge_radius);
    std::vector<Vector3f>& spine = tube.get_spine();
    spine.resize(2);
    spine[0].set(src[0], src[1], 0);
    spine[1].set(trg[0], trg[1], 0);
    tube.draw(action);
  }
}

//! \brief draws the envelope vertices.
void Lower_envelope_tri_geo::draw_envelope_vertices(Draw_action* action)
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
