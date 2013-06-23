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
// $Id: Sgm_color_overlay_traits.hpp 7201 2009-01-15 09:23:56Z naamamay $
// $Revision: 7201 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SGM_COLOR_OVERLAY_TRAITS_HPP
#define SGAL_SGM_COLOR_OVERLAY_TRAITS_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <CGAL/basic.h>
#include <CGAL/Arr_spherical_gaussian_map_3/Arr_polyhedral_sgm_overlay.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

template <class Sgm>
class Sgm_color_overlay_traits : public CGAL::Arr_polyhedral_sgm_overlay<Sgm>
{
private:
  typedef CGAL::Arr_polyhedral_sgm_overlay<Sgm>         Base;
  
  typedef typename Sgm::Face_handle                     Face_handle;
  typedef typename Sgm::Vertex_handle                   Vertex_handle;
  typedef typename Sgm::Halfedge_handle                 Halfedge_handle;

  typedef typename Sgm::Face_const_handle               Face_const_handle;
  typedef typename Sgm::Vertex_const_handle             Vertex_const_handle;
  typedef typename Sgm::Halfedge_const_handle           Halfedge_const_handle;

public:
  /*! Default Constructor */
  Sgm_color_overlay_traits() {}

  /*! 1 */
  void create_face(Face_const_handle f1, Face_const_handle f2, Face_handle f)
  {
    Base::create_face(f1, f2, f);
    Vector3f color;
    color.add(f1->color(), f2->color());
    color.scale(0.5);
    f->set_color(color);
  }

  /*! 2 */
  void create_vertex(Halfedge_const_handle h1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
    Base::create_vertex(h1, h2, v);
    Vector3f color;
    color.add(h1->color(), h2->color());
    color.scale(0.5);
    v->set_color(color);
  }

  /*! 3 */
  void create_vertex(Vertex_const_handle v1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    Base::create_vertex(v1, v2, v);
    Vector3f color;
    color.add(v1->color(), v2->color());
    color.scale(0.5);
    v->set_color(color);
  }

  /*! 4 */
  void create_vertex(Vertex_const_handle v1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
    Base::create_vertex(v1, h2, v);
    v->set_color(v1->color());
  }

  /*! 5 */
  void create_vertex(Halfedge_const_handle h1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    Base::create_vertex(h1, v2, v);
    v->set_color(v2->color());
  }
  
  /*! 6 */
  void create_vertex(Face_const_handle f1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
    Base::create_vertex(f1, v2, v);
    v->set_color(v2->color());
  }

  /*! 7 */
  void create_vertex(Vertex_const_handle v1, Face_const_handle f2,
                     Vertex_handle v)
  {
    Base::create_vertex(v1, f2, v);
    v->set_color(v1->color());
  }

  /*! 8 */
  void create_edge(Halfedge_const_handle h1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    Base::create_edge(h1, h2, h);
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
    Base::create_edge(h1, f2, h);
    h->set_color(h1->color());
    h->twin()->set_color(h1->color());
  }

  /*! 10 */
  void create_edge(Face_const_handle f1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    Base::create_edge(f1, h2, h);
    h->set_color(h2->color());
    h->twin()->set_color(h2->color());
  }
};

SGAL_END_NAMESPACE

#endif
