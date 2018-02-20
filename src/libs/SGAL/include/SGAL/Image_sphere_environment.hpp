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

#ifndef SGAL_IMAGE_SPHERE_ENVIRONMENT_HPP
#define SGAL_IMAGE_SPHERE_ENVIRONMENT_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Image.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Image_sphere_environment : public Sphere_environment {
public:
  typedef boost::shared_ptr<Image>              Shared_image;

  /*! Constructor */
  Image_sphere_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_sphere_environment();

  /*! Construct the prototype. */
  static Image_sphere_environment* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of this image textute extracted from the VRML or X3D
   * file.
   * \param elem contains lists of attribute names and values
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

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The image. */
  Shared_image m_images[2];
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Image_sphere_environment* Image_sphere_environment::prototype()
{ return new Image_sphere_environment(true); }

//! \brief clones.
inline Container* Image_sphere_environment::clone()
{ return new Image_sphere_environment(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image_sphere_environment::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
