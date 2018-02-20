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

#ifndef SGAL_FIND_FILE_HPP
#define SGAL_FIND_FILE_HPP

#include <string>
#include <list>

#include <boost/filesystem/path.hpp>

#include "SGAL/basic.hpp"

namespace fi = boost::filesystem;

SGAL_BEGIN_NAMESPACE

/*! Find a file in given directories.
 */
extern void find_file(const std::string& filename,
                      std::list<fi::path>& m_dirs,
                      std::string& fullname);

SGAL_END_NAMESPACE

#endif
