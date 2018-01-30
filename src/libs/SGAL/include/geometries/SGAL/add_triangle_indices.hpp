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

#ifndef SGAL_ADD_TRIANGLE_INDICES_HPP
#define SGAL_ADD_TRIANGLE_INDICES_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Indices_types.hpp"

SGAL_BEGIN_NAMESPACE

/*! Add triangle indices given four points that form a quad.
 * \param k[in] the size of the coordinate indices array before the addition.
 * \param indices[out] the array of indices into triangles.
 * \param[in] ll lower left corner.
 * \param[in] lr lower right corner.
 * \param[in] ur upper right corner.
 * \param[in] ul upper left corner.
 * \return the size of the coordinate indices array after the addition.
 * \pre The indicaes array has the capacity to store additional 2 triangles.
 */
inline size_t add_triangle_indices(size_t k, Triangle_indices& indices,
                                   size_t ll, size_t lr, size_t ur, size_t ul)
{
  indices[k][0] = ll;
  indices[k][1] = lr;
  indices[k][2] = ur;
  ++k;
  indices[k][0] = ll;
  indices[k][1] = ur;
  indices[k][2] = ul;
  ++k;
  return k;
}

SGAL_END_NAMESPACE

#endif
