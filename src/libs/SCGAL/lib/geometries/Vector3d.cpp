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
// File          : demos/xqvisualization/Vector3d.C
// QdX_release   : $Name:  $
// Revision      : $Revision: 5272 $
// Revision_date : $Date: 2007-10-22 15:08:57 +0200 (Mon, 22 Oct 2007) $
//
// Author(s)     : Roman Tulchyner <rtulchy@mpi-inf.mpg.de>
//
//============================================================================

#include <cmath>
#include <math.h>
#include <iostream>

#include "Vector3d.hpp"

Vector3d::Vector3d()
{
  this->v[0] = 0; this->v[1] = 0; this->v[2] = 0;
}

Vector3d::Vector3d( double x, double y, double z )
{
  this->v[0] = x; this->v[1] = y; this->v[2] = z;
}

Vector3d::Vector3d( const Vector3d & w)
{
  this->v[0] = w[0]; this->v[1] = w[1]; this->v[2] = w[2];
}

Vector3d::~Vector3d(){
//  delete[] v; //brauch' nit, da nie explizit mit new erzeugt, flgl. ist v temporär
}

double Vector3d::x() const { return this->v[0]; } 
double Vector3d::y() const { return this->v[1]; } 
double Vector3d::z() const { return this->v[2]; } 

double Vector3d::operator[](int index) const
{
//  if (index>2 or index<0) {/*allert*/};
  return this->v[index];
}

double & Vector3d::operator[](int index)
{
//  if (index>2 or index<0) {/*allert*/};
  return this->v[index];
}

double Vector3d::length() const
{
  return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

double Vector3d::distance(const Vector3d& w) const
{
  double result = ( *this - w ).length();
  return result;
}

double Vector3d::dotproduct(const Vector3d & w) const
{
//  Vector3d v = *this;
  return v[0]*w[0]+v[1]*w[1]+v[2]*w[2];
}

double Vector3d::line_distance(const Vector3d& line_point_a,
                               const Vector3d& line_point_b) const
{
  Vector3d a(line_point_a);
  Vector3d b(line_point_b);

  Vector3d pa( *this - a );
  Vector3d ba( b - a );
  Vector3d d( pa.crossproduct(ba) );
  return d.length() /(double) ba.length();
}

double Vector3d::angle(const Vector3d & w) const
{  //wink. zw 2 vektoren
  double a = this->distance(w);
  double b = this->length();
  double c = w.length();

  double cosAngle = (b*b +c*c -a*a)/(double)(2*b*c);
  return acos(cosAngle);
}

Vector3d Vector3d::crossproduct(const Vector3d & w) const
{
  Vector3d result;
//  Vector3d v = *this;

  result[0] = v[1]*w[2] - v[2]*w[1];
  result[1] = v[2]*w[0] - v[0]*w[2];
  result[2] = v[0]*w[1] - v[1]*w[0];

  return result;
}

Vector3d Vector3d::normalize() const
{
  Vector3d result( *this );
  double len = result.length();

  for (int i=0; i<3; i++) result[i] /= len;

  return result;
}

Vector3d Vector3d::add(const Vector3d & w) const
{
  Vector3d result;
  
  for(int i=0; i<3; i++)
    result[i] = v[i] + w[i];

  return result;
}

Vector3d Vector3d::sub(const Vector3d & w) const
{
  Vector3d result;
  
  for (int i=0; i<3; i++)
    result[i] = v[i] - w[i];

  return result;
}

Vector3d Vector3d::scalar_multipl(double scalar) const
{
  Vector3d result;

  for (int i=0; i<3; i++)
    result[i] = scalar * v[i];

  return result;
}

Vector3d Vector3d::normal(const Vector3d& v1,
                          const Vector3d& v2,
                          const Vector3d& v3)
{
  Vector3d a(v1);
  Vector3d b(v2);
  Vector3d c(v3);
  Vector3d ba( a - b );
  Vector3d ca( a - c );
  
  Vector3d result(( ca.crossproduct(ba) ).normalize() );

  return result;
}

Vector3d Vector3d::rotate_y( double dSin, double dCos) const
{
  double x, y, z;
  x = dCos * this->x() + dSin * this->z();
  y = this->y();
  z = -1 * dSin * this->x() + dCos * this->z();

  Vector3d result(x,y,z);

  return result;
}

Vector3d Vector3d::rotate_z( double dSin, double dCos ) const
{
  double x, y, z;
  x = dCos * this->x() - dSin * this->y();
  y = dSin * this->x() + dCos * this->y();
  z = this->z();

  Vector3d result(x,y,z);

  return result;
}

Vector3d Vector3d::rotate_x( double dSin, double dCos ) const
{
  double x, y, z;
  x = this->x();
  y = dCos * this->y() - dSin * this->z();
  z = dSin * this->y() + dCos * this->z();

  Vector3d result(x,y,z);

  return result;
}

Vector3d Vector3d::translate( double dX, double dY, double dZ ) const
{
  Vector3d result(this->x()+dX,this->y()+dY,this->z()+dZ);

  return result;
}

bool Vector3d::equals(const Vector3d& w) const
{
  if ((v[0] == w[0]) && (v[1] == w[1]) && (v[2] == w[2]) ) return true;
  return false;
}

Vector3d Vector3d::operator+(const Vector3d& v ) const
{
  return this->add(v);
}
  
Vector3d Vector3d::operator-(const Vector3d& v ) const
{
  return this->sub(v);
}
     
Vector3d Vector3d::operator*( double scalar ) const
{
  return this->scalar_multipl(scalar);
}
  
Vector3d Vector3d::operator/( double scalar ) const
{
  if (scalar !=0 ) {
    return this->scalar_multipl( 1 /(double) scalar );
  } else {
    /*error*/
    Vector3d null;
    return null;
  }
}
  
double Vector3d::operator*(const Vector3d & v ) const
{
  return this->dotproduct(v);
}
  
bool Vector3d::operator==(const Vector3d & v )const
{
  return this->equals(v);
}
  
bool Vector3d::operator!=(const Vector3d & v )const
{
  return !( this->equals(v) );
}

