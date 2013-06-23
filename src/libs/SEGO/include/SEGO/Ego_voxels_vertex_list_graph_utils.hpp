// Copyright (c) 2012 Israel.
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
// Author(s)     : Ophir Setter         <ophir.setter@gmail.com>

#ifndef _EGO_VOXELS_VERTEX_LIST_GRAPH_UTILS_HPP_
#define _EGO_VOXELS_VERTEX_LIST_GRAPH_UTILS_HPP_

#include <functional>

#include "SEGO/basic.hpp"
#include "SEGO/Ego_voxels_vertex_list_graph.hpp"

SGAL_BEGIN_NAMESPACE

class Ego_voxels;

class SGAL_SEGO_DECL Is_inside_voxels :
  public std::unary_function<const Ego_voxels_vertex_list_graph::
                             edge_descriptor&, bool>
{
public:
  typedef Ego_voxels_vertex_list_graph::vertex_descriptor vertex_descriptor;
  typedef Ego_voxels_vertex_list_graph::edge_descriptor   edge_descriptor;

  // TODO: Why do I need default constructor?
  Is_inside_voxels() {}
  Is_inside_voxels(const boost::tuple<std::size_t, std::size_t,
                   std::size_t>& dim) : m_dim(dim) {}
  
  bool operator() (const vertex_descriptor& point) const;
  bool operator() (const edge_descriptor& edge) const;
  
protected:
    boost::tuple<std::size_t, std::size_t, std::size_t> m_dim;
};

class Is_filling_identical :
  public std::unary_function<const Ego_voxels_vertex_list_graph::
                             edge_descriptor&, bool>
{
public:
  typedef Ego_voxels_vertex_list_graph::vertex_descriptor vertex_descriptor;
  typedef Ego_voxels_vertex_list_graph::edge_descriptor   edge_descriptor;

  // TODO: Why do I need default constructor?
  Is_filling_identical() : m_voxels(NULL) {}
    
  Is_filling_identical(const Ego_voxels& voxels) : m_voxels(&voxels) {}
    
  bool operator() (const edge_descriptor& edge) const;
    
protected:
  const Ego_voxels* m_voxels;
};

class Not_self_edge :
  public std::unary_function<const Ego_voxels_vertex_list_graph::
                             edge_descriptor&, bool>
{
public:
  typedef Ego_voxels_vertex_list_graph::edge_descriptor   edge_descriptor;

  bool operator() (const edge_descriptor& edge) const;
};

class Is_same_level :
  public std::unary_function<const Ego_voxels_vertex_list_graph::
                             edge_descriptor&, bool>
{
public:
  typedef Ego_voxels_vertex_list_graph::edge_descriptor   edge_descriptor;

  bool operator() (const edge_descriptor& edge) const;
};

class Is_orthogonal :
  public std::unary_function<const Ego_voxels_vertex_list_graph::
                             edge_descriptor&, bool>
{
public:
  typedef Ego_voxels_vertex_list_graph::edge_descriptor   edge_descriptor;

  bool operator() (const edge_descriptor& edge) const;
};

// Get the neighborhood of a vertex.
// TODO: make this iterator_adaptor
class neighborhood_iterator : public boost::iterator_facade<
  neighborhood_iterator, Ego_voxels_vertex_list_graph::edge_descriptor,
  boost::forward_traversal_tag, Ego_voxels_vertex_list_graph::edge_descriptor>
{
  typedef Ego_voxels_vertex_list_graph::edge_descriptor edge_descriptor;
  typedef Ego_voxels_vertex_list_graph::vertex_descriptor vertex_descriptor;
  typedef Ego_voxels_vertex_list_graph::vertex_iterator vertex_iterator;

public:
  neighborhood_iterator();
  neighborhood_iterator(const vertex_descriptor& c);
  
protected:
  friend class boost::iterator_core_access;
  
  typedef boost::iterator_facade<neighborhood_iterator, edge_descriptor,
                                 boost::forward_traversal_tag, edge_descriptor>
                                                       Base;
  typedef Base                                         Facade;
  
  Facade::reference dereference() const;
  bool equal(const neighborhood_iterator& o) const;
  void increment();
  
private:
  bool is_end() const;
  
  vertex_descriptor m_center;
  vertex_iterator m_begin;
  vertex_iterator m_end;
  bool m_null;
};


SGAL_END_NAMESPACE


#endif

