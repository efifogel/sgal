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

#ifndef DXF_EXPORTER_HPP
#define DXF_EXPORTER_HPP

#include <iomanip>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Base_writer.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

/*! Export a value.
 */
template <typename T>
struct Dxf_exporter {

  SGAL::Base_writer& m_writer;

  Dxf_exporter(SGAL::Base_writer& writer) : m_writer(writer) {}

  void operator()(const T& value)
  { m_writer.out() << value << std::endl; }
};

/*! Export an int8_t value.
 * C/C++ defines int8_t to be 'signed char'; thus, a negative integer cannot
 * be imported directly, since the preceding '-' is interpreted as the (sole)
 * char input.
 */
template <>
struct Dxf_exporter<int8_t> {

  SGAL::Base_writer& m_writer;

  Dxf_exporter(SGAL::Base_writer& writer) : m_writer(writer) {}

  void operator()(const int8_t& value)
  { m_writer.out() << value << std::endl; }
};

/*! Export a hex value.
 */
template <>
struct Dxf_exporter<SGAL::Uint> {

  SGAL::Base_writer& m_writer;

  Dxf_exporter(SGAL::Base_writer& writer) : m_writer(writer) {}

  void operator()(const SGAL::Uint& value)
  { m_writer.out() << value << std::endl; }
};

/*! Export an SGAL::String value.
 */
template <>
struct Dxf_exporter<SGAL::String> {

  SGAL::Base_writer& m_writer;

  Dxf_exporter(SGAL::Base_writer& writer) : m_writer(writer) {}

  void operator()(const SGAL::String& value)
  { m_writer.out() << value << std::endl; }
};

/*! Export a double.
 */
template <>
struct Dxf_exporter<double> {

  SGAL::Base_writer& m_writer;

  Dxf_exporter(SGAL::Base_writer& writer) : m_writer(writer) {}

  void operator()(const double& value)
  {
    m_writer.out() << std::setprecision(12) << value
                   << std::endl;
  }
};

DXF_END_NAMESPACE

#endif
