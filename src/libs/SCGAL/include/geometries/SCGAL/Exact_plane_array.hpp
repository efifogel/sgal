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
// $Revision: 9188 $
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EXACT_PLANE_ARRAY_HPP
#define SCGAL_EXACT_PLANE_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

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
class SGAL_SCGAL_DECL Exact_plane_array : public Container {
public:
  typedef std::vector<Exact_plane_3>                    Exact_plane_vector;
  typedef Exact_plane_vector::iterator                  Exact_plane_iter;
  typedef Exact_plane_vector::const_iterator            Exact_plane_const_iter;

  enum {
    FIRST = Container::LAST - 1,
    EXACT_POINT,
    LAST
  };

  /*! Constructor. */
  Exact_plane_array(Boolean proto = false);

  /*! Constructor. */
  Exact_plane_array(Uint n);

  /*! Destructor. */
  virtual ~Exact_plane_array();

  /* Construct the prototype. */
  static Exact_plane_array* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Size. */
  Uint size() const;

  /*! Resize. */
  void resize(Uint n);

  /*! Clear the array. */
  void clear();

  /*! Begin. */
  Exact_plane_iter begin();
  Exact_plane_const_iter begin() const;

  /*! End. */
  Exact_plane_iter end();
  Exact_plane_const_iter end() const;

  /*! Array indexing operator. */
  Exact_plane_3& operator[](Uint n);

  /*! Array indexing operator. */
  const Exact_plane_3& operator[](Uint n) const;

  /*! Insert a new element at the end. */
  void push_back(const Exact_plane_3& p);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! The exact coordinate array.
  Exact_plane_vector m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Exact_plane_array* Exact_plane_array::prototype()
{ return new Exact_plane_array(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Exact_plane_array::create()
{ return new Exact_plane_array(); }

//! \brief Size.
inline Uint Exact_plane_array::size() const { return m_array.size(); }

//! \brief Resize.
inline void Exact_plane_array::resize(Uint n) { m_array.resize(n); }

//! \brief clears the array.
inline void Exact_plane_array::clear() { m_array.clear(); }

//! \brief obtains the (non-const) begin iterator.
inline Exact_plane_array::Exact_plane_iter Exact_plane_array::begin()
{ return m_array.begin(); }

//! \brief obtains the (const) begin iterator.
inline Exact_plane_array::Exact_plane_const_iter Exact_plane_array::begin()
  const
{ return m_array.begin(); }

//! \brief obtains the (non-const) pass-the-end iterator.
inline Exact_plane_array::Exact_plane_iter
Exact_plane_array::end() { return m_array.end(); }

//! \brief obtains the (const) pass-the-end iterator.
inline Exact_plane_array::Exact_plane_const_iter Exact_plane_array::end() const
{ return m_array.end(); }

//! \brief array indexing (non-const) operator.
inline Exact_plane_3& Exact_plane_array::operator[](Uint n)
{ return m_array[n]; }

//! \brief array indexing (const) operator.
inline const Exact_plane_3& Exact_plane_array::operator[](Uint n) const
{ return m_array[n]; }

//! \brief inserts a new element at the end.
inline void Exact_plane_array::push_back(const Exact_plane_3& p)
{ m_array.push_back(p); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Exact_plane_array::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
