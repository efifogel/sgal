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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COLLISION_HPP
#define SGAL_COLLISION_HPP

/*! \file
 * A node in the scene graph that has collidable child objects.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Collision : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    LAST
  };

  /*! Constructor */
  Collision(Boolean proto = false);

  /*! Copy constructor */
  Collision(const Collision & groop);

  /*! Destructor */
  virtual ~Collision();

  /*! Construct the prototype. */
  static Collision* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Protoype handling: */
  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that represents the container. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Collision* Collision::prototype() { return new Collision(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Collision::create() { return new Collision(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Collision::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
