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

#include "dxf/basic.hpp"
#include "dxf/Dxf_record_wrapper.hpp"
#include "dxf/Dxf_block.hpp"

DXF_BEGIN_NAMESPACE

/* This object contains the description of DXF BLOCK variables.
 * The DXF BLOCK variables listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 *
 * EF: Why is block-name mapped twice (s 2 & 3 below)?
 */
typedef Dxf_record_wrapper<Dxf_block>           Dxf_block_wrapper;
template <>
const std::map<int, Dxf_block_wrapper::Record_member>
Dxf_block_wrapper::s_record_members = {
  {5, {&Dxf_block::m_handle, 1, 0}},
  {330, {&Dxf_block::m_owner_handle, 1, 0}},
  {8, {&Dxf_block::m_layer_name, 1, 0}},
  {2, {&Dxf_block::m_name, 1, 0}},
  {70, {&Dxf_block::m_flags, 1, 0}},
  {10, {&Dxf_block::m_base_point, 3, 0}},
  {20, {&Dxf_block::m_base_point, 3, 1}},
  {30, {&Dxf_block::m_base_point, 3, 2}},
  {3, {&Dxf_block::m_name, 1, 0}},
  {1, {&Dxf_block::m_xref_path_name, 1, 0}},
  {4, {&Dxf_block::m_description, 1, 0}}
  // {67, Space, that is, model or paper space
};

//! \brief handles a value that requires special handling.
bool Dxf_block::handle_value(int code, int16_t value)
{
  //! What to do with the value?
  if (code == 67) return true;
  return false;
}

DXF_END_NAMESPACE
