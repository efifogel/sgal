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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/Arc.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Arc::s_tag = "Arc";
Container_proto* Arc::s_prototype(nullptr);

// Default values:
const Float Arc::s_def_radius(1.0f);
const Uint Arc::s_def_stacks(4);
const Uint Arc::s_def_slices(4);
const Float Arc::s_def_alpha(0.0f);
const Float Arc::s_def_beta(0.0f);
const Float Arc::s_def_gamma(SGAL_TWO_PI);
const Float Arc::s_def_delta(SGAL_PI);
const Boolean Arc::s_def_is_solid(false);

//! Halftone stipple pattern for backfacing elements.
Ubyte Arc::s_halftone[] = {
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
  0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55
};

REGISTER_TO_FACTORY(Arc, "Arc");

//! \brief constructor.
Arc::Arc(Boolean proto) :
  Geometry(proto),
  m_radius(s_def_radius),
  m_stacks(s_def_stacks),
  m_slices(s_def_slices),
  m_alpha(s_def_alpha),
  m_beta(s_def_beta),
  m_gamma(s_def_gamma),
  m_delta(s_def_delta),
/*
alpha 2.0 #arc start position in radians
beta 1.0 #Arc height on the main axis in radians
gamma 3.14 #Arc length in radians
delta 0.01 #thickness
*/
  m_is_solid(s_def_is_solid)
{}

//! \brief destructor.
Arc::~Arc() {}

//! \brief draws the arc.
void Arc::draw(Draw_action* action)
{
  Context* context = action->get_context();
  if (!context) return;

  Uint pass_no = action->get_pass_no();
  if (pass_no == 0) {
    action->set_second_pass_required(true);

    // Backfacing:
    context->draw_cull_face(Gfx::FRONT_CULL);
    context->draw_light_model_sides(Gfx::TWO_SIDE);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(s_halftone);
  }

  if (has_scale()) glEnable(GL_NORMALIZE);
//   if (!m_is_solid) {
//     context->draw_cull_face(Gfx::NO_CULL);
//     context->draw_light_model_sides(Gfx::TWO_SIDE);
//   }

  // Calculate lower left vertex:
  Vector3f vbl, vtl, vtr, vbr;

  Uint i, j;
  float dGamma = m_gamma / m_slices;
  float d_delta = m_delta / m_stacks;
  float alpha = m_alpha;
  float alpha_right;
  for (i = 0; i < m_slices; i++) {
    float sin_alpha_left, sin_alpha_right;
    float cos_alpha_left, cos_alpha_right;
    alpha_right = alpha + dGamma;

    sincosf(alpha, &sin_alpha_left, &cos_alpha_left);
    sincosf(alpha_right, &sin_alpha_right, &cos_alpha_right);

    float beta = m_beta;
    float beta_top;

    for (j = 0; j < m_stacks; j++) {
      float sin_beta_bottom, sin_beta_top;
      float cos_beta_bottom, cos_beta_top;

      beta_top = beta + d_delta;

      sincosf(beta, &sin_beta_bottom, &cos_beta_bottom);
      sincosf(beta_top, &sin_beta_top, &cos_beta_top);

      // bottom left:
      vbl[0] = sin_beta_bottom * sin_alpha_left;
      vbl[2] = sin_beta_bottom * cos_alpha_left;
      // vbl[1] = cos_beta_bottom;

      // top left:
      vtl[0] = sin_beta_top * sin_alpha_left;
      vtl[2] = sin_beta_top * cos_alpha_left;
      // vtl[1] = cos_beta_top;

      // top right:
      vtr[0] = sin_beta_top * sin_alpha_right;
      vtr[2] = sin_beta_top * cos_alpha_right;
      // vtr[1] = cos_beta_top;

      // bottom right:
      vbr[0] = sin_beta_bottom * sin_alpha_right;
      vbr[2] = sin_beta_bottom * cos_alpha_right;
      // vbr[1] = cos_beta_bottom;

      float bottom = m_radius * cos_beta_bottom;
      float top = m_radius * cos_beta_top;

      // Render:
      glBegin(GL_QUADS);
      glNormal3f(vbl[0], cos_beta_bottom, vbl[2]);
      if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f);
      glVertex3f(m_radius * vbl[0], bottom, m_radius * vbl[2]);

      glNormal3f(vtl[0], cos_beta_top, vtl[2]);
      if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f);
      glVertex3f(m_radius * vtl[0], top, m_radius * vtl[2]);

      glNormal3f(vtr[0], cos_beta_top, vtr[2]);
      if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f);
      glVertex3f(m_radius * vtr[0], top, m_radius * vtr[2]);

      glNormal3f(vbr[0], cos_beta_bottom, vbr[2]);
      if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f);
      glVertex3f(m_radius * vbr[0], bottom, m_radius * vbr[2]);

      glEnd();

      beta = beta_top;
    }
    alpha = alpha_right;
  }

//   if (!m_is_solid) {
//     context->draw_cull_face(Gfx::BACK_CULL);
//     context->draw_light_model_sides(Gfx::ONE_SIDE);
//   }
  if (has_scale()) glDisable(GL_NORMALIZE);

  if (pass_no == 0) {
    context->draw_light_model_sides(Gfx::ONE_SIDE);
    context->draw_cull_face(Gfx::BACK_CULL);
    glDisable(GL_POLYGON_STIPPLE);
  }
  // context->draw_depth_func(Gfx::LESS_DFUNC);
}

//! \brief draws the object in selection mode.
void Arc::isect(Isect_action* /* action */)
{
  // Calculate lower left vertex:
  Vector3f vbl, vtl, vtr, vbr;

  Uint i, j;
  float dGamma = m_gamma / m_slices;
  float d_delta = m_delta / m_stacks;
  float alpha = m_alpha;
  float alpha_right;
  for (i = 0; i < m_stacks; i++) {
    float sin_alpha_left, sin_alpha_right;
    float cos_alpha_left, cos_alpha_right;
    alpha_right = alpha + dGamma;

    sincosf(alpha, &sin_alpha_left, &cos_alpha_left);
    sincosf(alpha_right, &sin_alpha_right, &cos_alpha_right);

    float beta = m_beta;
    float beta_top;

    for (j = 0; j < m_slices; j++) {
      float sin_beta_bottom, sin_beta_top;
      float cos_beta_bottom, cos_beta_top;

      beta_top = beta + d_delta;

      sincosf(beta, &sin_beta_bottom, &cos_beta_bottom);
      sincosf(beta_top, &sin_beta_top, &cos_beta_top);

      // bottom left:
      vbl[0] = sin_beta_bottom * sin_alpha_left;
      vbl[2] = sin_beta_bottom * cos_alpha_left;
      // vbl[1] = cos_beta_bottom;

      // top left:
      vtl[0] = sin_beta_top * sin_alpha_left;
      vtl[2] = sin_beta_top * cos_alpha_left;
      // vtl[1] = cos_beta_top;

      // top right:
      vtr[0] = sin_beta_top * sin_alpha_right;
      vtr[2] = sin_beta_top * cos_alpha_right;
      // vtr[1] = cos_beta_top;

      // bottom right:
      vbr[0] = sin_beta_bottom * sin_alpha_right;
      vbr[2] = sin_beta_bottom * cos_alpha_right;
      // vbr[1] = cos_beta_bottom;

      float bottom = m_radius * cos_beta_bottom;
      float top = m_radius * cos_beta_top;

      // Render:
      glBegin(GL_QUADS);
      glVertex3f(m_radius * vbl[0], bottom, m_radius * vbl[2]);
      glVertex3f(m_radius * vtl[0], top, m_radius * vtl[2]);
      glVertex3f(m_radius * vtr[0], top, m_radius * vtr[2]);
      glVertex3f(m_radius * vbr[0], bottom, m_radius * vbr[2]);
      glEnd();

      beta = beta_top;
    }
    alpha = alpha_right;
  }
}

//! \brief cleans the sphere bound of the arc.
void Arc::clean_sphere_bound()
{
  m_sphere_bound.set_radius(m_radius);
  m_sphere_bound.set_center(Vector3f(0, 0, 0));
  m_dirty_sphere_bound = false;
}

//! \brief sets the attributes of this container.
void Arc::set_attributes(Element* elem)
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
    if (name == "stacks") {
      set_stacks(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "slices") {
      set_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "alpha") {
      set_alpha(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "beta") {
      set_beta(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "gamma") {
      set_gamma(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "delta") {
      set_delta(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "solid") {
      m_is_solid = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! \brief obtains a list of attributes for the container.
 * The list is of name=value pairs.
 * \return a list of pairs of strings
 */
Attribute_list Arc::get_attributes()
{
  Attribute_list attribs;
  // attribs = Geometry::get_attributes();
  Attribue attrib;
  char buf[32];

  attribs = Geometry::get_attributes();

  if (m_radius != s_def_radius) {
    attrib.first = "radius";
    sprintf(buf, "%g", get_radius());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_slices != s_def_slices) {
    attrib.first = "slices";
    sprintf(buf, "%d", get_slices());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_stacks != s_def_stacks) {
    attrib.first = "stacks";
    sprintf(buf, "%d", get_stacks());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_alpha != s_def_alpha) {
    attrib.first = "alpha";
    sprintf(buf, "%f", get_alpha());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_beta != s_def_beta) {
    attrib.first = "beta";
    sprintf(buf, "%f", get_beta());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_gamma != s_def_gamma) {
    attrib.first = "gamma";
    sprintf(buf, "%f", get_gamma());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_delta != s_def_delta) {
    attrib.first = "delta";
    sprintf(buf, "%f", get_delta());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief initializes the arc prototype.
void Arc::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // radius
  Execution_function exec_func =
    static_cast<Execution_function>(&Geometry::sphere_bound_changed);
  Float_handle_function radius_func =
    static_cast<Float_handle_function>(&Arc::radius_handle);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           RULE_EXPOSED_FIELD,
                                           radius_func,
                                           exec_func));

  // stacks
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Uint_handle_function stacks_func =
    static_cast<Uint_handle_function>(&Arc::stacks_handle);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          RULE_EXPOSED_FIELD,
                                          stacks_func,
                                          exec_func));

  // slices
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Uint_handle_function slices_func =
    static_cast<Uint_handle_function>(&Arc::slices_handle);
  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          RULE_EXPOSED_FIELD,
                                          slices_func,
                                          exec_func));

  // alpha
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function alpha_func =
    static_cast<Float_handle_function>(&Arc::alpha_handle);
  s_prototype->add_field_info(new SF_float(ALPHA, "alpha",
                                           RULE_EXPOSED_FIELD,
                                           alpha_func,
                                           exec_func));

  // beta
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function beta_func =
    static_cast<Float_handle_function>(&Arc::beta_handle);
  s_prototype->add_field_info(new SF_float(BETA, "beta",
                                           RULE_EXPOSED_FIELD,
                                           beta_func,
                                           exec_func));

  // gamma
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function gamma_func =
    static_cast<Float_handle_function>(&Arc::gamma_handle);
  s_prototype->add_field_info(new SF_float(GAMMA, "gamma",
                                           RULE_EXPOSED_FIELD,
                                           gamma_func,
                                           exec_func));

  // delta
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function delta_func =
    static_cast<Float_handle_function>(&Arc::delta_handle);
  s_prototype->add_field_info(new SF_float(DELTA, "delta",
                                           RULE_EXPOSED_FIELD,
                                           delta_func,
                                           exec_func));
}

//! \brief deletes the arc prototype.
void Arc::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the arc prototype.
Container_proto* Arc::get_prototype()
{
  if (!s_prototype) Arc::init_prototype();
  return s_prototype;
}

//! \brief sets the arc radius.
void Arc::set_radius(Float radius)
{
  m_radius = radius;
  m_dirty_sphere_bound = true;
}

SGAL_END_NAMESPACE
