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

#ifndef SGAL_LEXICAL_CAST_VECTOR3F_HPP
#define SGAL_LEXICAL_CAST_VECTOR3F_HPP

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f.hpp"

namespace boost {

template<>
inline std::string lexical_cast<std::string>(const SGAL::Vector3f& v)
{
  return std::string(std::to_string(v[0])).append(",").
    append(std::to_string(v[1].append(std::to_string(v[2]))));
}

template<>
inline SGAL::Vector3f lexical_cast<SGAL::Vector3f>(const std::string& str)
{
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  // Remove double quotes if present.
  std::string s((str.front() == '"') ? str.substr(1, str.size()-2) : str);
  tokenizer tokens(s, sep);
  SGAL::Float x(0), y(0), z(0);
  auto it = tokens.begin();
  if (it != tokens.end()) x = boost::lexical_cast<SGAL::Float>(*it++);
  if (it != tokens.end()) y = boost::lexical_cast<SGAL::Float>(*it++);
  if (it != tokens.end()) z = boost::lexical_cast<SGAL::Float>(*it++);
  if (it != tokens.end()) throw boost::bad_lexical_cast();
  return SGAL::Vector3f(x, y, z);
}

}

#endif
