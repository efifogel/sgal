// Copyright (c) 2014 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COORD_ARRAY_3D_HPP
#define SGAL_COORD_ARRAY_3D_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Coord_array_3d Coord_array_3d.hpp
 * Coord_array_3d maintains an array of 3D vertex-coordinates of floating point
 * type.
 */
class SGAL_SGAL_DECL Coord_array_3d : public Coord_array {
public:
  enum {
    FIRST = Coord_array::LAST - 1,
    LAST
  };

  typedef Vector3f_array::iterator       iterator;
  typedef Vector3f_array::const_iterator const_iterator;

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Coord_array_3d(Boolean proto = false);

  /*! Construct.
   */
  Coord_array_3d(Size n);

  /*! Destruct.
   */
  virtual ~Coord_array_3d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Coord_array_3d* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype.
   */
  virtual void delete_prototype();

  /*! Obtains the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Vector3f_array* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node.
   * \param[in] elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Obtain the array size.
   */
  virtual Size size() const;

  /*! Determine whether the array is empty.
   */
  Boolean empty() const;

  /*! Resize the array capacity.
   */
  virtual void resize(Size n);

  /*! Clear the array.
   */
  virtual void clear();

  /*! Obtain the number of texture coordinate dimensions.
   * \return the number of texture coordinate dimensions.
   */
  virtual Size num_coordinates() const;

  /*! Obtain the data size.
    \return the data size.
   */
  virtual Size data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const;

  /*! Obtain the datum at a given index.
   * \param[in] i the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const;

  /*! Obtain the iterator to the array first element.
   */
  Vector3f_array::iterator begin();

  /*! Obtain the const iterator to the array first element.
   */
  const Vector3f_array::const_iterator begin() const;

  /*! Obtain the iterator to the array past-the-end element.
   */
  Vector3f_array::iterator end();

  /*! Obtain the const iterator to the array past-the-end element.
   */
  const Vector3f_array::const_iterator end() const;

  /*! Push a new element at the back.
   * \param[in] val the new element.
   */
  void push_back(const Vector3f& val);

  /*! Array indexing operator.
   */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator.
   */
  const Vector3f& operator[](Uint n) const;

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! The array of coordinates.
  Vector3f_array m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Coord_array_3d* Coord_array_3d::prototype()
{ return new Coord_array_3d(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Coord_array_3d::create() { return new Coord_array_3d(); }

//! \brief obtains the array size.
inline Size Coord_array_3d::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Coord_array_3d::resize(Size n) { m_array.resize(n); }

//! \brief clears the array.
inline void Coord_array_3d::clear() { m_array.clear(); }

//! \brief determines whether the array is empty.
inline Boolean Coord_array_3d::empty() const { return m_array.empty(); }

//! \brief obtains the iterator to the array first element.
inline Vector3f_array::iterator Coord_array_3d::begin()
{ return m_array.begin(); }

//! \brief obtains the const iterator to the array first element.
inline const Vector3f_array::const_iterator Coord_array_3d::begin() const
{ return m_array.begin(); }

//! \brief obtains the iterator to the array past-the-end element.
inline Vector3f_array::iterator Coord_array_3d::end() { return m_array.end(); }

//! \brief obtains the const iterator to the array past-the-end element.
inline const Vector3f_array::const_iterator Coord_array_3d::end() const
{ return m_array.end(); }

//! \brief pushes a new element at the back.
inline void Coord_array_3d::push_back(const Vector3f& val)
{ m_array.push_back(val); }

//! \brief array indexing operator.
inline Vector3f& Coord_array_3d::operator[](Uint n) { return m_array[n]; }

//! \brief array indexing operator.
inline const Vector3f& Coord_array_3d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtains the number of texture coordinate dimensions.
inline Size Coord_array_3d::num_coordinates() const { return 3; }

//! \brief obtain the data size.
inline Size Coord_array_3d::data_size() const
{ return m_array.size() * sizeof(Vector3f); }

//! \brief obtains the data.
inline const GLfloat* Coord_array_3d::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Coord_array_3d::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Coord_array_3d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
