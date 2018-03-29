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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Field_rule.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief the names of the field rules.
static const char* s_rule_names[] = {
  "eventIn", "eventOut", "field", "exposedField"
};

//! \brief obtains the name of a rule.
const char* get_field_rule_name(Field_rule id)
{ return s_rule_names[static_cast<int>(id)]; }

SGAL_END_NAMESPACE
