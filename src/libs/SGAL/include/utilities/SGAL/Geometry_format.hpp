// Copyright (c) 2014 Israel.
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

#ifndef SGAL_GEOMETRY_FORMAT_HPP
#define SGAL_GEOMETRY_FORMAT_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Option_mapper.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Geometry_format : public Option_mapper {
public:
  //! Format codes
  enum Code {
    WRL = START_CODE,
    X3D,
    OFF,
    STL,
    OBJ,
    JSON,
    NUM_CODES
  };

  /*! Obtain the trace singleton.
   * \return the trace singleton.
   */
  static Geometry_format* get_instance();

private:
  /*! Construct.
   */
  Geometry_format();

  //! The singleton.
  static Geometry_format* s_instance;
};

SGAL_END_NAMESPACE

#endif
