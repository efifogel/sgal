// Copyright (c) 2004 Israel.
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
// $Id: $
// $Revision: 1311 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Base class for async. loaded primitive items
 */

#ifndef SGAL_ITEM_PRIMITIVE_HPP
#define SGAL_ITEM_PRIMITIVE_HPP

#include <string>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
template class SGAL_SGAL_DECL std::allocator<std::string>;
template class SGAL_SGAL_DECL std::vector<std::string>;
#endif

class SGAL_SGAL_DECL Item_primitive {
public:
  /*! Destructor */
  virtual ~Item_primitive() {}

  std::string get_value(unsigned int param_id) const;
  void get_value(unsigned int param_id, float& value) const;
  void get_value(unsigned int param_id, int& value) const;
  void get_value(unsigned int param_id, bool& value) const;
  void set_value(unsigned int param_id, const std::string& value);
  void set_value(unsigned int param_id, int value);
  void set_value(unsigned int param_id, float value);
  void set_value(unsigned int param_id, bool value);

protected:
  typedef std::vector <std::string> String_vector;

  /*! Constructor */
  Item_primitive() {}

  String_vector m_attributes;
};

SGAL_END_NAMESPACE

#endif
