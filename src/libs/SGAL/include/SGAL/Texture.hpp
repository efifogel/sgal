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
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TEXTURE_HPP
#define SGAL_TEXTURE_HPP

/*! \file
 * Base class for texture mapping.
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
    IMAGE_LEVEL,
    FORMAT,
    REPEAT_S,
    REPEAT_T,
    MIN_FILTER,
    MAG_FILTER,
    LAST
  };

  /*! Constructor */
  Texture(Boolean proto = false);

  /*! Destructor */
  virtual ~Texture();

  /*! Construct the prototype. */
  static Texture* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype(); 
  
  /*! Set the attributes of the texture extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  enum Min_filter {
    NEAREST_MIN = 0, 
    LINEAR_MIN,
    NEAREST_MIPMAP_NEAREST, 
    NEAREST_MIPMAP_LINEAR, 
    LINEAR_MIPMAP_LINEAR
  };

  enum Mag_filter {
    NEAREST_MAG = 0, 
    LINEAR_MAG
  };

  enum Wrap {
    CLAMP = 0, 
    REPEAT,
    CLAMP_TO_EDGE
  };

  enum Target {
    TEXTURE_1D = 0,
    TEXTURE_2D,
    TEXTURE_3D,
    TEXTURE_1D_ARRAY,
    TEXTURE_2D_ARRAY,
    TEXTURE_RECTANGLE,
    TEXTURE_CUBE_MAP,
    TEXTURE_CUBE_MAP_ARRAY,
    TEXTURE_BUFFER,
    TEXTURE_2D_MULTISAMPLE,
    TEXTURE_2D_MULTISAMPLE_ARRAY
  };

  /*! Set the texture target. */
  void set_target(Uint taregt);

  /*! Obtain the texture target. */
  Uint get_target() const;
  
  /*! Obtain the texture image. */
  Image* get_image() const;

  /*! Set the texture image. */
  void set_image(Image* image);
  
  /*! Obtain the texture format. */
  Image::Format get_format() const;

  /*! Obtain the texture width. */
  Int get_width() const;

  /*! Obtain the texture height. */
  Int get_height() const;

  /*! Obtain the texture pixel-data. */
  void* get_pixels() const;
  
  /*! Obtain the texture number of components. */
  int get_component_count() const;

  /*! Set the wrapping factor on the S. */
  void set_wraps(Texture::Wrap wraps);

  /*! Set the wrapping factor on the S. */
  void set_wraps(Boolean flag);

  /*! Notify that the wrapping factor on the S changed. */
  void wraps_changed(Field_info* field_info);

  /*! Obtain the wrapping factor on the S. */
  Texture::Wrap get_wraps() const;

  /*! Set the wrapping factor on the T. */
  void set_wrapt(Texture::Wrap wrapt);
  
  /*! Set the wrapping factor on the T. */
  void set_wrapt(Boolean flag);
  
  /*! Notify that the wrapping factor on the T changed. */  
  void wrapt_changed(Field_info* field_info);
  
  /*! Obtain the wrapping factor on the T. */
  Texture::Wrap get_wrapt() const;

  /*! Set the minimization filter. */
  void set_min_filter(Texture::Min_filter min_filter);

  /*! Set the minimization filter. */
  void set_min_filter(const std::string& filter);

  /*! Set the minimization filter. */
  void min_filter_changed(Field_info* field_info);

  /*! Obtain the minimization filter. */
  Texture::Min_filter get_min_filter() const;

  /*! Sets the magnification filter. */
  void set_mag_filter(Texture::Mag_filter mag_filter);

  /*! Set the magnification filter. */
  void set_mag_filter(const std::string& filter);

  /*! Set the magnification filter. */
  void mag_filter_changed(Field_info* field_info);

  /*! Obtain the magnification filter. */
  Texture::Mag_filter get_mag_filter() const;

  /*! Draw the texture. */
  virtual void draw(Context* ctx);

  Boolean is_dirty() const;

  void set_dirty();

  virtual void convert();

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  void set_text(Text_texture* text);

  Text_texture* get_text() const;

  Boolean is_equal(const Texture* tex) const;

  Boolean empty();
  
  virtual Boolean attach_context(Context* context);
  virtual Boolean detach_context(Context* context = 0);

  /*! Prin texture information */
  void print_info();

protected:

  /*! The texture target index. */
  Uint m_target;
  
  /*! a unique id for the texture (used in OpenGL for texture binding) */
  Uint m_id;

  /*! The texture pixels */
  Image* m_image;
  
  /*! Repeating in S parameter */
  Wrap m_wraps;

  Boolean m_repeats;

  /*! Repeating in T parameter */
  Wrap m_wrapt;

  Boolean m_repeatt;

  /*! The minification filtering method */
  Min_filter m_min_filter;

  std::string m_min_filter_str;

  /*! The magnification filtering method */
  Mag_filter m_mag_filter;

  std::string m_mag_filter_str;

#define SGAL_MAX_LEVELS 10

  Normal_map* m_normal_maps[SGAL_MAX_LEVELS];

  Int m_map_levels;

  /*! true after a call to clean. */
  Boolean m_dirty;

  /*! */
  Boolean m_height_field;

  /*! */
  Float m_scale;

  /*! */
  Text_texture* m_text;

  // Default values:
  static const Image::Format m_def_format;
  static const Float m_def_scale;
  static const Min_filter m_def_min_filter;
  static const Mag_filter m_def_mag_filter;
  static const Wrap m_def_wraps;
  static const Wrap m_def_wrapt;

  static const GLenum s_targets[];

  static const char* s_min_filter_names[5];

  static const char* s_mag_filter_names[2];

  static const char* s_wrap_names[3];
  
  static const GLenum s_min_filter_tokens[];

  static const GLenum s_mag_filter_tokens[];

  static const GLenum s_wrap_tokens[];

  /*! */
  void load_color_map();

  /*! Convert the supplied height-field image into a normal map (a normalized
   * vector compressed to the [0,1] range in RGB and A=1.0).  Load the
   * base texture level, then recursively downsample and load successive
   * normal map levels (being careful to expand, average, renormalize,
   * and unexpand each RGB value an also accumulate the average vector
   * shortening in alpha).
   */
  void load_height_map();

  /*! Convert a height field image into a normal map.  This involves
   * differencing each texel with its right and upper neighboor, then
   * normalizing the cross product of the two difference vectors.
   */
  void convert_height_field_to_normal_map(Uint level, Uint w, Uint h,
                                          Uint wr, Uint hr, Float scale);
  /*! Given a normal map, create a downsampled version of the normal map
   * at half the width and height.  Use a 2x2 box filter to create each
   * downsample.  gluBuild2DMipmaps is not suitable because each downsampled
   * texel must also be renormalized.
   */
  void down_sample_normal_map(int src_id, int dst_id,
                              int w2, int h2, int w, int h);

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Texture* Texture::prototype() { return new Texture(true); }

/*! \brief clones. */
inline Container* Texture::clone() { return new Texture(); }

/*! \brief sets the texture target. */
inline void Texture::set_target(Uint target) { m_target = target; }

/*! \brief obtains the texture target. */
inline Uint Texture::get_target() const { return m_target; }

/*! \brief obtains the texture image. */
inline Image* Texture::get_image() const { return m_image; }

/*! \brief sets the texture image. */
inline  void Texture::set_image(Image* image) { m_image = image; }
  
/*! \brief obtain the texture format. */
inline  Image::Format Texture::get_format() const
{ return m_image->get_format(); }

/*! \brief obtains the texture width. */
inline Int Texture::get_width() const { return m_image->get_width(); }

/*! \brief obtains the texture height. */
inline Int Texture::get_height() const { return m_image->get_height(); }

/*! \brief obtains the texture pixel-data. */
inline void* Texture::get_pixels() const { return m_image->get_pixels(); }
  
/*! \brief sets the wrapping factor on the S. */
inline void Texture::set_wraps(Texture::Wrap wraps) { m_wraps = wraps; }

/*! \brief sets the wrapping factor on the S. */
inline void Texture::set_wraps(Boolean flag)
{ flag ? set_wraps(REPEAT) : set_wraps(CLAMP); }

/*! \brief obtains the wrapping factor on the S. */
inline Texture::Wrap Texture::get_wraps() const { return m_wraps; }

/*! \brief sets the wrapping factor on the T. */
inline void Texture::set_wrapt(Texture::Wrap wrapt) { m_wrapt = wrapt; }
  
/*! \brief sets the wrapping factor on the T. */
inline void Texture::set_wrapt(Boolean flag)
{ flag ? set_wrapt(REPEAT) : set_wrapt(CLAMP); }
  
/*! \brief obtains the wrapping factor on the T. */
inline Texture::Wrap Texture::get_wrapt() const { return m_wrapt; }

/*! \brief sets the minimization filter. */
inline void Texture::set_min_filter(Texture::Min_filter min_filter)
{ m_min_filter = min_filter; }

/*! \brief obtains the minimization filter. */
inline Texture::Min_filter Texture::get_min_filter() const
{ return m_min_filter; }

/*! \brief sets the manification filter. */
inline void Texture::set_mag_filter(Texture::Mag_filter mag_filter)
{ m_mag_filter = mag_filter; }

/*! \brief obtains the magnification filter. */
inline Texture::Mag_filter Texture::get_mag_filter() const
{ return m_mag_filter; }

/*! \brief */
inline Boolean Texture::is_dirty() const { return m_dirty; }

/*! \brief */
inline void Texture::set_dirty() { m_dirty = true; }

/*! \brief */
inline void Texture::convert() {}

/*! \brief */
inline void Texture::set_text(Text_texture* text) { m_text = text; }

/*! \brief */
inline Text_texture* Texture::get_text() const { return m_text; }

/*! \brief obtains the texture number of components. */
inline int Texture::get_component_count() const
{ return Image::get_format_components(m_image->get_format()); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Texture::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
