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

#ifndef DXF_HEADER_FULL_WRAPPER_HPP
#define DXF_HEADER_FULL_WRAPPER_HPP

#include <list>
#include <map>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header_full.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header_full_wrapper {
  /// \name Header types
  //@{

  // Header data member types.
  typedef SGAL::String Dxf_header_full::*    String_header;
  typedef double Dxf_header_full::*          Double_header;
  typedef double (Dxf_header_full::*Double_2d_header)[2];
  typedef double (Dxf_header_full::*Double_3d_header)[3];
  typedef int8_t Dxf_header_full::*          Int8_header;
  typedef int16_t Dxf_header_full::*         Int16_header;
  typedef int32_t Dxf_header_full::*         Int32_header;
  typedef SGAL::Uint Dxf_header_full::*      Uint_header;
  typedef bool Dxf_header_full::*            Bool_header;

  //! The variant type of handle to all types of HEADER data members.
  typedef boost::variant<String_header,
                         Double_header,
                         Double_2d_header,
                         Double_3d_header,
                         Int8_header,
                         Int16_header,
                         Int32_header,
                         Uint_header,
                         Bool_header>   Header_member_type;

  //! Information of a header member.
  struct Header_member {
    Header_member(Header_member_type handle, std::list<int> codes) :
      m_handle(handle), m_codes(codes)
    {}
    Header_member_type m_handle;
    std::list<int> m_codes;
  };

  static const std::map<SGAL::String, Header_member> s_header_full_members;

  //@}
};

DXF_END_NAMESPACE

#endif
