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
// $Id: Arrangement_label_overlay_traits.hpp 5969 2008-02-25 12:43:19Z efif $
// $Revision: 5969 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ARRANGEMENT_LABEL_OVERLAY_HPP
#define SGAL_ARRANGEMENT_LABEL_OVERLAY_HPP

/*! \file
 * An arrangement overlay traits class template
 */

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

template <class Aos>
class Arrangement_label_overlay {
private:
    Uint m_vertex_label1;
    Uint m_edge_label1;
    Uint m_face_label1;

    Uint m_vertex_label2;
    Uint m_edge_label2;
    Uint m_face_label2;
    
public:
  typedef typename Aos::Face_handle                     Face_handle;
  typedef typename Aos::Vertex_handle                   Vertex_handle;
  typedef typename Aos::Halfedge_handle                 Halfedge_handle;

  typedef typename Aos::Face_const_handle               Face_const_handle;
  typedef typename Aos::Vertex_const_handle             Vertex_const_handle;
  typedef typename Aos::Halfedge_const_handle           Halfedge_const_handle;

  /*! Default Constructor */
  Arrangement_label_overlay() :
    m_vertex_label1(0xffffffff),
    m_edge_label1(0xffffffff),
    m_face_label1(0xffffffff),
    m_vertex_label2(0xffffffff),
    m_edge_label2(0xffffffff),
    m_face_label2(0xffffffff)
  {}

  /*! Set the feature labels
   * \param vertex_label1
   * \param edge_label1
   * \param face_label1,
   * \param vertex_label2
   * \param edge_label2
   * \param face_label2
   */
  void set_label(Uint vertex_label1, Uint edge_label1, Uint face_label1,
                 Uint vertex_label2, Uint edge_label2, Uint face_label2)
  {
    m_vertex_label1 = vertex_label1;
    m_edge_label1 = edge_label1;
    m_face_label1 = face_label1;
    m_vertex_label2 = vertex_label2;
    m_edge_label2 = edge_label2;
    m_face_label2 = face_label2;
  }
    
  /*! 1 */
  void create_face(Face_const_handle f1, Face_const_handle f2, Face_handle f)
  { f->set_label(m_face_label1 + m_face_label2); }

  /*! 2 */
  void create_vertex(Halfedge_const_handle h1, Halfedge_const_handle h2,
                     Vertex_handle v)
  { v->set_label(m_edge_label1 + m_edge_label2); }

  /*! 3 */
  void create_vertex(Vertex_const_handle v1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_label(m_vertex_label1 + m_vertex_label2); }

  /*! 4 */
  void create_vertex(Vertex_const_handle v1, Halfedge_const_handle h2,
                     Vertex_handle v)
  { v->set_label(m_vertex_label1 + m_edge_label2); }

  /*! 5 */
  void create_vertex(Halfedge_const_handle h1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_label(m_edge_label1 + m_vertex_label2); }
  
  /*! 6 */
  void create_vertex(Face_const_handle f1, Vertex_const_handle v2,
                     Vertex_handle v)
  { v->set_label(m_face_label1 + m_vertex_label2); }

  /*! 7 */
  void create_vertex(Vertex_const_handle v1, Face_const_handle f2,
                     Vertex_handle v)
  { v->set_label(m_vertex_label1 + m_face_label2); }

  /*! 8 */
  void create_edge(Halfedge_const_handle h1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    h->set_label(m_edge_label1 + m_edge_label2);
    h->twin()->set_label(m_edge_label1 + m_edge_label2);
  }

  /*! 9 */
  void create_edge(Halfedge_const_handle h1, Face_const_handle f2,
                   Halfedge_handle h)
  {
    h->set_label(m_edge_label1 + m_face_label2);
    h->twin()->set_label(m_edge_label1 + m_face_label2);
  }

  /*! 10 */
  void create_edge(Face_const_handle f1, Halfedge_const_handle h2,
                   Halfedge_handle h)
  {
    h->set_label(m_face_label1 + m_edge_label2);
    h->twin()->set_label(m_face_label1 + m_edge_label2);
  }
};

SGAL_END_NAMESPACE

#endif
