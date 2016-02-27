// Copyright (c) 2016 Israel.
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

#ifndef SGAL_TO_VECTOR_HPP
#define SGAL_TO_VECTOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Convert a point into Vector2f.
 */
template <typename Point>
inline Vector2f to_vector2f(const Point & point)
{
  return Vector2f(static_cast<float>(CGAL::to_double(point.x())),
                  static_cast<float>(CGAL::to_double(point.y())));
}

/*! Convert a point into Vector3f.
 */
template <typename Point>
inline Vector3f to_vector3f(const Point& point)
{
  return Vector3f(static_cast<float>(CGAL::to_double(point.x())),
                  static_cast<float>(CGAL::to_double(point.y())),
                  static_cast<float>(CGAL::to_double(point.z())));
}

SGAL_END_NAMESPACE

#endif
