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
 * Position interpolator engine
 *
 *: Inherites from Vector3f_interpolator and replace the tag.
 */

#ifndef SGAL_POSITION_INTERPOLATOR_HPP
#define SGAL_POSITION_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

class Position_interpolator : public Vector3f_interpolator {
public:
  /*! Constructor */
  Position_interpolator(Boolean interpolate_flag = true,
                        Boolean proto = SGAL_FALSE);
  
  /*! Destructor */
  virtual ~Position_interpolator() {};

  /* Construct the prototype */
  static Position_interpolator * prototype()
  { return new Position_interpolator(SGAL_FALSE, SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Position_interpolator (); }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;
};

SGAL_END_NAMESPACE

#endif
