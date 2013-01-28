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
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Texture_base.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Appearance;
class Text_texture;
class Element;
class Container_proto;

class SGAL_CLASSDEF Texture : public Texture_base {
public: 
  enum {
    FIRST = Texture_base::LAST - 1,
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
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

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

  /*! Draw the texture. */
  virtual void draw(Context* ctx);

  /*! Determine whether the object is dirty, and thus needs cleaning. */
  Boolean is_dirty() const;

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  void set_text(Text_texture* text);

  Text_texture* get_text() const;

  Boolean empty();
  
  /*! Prin texture information */
  void print_info();

protected:
  /*! The texture pixels. */
  Image* m_image;

  /*! true after a call to clean. */
  Boolean m_dirty;

  /*! */
  Text_texture* m_text;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Texture* Texture::prototype() { return new Texture(true); }

/*! \brief clones. */
inline Container* Texture::clone() { return new Texture(); }

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
  
/*! \brief */
inline Boolean Texture::is_dirty() const { return m_dirty; }

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
