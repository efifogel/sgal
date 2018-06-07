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

#ifndef SGAL_DXF_BASE_PARSER_HPP
#define SGAL_DXF_BASE_PARSER_HPP

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

//!
class SGAL_SGAL_DECL Dxf_base_parser {
  template<typename> friend struct Dxf_importer;

public:
  /*! Construct.
   * \param[i] is the input stream.
   * \param[i] filename the input filename.
   */
  Dxf_base_parser(std::istream& is, const String& filename);

protected:
   //! The text input stream to parse.
  std::istream& m_is;

  //! The file name
  const String& m_filename;

  //! The current line number
  size_t m_line;


public: //! \todo temporary
  //! The trace code.
  static size_t m_trace_code;
};

SGAL_END_NAMESPACE

#endif
