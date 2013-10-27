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

#ifndef SGAL_TEX_COORD_ARRAY_2D_HPP
#define SGAL_TEX_COORD_ARRAY_2D_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
template class SGAL_SGAL_DECL Array<Vector2f>;
#endif

/*! This class maintains an array of vertex-texture-coordinates pairs */
class SGAL_SGAL_DECL Tex_coord_array_2d : public Tex_coord_array {
public:
  enum {
    FIRST = Tex_coord_array::LAST - 1,
    POINT,
    LAST
  };

  /*! Constructor */
  Tex_coord_array_2d(Boolean proto = false);

  /*! Constructor */
  Tex_coord_array_2d(Uint n);

  /*! Destructor */
  virtual ~Tex_coord_array_2d();

  /* Construct the prototype */
  static Tex_coord_array_2d* prototype();

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

  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  virtual Uint size() const;

  /*! Resize the array capacity .*/
  virtual void resize(Uint n);

  /*! Obtain the GL data. */
  virtual GLfloat* get_gl_data();

  /*! Clear the array. */
  virtual void clear();

  /*! Obtain the iterator to the Array first element. */
  Vector2f* begin();
  const Vector2f* begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  Vector2f* end();
  const Vector2f* end() const;

  /*! Obtain the nth element in the array (non-const). */
  Vector2f& operator[](Uint n);

  /*! Obtain the nth element in the array (const). */
  const Vector2f& operator[](Uint n) const;

  /*! Obtain the vector. */
  Vector2f* get_vector();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The normal array. */
  SGAL::Array<Vector2f> m_array;
};

/* \brief constructs the prototype. */
inline Tex_coord_array_2d* Tex_coord_array_2d::prototype()
{ return new Tex_coord_array_2d(true); }

/*! \brief clones. */
inline Container* Tex_coord_array_2d::clone()
{ return new Tex_coord_array_2d(); }

/*! \brief obtains the array size. */
inline Uint Tex_coord_array_2d::size() const { return m_array.size(); }

/*! \brief obtains the GL data. */
inline GLfloat* Tex_coord_array_2d::get_gl_data()
{ return (GLfloat*)(get_vector()); }

/*! \brief resizes the array capacity .*/
inline void Tex_coord_array_2d::resize(Uint n) { m_array.resize(n); }

/*! \brief clears the array. */
inline void Tex_coord_array_2d::clear() { m_array.clear(); }

/*! \brief obtains the iterator to the Array first element. */
inline Vector2f* Tex_coord_array_2d::begin() { return m_array.begin(); }
inline const Vector2f* Tex_coord_array_2d::begin() const
{ return m_array.begin(); }

/*! \brief obtains the iterator to the Array past-the-end element. */
inline Vector2f* Tex_coord_array_2d::end() { return m_array.end(); }
inline const Vector2f* Tex_coord_array_2d::end() const { return m_array.end(); }

/*! \brief obtains the nth element in the array (non-const). */
inline Vector2f& Tex_coord_array_2d::operator[](Uint n) { return m_array[n]; }

/*! \brief obtains the nth element in the array (const). */
inline const Vector2f& Tex_coord_array_2d::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief obtains the vector. */
inline Vector2f* Tex_coord_array_2d::get_vector()
{ return m_array.get_vector(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Tex_coord_array_2d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
