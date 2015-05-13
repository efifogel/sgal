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
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Image_url.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Container_proto.hpp"

SGAL_BEGIN_NAMESPACE

class Element;

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
  typedef boost::shared_ptr<Image_url>          Shared_image;

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

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the URL. */
  void set_url(const std::string& url);

  /*! Obtain the URL. */
  const std::string get_url() const;

  /*! Set the flag that indicates whether to reflect the image. */
  void set_flip(Boolean flag);

  /*! Obtain the flag that indicates whether to reflect the image. */
  Boolean get_flip() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The image. */
  Shared_image m_image;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Image_texture* Image_texture::prototype()
{ return new Image_texture(true); }

//! \brief clones.
inline Container* Image_texture::clone() { return new Image_texture(); }

//! \brief sets the URL.
inline void Image_texture::set_url(const std::string& url)
{ m_image->set_url(url); }

//! \brief obtains the URL.
inline const std::string Image_texture::get_url() const
{ return m_image->get_url(); }

//! \brief sets the flag that indicates whether to reflect the image.
inline void Image_texture::set_flip(Boolean flag)
{ m_image->set_flip(flag); }

//! \brief obtains the flag that indicates whether to reflect the image.
inline Boolean Image_texture::get_flip() const
{ return m_image->get_flip(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_texture::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
