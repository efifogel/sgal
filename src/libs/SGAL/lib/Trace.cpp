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

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

//! Trace options.
std::map<String, size_t> Trace::s_trace_opts = {
  {"graphics", Trace::GRAPHICS},
  {"vrml-parsing", Trace::VRML_PARSING},
  {"window-manager", Trace::WINDOW_MANAGER},
  {"events", Trace::EVENTS},
  {"script", Trace::SCRIPT},
  {"ifs", Trace::INDEXED_FACE_SET},
  {"ils", Trace::INDEXED_LINE_SET},
  {"polyhedron", Trace::POLYHEDRON},
  {"cgm", Trace::CUBICAL_GAUSSIAN_MAP},
  {"destructor", Trace::DESTRUCTOR},
  {"snapshot", Trace::SNAPSHOT},
  {"export", Trace::EXPORT},
  {"font", Trace::FONT},
  {"proto", Trace::PROTO},
  {"dxf-parsing", Trace::DXF_PARSING}
};

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
  m_free_codes.insert(ival::closed(NUM_CODES, MAX_NUM_CODES));
}

//! \brief obtains the number of trace options.
size_t Trace::trace_opts_size() const { return s_trace_opts.size(); }

//! \brief finds the trace code of an option.
size_t Trace::find_trace_code(const String& opt) const
{
  auto it = s_trace_opts.find(opt);
  return (it == s_trace_opts.end()) ? INVALID : it->second;
}

//! \brief adds a trace option.
size_t Trace::add_trace_opt(const String& opt)
{
  SGAL_assertion_msg(! m_free_codes.empty(), "Increase MAX_NUM_CODES!");
  auto tit = s_trace_opts.find(opt);
  if (tit != s_trace_opts.end()) return tit->second;

  auto it = m_free_codes.begin();
  size_t code = it->lower();
  m_free_codes.erase(ival::closed(code, code+1));

  s_trace_opts[opt] = code;
  return code;
}

//! \brief removes a trace option.
void Trace::remove_trace_opt(const String& opt)
{
  auto it = s_trace_opts.find(opt);
  if (it == s_trace_opts.end()) return;
  auto code = it->second;
  m_free_codes.insert(ival::closed(code, code));
}

SGAL_END_NAMESPACE
