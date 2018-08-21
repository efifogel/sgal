// Copyright (c) 2018 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EPIC_TRIANGULATION_HPP
#define SCGAL_EPIC_TRIANGULATION_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Vertex_index.hpp"
#include "SGAL/Face_nesting_level.hpp"

SGAL_BEGIN_NAMESPACE

/* The vertex type is extended with the type Vertex_index. The Vertex_index
 * construct contains only a size_t type member (typically used as an index). We
 * must use a construct, namely Vertex_index, to extend the vertex instead of
 * directly using the simple type size_t, because we need to initialize it with
 * a significant value, namely -1, upon construction of the containing vertex.
 *
 * The member function tri.insert(p) inserts a point into the triangulation and
 * returns a handle to the corresponding vertex. If the point exists already,
 * the function returns a handle to the existing vertex. Otherwise, it returns
 * a handle to a new vertex with the index initialized to -1. We increase the
 * index counter only if we get a new vertex (with its index initialized to -1).
 *
 * Had the triangulation supported a mechanism to initialize the extended data
 * of the vertex, e.g., an observer that contains a call back invoked
 * immediately after a vertex is constructed, we could have avoided all this,
 * and use the simple type size_t directly.
 */
typedef CGAL::Triangulation_vertex_base_with_info_2<Vertex_index, Epic_kernel>
  Epic_vb;

typedef CGAL::Triangulation_face_base_with_info_2<Face_nesting_level,
                                                  Epic_kernel>
  Epic_fbi;

typedef CGAL::Constrained_triangulation_face_base_2<Epic_kernel, Epic_fbi>
  Epic_fb;

typedef CGAL::Triangulation_data_structure_2<Epic_vb, Epic_fb>
  Epic_tds;

// typedef CGAL::No_intersection_tag                            Epic_itag;
typedef CGAL::Exact_predicates_tag                              Epic_itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<Epic_kernel, Epic_tds,
                                                   Epic_itag>
  Epic_triangulation;

SGAL_END_NAMESPACE

#endif
