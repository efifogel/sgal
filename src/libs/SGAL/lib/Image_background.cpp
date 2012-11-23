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
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Image_background.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Context.hpp"

SGAL_BEGIN_NAMESPACE

std::string Image_background::s_tag = "sgalImageBackground";
Container_proto * Image_background::s_prototype = NULL;

REGISTER_TO_FACTORY(Image_background, "Image_background");

/*! Constructor */
Image_background::Image_background(Boolean proto) :
  Background(proto),
  m_appearance(NULL),
  m_is_default_appearance(SGAL_FALSE)
{}

/*! Destructor */
Image_background::~Image_background()
{
  if (m_is_default_appearance && m_appearance) {
    delete m_appearance;
    m_appearance = NULL;
  }
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Image_background::set_attributes(Element * elem) 
{
  Background::set_attributes(elem);

  typedef Element::Cont_attr_iter         Cont_attr_iter;

  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "appearance") {
      Appearance * app = dynamic_cast<Appearance*>(cont);
      set_appearance(app);
      elem->mark_delete(cai);
      continue;
    }
  }
  
  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*!
 */
void Image_background::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance
  s_prototype = new Container_proto(Background::get_prototype());
}

/*!
 */
void Image_background::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*!
 */
Container_proto * Image_background::get_prototype() 
{  
  if (s_prototype == NULL) Image_background::init_prototype();
  return s_prototype;
}

/*! Set the appearance of the object 
 * @param app the appearance
 */
void Image_background::set_appearance(Appearance *app) 
{
  m_appearance = app;
  m_appearance->set_light_enable(SGAL_FALSE);
  m_appearance->set_depth_enable(SGAL_FALSE);
  m_appearance->set_poly_mode(Gfx::FILL_PMODE);
}

/*! Draws the appearance and then all the geometries 
 * @param draw_action
 */
void Image_background::draw(Draw_action * draw_action)
{
  if (!m_appearance) return;

  Context * context = draw_action->get_context();
  if (!context) return;
  context->clear_depth_buffer();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glOrtho(-1, 1, -1, 1, 1, -1);
  m_appearance->draw(draw_action);
  draw_polygon();

  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

SGAL_END_NAMESPACE
