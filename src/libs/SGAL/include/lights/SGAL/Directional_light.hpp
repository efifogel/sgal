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

/**
 *
 * @class: Directional_light  
 *  
 * Purpose: An implementation of a directional light.
 *  
 * Description:
 *  A directional light is a light that is positioned at infinity
 *  and has only a direction specified.
 *  
 *  Inherits from Light
 */

#ifndef SGAL_DIRECTIONAL_LIGHT_HPP
#define SGAL_DIRECTIONAL_LIGHT_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Light.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Element;
class Container_proto;

class Directional_light : public Light {
public:
  enum {
    FIRST = Light::LAST-1,
    DIRECTION,
    LAST
  };

  /*! Constructor */
  Directional_light(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Directional_light();

  /* Construct the prototype */
  static Directional_light * prototype()
  { return new Directional_light(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Directional_light(); }

  virtual Action::Trav_directive draw(Draw_action * draw_action);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto * get_prototype();

  /*! \biref sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  void set_direction(const Vector3f & direction);
  void get_direction(Vector3f & direction) const;
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }
  
private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The default direction of the light */
  static const Vector3f s_def_direction;

  /*! The direction of the light */
  Vector3f m_direction;
};

SGAL_END_NAMESPACE

#endif
