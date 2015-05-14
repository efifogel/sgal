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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_TEXTURE_HPP
#define SGAL_IMAGE_TEXTURE_HPP

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Image_texture Image_texture.hpp
 * Image_texture is a container that specifies 2D texture mapping parameters,
 * namely, an image file, and two additional parameters that specify how the
 * texture wraps. The mapping is defined from the unit square [0,1]x[0,1].
 */
class SGAL_SGAL_DECL Image_texture : public Texture_2d {
public:
  enum {
    FIRST = Texture_2d::LAST - 1,
    URL,
    FLIP,
    LAST
  };

  /*! Construct */
  Image_texture(Boolean proto = false);

  /*! Destruct */
  virtual ~Image_texture();

  /*! Construct the prototype. */
  static Image_texture* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this image textute extracted from the VRML or X3D
   * file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param[in] scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::string* url_handle(const Field_info*) { return &m_url; }
  Boolean* flip_handle(const Field_info*) { return &m_flip; }
  //@}

  /*! Respond to a change to a field.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  void field_changed(const Field_info* field_info);

  /*! Set the image. */
  void set_image(Shared_image image);

  /*! Obtain the image. */
  const Shared_image get_image();

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url();

  /*! Set the flag that indicates whether to reflect the image. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether to reflect the image. */
  Boolean get_flip();

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

  /*! Clean the object using the new decoded data. */
  virtual void clean();

  /*! Determines whether the texture is empty. */
  virtual Boolean empty();

  /*! Obtain the texture number of components. */
  virtual Uint get_component_count();

  /*! Clean the image.
   */
  void clean_image();

  /*! Clean the attributes.
   */
  void clean_attributes();

  /*! The uniform resource locator (url) that refers to the (Internet)
   * resource that contains the image.
   */
  std::string m_url;

  /*! Indicates whether the image should be reflected when read from file. */
  Boolean m_flip;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether the image is dirty and thus must be cleaned. */
  Boolean m_dirty_image;

  /*! Indicates whether the url attribute is dirty and thus must be cleaned. */
  Boolean m_dirty_url;

  /*! Indicates whether the flip attribute is dirty and thus must be cleaned. */
  Boolean m_dirty_flip;

  /*! The scene graph. */
  Scene_graph* m_scene_graph;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Image_texture* Image_texture::prototype()
{ return new Image_texture(true); }

//! \brief clones.
inline Container* Image_texture::clone() { return new Image_texture(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_texture::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
