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

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

/*! This class maintains an array of vertex-normals */
class SGAL_SGAL_DECL Normal_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    VECTOR,
    LAST
  };

  /*! Constructor */
  Normal_array(Boolean proto = false) : Container(proto) {}

  /*! Constructor */
  Normal_array(Uint n) { m_array.resize(n); }

  /*! Destructor */
  virtual ~Normal_array() {}

  /* Construct the prototype */
  static Normal_array* prototype() { return new Normal_array(true); }

  /*! Clone */
  virtual Container* clone() { return new Normal_array(); }

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
  Uint size() const { return m_array.size(); }

  /*! Resize the array capacity */
  void resize(Uint n) { m_array.resize(n); }

  /*! Clear the array */
  void clear() { m_array.clear(); }

  /*! The iterator to the Array first element */
  Vector3f* begin() { return m_array.begin(); }
  const Vector3f* begin() const { return m_array.begin(); }

  /*! The iterator to the Array past-the-end element */
  Vector3f* end() { return m_array.end(); }
  const Vector3f* end() const { return m_array.end(); }

  /*! Array indexing operator */
  Vector3f& operator[](Uint n) { return m_array[n]; }

  /*! Array indexing operator */
  const Vector3f& operator[](Uint n) const { return m_array[n]; }

  /*! Obtain the vector */
  Vector3f* get_vector() { return m_array.get_vector(); }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The normal array */
  SGAL::Array<Vector3f> m_array;
};

SGAL_END_NAMESPACE

#endif
