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

class SGAL_CLASSDEF Texture_base : public Container {
public: 
  enum {
    FIRST = Container::LAST - 1,
    REPEAT_S,
    REPEAT_T,
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
  void set_target(Uint taregt);

  /*! Obtain the texture target. */
  Uint get_target() const;
  
  /*! Set the wrapping factor on the S. */
  void set_wraps(Texture_base::Wrap wraps);

  /*! Set the wrapping factor on the S. */
  void set_wraps(Boolean flag);

  /*! Notify that the wrapping factor on the S changed. */
  void wraps_changed(Field_info* field_info);

  /*! Obtain the wrapping factor on the S. */
  Texture_base::Wrap get_wraps() const;

  /*! Set the wrapping factor on the T. */
  void set_wrapt(Texture_base::Wrap wrapt);
  
  /*! Set the wrapping factor on the T. */
  void set_wrapt(Boolean flag);
  
  /*! Notify that the wrapping factor on the T changed. */  
  void wrapt_changed(Field_info* field_info);
  
  /*! Obtain the wrapping factor on the T. */
  Texture_base::Wrap get_wrapt() const;

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
  Uint m_target;
  
  /*! A unique id for the texture (used in OpenGL for texture binding) */
  Uint m_id;

  /*! Repeating in S parameter */
  Wrap m_wraps;

  /*! Specify how the texture wraps in the S direction. If repeats is true
   * (the default), the texture map is repeated outside the [0.0, 1.0] texture
   * coordinate range in the S direction so that it fills the shape. If repeats
   * is false, the texture coordinates are clamped in the S direction to lie
   * within the [0.0, 1.0] range.
   */
  Boolean m_repeats;

  /*! Repeating in T parameter */
  Wrap m_wrapt;

  /*! Specify how the texture wraps in the T direction. If repeatt is true
   * (the default), the texture map is repeated outside the [0.0, 1.0] texture
   * coordinate range in the S direction so that it fills the shape. If repeatt
   * is false, the texture coordinates are clamped in the T direction to lie
   * within the [0.0, 1.0] range.
   */
  Boolean m_repeatt;

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
  static const Wrap m_def_wraps;
  static const Wrap m_def_wrapt;

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

private:
  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief sets the texture target. */
inline void Texture_base::set_target(Uint target) { m_target = target; }

/*! \brief obtains the texture target. */
inline Uint Texture_base::get_target() const { return m_target; }

/*! \brief sets the wrapping factor on the S. */
inline void Texture_base::set_wraps(Texture_base::Wrap wraps)
{ m_wraps = wraps; }

/*! \brief sets the wrapping factor on the S. */
inline void Texture_base::set_wraps(Boolean flag)
{ flag ? set_wraps(REPEAT) : set_wraps(CLAMP); }

/*! \brief obtains the wrapping factor on the S. */
inline Texture_base::Wrap Texture_base::get_wraps() const { return m_wraps; }

/*! \brief sets the wrapping factor on the T. */
inline void Texture_base::set_wrapt(Texture_base::Wrap wrapt)
{ m_wrapt = wrapt; }
  
/*! \brief sets the wrapping factor on the T. */
inline void Texture_base::set_wrapt(Boolean flag)
{ flag ? set_wrapt(REPEAT) : set_wrapt(CLAMP); }
  
/*! \brief obtains the wrapping factor on the T. */
inline Texture_base::Wrap Texture_base::get_wrapt() const { return m_wrapt; }

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
