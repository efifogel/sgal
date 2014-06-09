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

#ifndef SGAL_TEX_COORD_ARRAY_2D_HPP
#define SGAL_TEX_COORD_ARRAY_2D_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Tex_coord_array_2d Tex_coord_array_2d.hpp
 * Tex_coord_array maintains an array of 2D texture-coordinates of floating
 * point type.
 */
class SGAL_SGAL_DECL Tex_coord_array_2d : public Tex_coord_array {
public:
  enum {
    FIRST = Tex_coord_array::LAST - 1,
    POINT,
    LAST
  };

  typedef std::vector<Vector2f>::iterator       iterator;
  typedef std::vector<Vector2f>::const_iterator const_iterator;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Tex_coord_array_2d(Boolean proto = false);

  /*! Constructor. */
  Tex_coord_array_2d(Uint n);

  /*! Destructor. */
  virtual ~Tex_coord_array_2d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Tex_coord_array_2d* prototype();

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

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  std::vector<Vector2f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size. */
  virtual Uint size() const;

  /*! Resize the array capacity .*/
  virtual void resize(Uint n);

  /*! Clear the array. */
  virtual void clear();

  /*! Obtain the number of texture coordinate dimensions.
   * \return the number of texture coordinate dimensions.
   */
  virtual Uint num_coordinates() const;

  /*! Obtain the data size.
   * \retun the data size.
   */
  virtual Uint data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const;

  /*! Obtain the datum at a given index.
   * \param i (in) the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const;

  /*! Obtain the iterator to the Array first element. */
  std::vector<Vector2f>::iterator begin();
  const std::vector<Vector2f>::const_iterator begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  std::vector<Vector2f>::iterator end();
  const std::vector<Vector2f>::const_iterator end() const;

  /*! Obtain the nth element in the array (non-const). */
  Vector2f& operator[](Uint n);

  /*! Obtain the nth element in the array (const). */
  const Vector2f& operator[](Uint n) const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The normal array. */
  std::vector<Vector2f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Tex_coord_array_2d* Tex_coord_array_2d::prototype()
{ return new Tex_coord_array_2d(true); }

//! \brief clones.
inline Container* Tex_coord_array_2d::clone()
{ return new Tex_coord_array_2d(); }

//! \brief obtains the array size.
inline Uint Tex_coord_array_2d::size() const { return m_array.size(); }

//! \brief resizes the array capacity.
inline void Tex_coord_array_2d::resize(Uint n) { m_array.resize(n); }

//! \brief clears the array.
inline void Tex_coord_array_2d::clear() { m_array.clear(); }

//! \brief obtains the iterator to the Array first element.
inline std::vector<Vector2f>::iterator Tex_coord_array_2d::begin()
{ return m_array.begin(); }
inline const std::vector<Vector2f>::const_iterator Tex_coord_array_2d::begin()
  const
{ return m_array.begin(); }

//! \brief obtains the iterator to the Array past-the-end element.
inline std::vector<Vector2f>::iterator Tex_coord_array_2d::end()
{ return m_array.end(); }
inline const std::vector<Vector2f>::const_iterator Tex_coord_array_2d::end()
  const
{ return m_array.end(); }

//! \brief obtains the nth element in the array (non-const).
inline Vector2f& Tex_coord_array_2d::operator[](Uint n) { return m_array[n]; }

//! \brief obtains the nth element in the array (const).
inline const Vector2f& Tex_coord_array_2d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtains the number of texture coordinate dimensions.
inline Uint Tex_coord_array_2d::num_coordinates() const { return 2; }

//! \brief obtain the data size.
inline Uint Tex_coord_array_2d::data_size() const
{ return m_array.size() * sizeof(Vector2f); }

//! \brief obtains the data.
inline const GLfloat* Tex_coord_array_2d::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the datum at a given index.
inline const GLfloat* Tex_coord_array_2d::datum(Uint i) const
{ return (GLfloat*)(&(m_array[i])); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Tex_coord_array_2d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
