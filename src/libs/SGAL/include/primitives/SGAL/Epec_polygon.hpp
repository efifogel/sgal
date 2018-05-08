// Copyright (c) 2004 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>
//                              <ramsha7@gmail.com>

#ifndef FPG_POLYGON_HPP
#define FPG_POLYGON_HPP

#include <CGAL/basic.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

#include <SGAL/Epec_kernel.hpp>

SGAL_BEGIN_NAMESPACE

typedef CGAL::Polygon_2<SGAL::Epec_kernel>            Epec_polygon;
typedef CGAL::Polygon_with_holes_2<SGAL::Epec_kernel> Epec_polygon_with_holes;

SGAL_END_NAMESPACE

#endif
