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

#ifndef SGAL_TEXTURE_HPP
#define SGAL_TEXTURE_HPP

/*! \file
 * A class that implements a texture.
 *
 * Inherits from Container
 */

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Gfx.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Appearance;
class Text_texture;
class Element;
class Container_proto;

// Structure to encode a normal like an 8-bit unsigned BGRA vector:
typedef struct {
  // Normalized tangent space peturbed surface normal.  The
  // [0,1] range of (nx,ny,nz) gets expanded to the [-1,1]
  // range in the combiners.  The (nx,ny,nz) is always a
  // normalized vector.
  GLubyte nz, ny, nx;

  // A scaling factor for the normal.  Mipmap level 0 has a constant
  // magnitude of 1.0, but downsampled mipmap levels keep track of
  // the unnormalized vector sum length.  For diffuse per-pixel
  // lighting, it is preferable to make N' be the _unnormalized_
  // vector, but for specular lighting to work reasonably, the
  // normal vector should be normalized.  In the diffuse case, we
  // can multiply by the "mag" to get the possibly shortened unnormalized
  // length.
  GLubyte mag;

  // Why does "mag" make sense for diffuse lighting?
  //
  // Because sum(L dot Ni)/n == (L dot sum(Ni))/n 
  // 
  // Think about a bumpy diffuse surface in the distance.  It should
  // have a duller illumination than a flat diffuse surface in the
  // distance.
  
  // On NVIDIA GPUs, the RGB8 internal format is just as memory
  // efficient as the RGB8 internal texture format so keeping
  // "mag" around is just as cheap as not having it.

} Normal_map;

class SGAL_CLASSDEF Texture : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    FILE_NAME,
    IMAGE_LEVEL,
    FORMAT,
    REPEAT_S,
    REPEAT_T,
    MIN_FILTER,
    MAG_FILTER,
    LAST
  };

  /*! Constructor */
  Texture(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Texture();

  /*! Construct the prototype */
  static Texture * prototype() { return new Texture(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Texture(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto * get_prototype(); 
  
  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  enum Min_filter {
    NEAREST_MIN, 
    LINEAR_MIN,
    NEAREST_MIPMAP_NEAREST, 
    NEAREST_MIPMAP_LINEAR, 
    LINEAR_MIPMAP_LINEAR
  };

  enum Mag_filter {
    NEAREST_MAG, 
    LINEAR_MAG
  };

  enum Wrap {
    CLAMP, 
    REPEAT,
    CLAMP_TO_EDGE
  };

  /*! Obtain the texture image */
  Image * get_image() const { return m_image; }

  /*! Set the texture image */
  void set_image(Image * image) { m_image = image; }
  
  /*! Obtain the texture format */
  Image::Format get_format() { return m_image->get_format(); }

  /*! Obtain the texture width */
  Int get_width() { return m_image->get_width(); }

  /*! Obtain the texture height */
  Int get_height() { return m_image->get_height(); }

  /*! Obtain the texture pixel-data */
  void * get_pixels() { return m_image->get_pixels(); }
  
  /*! Obtain the texture number of components */
  int get_component_count();

  /*! Set the wrapping factor on the S */
  void set_wraps(Texture::Wrap wraps) { m_wraps = wraps; }

  /*! Set the wrapping factor on the S */
  void set_wraps(Boolean flag) { flag ? set_wraps(REPEAT) : set_wraps(CLAMP); }

  /*! Set the wrapping factor on the S */
  void wraps_changed(Field_info * field_info);

  /*! Obtain the wrapping factor on the S */
  Texture::Wrap get_wraps() const { return m_wraps; }

  /*! Set the wrapping factor on the T */
  void set_wrapt(Texture::Wrap wrapt) { m_wrapt = wrapt; }
  
  /*! Set the wrapping factor on the T */
  void set_wrapt(Boolean flag) { flag ? set_wrapt(REPEAT) : set_wrapt(CLAMP); }
  
  /*! Set the wrapping factor on the T */  
  void wrapt_changed(Field_info * field_info);
  
  /*! Obtain the wrapping factor on the T */
  Texture::Wrap get_wrapt() const { return m_wrapt; }

  /*! Set the min_filter */
  void set_min_filter(Texture::Min_filter min_filter);

  /*! Set the min_filter */
  void set_min_filter(const std::string & filter);

  /*! Set the min_filter */
  void min_filter_changed(Field_info * field_info);

  /*! Obtain the min_filter */
  Texture::Min_filter get_min_filter() const { return m_min_filter; }

  /*! Sets the mag_filter */
  void set_mag_filter(Texture::Mag_filter mag_filter);

  /*! Set the mag_filter */
  void set_mag_filter(const std::string & filter);

  /*! Set the mag_filter */
  void mag_filter_changed(Field_info * field_info);

  /*! Obtain the mag_filter */
  Texture::Mag_filter get_mag_filter() const { return m_mag_filter; }

  /*! Draw the texture */
  virtual void draw(Context * ctx);

  Boolean is_dirty() const { return m_dirty; }
  void set_dirty() { m_dirty = SGAL_TRUE; }

  virtual void convert() {}
  virtual void update();

  void set_text(Text_texture * text) { m_text = text; }
  Text_texture * get_text() const { return m_text; }

  Boolean is_equal(const Texture * tex) const;
  Boolean empty();
  
  virtual Boolean attach_context(Context * context);
  virtual Boolean detach_context(Context * context = 0);

  /*! Prin texture information */
  void print_info();

protected:
  /*! a unique id for the texture (used in OpenGL for texture binding) */
  Uint m_id;

  /*! The texture pixels */
  Image * m_image;
  
  /*! repeating in S parameter */
  Wrap m_wraps;
  Boolean m_repeats;

  /*! repeating in T parameter */
  Wrap m_wrapt;
  Boolean m_repeatt;

  /*! the minification filtering method */
  Min_filter m_min_filter;
  std::string m_min_filter_str;

  /*! the magnification filtering method */
  Mag_filter m_mag_filter;
  std::string m_mag_filter_str;

#define SGAL_MAX_LEVELS 10

  Normal_map * m_normal_maps[SGAL_MAX_LEVELS];

  Int m_map_levels;

  /*! SGAL_TRUE after a call to Update. */
  Boolean m_dirty;

  /*! */
  Boolean m_height_field;

  /*! */
  Float m_scale;

  /*! */
  Text_texture * m_text;

  // Default values:
  static const Image::Format m_def_format;
  static const Float m_def_scale;
  static const Min_filter m_def_min_filter;
  static const Mag_filter m_def_mag_filter;
  static const Wrap m_def_wraps;
  static const Wrap m_def_wrapt;

  static const char * m_min_filter_names[5];
  static const char * m_mag_filter_names[2];
  static const char * m_wrap_names[3];
  
  /*! */
  void load_color_map();

  /*! Convert the supplied height-field image into a normal map */
  void load_height_map();

  /*! Convert a height field image into a normal map */
  void convert_height_field_to_normal_map(Uint level, Uint w, Uint h,
                                          Uint wr, Uint hr, Float scale);
  /*! Create a downsampled version of the normal map */
  void down_sample_normal_map(int src_id, int dst_id,
                              int w2, int h2, int w, int h);

  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;
};

/*! Set the min_filter */
inline void Texture::set_min_filter(Texture::Min_filter min_filter)
{ m_min_filter = min_filter; }

/*! Sets the mag_filter */
inline void Texture::set_mag_filter(Texture::Mag_filter mag_filter)
{ m_mag_filter = mag_filter; }

/*! Obtain the texture number of components */
inline int Texture::get_component_count()
{ return Image::get_format_components(m_image->get_format()); }

SGAL_END_NAMESPACE

#endif
