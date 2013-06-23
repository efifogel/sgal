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

SGAL_BEGIN_NAMESPACE

template <class Aos>
class Arrangement_marked_overlay_traits {
public:
  typedef typename Aos::Face_handle                   Face_handle;
  typedef typename Aos::Vertex_handle                 Vertex_handle;
  typedef typename Aos::Halfedge_handle               Halfedge_handle;

  typedef typename Aos::Face_const_handle             Face_const_handle;
  typedef typename Aos::Vertex_const_handle           Vertex_const_handle;
  typedef typename Aos::Halfedge_const_handle         Halfedge_const_handle;

  typedef typename Aos::Outer_ccb_iterator            Outer_ccb_iterator;
  typedef typename Aos::Inner_ccb_iterator            Inner_ccb_iterator;
  typedef typename Aos::Ccb_halfedge_circulator       Ccb_halfedge_circulator;

  typedef typename Aos::Outer_ccb_const_iterator      Outer_ccb_const_iterator;
  typedef typename Aos::Inner_ccb_const_iterator      Inner_ccb_const_iterator;
  typedef typename Aos::Ccb_halfedge_const_circulator
    Ccb_halfedge_const_circulator;
  
private:
  std::size_t count(Face_const_handle f) const
  {
    std::size_t cnt = 0;
    Outer_ccb_const_iterator ocit;
    Inner_ccb_const_iterator icit;
    for (ocit = f->outer_ccbs_begin(); ocit != f->outer_ccbs_end(); ++ocit) {
      Ccb_halfedge_const_circulator curr = *ocit;
      do ++cnt;
      while (++curr != *ocit);
    }
    for (icit = f->inner_ccbs_begin(); icit != f->inner_ccbs_end(); ++icit) {
      Ccb_halfedge_const_circulator curr = *icit;
      do ++cnt;
      while (++curr != *icit);
    }
    return cnt;
  }
    
  std::size_t count(Face_handle f) const
  {
    std::size_t cnt = 0;
    Outer_ccb_iterator ocit;
    Inner_ccb_iterator icit;
    for (ocit = f->outer_ccbs_begin(); ocit != f->outer_ccbs_end(); ++ocit) {
      Ccb_halfedge_circulator curr = *ocit;
      do ++cnt;
      while (++curr != *ocit);
    }
    for (icit = f->inner_ccbs_begin(); icit != f->inner_ccbs_end(); ++icit) {
      Ccb_halfedge_circulator curr = *icit;
      do ++cnt;
      while (++curr != *icit);
    }
    return cnt;
  }

public:
  /*! Default Constructor */
  Arrangement_marked_overlay_traits() {}
  
  /*! 1 */
  void create_face(Face_const_handle f1, Face_const_handle f2, Face_handle f)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  f1: " << count(f1) << ", " << f1->mark() << std::endl;
    std::cout << "  f2: " << count(f2) << ", " << f2->mark() << std::endl;
#endif
    f->set_mark(f1->mark() || f2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  f: " << count(f) << ", " << f->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 2 */
  void create_vertex(Halfedge_const_handle h1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  h1: " << h1->source()->point() << "=>"
              << h1->target()->point()
              << ", " << h1->mark() << std::endl;
    std::cout << "  h2: " << h2->source()->point()  << "=>"
              << h2->target()->point() 
              << ", " << h2->mark() << std::endl;
#endif
    v->set_mark(h1->mark() || h2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  v: " << v->point() << ", " << v->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 3 */
  void create_vertex(Vertex_const_handle v1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  v1: " << v1->point() << ", " << v1->mark() << std::endl;
    std::cout << "  v2: " << v2->point() << ", " << v2->mark() << std::endl;
#endif
    v->set_mark(v1->mark() || v2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
      std::cout << "  v: " << v->point() << ", " << v->mark() << std::endl;
      std::cout << std::endl;
#endif
  }

  /*! 4 */
  void create_vertex(Vertex_const_handle v1, Halfedge_const_handle h2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  v1: " << v1->point() << ", " << v1->mark() << std::endl;
    std::cout << "  h2: " << h2->source()->point() << "=>"
              << h2->target()->point()
              << ", " << h2->mark() << std::endl;
#endif
    v->set_mark(v1->mark() || h2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  v: " << v->point() << ", " << v->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 5 */
  void create_vertex(Halfedge_const_handle h1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  h1: " << h1->source()->point() << "=>"
              << h1->target()->point()
              << ", " << h1->mark() << std::endl;
    std::cout << "  v2: " << v2->point() << ", " << v2->mark() << std::endl;
#endif
    v->set_mark(h1->mark() || v2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE      
    std::cout << "  v: " << v->point() << ", " << v->mark() << std::endl;
    std::cout << std::endl;
#endif
  }
  
  /*! 6 */
  void create_vertex(Face_const_handle f1, Vertex_const_handle v2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  f1: " << count(f1) << ", " << f1->mark() << std::endl;
    std::cout << "  v2: " << v2->point() << ", " << v2->mark() << std::endl;
#endif
    v->set_mark(f1->mark() || v2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE      
    std::cout << "  v: " << v->point() << ", " << v->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 7 */
  void create_vertex(Vertex_const_handle v1, Face_const_handle f2,
                     Vertex_handle v)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  v1: " << v1->point() << ", " << v1->mark() << std::endl;
    std::cout << "  f2: " << count(f2) << ", " << f2->mark() << std::endl;
#endif
    v->set_mark(v1->mark() || f2->mark());
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  V: " << v->point() << ", " << v->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 8 */
  void create_edge(Halfedge_const_handle h1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  h1: " << h1->source()->point() << "=>"
              << h1->target()->point()
              << ", " << h1->mark() << std::endl;
    std::cout << "  h2: " << h2->source()->point() << "=>"
              << h2->target()->point()
              << ", " << h2->mark() << std::endl;
#endif
    Boolean mark = h1->mark() || h2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  e: " << h->source()->point() << "=>"
              << h->target()->point()
              << ", " << h->mark() << std::endl;
      std::cout << std::endl;
#endif
  }

  /*! 9 */
  void create_edge(Halfedge_const_handle h1, Face_const_handle f2,
                   Halfedge_handle h)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  h1: " << h1->source()->point() << "=>"
              << h1->target()->point()
              << ", " << h1->mark()
              << std::endl;
    std::cout << "  f2: " << count(f2) << ", " << f2->mark() << std::endl;
#endif
    Boolean mark = h1->mark() || f2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  e: " << h->source()->point() << "=>"
              << h->target()->point()
              << ", " << h->mark() << std::endl;
    std::cout << std::endl;
#endif
  }

  /*! 10 */
  void create_edge(Face_const_handle f1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  f1: " << count(f1) << ", " << f1->mark() << std::endl;
    std::cout << "  h2: " << h2->source()->point() << "=>"
              << h2->target()->point()
              << ", " << h2->mark() << std::endl;
#endif
    Boolean mark = f1->mark() || h2->mark();
    h->set_mark(mark);
    h->twin()->set_mark(mark);
#ifdef SGAL_OVERLAY_TRAITS_VERBOSE
    std::cout << "  e: " << h->source()->point() << "=>"
              << h->target()->point()
              << ", " << h->mark() << std::endl;
    std::cout << std::endl;
#endif
  }
};

SGAL_END_NAMESPACE

#endif
