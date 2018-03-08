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
// $Revision: 6690 $
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_MSVC_STANDARD_HEADER_FIXES_HPP
#define SGAL_MSVC_STANDARD_HEADER_FIXES_HPP

#pragma warning ( disable : 4503 )
// #pragma warning ( once : 4503 )

#include <cmath>
namespace std {
  using ::sqrt;
}

#include <cstddef>
namespace std {
  using ::size_t;
  using ::ptrdiff_t;
}

#include <ctime>
namespace std {
  using ::time_t;
}

#endif
