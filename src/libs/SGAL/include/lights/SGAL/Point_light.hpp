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
// $Source$
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * \brief: An implementation of a point light.
 *  
 * A point light is a light that is positioned as specified, and has no
 * direction and a cut off cone angle of 180.
 *
 * Inherits from Spot_light
 */

#ifndef SGAL_POINT_LIGHT_HPP
#define SGAL_POINT_LIGHT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Spot_light.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;

class Point_light : public Spot_light {
public:
  /*! Constructor */
  Point_light(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Point_light();

  /* Construct the prototype */
  static Point_light * prototype() { return new Point_light(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Point_light(); }

  /*! Draw the node */
  virtual Action::Trav_directive draw(Draw_action * draw_action);

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();
  
  // this has to be implemented as an emty methid to avoid double deletion
  // of the prototype in the bae class.
  virtual void delete_prototype() {}

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;
};

SGAL_END_NAMESPACE

#endif
