// Copyright (c) 2017 Israel.
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

#include <boost/filesystem.hpp>

#include "SGAL/find_file.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief finds a file.
void find_file(const std::string& const_filename, std::list<fi::path>& m_dirs,
               std::string& fullname)
{
  std::string filename(const_filename);

  SGAL_assertion(!filename.empty());

#if (defined _MSC_VER)
  // Convert the ROOT from cygwin path to windows path, if relevant:
  std::string cygdrive = filename.substr(0, 10);
  if (cygdrive == std::string("/cygdrive/")) {
    filename.erase(0, 10);
    filename.insert(1, ":");
  }
#endif

  fi::path file_path(filename);
  if (file_path.is_complete()) {
    if (fi::exists(file_path)) fullname = file_path.string();
    return;
  }

  for (auto& pi : m_dirs) {
    fi::path full_file_path = pi / file_path;
    if (!fi::exists(full_file_path)) continue;
    fullname = full_file_path.string();
    return;
  }
}

SGAL_END_NAMESPACE
