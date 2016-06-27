// Copyright (c) 2014 Israel.
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

#include "SGAL/basic.hpp"
#include "SGAL/File_format_3d.hpp"

SGAL_BEGIN_NAMESPACE

//! file format options.
const Char* File_format_3d::s_names[] = {
  "none",
  "wrl",
  "x3d",
  "off",
  "stl",
  "obj"
};

//! \brief compares the name of the ith format to a given token.
Boolean File_format_3d::compare_name(Uint i, const std::string& token)
{ return (token.compare(s_names[i]) == 0); }

//! obtains the ith format name.
const Char* File_format_3d::get_name(Uint i) { return s_names[i]; }

SGAL_END_NAMESPACE
