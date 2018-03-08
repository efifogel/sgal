// Copyright (c) 2015 Israel.
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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOADER_ERRORS_HPP
#define SGAL_LOADER_ERRORS_HPP

#include "SGAL/basic.hpp"
#include "SGAL/errors.hpp"

SGAL_BEGIN_NAMESPACE

/*! Class thrown when the opening of a file fails. */
class SGAL_SGAL_DECL Loader_error : public error {
 private:
  std::string m_filename;

 public:
  const std::string& filename() const { return m_filename; }
  Loader_error(const std::string& message, const std::string& filename) :
    error(message),
    m_filename(filename)
  {}
  ~Loader_error() SGAL_NOTHROW {}
};

/*! Class thrown when a source file cannot be found. */
class SGAL_SGAL_DECL Find_file_error : public Loader_error {
public:
  Find_file_error(const std::string& filename) :
    Loader_error(std::string("Error: Failed to find file"), filename) {}
  ~Find_file_error() SGAL_NOTHROW {}
};

/*! Class thrown when the parsing of a stream fails. */
class SGAL_SGAL_DECL Parse_error : public Loader_error {
public:
  Parse_error(const std::string& filename) :
    Loader_error(std::string("Error: Failed to parse"), filename) {}
  ~Parse_error() SGAL_NOTHROW {}
};

/*! Class thrown when the data is inconistent. */
class SGAL_SGAL_DECL Inconsistent_error : public Loader_error {
public:
  Inconsistent_error(const std::string& filename) :
    Loader_error(std::string("Error: Inconsistent data"), filename) {}
  ~Inconsistent_error() SGAL_NOTHROW {}
};

/*! Class thrown when an overflow occurred. */
class SGAL_SGAL_DECL Overflow_error : public Loader_error {
public:
  Overflow_error(const std::string& filename) :
    Loader_error(std::string("Error: Overflow occurred"), filename) {}
  ~Overflow_error() SGAL_NOTHROW {}
};

/*! Class thrown when the reading of a stream fails. */
class SGAL_SGAL_DECL Read_error : public Loader_error {
public:
  Read_error(const std::string& filename) :
    Loader_error(std::string("Error: Failed to read"), filename) {}
  ~Read_error() SGAL_NOTHROW {}
};

/*! Class thrown when the opening of a file failed. */
class SGAL_SGAL_DECL Open_file_error : public Loader_error {
public:
  Open_file_error(const std::string& filename) :
    Loader_error(std::string("Error: Failed to open file"), filename) {}
  ~Open_file_error() SGAL_NOTHROW {}
};

/*! Class thrown when the file is empty. */
class SGAL_SGAL_DECL Empty_error : public Loader_error {
public:
  Empty_error(const std::string& filename) :
    Loader_error(std::string("Error: Empty"), filename) {}
  ~Empty_error() SGAL_NOTHROW {}
};

SGAL_END_NAMESPACE

#endif
