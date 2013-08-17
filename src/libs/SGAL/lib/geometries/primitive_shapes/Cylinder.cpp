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
// $Id: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

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
Container_proto* Cylinder::s_prototype(NULL);

// default values:
const Float Cylinder::s_def_radius(1);
const Float Cylinder::s_def_height(2);
const Uint  Cylinder::s_def_stacks(1);
const Uint  Cylinder::s_def_slices(16);
const Boolean  Cylinder::s_def_is_bottom_visible(true);
const Boolean  Cylinder::s_def_is_top_visible(true);
const Boolean  Cylinder::s_def_is_body_visible(true);

REGISTER_TO_FACTORY(Cylinder, "Cylinder");

/*! Constructor */
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
  m_cylinder(NULL),
  m_cylinder_base(NULL)
{}

/*! Destructor */
Cylinder::~Cylinder()
{
  gluDeleteQuadric(m_cylinder);
  gluDeleteQuadric(m_cylinder_base);
}

/*! \brief draws the cylinder. */
void Cylinder::draw(Draw_action* action)
{
  if (is_dirty()) clean();

  if (has_scale()) glEnable(GL_NORMALIZE);

  glPushMatrix();
  glRotatef(-90, 1, 0, 0);

  if (m_is_body_visible) {
    // draw the cylinder
    glPushMatrix();
    glTranslatef(0, 0, -m_height / 2);
    gluQuadricTexture(m_cylinder, (m_generated_tex_coord) ? GL_TRUE : GL_FALSE);
    gluCylinder(m_cylinder, m_radius, m_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  // draw the base
  if (m_is_top_visible) {
    glPushMatrix();
    glTranslatef(0, 0, m_height/2);
    gluQuadricTexture(m_cylinder_base,
                      (m_generated_tex_coord) ? GL_TRUE : GL_FALSE);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    glPopMatrix();
  }

  if (m_is_bottom_visible) {
    glPushMatrix();
    glTranslatef(0, 0, -m_height/2);
    //glRotatef(180, 1, 0, 0);
    //glRotatef(180, 0, 1, 0);
    gluQuadricTexture(m_cylinder_base,
                      (m_generated_tex_coord) ? GL_TRUE : GL_FALSE);
    gluQuadricOrientation(m_cylinder_base,GLU_INSIDE);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    gluQuadricOrientation(m_cylinder_base,GLU_OUTSIDE);
    glPopMatrix();
  }

  glPopMatrix();

  glDisable(GL_NORMALIZE);
}

/*! \brief draws the cylinder in selection mode. */
void Cylinder::isect(Isect_action* action)
{
  if (m_dirty) clean();

  glPushMatrix();
  glRotatef(-90, 1, 0, 0);

  if (m_is_body_visible) {
    // draw the cylinder
    glPushMatrix();
    glTranslatef(0, 0, -m_height / 2);
    gluCylinder(m_cylinder, m_radius, m_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  // draw the base
  if (m_is_top_visible) {
    glTranslatef(0, 0, m_height / 2);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
  }

  if (m_is_bottom_visible) {
    glTranslatef(0, 0, -m_height);
    //glRotatef(180, 1, 0, 0);
    gluQuadricOrientation(m_cylinder_base,GLU_INSIDE);
    gluDisk(m_cylinder_base, 0, m_radius, m_slices, 1);
    gluQuadricOrientation(m_cylinder_base,GLU_OUTSIDE);
  }

  glPopMatrix();
}

/*! \brief cleans (regenerate the coordinates of) the cylinder. */
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

/*! \brief cleans (recompute) the bounding sphere of the cylinder. */
Boolean Cylinder::clean_sphere_bound()
{
  float radius = sqrtf(m_height * m_height / 4 + m_radius * m_radius);
  m_sphere_bound.set_radius(radius);
  m_sphere_bound.set_center(Vector3f(0, 0, 0));
  return true;
}

/*! \brief sets the attributes of the cylinder. */
void Cylinder::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "radius") {
      set_radius(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "height") {
      set_height(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stacks") {
      set_stacks(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "slices") {
      set_slices(atoi(value.c_str()));
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

#if 0
/*! Get a list of attributes for the object.
 * The list is of name=value pairs.
 * @return a list of pairs of strings
 */
Attribute_list Cylinder::get_attributes()
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

  if (get_is_bottom_visible() != s_def_is_bottom_visible) {
    attrib.first = "bottom";
    attrib.second = (get_is_bottom_visible()) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  if (get_is_body_visible() != s_def_is_body_visible) {
    attrib.first = "side";
    attrib.second = (get_is_body_visible()) ? TRUE_STR : FALSE_STR;
    attribs.push_back(attrib);
  }

  if (get_is_top_visible() != s_def_is_top_visible) {
    attrib.first = "top";
    attrib.second = (get_is_top_visible()) ? TRUE_STR: FALSE_STR;
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

/*! initializes the cylinder prototype */
void Cylinder::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // radius
  Execution_function exec_func =
    static_cast<Execution_function>(&Geometry::sphere_bound_changed);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           get_member_offset(&m_radius),
                                           exec_func));

  // height
  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           get_member_offset(&m_height),
                                           exec_func));

  // side
  s_prototype->add_field_info(new SF_bool(SIDE, "side",
                                          get_member_offset(&m_is_body_visible),
                                          exec_func));

  // bottom
  s_prototype->
    add_field_info(new SF_bool(BOTTOM, "bottom",
                               get_member_offset(&m_is_bottom_visible),
                               exec_func));

  // Rendering required
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_bool(TOP, "top",
                                          get_member_offset(&m_is_top_visible),
                                          exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_int(SLICES, "slices",
                                         get_member_offset(&m_slices),
                                         exec_func));

  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_int(STACKS, "stacks",
                                         get_member_offset(&m_stacks),
                                         exec_func));
}

/*! */
void Cylinder::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Cylinder::get_prototype()
{
  if (!s_prototype) Cylinder::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
