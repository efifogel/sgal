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

#ifndef SGAL_DXF_SIMPLE_RECORD_WRAPPER_HPP
#define SGAL_DXF_SIMPLE_RECORD_WRAPPER_HPP

#include <map>

#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Record_>
struct SGAL_SGAL_DECL Dxf_simple_record_wrapper {
  typedef Record_                               Record;

  ///
  //@{ Types and data of data members.

  // Record types:
  typedef String Record::*                      String_record;
  typedef bool Record::*                        Bool_record;
  typedef int8_t Record::*                      Int8_record;
  typedef int16_t Record::*                     Int16_record;
  typedef int32_t Record::*                     Int32_record;
  typedef double Record::*                      Double_record;
  typedef Uint Record::*                        Uint_record;

  typedef boost::variant<String_record,
                         Bool_record,
                         Int8_record,
                         Int16_record,
                         Int32_record,
                         Double_record,
                         Uint_record>           Record_member_type;

  //!
  static const std::map<int, Record_member_type> s_record_members;
  //@}
};

SGAL_END_NAMESPACE

#endif
