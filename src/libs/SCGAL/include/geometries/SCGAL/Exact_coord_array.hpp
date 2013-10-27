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
// $Id: Exact_coord_array.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EXACT_COORD_ARRAY_HPP
#define SCGAL_EXACT_COORD_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Coord_array.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Exact_kernel.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

#if defined(_MSC_VER)
template class SGAL_SGAL_DECL std::allocator<Exact_point_3>;
template class SGAL_SGAL_DECL std::vector<Exact_point_3>;
template class SGAL_SGAL_DECL std::allocator<Exact_vector_3>;
template class SGAL_SGAL_DECL std::vector<Exact_vector_3>;
#endif

/*! Maintains an array of 3D vertex-coordinate */
class SGAL_SCGAL_DECL Exact_coord_array : public Coord_array {
public:
  typedef std::vector<Exact_point_3>                    Exact_point_vector;
  typedef Exact_point_vector::iterator                  Exact_point_iter;
  typedef Exact_point_vector::const_iterator            Exact_point_const_iter;

  enum {
    FIRST = Coord_array::LAST - 1,
    EXACT_POINT,
    LAST
  };

  /*! Constructor */
  Exact_coord_array(Boolean proto = false) : Coord_array(proto) {}

  /*! Constructor */
  Exact_coord_array(Uint n) { m_array.resize(n); }

  /*! Destructor */
  virtual ~Exact_coord_array() { clear(); }

  /* Construct the prototype */
  static Exact_coord_array* prototype();

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

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Size */
  Uint size() const { return m_array.size(); }

  /*! Resize */
  void resize(Uint n) { m_array.resize(n); }

  /*! Clear the array */
  void clear() { m_array.clear(); }

  /*! Begin */
  Exact_point_iter begin() { return m_array.begin(); }
  Exact_point_const_iter begin() const { return m_array.begin(); }

  /*! End */
  Exact_point_iter end() { return m_array.end(); }
  Exact_point_const_iter end() const { return m_array.end(); }

  /*! Array indexing operator */
  Exact_point_3& operator[](Uint n) { return m_array[n]; }

  /*! Array indexing operator */
  const Exact_point_3& operator[](Uint n) const { return m_array[n]; }

  /*! Inserts a new element at the end */
  void push_back(const Exact_point_3& p) { m_array.push_back(p); }

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The exact coordinate array */
  Exact_point_vector m_array;
};

/* \brief constructs the prototype. */
inline Exact_coord_array* Exact_coord_array::prototype()
{ return new Exact_coord_array(true); }

/*! \brief clones. */
inline Container* Exact_coord_array::clone() { return new Exact_coord_array(); }

SGAL_END_NAMESPACE

#endif
