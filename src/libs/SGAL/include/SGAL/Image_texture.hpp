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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_TEXTURE_HPP
#define SGAL_IMAGE_TEXTURE_HPP

/*! \file
 *
 */

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;

class SGAL_CLASSDEF Image_texture : public Texture_2d {
public:
  typedef Image::Path_list                          Path_list;
  
  enum {
    FIRST = Texture_2d::LAST - 1,
    URL,
    FLIP,
    LAST
  };

  /*! Constructor */
  Image_texture(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_texture() {}

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

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Set the flag that indicates whether to reflect the image. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether to reflect the image. */
  Boolean get_flip() const;
  
  /*! Set the directory-search structure. */
  void set_dirs(const Path_list& dirs);

  /*! Obtain the directory-search structure. */
  const Path_list& get_dirs() const;
  
protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;
  
private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The image. */
  Image m_image;
};

/*! \brief constructs the prototype. */
inline Image_texture* Image_texture::prototype()
{ return new Image_texture(true); }

/*! \brief clones. */
inline Container* Image_texture::clone() { return new Image_texture(); }

/*! \brief sets the URL. */
inline void Image_texture::set_url(const std::string& url)
{ m_image.set_url(url); }

/*! \brief obtains the URL. */
inline const std::string Image_texture::get_url() const
{ return m_image.get_url(); }

/*! \brief sets the flag that indicates whether to reflect the image. */
inline void Image_texture::set_flip(Boolean flag)
{ m_image.set_flip(flag); }

/*! \brief obtains the flag that indicates whether to reflect the image. */
inline Boolean Image_texture::get_flip() const
{ return m_image.get_flip(); }
 
/*! \brief sets the directory-search structure. */
inline void Image_texture::set_dirs(const Image::Path_list& dirs)
{ m_image.set_dirs(dirs); }

/*! \brief obtains the directory-search structure. */
inline const Image::Path_list& Image_texture::get_dirs() const
{ return m_image.get_dirs(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Image_texture::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
