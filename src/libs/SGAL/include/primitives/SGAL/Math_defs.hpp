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
// $Revision: 12381 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_MATH_DEFS_HPP
#define SGAL_MATH_DEFS_HPP

#include <stdio.h>
#include <math.h>
#include <boost/math/special_functions/fpclassify.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

namespace bm = boost::math;

/*!
 */
template <class T>
inline const T& clamp(const T& x, const T& lo, const T& hi)
{
  return ((x < lo) ? lo : (x > hi) ? hi : x);
}

/*! abs is faster than calling fabsf
 */
template <class T>
inline const T abs(const T x)
{
  return ((x < 0) ? -x : x);
}

// abs_lt etc are faster than (abs(x1) < x2)
#define abs_lt(_x1,_x2) ((_x1) < (_x2) && -(_x1) < (_x2))
#define abs_gt(_x1,_x2) ((_x1) > (_x2) || -(_x1) > (_x2))
#define abs_le(_x1,_x2) ((_x1) <= (_x2) && -(_x1) <= (_x2))
#define abs_ge(_x1,_x2) ((_x1) >= (_x2) || -(_x1) >= (_x2))

// Speed oriented macros:
  // F for SP float
#define SGAL_PI          3.14159265358979323846f 

  // Slower DP for more precision
#define SGAL_PI_D        3.14159265358979323846  
#define SGAL_TWO_PI      6.283185307179586476925286766559f
#define SGAL_HALF_PI     (SGAL_PI/2.0)
#define SGAL_EPSILON     1e-6
#define SGAL_HUGEVAL     3.40282347e+37f

inline float deg2rad(float x) {return ((x) * SGAL_PI  / 180.0f);}
inline double deg2radd(double x) {return ((x) * SGAL_PI_D / 180.0);}

inline float rad2deg(float x) {return ((x) * 180.0f / SGAL_PI);}
inline double rad2degd(double x) {return ((x) * 180.0 / SGAL_PI_D);}

inline Uint bits2bytes(Uint bits) { return ((bits) + 7) >> 3; }

#if defined(__sun) || defined(__hpux)
inline float fmodf(float a, float b)
{
  return ((float)::fmod((double)(a),(double)(b)));
}
inline float ceilf(float a) {return ((float)::ceil((double)(a)));}
inline float floorf(float a) {return ((float)::floor((double)(a)));}
#endif

#if defined(__sun) || defined(__hpux)
inline float fsqrt(float a) {return sqrtf(a);}
#endif

#if defined(__sun)
inline float powf(float a, float b)
{
  return ((float)::pow((double)(a),(double)(b)));
}
#endif

#if defined(__WATCOMC__) || defined(x86) || defined(__sun)
inline float sqrtf(float a) {return ((float)::sqrt((double)(a)));}
inline float cosf(float a) {return ((float)::cos((double)(a)));}
inline float sinf(float a) {return ((float)::sin((double)(a)));}
inline float tanf(float a) {return ((float)::tan((double)(a)));}
inline float atanf(float a) {return ((float)::atan((double)(a)));}
inline float atan2f(float y, float x)
{
  return ((float)::atan2((double)(y),(double)(x)));
}
inline float acosf(float a) {return ((float)::acos((double)(a)));}
inline float asinf(float a) {return ((float)::asin((double)(a)));}
#endif

#if (defined __GNUC__) || (defined _MSC_VER)
inline float sinf(float a) {return ::sinf(a);}
inline float cosf(float a) {return ::cosf(a);}
inline float tanf(float a) {return ::tanf(a);}
inline float atan2f(float a, float b) {return ::atan2f(a, b);}
#endif
                                 
inline int round(float a) {return (int)(a+0.5f);}

inline bool is_sqr(float num) {return (num >= 0.0f && bm::isfinite(num));}

#if !defined(SGAL_DEBUG)
inline float squarerootf(float a) {return sqrtf(a);}
inline float arccosf(float rad) {return (acosf(rad));}
inline float arcsinf(float rad) {return (asinf(rad));}
inline void sincosf(float rad, float * s, float * c)
{
  *s = sinf(rad);
  *c = cosf(rad);
}
#else
inline float squarerootf(float a)
{
  ASSERT(is_sqr(a));
  return sqrtf(a);
}
inline float arccosf(float rad) 
{
  if (rad < -1) rad = -1;
  else if (rad > 1) rad = 1;
  return (acosf_int(rad));
}
inline float arcsinf(float rad) 
{ 
  if (rad < -1) rad = -1;
  else if (rad > 1) rad = 1;
  return (asinf(rad));
}

inline void sincosf(float rad, float * s, float * c)
{
  *s = sinf(rad);
  *c = cosf(rad);
}
#endif

/*! Fast square root.
 * Thresholds chosen so it's no worse than sqrtf()
 */
inline float sqrtf1(float x)
{
  return ((x > 0.9996f && x < 1.001f) ? 0.5f + 0.5f * x : sqrtf(x));
}

/*! Fast inverse square-root.
 * Thresholds chosen so it's no worse than sqrtf()
 */
inline float inverse_sqrtf1(float x)
{
  return ((x > 0.9996f && x < 1.001f) ? 1.5f - 0.5f * x : 1.0f / sqrtf(x));
}

/*! Generic square */
template <class T> inline const T square(const T x) {return (x * x);}

/*** FAST INVERSE SQUARE ROOT ***/

/* From "Graphics Gems V", Alan Paeth (Editor)
 * ISBN 0125434553/9649-1547332-306386
 * Published by Ap Profession, 1995
 */

/* Compute the Inverse Square Root
 * of an IEEE Single Precision Floating-Point number.
 *
 * Written by Ken Turkowski.
 */

/* Specified parameters */
#define LOOKUP_BITS    6   /* Number of mantissa bits for lookup */
#define EXP_POS       23   /* Position of the exponent */
#define EXP_BIAS     127   /* Bias of exponent */
/* The mantissa is assumed to be just down from the exponent */

/* Derived parameters */
#define LOOKUP_POS   (EXP_POS-LOOKUP_BITS)  /* Position of mantissa lookup */
#define SEED_POS     (EXP_POS-8)            /* Position of mantissa seed */
#define TABLE_SIZE   (2 << LOOKUP_BITS)     /* Number of entries in table */
#define LOOKUP_MASK  (TABLE_SIZE - 1)           /* Mask for table input */
#define GET_EXP(a)   (((a) >> EXP_POS) & 0xFF)  /* Extract exponent */
#define SET_EXP(a)   ((a) << EXP_POS)           /* Set exponent */
#define GET_EMANT(a) (((a) >> LOOKUP_POS) & LOOKUP_MASK)  /* Extended mantissa
                                                           * MSB's */
#define SET_MANTSEED(a) (((unsigned long)(a)) << SEED_POS)  /* Set mantissa
                                                             * 8 MSB's */
class SGAL_CLASSDEF Math {
public:
  // Non-WinTel platforms don't need fastcall:
#ifndef FASTCALL
#define FASTCALL
#endif

  Math() { make_inverse_sqrt_lookup_table(); }

  /*!
   * See http://en.wikipedia.org/wiki/Fast_inverse_square_root
   */
  static float FASTCALL sqrt_reciprocalf(float x);

private:
  static void make_inverse_sqrt_lookup_table(void);
  static unsigned char m_isqrt[TABLE_SIZE];
};

#if (defined _MSC_VER) || (defined __GNUC__)
inline float atoff(const char * str) { return (float) ::atof(str); }
#endif

#if (defined _MSC_VER)
inline float sqrtf(float X) { return (float) ::sqrt((float) X); }
#endif

SGAL_END_NAMESPACE

#endif
