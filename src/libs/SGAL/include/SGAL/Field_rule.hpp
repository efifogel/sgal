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

#ifndef SGAL_FIELD_RULE_HPP
#define SGAL_FIELD_RULE_HPP

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

enum class Field_rule {
  RULE_IN = 0,
  RULE_OUT,
  RULE_FIELD,
  RULE_EXPOSED_FIELD,
  NUM_FIELD_RULES
};

/*! Obtain the name of the field rule.
 * \return the name of field rule.
 */
extern const char* get_field_rule_name(Field_rule id);

SGAL_END_NAMESPACE

#endif
