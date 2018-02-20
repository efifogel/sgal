// Copyright (c) 2015 Israel.
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

#ifndef SGAL_SURFACE_AREA_ACTION_HPP
#define SGAL_SURFACE_AREA_ACTION_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Surface_area_action Surface_area_action.hpp
 */
class SGAL_SGAL_DECL Surface_area_action : public Action {
public:
  /*! Construct default.
   */
  Surface_area_action();

  /*! Destruct. */
  virtual ~Surface_area_action();

  /*! Apply the action to a given node.
   * \param[in] node  the node to apply the action on.
   */
  virtual Trav_directive apply(Shared_node node);

  /*! Add some area to the accumulated area.
   * \param[in] the area to add.
   */
  void add(Float area);

  /*! Obtain the accumulated area.
   * \return the accumulated area.
   */
  Float surface_area();

private:
  /*! The accumulated area. */
  Float m_area;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs default.
inline Surface_area_action::Surface_area_action() : m_area(0.0f) {}

//! \brief destructs.
inline Surface_area_action::~Surface_area_action() {}

//! \brief adds some area to the accumulated volume.
inline void Surface_area_action::add(Float area) { m_area += area; }

//! \brief obtains the accumulated area.
inline Float Surface_area_action::surface_area() { return m_area; }

SGAL_END_NAMESPACE

#endif
