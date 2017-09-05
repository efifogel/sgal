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
// Author(s): Efi Fogel <efifogel@gmail.com>
//            Tzvika Geft <zvigreg@mail.tau.ac.il>

#ifndef SCGAL_ARRANGEMENT_GRAPH_OVERLAY_TRAITS_HPP
#define SCGAL_ARRANGEMENT_GRAPH_OVERLAY_TRAITS_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <boost/graph/graph_utility.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "SCGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Aos_mark, typename Aos_graph>
class Arrangement_graph_overlay_traits {
public:
  typedef boost::adjacency_matrix<boost::directedS> Graph;
  typedef std::pair<Uint, Uint>                     Key;
  typedef std::vector<Key>                          Delta;

  typedef typename Aos_graph::Face_handle           Face_graph_handle;
  typedef typename Aos_graph::Vertex_handle         Vertex_graph_handle;
  typedef typename Aos_graph::Halfedge_handle       Halfedge_graph_handle;

  typedef typename Aos_graph::Face_const_handle     Face_graph_const_handle;
  typedef typename Aos_graph::Vertex_const_handle   Vertex_graph_const_handle;
  typedef typename Aos_graph::Halfedge_const_handle Halfedge_graph_const_handle;

  typedef typename Aos_mark::Face_const_handle      Face_mark_const_handle;
  typedef typename Aos_mark::Vertex_const_handle    Vertex_mark_const_handle;
  typedef typename Aos_mark::Halfedge_const_handle  Halfedge_mark_const_handle;


private:
  Uint m_number_of_pieces;
  Key m_key;
  Boolean m_graph_update_required;
  Face_graph_handle m_graph_face;

public:
  /*! Constructor */
  Arrangement_graph_overlay_traits(Uint number_of_pieces = 0) :
    m_number_of_pieces(number_of_pieces),
    m_key(0, 0),
    m_graph_update_required(false),
    m_graph_face(NULL)
  {}

  /*! Set the current key */
  void set_key(const Key & key) { m_key = key; }

  void require_graph_update() { m_graph_update_required = true; }

  Face_graph_handle graph_face() {return m_graph_face;}

  /*! 1 */
  void create_face(Face_graph_const_handle f1, Face_mark_const_handle f2,
                   Face_graph_handle f)
  {
    if (!m_graph_update_required || f1->graph() == NULL) return;
    Graph* graph = new Graph(m_number_of_pieces);
    const Graph* graph1 = f1->graph();
    *graph = *graph1;
    if (f2->mark()) boost::add_edge(m_key.second, m_key.first, *graph);
    f->set_graph(graph);
    m_graph_update_required = false;
    m_graph_face = f;
  }

  /*! 2 */
  void create_vertex(Halfedge_graph_const_handle h1,
                     Halfedge_mark_const_handle h2,
                     Vertex_graph_handle v)
  {}

  /*! 3 */
  void create_vertex(Vertex_graph_const_handle v1, Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    // propogate info to v (case 7)
    create_vertex(v1, (Face_mark_const_handle) NULL, v);
    handle_mark_vertex(v2, v);
  }

  /*! 4 */
  void create_vertex(Vertex_graph_const_handle v1,
                     Halfedge_mark_const_handle h2,
                     Vertex_graph_handle v)
  {
    // propogate info to v (case 7)
    create_vertex(v1, (Face_mark_const_handle) NULL, v);
  }

  /*! 5 */
  void create_vertex(Halfedge_graph_const_handle h1,
                     Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    handle_mark_vertex(v2, v);
  }

  /*! 6 */
  void create_vertex(Face_graph_const_handle f1, Vertex_mark_const_handle v2,
                     Vertex_graph_handle v)
  {
    handle_mark_vertex(v2, v);
  }

  /*! 7 */
  // This function is also used by other to propagate the delta from the old to
  // the new vertex
  void create_vertex(Vertex_graph_const_handle v1,
                     Face_mark_const_handle f2,
                     Vertex_graph_handle v)
  {
    if (v1->delta() != NULL){
        Delta* delta = new Delta();
        const Delta* delta1 = v1->delta();
        *delta = *delta1;
        v->set_delta(delta);
    }
  }

  /*! 8 */
  void create_edge(Halfedge_graph_const_handle h1,
                   Halfedge_mark_const_handle h2,
                   Halfedge_graph_handle h)
  {
    // propogate info to h (case 9)
    create_edge(h1, (Face_mark_const_handle) NULL, h);

    if (h2->mark()) return;
    if (h2->face()->mark() && h2->twin()->face()->mark()) {
      // isolated unmarked edge
      if (h->local_delta() == NULL) {
        Delta* delta = new Delta();
        delta->push_back(m_key);
        h->set_local_delta(delta);
        h->twin()->set_local_delta(delta);
        return;
      }

      h->local_delta()->push_back(m_key);
      return;
    }

    if (h->delta() == NULL) {
      Delta* delta = new Delta();
      delta->push_back(m_key);
      h->set_delta(delta);
      h->twin()->set_delta(delta);
      return;
    }

    h->delta()->push_back(m_key);
  }

  /*! 9 */
  // This function is also used by others to propagate the deltas from the old
  // to the new edge
  void create_edge(Halfedge_graph_const_handle h1, Face_mark_const_handle f2,
                   Halfedge_graph_handle h)
  {
    if (h1->delta() != NULL){
      Delta* delta = new Delta();
      const Delta* delta1 = h1->delta();
      *delta = *delta1;
      h->set_delta(delta);
      h->twin()->set_delta(delta);
    }

    if (h1->local_delta() != NULL){
      Delta* local_delta = new Delta();
      const Delta* local_delta1 = h1->local_delta();
      *local_delta = *local_delta1;
      h->set_local_delta(local_delta);
      h->twin()->set_local_delta(local_delta);
    }
  }

  /*! 10 */
  void create_edge(Face_graph_const_handle f1, Halfedge_mark_const_handle h2,
                   Halfedge_graph_handle h)
  {
    if (h2->mark()) return;
    Delta* delta = new Delta();
    delta->push_back(m_key);
    if (h2->face()->mark() && h2->twin()->face()->mark()) { // isolated unmarked edge
        h->set_local_delta(delta);
        h->twin()->set_local_delta(delta);
        return;
    }
    h->set_delta(delta);
    h->twin()->set_delta(delta);
  }

private:
  // Used when a vertex is introduced to the graph aos from the mark aos
  void handle_mark_vertex(Vertex_mark_const_handle v_mark,
                          Vertex_graph_handle v_graph)
  {
    if (v_mark->is_isolated()) {
      if (v_mark->mark() || v_mark->mark() == v_mark->face()->mark()) return;

      if (v_graph->delta() == NULL){
        Delta* delta = new Delta();
        delta->push_back(m_key);
        v_graph->set_delta(delta);
        return;
      }

      v_graph->delta()->push_back(m_key);
    }
  }
};

SGAL_END_NAMESPACE

#endif
