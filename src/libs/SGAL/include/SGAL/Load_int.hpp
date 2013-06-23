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
// $Source$
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOAT_INT_HPP
#define SGAL_LOAT_INT_HPP

/*! \file
 * Interface to Scene_graph objects which use async loading.  
 *
 * There are three methods inthis interrface: to get a pointer
 * to the data of the object, to convert the data (to tri strips
 * in case of geometry) and an update method to indicate that 
 * a level of the data is ready.
 * <P>This is a pure virtual class.
 */

#include "SGAL/basic.hpp"

SGAL_BEGIN_NAMESPACE

class Item_primitive;

class SGAL_SGAL_DECL Load_int {
public:
  Load_int() {}
  virtual ~Load_int() {}

  virtual void allocate_memory() = 0;

  /** Get the object data pointer (void * cast) of the decoded data */
  virtual Item_primitive * get_data() = 0;

  /** Conversion method. In case of a geometry, the implementation should
   * be conversion to tri strips */
  virtual void convert() = 0;

  /** Upadate the object using the new decoded data */
  virtual void update(Boolean is_last) = 0;

  /** Delete decoded data and reset item */
  virtual void delete_data() = 0;
};

SGAL_END_NAMESPACE

#endif
