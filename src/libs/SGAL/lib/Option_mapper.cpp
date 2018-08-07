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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/bind.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Option_mapper.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Option_mapper::Option_mapper()
{
  m_options = {
    {"", INVALID}
  };
  m_free_codes.insert(ival::closed(m_options.size(),
                                   std::numeric_limits<size_t>::max()));
}

//! \brief destructs.
Option_mapper::~Option_mapper()
{
  m_free_codes.clear();
  m_options.clear();
}

//! \brief initializes the map.
void Option_mapper::set_options(Map&& options)
{
  SGAL_assertion(m_options.size() == 1);
  m_options = options;
  m_free_codes.clear();
  m_free_codes.insert(ival::closed(m_options.size(),
                                   std::numeric_limits<size_t>::max()));
}

//! \brief obtains the number of trace options.
size_t Option_mapper::options_size() const { return m_options.size(); }

//! \brief finds the trace code of an option.
size_t Option_mapper::find_code(const String& opt) const
{
  auto it = m_options.find(opt);
  return (it == m_options.end()) ? INVALID : it->second;
}

//! \brief find the option of a code.
const String& Option_mapper::find_option(size_t code) const
{
  auto it = std::find_if(m_options.begin(), m_options.end(),
                         (boost::bind(&Map::value_type::second, _1) == code));
  return (it != m_options.end()) ? it->first : find_option(INVALID);
}

//! \brief registers a trace option.
size_t Option_mapper::register_option(const String& opt)
{
  SGAL_assertion(! m_free_codes.empty());
  auto tit = m_options.find(opt);
  if (tit != m_options.end()) return tit->second;

  auto it = m_free_codes.begin();
  size_t code = it->lower();
  m_free_codes.erase(ival::closed(code, code+1));

  m_options[opt] = code;
  return code;
}

//! \brief unregisters a trace option.
void Option_mapper::unregister_option(const String& opt)
{
  auto it = m_options.find(opt);
  if (it == m_options.end()) return;
  auto code = it->second;
  m_free_codes.insert(ival::closed(code, code));
}

//! \brief obtains the trace options container.
const Option_mapper::Map& Option_mapper::get_options() { return m_options; }

SGAL_END_NAMESPACE
