// Copyright (c) 2008 Israel.
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
// Author(s)     : Efi Fogel          <efifogel@gmail.com>;
//                 Boris Kozorovitzky <zbzzn@hotmail.com>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <list>
#include <math.h>

#include "SGAL/Geodesic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/io_vector2f.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Geodesic::s_tag = "Geodesic";
Container_proto* Geodesic::s_prototype(nullptr);

// Default values:
const Float Geodesic::s_def_radius(1.0f);
const Vector2f Geodesic::s_def_start(0.0,0.0);
const Vector2f Geodesic::s_def_end(0.0,0.0);
const Uint Geodesic::s_def_stacks(4);
const Uint Geodesic::s_def_breaks(4);
const Boolean Geodesic::s_def_is_solid(false);
const Boolean Geodesic::s_def_is_complement(false);

REGISTER_TO_FACTORY(Geodesic, "Geodesic");

//! \brief constructor.
Geodesic::Geodesic(Boolean proto) :
  Geometry(proto),
  m_radius(s_def_radius),
  m_start(s_def_start),
  m_end(s_def_end),
  m_stacks(s_def_stacks),
  m_breaks(s_def_breaks),
  m_is_solid(s_def_is_solid),
  m_is_complement(s_def_is_complement)
{}

//! \brief destructor.
Geodesic::~Geodesic(){}

//! \brief cleans the geodesic arc internal representation.
void Geodesic::clean() { /* m_dirty = false; */ }

//! \brief draws the arc.
void Geodesic::draw(Draw_action * action)
{
  // if (!is_dirty()) clean();

  if (has_scale()) glEnable(GL_NORMALIZE);
  Context * context = action->get_context();
  if (!m_is_solid && context) {
    context->draw_cull_face(Gfx::NO_CULL);
    context->draw_light_model_sides(Gfx::TWO_SIDE);
  }

  float x_start = m_radius*cos(m_start[0])*sin(m_start[1]);
  float y_start = m_radius*sin(m_start[0])*sin(m_start[1]);
  float z_start = m_radius*cos(m_start[1]);

  Vector3f source = Vector3f(x_start,y_start,z_start);

  float x_end = m_radius*cos(m_end[0])*sin(m_end[1]);
  float y_end = m_radius*sin(m_end[0])*sin(m_end[1]);
  float z_end = m_radius*cos(m_end[1]);

  Vector3f target = Vector3f(x_end,y_end,z_end);
  std::list<Vector3f> points;
  points.push_front(source);
  points.push_back(target);
  std::list<Vector3f>::iterator iter;
  std::list<Vector3f>::iterator nextIter;
  glBegin(GL_LINE_STRIP);
  Uint i;
  int multiplier = m_is_complement? -1 : 1;

  for (i = 1; i <= m_breaks; ++i) {
    if (i > 1) multiplier = 1;
    iter = points.begin();
    nextIter = iter;
    nextIter++;
    while (nextIter != points.end()) {
      Vector3f vertex1 = *iter;
      Vector3f vertex2 = *nextIter;
      float x = (vertex2[0] + vertex1[0])/2;
      float y = (vertex2[1] + vertex1[1])/2;
      float z = (vertex2[2] + vertex1[2])/2;

      float r = sqrt(x*x+y*y+z*z);
      float theta = atan2(y,x);
      float fi = acos(z/r);

      float x_new = multiplier*m_radius*cos(theta)*sin(fi);
      float y_new = multiplier*m_radius*sin(theta)*sin(fi);
      float z_new = multiplier*m_radius*cos(fi);
      iter = points.insert(nextIter, Vector3f(x_new,y_new,z_new));
      ++iter;
      ++nextIter;
    }
  }
  for (iter = points.begin(); iter!= points.end(); iter++) {
    Vector3f temp = *iter;
    glVertex3fv((float*)&temp);
  }
  glEnd();

  // // Calculate lower left vertex:
  // Vector3f vbl, vtl, vtr, vbr;

  // Uint i, j;
  // float dGamma = m_gamma / m_breaks;
  // float d_delta = m_delta / m_stacks;
  // float alpha = m_alpha;
  // float alpha_right;
  // for (i = 0; i < m_breaks; i++) {
  // float sin_alpha_left, sin_alpha_right;
  // float cos_alpha_left, cos_alpha_right;
  // alpha_right = alpha + dGamma;

  // sincosf(alpha, &sin_alpha_left, &cos_alpha_left);
  // sincosf(alpha_right, &sin_alpha_right, &cos_alpha_right);

  // float beta = m_beta;
  // float beta_top;

  // for (j = 0; j < m_stacks; j++) {
  // float sin_beta_bottom, sin_beta_top;
  // float cos_beta_bottom, cos_beta_top;

  // beta_top = beta + d_delta;

  // sincosf(beta, &sin_beta_bottom, &cos_beta_bottom);
  // sincosf(beta_top, &sin_beta_top, &cos_beta_top);

  // // bottom left:
  // vbl[0] = sin_beta_bottom * sin_alpha_left;
  // vbl[2] = sin_beta_bottom * cos_alpha_left;
  // // vbl[1] = cos_beta_bottom;

  // // top left:
  // vtl[0] = sin_beta_top * sin_alpha_left;
  // vtl[2] = sin_beta_top * cos_alpha_left;
  // // vtl[1] = cos_beta_top;

  // // top right:
  // vtr[0] = sin_beta_top * sin_alpha_right;
  // vtr[2] = sin_beta_top * cos_alpha_right;
  // // vtr[1] = cos_beta_top;

  // // bottom right:
  // vbr[0] = sin_beta_bottom * sin_alpha_right;
  // vbr[2] = sin_beta_bottom * cos_alpha_right;
  // // vbr[1] = cos_beta_bottom;

  // float bottom = m_radius * cos_beta_bottom;
  // float top = m_radius * cos_beta_top;

  // // Render:
  // glBegin(GL_QUADS);
  // glNormal3f(vbl[0], cos_beta_bottom, vbl[2]);
  // glTexCoord2f(0.0f, 0.0f);
  // glVertex3f(m_radius * vbl[0], bottom, m_radius * vbl[2]);

  // glNormal3f(vtl[0], cos_beta_top, vtl[2]);
  // glTexCoord2f(0.0f, 1.0f);
  // glVertex3f(m_radius * vtl[0], top, m_radius * vtl[2]);

  // glNormal3f(vtr[0], cos_beta_top, vtr[2]);
  // glTexCoord2f(1.0f, 1.0f);
  // glVertex3f(m_radius * vtr[0], top, m_radius * vtr[2]);

  // glNormal3f(vbr[0], cos_beta_bottom, vbr[2]);
  // glTexCoord2f(1.0f, 0.0f);
  // glVertex3f(m_radius * vbr[0], bottom, m_radius * vbr[2]);

  // glEnd();

  // beta = beta_top;
  // }
  // alpha = alpha_right;
  // }

  if (!m_is_solid && context) {
    context->draw_cull_face(Gfx::BACK_CULL);
    context->draw_light_model_sides(Gfx::ONE_SIDE);
  }
  if (has_scale()) glDisable(GL_NORMALIZE);
}

//! \brief draws the object in selection mode.
void Geodesic::isect(Isect_action* /* action */)
{
  // // if (!is_dirty()) clean();

  // // Calculate lower left vertex:
  // Vector3f vbl, vtl, vtr, vbr;

  // Uint i, j;
  // float dGamma = m_gamma / m_breaks;
  // float d_delta = m_delta / m_stacks;
  // float alpha = m_alpha;
  // float alpha_right;
  // for (i = 0; i < m_stacks; i++) {
  // float sin_alpha_left, sin_alpha_right;
  // float cos_alpha_left, cos_alpha_right;
  // alpha_right = alpha + dGamma;

  // sincosf(alpha, &sin_alpha_left, &cos_alpha_left);
  // sincosf(alpha_right, &sin_alpha_right, &cos_alpha_right);

  // float beta = m_beta;
  // float beta_top;

  // for (j = 0; j < m_breaks; j++) {
  // float sin_beta_bottom, sin_beta_top;
  // float cos_beta_bottom, cos_beta_top;

  // beta_top = beta + d_delta;

  // sincosf(beta, &sin_beta_bottom, &cos_beta_bottom);
  // sincosf(beta_top, &sin_beta_top, &cos_beta_top);

  // // bottom left:
  // vbl[0] = sin_beta_bottom * sin_alpha_left;
  // vbl[2] = sin_beta_bottom * cos_alpha_left;
  // // vbl[1] = cos_beta_bottom;

  // // top left:
  // vtl[0] = sin_beta_top * sin_alpha_left;
  // vtl[2] = sin_beta_top * cos_alpha_left;
  // // vtl[1] = cos_beta_top;

  // // top right:
  // vtr[0] = sin_beta_top * sin_alpha_right;
  // vtr[2] = sin_beta_top * cos_alpha_right;
  // // vtr[1] = cos_beta_top;

  // // bottom right:
  // vbr[0] = sin_beta_bottom * sin_alpha_right;
  // vbr[2] = sin_beta_bottom * cos_alpha_right;
  // // vbr[1] = cos_beta_bottom;

  // float bottom = m_radius * cos_beta_bottom;
  // float top = m_radius * cos_beta_top;

  // // Render:
  // glBegin(GL_QUADS);
  // glVertex3f(m_radius * vbl[0], bottom, m_radius * vbl[2]);
  // glVertex3f(m_radius * vtl[0], top, m_radius * vtl[2]);
  // glVertex3f(m_radius * vtr[0], top, m_radius * vtr[2]);
  // glVertex3f(m_radius * vbr[0], bottom, m_radius * vbr[2]);
  // glEnd();

  // beta = beta_top;
  // }
  // alpha = alpha_right;
  // }
}

//! \brief cleans the sphere bound of the geodesic.
void Geodesic::clean_bounding_sphere()
{
  m_bounding_sphere.set_radius(m_radius);
  m_bounding_sphere.set_center(Vector3f(0, 0, 0));
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of the object extracted from the input file.
void Geodesic::set_attributes(Element * elem)
{
  Geometry::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "radius") {
      set_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }

    if (name == "start") {
      set_start(boost::lexical_cast<Vector2f>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "end") {
      set_end(boost::lexical_cast<Vector2f>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stacks") {
      set_stacks(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "breaks") {
      set_breaks(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "solid") {
      m_is_solid = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
	if (name == "complement") {
      m_is_complement = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief initializes the geodesic prototype.
void Geodesic::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // radius
  auto exec_func =
    static_cast<Execution_function>(&Geometry::bounding_sphere_changed);
  auto radius_func =
    static_cast<Float_handle_function>(&Geodesic::radius_handle);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           radius_func,
                                           exec_func));

  // start
  auto start_func =
    static_cast<Vector2f_handle_function>(&Geodesic::start_handle);
  s_prototype->add_field_info(new SF_vector2f(START, "start",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              start_func,
                                              exec_func));

  // end
  auto end_func =
    static_cast<Vector2f_handle_function>(&Geodesic::end_handle);
  s_prototype->add_field_info(new SF_vector2f(END, "end",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              end_func,
                                              exec_func));

  // stacks
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto stacks_func =
    static_cast<Uint_handle_function>(&Geodesic::stacks_handle);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          stacks_func,
                                          exec_func));

  // breaks
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto breaks_func =
    static_cast<Uint_handle_function>(&Geodesic::breaks_handle);
  s_prototype->add_field_info(new SF_uint(BREAKS, "breaks",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          breaks_func,
                                          exec_func));

  // complement
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto is_complement_func =
    static_cast<Boolean_handle_function>(&Geodesic::is_complement_handle);
  s_prototype->add_field_info(new SF_bool(COMPLEMENT, "complement",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_complement_func, exec_func));
}

//! \brief deletes the geodesic prototype.
void Geodesic::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the geodesic prototype.
Container_proto* Geodesic::get_prototype()
{
  if (!s_prototype) Geodesic::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
