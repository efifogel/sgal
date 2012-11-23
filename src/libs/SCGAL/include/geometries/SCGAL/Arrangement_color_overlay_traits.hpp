// Copyright (c) 2007 Israel.
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
// $Id: Arrangement_color_overlay_traits.hpp 6313 2008-05-01 13:48:00Z efif $
// $Revision: 6313 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_COLOR_OVERLAY_TRAITS_HPP
#define SGAL_ARRANGEMENT_COLOR_OVERLAY_TRAITS_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

template <class Aos>
class Arrangement_color_overlay_traits {
public:
  typedef typename Aos::Face_handle                     Face_handle;
  typedef typename Aos::Vertex_handle                   Vertex_handle;
  typedef typename Aos::Halfedge_handle                 Halfedge_handle;

  typedef typename Aos::Face_const_handle               Face_const_handle;
  typedef typename Aos::Vertex_const_handle             Vertex_const_handle;
  typedef typename Aos::Halfedge_const_handle           Halfedge_const_handle;

  /*! Default Constructor */
  Arrangement_color_overlay_traits() {}

  /*! 1 */
  void create_face(Face_const_handle f1, Face_const_handle f2, Face_handle f)
  {
    Vector3f color;
    color.add(f1->color(), f2->color());
    color.scale(0.5);
    f->set_color(color);
  }

  /*! 2 */
  void create_vertex(Halfedge_const_handle h1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
    Vector3f color;
    color.add(h1->color(), h2->color());
    color.scale(0.5);
    v->set_color(color);
  }

  /*! 3 */
  void create_vertex(Vertex_const_handle v1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    Vector3f color;
    color.add(v1->color(), v2->color());
    color.scale(0.5);
    v->set_color(color);
  }

  /*! 4 */
  void create_vertex(Vertex_const_handle v1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
    v->set_color(v1->color());
  }

  /*! 5 */
  void create_vertex(Halfedge_const_handle h1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    v->set_color(v2->color());
  }
  
  /*! 6 */
  void create_vertex(Face_const_handle f1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    v->set_color(v2->color());
  }

  /*! 7 */
  void create_vertex(Vertex_const_handle v1, Face_const_handle f2,
                     Vertex_handle v)
  {
    v->set_color(v1->color());
  }

  /*! 8 */
  void create_edge(Halfedge_const_handle h1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    Vector3f color;
    color.add(h1->color(), h2->color());
    color.scale(0.5);
    h->set_color(color);
    h->twin()->set_color(color);
  }

  /*! 9 */
  void create_edge(Halfedge_const_handle h1, Face_const_handle f2,
                   Halfedge_handle h)
  {
    h->set_color(h1->color());
    h->twin()->set_color(h1->color());
  }

  /*! 10 */
  void create_edge(Face_const_handle f1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    h->set_color(h2->color());
    h->twin()->set_color(h2->color());
  }
};

SGAL_END_NAMESPACE

#endif
