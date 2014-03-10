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

#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

#include "SGAL/Math_defs.hpp"

SGAL_BEGIN_NAMESPACE

unsigned char Math::m_isqrt[TABLE_SIZE];

float FASTCALL Math::sqrt_reciprocalf(float number)
{
  typedef boost::uint32_t Bits32;
  BOOST_STATIC_ASSERT(sizeof(float) == sizeof(Bits32));

  union Turner {
    float m_float;
    Bits32 m_bits;
  } turner;

  const float threehalfs = 1.5F;
  float x2 = number * 0.5F;
  float y = number;

  turner.m_float = y;
  Bits32 i = turner.m_bits;             // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);            // what the fuck?
  turner.m_bits = i;
  y = turner.m_float;
  y = y * (threehalfs - (x2 * y * y));  // 1st iteration
  // y  = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed
  return y;
}

void Math::make_inverse_sqrt_lookup_table(void)
{
  //! \todo make_inverse_sqrt_lookup_table
}

SGAL_END_NAMESPACE
