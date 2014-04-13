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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VECTOR3F_INTERPOLATOR_HPP
#define SGAL_VECTOR3F_INTERPOLATOR_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Interpolator.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL std::allocator<Vector3f>;
template class SGAL_SGAL_DECL std::vector<Vector3f>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Vector3f_interpolator Vector3f_interpolator.hpp
 * Vector3f_interpolator is a linear interpolator engine. It linearly
 * interpolates between 3D vectors. It is derived from from Interpolator,
 * and uses the following attributes:
 * - fraction - a rational number in the closed interval [0,1]
 * - value - the engine output 3D vector (Vector3f)
 * - keys - a monotone sequence of rational numbers.
 * - values - an array of 3D vectors of of size equal to the size of keys.
 * - interpolation flag - a Boolean flag
 *
 * When the fraction field is set, the interpolating function (execute)
 * is evaluated, resulting in a change of value with the same timestamp
 * as the timestamp of fraction change. The outout value is calculated
 * according to the fraction, keys, and values.
 * If the interpolation flag is false, instead of interpolating between
 * two values, last value whose key is smaller than fraction is assumed.
 */
class SGAL_SGAL_DECL Vector3f_interpolator : public Interpolator {
public:
  enum {
    FIRST = Interpolator::LAST-1,
    KEY_VALUE,
    VALUE,
    LAST
  };

  /*! Constructor
   * \param interpolate_flag (in) initialization value for the interpolate flag
   *                              field
   * \param proto (in) indicates whether the object to construct is the
   *                   prototype.
   */
  Vector3f_interpolator(Boolean interpolate_flag = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Vector3f_interpolator();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Vector3f_interpolator* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f* value_handle(const Field_info*) { return &m_value; }
  Vector3f_array* values_handle(const Field_info*) { return &m_values; }
  //@}

  // Functions that handles the creation of an instance in the scene graph.
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! The interpolation execution function.
   * Activated through the cascade of the m_fraction field.
   * The function calculates m_value, updates it and activate cascade on it
   * \param field_info (in) to the cascaded field's field info
   */
  virtual void execute(const Field_info* field_info);

  /*! Obtain the range keys.
   */
  const std::vector<Float>& get_keys() const;

  /*! Obtain the range keys.
   */
  std::vector<Float>& get_keys();

  /*! Obtain the domain values.
   */
  const std::vector<Vector3f>& get_values() const;

  /*! Obtain the domain values.
   */
  std::vector<Vector3f>& get_values();

protected:
  /*! The interpolator domain key-values.
   */
  std::vector<Vector3f> m_values;

  /*! Obtain the tag (type) of the container.
   */
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

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Vector3f_interpolator* Vector3f_interpolator::prototype()
{ return new Vector3f_interpolator(false, true); }

/*! \brief clones. */
inline Container* Vector3f_interpolator::clone()
{ return new Vector3f_interpolator (); }

/*! \brief obtains the range keys. */
inline const std::vector<Float>& Vector3f_interpolator::get_keys() const
{ return m_keys; }

/*! \brief obtains the range keys. */
inline std::vector<Float>& Vector3f_interpolator::get_keys() { return m_keys; }

/*! \brief obtains the domain values. */
inline const std::vector<Vector3f>& Vector3f_interpolator::get_values() const
{ return m_values; }

/*! \brief obtains the domain values. */
inline std::vector<Vector3f>& Vector3f_interpolator::get_values()
{ return m_values; }

/*! Obtain the tag (type) of the container. */
inline const std::string& Vector3f_interpolator::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
