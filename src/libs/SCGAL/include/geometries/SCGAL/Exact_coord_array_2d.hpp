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

#ifndef SCGAL_EXACT_COORD_ARRAY_2D_HPP
#define SCGAL_EXACT_COORD_ARRAY_2D_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Vector2f.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Maintains an array of 3D vertex-coordinate */
class SGAL_SCGAL_DECL Exact_coord_array_2d : public Coord_array {
public:
  typedef std::vector<Exact_point_2>                    Exact_point_vector;
  typedef Exact_point_vector::iterator                  Exact_point_iter;
  typedef Exact_point_vector::const_iterator            Exact_point_const_iter;

  enum {
    FIRST = Coord_array::LAST - 1,
    POINT,
    LAST
  };

  /*! Constructor. */
  Exact_coord_array_2d(Boolean proto = false);

  /*! Constructor.
   * \param n (in) the initial size of the array.
   */
  Exact_coord_array_2d(Uint n);

  /*! Destructor. */
  virtual ~Exact_coord_array_2d();

  /* Construct the prototype. */
  static Exact_coord_array_2d* prototype();

  /*! Clone. */
  virtual Container* clone();

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
  virtual Uint size() const;

  /*! Resize.
   * \param n (in) the new size.
   */
  virtual void resize(Uint n);

  /*! Clear the array */
  virtual void clear();

  /*! Obtain the number of coordinate dimensions.
   * \return the number of coordinate dimensions.
   */
  virtual Uint num_coordinates() const;

  /*! Obtain the data size.
    \return the data size.
   */
  virtual Uint data_size() const;

  /*! Obtain the data.
   * \return the data.
   */
  virtual const GLfloat* data() const;

  /*! Obtain the begin iterator. */
  Exact_point_iter begin();
  Exact_point_const_iter begin() const;

  /*! Obtain the pass-the-end iterator. */
  Exact_point_iter end();
  Exact_point_const_iter end() const;

  /*! Array indexing operator.
   */
  Exact_point_2& operator[](Uint n);

  /*! Array indexing operator.
   */
  const Exact_point_2& operator[](Uint n) const;

  /*! Inserts a new element at the end.
   */
  void push_back(const Exact_point_2& p);

  /*! Obtain the inexact coordinates.
   * \return the inexact coordinates.
   */
  const std::vector<Vector2f>& get_inexact_coords() const;

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

private:

  /*! Clean the raw inexact coordinates. */
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
  mutable std::vector<Vector2f> m_inexact_coords;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Exact_coord_array_2d* Exact_coord_array_2d::prototype()
{ return new Exact_coord_array_2d(true); }

//! \brief clones.
inline Container* Exact_coord_array_2d::clone()
{ return new Exact_coord_array_2d(); }

//! \brief obtains the size.
inline Uint  Exact_coord_array_2d::size() const { return m_array.size(); }

//! \brief resizes. */
inline void  Exact_coord_array_2d::resize(Uint n) { m_array.resize(n); }

//! \brief obtains the number of coordinate dimensions.
inline Uint Exact_coord_array_2d::num_coordinates() const { return 2; }

//! \brief obtains the begin iterator.
inline Exact_coord_array_2d::Exact_point_iter Exact_coord_array_2d::begin()
{ return m_array.begin(); }

inline Exact_coord_array_2d::Exact_point_const_iter
Exact_coord_array_2d::begin() const
{ return m_array.begin(); }

//! \brief obtains the pass-the-end iterator.
inline Exact_coord_array_2d::Exact_point_iter Exact_coord_array_2d::end()
{ return m_array.end(); }

inline Exact_coord_array_2d::Exact_point_const_iter
Exact_coord_array_2d::end() const
{ return m_array.end(); }

//! \brief array indexing operator.
inline Exact_point_2& Exact_coord_array_2d::operator[](Uint n)
{ return m_array[n]; }

//! \brief array indexing operator.
inline const Exact_point_2& Exact_coord_array_2d::operator[](Uint n) const
{ return m_array[n]; }

//! \brief inserts a new element at the end.
inline void Exact_coord_array_2d::push_back(const Exact_point_2& p)
{ m_array.push_back(p); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exact_coord_array_2d::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
