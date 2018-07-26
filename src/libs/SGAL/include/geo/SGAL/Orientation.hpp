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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ORIENTATIO_HPP
#define SGAL_ORIENTATIO_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

/*! Enumerations of orientation of three consecutive points.
 */
enum class Orientation {
  Right_turn = -1,
  Collinear = 0,
  Left_turn = 1
};

/*! Determine the orientation of three consecutive points.
 * \param[in] a the first point.
 * \param[in] b the second point.
 * \param[in] c the third point.
 * \return the orientation of three consecutive points.
 */
template <typename Vector>
inline Orientation orientation(const Vector& a, const Vector& b, const Vector& c)
{
  Vector v, u;
  v.sub(b, a);
  u.sub(c, b);
  auto x = v[0] * u[1] - v[1] * v[0];
  return (x < 0) ? Orientation::Right_turn :
    ((x == 0) ? Orientation::Collinear : Orientation::Left_turn);
}

SGAL_END_NAMESPACE

#endif
