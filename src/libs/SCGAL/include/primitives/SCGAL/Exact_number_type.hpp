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
// $Revision: 5325 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SCGAL_EXACT_NUMBER_TYPE_HPP
#define SCGAL_EXACT_NUMBER_TYPE_HPP

#define SCGAL_DOUBLE_NT                 0
#define SCGAL_LEDA_RAT_NT               1
#define SCGAL_QUOTIENT_MP_FLOAT_NT      2
#define SCGAL_CGAL_GMPQ_NT              3
#define SCGAL_CGAL_GMPZ_NT              4

#include <CGAL/basic.h>

#if SCGAL_NT == SCGAL_DOUBLE_NT

#elif SCGAL_NT == SCGAL_LEDA_RAT_NT
#include <CGAL/leda_rational.h>

#elif SCGAL_NT == SCGAL_QUOTIENT_MP_FLOAT_NT
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>

#elif SCGAL_NT == SCGAL_CGAL_GMPQ_NT
#include <CGAL/Gmpq.h>

#elif SCGAL_NT == SCGAL_CGAL_GMPZ_NT
#include <CGAL/Gmpz.h>

#else
#error No Number Type (SCGAL_NT) specified!

#endif

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

// Kernel, Number type, etc.
#if SCGAL_NT == SCGAL_DOUBLE_NT
typedef double                                          Exact_number_type;

#elif SCGAL_NT == SCGAL_QUOTIENT_MP_FLOAT_NT
typedef CGAL::Quotient<CGAL::MP_Float>                  Exact_number_type;

#elif SCGAL_NT == SCGAL_CGAL_GMPQ_NT
typedef CGAL::Gmpq                                      Exact_number_type;

#elif SCGAL_NT == SCGAL_LEDA_RAT_NT
typedef leda_rational                                   Exact_number_type;

#elif SCGAL_NT == SCGAL_CGAL_GMPZ_NT
typedef CGAL::Gmpz                                      Exact_number_type;

#else
#error Invalid number type!

#endif

SGAL_END_NAMESPACE

#endif
