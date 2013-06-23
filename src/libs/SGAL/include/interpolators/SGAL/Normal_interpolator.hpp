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

/*!
 * Normal interpolator engine
 *
 * Inherites from Vector3f_interpolator and replace the tag.
 */

#ifndef SGAL_NORMAL_INTERPOLATOR_HPP
#define SGAL_NORMAL_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "Vector3f_interpolator.h"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Normal_interpolator : public Vector3f_interpolator {
public:
  /*! Constructor */
  Normal_interpolator(Boolean interpolate_flag = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Normal_interpolator() {};

  /* Construct the prototype */
  static Normal_interpolator* prototype()
  { return new Normal_interpolator(false, true); }

  /*! Clone */
  virtual Container* clone() { return new Normal_interpolator (); }
};

SGAL_END_NAMESPACE

#endif
