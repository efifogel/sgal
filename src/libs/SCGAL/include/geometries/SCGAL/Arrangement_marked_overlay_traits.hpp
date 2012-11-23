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
// $Id: Arrangement_marked_overlay_traits.hpp 14196 2012-11-22 22:45:20Z efif $
// $Revision: 14196 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_MARKED_OVERLAY_TRAITS_HPP
#define SGAL_ARRANGEMENT_MARKED_OVERLAY_TRAITS_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"

SGAL_BEGIN_NAMESPACE

template <class Aos>
class Arrangement_marked_overlay_traits {
public:
  typedef typename Aos::Face_handle                     Face_handle;
  typedef typename Aos::Vertex_handle                   Vertex_handle;
  typedef typename Aos::Halfedge_handle                 Halfedge_handle;

  typedef typename Aos::Face_const_handle               Face_const_handle;
  typedef typename Aos::Vertex_const_handle             Vertex_const_handle;
  typedef typename Aos::Halfedge_const_handle           Halfedge_const_handle;

  /*! Default Constructor */
  Arrangement_marked_overlay_traits() {}
  
  /*! 1 */
  void create_face(Face_const_handle f1, Face_const_handle f2, Face_handle f)
  { f->set_mark(f1->mark() || f2->mark()); }

  /*! 2 */
  void create_vertex(Halfedge_const_handle h1, Halfedge_const_handle h2,
                     Vertex_handle v)
  { v->set_mark(h1->mark() || h2->mark()); }

  /*! 3 */
  void create_vertex(Vertex_const_handle v1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_mark(v1->mark() || v2->mark()); }

  /*! 4 */
  void create_vertex(Vertex_const_handle v1, Halfedge_const_handle h2,
                     Vertex_handle v)
  { v->set_mark(v1->mark() || h2->mark()); }

  /*! 5 */
  void create_vertex(Halfedge_const_handle h1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_mark(h1->mark() || v2->mark()); }
  
  /*! 6 */
  void create_vertex(Face_const_handle f1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_mark(f1->mark() || v2->mark()); }

  /*! 7 */
  void create_vertex(Vertex_const_handle v1, Face_const_handle f2,
                     Vertex_handle v)
  { v->set_mark(v1->mark() || f2->mark()); }

  /*! 8 */
  void create_edge(Halfedge_const_handle h1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    Boolean mark = h1->mark() || h2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
  }

  /*! 9 */
  void create_edge(Halfedge_const_handle h1, Face_const_handle f2,
                   Halfedge_handle h)
  {
    Boolean mark = h1->mark() || f2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
  }

  /*! 10 */
  void create_edge(Face_const_handle f1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    Boolean mark = f1->mark() || h2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
  }
};

SGAL_END_NAMESPACE

#endif
