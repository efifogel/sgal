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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <limits>
#include <functional>

#include <boost/bind.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

//! The trace singleton.
Trace* Trace::s_instance = nullptr;

//! \brief obtains a trace singleton.
Trace* Trace::get_instance()
{
  if (!s_instance) s_instance = new Trace();
  return s_instance;
}

//! \brief constructs.
Trace::Trace() :
  m_signature(0x0)
{
  // Trace options.
  m_options = {
    {"", INVALID},
    {"graphics", GRAPHICS},
    {"vrml-parsing", VRML_PARSING},
    {"window-manager", WINDOW_MANAGER},
    {"events", EVENTS},
    {"script", SCRIPT},
    {"ifs", INDEXED_FACE_SET},
    {"ils", INDEXED_LINE_SET},
    {"polyhedron", POLYHEDRON},
    {"cgm", CUBICAL_GAUSSIAN_MAP},
    {"destructor", DESTRUCTOR},
    {"snapshot", SNAPSHOT},
    {"export", EXPORT},
    {"font", FONT},
    {"proto", PROTO}
  };

  m_free_codes.insert(ival::closed(NUM_CODES, MAX_NUM_CODES));
}

//! \brief obtains the number of trace options.
size_t Trace::options_size() const { return m_options.size(); }

//! \brief finds the trace code of an option.
size_t Trace::find_code(const String& opt) const
{
  auto it = m_options.find(opt);
  return (it == m_options.end()) ? INVALID : it->second;
}

//! \brief find the option of a code.
const String& Trace::find_option(size_t code) const
{
  auto it = std::find_if(m_options.begin(), m_options.end(),
                         (boost::bind(&Map::value_type::second, _1) == code));
  return (it != m_options.end()) ? it->first : find_option(INVALID);
}

//! \brief registers a trace option.
size_t Trace::register_option(const String& opt)
{
  SGAL_assertion_msg(! m_free_codes.empty(), "Increase MAX_NUM_CODES!");
  auto tit = m_options.find(opt);
  if (tit != m_options.end()) return tit->second;

  auto it = m_free_codes.begin();
  size_t code = it->lower();
  m_free_codes.erase(ival::closed(code, code+1));

  m_options[opt] = code;
  return code;
}

//! \brief unregisters a trace option.
void Trace::unregister_option(const String& opt)
{
  auto it = m_options.find(opt);
  if (it == m_options.end()) return;
  auto code = it->second;
  m_free_codes.insert(ival::closed(code, code));
}

//! \brief obtains the trace options container.
const Trace::Map& Trace::get_options() { return m_options; }

SGAL_END_NAMESPACE
