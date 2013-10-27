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

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL Array<Vector3f>;
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
  Vector3f_array* array_handle(Field_info*) { return &m_array; }
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
  Vector3f* begin();
  const Vector3f* begin() const;

  /*! The iterator to the Array past-the-end element */
  Vector3f* end();
  const Vector3f* end() const;

  /*! Array indexing operator. */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the vector. */
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
  Vector3f_array m_array;
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
inline Vector3f* Coord_array::begin() { return m_array.begin(); }
inline const Vector3f* Coord_array::begin() const { return m_array.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element */
inline Vector3f* Coord_array::end() { return m_array.end(); }
inline const Vector3f* Coord_array::end() const { return m_array.end(); }

/*! \brief array indexing operator. */
inline Vector3f& Coord_array::operator[](Uint n) { return m_array[n]; }

/*! \brief array indexing operator. */
inline const Vector3f& Coord_array::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief obtains the vector. */
inline const Vector3f* Coord_array::get_vector() const
{ return m_array.get_vector(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Coord_array::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
