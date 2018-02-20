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

#ifndef SGAL_ADD_STRIP_TRIANGLE_INDICES_HPP
#define SGAL_ADD_STRIP_TRIANGLE_INDICES_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"
#include "SGAL/add_triangle_indices.hpp"

SGAL_BEGIN_NAMESPACE

/*! Add triangle indices given two sequences of points of equal size.
 * \param indices[out] the array of indices into triangles.
 * \param index[in] the size of the coordinate indices array before the addition.
 * \param[in] bottom_offset the offset into the coordinate array of the bottom
 *                          sequence.
 * \param[in] top_offset the offset into the coordinate array of the top
 *                       sequence.
 * \param size the size of the sequences.
 * \return the size of the coordinate indices array after the addition.
 * \pre The indicaes array has the capacity to store additional size quads.
 */
inline size_t add_strip_triangle_indices(Triangle_indices& indices, size_t index,
                                         size_t bottom_offset, size_t top_offset,
                                         size_t size)
{
  size_t i(0);
  for (i = 0; i < size-1; ++i) {
    auto ll = bottom_offset + i;
    auto ul = top_offset + i;
    auto lr = ll + 1;
    auto ur = ul + 1;
    index = add_triangle_indices(indices, index, ll, lr, ur, ul);
  }
  auto ll = bottom_offset + i;
  auto ul = top_offset + i;
  auto lr = bottom_offset;
  auto ur = top_offset;
  index = add_triangle_indices(indices, index, ll, lr, ur, ul);
  return index;
}

SGAL_END_NAMESPACE

#endif
