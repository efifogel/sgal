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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COORD_ARRAY_2D_HPP
#define SGAL_COORD_ARRAY_2D_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Coord_array_2d Coord_array_2d.hpp
 * Coord_array_2d maintains an array of 2D vertex coordinates of floating point
 * type.
 */
class SGAL_SGAL_DECL Coord_array_2d : public Coord_array {
public:
  enum {
    FIRST = Coord_array::LAST - 1,
    POINT,
    LAST
  };

  typedef std::vector<Vector2f>::iterator       iterator;
  typedef std::vector<Vector2f>::const_iterator const_iterator;

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Coord_array_2d(Boolean proto = false);

  /*! Construct. */
  Coord_array_2d(Size n);

  /*! Destruct. */
  virtual ~Coord_array_2d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Coord_array_2d* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  std::vector<Vector2f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node.
   * \param[in] elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  virtual Size size() const;

  /*! Resize the array capacity. */
  virtual void resize(Size n);

  /*! Clear the array. */
  virtual void clear();

  /*! Obtain the number of coordinate dimensions.
   * \return the number of coordinate dimensions.
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

  /*! The iterator to the Array first element. */
  std::vector<Vector2f>::iterator begin();
  const std::vector<Vector2f>::const_iterator begin() const;

  /*! The iterator to the Array past-the-end element */
  std::vector<Vector2f>::iterator end();
  const std::vector<Vector2f>::const_iterator end() const;

  /*! Push a new element at the back */
  void push_back(const Vector2f& val);

  /*! Array indexing operator. */
  Vector2f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector2f& operator[](Uint n) const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The coordinate array. */
  std::vector<Vector2f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Coord_array_2d* Coord_array_2d::prototype()
{ return new Coord_array_2d(true); }

//! \brief clones.
inline Container* Coord_array_2d::clone() { return new Coord_array_2d(); }

//! \brief obtains the array size.
inline Size Coord_array_2d::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Coord_array_2d::resize(Size n) { m_array.resize(n); }

//! \brief clears the array.
inline void Coord_array_2d::clear() { m_array.clear(); }

//! \brief obtains the iterator to the Array first element.
inline std::vector<Vector2f>::iterator Coord_array_2d::begin()
{ return m_array.begin(); }

inline const std::vector<Vector2f>::const_iterator
Coord_array_2d::begin() const
{ return m_array.begin(); }

//! \brief obtains the iterator to the Array past-the-end element.
inline std::vector<Vector2f>::iterator Coord_array_2d::end()
{ return m_array.end(); }

inline const std::vector<Vector2f>::const_iterator Coord_array_2d::end() const
{ return m_array.end(); }

//! \brief pushes a new element at the back.
inline void Coord_array_2d::push_back(const Vector2f& val)
{ m_array.push_back(val); }

//! \brief array indexing operator.
inline Vector2f& Coord_array_2d::operator[](Uint n) { return m_array[n]; }

//! \brief array indexing operator.
inline const Vector2f& Coord_array_2d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtains the number of texture coordinate dimensions.
inline Size Coord_array_2d::num_coordinates() const { return 2; }

//! \brief obtain the data size.
inline Size Coord_array_2d::data_size() const
{ return m_array.size() * sizeof(Vector2f); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Coord_array_2d::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the data.
inline const GLfloat* Coord_array_2d::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Coord_array_2d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
