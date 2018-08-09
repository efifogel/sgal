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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <fstream>

#include "SGAL/basic.hpp"
#include "SGAL/Writer.hpp"
#include "SGAL/Base_writer.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Geometry_format.hpp"

SGAL_BEGIN_NAMESPACE

//! The Loader singleton.
Writer* Writer::s_instance(nullptr);

//! \brief constructs.
Writer::Writer() {}

//! \brief destructs.
Writer::~Writer() { m_writers.clear(); }

//! \brief obtains the writer singleton.
Writer* Writer::get_instance()
{
  if (!s_instance) s_instance = new Writer();
  return s_instance;
}

//! \brief registers a writer.
void Writer::doregister(size_t format, Base_writer* writer)
{ m_writers[format] = writer; }

//! \brief writes a scene to a file.
void Writer::operator()(Scene_graph* sg, const std::string& filename,
                        size_t format, bool is_binary)
{
  if (filename.empty()) operator()(sg, filename, std::cout, format, is_binary);
  else {
    std::ios_base::openmode mode = (is_binary) ?
      (std::ios_base::out | std::ios_base::binary) : std::ios_base::out;
    std::ofstream os(filename, mode);
    if (!os.is_open()) return;
    operator()(sg, filename, os, format, is_binary);
    os.close();
  }
}

//! \brief writes a scene to a stream.
void Writer::operator()(Scene_graph* sg, const std::string& filename,
                        std::ostream& os, size_t format, bool is_binary)
{
  switch (format) {
   case Geometry_format::WRL: sg->write_vrml(filename, os); break;
   case Geometry_format::OFF:
    SGAL_error_msg("Exporting OFF is not yet supported!");
    break;

   case Geometry_format::STL: sg->write_stl(filename, os, is_binary); break;
   case Geometry_format::OBJ: sg->write_obj(filename, os); break;
   case Geometry_format::JSON: sg->write_json(filename, os); break;

   default:
    // Try registered writers
    {
      auto it = m_writers.find(format);
      SGAL_assertion(it != m_writers.end());
      auto& writer = *(it->second);
      writer(sg, os, filename, is_binary);
    }
    break;
  }
}

SGAL_END_NAMESPACE
