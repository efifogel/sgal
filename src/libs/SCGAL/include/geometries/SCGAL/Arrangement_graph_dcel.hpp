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
// Author(s): Efi Fogel <efifogel@gmail.com>
//            Tzvika Geft <zvigreg@mail.tau.ac.il>

#ifndef SCGAL_ARRANGEMENT_GRAPH_DCEL_HPP
#define SCGAL_ARRANGEMENT_GRAPH_DCEL_HPP

/*! \file
 * An arrangement DCEL, the vertex, halfedge, and face of which are extended
 * each with a vector of edges (that change in the DBG).
 */

#include <boost/graph/adjacency_matrix.hpp>

#include <CGAL/basic.h>
#include <CGAL/Arr_dcel_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/*! Extend the arrangement vertex */
template <typename Point_2>
class Arrangement_graph_vertex : public CGAL::Arr_vertex_base<Point_2> {
public:
  typedef std::pair<Uint, Uint>                     Key;
  typedef std::vector<Key>                          Delta;

private:
  /*! The local changes in the DBG */
  Delta* m_delta;
  Boolean m_visited;

public:
  /*! Constructor */
  Arrangement_graph_vertex() : m_delta(NULL), m_visited(false) {}

  /*! Obtain the delta */
  Delta* delta() { return m_delta; }
  const Delta* delta() const { return m_delta; }

  /*! Set a delta
   * \param delta the delta
   */
  void set_delta(Delta* delta) { m_delta = delta; }

  Boolean visited() const { return m_visited; }
  void set_visited(Boolean visited) { m_visited = visited; }

  /*! Assign from another vertex.
   * \param v the other vertex
   */
  virtual void assign(const Arrangement_graph_vertex& v)
  {
    CGAL::Arr_vertex_base<Point_2>::assign(v);
    m_delta = v.m_delta;
    m_visited = v.m_visited;
  }
};

/*! Extend the arrangement halfedge */
template <typename X_monotone_curve_2>
class Arrangement_graph_halfedge :
  public CGAL::Arr_halfedge_base<X_monotone_curve_2>
{
public:
  typedef std::pair<Uint, Uint>                     Key;
  typedef std::vector<Key>                          Delta;

private:
  /*! The changes in the DBG when crossing the edge*/
  Delta* m_delta;
  /*! The local changes in the DBG when on the edge*/
  Delta* m_local_delta;
  Boolean m_visited;

public:
  /*! Constructor */
  Arrangement_graph_halfedge() :
    m_delta(NULL),
    m_local_delta(NULL),
    m_visited(false)
  {}

  /*! Obtain the delta */
  Delta* delta() { return m_delta; }
  const Delta* delta() const { return m_delta; }

  /*! Set a delta
   * \param delta the delta
   */
  void set_delta(Delta* delta) { m_delta = delta; }

  /*! Obtain the local delta */
  Delta* local_delta() { return m_local_delta; }
  const Delta* local_delta() const { return m_local_delta; }

  /*! Set a local delta
   * \param local delta the local delta
   */
  void set_local_delta(Delta* local_delta) { m_local_delta = local_delta; }

  Boolean visited() const { return m_visited; }
  void set_visited(Boolean visited) { m_visited = visited; }

  /*! Assign from another halfedge.
   * \param he the other halfedge
   */
  virtual void assign(const Arrangement_graph_halfedge& he)
  {
    CGAL::Arr_halfedge_base<X_monotone_curve_2>::assign(he);
    m_delta = he.m_delta;
    m_local_delta = he.m_local_delta;
    m_visited = he.m_visited;
  }
};

/*! Extend the arrangement face */
class Arrangement_graph_face : public CGAL::Arr_face_base {
public:
  typedef boost::adjacency_matrix<boost::directedS>     Graph;

private:
  /*! An adjacency matrix graph */
  Graph * m_graph;
  Boolean m_visited;

public:
  /*! Constructor */
  Arrangement_graph_face() : m_graph(NULL), m_visited(false) {}

  /*! Obtain the graph */
  Graph* graph() { return m_graph; }
  const Graph* graph() const { return m_graph; }

  /*! Set a graph
   * \param graph the graph
   */
  void set_graph(Graph* graph) { m_graph = graph; }

  Boolean visited() const { return m_visited; }
  void set_visited(Boolean visited) { m_visited = visited; }

  /*! Assign from another face.
   * \param f the other face
   */
  virtual void assign(const Arrangement_graph_face & f)
  {
    CGAL::Arr_face_base::assign(f);
    m_graph = f.m_graph;
    m_visited = f.m_visited;
  }
};

/*! A new dcel builder with extended features */
template <typename Traits>
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
