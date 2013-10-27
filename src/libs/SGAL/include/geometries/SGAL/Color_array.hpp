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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_COLOR_ARRAY_HPP
#define SGAL_COLOR_ARRAY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL Array<Vector3f>;
#endif

/*! This class maintains an array of vertex-colors */
class SGAL_SGAL_DECL Color_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    COLOR,
    LAST
  };

  /*! Constructor */
  Color_array(Boolean proto = false) : Container(proto) {}

  /*! Constructor */
  Color_array(Int n) { m_array.resize(n); }

  /*! Destructor */
  virtual ~Color_array() {}

  /*! Construct the prototype. */
  static Color_array* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /*! Sets the attributes of this node. */
  virtual void set_attributes(Element * elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  Uint size() const;

  /*! Resize the array capacity. */
  void resize(Uint n);

  /*! Clear the array. */
  void clear();

  /*! Obtain the iterator to the Array first element. */
  Vector3f* begin();

  /*! Obtain the const iterator to the Array first element. */
  const Vector3f * begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  Vector3f* end();

  /*! Obtain the const iterator to the Array past-the-end element. */
  const Vector3f* end() const ;

  /*! Array indexing operator. */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator. */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the vector. */
  Vector3f* get_vector();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The array of colors. */
  Array<Vector3f> m_array;
};

/*! \brief constructs the prototype. */
inline Color_array* Color_array::prototype() { return new Color_array(true); }

/*! \brief clones. */
inline Container* Color_array::clone() { return new Color_array(); }

/*! \brief obtains the array size. */
inline Uint Color_array::size() const { return m_array.size(); }

/*! \brief resizes the array capacity. */
inline void Color_array::resize(Uint n) { m_array.resize(n); }

/*! \brief clears the array. */
inline void Color_array::clear() { m_array.clear(); }

/*! \brief obtains the iterator to the Array first element. */
inline Vector3f* Color_array::begin() { return m_array.begin(); }

/*! \brief obtains the const iterator to the Array first element. */
inline const Vector3f * Color_array::begin() const { return m_array.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element. */
inline Vector3f* Color_array::end() { return m_array.end(); }

/*! \brief obtains the const iterator to the Array first element. */
inline const Vector3f* Color_array::end() const { return m_array.end(); }

/*! \brief Array indexing operator. */
inline Vector3f& Color_array::operator[](Uint n) { return m_array[n]; }

/*! \brief Array indexing operator. */
inline const Vector3f& Color_array::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief obtains the vector. */
inline Vector3f* Color_array::get_vector() { return m_array.get_vector(); }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Color_array::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
