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
// $Source$
// $Revision: 6147 $
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

std::string Gradient_background::s_tag = "sgalGradientBackground";
Container_proto * Gradient_background::s_prototype = 0;

// Default values:
const Vector4f Gradient_background::m_defUlColor(0.5, 0, 0  , 0);
const Vector4f Gradient_background::m_defUrColor(0.5, 0, 0  , 0);
const Vector4f Gradient_background::m_defLlColor(0,   0, 0.5, 0);
const Vector4f Gradient_background::m_defLrColor(0,   0, 0.5, 0);

float Gradient_background::m_size = 1.0f;

REGISTER_TO_FACTORY(Gradient_background, "Gradient_background");

/*! Constructor */
Gradient_background::Gradient_background(Boolean proto) :
  Background(proto),
  m_ulColor(m_defUlColor),
  m_urColor(m_defUrColor),
  m_llColor(m_defLlColor),
  m_lrColor(m_defLrColor),
  m_appearance(0)
{}

/*! Destructor */
Gradient_background::~Gradient_background()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Gradient_background ...");
  delete m_appearance;
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/** 
 * set the color of the upper left corner
 * @param color (in) the color
 */
void Gradient_background::SetULColor(Vector4f color) 
{
  m_ulColor = color;
}

/** 
 * get the color of the upper left corner
 * @return the color
 */
Vector4f Gradient_background::GetULColor() const
{
  return m_ulColor;
}

/** 
 * set the color of the upper right corner
 * @param color (in) the color
 */
void Gradient_background::SetURColor(Vector4f color) 
{
  m_urColor = color;
}

/** 
 * get the color of the upper right corner
 * @return the color
 */
Vector4f Gradient_background::GetURColor() const
{
  return m_urColor;
}

/** 
 * set the color of the lower left corner
 * @param color (in)  the color
 */
void Gradient_background::SetLLColor(Vector4f color) 
{
  m_llColor = color;
}

/** 
 * get the color of the lower left corner
 * @return the color
 */
Vector4f Gradient_background::GetLLColor() const
{
  return m_llColor;
}

/** 
 * set the color of the lower right corner
 * @param color (in)  the color
 */
void Gradient_background::SetLRColor(Vector4f color) 
{
  m_lrColor = color;
}

/** 
 * get the color of the lower right corner
 * @return the color
 */
Vector4f Gradient_background::GetLRColor() const
{
  return m_lrColor;
}

/** draw the background. This is done by setting the clear color. */
Trav_directive Gradient_background::Draw(Draw_action *draw_action)
{
  Context *context = draw_action->get_context();
  if (!context) {
    return Trav_cont;
  }
  context->ClearDepthBuffer();

  if (!m_appearance) {
    CreateAppearance();
  }

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

void Gradient_background::DrawPolygon(Context *context) 
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
  m_appearance->SetLightEnable(SGAL_FALSE);
  m_appearance->SetDepthEnable(SGAL_FALSE);
  m_appearance->SetPolyMode(Gfx::FILL_PMODE);
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Gradient_background::set_attributes(Element * elem)
{
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "ll") {
      SetLLColor(Vector4f(value));
      elem->mark_delete(ai);
    } else if (name == "lr")  {
      SetLRColor(Vector4f(value));
      elem->mark_delete(ai);
    } else if (name == "ur") {
      SetURColor(Vector4f(value));
      elem->mark_delete(ai);
    } else if (name == "ul") {
      SetULColor(Vector4f(value));
      elem->mark_delete(ai);
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*!
 */
Attribute_list Gradient_background::get_attributes()
{
  Attribute_list attribs; 
  Attribue attrib;

  attribs = Node::get_attributes();

  if (m_llColor != m_defLlColor)
  {
    attrib.first = "ll";
    attrib.second = GetLLColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_lrColor != m_defLrColor)
  {
    attrib.first = "lr";
    attrib.second = GetLRColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_urColor != m_defUrColor)
  {
    attrib.first = "ur";
    attrib.second = GetURColor().get_text();
    attribs.push_back(attrib);
  }

  if (m_ulColor != m_defUlColor)
  {
    attrib.first = "ul";
    attrib.second = GetULColor().get_text();
    attribs.push_back(attrib);
  }

  return attribs;
}

void Gradient_background::init_prototype()
{
  if (s_prototype != NULL) {
    return;
  }

  // Allocate a prototype instance
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  s_prototype->add_field_info(new SF_Vector3f(ULCOLOR,
                      "ulColor",
                      get_member_offset(&m_ulColor),
                      (Execution_func_type)&Container::SetRenderingRequired));    
  s_prototype->add_field_info(new SF_Vector3f(URCOLOR,
                      "urColor",
                      get_member_offset(&m_urColor),
                      (Execution_func_type)&Container::SetRenderingRequired));    
  s_prototype->add_field_info(new SF_Vector3f(LLCOLOR,
                      "llColor",
                      get_member_offset(&m_llColor),
                      (Execution_func_type)&Container::SetRenderingRequired));    
  s_prototype->add_field_info(new SF_Vector3f(LRCOLOR,
                      "lrColor",
                      get_member_offset(&m_lrColor),
                      (Execution_func_type)&Container::SetRenderingRequired));    
}

/*!
 */
void Gradient_background::delete_prototype()
{
  delete s_prototype;
}

/*!
 */
Container_proto * Gradient_background::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
