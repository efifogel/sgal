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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node that represents a reflection cubical map.
 */

#ifndef SGAL_CUBE_ENVIRONMENT_HPP
#define SGAL_CUBE_ENVIRONMENT_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Texture.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_CLASSDEF Cube_environment : public Texture {
public:
  enum {
    FIRST = Texture::LAST - 1,
    LAST
  };

  /*! Constructor */
  Cube_environment(Boolean proto = false);

  /*! Destructor */
  virtual ~Cube_environment();

  /*! Construct the prototype. */
  static Cube_environment* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Set the attributes of the cubical environment map. */
  virtual void set_attributes(Element* elem);
  
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! the node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Cube_environment* Cube_environment::prototype()
{ return new Cube_environment(true); }

/*! \brief clones. */
inline Container* Cube_environment::clone() { return new Cube_environment(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Cube_environment::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
