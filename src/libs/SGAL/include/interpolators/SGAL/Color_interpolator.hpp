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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Color interpolator engine
 *
 * Inherites from Vector3f_interpolator and replace the tag.
 */

#ifndef SGAL_COLOR_INTERPOLATOR_HPP
#define SGAL_COLOR_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Color_interpolator : public Vector3f_interpolator {
public:
  /*! Constructor */
  Color_interpolator(Boolean interpolate_flag = true, Boolean proto = false);

  /*! Destructor */
  virtual ~Color_interpolator() {}

  /* Construct the prototype */
  static Color_interpolator* prototype()
  { return new Color_interpolator(false, true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  virtual void delete_prototype() {}
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Color_interpolator::create()
{ return new Color_interpolator(); }

SGAL_END_NAMESPACE

#endif
