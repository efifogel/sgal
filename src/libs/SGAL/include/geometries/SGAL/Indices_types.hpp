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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_INDICES_TYPES_HPP
#define SGAL_INDICES_TYPES_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

/* The type Index_array is used to store an array of unsigned int indices.
 * Arrays of such type are used as arguments in OpenGL calls, where the
 * type of the index, namely unsigned int, is hard coded.
 */
typedef uint32_t                                        Index_type;
typedef std::vector<Index_type>                         Index_array;

typedef std::vector<std::array<Index_type, 3> >         Triangle_indices;
typedef std::vector<std::array<Index_type, 4> >         Quad_indices;
typedef std::vector<std::vector<Index_type> >           Polygon_indices;
typedef std::vector<Index_type>                         Flat_indices;
typedef boost::variant<Triangle_indices, Quad_indices, Polygon_indices,
                       Flat_indices>                    Facet_indices;

SGAL_END_NAMESPACE

#endif
