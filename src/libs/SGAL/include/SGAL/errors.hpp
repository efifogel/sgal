// Copyright (c) 2007 INRIA Sophia-Antipolis (France) and
// Max-Planck-Institute Saarbruecken (Germany).
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

#ifndef SGAL_ERRORS_HPP
#define SGAL_ERRORS_HPP

/*! \file Error exceptions for SGAL
 */

#include <string>
#include <stdexcept>

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4275 )
#endif

/*! Base class for all errors in the library */
class SGAL_SGAL_DECL error : public std::logic_error {
public:
  error(const std::string& what) : std::logic_error(what) {}
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! Class thrown when the input file cannot be found. */
class SGAL_SGAL_DECL File_not_found_error : public error {
public:
  File_not_found_error(const std::string& filename) :
    error(std::string("File ").append(filename).append(" not found!")) {}

  ~File_not_found_error() SGAL_NOTHROW {}
};

/*! Class thrown when a required extension is not supported. */
class SGAL_SGAL_DECL Extension_not_supported_error : public error {
public:
  Extension_not_supported_error(const std::string& extension) :
    error(std::string("Extension ").append(extension).append(" not supported!"))
  {}

  ~Extension_not_supported_error() SGAL_NOTHROW {}
};

/*! Class thrown when a required frame buffer is not allocated. */
class SGAL_SGAL_DECL Frame_buffer_not_allocated_error : public error {
public:
  Frame_buffer_not_allocated_error(const std::string& name) :
    error(std::string("Frame buffer ").append(name).append(" not allocated!"))
  {}

  ~Frame_buffer_not_allocated_error() SGAL_NOTHROW {}
};

/*! Class thrown when a visual cannot be selected. */
class SGAL_SGAL_DECL Visual_selection_error : public error {
public:
  Visual_selection_error() :
    error(std::string("Failed to choose Visual!")) {}

  ~Visual_selection_error() SGAL_NOTHROW {}
};

/*! Class thrown when a visual configuration fails. */
class SGAL_SGAL_DECL Visual_configuration_error : public error {
public:
  Visual_configuration_error() :
    error(std::string("Failed to configure Visual!")) {}

  ~Visual_configuration_error() SGAL_NOTHROW {}
};

SGAL_END_NAMESPACE

#endif
