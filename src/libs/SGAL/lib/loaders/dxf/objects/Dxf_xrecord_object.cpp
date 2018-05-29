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

#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_xrecord_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_xrecord_object>  Dxf_xrecord_object_wrapper;

template <>
const std::map<int, Dxf_xrecord_object_wrapper::Record_member>
Dxf_xrecord_object_wrapper::s_record_members = {
  {280, {&Dxf_xrecord_object::m_flags, 1, 0}}
  // {1-4, STRING}
  // {6-9, STRING}
  // {10-59, DOUBLE}
  // {60-79, INT16}
  // {80-99, INT32}
  // {101-101, STRING}
  // {103-104, STRING}
  // {110-169, DOUBLE}
  // {170-179, INT16}
  // {181-269,
  // {270-279, INT16
  // {280-289, INT8
  // {290-369, except 330, 360
};

//! \brief handles a value that requires special handling.
bool Dxf_xrecord_object::handle_value(int code, const String& value)
{
  //! What to do with the values?
  if ((1 <= code) && (code <= 4)) return true;
  if ((6 <= code) && (code <= 9)) return true;
  if (101 == code) return true;
  if ((103 <= code) && (code <= 104)) return true;
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_xrecord_object::handle_value(int code, int8_t value)
{
  //! What to do with the values?
  if ((280 <= code) && (code <= 289)) return true;
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_xrecord_object::handle_value(int code, int16_t value)
{
  //! What to do with the values?
  if ((60 <= code) && (code <= 79)) return true;
  if ((170 <= code) && (code <= 179)) return true;
  if ((270 <= code) && (code <= 279)) return true;
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_xrecord_object::handle_value(int code, int32_t value)
{
  //! What to do with the values?
  if ((88 <= code) && (code <= 99)) return true;
  return false;
}

//! \brief handles a value that requires special handling.
bool Dxf_xrecord_object::handle_value(int code, double value)
{
  //! What to do with the values?
  if ((10 <= code) && (code <= 59)) return true;
  if ((110 <= code) && (code <= 169)) return true;
  return false;
}

SGAL_END_NAMESPACE
