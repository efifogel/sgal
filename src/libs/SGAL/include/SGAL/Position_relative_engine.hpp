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

// Copyright 2000 Enbaya Ltd.
/**
 @class:   Position_relative_engine

 Purpose: Relative engine for positions

 Description: The position relative engine has the following attributes:
        - fraction (field)
        - value (of type Vector3f) (field)
        - position (field)
        - delta - required relative change in position (field)
        - rotation - direction for the relative position xhange (field)
                     used to calculate the new position

        The fraction field is the trigger to the execute function.
        So - for each cascade of the fraction field execute() is activated ->
        the value field is calculated according to the fraction, position, delta
        and rotation.
        currentRotation is updated to the new rotation when a fraction value,
        which is smaller than the last fraction value is given (this means
        this is a new cycle).
*/

#include "SGAL/basic.hpp"
#include "Node.h"
#include "Rotation.h"

class Element;
class Container_proto;

SGAL_BEGIN_NAMESPACE

class Position_relative_engine : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    FRACTION,
    VALUE,
    POSITION,
    DELTA,
    ROTATION,
    LAST
  };

  /*! Constructor */
  Position_relative_engine(Boolean proto = false);

  /*! Destructor */
  virtual ~Position_relative_engine();

  /*! Construct the prototype */
  static Position_relative_engine* prototype()
  { return new Position_relative_engine(); }

  /*! Clone */
  virtual Container* clone() { return new Position_relative_engine(true); }

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  // The execution function of the engine - calculates the suitable rotation
  // using the currentRotation relative angle and fraction
  virtual void execute(Field_info*);

  virtual Trav_directive Draw(Draw_action* draw_action) {return Trav_cont;};

private:
  static Container_proto* s_prototype;

  Float m_fraction;
  Vector3f m_value;
  Vector3f m_position;
  Rotation m_rotation;
  Vector3f m_delta;
  Float m_lastFraction;
};

SGAL_END_NAMESPACE
