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
// $Source$
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_MATRIX3F_HPP
#define SGAL_MATRIX3F_HPP

/*! \file
 *
 */

#include <string.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Matrix3f {
private:
  float m_matrix[3][3];

public:
  // Constructors
  Matrix3f();
  Matrix3f(float a00, float a01, float a02,
           float a10, float a11, float a12,
           float a20, float a21, float a22);

  // Creates an instance of Matrix3f with its entries set to those of m
  Matrix3f(const Matrix3f & m);

public:
  // Accessor functions
  float get(int row, int col) const;
  void set(int row, int col, float v);
  void set(const float m[]);
  void set_row(int r, const Vector3f & v);
  void set_row(int r, float x, float y, float z);
  void get_row(int r, Vector3f & dst) const;
  void get_row(int r, float * x, float * y, float * z) const;
  void set_col(int c, const Vector3f & v);
  void set_col(int c, float x, float y, float z);
  void get_col(int c, Vector3f & dst) const;
  void get_col(int c, float * x, float * y, float * z) const;
  void set(const Matrix3f & v);
  void get(Matrix3f & v) const;

public:
  // Math
  bool equal(const Matrix3f & m) const;
  void transpose(const Matrix3f & m);
  int invert(const Matrix3f & m);
    
public:
  // Linear algebra
  void svd(float a[3][3], float u[3][3], float vt[3][3]);
  void clean_svd(float a[3][3], float u[3][3], float vt[3][3]);   
  void get_axis(float m[3][3], float a[3], float & angle);
  int lu_decompose(float a[3][3], int rowSwap[3], int colSwap[3]);
  void solve_lx_equal_y(float l[3][3], float x[3], float y[3]);

public:
  // Building routines
  void make_identity();

public:
  // I/O Methods

  // void Load(const Field field);
  // void Save(Field field) const;

public:
  // Operators
  float * operator [](int i);
  const float * operator [](int i) const;
  bool operator == (const Matrix3f & m) const;
  bool operator != (const Matrix3f & m) const;

  Matrix3f & operator =(const Matrix3f & m);
};

/*!
 */
inline void Matrix3f::set(const float * m)
{
  ::memcpy(m_matrix, m, sizeof(float) * 9);
}

/*!
 */
inline float * Matrix3f::operator[](int i)
{
  return m_matrix[i];
}

/*!
 */
inline const float * Matrix3f::operator[](int i) const
{
  return m_matrix[i];
}

/*!
 */
inline float Matrix3f::get(int row, int col) const
{
  return m_matrix[row][col];
}

/*!
 */
inline void Matrix3f::set(int row, int col, float val)
{
  m_matrix[row][col] = val;
}

/*!
 */
inline void Matrix3f::set_row(int r, float x, float y, float z)
{
  m_matrix[r][0] = x; m_matrix[r][1] = y; m_matrix[r][2] = z;
}

/*!
 */
inline void Matrix3f::set_row(int r, const Vector3f & v)
{
  m_matrix[r][0] = v[0]; m_matrix[r][1] = v[1]; m_matrix[r][2] = v[2];
}

/*!
 */
inline void Matrix3f::get_row(int r, float * x, float * y, float * z) const
{
  *x = m_matrix[r][0]; *y = m_matrix[r][1]; *z = m_matrix[r][2];
}

/*!
 */
inline void Matrix3f::get_row(int r, Vector3f & v) const
{
  v[0] = m_matrix[r][0]; v[1] = m_matrix[r][1]; v[2] = m_matrix[r][2];
}

/*!
 */
inline void Matrix3f::set_col(int c, float x, float y, float z)
{
  m_matrix[0][c] = x; m_matrix[1][c] = y; m_matrix[2][c] = z;
}

/*!
 */
inline void Matrix3f::set_col(int c, const Vector3f & v)
{
  m_matrix[0][c] = v[0]; m_matrix[1][c] = v[1]; m_matrix[2][c] = v[2];
}

/*!
 */
inline void Matrix3f::get_col(int c, float * x, float * y, float * z) const
{
    *x = m_matrix[0][c]; *y = m_matrix[1][c]; *z = m_matrix[2][c];
}

/*!
 */
inline void Matrix3f::get_col(int c, Vector3f & v) const
{
  v[0] = m_matrix[0][c]; v[1] = m_matrix[1][c]; v[2] = m_matrix[2][c];
}

/*!
 */
inline void Matrix3f::set(const Matrix3f & v)
{
  ::memcpy(m_matrix, v.m_matrix, sizeof(float) * 9);
}

/*!
 */
inline void Matrix3f::get(Matrix3f & m) const
{
  ::memcpy(m.m_matrix, m_matrix, sizeof(float) * 9);
}

/*!
 */
inline bool Matrix3f::equal(const Matrix3f & m) const
{
  int i;
  for (i = 0; i < 9; i++)
    if (m_matrix[i] != m[i]) return false;
  return true;
}

/*!
 */
inline bool Matrix3f::operator==(const Matrix3f & m) const
{
  return equal(m);
}

/*!
 */
inline bool Matrix3f::operator!=(const Matrix3f & m) const
{
  return !equal(m);
}

/*!
 */
inline Matrix3f & Matrix3f::operator=(const Matrix3f & m)
{
  set(m);
  return *this;
}

/*!
 */
inline Matrix3f::Matrix3f()
{
  this->make_identity();
}

/*!
 */
inline Matrix3f::Matrix3f(const Matrix3f & m)
{
  set(m);
}

SGAL_END_NAMESPACE

#endif
