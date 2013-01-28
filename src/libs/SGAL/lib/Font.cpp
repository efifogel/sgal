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
// $Revision: 5647 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/Font.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Font::Font(const std::string& name, Boolean antialias,
           Boolean left_to_right, Boolean top_to_bottom) :
  m_name(name),
  m_antialias(antialias),
  m_left_to_right(left_to_right),
  m_top_to_bottom(top_to_bottom)
{}

SGAL_END_NAMESPACE
