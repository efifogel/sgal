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
// $Source: $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Cone.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

std::string Cone::s_tag = "Cone";
Container_proto* Cone::s_prototype(NULL);

// Default values:
const Float Cone::s_def_bottom_radius(1);
const Float Cone::s_def_height(2);
const Uint  Cone::s_def_stacks(1);
const Uint  Cone::s_def_slices(20);
const Boolean  Cone::s_def_bottom_visible(true);
const Boolean  Cone::s_def_side_visible(true);

REGISTER_TO_FACTORY(Cone, "Cone");

/*! Constructor */
Cone::Cone(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_bottom_radius(s_def_bottom_radius),
  m_height(s_def_height),
  m_stacks(s_def_stacks),
  m_slices(s_def_slices),
  m_side_visible(s_def_side_visible),
  m_bottom_visible(s_def_bottom_visible),
  m_cone(0),
  m_cone_base(0)
{
}

/*! Destructor */
Cone::~Cone()
{
  gluDeleteQuadric(m_cone);
  gluDeleteQuadric(m_cone_base);
}

/**
 * Draws the Cone.
 * @param draw_action the draw action
 */
void Cone::draw(Draw_action* action)
{
  if (is_dirty()) clean();

  if (has_scale()) glEnable(GL_NORMALIZE);

  glPushMatrix();
  glRotatef(90, 1, 0, 0);

  if (m_side_visible) {
    // draw the cone. (cylinder with up radius = 0)
    glPushMatrix();
    glTranslatef(0, 0, -m_height/2);
    gluQuadricTexture(m_cone, (m_generated_tex_coord) ? GL_TRUE : GL_FALSE);
    gluCylinder(m_cone, 0, m_bottom_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  if (m_bottom_visible) {
    glTranslatef(0, 0, m_height/2);
    gluQuadricTexture(m_cone_base,
                      (m_generated_tex_coord) ? GL_TRUE : GL_FALSE);
    gluDisk(m_cone_base, 0, m_bottom_radius, m_slices, 1);
  }

  glPopMatrix();

  glDisable(GL_NORMALIZE);
}

/**
 * Draws the object in selection mode
 * @param action
 */
void Cone::isect(Isect_action* action)
{
  if (is_dirty()) clean();

  glPushMatrix();
  glRotatef(90, 1, 0, 0);

  if (m_side_visible) {
    // draw the cone. (cylinder with up radius = 0)
    glPushMatrix();
    glTranslatef(0, 0, -m_height/2);
    gluCylinder(m_cone, 0, m_bottom_radius, m_height, m_slices, m_stacks);
    glPopMatrix();
  }

  if (m_bottom_visible) {
    glTranslatef(0, 0, m_height/2);
    gluDisk(m_cone_base, 0, m_bottom_radius, m_slices, m_slices);
  }

  glPopMatrix();
}

/*! \brief calculates the sphere bound of the cone. */
Boolean Cone::clean_sphere_bound()
{
  float radius = (m_height * m_height + m_bottom_radius * m_bottom_radius) /
    (2 * m_height);
  m_sphere_bound.set_radius(radius);
  m_sphere_bound.set_center(Vector3f(0, m_height / 2 - radius, 0));
  return true;
}

/*! \brief initializes the quadric object. */
void Cone::clean()
{
  m_cone = gluNewQuadric();
  gluQuadricOrientation(m_cone, GLU_OUTSIDE);
  gluQuadricNormals(m_cone, GLU_SMOOTH);
  gluQuadricDrawStyle(m_cone, GLU_FILL);

  m_cone_base = gluNewQuadric();
  gluQuadricOrientation(m_cone_base, GLU_OUTSIDE);
  gluQuadricNormals(m_cone_base, GLU_SMOOTH);
  gluQuadricDrawStyle(m_cone_base, GLU_FILL);

  m_dirty = false;
}

/*! \brief sets the attributes of the object extracted from the VRML or X3D
 * file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Cone::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "bottomRadius") {
      set_bottom_radius(atoff(value.c_str()));
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
    else if (name == "slices") {
      set_slices(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bottom") {
      set_is_bottom_visible(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "side") {
      set_is_side_visible(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  // calculate_sphere_bound();
}

#if 0
/**
 * Get a list of attributes for the object.
 * The list is of name=value pairs.
 * @return a list of pairs of strings
 */
Attribute_list Cone::get_attributes()
{
  Attribute_list attribs;
  // attribs = Geometry::get_attributes();
  Attribue attrib;
  char buf[32];

  attribs = Geometry::get_attributes();

  if(get_bottom_radius() != s_def_bottom_radius) {
    attrib.first = "bottom_radius";
    sprintf(buf, "%g", get_bottom_radius());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_height() != s_def_height) {
    attrib.first = "height";
    sprintf(buf, "%g", get_height());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_slices() != s_def_slices) {
    attrib.first = "slices";
    sprintf(buf, "%d", get_slices());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if(get_stacks() != s_def_stacks) {
    attrib.first = "stacks";
    sprintf(buf, "%d", get_stacks());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (get_is_side_visible() != s_def_is_side_visible) {
    attrib.first = "side";
    if (get_is_side_visible()) attrib.second = TRUE_STR;
    else attrib.second = FALSE_STR;
    attribs.push_back(attrib);
  }

  if (get_is_bottom_visible() != s_def_is_bottom_visible) {
    attrib.first = "bottom";
    if (get_is_bottom_visible()) attrib.second = TRUE_STR;
    else attrib.second = FALSE_STR;
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

/*! \brief initilalizes the prototype object in the class. */
void Cone::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:

  // bottomRadius
  Execution_function exec_func =
    static_cast<Execution_function>(&Geometry::sphere_bound_changed);
  s_prototype->add_field_info(new SF_float(BOTTOM_RADIUS, "bottomRadius",
                                           get_member_offset(&m_bottom_radius),
                                           exec_func));

  // height
  s_prototype->add_field_info(new SF_float(HEIGHT, "height",
                                           get_member_offset(&m_height),
                                           exec_func));

  // side
  s_prototype->
    add_field_info(new SF_bool(SIDE, "side",
                               get_member_offset(&m_side_visible),
                               exec_func));

  // bottom
  s_prototype->
    add_field_info(new SF_bool(BOTTOM, "bottom",
                               get_member_offset(&m_bottom_visible),
                               exec_func));

  // slices
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_int(SLICES, "slices",
                                         get_member_offset(&m_slices),
                                         exec_func));

  // stacks
  s_prototype->add_field_info(new SF_int(STACKS, "stacks",
                                         get_member_offset(&m_stacks),
                                         exec_func));
}

/*! */
void Cone::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Cone::get_prototype()
{
  if (!s_prototype) Cone::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
