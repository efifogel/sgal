// Copyright (c) 2013 Israel.
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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Texture.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const Texture::Min_filter Texture::m_def_min_filter = Texture::LINEAR_MIN;
const Texture::Mag_filter Texture::m_def_mag_filter = Texture::LINEAR_MAG;

const Texture::Wrap Texture::m_def_wrap_s = Texture::REPEAT;
const Texture::Wrap Texture::m_def_wrap_t = Texture::REPEAT;
const Texture::Wrap Texture::m_def_wrap_r = Texture::REPEAT;

Container_proto* Texture::s_prototype = NULL;

const char* Texture::s_min_filter_names[] = {
  "nearest",                  // NEAREST_MIN, 
  "linear",                   // LINEAR_MIN,
  "mipmapNearest",            // NEAREST_MIPMAP_NEAREST, 
  "mipmapLinear",             // NEAREST_MIPMAP_LINEAR, 
  "linearMipmapLinear"        // LINEAR_MIPMAP_LINEAR
};

const char* Texture::s_mag_filter_names[] = {
  "nearest",                  // NEAREST_MAG, 
  "linear"                    // LINEAR_MAG
};

const char* Texture::s_wrap_names[] = {
  "clamp",                    // CLAMP, 
  "repeat",                   // REPEAT,
  "clampToEdge"               // CLAMP_TO_EDGE
};

const GLenum Texture::s_targets[] = {
  GL_TEXTURE_1D,
  GL_TEXTURE_2D,
  GL_TEXTURE_3D,
  GL_TEXTURE_1D_ARRAY,
  GL_TEXTURE_2D_ARRAY,
  GL_TEXTURE_RECTANGLE,
  GL_TEXTURE_CUBE_MAP,
  GL_TEXTURE_CUBE_MAP_ARRAY,
  GL_TEXTURE_BUFFER,
  GL_TEXTURE_2D_MULTISAMPLE,
  GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

const GLenum Texture::s_wrap_tokens[] = {
  GL_CLAMP,
  GL_REPEAT,
  GL_CLAMP_TO_EDGE
};

const GLenum Texture::s_min_filter_tokens[] = {
  GL_NEAREST,
  GL_LINEAR,
  GL_NEAREST_MIPMAP_NEAREST,
  GL_LINEAR_MIPMAP_NEAREST,
  GL_NEAREST_MIPMAP_LINEAR,
  GL_LINEAR_MIPMAP_LINEAR
};

const GLenum Texture::s_mag_filter_tokens[] = {GL_NEAREST, GL_LINEAR};

/*! Constructor */
Texture::Texture(Boolean proto) :
  Container(proto),
  m_target(TEXTURE_2D),
  m_id(0xffffffff),
  m_wrap_s(m_def_wrap_s),
  m_wrap_t(m_def_wrap_t),
  m_wrap_r(m_def_wrap_r),
  m_min_filter(m_def_min_filter),
  m_mag_filter(m_def_mag_filter),
  m_dirty(true)
{ }

/*! Destructor */
Texture::~Texture() {}

/*! \brief sets the wrapping factor on the S. */
void Texture::wrap_s_changed(Field_info* /* field_info */)
{
  set_wrap_s(m_repeat_s);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the wrapping factor on the T. */
void Texture::wrap_t_changed(Field_info* /* field_info */)
{
  set_wrap_t(m_repeat_t);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the wrapping factor on the Q. */
void Texture::wrap_r_changed(Field_info* /* field_info */)
{
  set_wrap_r(m_repeat_r);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the minimization filter. */
void Texture::set_min_filter(const std::string& value)
{
  unsigned int i;
  for (i = 0; i < sizeof(s_min_filter_names)/sizeof(char *); ++i)
    if (value == s_min_filter_names[i]) break;
  if (i < sizeof(s_min_filter_names)/sizeof(char *))
    set_min_filter((Min_filter) i);
}

/*! \brief notifies that the minimization filter has changed. */
void Texture::min_filter_changed(Field_info* /* field_info */)
{
  set_min_filter(m_min_filter_str);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the magnification filter. */
void Texture::set_mag_filter(const std::string& value)
{
  unsigned int i;
  for ( i = 0; i < sizeof(s_mag_filter_names)/sizeof(char *); ++i)
    if (value == s_mag_filter_names[i]) break;
  if (i < sizeof(s_mag_filter_names)/sizeof(char *))
    set_mag_filter((Mag_filter) i);
}

/*! \brief notifies that the magnification filter has changed. */
void Texture::mag_filter_changed(Field_info* /* field_info */)
{
  set_mag_filter(m_mag_filter_str);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief draws the texture. */
void Texture::draw(Context* ctx)
{
  if (m_dirty) {
    clean();
    return;
  }
  glBindTexture(s_targets[m_target], m_id);
}

/*! \brief cleans the object using the new decoded data. */
void Texture::clean()
{
  glGenTextures(1, &m_id);
  GLenum target = s_targets[m_target];
  glBindTexture(s_targets[m_target], m_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
                  s_mag_filter_tokens[m_mag_filter]);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
                  s_min_filter_tokens[m_min_filter]);
  switch (m_target) {
   case TEXTURE_3D:
   case TEXTURE_CUBE_MAP:
   case TEXTURE_CUBE_MAP_ARRAY:
   case TEXTURE_BUFFER:
    glTexParameteri(target, GL_TEXTURE_WRAP_R, s_wrap_tokens[m_wrap_r]);

   case TEXTURE_2D:
   case TEXTURE_2D_ARRAY:
   case TEXTURE_RECTANGLE:
   case TEXTURE_2D_MULTISAMPLE:
   case TEXTURE_2D_MULTISAMPLE_ARRAY:
    glTexParameteri(target, GL_TEXTURE_WRAP_T, s_wrap_tokens[m_wrap_t]);

   case TEXTURE_1D:
   case TEXTURE_1D_ARRAY:
    glTexParameteri(target, GL_TEXTURE_WRAP_S, s_wrap_tokens[m_wrap_s]);
  }
  m_dirty = false;
}

/*! \brief sets the attributes of the texture. */
void Texture::set_attributes(Element* elem) 
{
  typedef Element::Str_attr_iter        Str_attr_iter;
  
  Container::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "repeatS") {
      set_wrap_s(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "repeatT") {
      set_wrap_t(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "minFilter") {
      set_min_filter(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "magFilter") {
      set_mag_filter(value);
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*!
 */
Attribute_list Texture::get_attributes() 
{ 
  Attribute_list attrs; 
  Attribue attrib;
  char buf[32];

  attrs = Container::get_attributes();

  if (m_wrap_s != s_def_wrap_s) {
    attrib.first = "repeatS";
    attrib.second = (m_wrap_s == CLAMP) ? "FALSE" : "TRUE";
    attrs.push_back(attrib);
  }
  if (m_wrap_t != s_def_wrap_t) {
    attrib.first = "repeatT";
    attrib.second = (m_wrap_t == CLAMP) ? "FALSE" : "TRUE";
    attrs.push_back(attrib);
  }
  if (m_min_filter != m_defMinFilter) {
    attrib.first = "minFilter";
    sprintf(buf, "%s", m_min_filterNames[m_min_filter]);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_mag_filter != m_defMagFilter) {
    attrib.first = "magFilter";
    sprintf(buf, "%s", m_mag_filterNames[m_mag_filter]);
    attrib.second = buf;
    attrs.push_back(attrib);
  }

  return attrs; 
}

/*! add a material to the material pool. connect the material to
 * its parent appearance.
 */
void Texture::AddToScene(Scene_graph* sg, XML_entity* parent)
{
    Container::AddToScene(sg, parent);
    sg->AddContainer(this);
    Appearance* appearance = dynamic_cast<Appearance *>(parent);
    if (appearance) {
      // set the texture but do not set the bits yet
      appearance->set_Texture(this);
      appearance->set_TexEnable(true);
    }

    Environment_Map* env = dynamic_cast<Environment_Map *>(parent);
    if (env) {
        env->set_Texture(this);
        return;
    }

    /*
    Bump* bump = dynamic_cast<Bump *>(parent);
    if (bump) {
        bump->set_Texture(this);
        m_heightField = true;
        return;
    }
    */
}
#endif

/*! \brief initializes the node prototype. */
void Texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func;
  
  exec_func = static_cast<Execution_function>(&Texture::min_filter_changed);
  SF_int* min_filter_field_info = new SF_int(MIN_FILTER, "minFilter",
                                             get_member_offset(&m_min_filter),
                                             exec_func);
  s_prototype->add_field_info(min_filter_field_info);

  exec_func = static_cast<Execution_function>(&Texture::mag_filter_changed);
  SF_int* mag_filter_field_info = new SF_int(MAG_FILTER, "magFilter",
                                             get_member_offset(&m_mag_filter),
                                             exec_func);  
  s_prototype->add_field_info(mag_filter_field_info);

  exec_func = static_cast<Execution_function>(&Texture::wrap_s_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_S, "repeatS",
                                          get_member_offset(&m_repeat_s),
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Texture::wrap_t_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_T, "repeatT",
                                          get_member_offset(&m_repeat_t),
                                          exec_func));
}

/*! \brief deletes the prototype. */
void Texture::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the prototype. */
Container_proto* Texture::get_prototype() 
{  
  if (!s_prototype) Texture::init_prototype();
  return s_prototype;
}

/*! \brief downloads the image as a 2D color map to the graphics pipe. */
void Texture::load_color_map(Image* image, GLenum target)
{
  Uint width = image->get_width();
  Uint height = image->get_height();
  Image::Format format = image->get_format();
  
  GLint req_format = Image::get_format_format(format);
  GLint req_type = Image::get_format_type(format);

  // Uint components = Image::get_format_components(format);
  int border = (width & (width - 1)) ?
    (((width - 2) & (width - 3)) ? -1: 1) : 0;
  int height_border = (height & (height - 1)) ?
    (((height - 2) & (height - 3)) ? -1: 1) : 0;

  if (border == -1 || border != height_border) {
    ;
  }

  GLenum internal_format = Image::get_format_internal_format(format);
  
  if (m_min_filter >= NEAREST_MIPMAP_NEAREST) {
    int rc = gluBuild2DMipmaps(target, internal_format, width, height,
                               req_format, req_type, image->get_pixels());
    (void) rc;
    return;
    //! \todo download images
  }
  glTexImage2D(target, 0, internal_format, width, height, border,
               req_format, req_type, image->get_pixels());
}

SGAL_END_NAMESPACE
