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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Math_defs.hpp"

SGAL_BEGIN_NAMESPACE

unsigned char Math::m_isqrt[TABLE_SIZE];

float FASTCALL Math::sqrt_reciprocalf(float /* x */)
{
  //! \todo sqrt_reciprocalf
  return 0;
}

void Math::make_inverse_sqrt_lookup_table(void)
{
  //! \todo make_inverse_sqrt_lookup_table
}

SGAL_END_NAMESPACE
