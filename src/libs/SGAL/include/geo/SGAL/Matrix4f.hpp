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

#ifndef SGAL_MATRIX4F_HPP
#define SGAL_MATRIX4F_HPP

/*! \file
 *
 */

#include <string.h>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Rotation;

class SGAL_SGAL_DECL Matrix4f {
private:
  float m_matrix[4][4];

public:
  /*! Default constructor */
  Matrix4f();

  /*! Copy constructor */
  Matrix4f(const Matrix4f& m);

  /*! Constructor */
  Matrix4f(const Float matrix[]);

  /*! Constructor */
  Matrix4f(float a00, float a01, float a02, float a03,
           float a10, float a11, float a12, float a13,
           float a20, float a21, float a22, float a23,
           float a30, float a31, float a32, float a33);

  /*! A constructor that sets this matrix to be m1 * m2.
   * It is provided to enable the call to the emplace() function, a member of
   * certain container, which constructs the matrix in place.
   * \param m1 (in) the first matrix.
   * \param m2 (in) the second matrix.
   * \pre m1 is affine, that is, the last column of m1 is 0,0,0,1.
   * \pre m2 is affine, that is, the last column of m2 is 0,0,0,1.
   */
  Matrix4f(const Matrix4f& m1, const Matrix4f& m2);

  float get(int row, int col) const;
  void set(int row, int col, float v);
  void set(const float m[]);
  void set_row(int r, const Vector3f& v);
  void set_row(int r, float x, float y, float z, float w);
  void get_row(int r, Vector3f& dst) const;
  void get_row(int r, float* x, float* y, float* z, float* w) const;
  void set_col(int c, const Vector3f& v);
  void set_col(int c, float x, float y, float z, float w);
  void get_col(int c, Vector3f& dst) const;
  void get_col(int c, float* x, float* y, float* z, float* w) const;
  void set(const Matrix4f& v);

  /*! Obtain (a copy of) the matrix. */
  void get(Matrix4f& v) const;

  void make_identity();
  void make_euler(float hdeg, float pdeg, float rdeg);
  void make_rot(float x, float y, float z, float radians);
  void make_rot(const Vector3f& v, float radians);
  void make_trans(float x, float y, float z);
  void make_trans(const Vector3f& v);
  void make_scale(float x, float y, float z);
  void make_scale(const Vector3f& v);
  void make_vec_rot_vec(const Vector3f& v1, const Vector3f& v2);

  Boolean is_identity() const;
  Boolean equal(const Matrix4f& m) const;
  void transpose(const Matrix4f& m);
  void transpose();
  void mult(const Matrix4f& m1, const Matrix4f& m2);
  void full_mult(const Matrix4f& m1, const Matrix4f& m2);
  void post_mult(const Matrix4f&  m);        // this = this * m
  void pre_mult(const Matrix4f&  m);         // this = m * this
  int invert_full(Matrix4f& m);
  void invert_affine(const Matrix4f& m);
  void invert_ortho(const Matrix4f& m);
  void invert_ortho_norm(Matrix4f& m);

  void pre_trans(float x, float y, float z, Matrix4f&  m);
  void post_trans(const Matrix4f&  m, float x, float y, float z);
  void pre_rot(float x, float y, float z, float radians, Matrix4f& m);
  void post_rot(const Matrix4f& m, float x, float y, float z, float radians);
  void pre_scale(float xs, float ys, float zs, Matrix4f&  m);
  void post_scale(const Matrix4f&  m, float xs, float ys, float zs);

  float* operator [](int i);
  const float* operator [](int i) const;
  Boolean operator == (const Matrix4f& m) const;
  Boolean operator != (const Matrix4f& m) const;

  Matrix4f& operator =(const Matrix4f& m);
  Matrix4f& operator *=(const Matrix4f& m);

private:
  enum {
    TRANS = 0x01,               // something in 4th row
    ROT = 0x02,                 // 3x3 is pure rotation
    SCALE = 0x04,               // uniform scaling going on
    NONUNIFORM_SCALE = 0x08,    // non uniform scaling going on
    NONORTHO = 0x10,            // 3x3 not orthogonal
    PROJ = 0x20,                // something in 4th column
    HOM_SCALE = 0x40,           // [3][3] not 1.0
    MIRROR = 0x80               // 3x3 mirrors
  };

#ifdef ENB_NO_PLUGIN
  // Computes the rotation axis and angles (see Rotation) from this
  // matrix (quaternion operation). The result is stored in dst. The trace
  // of the matrix must be <= 1 for this operation to be valid.
  void get_ortho_rotation(Rotation& dst) const;

  // Returns true if this matrix is equal to m within tolerance of tol.
  // Returns false otherwise.
  Boolean almost_equal(const Matrix4f& m2, float tol) const;

  int lu_decomp_mat(int index[4]);

  void lu_back_sub_mat(int index[4], float b[4]);
#endif
};

//! \brief constructor.
inline Matrix4f::Matrix4f(const Float matrix[]) { set(matrix); }

//! \brief initializes the matrix.
inline void Matrix4f::set(const float* m)
{ ::memcpy(m_matrix, m, sizeof(float) * 16); }

//! \brief obtains the ith row.
inline float* Matrix4f::operator[](int i) { return m_matrix[i]; }

//! \brief obtains the ith row.
inline const float* Matrix4f::operator[](int i) const { return m_matrix[i]; }

//! \brief obtains the element at a given row and column.
inline float Matrix4f::get(int row, int col) const
{ return m_matrix[row][col]; }

//! \brief sets the element at a given row and column.
inline void Matrix4f::set(int row, int col, float val)
{ m_matrix[row][col] = val; }

//! \brief sets a row.
inline void Matrix4f::set_row(int r, float x, float y, float z, float w)
{
  m_matrix[r][0] = x; m_matrix[r][1] = y;
  m_matrix[r][2] = z; m_matrix[r][3] = w;
}

//! \brief sets a row.
inline void Matrix4f::set_row(int r, const Vector3f& v)
{ m_matrix[r][0] = v[0]; m_matrix[r][1] = v[1]; m_matrix[r][2] = v[2]; }

//! \brief obtains a row.
inline void Matrix4f::get_row(int r, float* x, float* y, float* z, float* w)
  const
{
  *x = m_matrix[r][0]; *y = m_matrix[r][1];
  *z = m_matrix[r][2]; *w = m_matrix[r][3];
}

//! \brief obtains a row.
inline void Matrix4f::get_row(int r, Vector3f& v) const
{ v[0] = m_matrix[r][0]; v[1] = m_matrix[r][1]; v[2] = m_matrix[r][2]; }

//! \brief sets a columns.
inline void Matrix4f::set_col(int c, float x, float y, float z, float w)
{
  m_matrix[0][c] = x; m_matrix[1][c] = y;
  m_matrix[2][c] = z; m_matrix[3][c] = w;
}

//! \brief sets a columns.
inline void Matrix4f::set_col(int c, const Vector3f& v)
{ m_matrix[0][c] = v[0]; m_matrix[1][c] = v[1]; m_matrix[2][c] = v[2]; }

//! \brief obtains a columns.
inline void Matrix4f::get_col(int c, float* x, float* y, float* z, float* w)
  const
{
  *x = m_matrix[0][c]; *y = m_matrix[1][c];
  *z = m_matrix[2][c]; *w = m_matrix[3][c];
}

//! \brief obtains a columns.
inline void Matrix4f::get_col(int c, Vector3f& v) const
{ v[0] = m_matrix[0][c]; v[1] = m_matrix[1][c]; v[2] = m_matrix[2][c]; }

//! \brief sets the matrix.
inline void Matrix4f::set(const Matrix4f& v)
{ ::memcpy(m_matrix, v.m_matrix, sizeof(float) * 16); }

//! \brief obtains the matrix.
inline void Matrix4f::get(Matrix4f& v) const
{ ::memcpy(v.m_matrix, m_matrix, sizeof(float) * 16); }

//! \brief determines whether the matrix is equal to another.
inline Boolean Matrix4f::equal(const Matrix4f& v) const
{
  for (Uint i = 0; i < 16; ++i)
    if (((float*)m_matrix)[i] != ((float*)v.m_matrix)[i]) return false;
  return true;
}

//! \brief equality operator
inline Boolean Matrix4f::operator==(const Matrix4f& m) const
{ return equal(m); }

//! \brief inequality operator
inline Boolean Matrix4f::operator!=(const Matrix4f& m) const
{ return !equal(m); }

//! \brief assignment operator
inline Matrix4f& Matrix4f::operator=(const Matrix4f& m)
{
  set(m);
  return *this;
}

//! \brief post product operator.
inline Matrix4f& Matrix4f::operator*=(const Matrix4f& m)
{
  post_mult(m);
  return * this;
}

//! \brief default constructor.
inline Matrix4f::Matrix4f() { this->make_identity(); }

//! \brief copy constructor.
inline Matrix4f::Matrix4f(const Matrix4f& m) { set(m); }

//! \brief constructor.
inline Matrix4f::Matrix4f(const Matrix4f& m1, const Matrix4f& m2)
{ mult(m1, m2); }

//! \brief exporter.
inline std::ostream& operator<<(std::ostream& os, const Matrix4f& mat)
{
  os << mat[0][0] << ", " << mat[0][1] << ", "
     << mat[0][2] << ", " << mat[0][3] << std::endl
     << mat[1][0] << ", " << mat[1][1] << ", "
     << mat[1][2] << ", " << mat[1][3] << std::endl
     << mat[2][0] << ", " << mat[2][1] << ", "
     << mat[2][2] << ", " << mat[2][3] << std::endl
     << mat[3][0] << ", " << mat[3][1] << ", "
     << mat[3][2] << ", " << mat[3][3] << std::endl;
    return os;
}

SGAL_END_NAMESPACE

#endif
