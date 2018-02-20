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
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Item_primitive {
public:
  /*! Destructor */
  virtual ~Item_primitive() {}

  std::string get_value(Uint param_id) const;

  void get_value(Uint param_id, float& value) const;

  void get_value(Uint param_id, int& value) const;

  void get_value(Uint param_id, bool& value) const;

  /*! Set a value of a parameter.
   * \param paramName (in) the id of the parameter
   * \param value (in) the value of the parameter in string format
   */
  void set_value(Uint param_id, const std::string& value);

  /*! Set a value of a parameter.
   * \param paramName (in) the id of the parameter.
   * \param value (in) the value of an integer parameter.
   */
  void set_value(Uint param_id, int value);

  /*! Set a value of a parameter.
   * \param paramName (in) the id of the parameter.
   * \param value (in) the value of an float parameter.
   */
  void set_value(Uint param_id, float value);

  /*! Set a value of a parameter.
   * \param paramName (in) the id of the parameter.
   * \param value (in) the value of a Boolean parameter.
   */
  void set_value(Uint param_id, bool value);

protected:
  typedef std::vector <std::string> String_vector;

  /*! Constructor */
  Item_primitive() {}

  String_vector m_attributes;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

SGAL_END_NAMESPACE

#endif
