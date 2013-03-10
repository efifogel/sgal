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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EXACT_NORMAL_ARRAY_HPP
#define SGAL_EXACT_NORMAL_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SCGAL/Exact_kernel.hpp"

#include "SGAL/basic.hpp"
#include "SGAL/Normal_array.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

/*! Maintains an array of 3D normals */
class Exact_normal_array : public Normal_array {
public:
  typedef std::vector<Exact_vector_3>                   Exact_vector_vector;
  typedef Exact_vector_vector::iterator                 Exact_vector_iter;
  typedef Exact_vector_vector::const_iterator           Exact_vector_const_iter;

  enum {
    FIRST = Normal_array::LAST - 1,
    EXACT_POINT,
    LAST
  };

  /*! Constructor */
  Exact_normal_array(Boolean proto = SGAL_FALSE) : Normal_array(proto) {}

  /*! Constructor */
  Exact_normal_array(Uint n) { m_array.resize(n); }

  /*! Destructor */
  virtual ~Exact_normal_array() { clear(); }

  /* Construct the prototype */
  static Exact_normal_array* prototype()
  { return new Exact_normal_array(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Exact_normal_array(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */  
  virtual Container_proto* get_prototype();
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Size */
  Uint size() const { return m_array.size(); }
  
  /*! Resize */
  void resize(Uint n) { m_array.resize(n); }

  /*! Clear the array */
  void clear() { m_array.clear(); }

  /*! Begin */
  Exact_vector_iter begin() { return m_array.begin(); }
  Exact_vector_const_iter begin() const { return m_array.begin(); }
  
  /*! End */
  Exact_vector_iter end() { return m_array.end(); }
  Exact_vector_const_iter end() const { return m_array.end(); }

  /*! Array indexing operator */
  Exact_vector_3& operator[](Uint n) { return m_array[n]; }

  /*! Array indexing operator */
  const Exact_vector_3& operator[](Uint n) const { return m_array[n]; }

  /*! Inserts a new element at the end */
  void push_back(const Exact_vector_3& p) { m_array.push_back(p); }
    
protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The exact normal array */
  Exact_vector_vector m_array;
};

SGAL_END_NAMESPACE

#endif
