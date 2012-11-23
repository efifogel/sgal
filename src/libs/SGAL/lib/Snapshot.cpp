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
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Magick++.h>

#if (defined _MSC_VER)
#define __WIN32__
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Snapshot.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

REGISTER_TO_FACTORY(Snapshot, "Snapshot");

std::string Snapshot::s_tag = "Snapshot";
Container_proto * Snapshot::s_prototype = NULL;

// Default values
const std::string Snapshot::s_def_dir_name = ".";
const std::string Snapshot::s_def_file_name = "snaphsot";
const Snapshot::File_format Snapshot::s_def_file_format = Snapshot::FF_ppm;
const Int Snapshot::s_def_quality = 50;

/*! File format names */
const char * Snapshot::s_file_format_names[] =
  { "", "jpg", "png", "gif", "tiff", "bmp", "ppm", "pgm", "pbm" };

/*! Constructor */
Snapshot::Snapshot(Boolean proto) :
  Node(proto),
  m_image(NULL),
  m_dir_name(s_def_dir_name),
  m_file_name(s_def_file_name),
  m_file_format(s_def_file_format),
  m_quality(s_def_quality),
  m_sequence(SGAL_FALSE),
  m_count(0),
  m_triggered(SGAL_FALSE),
  m_use_front_buffer(SGAL_TRUE),
  m_size(0),
  m_flip(SGAL_TRUE)
{
}

/*! Destructor */
Snapshot::~Snapshot()
{
  Uchar * pixels = (Uchar *) m_image->get_pixels();
  if (pixels) delete [] pixels;
  m_image->set_pixels(NULL);
  m_image->set_width(0);
  m_image->set_height(0);
  m_size = 0;
}

/*! Take a snapshot and write to a file if triggered
 */
Action::Trav_directive Snapshot::draw(Draw_action * draw_action) 
{
  if (!m_triggered) return Action::TRAV_CONT;
  m_triggered = SGAL_FALSE;

  m_use_front_buffer = draw_action->is_snap_from_front();
  if (!allocate_space(draw_action)) return Action::TRAV_CONT;
  take_snapshot();
  write_image();
  return Action::TRAV_CONT; 
}

/*! Allocate space for the image */
Boolean Snapshot::allocate_space(Draw_action * action)
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();

  if ((width == 0) || (height == 0)) {
    Context * context = action->get_context();
    if (!context) return SGAL_FALSE;
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

  if (width == 0 || height == 0) return SGAL_FALSE;
  Uint size = Image::get_size(width, height, format);
  if (size > m_size) {
    Uchar * pixels = (Uchar *) m_image->get_pixels();
    if (pixels) delete [] pixels;
    pixels = new Uchar[size];
    if (!pixels) return SGAL_FALSE;
    m_image->set_pixels(pixels);
    m_size = size;
  }
  return SGAL_TRUE;
}

/*! Take a snapshot of the window */
void Snapshot::take_snapshot()
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  void * pixels = m_image->get_pixels();
  GLenum gl_format = m_image->get_format_format(format);
  GLenum gl_type = m_image->get_format_type(format);

  GLint val;
  glGetIntegerv(GL_READ_BUFFER, &val);
  GLenum read_buffer_mode = (GLenum)val;
  (m_use_front_buffer) ? glReadBuffer(GL_FRONT) : glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, gl_format, gl_type, pixels);
  glReadBuffer(read_buffer_mode);
}

/*! Write the image into a file */
void Snapshot::write_image()
{
  std::string file_name = m_dir_name + "/" + m_file_name;
  if (m_sequence) {
    std::ostringstream oss;
    oss << m_count++;
    file_name += "_" + oss.str();
  }
  file_name += std::string(".") + s_file_format_names[m_file_format];

  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Uchar * pixels = static_cast<Uchar *>(m_image->get_pixels());
  Magick::Image image(width, height, "RGB", Magick::CharPixel, pixels);
  if (m_flip) image.flip();
  image.magick(s_file_format_names[m_file_format]);
  image.write(file_name); 
}

/*! Initialize the container prototype */
void Snapshot::init_prototype()
{
  if (s_prototype)  return;
  s_prototype = new Container_proto(Node::get_prototype());

  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          get_member_offset(&m_triggered)));
  
  s_prototype->add_field_info(new SF_string(DIR_NAME, "dirName",
                                            get_member_offset(&m_dir_name)));

  s_prototype->add_field_info(new SF_string(FILE_NAME, "fileName",
                                            get_member_offset(&m_file_name)));
  
  s_prototype->add_field_info(new SF_int(FILE_FORMAT, "fileFormat",
                                         get_member_offset(&m_file_format)));
}

/*! Delete the container prototype */
void Snapshot::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the container prototype */
Container_proto * Snapshot::get_prototype() 
{  
  if (!s_prototype) Snapshot::init_prototype();
  return s_prototype;
}

/*! Set the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Snapshot::set_attributes(Element * elem)
{
  Node::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "dirName") {
      m_dir_name = strip_double_quotes(value);
      if (m_dir_name.empty()) m_dir_name = s_def_dir_name;
      Uint n = m_dir_name.size() - 1;
      if (m_dir_name[n] == '/') m_dir_name.resize(n);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileName") {
      m_file_name = strip_double_quotes(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fileFormat") {
      std::string str = strip_double_quotes(value);
      Uint i;
      for (i = 0; i < FF_num; ++i) {
        if (str == s_file_format_names[i]) {
          m_file_format = (File_format) i;
          break;
        }
      }
      if (i == FF_num) {
        std::cerr << "Illegal file format (" << value.c_str() << ")!"
                  << std::endl;
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      m_quality = atoi(value.c_str());
      elem->mark_delete(ai);
      continue;
     }
    if (name == "sequence") {
      m_sequence = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string & name = elem->get_name(cai);
    Container * cont = elem->get_value(cai);
    if (name == "image") {
      m_image = dynamic_cast<Image*>(cont);
      elem->mark_delete(cai);
      continue;
    }
  }
  
  // Remove all the marked attributes:
  elem->delete_marked();
}

/*! \brief adds the container to a given scene */  
void Snapshot::add_to_scene(Scene_graph * sg)
{
  sg->add_snaphot(this);
}

#if 0
/*!
 */
Attribute_list Snapshot::get_attributes()
{
  Attribute_list attribs; 
  Attribue attrib;

  attribs = Node::get_attributes();

  if ((m_fileName != s_def_file_name)) {
    attrib.first = "fileName";
    attrib.second = get_file_name();
    attribs.push_back(attrib);
  }

  return attribs; 
}

#endif

SGAL_END_NAMESPACE
