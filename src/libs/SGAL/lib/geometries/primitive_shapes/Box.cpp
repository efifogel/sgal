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

#include "SGAL/Box.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Box::s_tag = "Box";
Container_proto* Box::s_prototype(nullptr);

// Default values:
const Vector3f Box::s_def_size(2, 2, 2);

REGISTER_TO_FACTORY(Box, "Box");

//! \brief constructor.
Box::Box(Boolean proto) : Geometry(proto), m_size(s_def_size) {}

//! \brief destructor.
Box::~Box() {}

//! \brief draws the box by calling the display list.
void Box::draw(Draw_action* /* action */) { draw_box(); }

//! \brief draws the box for selection.
void Box::isect(Isect_action* /* action */)
{
  float w = m_size[0] * 0.5f;
  float h = m_size[1] * 0.5f;
  float d = m_size[2] * 0.5f;

  float p0[] = { -w, -h, -d };
  float p1[] = {  w, -h, -d };
  float p2[] = {  w,  h, -d };
  float p3[] = { -w,  h, -d };
  float p4[] = { -w, -h,  d };
  float p5[] = {  w, -h,  d };
  float p6[] = {  w,  h,  d };
  float p7[] = { -w,  h,  d };

  glBegin(GL_QUADS);

  // front
  glVertex3fv(p0);
  glVertex3fv(p3);
  glVertex3fv(p2);
  glVertex3fv(p1);

  // right
  glVertex3fv(p1);
  glVertex3fv(p2);
  glVertex3fv(p6);
  glVertex3fv(p5);

  // back
  glVertex3fv(p5);
  glVertex3fv(p6);
  glVertex3fv(p7);
  glVertex3fv(p4);

  // left
  glVertex3fv(p4);
  glVertex3fv(p7);
  glVertex3fv(p3);
  glVertex3fv(p0);

  //top
  glVertex3fv(p3);
  glVertex3fv(p7);
  glVertex3fv(p6);
  glVertex3fv(p2);

  // bottom
  glVertex3fv(p4);
  glVertex3fv(p0);
  glVertex3fv(p1);
  glVertex3fv(p5);

  glEnd();
}

//! \brief calculates the bounding sphere of the box.
Boolean Box::clean_sphere_bound()
{
  float radius = sqrtf(m_size[0] * m_size[0] * 0.25f +
                       m_size[1] * m_size[1] * 0.25f +
                       m_size[2] * m_size[2] * 0.25f);
  m_sphere_bound.set_radius(radius);
  Vector3f vect(0, 0, 0);
  m_sphere_bound.set_center(vect);
  return true;
}

//! \brief initializes the display list that is used to draw the box.
void Box::init()
{
#if 0
  m_id = glGenLists(1);

  glNewList(m_id, GL_COMPILE);
  draw_box();
  glEndList();
#endif
}

//! \brief draws the box.
void Box::draw_box()
{
  float w = m_size[0] * 0.5f;
  float h = m_size[1] * 0.5f;
  float d = m_size[2] * 0.5f;

  float p0[] = { -w, -h, -d };
  float p1[] = {  w, -h, -d };
  float p2[] = {  w,  h, -d };
  float p3[] = { -w,  h, -d };
  float p4[] = { -w, -h,  d };
  float p5[] = {  w, -h,  d };
  float p6[] = {  w,  h,  d };
  float p7[] = { -w,  h,  d };

  if (has_scale()) glEnable(GL_NORMALIZE);

  glBegin(GL_QUADS);

  //! \todo handle different modal combinations
  // front
  glNormal3f(0, 0, -1);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p0);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p3);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p2);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p1);

  // right
  glNormal3f(1, 0, 0);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p1);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p2);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p6);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p5);

  // back
  glNormal3f(0, 0, 1);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p5);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p6);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p7);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p4);

  // left
  glNormal3f(-1, 0, 0);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p4);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p7);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p3);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p0);

  //top
  glNormal3f(0, 1,0);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p3);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p7);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p6);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p2);

  // bottom
  glNormal3f(0, -1, 0);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 0.0f); glVertex3fv(p4);
  if (do_generate_tex_coord()) glTexCoord2f(1.0f, 1.0f); glVertex3fv(p0);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 1.0f); glVertex3fv(p1);
  if (do_generate_tex_coord()) glTexCoord2f(0.0f, 0.0f); glVertex3fv(p5);

  glEnd();

  glDisable(GL_NORMALIZE);
}

//! \brief sets the attributes of this container.
void Box::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;

  Geometry::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "size") {
      set_size(Vector3f(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  // calculate_sphere_bound();
}

#if 0
//! \brief obtains the attributes of the box.
Attribute_list Box::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Geometry::get_attributes();

  if (m_size != s_def_size) {
    attrib.first = "size";
    attrib.second = get_size().get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief initializes the box prototype.
void Box::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // size
  Execution_function exec_func =
    static_cast<Execution_function>(&Geometry::sphere_bound_changed);
  Vector3f_handle_function size_func =
    static_cast<Vector3f_handle_function>(&Box::size_handle);
  s_prototype->add_field_info(new SF_vector3f(SIZE, "size",
                                              RULE_EXPOSED_FIELD,
                                              size_func,
                                              exec_func));
}

//! \brief deletes the prototype.
void Box::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Box::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
