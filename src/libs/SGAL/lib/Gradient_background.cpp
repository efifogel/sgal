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

#include "SGAL/basic.hpp"
#include "Gradient_background.h"
#include "Context.h"
#include "Scene_graph.h"
#include "Draw_action.h"
#include "Element.h"
#include "Container_proto.h"
#include "Trace.h"

SGAL_BEGIN_NAMESPACE

std::string Gradient_background::s_tag = "GradientBackground";
Container_proto * Gradient_background::s_prototype(NULL);

// Default values:
const Vector4f Gradient_background::s_defUlColor(0.5, 0, 0  , 0);
const Vector4f Gradient_background::s_defUrColor(0.5, 0, 0  , 0);
const Vector4f Gradient_background::s_defLlColor(0,   0, 0.5, 0);
const Vector4f Gradient_background::s_defLrColor(0,   0, 0.5, 0);

float Gradient_background::m_size(1.0f);

REGISTER_TO_FACTORY(Gradient_background, "Gradient_background");

/*! Constructor */
Gradient_background::Gradient_background(Boolean proto) :
  Background(proto),
  m_ulColor(s_defUlColor),
  m_urColor(s_defUrColor),
  m_llColor(s_defLlColor),
  m_lrColor(s_defLrColor),
  m_appearance(0)
{}

/*! Destructor */
Gradient_background::~Gradient_background() { delete m_appearance; }

/*! \brief draws the background. This is done by setting the clear color. */
Trav_directive Gradient_background::draw(Draw_action* draw_action)
{
  Context* context = draw_action->get_context();
  if (!context) return Trav_cont;
  context->ClearDepthBuffer();

  if (!m_appearance) CreateAppearance();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glOrtho(-m_size, m_size, -m_size, m_size, m_size, -m_size);
  m_appearance->Draw(draw_action);
  DrawPolygon(context);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  return Trav_cont;
}

void Gradient_background::DrawPolygon(Context* context)
{
  glBegin(GL_QUADS);
    glColor3fv((float*)&m_llColor);
    glVertex2f(-m_size, -m_size);
    glColor3fv((float *)&m_lrColor);
    glVertex2f(m_size, -m_size);
    glColor3fv((float *)&m_urColor);
    glVertex2f(m_size,  m_size);
    glColor3fv((float *)&m_ulColor);
    glVertex2f(-m_size,  m_size);
  glEnd();

}

void Gradient_background::CreateAppearance()
{
  m_appearance = new Appearance();
  m_appearance->SetLightEnable(false);
  m_appearance->SetDepthEnable(false);
  m_appearance->SetPolyMode(Gfx::FILL_PMODE);
}

/*! \brief sets the attributes of this container. */
void Gradient_background::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "ll") {
      SetLLColor(Vector4f(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "lr")  {
      SetLRColor(Vector4f(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "ur") {
      SetURColor(Vector4f(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "ul") {
      SetULColor(Vector4f(value));
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief */
Attribute_list Gradient_background::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

  attribs = Node::get_attributes();

  if (m_llColor != m_defLlColor) {
    attrib.first = "ll";
    attrib.second = GetLLColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_lrColor != m_defLrColor) {
    attrib.first = "lr";
    attrib.second = GetLRColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_urColor != m_defUrColor) {
    attrib.first = "ur";
    attrib.second = GetURColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_ulColor != m_defUlColor) {
    attrib.first = "ul";
    attrib.second = GetULColor().get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}

void Gradient_background::init_prototype()
{
  if (s_prototype != NULL) return;
  s_prototype = new Container_proto(Node::get_prototype());

  Execution_function exec_func;

  // Add the object fields to the prototype
  exec_func = static_cast<Execution_function>
    (&Gradient_background::SetRenderingRequired);
  Vector3f_handle_function ul_color_func =
    static_cast<Vector3f_handle_function>(&Gradient_background::ul_color_handle);
  s_prototype->add_field_info(new SF_Vector3f(ULCOLOR, "ul_color",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              ul_color_func, exec_func));

  Vector3f_handle_function ur_color_func =
    static_cast<Vector3f_handle_function>(&Gradient_background::ur_color_handle);
  s_prototype->add_field_info(new SF_Vector3f(URCOLOR, "ur_color",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              ur_color_func, exec_func));

  Vector3f_handle_function ll_color_func =
    static_cast<Vector3f_handle_function>(&Gradient_background::ll_color_handle);
  s_prototype->add_field_info(new SF_Vector3f(LLCOLOR, "ll_color",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              ll_color_func, exec_func));

  Vector3f_handle_function lr_color_func =
    static_cast<Vector3f_handle_function>(&Gradient_background::lr_color_handle);
  s_prototype->add_field_info(new SF_Vector3f(LRCOLOR, "lr_color",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              lr_color_func, exec_func));
}

/*! \brief */
void Gradient_background::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Gradient_background::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
