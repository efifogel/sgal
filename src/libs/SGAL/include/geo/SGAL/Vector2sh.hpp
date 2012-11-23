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

#ifndef SGAL_VECTOR2SH_HPP
#define SGAL_VECTOR2SH_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_CLASSDEF Vector2sh {
private:
  short m_vector[2];

public:
  // Constructors
  Vector2sh();
  Vector2sh(short a, short b);
  Vector2sh(const Vector2sh & v);

public:
  // set/get

  // Accessor functions
  void set(short a, short b);
  void get(short * a, short * b) const;
   
  short get(int i) const;
  void set(const Vector2sh & v);
  void get(Vector2sh & v) const;
  void add(const Vector2sh & v1);
  void add(const Vector2sh & v1, const Vector2sh & v2);
  void sub(const Vector2sh & v1);
  void sub(const Vector2sh & v1, const Vector2sh & v2);

  bool equal(const Vector2sh & v) const;
  void negate(const Vector2sh & v);
  short dot(const Vector2sh & v) const;
  void mul(const Vector2sh & v1, const Vector2sh & v2);
  short sqr_distance(const Vector2sh & v) const;
  
public:
  // Opertors
  Vector2sh & operator=(const Vector2sh & v);
  Vector2sh & operator=(short v);
  short & operator[](int i);
  short operator[](int i) const;
  bool operator==(const Vector2sh & v) const;
  bool operator!=(const Vector2sh & v) const;
};

SGAL_END_NAMESPACE

#endif
