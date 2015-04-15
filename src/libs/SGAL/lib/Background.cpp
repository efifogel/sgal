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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Background::s_tag = "Background";
Container_proto* Background::s_prototype(NULL);

REGISTER_TO_FACTORY(Background, "Background");

//! \brief constructs.
Background::Background(Boolean proto) :
  Bindable_node(proto),
  m_clear_color(true),
  m_clear_depth(true),
  m_clear_stencil(false)
{}

//!\brief destructs.
Background::~Background() {}

//! Obtain the bindable stack.
Bindable_stack* Background::get_stack()
{ return m_scene_graph->get_background_stack(); }

//! \brief sets the attributes of this object.
void Background::set_attributes(Element* elem)
{
  Bindable_node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "clearColor") {
      set_clear_color(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "clearDepth") {
      set_clear_depth(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "clearStencil") {
      set_clear_stencil(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Background::add_to_scene(Scene_graph* sg)
{
  set_scene_graph(sg);

  // Push this bindable Navigation_info node onto the respective stack:
  insert_stack(this);
}

//! \brief initializes the protoype.
void Background::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Bindable_node::get_prototype());
}

//! \brief deletes the protoype.
void Background::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the protoype.
Container_proto* Background::get_prototype()
{
  if (s_prototype == NULL) Background::init_prototype();
  return s_prototype;
}

//! \breif drwas the background.
void Background::draw(Draw_action* draw_action)
{
  Context* context = draw_action->get_context();
  if (!context) return;
  Uint which = 0;
  if (m_clear_color) which |= Gfx::COLOR_CLEAR;
  if (m_clear_depth) which |= Gfx::DEPTH_CLEAR;
  if (m_clear_stencil) which |= Gfx::STENCIL_CLEAR;
  context->clear(which);
}

//! \brief draws a 2D polygon with texture coordinates.
void Background::draw_polygon()
{
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex2f(-1, -1);
  glTexCoord2f(1, 0); glVertex2f(1, -1);
  glTexCoord2f(1, 1); glVertex2f(1, 1);
  glTexCoord2f(0, 1); glVertex2f(-1, 1);
  glEnd();
}

SGAL_END_NAMESPACE
