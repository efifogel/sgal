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
 * Rotation interpolator engine
 *
 * The rotation interpolator engine has the following attributes:
 *        - fraction (field)
 *        - value (of type Rotation) (field)
 *        - keys 
 *        - values (array of Rotation)
 *        - interpolation flag
 *        
 *        The fraction field is the trigger to the execute function.
 *        So - for each cascade of the fraction field execute() is activated ->
 *        the value field is calculated according to the fraction,keys,values
 *        (as defined in VRML).
 *        If interpolation flag is false - instead of interpolating between two
 *        values - take the last value whose key is smaller than fraction.
 */

#ifndef SGAL_ROTATION_INTERPOLATOR_HPP
#define SGAL_ROTATION_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Interpolator.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;

class SGAL_SGAL_DECL Rotation_interpolator : public Interpolator {
public:
  enum {
    FIRST = Interpolator::LAST-1,
    VALUE,
    LAST
  };

  /*! Constructor */
  Rotation_interpolator(Boolean interpolate_flag = true,
                        Boolean proto = false);

  /*! Destructor */
  virtual ~Rotation_interpolator();

  /*! Construct the prototype */
  static Rotation_interpolator* prototype()
  { return new Rotation_interpolator(false, true); }

  /*! Clone */
  virtual Container* clone(){ return new Rotation_interpolator (); }

  /*! Initializes the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */
  virtual Container_proto* get_prototype();

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  // The execution function of the interpolator - calculates the interpolation
  // between two rotation fields according to the current m_fraction
  virtual void execute(Field_info*);

protected:
  /*! Indicates whether to interpolate between keys */
  Boolean m_interpolate_flag;

  /*! The interpolator domain key-values */
  Array<Rotation> m_values;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Uint m_last_location;

  Rotation m_value;
};

SGAL_END_NAMESPACE

#endif
