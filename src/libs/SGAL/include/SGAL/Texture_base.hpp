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

#ifndef SGAL_TEXTURE_BASE_HPP
#define SGAL_TEXTURE_BASE_HPP

#include <string>

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

SGAL_BEGIN_NAMESPACE

class Image;

class SGAL_CLASSDEF Texture_base : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    REPEAT_S,
    REPEAT_T,
    REPEAT_R,
    MIN_FILTER,
    MAG_FILTER,
    LAST
  };

  /*! Constructor */
  Texture_base(Boolean proto = false);

  /*! Destructor */
  virtual ~Texture_base();

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
  void set_target(Target taregt);

  /*! Obtain the texture target. */
  Target get_target() const;
  
  /*! Set the wrapping factor on the S. */
  void set_wrap_s(Texture_base::Wrap wrap);

  /*! Set the wrapping factor on the S. */
  void set_wrap_s(Boolean flag);

  /*! Obtain the wrapping factor on the S. */
  Texture_base::Wrap get_wrap_s() const;

  /*! Notify that the wrapping factor on the S changed. */
  void wrap_s_changed(Field_info* field_info);

  /*! Set the wrapping factor on the T. */
  void set_wrap_t(Texture_base::Wrap wrap);
  
  /*! Set the wrapping factor on the T. */
  void set_wrap_t(Boolean flag);
  
  /*! Obtain the wrapping factor on the T. */
  Texture_base::Wrap get_wrap_t() const;

  /*! Notify that the wrapping factor on the T changed. */  
  void wrap_t_changed(Field_info* field_info);
  
  /*! Set the wrapping factor on the R. */
  void set_wrap_r(Texture_base::Wrap wrap);
  
  /*! Set the wrapping factor on the R. */
  void set_wrap_r(Boolean flag);
  
  /*! Obtain the wrapping factor on the R. */
  Texture_base::Wrap get_wrap_r() const;
  
  /*! Notify that the wrapping factor on the R changed. */  
  void wrap_r_changed(Field_info* field_info);
  
  /*! Set the minimization filter. */
  void set_min_filter(Texture_base::Min_filter min_filter);

  /*! Set the minimization filter. */
  void set_min_filter(const std::string& filter);

  /*! Set the minimization filter. */
  void min_filter_changed(Field_info* field_info);

  /*! Obtain the minimization filter. */
  Texture_base::Min_filter get_min_filter() const;

  /*! Sets the magnification filter. */
  void set_mag_filter(Texture_base::Mag_filter mag_filter);

  /*! Set the magnification filter. */
  void set_mag_filter(const std::string& filter);

  /*! Set the magnification filter. */
  void mag_filter_changed(Field_info* field_info);

  /*! Obtain the magnification filter. */
  Texture_base::Mag_filter get_mag_filter() const;

  /*! Draw the texture. */
  virtual void draw(Context* ctx);

  /*! Determine whether the object is dirty, and thus needs cleaning. */
  Boolean is_dirty() const;

  /*! Clean the object. */
  virtual void clean();

protected:
  /*! The texture target index. */
  Target m_target;
  
  /*! A unique id for the texture (used in OpenGL for texture binding) */
  Uint m_id;

  /*! Repeating in the S direction. */
  Wrap m_wrap_s;

  /*! Specify how the texture wrap in the S direction. If repeat_s is true
   * (the default), the texture map is repeated outside the [0.0, 1.0] texture
   * coordinate range in the S direction so that it fills the shape. If 
   * repeat_s is false, the texture coordinates are clamped in the S direction
   * to lie within the [0.0, 1.0] range.
   */
  Boolean m_repeat_s;

  /*! Repeating in the T direction. */
  Wrap m_wrap_t;

  /*! Specify how the texture wrap in the T direction. If repeat_t is true
   * (the default), the texture map is repeated outside the [0.0, 1.0] texture
   * coordinate range in the T direction so that it fills the shape. If
   * repeat_t is false, the texture coordinates are clamped in the T direction
   * to lie within the [0.0, 1.0] range.
   */
  Boolean m_repeat_t;

  /*! Repeating in the R parameter. */
  Wrap m_wrap_r;

  /*! Specify how the texture wrap in the R direction. If repeat_r is true
   * (the default), the texture map is repeated outside the [0.0, 1.0] texture
   * coordinate range in the R direction so that it fills the shape. If 
   * repeat_r is false, the texture coordinates are clamped in the R direction 
   * to lie within the [0.0, 1.0] range.
   */
  Boolean m_repeat_r;

  /*! The minification filtering method. */
  Min_filter m_min_filter;

  /*! The minification filtering method name. */
  std::string m_min_filter_str;

  /*! The magnification filtering method. */
  Mag_filter m_mag_filter;

  /*! The magnification filtering method name. */
  std::string m_mag_filter_str;

  /*! Determine whether the object is dirty, and thus needs cleaning. */
  Boolean m_dirty;

  // Default values:
  static const Min_filter m_def_min_filter;
  static const Mag_filter m_def_mag_filter;
  static const Wrap m_def_wrap_s;
  static const Wrap m_def_wrap_t;
  static const Wrap m_def_wrap_r;

  /*! An array of possible texture targets tokens. */
  static const GLenum s_targets[];

  /*! An array of minimization filter names. */
  static const char* s_min_filter_names[5];

  /*! An array of maximization filter names. */
  static const char* s_mag_filter_names[2];

  /*! An array of wrap names. */
  static const char* s_wrap_names[3];
  
  /*! An array of minimization filter tokens. */
  static const GLenum s_min_filter_tokens[];

  /*! An array of maximization filter tokens. */
  static const GLenum s_mag_filter_tokens[];

  /*! An array of wrap tokens. */
  static const GLenum s_wrap_tokens[];

  /*! Download the image as a 2D color map to the graphics pipe. */
  void load_color_map(Image* image, GLenum target);
  
private:
  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief sets the texture target. */
inline void Texture_base::set_target(Target target) { m_target = target; }

/*! \brief obtains the texture target. */
inline Texture_base::Target Texture_base::get_target() const { return m_target; }

/*! \brief sets the wrapping factor on the S. */
inline void Texture_base::set_wrap_s(Texture_base::Wrap wrap)
{ m_wrap_s = wrap; }

/*! \brief sets the wrapping factor on the S. */
inline void Texture_base::set_wrap_s(Boolean flag)
{ flag ? set_wrap_s(REPEAT) : set_wrap_s(CLAMP); }

/*! \brief obtains the wrapping factor on the S. */
inline Texture_base::Wrap Texture_base::get_wrap_s() const { return m_wrap_s; }

/*! \brief sets the wrapping factor on the T. */
inline void Texture_base::set_wrap_t(Texture_base::Wrap wrap)
{ m_wrap_t = wrap; }
  
/*! \brief sets the wrapping factor on the T. */
inline void Texture_base::set_wrap_t(Boolean flag)
{ flag ? set_wrap_t(REPEAT) : set_wrap_t(CLAMP); }
  
/*! \brief obtains the wrapping factor on the T. */
inline Texture_base::Wrap Texture_base::get_wrap_t() const { return m_wrap_t; }

/*! \brief sets the wrapping factor on the R. */
inline void Texture_base::set_wrap_r(Texture_base::Wrap wrap)
{ m_wrap_r = wrap; }
  
/*! \brief sets the wrapping factor on the R. */
inline void Texture_base::set_wrap_r(Boolean flag)
{ flag ? set_wrap_r(REPEAT) : set_wrap_r(CLAMP); }
  
/*! \brief obtains the wrapping factor on the R. */
inline Texture_base::Wrap Texture_base::get_wrap_r() const { return m_wrap_r; }

/*! \brief sets the minimization filter. */
inline void Texture_base::set_min_filter(Texture_base::Min_filter min_filter)
{ m_min_filter = min_filter; }

/*! \brief obtains the minimization filter. */
inline Texture_base::Min_filter Texture_base::get_min_filter() const
{ return m_min_filter; }

/*! \brief sets the manification filter. */
inline void Texture_base::set_mag_filter(Texture_base::Mag_filter mag_filter)
{ m_mag_filter = mag_filter; }

/*! \brief obtains the magnification filter. */
inline Texture_base::Mag_filter Texture_base::get_mag_filter() const
{ return m_mag_filter; }

/*! \brief determines whether the object is dirty, and thus needs cleaning. */
inline Boolean Texture_base::is_dirty() const { return m_dirty; }

SGAL_END_NAMESPACE

#endif
