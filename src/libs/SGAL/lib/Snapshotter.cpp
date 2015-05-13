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

const std::string Snapshotter::s_tag = "Snapshotter";
Container_proto* Snapshotter::s_prototype(nullptr);

// Default values
const Boolean Snapshotter::s_def_flip(true);
const Boolean Snapshotter::s_def_front_buffer(false);

REGISTER_TO_FACTORY(Snapshotter, "Snapshotter");

//! \brief constructor
Snapshotter::Snapshotter(Boolean proto) :
  Node(proto),
  m_count(0),
  m_trigger(false),
  m_front_buffer(s_def_front_buffer),
  m_flip(s_def_flip),
  m_size(0)
{}

//! \brief destructor
Snapshotter::~Snapshotter() {}

//! \brief takes a snapshot and write to a file if triggered.
Action::Trav_directive Snapshotter::draw(Draw_action* draw_action)
{
  if (!m_trigger) return Action::TRAV_CONT;
  m_trigger = false;

  if (!allocate_space(draw_action)) return Action::TRAV_CONT;
  take_snapshot();
  auto* field = get_field(IMAGE);
  if (field != nullptr) field->cascade();
  return Action::TRAV_CONT;
}

//! \brief allocates space for the image.
Boolean Snapshotter::allocate_space(Draw_action* action)
{
  if (!m_image) m_image.reset(new Image_base);
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();

  if ((width == 0) || (height == 0)) {
    Context* context = action->get_context();
    if (!context) return false;
    Uint x0 = 0, y0 = 0;
    Uint tw, th;
    context->get_viewport(x0, y0, tw, th);
    if (width == 0) {
      m_image->set_width(tw);
      width = tw;
    }
    if (height == 0) {
      height = th;
      m_image->set_height(th);
    }
  }

  if (width == 0 || height == 0) return false;
  Uint size = Image::get_size(width, height, format);
  if (size > m_size) {
    Uchar* pixels = (Uchar *) m_image->get_pixels();
    if (pixels) delete [] pixels;
    pixels = new Uchar[size];
    if (!pixels) return false;
    m_image->set_pixels(pixels);
    m_size = size;
  }
  return true;
}

//! \brief takes a snapshot of the window.
void Snapshotter::take_snapshot()
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  void* pixels = m_image->get_pixels();
  GLenum gl_format = m_image->get_format_format(format);
  GLenum gl_type = m_image->get_format_type(format);

  GLint val;
  glGetIntegerv(GL_READ_BUFFER, &val);
  GLenum read_buffer_mode = (GLenum)val;
  (is_front_buffer()) ? glReadBuffer(GL_FRONT) : glReadBuffer(GL_BACK);
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
  // frontBuffer
  auto front_buffer_func =
    static_cast<Boolean_handle_function>(&Snapshotter::front_buffer_handle);
  s_prototype->add_field_info(new SF_bool(FLIP, "frontBuffer",
                                          Field_info::RULE_EXPOSED_FIELD,
                                          front_buffer_func,
                                          s_def_front_buffer));

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
    if (name == "frontBuffer") {
      set_front_buffer(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "flip") {
      set_flip(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "sequence") {
      m_sequence = compare_to_true(value);
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
void Snapshotter::add_to_scene(Scene_graph* sg) { sg->set_snaphotter(this); }

SGAL_END_NAMESPACE
