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

#ifndef SGAL_FONT_OULINER_HPP
#define SGAL_FONT_OULINER_HPP

#include <list>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Font_outliner {
public:
  typedef std::list<Vector2f>                         Outline;
  typedef std::vector<Outline>                          Outlines;

  /*! Constructor.
   */
  Font_outliner(Outlines& outlines) : m_outlines(outlines) {}

  /*! Obtain the polygon outlines.
   */
  const Outlines& get_outlines() const;

  /*! Process 'move to' instructions.
   * \param b0 (in) the target point of the 'move to'.
   */
  void move_to(const Vector2f& b0);

  /*! Process 'line to' instructions.
   * \param b1 (in) the end point of the line.
   */
  void line_to(const Vector2f& b1);

  /*! Process 'conic to' instructions.
   * A 'conic to' indicate a second-order Bézier arc in the outline.
   * \param b1 (in) the intermediate control point between the last
   *                position and the new target in to.
   * \param b2 (in) the target end point of the conic arc.
   */
  void conic_to(const Vector2f& b1, const Vector2f& b2);

  /*! Process 'cubic to' instructions.
   * A 'cubic to' indicates a third-order Bézier arc  in the outline.
   * \param control1 (in) the first Bézier control point.
   * \param control2 (in) the second Bézier control point.
   * \param to (in) the target end point of the cubic arc.
   * \param user (out) points to this.
   */
  void cubic_to(const Vector2f& b1, const Vector2f& b2, const Vector2f& b3);

  /*! Flatten a conic Bezier curve. Observe that the 1st point must be already
   * stored in the internal outlines structure.
   * \param b0 (in) first point.
   * \param b1 (in) second (control) point.
   * \param b2 (in) third point.
   */
  void flatten(const Vector2f& b0, const Vector2f& b1, const Vector2f& b2);

  /*! Determines whether a conic Bezier curve is suffifficiently flat and thus
   * neen not be further subdived.
   * \param b0 (in) first point.
   * \param b1 (in) second (control) point.
   * \param b2 (in) third point.
   */
  Boolean is_sufficiently_flat(const Vector2f& b0, const Vector2f& b1,
                               const Vector2f& b2) const;

  /*! Flatten a cubic Bezier curve. Observe that the 1st point is already
   * stored in the internal outlines structure.
   * \param b0 (in) first point.
   * \param b1 (in) second (control) point.
   * \param b2 (in) third (control) point.
   * \param b3 (in) fourth point.
   */
  void flatten(const Vector2f& b0, const Vector2f& b1,
               const Vector2f& b2, const Vector2f& b3);

  /*! Determine whether a cubic Bezier curve is suffifficiently flat and thus
   * neen not be further subdived.
   * \param b0 (in) first point.
   * \param b1 (in) second (control) point.
   * \param b2 (in) third (control) point.
   * \param b3 (in) fourth point.
   */
  Boolean is_sufficiently_flat(const Vector2f& b0, const Vector2f& b1,
                               const Vector2f& b2, const Vector2f& b3) const;

private:
  /*! The output polygon outlines. */
  Outlines& m_outlines;

  /*! The cubic tolerance. */
  static const Float s_cubic_tolerance;
};

//! \brief obtains the polygon outlines.
inline const Font_outliner::Outlines& Font_outliner::get_outlines() const
{ return m_outlines; }

SGAL_END_NAMESPACE

#endif
