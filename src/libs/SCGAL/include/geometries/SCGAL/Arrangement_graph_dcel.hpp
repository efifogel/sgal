// Copyright (c) 2008 Israel.
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
// $Id: Arrangement_graph_dcel.hpp 7204 2009-01-24 21:43:15Z efif $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_GRAPH_DCEL_HPP
#define SGAL_ARRANGEMENT_GRAPH_DCEL_HPP

/*! \file
 * An arrangement DCEL, the vertex, halfedge, and face of which are extended
 * each with an adjacency matrix graph . 
 */

#include <boost/graph/adjacency_matrix.hpp>

#include <CGAL/basic.h>
#include <CGAL/Arr_dcel_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <class Point_2>
class Arrangement_graph_vertex : public CGAL::Arr_vertex_base<Point_2> {
public:
  typedef boost::adjacency_matrix<boost::directedS>     Graph;

private:
  /*! An adjacency matrix graph */
  Graph * m_graph;

public:
  /*! Constructor */
  Arrangement_graph_vertex() : m_graph(NULL) {}

  /*! Obtain the graph */
  Graph * graph() { return m_graph; }
  const Graph * graph() const { return m_graph; }

  /*! Set a graph
   * \param graph the graph
   */
  void set_graph(Graph * graph) { m_graph = graph; }

  /*! Assign from another vertex.
   * \param v the other vertex
   */
  virtual void assign(const Arrangement_graph_vertex & v)
  {
    CGAL::Arr_vertex_base<Point_2>::assign(v);
    m_graph = v.m_graph;
  }
};

/*! Extend the arrangement halfedge */
template <class X_monotone_curve_2>
class Arrangement_graph_halfedge :
  public CGAL::Arr_halfedge_base<X_monotone_curve_2>
{
public:
  typedef boost::adjacency_matrix<boost::directedS>     Graph;

private:
  /*! An adjacency matrix graph */
  Graph * m_graph;

public:
  /*! Constructor */
  Arrangement_graph_halfedge() : m_graph(NULL) {}

  /*! Obtain the graph */
  Graph * graph() { return m_graph; }
  const Graph * graph() const { return m_graph; }

  /*! Set a graph
   * \param graph the graph
   */
  void set_graph(Graph * graph) { m_graph = graph; }

  /*! Assign from another halfedge.
   * \param he the other halfedge
   */
  virtual void assign(const Arrangement_graph_halfedge & he)
  {
    CGAL::Arr_halfedge_base<X_monotone_curve_2>::assign(he);
    m_graph = he.m_graph;
  }
};
  
/*! Extend the arrangement face */
class Arrangement_graph_face : public CGAL::Arr_face_base {
public:
  typedef boost::adjacency_matrix<boost::directedS>     Graph;

private:
  /*! An adjacency matrix graph */
  Graph * m_graph;
    
public:
  /*! Constructor */
  Arrangement_graph_face() : m_graph(NULL) {}

  /*! Obtain the graph */
  Graph * graph() { return m_graph; }
  const Graph * graph() const { return m_graph; }

  /*! Set a graph
   * \param graph the graph
   */
  void set_graph(Graph * graph) { m_graph = graph; }

  /*! Assign from another face.
   * \param f the other face
   */
  virtual void assign(const Arrangement_graph_face & f)
  {
    CGAL::Arr_face_base::assign(f);
    m_graph = f.m_graph;
  }
};

/*! A new dcel builder with extended features */
template <class Traits>
class Arrangement_graph_dcel :
  public CGAL::Arr_dcel_base<Arrangement_graph_vertex<typename Traits::
                                                     Point_2>,
                             Arrangement_graph_halfedge<typename Traits::
                                                       X_monotone_curve_2>,
                             Arrangement_graph_face>
{
public:
  Arrangement_graph_dcel() {}
};

SGAL_END_NAMESPACE

#endif
