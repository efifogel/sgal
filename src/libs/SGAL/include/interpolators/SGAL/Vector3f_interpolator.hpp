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

/*! \file
 * Vector3f interpolator engine
 *
 * The vector3f linearly interpolator engine has the following attributes:
 * - fraction (field)
 * - value (of type vector3f) (field)
 * - keys - array of floating-point numbers
 * - values - array of vector3f
 * - interpolation flag
 *
 * When the fraction field is set the interpolating function (execute)
 * is evaluated, resulting in a change of value with the same timestamp
 * as the timestamp of fraction change. The intepolating value is
 * calculated according to the fraction, keys, and values.
 * If the interpolation flag is false, instead of interpolating between
 * two values, last value whose key is smaller than fraction is assumed.
 */

#ifndef SGAL_VECTOR3F_INTERPOLATOR_HPP
#define SGAL_VECTOR3F_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Interpolator.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Array.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

class SGAL_SGAL_DECL Vector3f_interpolator : public Interpolator {
public:
  enum {
    FIRST = Interpolator::LAST-1,
    KEY_VALUE,
    VALUE,
    LAST
  };

  // Constructor
  Vector3f_interpolator(Boolean interpolate_flag = true,
                        Boolean proto = false);

  // Destructor
  virtual ~Vector3f_interpolator();

  /* Construct the prototype. */
  static Vector3f_interpolator* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f* value_handle(Field_info*) { return &m_value; }
  //@}

  // Functions that handles the creation of an instance in the scene graph.
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function. */
  virtual void execute(Field_info* field_info);

  /*! Obtain the range keys. */
  const Array<Float>& get_keys() const;

  /*! Obtain the range keys. */
  Array<Float>& get_keys();

  /*! Obtain the domain values. */
  const Array<Vector3f>& get_values() const;

  /*! Obtain the domain values. */
  Array<Vector3f>& get_values();

protected:
  /*! The interpolator domain key-values. */
  Array<Vector3f> m_values;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype .*/
  static Container_proto* s_prototype;

  /*! The last range location. */
  Uint m_last_location;

  /*! The interpolator value. */
  Vector3f m_value;
};

/* \brief constructs the prototype. */
inline Vector3f_interpolator* Vector3f_interpolator::prototype()
{ return new Vector3f_interpolator(false, true); }

/*! \brief clones. */
inline Container* Vector3f_interpolator::clone()
{ return new Vector3f_interpolator (); }

/*! \brief obtains the range keys. */
inline const Array<Float>& Vector3f_interpolator::get_keys() const
{ return m_keys; }

/*! \brief obtains the range keys. */
inline Array<Float>& Vector3f_interpolator::get_keys() { return m_keys; }

/*! \brief obtains the domain values. */
inline const Array<Vector3f>& Vector3f_interpolator::get_values() const
{ return m_values; }

/*! \brief obtains the domain values. */
inline Array<Vector3f>& Vector3f_interpolator::get_values() { return m_values; }

/*! Obtain the tag (type) of the container. */
inline const std::string& Vector3f_interpolator::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
