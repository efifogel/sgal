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

#ifndef SGAL_TEXTURE_2D_HPP
#define SGAL_TEXTURE_2D_HPP

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
#include "SGAL/Texture.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

class Context;
class Appearance;
#if 0
class Text_texture;
#endif
class Element;
class Container_proto;
class Scene_graph;

class SGAL_SGAL_DECL Texture_2d : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    IMAGE,
    LAST
  };

  /*! Constructor */
  Texture_2d(Boolean proto = false);

  /*! Destructor */
  virtual ~Texture_2d();

  /*! Construct the prototype. */
  static Texture_2d* prototype();

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

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

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

  /*! Draw the texture. */
  virtual void draw(Context* ctx);

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  /*! Determines whether the texture is empty. */
  virtual Boolean empty();

  /*! Obtain the texture number of components. */
  virtual Uint get_component_count() const;

#if 0
  void set_text(Text_texture* text);

  Text_texture* get_text() const;
#endif

  /*! Prin texture information */
  void print_info();

protected:
  /*! The texture pixels. */
  Image* m_image;

#if 0
  /*! */
  Text_texture* m_text;
#endif

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Texture_2d* Texture_2d::prototype() { return new Texture_2d(true); }

/*! \brief clones. */
inline Container* Texture_2d::clone() { return new Texture_2d(); }

/*! \brief obtains the texture image. */
inline Image* Texture_2d::get_image() const { return m_image; }

/*! \brief sets the texture image. */
inline  void Texture_2d::set_image(Image* image) { m_image = image; }

/*! \brief obtain the texture format. */
inline  Image::Format Texture_2d::get_format() const
{ return m_image->get_format(); }

/*! \brief obtains the texture width. */
inline Int Texture_2d::get_width() const { return m_image->get_width(); }

/*! \brief obtains the texture height. */
inline Int Texture_2d::get_height() const { return m_image->get_height(); }

/*! \brief obtains the texture pixel-data. */
inline void* Texture_2d::get_pixels() const { return m_image->get_pixels(); }

#if 0
/*! \brief */
inline void Texture_2d::set_text(Text_texture* text) { m_text = text; }

/*! \brief */
inline Text_texture* Texture_2d::get_text() const { return m_text; }
#endif

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Texture_2d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
