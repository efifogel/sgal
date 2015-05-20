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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Snapshotter.hpp"
#include "SGAL/File_format_2d.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Field.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Snapshotter::s_tag("Snapshotter");
Container_proto* Snapshotter::s_prototype(nullptr);

// Default values
const Boolean Snapshotter::s_def_flip(true);
const Snapshotter::Mode Snapshotter::s_def_mode(MODE_BACK);

//! The color buffer names.
const Char* Snapshotter::s_mode_names[] = {
  "frontLeft",
  "frontRight",
  "backLeft",
  "backRight",
  "front",
  "back",
  "left",
  "right",
  "colorAttachment"
};

REGISTER_TO_FACTORY(Snapshotter, "Snapshotter");

//! \brief constructor
Snapshotter::Snapshotter(Boolean proto) :
  Node(proto),
  m_trigger(false),
  m_mode(s_def_mode),
  m_flip(s_def_flip)
{}

//! \brief destructor
Snapshotter::~Snapshotter() {}

//! \brief takes a snapshot and writes it to an image file.
void Snapshotter::execute()
{
  if (!m_image) return;
  m_image->allocate_space();
  take_snapshot();
  auto* field = get_field(IMAGE);
  if (field != nullptr) field->cascade();
}

//! \brief if triggered takes a snapshot and writes it to an image file.
Action::Trav_directive Snapshotter::draw(Draw_action* draw_action)
{
  if (!m_trigger) return Action::TRAV_CONT;
  m_trigger = false;

  set_size(draw_action);
  execute();
  return Action::TRAV_CONT;
}

/*! Set the size of image.
 */
void Snapshotter::set_size(Draw_action* draw_action)
{
  if (!m_image) m_image.reset(new Image);
  auto width = m_image->get_width();
  auto height = m_image->get_height();

  if ((width != 0) && (height != 0)) return;
  auto* context = draw_action->get_context();
  SGAL_assertion(context);
  Uint x0 = 0, y0 = 0;
  Uint tw, th;
  context->get_viewport(x0, y0, tw, th);
  if (width == 0) m_image->set_width(tw);
  if (height == 0) m_image->set_height(th);
}

//! \brief takes a snapshot of the window.
void Snapshotter::take_snapshot()
{
  auto width = m_image->get_width();
  auto height = m_image->get_height();
  Image::Format format = m_image->get_format();
  void* pixels = m_image->get_pixels();
  GLenum gl_format = m_image->get_format_format(format);
  GLenum gl_type = m_image->get_format_type(format);

  GLint val;
  glGetIntegerv(GL_READ_BUFFER, &val);
  GLenum read_buffer_mode = (GLenum)val;
  static GLenum gl_modes[] = {
    GL_FRONT_LEFT,
    GL_FRONT_RIGHT,
    GL_BACK_LEFT,
    GL_BACK_RIGHT,
    GL_FRONT,
    GL_BACK,
    GL_LEFT,
    GL_RIGHT,
    GL_COLOR_ATTACHMENT0
  };
  glReadBuffer(gl_modes[get_mode()]);
  glReadPixels(0, 0, width, height, gl_format, gl_type, pixels);
  glReadBuffer(read_buffer_mode);
}

//! \brief initializes the container prototype.
void Snapshotter::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Node::get_prototype());

  // trigger
  auto trigger_func =
    static_cast<Boolean_handle_function>(&Snapshotter::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER,
                                          "trigger",
                                          Field_info::RULE_IN,
                                          trigger_func,
                                          false));

  // mode
  auto mode_func =
    reinterpret_cast<Uint_handle_function>(&Snapshotter::mode_handle);
  s_prototype->add_field_info(new SF_uint(FLIP, "mode",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          mode_func,
                                          s_def_mode));

  // flip
  auto flip_func =
    static_cast<Boolean_handle_function>(&Snapshotter::flip_handle);
  s_prototype->add_field_info(new SF_bool(FLIP, "flip",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          flip_func,
                                          s_def_flip));

  // Image
  auto image_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Snapshotter::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE,
                                                      "image",
                                                      Field_info::RULE_EXPOSED_FIELD,
                                                      image_func));
}

//! \brief deletes the container prototype.
void Snapshotter::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Snapshotter::get_prototype()
{
  if (!s_prototype) Snapshotter::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Snapshotter::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "mode") {
      auto num = sizeof(s_mode_names) / sizeof(char *);
      const auto** found =
        std::find(s_mode_names, &s_mode_names[num], strip_double_quotes(value));
      auto index = found - s_mode_names;
      if (index < num) set_mode(static_cast<Mode>(index));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "image") {
      m_image = boost::dynamic_pointer_cast<Image>(cont);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

//! \brief adds the container to a given scene.
void Snapshotter::add_to_scene(Scene_graph* sg) { sg->add_snaphotter(this); }

SGAL_END_NAMESPACE
