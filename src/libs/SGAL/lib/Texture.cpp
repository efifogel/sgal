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
// $Revision: 12369 $
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

const Image::Format Texture::m_def_format = Image::kRGB8_8_8;
const Float Texture::m_def_scale = 1.0f;

const Texture::Min_filter Texture::m_def_min_filter = Texture::LINEAR_MIN;
const Texture::Mag_filter Texture::m_def_mag_filter = Texture::LINEAR_MAG;

const Texture::Wrap Texture::m_def_wraps = Texture::REPEAT;
const Texture::Wrap Texture::m_def_wrapt = Texture::REPEAT;

Container_proto* Texture::s_prototype = NULL;
std::string Texture::s_tag = "Texture";

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
  m_image(NULL),
  m_wraps(m_def_wraps),
  m_wrapt(m_def_wrapt),
  m_min_filter(m_def_min_filter),
  m_mag_filter(m_def_mag_filter),
  m_map_levels(0),
  m_dirty(true),
  m_height_field(false),
  m_scale(m_def_scale),
  m_text(0)
{ for (int i = 0; i < SGAL_MAX_LEVELS; ++i) m_normal_maps[i] = 0; }

/*! Destructor */
Texture::~Texture() {}

/*! \brief sets the wrapping factor on the S. */
void Texture::wraps_changed(Field_info* /* field_info */)
{
  set_wraps(m_repeats);
  m_dirty = true;
  set_rendering_required();
}

/*! \brief sets the wrapping factor on the T. */
void Texture::wrapt_changed(Field_info* /* field_info */)
{
  set_wrapt(m_repeatt);
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
  if (m_text) m_text->draw(ctx);

  if (m_dirty) {
    clean();
    return;
  }
  glBindTexture(s_targets[m_target], m_id);
}

/*! \brief transmits the texture to the graphics pipe. */
void Texture::load_color_map()
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  
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
    int rc = gluBuild2DMipmaps(s_targets[m_target], internal_format,
                               width, height,
                               req_format, req_type, m_image->get_pixels());
    (void) rc;
    return;
    //! \todo download images
  }
  glTexImage2D(s_targets[m_target], 0, internal_format, width, height, border,
               req_format, req_type, m_image->get_pixels());
}

/*! \brief converts the supplied height-field image into a normal map. */
void Texture::load_height_map()
{
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  int width_border = (width & (width - 1)) ?
    (((width - 1) & (width - 2)) ? -1: 1) : 0;
  if (width_border == -1) {
    ;
  }
  int height_border = (height & (height - 1)) ?
    (((height - 1) & (height - 2)) ? -1: 1) : 0;
  if (height_border == -1) {
    ;
  }
  Uint new_width = width - width_border;
  Uint new_height = height - height_border;
  int level = 0;
  convert_height_field_to_normal_map(level, new_width, new_height,
                                     width, height, m_scale);

  // Load original maximum resolution normal map:

  // The BGRA color component ordering is fastest for NVIDIA:
  glTexImage2D(s_targets[m_target], level, GL_RGBA8,
               new_width, new_height, level,
               GL_BGRA_EXT, GL_UNSIGNED_BYTE, &m_normal_maps[level]->nz);

  if (m_min_filter <= NEAREST_MIPMAP_NEAREST) return;
    
  // Downsample the normal map for mipmap levels down to 1x1:
  while (new_width > 1 || new_height > 1) {
    level++;

    // Half width and height but not beyond one:
    int nw = new_width >> 1;
    int nh = new_height >> 1;
    if (nw == 0) nw = 1;
    if (nh == 0) nh = 1;

    down_sample_normal_map(level-1, level, new_width, new_height, nw, nh);

    glTexImage2D(s_targets[m_target], level, GL_RGBA8, nw, nh, 0,
                 GL_BGRA_EXT, GL_UNSIGNED_BYTE, &m_normal_maps[level]->nz);

    // Make the new width and height the old width and height:
    new_width = nw;
    new_height = nh;
  }
}

/*! \brief creates a downsampled version of the normal map. */
void Texture::down_sample_normal_map(int src_id, int dst_id,
                                     int w2, int h2, int w, int /* h */)
{
  const float one_over_127 = 1.0f / 127.0f;
  const float one_over_255 = 1.0f / 255.0f;

  float x, y, z, l, invl;
  float mag00, mag01, mag10, mag11;
  int ii, jj;

  Normal_map* src = m_normal_maps[src_id];
  Normal_map* dst = m_normal_maps[dst_id];
  for (int i = 0; i < h2; i += 2) {
    for (int j = 0; j < w2; j += 2) {
      // The "%w2" and "%h2" modulo arithmetic makes sure that
      // Nx1 and 1xN normal map levels are handled correctly.

      // Fetch the magnitude of the four vectors to be downsampled:
      mag00 = one_over_255 * src[ (i)      *w2 +  (j)      ].mag;
      mag01 = one_over_255 * src[ (i)      *w2 + ((j+1)%h2)].mag;
      mag10 = one_over_255 * src[((i+1)%w2)*w2 +  (j)      ].mag;
      mag11 = one_over_255 * src[((i+1)%w2)*w2 + ((j+1)%h2)].mag;

      // Sum 2x2 footprint of red component scaled back to [-1,1] floating
      // point range:
      x =  mag00 * (one_over_127 * src[ (i)      *w2 +  (j)      ].nx - 1.0f);
      x += mag01 * (one_over_127 * src[ (i)      *w2 + ((j+1)%h2)].nx - 1.0f);
      x += mag10 * (one_over_127 * src[((i+1)%w2)*w2 +  (j)      ].nx - 1.0f);
      x += mag11 * (one_over_127 * src[((i+1)%w2)*w2 + ((j+1)%h2)].nx - 1.0f);

      // Sum 2x2 footprint of green component scaled back to [-1,1] floating
      // point range:
      y =  mag00 * (one_over_127 * src[ (i)      *w2 +  (j)      ].ny - 1.0f);
      y += mag01 * (one_over_127 * src[ (i)      *w2 + ((j+1)%h2)].ny - 1.0f);
      y += mag10 * (one_over_127 * src[((i+1)%w2)*w2 +  (j)      ].ny - 1.0f);
      y += mag11 * (one_over_127 * src[((i+1)%w2)*w2 + ((j+1)%h2)].ny - 1.0f);

      // Sum 2x2 footprint of blue component scaled back to [-1,1] floating
      // point range:
      z =  mag00 * (one_over_127 * src[ (i)      *w2 +  (j)      ].nz - 1.0f);
      z += mag01 * (one_over_127 * src[ (i)      *w2 + ((j+1)%h2)].nz - 1.0f);
      z += mag10 * (one_over_127 * src[((i+1)%w2)*w2 +  (j)      ].nz - 1.0f);
      z += mag11 * (one_over_127 * src[((i+1)%w2)*w2 + ((j+1)%h2)].nz - 1.0f);

      // Compute length of the (x,y,z) vector:
      l = sqrtf(x*x + y*y + z*z);
      if (l == 0.0) {
        // Ugh, a zero length vector.  Avoid division by zero and just
        // use the unpeturbed normal (0,0,1). */
        x = 0.0f;
        y = 0.0f;
        z = 1.0f;
      } else {
        // Normalize the vector to unit length:
        invl = 1.0f/l;
        x = x*invl;
        y = y*invl;
        z = z*invl;
      }

      ii = i >> 1;
      jj = j >> 1;

      // Pack the normalized vector into an RGB unsigned byte vector
      // in the downsampled image. */
      dst[ii*w+jj].nx = (GLubyte) (128 + 127 * x);
      dst[ii*w+jj].ny = (GLubyte) (128 + 127 * y);
      dst[ii*w+jj].nz = (GLubyte) (128 + 127 * z);

      // Store the magnitude of the average vector in the alpha
      // component so we keep track of the magntiude. */
      l = l/4.0f;
      if (l > 1.0) dst[ii*w+jj].mag = 255;
      else dst[ii*w+jj].mag = (GLubyte) (255 * l);
    }
  }
}

/*! \brief converts a height field image into a normal map. */
void Texture::convert_height_field_to_normal_map(Uint level, Uint w, Uint h,
                                                 Uint wr, Uint hr, Float scale)
{
  const float one_over_255 = 1.0f/255.0f;

  GLubyte* pixels = (GLubyte *) m_image->get_pixels();
  for (Uint i = 0; i < h; i++) {
    for (Uint j = 0; j < w; j++) {
      // Expand [0,255] texel values to the [0,1] range:
      float c = pixels[i*wr + j] * one_over_255;
      // Expand the texel to its right:
      float cx = pixels[i*wr + (j+1)%wr] * one_over_255;
      // Expand the texel one up:
      float cy = pixels[((i+1)%hr)*wr + j] * one_over_255;
      float dcx = scale * (c - cx);
      float dcy = scale * (c - cy);

      // Normalize the vector:
      float sqlen = dcx*dcx + dcy*dcy + 1;
      float reciplen = 1.0f/sqrtf(sqlen);
      float nx = dcy*reciplen;
      float ny = -dcx*reciplen;
      float nz = reciplen;

      // Repack the normalized vector into an RGB unsigned byte
      // vector in the normal map image:
      m_normal_maps[level][i*w+j].nx = (GLubyte) (128 + 127 * nx);
      m_normal_maps[level][i*w+j].ny = (GLubyte) (128 + 127 * ny);
      m_normal_maps[level][i*w+j].nz = (GLubyte) (128 + 127 * nz);

      // The highest resolution mipmap level always has a
      // unit length magnitude:
      m_normal_maps[level][i*w+j].mag = 255;
    }
  }
}

#if 0
/*!
 */
void Texture::destroy_data()
{
  m_image_buffers[m_current_buffer]->delete_pixels();

  for (int i = 0; i < SGAL_MAX_LEVELS; i++) {
    if (m_normal_maps[i]) delete m_normal_maps[i];
  }

  m_current_buffer = 0;
  m_dirty = false;
  m_map_levels = 0;
}

/*!
 */
void Texture::allocate_memory()
{
  int width = atoi(m_decoder_image.get_value(Image_data::WIDTH).c_str());
  int height = atoi(m_decoder_image.get_value(Image_data::HEIGHT).c_str());
  int components =
    atoi(m_decoder_image.get_value(Image_data::COMPONENET_NUM).c_str());

  Image::Format format = Image::kIllegal;
  switch (components) {
   case 1: format = Image::kLuminance8; break;
   case 2: format = Image::kLuminanceAlpha8_8; break;
   case 3: format = Image::kRGB8_8_8; break;
   case 4: format = Image::kRGBA8_8_8_8; break;
  }
  
  m_image_buffers[0]->set_width(width);
  m_image_buffers[0]->set_height(height);
  m_image_buffers[0]->set_format(format);
  m_image_buffers[0]->allocate_memory();

  m_image_buffers[1]->set_width(width);
  m_image_buffers[1]->set_height(height);
  m_image_buffers[1]->set_format(format);
  m_image_buffers[1]->allocate_memory();

  if (m_height_field) while (width > 1 || height > 1) {
    if (m_maplevels >= SGAL_MAX_LEVELS) break;
    // Half width and height but not beyond one:
    int nw = width >> 1;
    int nh = height >> 1;
    if (nw == 0) nw = 1;
    if (nh == 0) nh = 1;
    m_normal_maps[m_maplevels] = new Normal_map[width * height];
    m_map_levels++;
  }
    
  m_decoder_image.set_pixels(m_image_buffers[m_current_buffer]->get_pixels());
  m_current_buffer = (m_current_buffer == ZERO ? ONE : ZERO);
  m_current_image->set_pixels(m_image_buffers[m_current_buffer]->get_pixels());
  m_current_image->set_format(format);
}
#endif

/*! \brief cleans the object using the new decoded data. */
void Texture::clean()
{
  if (m_image->is_dirty()) m_image->clean();
      
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

  (m_height_field) ? load_height_map() : load_color_map();

  if (m_text) m_text->clean();
  m_dirty = false;
}

/* \brief */
Boolean Texture::is_equal(const Texture* t) const
{
  if (m_wraps != t->m_wraps) return false;
  if (m_wrapt != t->m_wrapt) return false;
  if (m_min_filter != t->m_min_filter) return false;
  if (m_mag_filter != t->m_mag_filter) return false;

#if 0
  if ((m_text && !t->m_text) || (!m_text && t->m_text)) return false;
  if (m_text)
    if (!m_text->is_equal(t->m_text)) 
      return true;
#endif
  
  return true;
}

/* \brief */
Boolean Texture::empty()
{
  return ((get_width() == 0) || (get_height() == 0) || (get_pixels() == NULL));
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
      set_wraps(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "repeatT") {
      set_wrapt(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scale") {
      m_scale = atoff(value.c_str());
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
  if (m_scale != m_defScale) {
    attrib.first = "sgalScale";
    sprintf(buf, "%.2f", m_scale);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_min_filter != m_defMinFilter) {
    attrib.first = "sgalMinFilter";
    sprintf(buf, "%s", m_min_filterNames[m_min_filter]);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_mag_filter != m_defMagFilter) {
    attrib.first = "sgalMagFilter";
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

/*! Initializes the node prototype */
void Texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  //! \todo why use the _str and not the attribute itself?
  Execution_function exec_func =
    static_cast<Execution_function>(&Texture::min_filter_changed);
  SF_string* min_filter_field_info =
    new SF_string(MIN_FILTER, "minFilter",
                  get_member_offset(&m_min_filter_str),
                  exec_func);
  s_prototype->add_field_info(min_filter_field_info);

  exec_func =
    static_cast<Execution_function>(&Texture::mag_filter_changed);
  SF_string* mag_filter_field_info =
    new SF_string(MAG_FILTER, "magFilter",
                  get_member_offset(&m_mag_filter_str),
                  exec_func);  
  s_prototype->add_field_info(mag_filter_field_info);

  exec_func = static_cast<Execution_function>(&Texture::wraps_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_S, "repeatS",
                                          get_member_offset(&m_repeats),
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Texture::wrapt_changed);
  s_prototype->add_field_info(new SF_bool(REPEAT_T, "repeatT",
                                          get_member_offset(&m_repeatt),
                                          exec_func));
}

/*! */
void Texture::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Texture::get_prototype() 
{  
  if (!s_prototype) Texture::init_prototype();
  return s_prototype;
}

/*!
 */
Boolean Texture::attach_context(Context* context)
{
  Boolean result = Container::attach_context (context);
  return result;
}

/*!
 */
Boolean Texture::detach_context(Context* context)
{
  Boolean result = Container::detach_context(context);
  // next draw with another context - recreate the texture
  m_dirty = true;
  return result;
}

/*! Print texture information */
void Texture::print_info()
{
  int width = get_width();
  int height = get_height();
  Image::Format format = get_format();
  unsigned int size = Image::get_size(width, height, format);
  std::cout << "Texture size: " << size << std::endl;
}

SGAL_END_NAMESPACE
