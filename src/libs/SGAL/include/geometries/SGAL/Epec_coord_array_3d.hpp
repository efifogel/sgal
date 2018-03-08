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

#ifndef SGAL_EPEC_COORD_ARRAY_3D_HPP
#define SGAL_EPEC_COORD_ARRAY_3D_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Epec_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Maintains an array of 3D vertex-coordinate */
class SGAL_SGAL_DECL Epec_coord_array_3d : public Coord_array {
public:
  typedef std::vector<Epec_point_3>                     Exact_point_vector;
  typedef Exact_point_vector::iterator                  Exact_point_iter;
  typedef Exact_point_vector::const_iterator            Exact_point_const_iter;

  typedef std::vector<Epec_point_3>::iterator           iterator;
  typedef std::vector<Epec_point_3>::const_iterator     const_iterator;

  enum {
    FIRST = Coord_array::LAST - 1,
    POINT,
    LAST
  };

  /*! Construct.
   * \param[in] proto determines whether to construct a prototype.
   */
  Epec_coord_array_3d(Boolean proto = false);

  /*! Construct.
   * \param[in] n the initial size of the array.
   */
  Epec_coord_array_3d(Size n);

  /*! Destruct. */
  virtual ~Epec_coord_array_3d();

  /* Construct the prototype. */
  static Epec_coord_array_3d* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Exact_point_vector* array_handle(const Field_info*) { return &m_array; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Obtain the size.
   */
  virtual Size size() const;

  /*! Resize.
   * \param n (in) the new size.
   */
  virtual void resize(Size n);

  /*! Clear the array */
  virtual void clear();

  /*! Obtain the number of coordinate dimensions.
   * \return the number of coordinate dimensions.
   */
  virtual Size num_coordinates() const;

  /*! Obtain the data size.
    \return the data size.
   */
  virtual Size data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const;

  /*! Obtain the datum at a given index.
   * \param i (in) the index of the obtained datum.
   * \return the datum at a given index.
   */
  virtual const GLfloat* datum(Uint i) const;

  /*! Obtain the begin iterator. */
  Exact_point_iter begin();
  Exact_point_const_iter begin() const;

  /*! Obtain the pass-the-end iterator. */
  Exact_point_iter end();
  Exact_point_const_iter end() const;

  /*! Array indexing operator.
   */
  Epec_point_3& operator[](Uint n);

  /*! Array indexing operator.
   */
  const Epec_point_3& operator[](Uint n) const;

  /*! Inserts a new element at the end.
   */
  void push_back(const Epec_point_3& p);

  /*! Obtain the inexact coordinates.
   * \return the inexact coordinates.
   */
  const std::vector<Vector3f>& get_inexact_coords() const;

  /*! Obtain the ith inexact coordinate.
   * \param i (in) the index of the inexact coordinate to return.
   * \return the inexact coordinate.
   */
  const Vector3f& get_inexact_coord(Uint i) const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:
  /*! Clean the inexact coordinates. */
  void clean_inexact_coords() const;

  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The exact coordinate array */
  Exact_point_vector m_array;

  /*! Indicates whether the inexact coordinates is dirty. */
  mutable bool m_dirty_inexact_coords;

  /*! The inexact coordinates. */
  mutable std::vector<Vector3f> m_inexact_coords;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief destruct.
inline Epec_coord_array_3d::~Epec_coord_array_3d() { clear(); }

//! \brief constructs the prototype.
inline Epec_coord_array_3d* Epec_coord_array_3d::prototype()
{ return new Epec_coord_array_3d(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Epec_coord_array_3d::create()
{ return new Epec_coord_array_3d(); }

//! \brief obtains the size.
inline Size  Epec_coord_array_3d::size() const { return m_array.size(); }

//! \brief resizes. */
inline void  Epec_coord_array_3d::resize(Size n) { m_array.resize(n); }

//! \brief obtains the number of coordinate dimensions.
inline Size Epec_coord_array_3d::num_coordinates() const { return 3; }

//! \brief obtains the begin iterator.
inline Epec_coord_array_3d::Exact_point_iter Epec_coord_array_3d::begin()
{ return m_array.begin(); }

inline Epec_coord_array_3d::Exact_point_const_iter
Epec_coord_array_3d::begin() const
{ return m_array.begin(); }

//! \brief obtains the pass-the-end iterator.
inline Epec_coord_array_3d::Exact_point_iter Epec_coord_array_3d::end()
{ return m_array.end(); }

inline Epec_coord_array_3d::Exact_point_const_iter Epec_coord_array_3d::end()
  const
{ return m_array.end(); }

//! \brief array indexing operator.
inline Epec_point_3& Epec_coord_array_3d::operator[](Uint n)
{ return m_array[n]; }

//! \brief array indexing operator.
inline const Epec_point_3& Epec_coord_array_3d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief inserts a new element at the end.
inline void Epec_coord_array_3d::push_back(const Epec_point_3& p)
{ m_array.push_back(p); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Epec_coord_array_3d::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
