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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FILE_FORMAT_3D_HPP
#define SGAL_FILE_FORMAT_3D_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL File_format_3d {
public:
  /*! Format ids */
  enum Id {
    NONE = 0,
    ID_WRL,
    ID_X3D,
    ID_OFF,
    ID_STL,
    ID_OBJ,
    ID_JSON,
    NUM_IDS
  };

  /*! Compare the name of the ith format to a given token.
   * \param[in] i The index of the format to compare against.
   * \param[in] token The token string to compare.
   */
  static Boolean compare_name(Uint i, const std::string& token);

  /*! obtains the ith format name.
   * \param[in] i The index of the format.
   * \return the name of the ith format.
   */
  static const Char* get_name(Uint i);

private:
  //! file format names also used as file extensions.
  static const Char* s_names[];
};

SGAL_END_NAMESPACE

#endif
