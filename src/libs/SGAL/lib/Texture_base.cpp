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
#include "SGAL/Text_texture.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Gfx_conf.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const Texture_base::Min_filter Texture_base::m_def_min_filter =
  Texture_base::LINEAR_MIN;
const Texture_base::Mag_filter Texture_base::m_def_mag_filter =
  Texture_base::LINEAR_MAG;

const Texture_base::Wrap Texture_base::m_def_wraps = Texture_base::REPEAT;
const Texture_base::Wrap Texture_base::m_def_wrapt = Texture_base::REPEAT;

Container_proto* Texture_base::s_prototype = NULL;

const char* Texture_base::s_min_filter_names[] = {
  "nearest",                  // NEAREST_MIN, 
  "linear",                   // LINEAR_MIN,
  "mipmapNearest",            // NEAREST_MIPMAP_NEAREST, 
  "mipmapLinear",             // NEAREST_MIPMAP_LINEAR, 
  "linearMipmapLinear"        // LINEAR_MIPMAP_LINEAR
};

const char* Texture_base::s_mag_filter_names[] = {
  "nearest",                  // NEAREST_MAG, 
  "linear"                    // LINEAR_MAG
};

const char* Texture_base::s_wrap_names[] = {
  "clamp",                    // CLAMP, 
  "repeat",                   // REPEAT,
  "clampToEdge"               // CLAMP_TO_EDGE
};

const GLenum Texture_base::s_targets[] = {
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

const GLenum Texture_base::s_wrap_tokens[] = {
  GL_CLAMP,
  GL_REPEAT,
  GL_CLAMP_TO_EDGE
};

const GLenum Texture_base::s_min_filter_tokens[] = {
  GL_NEAREST,
  GL_LINEAR,
  GL_NEAREST_MIPMAP_NEAREST,
  GL_LINEAR_MIPMAP_NEAREST,
  GL_NEAREST_MIPMAP_LINEAR,
  GL_LINEAR_MIPMAP_LINEAR
};

const GLenum Texture_base::s_mag_filter_tokens[] = {GL_NEAREST, GL_LINEAR};

/*! Constructor */
Texture_base::Texture_base(Boolean proto) :
  Container(proto),
  m_target(TEXTURE_2D),
  m_id(0xffffffff),
  m_wraps(m_def_wraps),
  m_wrapt(m_def_wrapt),
  m_min_filter(m_def_min_filter),
  m_mag_filter(m_def_mag_filter),
  m_dirty(true)
{ }

/*! Destructor */
Texture_base::~Texture_base() {}

/*! \brief sets the wrapping factor on the S. */
void Texture_base::wraps_changed(Field_info* /* field_info */)
{
  set_wraps(m_repeats);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the wrapping factor on the T. */
void Texture_base::wrapt_changed(Field_info* /* field_info */)
{
  set_wrapt(m_repeatt);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the minimization filter. */
void Texture_base::set_min_filter(const std::string& value)
{
  unsigned int i;
  for (i = 0; i < sizeof(s_min_filter_names)/sizeof(char *); ++i)
    if (value == s_min_filter_names[i]) break;
  if (i < sizeof(s_min_filter_names)/sizeof(char *))
    set_min_filter((Min_filter) i);
}

/*! \brief notifies that the minimization filter has changed. */
void Texture_base::min_filter_changed(Field_info* /* field_info */)
{
  set_min_filter(m_min_filter_str);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the magnification filter. */
void Texture_base::set_mag_filter(const std::string& value)
{
  unsigned int i;
  for ( i = 0; i < sizeof(s_mag_filter_names)/sizeof(char *); ++i)
    if (value == s_mag_filter_names[i]) break;
  if (i < sizeof(s_mag_filter_names)/sizeof(char *))
    set_mag_filter((Mag_filter) i);
}

/*! \brief notifies that the magnification filter has changed. */
void Texture_base::mag_filter_changed(Field_info* /* field_info */)
{
  set_mag_filter(m_mag_filter_str);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief draws the texture. */
void Texture_base::draw(Context* ctx)
{
  if (m_dirty) {
    clean();
    return;
  }
  glBindTexture(s_targets[m_target], m_id);
}

/*! \brief cleans the object using the new decoded data. */
void Texture_base::clean()
{
  glGenTextures(1, &m_id);
  GLenum target = s_targets[m_target];
  glBindTexture(s_targets[m_target], m_id);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, s_wrap_tokens[m_wraps]);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, s_wrap_tokens[m_wrapt]);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER,
                  s_mag_filter_tokens[m_mag_filter]);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
                  s_min_filter_tokens[m_min_filter]);
  m_dirty = false;
}

/*! \brief sets the attributes of the texture. */
void Texture_base::set_attributes(Element* elem) 
{
  typedef Element::Str_attr_iter        Str_attr_iter;
  
  Container::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "repeatS") {
      set_wraps(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "repeatT") {
      set_wrapt(compare_to_true(value));
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
Attribute_list Texture_base::get_attributes() 
{ 
  Attribute_list attrs; 
  Attribue attrib;
  char buf[32];

  attrs = Container::get_attributes();

  if (m_wrapS != m_defWrapS) {
    attrib.first = "repeatS";
    attrib.second = (m_wrapS == CLAMP) ? "FALSE" : "TRUE";
    attrs.push_back(attrib);
  }
  if (m_wrapT != m_defWrapT) {
    attrib.first = "repeatT";
    attrib.second = (m_wrapT == CLAMP) ? "FALSE" : "TRUE";
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
void Texture_base::AddToScene(Scene_graph* sg, XML_entity* parent)
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

/*! Initializes the node prototype */
void Texture_base::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  //! \todo why use the _str and not the attribute itself?
  Execution_function exec_func =
    static_cast<Execution_function>(&Texture_base::min_filter_changed);
  SF_string* min_filter_field_info =
    new SF_string(MIN_FILTER, "minFilter",
                  get_member_offset(&m_min_filter_str),
                  exec_func);
  s_prototype->add_field_info(min_filter_field_info);

  exec_func =
    static_cast<Execution_function>(&Texture_base::mag_filter_changed);
  SF_string* mag_filter_field_info =
    new SF_string(MAG_FILTER, "magFilter",
                  get_member_offset(&m_mag_filter_str),
                  exec_func);  
  s_prototype->add_field_info(mag_filter_field_info);

  exec_func = static_cast<Execution_function>(&Texture_base::wraps_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_S, "repeatS",
                                          get_member_offset(&m_repeats),
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Texture_base::wrapt_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_T, "repeatT",
                                          get_member_offset(&m_repeatt),
                                          exec_func));
}

/*! */
void Texture_base::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Texture_base::get_prototype() 
{  
  if (!s_prototype) Texture_base::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
