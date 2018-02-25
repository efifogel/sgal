// Copyright (c) 2018 Israel.
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

#ifndef SGAL_LEXICAL_CAST_BOOLEAN_HPP
#define SGAL_LEXICAL_CAST_BOOLEAN_HPP

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

namespace boost {

// Do not the explicit second template type (const SGAL::String&).
// If you do, the (gcc) compiler will fail to find this correct spcialization
// even when the type of the single argument is exactly the same as the
// type of explicit template type.
// It seems like a bug in the compiler, but I'm not certain.
// I suspect that if you call the function (passing in a 'const String&'),
// the compiler first tries to mmatch using 'String', and such a specialization
// is provided by boost to start with.
template <>
inline SGAL::Boolean
lexical_cast<SGAL::Boolean, const SGAL::String&>(const SGAL::String& str)
{
  if (boost::iequals(str, "true")) return true;
  if (boost::iequals(str, "false")) return false;
  boost::bad_lexical_cast();
  return false;
}

}

#endif
