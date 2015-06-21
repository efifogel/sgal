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

#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Cylinder.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"

SGAL_BEGIN_NAMESPACE

std::string Cylinder::s_tag = "Cylinder";
Container_proto* Cylinder::s_prototype(nullptr);

// default values:
const Float Cylinder::s_def_radius(1);
const Float Cylinder::s_def_height(2);
const Uint  Cylinder::s_def_stacks(1);
const Uint  Cylinder::s_def_slices(16);
const Boolean  Cylinder::s_def_is_bottom_visible(true);
const Boolean  Cylinder::s_def_is_top_visible(true);
const Boolean  Cylinder::s_def_is_body_visible(true);

REGISTER_TO_FACTORY(Cylinder, "Cylinder");

//! \brief constructor.
Cylinder::Cylinder(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_radius(s_def_radius),
  m_height(s_def_height),
  m_stacks(s_def_stacks),
  m_slices(s_def_slices),
  m_is_bottom_visible(s_def_is_bottom_visible),
  m_is_top_visible(s_def_is_top_visible),
  m_is_body_visible(s_def_is_body_visible),
  m_cylinder(nullptr),
  m_cylinder_base(nullptr)
{}

//! \brief destructor.
Cylinder::~Cylinder()
{
  gluDeleteQuadric(m_cylinder);
  gluDeleteQuadric(m_cylinder_base);
}

//! \brief draws the cylinder.
void Cylinder::draw(Draw_action* /* action */)
{
  if (is_dirty()) clean();

  if (has_scale()) glEnable(GL_NORMALIZE);

  if (m_is_body_visible) {
    // draw the cylinder
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, -m_height/2);
    gluQuadricTexture(m_cylinder,
                      (do_generate_tex_coord()) ? GL_TRUE : GL_FALSE);
    gluCylinder(m_cylinder, m_radius, m_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  // draw the top
  if (m_is_top_visible) {
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, m_height/2);
    gluQuadricTexture(m_cylinder_base,
                      (do_generate_tex_coord()) ? GL_TRUE : GL_FALSE);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    glPopMatrix();
  }

  // draw the bottom
  if (m_is_bottom_visible) {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, m_height/2);
    gluQuadricTexture(m_cylinder_base,
                      (do_generate_tex_coord()) ? GL_TRUE : GL_FALSE);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    glPopMatrix();
  }

  glDisable(GL_NORMALIZE);
}

//! \brief draws the cylinder in selection mode.
void Cylinder::isect(Isect_action* /* action */)
{
  if (m_dirty) clean();

  glPushMatrix();

  if (m_is_body_visible) {
    // draw the cylinder
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, -m_height/2);
    gluCylinder(m_cylinder, m_radius, m_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  // draw the top
  if (m_is_top_visible) {
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, m_height/2);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    glPopMatrix();
  }

  // draw the bottom
  if (m_is_bottom_visible) {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, m_height/2);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    glPopMatrix();
  }
}

//! \brief cleans (regenerate the coordinates of) the cylinder.
void Cylinder::clean()
{
  m_cylinder = gluNewQuadric();
  gluQuadricOrientation(m_cylinder, GLU_OUTSIDE);
  gluQuadricNormals(m_cylinder, GLU_SMOOTH);
  gluQuadricDrawStyle(m_cylinder, GLU_FILL);

  m_cylinder_base = gluNewQuadric();
  gluQuadricOrientation(m_cylinder_base, GLU_OUTSIDE);
  gluQuadricNormals(m_cylinder_base, GLU_SMOOTH);
  gluQuadricDrawStyle(m_cylinder_base, GLU_FILL);

  m_dirty = false;
}

//! \brief cleans (recalculate) the bounding sphere of the cylinder.
void Cylinder::clean_bounding_sphere()
{
  float radius = sqrtf(m_height * m_height / 4 + m_radius * m_radius);
  m_bounding_sphere.set_radius(radius);
  m_bounding_sphere.set_center(Vector3f(0, 0, 0));
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of the cylinder.
void Cylinder::set_attributes(Element* elem)
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
    if (name == "height") {
      set_height(boost::lexical_cast<Float>(value));
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
    if (name == "bottomVisible") {
      set_is_bottom_visible(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "sideVisible") {
      set_is_body_visible(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "topVisible") {
      set_is_top_visible(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initializes the cylinder prototype.
void Cylinder::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // radius
  auto exec_func =
    static_cast<Execution_function>(&Geometry::bounding_sphere_changed);
  auto radius_func =
    static_cast<Float_handle_function>(&Cylinder::radius_handle);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           radius_func,
                                           exec_func));

  // height
  auto height_func =
    static_cast<Float_handle_function>(&Cylinder::height_handle);
  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           height_func,
                                           exec_func));

  // side
  auto is_body_visible_func =
    static_cast<Boolean_handle_function>(&Cylinder::is_body_visible_handle);
  s_prototype->add_field_info(new SF_bool(SIDE, "side",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_body_visible_func,
                                          exec_func));

  // bottom
  auto is_bottom_visible_func =
    static_cast<Boolean_handle_function>
    (&Cylinder::is_bottom_visible_handle);
  s_prototype->add_field_info(new SF_bool(BOTTOM, "bottom",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_bottom_visible_func, exec_func));

  // Rendering required
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto is_top_visible_func =
    static_cast<Boolean_handle_function>(&Cylinder::is_top_visible_handle);
  s_prototype->add_field_info(new SF_bool(TOP, "top",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          is_top_visible_func,
                                          exec_func));

  // slices
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto slices_func =
    static_cast<Uint_handle_function>(&Cylinder::slices_handle);
  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          slices_func,
                                          exec_func));

  // stacks
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto stacks_func =
    static_cast<Uint_handle_function>(&Cylinder::stacks_handle);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          stacks_func,
                                          exec_func));
}

//! \brief deletes the cylinder prototype.
void Cylinder::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the cylinder prototype.
Container_proto* Cylinder::get_prototype()
{
  if (!s_prototype) Cylinder::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
