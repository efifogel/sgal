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
// $Revision: 1366 $
//
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

/*! \file
 * coordinate interpolator engine
 *
 * The coordinate interpolator engine has the following attributes:
 * - value (array of vector3f) (field)
 * - keyValue - array of vector3f
 *
 * When the fraction field is set the interpolating function (execute)
 * is evaluated, resulting in a change of value with the same timestamp
 * as the timestamp of fraction change. The intepolating value is
 * calculated according to the fraction, keys, and values.
 */

#ifndef SGAL_COORDINATE_INTERPOLATOR_HPP
#define SGAL_COORDINATE_INTERPOLATOR_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Interpolator.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL std::allocator<Vector3f>;
template class SGAL_SGAL_DECL std::vector<Vector3f>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Coordinate_interpolator : public Interpolator {
public:
  enum {
    FIRST = Interpolator::LAST-1,
    KEY_VALUE,
    VALUE,
    LAST
  };

  // Constructor
  Coordinate_interpolator(Boolean proto = false);

  // Destructor
  virtual ~Coordinate_interpolator();

  /* Construct the prototype */
  static Coordinate_interpolator* prototype()
  { return new Coordinate_interpolator(true); }

  /*! Clone */
  virtual Container* clone() { return new Coordinate_interpolator (); }

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f_array* value_handle(const Field_info*) { return &m_value; }
  //@}

  // Functions that handles the creation of an instance in the scene graph
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function */
  virtual void execute(Field_info* field_info);

  /*! Obtain the domain values */
  const Vector3f_array& get_values() const { return m_values; }

  /*! Obtain the domain values */
  Vector3f_array& get_values() { return m_values; }

protected:
  /*! The interpolator domain key-values. */
  Vector3f_array m_values;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Obtain a copy of the value in a specific location.
   * \param location (in) The location of the sub array.
   */
  Vector3f_array get_value(Uint location);

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The last range location */
  Uint m_last_location;

  /*! The interpolator value */
  Vector3f_array m_value;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
