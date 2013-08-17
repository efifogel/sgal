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

/*!
 * Te texture_transform class enables simple geometric transformations, scale,
 * rotation, and translation, on texture coordinates
 */

#ifndef SGAL_TEXTURE_TRANSFORM_HPP
#define SGAL_TEXTURE_TRANSFORM_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

class SGAL_SGAL_DECL Texture_transform : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    LAST
  };

  /*! Constructor */
  Texture_transform(Boolean proto = false);

  /*! Destructor */
  virtual ~Texture_transform();

  /* Construct the prototype */
  static Texture_transform* prototype()
  { return new Texture_transform(true); }

  /*! Clone */
  virtual Container* clone() { return new Texture_transform(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

SGAL_END_NAMESPACE

#endif
