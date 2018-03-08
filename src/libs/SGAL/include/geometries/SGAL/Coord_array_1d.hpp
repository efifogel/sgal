// Copyright (c) 2015 Israel.
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

#ifndef SGAL_COORD_ARRAY_1D_HPP
#define SGAL_COORD_ARRAY_1D_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Coord_array.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Coord_array_1d Coord_array_1d.hpp
 * Coord_array_1d maintains an array of 2D vertex coordinates of floating point
 * type.
 */
class SGAL_SGAL_DECL Coord_array_1d : public Coord_array {
public:
  enum {
    FIRST = Coord_array::LAST - 1,
    LAST
  };

  typedef Float_array::iterator       iterator;
  typedef Float_array::const_iterator const_iterator;

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Coord_array_1d(Boolean proto = false);

  /*! Construct. */
  Coord_array_1d(Size n);

  /*! Destruct. */
  virtual ~Coord_array_1d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Coord_array_1d* prototype();

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

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Float_array* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node.
   * \param[in] elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Set the array.
   * \param[in] array the new array.
   */
  void set_array(const Float_array& array);

  /*! Obtain the (const) array.
   * \return the (const) array.
   */
  const Float_array& get_array() const;

  /*! Obtain the (non-const) array.
   * \return the (non-const) array.
   */
  Float_array& get_array();

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
  Float_array::iterator begin();
  const Float_array::const_iterator begin() const;

  /*! The iterator to the Array past-the-end element */
  Float_array::iterator end();
  const Float_array::const_iterator end() const;

  /*! Push a new element at the back */
  void push_back(Float val);

  /*! Array indexing operator. */
  Float operator[](Uint n) const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The coordinate array. */
  Float_array m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Coord_array_1d* Coord_array_1d::prototype()
{ return new Coord_array_1d(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Coord_array_1d::create() { return new Coord_array_1d(); }

//! \brief sets the array.
inline void Coord_array_1d::set_array(const Float_array& array)
{ m_array = array; }

//! \brief obtains the (const) array.
inline const Float_array& Coord_array_1d::get_array() const {return m_array; }

//! \brief obtains the (non-const) array.
inline Float_array& Coord_array_1d::get_array() {return m_array; }

//! \brief obtains the array size.
inline Size Coord_array_1d::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Coord_array_1d::resize(Size n) { m_array.resize(n); }

//! \brief clears the array.
inline void Coord_array_1d::clear() { m_array.clear(); }

//! \brief obtains the iterator to the Array first element.
inline Float_array::iterator Coord_array_1d::begin() { return m_array.begin(); }

inline const Float_array::const_iterator Coord_array_1d::begin() const
{ return m_array.begin(); }

//! \brief obtains the iterator to the Array past-the-end element.
inline Float_array::iterator Coord_array_1d::end()
{ return m_array.end(); }

inline const Float_array::const_iterator Coord_array_1d::end() const
{ return m_array.end(); }

//! \brief pushes a new element at the back.
inline void Coord_array_1d::push_back(Float val)
{ m_array.push_back(val); }

//! \brief array indexing operator.
inline Float Coord_array_1d::operator[](Uint n) const { return m_array[n]; }

//! \brief obtains the number of texture coordinate dimensions.
inline Size Coord_array_1d::num_coordinates() const { return 2; }

//! \brief obtain the data size.
inline Size Coord_array_1d::data_size() const
{ return m_array.size() * sizeof(Float); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Coord_array_1d::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the data.
inline const GLfloat* Coord_array_1d::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Coord_array_1d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
