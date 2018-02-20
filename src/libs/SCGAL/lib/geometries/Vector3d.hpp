//============================================================================
//
// Copyright (c) 2001-2006 Max-Planck-Institut Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of EXACUS (http://www.mpi-inf.mpg.de/projects/EXACUS/);
// you may redistribute it under the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with EXACUS.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
//----------------------------------------------------------------------------
//
// Library       : QdX
// File          : demos/xqvisualization/include/Vector3d.h
// QdX_release   : $Name:  $
// Revision      : $Revision: 5272 $
// Revision_date : $Date: 2007-10-22 15:08:57 +0200 (Mon, 22 Oct 2007) $
//
// Author(s)     : Roman Tulchyner <rtulchy@mpi-inf.mpg.de>
//
//============================================================================

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <cmath>

class Vector3d {
public:
  Vector3d();
  Vector3d(double x, double y, double z);
  Vector3d(const Vector3d & vec);
  ~Vector3d();

  double x() const; 
  double y() const; 
  double z() const;
  double operator[](int index) const;
  double& operator[](int index);

  double length() const;
  double distance(const Vector3d & vec) const;
  double dotproduct(const Vector3d & vec) const;
  double line_distance(const Vector3d & line_point_a,
                       const Vector3d & line_point_b) const;
  double angle(const Vector3d & vec) const;
  Vector3d crossproduct(const Vector3d & vec) const;
  Vector3d normalize() const;
  Vector3d add(const Vector3d & vec) const;
  Vector3d sub(const Vector3d & vec) const;
  Vector3d scalar_multipl(double scalar) const;
  static Vector3d normal(const Vector3d & a, const Vector3d & b,
                         const Vector3d & c);
  Vector3d rotate_y(double dSin, double dCos) const;
  Vector3d rotate_z(double dSin, double dCos) const;
  Vector3d rotate_x(double dSin, double dCos) const;
  Vector3d translate(double dX, double dY, double dZ) const;
  bool equals(const Vector3d & vec) const;

  Vector3d operator+(const Vector3d & v) const;
  Vector3d operator-(const Vector3d & v) const;   
  Vector3d operator*(double scalar) const;
  Vector3d operator/(double scalar) const;  
  double operator*(const Vector3d & v) const;
  bool   operator==(const Vector3d & v) const;
  bool   operator!=(const Vector3d & v) const;

private:  
  double v[3];
};

#endif
