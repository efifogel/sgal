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
// $Id: $
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <assert.h>

#include "SGAL/basic.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Matrix3f.hpp"

SGAL_BEGIN_NAMESPACE

/*! \brief */
void Matrix3f::make_identity()
{
  m_matrix[0][0] = m_matrix[1][1] = m_matrix[2][2] = 1.0f;
  m_matrix[0][1] = m_matrix[0][2] = 0.0f;
  m_matrix[1][0] = m_matrix[1][2] = 0.0f;
  m_matrix[2][0] = m_matrix[2][1] = 0.0f;
}

/*! \brief */
void Matrix3f::svd(float a[3][3], float u[3][3], float vt[3][3])
{
  (void) a, (void) u, (void) vt;
  assert(0);
}

/*! \brief */
void Matrix3f::clean_svd(float a[3][3], float u[3][3], float vt[3][3])
{
  (void) a, (void) u, (void) vt;
  assert(0);
}

/*! \brief */
void Matrix3f::get_axis(float m[3][3], float a[3], float& angle)
{
  (void) m, (void) a, (void) angle;
  assert(0);
}

/*! \brief */
int Matrix3f::lu_decompose(float a[3][3], int row_swap[3], int col_swap[3])
{
  (void) a, (void) row_swap, (void) col_swap;
  assert(0);
  return 0;
}

/*! \brief */
void Matrix3f::solve_lx_equal_y(float l[3][3], float x[3], float y[3])
{
  (void) l, (void) x, (void) y;
  assert(0);
}

/*! \brief */
void Matrix3f::transpose(const Matrix3f& m)
{
  Matrix3f tmp;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      tmp[i][j] = m[j][i] ;
    }
  }
  set(tmp);
}

/*! \brief */
int Matrix3f::invert(const Matrix3f& /* m */)
{
  assert(0);
  return 0;
}

SGAL_END_NAMESPACE
