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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TEX_COORD_ARRAY_3D_HPP
#define SGAL_TEX_COORD_ARRAY_3D_HPP

#include <string>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Tex_coord_array_3d Tex_coord_array_3d.hpp
 * Tex_coord_array maintains an array of 3D texture-coordinates of floating
 * point type.
 */
class SGAL_SGAL_DECL Tex_coord_array_3d : public Tex_coord_array {
public:
  enum {
    FIRST = Tex_coord_array::LAST - 1,
    POINT,
    LAST
  };

  typedef std::vector<Vector3f>::iterator       iterator;
  typedef std::vector<Vector3f>::const_iterator const_iterator;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Tex_coord_array_3d(Boolean proto = false);

  /*! Constructor. */
  Tex_coord_array_3d(Uint n);

  /*! Destructor */
  ~Tex_coord_array_3d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Tex_coord_array_3d* prototype();

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
  std::vector<Vector3f>* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual void FieldChanged(short fieldId);
  //! \todo virtual Attribute_list get_attributes();

  /*! Obtain the array size.
   * \retun the array size.
   */
  virtual Uint size() const;

  /*! Resize the array capacity. */
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

  /*! Obtain the iterator to the Array first element. */
  std::vector<Vector3f>::iterator begin();
  const std::vector<Vector3f>::const_iterator begin() const;

  /*! Obtain the iterator to the Array past-the-end element. */
  std::vector<Vector3f>::iterator end();
  const std::vector<Vector3f>::const_iterator end() const;

  /*! Obtain the nth element in the array (non-const). */
  Vector3f& operator[](Uint n);

  /*! Obtain the nth element in the array (const). */
  const Vector3f& operator[](Uint n) const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The normal array. */
  std::vector<Vector3f> m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs.
inline Tex_coord_array_3d* Tex_coord_array_3d::prototype()
{ return new Tex_coord_array_3d(true); }

//! \brief clones.
inline Container* Tex_coord_array_3d::clone()
{ return new Tex_coord_array_3d(); }

//! \brief obtains the array size.
inline Uint Tex_coord_array_3d::size() const { return m_array.size(); }

//! \brief Resize the array capacity.
inline void Tex_coord_array_3d::resize(Uint n) { m_array.resize(n); }

//! \brief Clear the array.
inline void Tex_coord_array_3d::clear() { m_array.clear(); }

//! \brief obtains the iterator to the Array first element.
inline std::vector<Vector3f>::iterator Tex_coord_array_3d::begin()
{ return m_array.begin(); }

inline const std::vector<Vector3f>::const_iterator Tex_coord_array_3d::begin()
  const
{ return m_array.begin(); }

//! \brief obtains the iterator to the Array past-the-end element.
inline std::vector<Vector3f>::iterator Tex_coord_array_3d::end()
{ return m_array.end(); }

inline const std::vector<Vector3f>::const_iterator Tex_coord_array_3d::end()
  const
{ return m_array.end(); }

//! \brief obtains the Array indexing (non-const) operator.
inline Vector3f& Tex_coord_array_3d::operator[](Uint n) { return m_array[n]; }

//! \brief obtains the Array indexing (const) operator.
inline const Vector3f& Tex_coord_array_3d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief obtains the number of texture coordinate dimensions.
inline Uint Tex_coord_array_3d::num_coordinates() const { return 3; }

//! \brief obtain the data size.
inline Uint Tex_coord_array_3d::data_size() const
{ return m_array.size() * sizeof(Vector3f); }

//! \brief obtains the data.
inline const GLfloat* Tex_coord_array_3d::data() const
{ return (GLfloat*)(&(*(m_array.begin()))); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Tex_coord_array_3d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
