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

/**
 @class:   RotationRelativeEngine

 Purpose: Relative engine for rotations

 Description: The rotation relative engine has the following attributes:
        - fraction (field)
        - value (of type Rotation) (field)
        - rotation (field)
        - angle - required relative change in rotation (field)

        The fraction field is the trigger to the execute function.
        So - for each cascade of the fraction field execute() is activated ->
        the value field is calculated according to the fraction, rotation
        and angle (which is relative).
        rotation is updated to the new rotation when a fraction value,
        which is smaller than the last fraction value is given (this means
        this is a new cycle).
*/

#include "SGAL/basic.hpp"
#include "Node.h"
#include "Rotation.h"

SGAL_BEGIN_NAMESPACE

class Element;

class Rotation_relative_engine : public Node {
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
  Rotation_relative_engine(Boolean proto = false);

  /*! Destructor */
  virtual ~Rotation_relative_engine();

  /*! Construct the prototype */
  static Rotation_relative_engine * prototype()
  { return new Rotation_relative_engine(true); }

  /*! Clone */
  virtual Container * clone() { return new Rotation_relative_engine(); }

  // protoype handling
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* fraction_handle(const Field_info*) { return &m_fraction; }
  Rotation* value_handle(const Field_info*) { return &m_value; }
  Float* angle_handle(const Field_info*) { return &m_angle; }
  //@}

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
