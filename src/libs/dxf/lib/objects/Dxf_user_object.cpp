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
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_user_object.hpp"

DXF_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_user_object>  Dxf_user_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_user_object_wrapper::Record_member>
Dxf_user_object_wrapper::s_record_members = {};

//! Record handlers
template <>
const std::map<int, Dxf_user_object_wrapper::Record_handler_type>
Dxf_user_object_wrapper::s_record_handlers = {};

//! \brief handles a string value;
bool Dxf_user_object::handle_value(int code, const SGAL::String& value)
{
  //! \todo Implement
  return true;
}

//! \brief handles an int8_t value;
bool Dxf_user_object::handle_value(int code, int8_t value)
{
  //! \todo Implement
  return true;
}

//! \brief handles an int16_t value;
bool Dxf_user_object::handle_value(int code, int16_t value)
{
  //! \todo Implement
  return true;
}

//! \brief handles an int32_t value;
bool Dxf_user_object::handle_value(int code, int32_t value)
{
  //! \todo Implement
  return true;
}

//! \brief handles a SGAL::Uint value;
bool Dxf_user_object::handle_value(int code, uint value)
{
  //! \todo Implement
  return true;
}

//! \brief handles a bool value;
bool Dxf_user_object::handle_value(int code, bool value)
{
  //! \todo Implement
  return true;
}

//! \brief handles a double value;
bool Dxf_user_object::handle_value(int code, double value)
{
  //! \todo Implement
  return true;
}

DXF_END_NAMESPACE
