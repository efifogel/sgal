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
 * Position interpolator engine
 *
 *: Inherites from Vector3f_interpolator and replace the tag.
 */

#ifndef SGAL_POSITION_INTERPOLATOR_HPP
#define SGAL_POSITION_INTERPOLATOR_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Vector3f_interpolator.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Position_interpolator Position_interpolator.hpp
 * Position_interpolator is used to perform linear interpolations between
 * positions (3D vectors). It is derived from Vector3f_interpolator, which in
 * turn is derived from Interpolator. keyValue is an array of Vector3f
 * representing the position at each knot point of the interpolation (see
 * Interpolator for more information on the array "keys", which contains the
 * knot point values). value is an engine output, which represents the position
 * at the current "fraction" of the interpolation.
 */
class SGAL_SGAL_DECL Position_interpolator : public Vector3f_interpolator {
public:
  /*! Constructor. */
  Position_interpolator(Boolean interpolate_flag = true,
                        Boolean proto = false);

  /*! Destructor. */
  virtual ~Position_interpolator();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Position_interpolator* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype,
inline Position_interpolator* Position_interpolator::prototype()
{ return new Position_interpolator(false, true); }

//! \brief clones.
inline Container* Position_interpolator::clone()
{ return new Position_interpolator (); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Position_interpolator::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
