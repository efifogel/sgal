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

#ifndef SCGAL_EXACT_NORMAL_ARRAY_HPP
#define SCGAL_EXACT_NORMAL_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Normal_array.hpp"

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

/*! Maintains an array of 3D normals */
class SGAL_SCGAL_DECL Exact_normal_array : public Normal_array {
public:
  typedef std::vector<Exact_vector_3>                   Exact_vector_vector;
  typedef Exact_vector_vector::iterator                 Exact_vector_iter;
  typedef Exact_vector_vector::const_iterator           Exact_vector_const_iter;

  enum {
    FIRST = Normal_array::LAST - 1,
    EXACT_POINT,
    LAST
  };

  /*! Construct. */
  Exact_normal_array(Boolean proto = false) : Normal_array(proto) {}

  /*! Construct. */
  Exact_normal_array(Size n) { m_array.resize(n); }

  /*! Destruct. */
  virtual ~Exact_normal_array() { clear(); }

  /* Construct the prototype. */
  static Exact_normal_array* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtains the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Obain the size. */
  Size size() const;

  /*! Resize. */
  void resize(Size n);

  /*! Clear the array. */
  void clear();

  /*! Begin. */
  Exact_vector_iter begin();
  Exact_vector_const_iter begin() const;

  /*! End. */
  Exact_vector_iter end();
  Exact_vector_const_iter end() const;

  /*! Array indexing operator. */
  Exact_vector_3& operator[](Uint n);

  /*! Array indexing operator. */
  const Exact_vector_3& operator[](Uint n) const;

  /*! Inserts a new element at the end. */
  void push_back(const Exact_vector_3& p);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The exact normal array. */
  Exact_vector_vector m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/* \brief constructs the prototype. */
inline Exact_normal_array* Exact_normal_array::prototype()
{ return new Exact_normal_array(true); }

/*! \brief clones. */
inline Container* Exact_normal_array::clone()
{ return new Exact_normal_array(); }

/*! \brief obains the size. */
inline Size Exact_normal_array::size() const { return m_array.size(); }

/*! \brief resizes. */
inline void Exact_normal_array::resize(Size n) { m_array.resize(n); }

/*! \brief clears the array */
inline void Exact_normal_array::clear() { m_array.clear(); }

/*! \brief obtains the begin iterator. */
inline Exact_normal_array::Exact_vector_iter Exact_normal_array::begin()
{ return m_array.begin(); }

inline Exact_normal_array::Exact_vector_const_iter Exact_normal_array::begin()
  const
{ return m_array.begin(); }

/*! \brief obtains the pass-the-end iterator. */
inline Exact_normal_array::Exact_vector_iter Exact_normal_array::end()
{ return m_array.end(); }

inline Exact_normal_array::Exact_vector_const_iter Exact_normal_array::end()
  const
{ return m_array.end(); }

/*! \brief array indexing (non-const) operator. */
inline Exact_vector_3& Exact_normal_array::operator[](Uint n)
{ return m_array[n]; }

/*! \brief array indexing (const) operator */
inline const Exact_vector_3& Exact_normal_array::operator[](Uint n) const
{ return m_array[n]; }

/*! \brief inserts a new element at the end. */
inline void Exact_normal_array::push_back(const Exact_vector_3& p)
{ m_array.push_back(p); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Exact_normal_array::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
