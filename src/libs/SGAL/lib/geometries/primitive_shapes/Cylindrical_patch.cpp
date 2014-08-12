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
 * A geometry container that represents a cylindrical patch. Such a patch
 * is defined by the radius and height of the underlying cylinder and by
 * an angle of the horizontal cross section.
 */

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/Cylindrical_patch.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Cylindrical_patch::s_tag = "CylindricalPatch";
Container_proto* Cylindrical_patch::s_prototype(nullptr);

// default values:
const Float Cylindrical_patch::s_def_radius(1);
const Float Cylindrical_patch::s_def_height(2);
const Uint  Cylindrical_patch::s_def_slices(32);
const Float  Cylindrical_patch::s_def_alpha(0);
const Float  Cylindrical_patch::s_def_beta(SGAL_TWO_PI);

REGISTER_TO_FACTORY(Cylindrical_patch, "Cylindrical_patch");

//! \brief constructor.
Cylindrical_patch::Cylindrical_patch(Boolean proto) :
  Geometry(proto),
  m_radius(s_def_radius),
  m_height(s_def_height),
  m_alpha(s_def_alpha),
  m_beta(s_def_beta),
  m_slices(s_def_slices)
{}

//! \brief destructor.
Cylindrical_patch::~Cylindrical_patch(){}

//! \brief draws a single quadrilateral.
void Cylindrical_patch::draw_quad(Float cos_left, Float sin_left,
                                  Float cos_right, Float sin_right)
{
  Float v_left_x = m_radius * cos_left;
  Float v_left_z = -m_radius * sin_left;
  Float n_left_x = cos_left;
  Float n_left_z = -sin_left;
  Float top = m_height / 2;
  Float bottom = -top;

  Float v_right_x = m_radius * cos_right;
  Float v_right_z = -m_radius * sin_right;
  Float n_right_x = cos_right;
  Float n_right_z = -sin_right;

  glNormal3f(n_left_x, 0, n_left_z);
  if (do_generate_tex_coord()) glTexCoord2f(0, 0);
  glVertex3f(v_left_x, bottom, v_left_z);

  glNormal3f(n_right_x, 0, n_right_z);
  if (do_generate_tex_coord()) glTexCoord2f(0, 1);
  glVertex3f(v_right_x, bottom, v_right_z);

  glNormal3f(n_right_x, 0, n_right_z);
  if (do_generate_tex_coord()) glTexCoord2f(1, 1);
  glVertex3f(v_right_x, top, v_right_z);

  glNormal3f(n_left_x, 0, n_left_z);
  if (do_generate_tex_coord()) glTexCoord2f(1, 0);
  glVertex3f(v_left_x, top, v_left_z);
}

/*! \brief draws the cylindrical patch. */
void Cylindrical_patch::draw(Draw_action* action)
{
  if (has_scale()) glEnable(GL_NORMALIZE);

  Context* context = action->get_context();
  if (context) {
    context->draw_cull_face(Gfx::NO_CULL);
    context->draw_light_model_sides(Gfx::TWO_SIDE);
  }

  glBegin(GL_QUADS);
  unsigned int i;
  Float cos_left, sin_left, cos_right, sin_right;
  sincosf(m_alpha, &sin_left, &cos_left);
  Float delta = (m_beta - m_alpha) / m_slices;
  for (i = 1; i < m_slices; ++i) {
    Float acc_angle = m_alpha + delta * i;
    sincosf(acc_angle, &sin_right, &cos_right);
    draw_quad(cos_left, sin_left, cos_right, sin_right);
    cos_left = cos_right;
    sin_left = sin_right;
  }
  sincosf(m_beta, &sin_right, &cos_right);
  draw_quad(cos_left, sin_left, cos_right, sin_right);
  glEnd();

  if (context) {
    context->draw_cull_face(Gfx::BACK_CULL);
    context->draw_light_model_sides(Gfx::ONE_SIDE);
  }
  if (has_scale()) glDisable(GL_NORMALIZE);
}

//! \brief draws the cylindrical patch in selection mode.
void Cylindrical_patch::isect(Isect_action* /* action */)
{
  Float top = m_height / 2;
  Float bottom = -top;

  glBegin(GL_QUADS);
  unsigned int i;
  Float cos_left, sin_left, cos_right, sin_right;
  sincosf(m_alpha, &sin_left, &cos_left);
  Float delta = (m_beta - m_alpha) / m_slices;
  for (i = 1; i < m_slices; ++i) {
    Float acc_angle = m_alpha + delta * i;
    sincosf(acc_angle, &sin_right, &cos_right);
    Float v_left_x = m_radius * cos_left;
    Float v_left_z = -m_radius * sin_left;
    Float v_right_x = m_radius * cos_right;
    Float v_right_z = -m_radius * sin_right;
    glVertex3f(v_left_x, bottom, v_left_z);
    glVertex3f(v_right_x, bottom, v_right_z);
    glVertex3f(v_right_x, top, v_right_z);
    glVertex3f(v_left_x, top, v_left_z);
    cos_left = cos_right;
    sin_left = sin_right;
  }
  sincosf(m_beta, &sin_right, &cos_right);
  Float v_left_x = m_radius * cos_left;
  Float v_left_z = -m_radius * sin_left;
  Float v_right_x = m_radius * cos_right;
  Float v_right_z = -m_radius * sin_right;
  glVertex3f(v_left_x, bottom, v_left_z);
  glVertex3f(v_right_x, bottom, v_right_z);
  glVertex3f(v_right_x, top, v_right_z);
  glVertex3f(v_left_x, top, v_left_z);
  glEnd();
}

//! \brief calculares the sphere bound of the cylindrical patch.
Boolean Cylindrical_patch::clean_sphere_bound()
{
  float radius = sqrtf(m_height * m_height / 4 + m_radius * m_radius);
  m_sphere_bound.set_radius(radius);
  m_sphere_bound.set_center(Vector3f(0, 0, 0));
  return true;
}

//! \brief sets the container attributes.
void Cylindrical_patch::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "radius") {
      set_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      set_height(boost::lexical_cast<Float>(value));
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
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  // calculate_sphere_bound();
}

#if 0
/*! Get a list of attributes for the object.
 * The list is of name=value pairs.
 * \return a list of pairs of strings
 */
Attribute_list Cylindrical_patch::get_attributes()
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

  if (m_height != s_def_height) {
    attrib.first = "height";
    sprintf(buf, "%g", get_height());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_stacks != s_def_stacks) {
    attrib.first = "stacks";
    sprintf(buf, "%d", get_stacks());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (get_is_top_visible() != s_def_is_top_visible) {
    attrib.first = "alpha";
    attrib.second = get_alpha();
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief initializes the cylindrical patch prototype.
void Cylindrical_patch::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:

  // radius
  Execution_function exec_func =
    static_cast<Execution_function>(&Geometry::sphere_bound_changed);
  Float_handle_function radius_func =
    static_cast<Float_handle_function>(&Cylindrical_patch::radius_handle);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           RULE_EXPOSED_FIELD,
                                           radius_func,
                                           exec_func));

  // height
  Float_handle_function height_func =
    static_cast<Float_handle_function>(&Cylindrical_patch::height_handle);
  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           RULE_EXPOSED_FIELD,
                                           height_func,
                                           exec_func));

  // slices
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Uint_handle_function slices_func =
    static_cast<Uint_handle_function>(&Cylindrical_patch::slices_handle);
  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          RULE_EXPOSED_FIELD,
                                          slices_func,
                                          exec_func));

  // alpha
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function alpha_func =
    static_cast<Float_handle_function>(&Cylindrical_patch::alpha_handle);
  s_prototype->add_field_info(new SF_float(ALPHA, "alpha",
                                           RULE_EXPOSED_FIELD,
                                           alpha_func,
                                           exec_func));

  // beta
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Float_handle_function beta_func =
    static_cast<Float_handle_function>(&Cylindrical_patch::beta_handle);
  s_prototype->add_field_info(new SF_float(BETA, "beta",
                                           RULE_EXPOSED_FIELD,
                                           beta_func,
                                           exec_func));
}

//! \brief deletes the prototype.
void Cylindrical_patch::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Cylindrical_patch::get_prototype()
{
  if (!s_prototype) Cylindrical_patch::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
