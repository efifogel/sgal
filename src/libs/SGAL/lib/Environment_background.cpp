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

#include "SGAL/basic.hpp"
#include "SGAL/Environment_background.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Environment_background::s_tag = "Background";
Container_proto * Environment_background::s_prototype(NULL);

const Vector3f Environment_background::m_def_sky_color(0, 0, 0);

REGISTER_TO_FACTORY(Environment_background, "Environment_background");

/*! Constructor */
Environment_background::Environment_background(Boolean proto) :
  Background(proto),
  m_sky_color(m_def_sky_color),
  m_back_appearance(NULL),
  m_back_texture(NULL)
{}

/*! Destructor */
Environment_background::~Environment_background()
{
  if (m_back_texture) {
    delete m_back_texture;
    m_back_texture = NULL;
  }

  if (m_back_appearance) {
    delete m_back_appearance;
    m_back_appearance = NULL;

  }
}

/*! \brief sets the attributes of the object extracted from the input file */
void Environment_background::set_attributes(Element * elem)
{
  Background::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);

    if (name == "skyColor") {
      set_sky_color(Vector3f(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "backUrl") {
      //! \todo fix backUrl
//       std::string url = strip_double_quotes(value);
//       Appearance * app = new Appearance;
//       m_back_texture = new Image_texture;
//       m_back_texture->set_url(url);
//       m_back_texture->set_dirs(sg->get_data_dirs());
//       app->set_texture(m_back_texture);
//       app->set_tex_enable(true);
//       set_back_appearance(app);
//       url.clear();
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*!
 */
void Environment_background::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Background::get_prototype());
}

/*!
 */
void Environment_background::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*!
 */
Container_proto * Environment_background::get_prototype()
{
  if (s_prototype == NULL) Environment_background::init_prototype();
  return s_prototype;
}

/*! Set the appearance of the object
 * @param app the appearance
 */
void Environment_background::set_back_appearance(Appearance * app)
{
  m_back_appearance = app;
  m_back_appearance->set_light_enable(false);
  m_back_appearance->set_depth_enable(false);
  m_back_appearance->set_poly_mode(Gfx::FILL_PMODE);
}

/*! Drwa the background */
void Environment_background::draw(Draw_action * draw_action)
{
  //! \todo this is not right at all!!!
  if (m_back_appearance) {
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
    m_back_appearance->draw(draw_action);
    draw_polygon();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

  } else {
    Context * context = draw_action->get_context();
    if (context) context->clear_color_depth_buffer(m_sky_color);
  }
}

SGAL_END_NAMESPACE
