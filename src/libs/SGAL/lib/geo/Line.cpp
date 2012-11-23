// Copyright (c) 2011 Israel.
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
// $Id: Line.cpp 11014 2011-04-05 17:13:17Z efif $
// $Revision: 11014 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Types.hpp"
#include "SGAL/basic.hpp"
#include "SGAL/assertions.hpp"
#include "SGAL/Line.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Line::Line(const Vector3f& p1, const Vector3f& p2) :
  m_point(p1)
{
  m_direction.sub(p1, p2);
}

SGAL_END_NAMESPACE
