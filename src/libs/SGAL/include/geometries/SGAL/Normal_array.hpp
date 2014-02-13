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

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL std::vector<Vector3f>;
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! This class maintains an array of vertex-normals */
class SGAL_SGAL_DECL Normal_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    VECTOR,
    LAST
  };

  /*! Constructor */
  Normal_array(Boolean proto = false);

  /*! Constructor */
  Normal_array(Uint n);

  /*! Destructor */
  virtual ~Normal_array();

  /* Construct the prototype */
  static Normal_array* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size */
  Uint size() const;

  /*! Resize the array capacity */
  void resize(Uint n);

  /*! Clear the array */
  void clear();

  /*! The iterator to the Array first element */
  std::vector<Vector3f>::iterator begin();

  const std::vector<Vector3f>::const_iterator begin() const;

  /*! The iterator to the Array past-the-end element */
  std::vector<Vector3f>::iterator end();

  const std::vector<Vector3f>::const_iterator end() const;

  /*! Array indexing operator */
  Vector3f& operator[](Uint n);

  /*! Array indexing operator */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the vector */
  Vector3f* get_vector();

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The normal array */
  std::vector<Vector3f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructor.
inline Normal_array::Normal_array(Boolean proto) : Container(proto) {}

//! \brief constructor.
inline Normal_array::Normal_array(Uint n) { m_array.resize(n); }

//! \brief destructor.
inline Normal_array::~Normal_array() {}

//! \brief constructs the prototype.
inline Normal_array* Normal_array::prototype()
{ return new Normal_array(true); }

//! \brief clones.
inline Container* Normal_array::clone() { return new Normal_array(); }

//! \brief obtains the array size.
inline Uint Normal_array::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Normal_array::resize(Uint n) { m_array.resize(n); }

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

//! \brief array indexing operator.
inline Vector3f& Normal_array::operator[](Uint n) { return m_array[n]; }

//! \brief array indexing operator.
inline const Vector3f& Normal_array::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtains the vector.
inline Vector3f* Normal_array::get_vector() { return &(*(m_array.begin())); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Normal_array::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
