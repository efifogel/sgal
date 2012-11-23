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
// $Source: $
// $Revision: 4971 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <assert.h>

#include "SGAL/Math_defs.hpp"
#include "SGAL/Matrix4f.hpp"

using namespace SGAL;

void Matrix4f::make_identity()
{
  m_matrix[0][0] = m_matrix[1][1] = m_matrix[2][2] = m_matrix[3][3] = 1.0f;
  m_matrix[0][1] = m_matrix[0][2] = m_matrix[0][3] = 0.0f;
  m_matrix[1][0] = m_matrix[1][2] = m_matrix[1][3] = 0.0f;
  m_matrix[2][0] = m_matrix[2][1] = m_matrix[2][3] = 0.0f;
  m_matrix[3][0] = m_matrix[3][1] = m_matrix[3][2] = 0.0f;
}

/*! sets this matrix to be m1 * m2, assuming m1 and m2 are both affine
 * transformation matrices, that is, the last column is 0,0,0,1,
 */
void Matrix4f::mult(const Matrix4f & m1, const Matrix4f & m2)
{
  Matrix4f tmp;
  tmp[0][0] = m1[0][0]*m2[0][0] + m1[1][0]*m2[0][1] + m1[2][0]*m2[0][2];
  tmp[0][1] = m1[0][1]*m2[0][0] + m1[1][1]*m2[0][1] + m1[2][1]*m2[0][2];
  tmp[0][2] = m1[0][2]*m2[0][0] + m1[1][2]*m2[0][1] + m1[2][2]*m2[0][2];
  tmp[0][3] = 0.0f;

  tmp[1][0] = m1[0][0]*m2[1][0] + m1[1][0]*m2[1][1] + m1[2][0]*m2[1][2];
  tmp[1][1] = m1[0][1]*m2[1][0] + m1[1][1]*m2[1][1] + m1[2][1]*m2[1][2];
  tmp[1][2] = m1[0][2]*m2[1][0] + m1[1][2]*m2[1][1] + m1[2][2]*m2[1][2];
  tmp[1][3] = 0.0f;

  tmp[2][0] = m1[0][0]*m2[2][0] + m1[1][0]*m2[2][1] + m1[2][0]*m2[2][2];
  tmp[2][1] = m1[0][1]*m2[2][0] + m1[1][1]*m2[2][1] + m1[2][1]*m2[2][2];
  tmp[2][2] = m1[0][2]*m2[2][0] + m1[1][2]*m2[2][1] + m1[2][2]*m2[2][2];
  tmp[2][3] = 0.0f;

  tmp[3][0] = m1[0][0]*m2[3][0] + m1[1][0]*m2[3][1] + m1[2][0]*m2[3][2] +
      m1[3][0];
  tmp[3][1] = m1[0][1]*m2[3][0] + m1[1][1]*m2[3][1] + m1[2][1]*m2[3][2] +
      m1[3][1];
  tmp[3][2] = m1[0][2]*m2[3][0] + m1[1][2]*m2[3][1] + m1[2][2]*m2[3][2] +
      m1[3][2];
  tmp[3][3] = 1.0f;

  set(tmp);
}

/*! this =  m * this
 */
void Matrix4f::pre_mult(const Matrix4f & m)
{
  Matrix4f tmp;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tmp[i][j] = 0.0f;
      for (int k = 0; k < 4; k++) {
        tmp[i][j] += m_matrix[k][j] * m[i][k];
      }
    }
  }
  set(tmp);
}

// =======================================================================
// Matrix4f::PostMult
//
// this = this* m
// =======================================================================
void Matrix4f::post_mult(const Matrix4f & m)
{
  Matrix4f tmp;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tmp[i][j] = 0.0f;
      for (int k = 0; k < 4; k++) {
        tmp[i][j] += m[k][j] * m_matrix[i][k];
      }
    }
  }
  set(tmp);
}

// =======================================================================
// Matrix4f::Transpose
// =======================================================================
// why m is not declared const ? (in the cosmo hpp  ?)

void Matrix4f::transpose(Matrix4f & m)
{
  Matrix4f tmp;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tmp[i][j] = m[j][i] ;
    }
  }
  set(tmp);
}

void Matrix4f::transpose()
{
  Matrix4f tmp(*this);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tmp[i][j] = m_matrix[j][i] ;
    }
  }
  set(tmp);
}

/*! sets this matrix to be a rotation matrix which rotates around axis
 * (x, y, z) by radians.
 */
void Matrix4f::make_rot(float x, float y, float z, float radians)
{
  float mag = squarerootf(x * x + y * y + z * z);
  x /= mag;
  y /= mag;
  z /= mag;
  float mySin = sinf(radians);
  float myCos = cosf(radians);
  float tmp = 1.0f - myCos;

  m_matrix[0][0] = myCos + tmp * x * x;
  m_matrix[0][1] = tmp * x * y + z * mySin;
  m_matrix[0][2] = tmp * x * z - y * mySin;
  m_matrix[0][3] = 0.0f;

  m_matrix[1][0] = tmp * x * y - z * mySin;
  m_matrix[1][1] = myCos + tmp * y * y;
  m_matrix[1][2] = tmp * y * z + x * mySin;
  m_matrix[1][3] = 0.0f;

  m_matrix[2][0] = tmp * x * z + y * mySin;
  m_matrix[2][1] = tmp * y * z - x * mySin;
  m_matrix[2][2] = myCos + tmp * z * z;
  m_matrix[2][3] = 0.0f;
  
  m_matrix[3][0] = 0.0f;
  m_matrix[3][1] = 0.0f;
  m_matrix[3][2] = 0.0f;
  m_matrix[3][3] = 1.0f;
}

/*! sets this matrix to be a rotation matrix which rotates around axis
 * v by radians.
 */
void Matrix4f::make_rot(const Vector3f &v, float radians)
{
  make_rot(v[0], v[1], v[2], radians);
}

/*! this = Rotation(x,y,z,radians) * m
 */
void Matrix4f::pre_rot(float x, float y, float z, float radians, Matrix4f & m)
{
  Matrix4f rot_mat;
  rot_mat.make_rot(x, y, z, radians);
  mult(rot_mat, m);
}

// =======================================================================
// PostRot
//
// this = m * Rotation(x,y,z,radians)
// =======================================================================
void Matrix4f::post_rot(const Matrix4f & m, float x, float y, float z,
                       float radians)
{
  Matrix4f rot_mat;
  rot_mat.make_rot(x, y, z, radians);
  mult(m, rot_mat);
}

/*! sets this matrix to be the translation matrix which translates by
 * (x, y, z).
 */
void Matrix4f::make_trans(float x, float y, float z)
{
  m_matrix[0][0] = 1.0f;
  m_matrix[0][1] = 0.0f;
  m_matrix[0][2] = 0.0f;
  m_matrix[0][3] = 0.0f;

  m_matrix[1][0] = 0.0f;
  m_matrix[1][1] = 1.0f;
  m_matrix[1][2] = 0.0f;
  m_matrix[1][3] = 0.0f;

  m_matrix[2][0] = 0.0f;
  m_matrix[2][1] = 0.0f;
  m_matrix[2][2] = 1.0f;
  m_matrix[2][3] = 0.0f;

  m_matrix[3][0] = x;
  m_matrix[3][1] = y;
  m_matrix[3][2] = z;
  m_matrix[3][3] = 1.0f;
}

/*! sets this matrix to be the translation matrix which translates by v.
 */
void Matrix4f::make_trans(const Vector3f & v)
{
  make_trans(v[0], v[1], v[2]);
}

/*! this = translationMatrix(x,y,z) * m
 */
void Matrix4f::pre_trans(float x, float y, float z, Matrix4f &  m)
{
  Matrix4f trans_mat;
  trans_mat.make_trans(x, y, z);
  mult(trans_mat, m);
}

/*! this = m * translationMatrix(x,y,z)
 */
void Matrix4f::post_trans(const Matrix4f &  m, float x, float y, float z)
{
  Matrix4f trans_mat;
  trans_mat.make_trans(x, y, z);
  mult(m, trans_mat);
}

/*!
 */
void Matrix4f::make_scale(float x, float y, float z)
{
  m_matrix[0][0] = x;
  m_matrix[0][1] = 0.0f;
  m_matrix[0][2] = 0.0f;
  m_matrix[0][3] = 0.0f;

  m_matrix[1][0] = 0.0f;
  m_matrix[1][1] = y;
  m_matrix[1][2] = 0.0f;
  m_matrix[1][3] = 0.0f;

  m_matrix[2][0] = 0.0f;
  m_matrix[2][1] = 0.0f;
  m_matrix[2][2] = z;
  m_matrix[2][3] = 0.0f;

  m_matrix[3][0] = 0.0f;
  m_matrix[3][1] = 0.0f;
  m_matrix[3][2] = 0.0f;
  m_matrix[3][3] = 1.0f;
}

/*!
 */
void Matrix4f::make_scale(const Vector3f & v)
{
  make_scale(v[0], v[1], v[2]);
}
 
/*! this = scaleMatrix(xs ,ys, zs) * m
 */
void Matrix4f::pre_scale(float xs, float ys, float zs, Matrix4f &  m)
{
  Matrix4f scale_mat;
  scale_mat.make_scale(xs, ys, zs);
  mult(scale_mat, m);
}

/*! this = m * scaleMatrix(xs ,ys, zs)
 */
void Matrix4f::post_scale(const Matrix4f &  m, float xs, float ys, float zs)
{
  Matrix4f scale_mat;
  scale_mat.make_scale(xs, ys, zs);
  mult(m, scale_mat);
}

/*! sets this matrix to be the inverse of m.
 */
int Matrix4f::invert_full(Matrix4f & /* m */)
{
  assert(0);
  return 0;
}

/*! sets this matrix to be the inverse of m, with the assumption that
 * m is an affine matrix.
 *
 *            -1     -1
 *  -1  | A 0 |   | A     0 |
 * M  = |     | = |    -1   |
 *      | C 1 |   | -CA   1 |
 *
 */
void Matrix4f::invert_affine(const Matrix4f & m)
{
  // Save in temporary veriable:
  Matrix4f tmp;
  tmp.set(m);

  // Calculate inverse(A):
  float det_1;
  float det12 = tmp[1][1] * tmp[2][2] - tmp[1][2] * tmp[2][1];
  det_1 = tmp[0][0] * det12;
  float det20 = tmp[1][2] * tmp[2][0] - tmp[1][0] * tmp[2][2];
  det_1 += tmp[0][1] * det20;
  float det01 = tmp[1][0] * tmp[2][1] - tmp[1][1] * tmp[2][0];
  det_1 += tmp[0][2] * det01;
  det_1 = 1.0f / det_1;

  m_matrix[0][0] = det12 * det_1;
  m_matrix[1][0] = det20 * det_1;
  m_matrix[2][0] = det01 * det_1;

  m_matrix[0][1] = (tmp[0][2] * tmp[2][1] - tmp[0][1] * tmp[2][2]) * det_1;
  m_matrix[1][1] = (tmp[0][0] * tmp[2][2] - tmp[0][2] * tmp[2][0]) * det_1;
  m_matrix[2][1] = (tmp[0][1] * tmp[2][0] - tmp[0][0] * tmp[2][1]) * det_1;

  m_matrix[0][2] = (tmp[0][1] * tmp[1][2] - tmp[0][2] * tmp[1][1]) * det_1;
  m_matrix[1][2] = (tmp[0][2] * tmp[1][0] - tmp[0][0] * tmp[1][2]) * det_1;
  m_matrix[2][2] = (tmp[0][0] * tmp[1][1] - tmp[0][1] * tmp[1][0]) * det_1;

  // Calculate -C * inverse(A):
  m_matrix[3][0] = -tmp[3][0] * m_matrix[0][0] - tmp[3][1] * m_matrix[1][0] -
      tmp[3][2] * m_matrix[2][0];
  m_matrix[3][1] = -tmp[3][0] * m_matrix[0][1] - tmp[3][1] * m_matrix[1][1] -
      tmp[3][2] * m_matrix[2][1];
  m_matrix[3][2] = -tmp[3][0] * m_matrix[0][2] - tmp[3][1] * m_matrix[1][2] -
      tmp[3][2] * m_matrix[2][2];

  // Rest:
  m_matrix[0][3] = m_matrix[1][3] = m_matrix[2][3] = 0.0f;
  m_matrix[3][3] = 1.0f;
}

/*! sets this matrix to be the inverse of m with the assumption that
 * the top left 3x3 submatrix is an orthogonal matrix (the columns
 * are orthogonal to each other, but each column may not be unit vector).
 */
void Matrix4f::invert_ortho(const Matrix4f & /* m */)
{
  assert(0);
}

/*! sets this matrix to be the inverse of m, with the assumption that the
 * top left 3x3 submatrix is an orthonormal matrix (the columns are
 * orthogonal to each other, and each column is a unit vector).
 */
void Matrix4f::invert_ortho_norm(Matrix4f & /* m */)
{
  assert(0);
}

