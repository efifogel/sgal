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
 * Scalar interpolator engine
 *
 * The scalar interpolator engine has the following attributes:
 *        - fraction (field)
 *        - value (of type float) (field)
 *        - keys
 *        - values
 *        - interpolation flag
 *
 *        The fraction field is the trigger to the execute function.
 *        So - for each cascade of the fraction field execute() is activated ->
 *        the value field is calculated according to the fraction,keys,values
 *        (as defined in VRML).
 *        If interpolation flag is false - instead of interpolating between two
 *        values - take the last value whose key is smaller than fraction.
 */

#ifndef SGAL_SCALAR_INTERPOLATOR_HPP
#define SGAL_SCALAR_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Interpolator.hpp"
#include "SGAL/Array.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

class SGAL_SGAL_DECL Scalar_interpolator : public Interpolator {
public:
  enum {
    FIRST = Node::LAST-1,
    FRACTION,
    VALUE,
    LAST
  };

  /*! Constructor */
  Scalar_interpolator(Boolean interpolate_flag = true,
                      Boolean proto = false);

  /*! Destructor */
  virtual ~Scalar_interpolator();

  /* Construct the prototype */
  static Scalar_interpolator* prototype()
  { return new Scalar_interpolator(false, true); }

  /*! Clone */
  virtual Container* clone() { return new Scalar_interpolator (); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* value_handle(Field_info*) { return &m_value; }
  //@}

  // Functions that handles the creation of an instance in the scene graph
  virtual void set_attributes(Element* elem);
  // virtual Attribute_list get_attributes();

  // The execution function - executes the intepolation for each cascade of
  // m_fraction, updated m_value and activate Cascade on it.
  virtual void execute(Field_info*);

protected:
  Array<Float> m_values;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Int m_last_location;
  Float m_value;
};

SGAL_END_NAMESPACE

#endif
