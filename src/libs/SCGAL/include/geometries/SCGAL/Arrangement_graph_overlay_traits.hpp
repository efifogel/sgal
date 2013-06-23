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
// $Id: Arrangement_graph_overlay_traits.hpp 6632 2008-07-20 15:24:00Z efif $
// $Revision: 6632 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_GRAPH_OVERLAY_TRAITS_HPP
#define SGAL_ARRANGEMENT_GRAPH_OVERLAY_TRAITS_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <boost/graph/graph_utility.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Aos_mark, typename Aos_graph>
class Arrangement_graph_overlay_traits {
public:
  typedef boost::adjacency_matrix<boost::directedS> Graph;
  typedef std::pair<Uint, Uint>                     Key;

private:
  Uint m_number_of_pieces;
  Key m_key;
  
public:
  typedef typename Aos_graph::Face_handle           Face_graph_handle;
  typedef typename Aos_graph::Vertex_handle         Vertex_graph_handle;
  typedef typename Aos_graph::Halfedge_handle       Halfedge_graph_handle;

  typedef typename Aos_graph::Face_const_handle     Face_graph_const_handle;
  typedef typename Aos_graph::Vertex_const_handle   Vertex_graph_const_handle;
  typedef typename Aos_graph::Halfedge_const_handle Halfedge_graph_const_handle;

  typedef typename Aos_mark::Face_const_handle      Face_mark_const_handle;
  typedef typename Aos_mark::Vertex_const_handle    Vertex_mark_const_handle;
  typedef typename Aos_mark::Halfedge_const_handle  Halfedge_mark_const_handle;
  
  /*! Constructor */
  Arrangement_graph_overlay_traits(Uint number_of_pieces = 0) :
    m_number_of_pieces(number_of_pieces),
    m_key(0, 0)
  {}

  /*! Set the current key */
  void set_key(const Key & key) { m_key = key; }
  
  /*! 1 */
  void create_face(Face_graph_const_handle f1, Face_mark_const_handle f2,
                   Face_graph_handle f)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = f1->graph();
    *graph = *graph1;
    if (f2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    f->set_graph(graph);
  }

  /*! 2 */
  void create_vertex(Halfedge_graph_const_handle h1,
                     Halfedge_mark_const_handle h2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = h1->graph();
    *graph = *graph1;
    if (h2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }

  /*! 3 */
  void create_vertex(Vertex_graph_const_handle v1, Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = v1->graph();
    *graph = *graph1;
    if (v2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }

  /*! 4 */
  void create_vertex(Vertex_graph_const_handle v1,
                     Halfedge_mark_const_handle h2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = v1->graph();
    *graph = *graph1;
    if (h2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }

  /*! 5 */
  void create_vertex(Halfedge_graph_const_handle h1,
                     Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = h1->graph();
    *graph = *graph1;
    if (v2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }
  
  /*! 6 */
  void create_vertex(Face_graph_const_handle f1, Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = f1->graph();
    *graph = *graph1;
    if (v2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }

  /*! 7 */
  void create_vertex(Vertex_graph_const_handle v1,
                     Face_mark_const_handle f2,
                     Vertex_graph_handle v)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = v1->graph();
    *graph = *graph1;
    if (f2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    v->set_graph(graph);
  }

  /*! 8 */
  void create_edge(Halfedge_graph_const_handle h1,
                   Halfedge_mark_const_handle h2,
                   Halfedge_graph_handle h)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = h1->graph();
    *graph = *graph1;
    h->set_graph(graph);
    if (h2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    h->twin()->set_graph(graph);
  }

  /*! 9 */
  void create_edge(Halfedge_graph_const_handle h1, Face_mark_const_handle f2,
                   Halfedge_graph_handle h)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = h1->graph();
    *graph = *graph1;
    h->set_graph(graph);
    if (f2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    h->twin()->set_graph(graph);
  }

  /*! 10 */
  void create_edge(Face_graph_const_handle f1, Halfedge_mark_const_handle h2,
                   Halfedge_graph_handle h)
  {
    Graph * graph = new Graph(m_number_of_pieces);
    const Graph * graph1 = f1->graph();
    *graph = *graph1;
    h->set_graph(graph);
    if (h2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    h->twin()->set_graph(graph);
  }
};

SGAL_END_NAMESPACE

#endif
