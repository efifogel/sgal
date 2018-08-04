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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EPIC_TRIANGULATION_HPP
#define SCGAL_EPIC_TRIANGULATION_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Face_nesting_level.hpp"

SGAL_BEGIN_NAMESPACE

typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, Epic_kernel>
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
