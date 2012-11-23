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
// $Source$
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_EXACT_PLANE_ARRAY_HPP
#define SGAL_EXACT_PLANE_ARRAY_HPP

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>

#include "SCGAL/Exact_kernel.hpp"

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Scene_graph;
class Container_proto;

/*! Maintains an array of 3D vertex-coordinate */
class Exact_plane_array : public Container {
public:
  typedef std::vector<Exact_plane_3>                    Exact_plane_vector;
  typedef Exact_plane_vector::iterator                  Exact_plane_iter;
  typedef Exact_plane_vector::const_iterator            Exact_plane_const_iter;

  enum {
    FIRST = Container::LAST - 1,
    EXACT_POINT,
    LAST
  };

  /*! Constructor */
  Exact_plane_array(Boolean proto = SGAL_FALSE) : Container(proto) {}

  /*! Constructor */
  Exact_plane_array(Uint n) { m_array.resize(n); }

  /*! Destructor */
  virtual ~Exact_plane_array() { clear(); }

  /* Construct the prototype */
  static Exact_plane_array * prototype()
  { return new Exact_plane_array(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Exact_plane_array(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */  
  virtual Container_proto * get_prototype();
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  /*! Size */
  Uint size() const { return m_array.size(); }
  
  /*! Resize */
  void resize(Uint n) { m_array.resize(n); }

  /*! Clear the array */
  void clear() { m_array.clear(); }

  /*! Begin */
  Exact_plane_iter begin() { return m_array.begin(); }
  Exact_plane_const_iter begin() const { return m_array.begin(); }
  
  /*! End */
  Exact_plane_iter end() { return m_array.end(); }
  Exact_plane_const_iter end() const { return m_array.end(); }

  /*! Array indexing operator */
  Exact_plane_3 & operator[](Uint n) { return m_array[n]; }

  /*! Array indexing operator */
  const Exact_plane_3 & operator[](Uint n) const { return m_array[n]; }

  /*! Inserts a new element at the end */
  void push_back(const Exact_plane_3 & p) { m_array.push_back(p); }
    
protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The exact coordinate array */
  Exact_plane_vector m_array;
};

SGAL_END_NAMESPACE

#endif
