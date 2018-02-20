// Copyright (c) 2015 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/Font_outliner.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

//! The cubic tolerance.
const Float Font_outliner::s_cubic_tolerance(16^2);

//! \brief processes 'move to' instructions.
void Font_outliner::move_to(const Vector2f& b0)
{
  auto size = m_outlines.size();
  m_outlines.resize(size+1);
  auto& outline = m_outlines.back();
  outline.push_back(b0);
}

//! \brief
void Font_outliner::line_to(const Vector2f& b1)
{
  auto& outline = m_outlines.back();
  outline.push_back(b1);
}

//! \brief determines whether a conic Bezier curve is suffifficiently flat
Boolean Font_outliner::is_sufficiently_flat(const Vector2f& b0,
                                            const Vector2f& b1,
                                            const Vector2f& b2) const
{
  Vector2f l, r, b, c;
  l.combine(0.5f, b0, 0.5f, b1);
  r.combine(0.5f, b1, 0.5f, b2);
  b.combine(0.5f, b0, 0.5f, b2);
  c.combine(0.5f, l, 0.5f, r);
  return (c.sqr_distance(b) <= 1);
}

//! \brief flattens a conic Bezier curve.
void Font_outliner::flatten(const Vector2f& b0, const Vector2f& b1,
                            const Vector2f& b2)
{
  if (is_sufficiently_flat(b0, b1, b2)) {
    line_to(b2);                        // output as line-segment
    return;
  }

  Vector2f l, r, c;
  l.combine(0.5f, b0, 0.5f, b1);
  r.combine(0.5f, b1, 0.5f, b2);
  c.combine(0.5f, l, 0.5f, r);
  flatten(b0, l, c);                    // left subcurve
  flatten(c, r, b2);                    // right subcurve
}

//! \brief processes 'conic to' instructions.
void Font_outliner::conic_to(const Vector2f& b1, const Vector2f& b2)
{
  auto& outline = m_outlines.back();
  SGAL_assertion(!outline.empty());
  auto& b0 = outline.back();
  flatten(b0, b1, b2);
}

//! \brief determines whether a cubic Bezier curve is suffifficiently flat
Boolean Font_outliner::is_sufficiently_flat(const Vector2f& b0,
                                            const Vector2f& b1,
                                            const Vector2f& b2,
                                            const Vector2f& b3)
  const
{
  Float ux = 3.0 * b1[0] - 2.0 * b0[0] - b3[0];
  ux *= ux;
  Float uy = 3.0 * b1[1] - 2.0 * b0[1] - b3[1];
  uy *= uy;
  Float vx = 3.0 * b2[0] - 2.0 * b3[0] - b0[0];
  vx *= vx;
  Float vy = 3.0 * b2[1] - 2.0 * b3[1] - b0[1];
  vy *= vy;
  if (ux < vx) ux = vx;
  if (uy < vy) uy = vy;
  return ((ux+uy) <= s_cubic_tolerance);
}

//! \brief flattenes a cubic Bezier curve.
void Font_outliner::flatten(const Vector2f& b0, const Vector2f& b1,
                            const Vector2f& b2, const Vector2f& b3)
{
  if (is_sufficiently_flat(b0, b1, b2, b3)) {
    line_to(b3);                        // output as line-segment
    return;
  }

  Vector2f l1, l2, c, r1, r2, m;
  l1.combine(0.5f, b0, 0.5f, b1);
  m.combine(0.5f, b1, 0.5f, b2);
  r2.combine(0.5f, b2, 0.5f, b3);
  l2.combine(0.5f, l1, 0.5f, m);
  r1.combine(0.5f, m, 0.5f, r2);
  c.combine(0.5f, l2, 0.5f, r1);
  flatten(b0, l1, l2, c);                    // left subcurve
  flatten(c, r1, r2, b3);                    // right subcurve
}

//! \brief processes 'cubic to' instructions.
void Font_outliner::cubic_to(const Vector2f& b1, const Vector2f& b2,
                             const Vector2f& b3)
{
  auto& outline = m_outlines.back();
  SGAL_assertion(!outline.empty());
  auto& b0 = outline.back();
  flatten(b0, b1, b2, b3);
}

SGAL_END_NAMESPACE
