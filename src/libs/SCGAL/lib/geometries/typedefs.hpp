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
// File          : demos/xqvisualization/include/typedefs.h
// QdX_release   : $Name:  $
// Revision      : $Revision: 5272 $
// Revision_date : $Date: 2007-10-22 15:08:57 +0200 (Mon, 22 Oct 2007) $
//
// Author(s)     : Roman Tulchyner <rtulchy@mpi-inf.mpg.de>
//
//============================================================================

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QdX/basic.h>

#if QdX_USE_AcX
#include <QdX/Quadric_3.h>
#else
#include <QdX/P_quadric_3.h>
#endif
#include <NiX/Arithmetic_traits.h>

#include <cmath>

const double pi = std::acos(-1.0);

typedef NiX::Arithmetic_traits AT;
#if QdX_USE_AcX
typedef QdX::Quadric_3<AT> Quadric_3;
#else
typedef QdX::P_quadric_3<AT> Quadric_3;
#endif

typedef AT::Integer Integer;
typedef AT::Rational Rational;
typedef AT::Field_with_sqrt Field_with_sqrt;
typedef AT::Poly_int3 Polynomial_3;
typedef CGAL::Fraction_traits<Rational>::Decompose Decompose;
typedef CGAL::Fraction_traits<Rational>::Compose Compose;
typedef NiX::Linear_algebra<Integer> LA_int;
typedef NiX::Linear_algebra<Rational> LA_rat;
typedef NiX::Linear_algebra<Field_with_sqrt> LA_field_with_sqrt;
typedef LA_int::Matrix Matrix_int;
typedef LA_rat::Matrix Matrix_rat;
typedef LA_field_with_sqrt::Matrix Matrix_real;
typedef LA_int::Vector Vector_int;
typedef LA_rat::Vector Vector_rat;
typedef LA_field_with_sqrt::Vector Vector_real;

enum { POLYGON_WIRED, POLYGON_FLAT, POLYGON_SMOOTH };
enum Axis {X, Y, Z};

#endif // TYPEDEFS_H 
