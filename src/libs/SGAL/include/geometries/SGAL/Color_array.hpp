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

#ifndef SGAL_COLOR_ARRAY_HPP
#define SGAL_COLOR_ARRAY_HPP

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

/*! \class Color_array Color_array.hpp
 * Coord_array maintains an array of 3D vertex-colors of floating point
 * type.
 */
class SGAL_SGAL_DECL Color_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    COLOR,
    LAST
  };

  typedef std::vector<Vector3f>::iterator       iterator;
  typedef std::vector<Vector3f>::const_iterator const_iterator;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Color_array(Boolean proto = false);

  /*! Constructor. */
  Color_array(Uint n);

  /*! Destructor. */
  virtual ~Color_array();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Color_array* prototype();

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

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  std::vector<Vector3f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size.
   * \return the array size.
   */
  Uint size() const;

  /*! Determine whether the array is empty. */
  Boolean empty() const;

  /*! Resize the array capacity. */
  void resize(Uint n);

  /*! Clear the array. */
  void clear();

  /*! Obtain the iterator to the Array first element. */
  std::vector<Vector3f>::iterator begin();

  /*! Obtain the const iterator to the Array first element. */
  const std::vector<Vector3f>::const_iterator begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  std::vector<Vector3f>::iterator end();

  /*! Obtain the const iterator to the Array past-the-end element. */
  const std::vector<Vector3f>::const_iterator end() const ;

  /*! Push a new element at the back.
   * \param val (in) the new element.
   */
  void push_back(const Vector3f& val);

  /*! Array indexing operator. */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the data size.
   * \return the data size.
   */
  Uint data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  const GLfloat* data() const;

  /*! Obtain the datum at a given index.
   * \param i (in) the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The array of colors. */
  std::vector<Vector3f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructor.
inline Color_array::Color_array(Boolean proto) : Container(proto) {}

//!\brief constructor.
inline Color_array::Color_array(Uint n) { m_array.resize(n); }

//! \brief destructor.
inline Color_array::~Color_array() {}

//! \brief constructs the prototype.
inline Color_array* Color_array::prototype() { return new Color_array(true); }

//! \brief clones.
inline Container* Color_array::clone() { return new Color_array(); }

//! \brief obtains the array size.
inline Uint Color_array::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Color_array::resize(Uint n) { m_array.resize(n); }

//! \brief clears the array.
inline void Color_array::clear() { m_array.clear(); }

//! \brief obtains the iterator to the Array first element.
inline std::vector<Vector3f>::iterator Color_array::begin()
{ return m_array.begin(); }

//! \brief obtains the const iterator to the Array first element.
inline const std::vector<Vector3f>::const_iterator Color_array::begin() const
{ return m_array.begin(); }

//! \brief obtains the iterator to the Array past-the-end element.
inline std::vector<Vector3f>::iterator Color_array::end()
{ return m_array.end(); }

//! \brief obtains the const iterator to the Array first element.
inline const std::vector<Vector3f>::const_iterator Color_array::end() const
{ return m_array.end(); }

//! \brief pushes a new element at the back.
inline void Color_array::push_back(const Vector3f& val)
{ m_array.push_back(val); }

//! \brief Array indexing operator.
inline Vector3f& Color_array::operator[](Uint n) { return m_array[n]; }

//! \brief Array indexing operator.
inline const Vector3f& Color_array::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtain the data size.
inline Uint Color_array::data_size() const
{ return m_array.size() * sizeof(Vector3f); }

//! \brief obtains the data.
inline const GLfloat* Color_array::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Color_array::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Color_array::get_tag() const { return s_tag; }

//! \brief determines whether the array is empty.
inline Boolean Color_array::empty() const {return (size() == 0); }

SGAL_END_NAMESPACE

#endif
