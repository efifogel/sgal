// Copyright (c) 2011 Israel.
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
// $Id: Line.hpp 11013 2011-04-05 17:10:19Z efif $
// $Revision: 11013 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LINE_HPP
#define SGAL_LINE_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Line {
public:
  /*! Constructor */
  Line(const Vector3f& p1, const Vector3f& p2);

  /*! Set representative point, which lies on the line */
  void set_point(Float x, Float y, Float z) { m_point.set(x, y, z); }

  /*! Set the direction */
  void set_point(const Vector3f & d) { m_point = d; }

  /*! Obtain the direction */
  const Vector3f& get_point() const { return m_point; }

  /*! Set the direction */
  void set_direction(Float x, Float y, Float z) { m_direction.set(x, y, z); }

  /*! Set the direction */
  void set_direction(const Vector3f & d) { m_direction = d; }

  /*! Obtain the direction */
  const Vector3f& get_direction() const { return m_direction; }

private:
  /*! A point on the line */
  Vector3f m_point;

  /*! The direction of the line */
  Vector3f m_direction;
};

SGAL_END_NAMESPACE

#endif
