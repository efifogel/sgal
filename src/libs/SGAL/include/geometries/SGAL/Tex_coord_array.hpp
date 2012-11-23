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
// $Source$
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TEX_COORD_ARRAY_HPP
#define SGAL_TEX_COORD_ARRAY_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

/*! This class maintains an array of vertex-texture-coordinates pairs */
class Tex_coord_array : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    POINT,
    LAST
  };

  /*! Constructor */
  Tex_coord_array(Boolean proto = SGAL_FALSE) : Container(proto) {}

  /*! Constructor */
  Tex_coord_array(Uint n) { m_array.resize(n); }
  
  /*! Destructor */
  virtual ~Tex_coord_array() {}

  /* Construct the prototype */
  static Tex_coord_array * prototype() { return new Tex_coord_array(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Tex_coord_array(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */  
  virtual Container_proto * get_prototype();
  
  /*! Sets the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size */
  Uint size() const { return m_array.size(); }

  /*! Resize the array capacity */
  void resize(Uint n) { m_array.resize(n); }

  /*! Clear the array */
  void clear() { m_array.clear(); }

  /*! The iterator to the Array first element */
  Vector2f * begin() { return m_array.begin(); }
  const Vector2f * begin() const { return m_array.begin(); }

  /*! The iterator to the Array past-the-end element */
  Vector2f * end() { return m_array.end(); }
  const Vector2f * end() const { return m_array.end(); }
  
  /*! Array indexing operator */
  Vector2f & operator[](Uint n) { return m_array[n]; }

  /*! Array indexing operator */
  const Vector2f & operator[](Uint n) const { return m_array[n]; }
  
  /*! Obtain the vector */
  Vector2f * get_vector() { return m_array.get_vector(); }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The normal array */
  SGAL::Array<Vector2f> m_array;
};

SGAL_END_NAMESPACE

#endif
