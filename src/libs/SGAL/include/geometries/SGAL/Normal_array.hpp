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

#ifndef SGAL_NORMAL_ARRAY_HPP
#define SGAL_NORMAL_ARRAY_HPP

/*! \file
 * A class to hold a list of normals
 *
 * Inherits from Container
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Normal_array Normal_array.hpp
 * Coord_array maintains an array of 3D vertex-normals of floating point
 * type.
 */
class SGAL_SGAL_DECL Normal_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    VECTOR,
    LAST
  };

  typedef std::vector<Vector3f>::iterator       iterator;
  typedef std::vector<Vector3f>::const_iterator const_iterator;

  /*! Constructor.
   * \param[in] proto determines whether to construct a prototype.
   */
  Normal_array(Boolean proto = false);

  /*! Construct. */
  Normal_array(Size n);

  /*! Destruct. */
  virtual ~Normal_array();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Normal_array* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  std::vector<Vector3f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size.
   * \return the size.
   */
  Size size() const;

  /*! Determine whether the array is empty. */
  Boolean empty() const;

  /*! Resize the array capacity. */
  void resize(Size n);

  /*! Clear the array. */
  void clear();

  /*! The iterator to the Array first element. */
  std::vector<Vector3f>::iterator begin();

  const std::vector<Vector3f>::const_iterator begin() const;

  /*! The iterator to the Array past-the-end element. */
  std::vector<Vector3f>::iterator end();

  const std::vector<Vector3f>::const_iterator end() const;

  /*! Push a new normal at the back.
   * \param[in] val the new normal.
   */
  void push_back(const Vector3f& val);

  /*! Array indexing operator. */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the data size.
   * \return the data size.
   */
  Size data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  const GLfloat* data() const;

  /*! Obtain the datum at a given index.
   * \param[in] i the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const;

protected:
  /*! obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The normal array. */
  std::vector<Vector3f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs.
inline Normal_array::Normal_array(Boolean proto) : Container(proto) {}

//! \brief constructs.
inline Normal_array::Normal_array(Size n) { m_array.resize(n); }

//! \brief destructs.
inline Normal_array::~Normal_array() {}

//! \brief constructs the prototype.
inline Normal_array* Normal_array::prototype()
{ return new Normal_array(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Normal_array::create()
{ return new Normal_array(); }

//! \brief obtains the array size.
inline Size Normal_array::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Normal_array::resize(Size n) { m_array.resize(n); }

//! \brief clears the array.
inline void Normal_array::clear() { m_array.clear(); }

//! \brief obtains the begin iterator.
inline std::vector<Vector3f>::iterator Normal_array::begin()
{ return m_array.begin(); }

inline const std::vector<Vector3f>::const_iterator Normal_array::begin() const
{ return m_array.begin(); }

//! \brief obtains the pass-the-end iterator.
inline std::vector<Vector3f>::iterator Normal_array::end()
{ return m_array.end(); }

inline const std::vector<Vector3f>::const_iterator Normal_array::end() const
{ return m_array.end(); }

//! \brief pushes a new element at the back.
inline void Normal_array::push_back(const Vector3f& val)
{ m_array.push_back(val); }

//! \brief array indexing operator.
inline Vector3f& Normal_array::operator[](Uint n) { return m_array[n]; }

//! \brief array indexing operator.
inline const Vector3f& Normal_array::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtain the data size.
inline Size Normal_array::data_size() const
{ return m_array.size() * sizeof(Vector3f); }

//! \brief obtains the data.
inline const GLfloat* Normal_array::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Normal_array::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Normal_array::get_tag() const { return s_tag; }

//! \brief determines whether the array is empty.
inline Boolean Normal_array::empty() const {return (size() == 0); }

SGAL_END_NAMESPACE

#endif
