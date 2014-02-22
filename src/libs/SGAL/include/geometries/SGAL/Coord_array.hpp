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
// $Revision: 7780 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COORD_ARRAY_HPP
#define SGAL_COORD_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <list>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL std::allocator<Vector3f>;
template class SGAL_SGAL_DECL std::vector<Vector3f>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Maintains an array of 3D vertex-coordinate. */
class SGAL_SGAL_DECL Coord_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    POINT,
    LAST
  };

  /*! Constructor. */
  Coord_array(Boolean proto = false);

  /*! Constructor. */
  Coord_array(Uint n);

  /*! Destructor. */
  virtual ~Coord_array();

  /* Construct the prototype. */
  static Coord_array* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::vector<Vector3f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  Uint size() const;

  /*! Resize the array capacity. */
  void resize(Uint n);

  /*! Clear the array. */
  void clear();

  /*! The iterator to the Array first element. */
  std::vector<Vector3f>::iterator begin();
  const std::vector<Vector3f>::const_iterator begin() const;

  /*! The iterator to the Array past-the-end element */
  std::vector<Vector3f>::iterator end();
  const std::vector<Vector3f>::const_iterator end() const;

  /*! Push a new element at the back */
  void push_back(const Vector3f& val);

  /*! Array indexing operator. */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the array itself.
   */
  const Vector3f* get_vector() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Process change of points.
   * \param field_info
   */
  void point_changed(Field_info* field_info);

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The coordinate array. */
  std::vector<Vector3f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Coord_array* Coord_array::prototype() { return new Coord_array(true); }

/*! \brief clones. */
inline Container* Coord_array::clone() { return new Coord_array(); }

/*! \brief obtains the array size. */
inline Uint Coord_array::size() const { return m_array.size(); }

/*! \brief resizes the array capacity. */
inline void Coord_array::resize(Uint n) { m_array.resize(n); }

/*! \brief clears the array. */
inline void Coord_array::clear() { m_array.clear(); }

/*! \brief obtains the iterator to the Array first element. */
inline std::vector<Vector3f>::iterator Coord_array::begin()
{ return m_array.begin(); }

inline const std::vector<Vector3f>::const_iterator Coord_array::begin() const
{ return m_array.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element */
inline std::vector<Vector3f>::iterator Coord_array::end()
{ return m_array.end(); }

inline const std::vector<Vector3f>::const_iterator Coord_array::end() const
{ return m_array.end(); }

/*! \brief pushes a new element at the back */
inline void Coord_array::push_back(const Vector3f& val)
{ m_array.push_back(val); }

/*! \brief array indexing operator. */
inline Vector3f& Coord_array::operator[](Uint n) { return m_array[n]; }

/*! \brief array indexing operator. */
inline const Vector3f& Coord_array::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief obtains the vector. */
inline const Vector3f* Coord_array::get_vector() const
{ return &(*(m_array.begin())); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Coord_array::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
