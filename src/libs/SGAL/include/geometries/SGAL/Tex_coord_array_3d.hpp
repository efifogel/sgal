// Copyright (c) 2013 Israel.
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

#ifndef SGAL_TEX_COORD_ARRAY_3D_HPP
#define SGAL_TEX_COORD_ARRAY_3D_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL Array<Vector3f>;
#endif

/*! This class maintains an array of vertex-texture-coordinates pairs */
class SGAL_SGAL_DECL Tex_coord_array_3d : public Tex_coord_array {
public:
  enum {
    FIRST = Tex_coord_array::LAST - 1,
    POINT,
    LAST
  };

  /*! Constructor */
  Tex_coord_array_3d(Boolean proto = false);

  /*! Constructor */
  Tex_coord_array_3d(Uint n);

  /*! Destructor */
  ~Tex_coord_array_3d();

  /* Construct the prototype. */
  static Tex_coord_array_3d* prototype() { return new Tex_coord_array_3d(true); }

  /*! Clone. */
  virtual Container* clone() { return new Tex_coord_array_3d(); }

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Sets the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  virtual Uint size() const;

  /*! Resize the array capacity. */
  virtual void resize(Uint n);

  /*! Obtain the GL data. */
  virtual GLfloat* get_gl_data();

  /*! Clear the array. */
  virtual void clear();

  /*! Obtain the iterator to the Array first element. */
  Vector3f* begin();
  const Vector3f* begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  Vector3f* end();
  const Vector3f* end() const;

  /*! Obtain the nth element in the array (non-const). */
  Vector3f& operator[](Uint n);

  /*! Obtain the nth element in the array (const). */
  const Vector3f& operator[](Uint n) const;

  /*! Obtain the vector. */
  Vector3f* get_vector();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The normal array. */
  SGAL::Array<Vector3f> m_array;
};

/*! \brief obtains the array size. */
inline Uint Tex_coord_array_3d::size() const { return m_array.size(); }

/*! \brief Obtain the GL data. */
inline GLfloat* Tex_coord_array_3d::get_gl_data()
{ return (GLfloat*)(get_vector()); }

/*! \brief Resize the array capacity */
inline void Tex_coord_array_3d::resize(Uint n) { m_array.resize(n); }

/*! \brief Clear the array */
inline void Tex_coord_array_3d::clear() { m_array.clear(); }

/*! \brief obtains the iterator to the Array first element */
inline Vector3f* Tex_coord_array_3d::begin() { return m_array.begin(); }
inline const Vector3f* Tex_coord_array_3d::begin() const
{ return m_array.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element */
inline Vector3f* Tex_coord_array_3d::end() { return m_array.end(); }
inline const Vector3f* Tex_coord_array_3d::end() const { return m_array.end(); }

/*! \brief obtains the Array indexing (non-const) operator */
inline Vector3f& Tex_coord_array_3d::operator[](Uint n) { return m_array[n]; }

/*! \brief obtains the Array indexing (const) operator */
inline const Vector3f& Tex_coord_array_3d::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief Obtain the vector */
inline Vector3f* Tex_coord_array_3d::get_vector()
{ return m_array.get_vector(); }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Tex_coord_array_3d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
