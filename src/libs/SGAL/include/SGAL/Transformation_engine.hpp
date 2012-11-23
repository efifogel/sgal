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

#ifndef SGAL_TRANSFORMATION_ENGINE_HPP
#define SGAL_TRANSFORMATION_ENGINE_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Element;

class Transformation_engine : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    FRACTION,
    VALUE,
    ROTATION,
    ANGLE,
    LAST
  };

  /*! Constructor */
  Transformation_engine(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Transformation_engine();

  /*! Construct the prototype */
  static Transformation_engine * prototype()
  { return new Transformation_engine(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Transformation_engine(); }

  // protoype handling
  virtual void init_prototype();
  virtual void delete_prototype()
  {
    delete s_prototype;
  }
  virtual Container_proto * get_prototype() 
  {  
    if (s_prototype == NULL)
      init_prototype();
    return s_prototype;
  }

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  virtual Attribute_list get_attributes();

  // The execution function of the engine - calculates the suitable rotation
  // using the currentRotation relative angle and fraction
  virtual void execute(Field_info *);

  virtual Trav_directive Draw(Draw_action * draw_action) { return Trav_cont; }; 

private:
  static Container_proto * s_prototype;

  Float m_fraction;
  Rotation m_value;
  Rotation m_rotation;
  Float m_angle;
  Float m_last_fraction;
};

SGAL_END_NAMESPACE

#endif
