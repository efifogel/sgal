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

#ifndef SCGAL_INEXACT_TRIANGULATION_HPP
#define SCGAL_INEXACT_TRIANGULATION_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Face_nesting_level.hpp"

SGAL_BEGIN_NAMESPACE

typedef Inexact_kernel                                          Kernel;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, Inexact_kernel>
  Inexact_vb;

typedef CGAL::Triangulation_face_base_with_info_2<Face_nesting_level,
                                                  Inexact_kernel>
  Inexact_fbi;

typedef CGAL::Constrained_triangulation_face_base_2<Inexact_kernel, Inexact_fbi>
  Inexact_fb;

typedef CGAL::Triangulation_data_structure_2<Inexact_vb, Inexact_fb>
  Inexact_tds;

typedef CGAL::No_intersection_tag                                     Itag;
// typedef CGAL::Exact_predicates_tag                                 Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<Inexact_kernel,
                                                   Inexact_tds, Itag>
  Inexact_triangulation;

SGAL_END_NAMESPACE

#endif
