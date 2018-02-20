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

#ifndef SGAL_VOLUME_ACTION_HPP
#define SGAL_VOLUME_ACTION_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Volume_action Volume_action.hpp
 */
class SGAL_SGAL_DECL Volume_action : public Action {
public:
  /*! Construct default.
   */
  Volume_action();

  /*! Destruct. */
  virtual ~Volume_action();

  /*! Apply the action to a given node.
   * \param[in] node  the node to apply the action on.
   */
  virtual Trav_directive apply(Shared_node node);

  /*! Add some volume to the accumulated volume.
   * \param[in] the volume to add.
   */
  void add(Float volume);

  /*! Obtain the accumulated volume.
   * \return the accumulated volume.
   */
  Float volume();

private:
  /*! The accumulated volume. */
  Float m_volume;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs default.
inline Volume_action::Volume_action() : m_volume(0.0f) {}

//! \brief destructs.
inline Volume_action::~Volume_action() {}

//! \brief adds some volume to the accumulated volume.
inline void Volume_action::add(Float volume) { m_volume += volume; }

//! \brief obtains the accumulated volume.
inline Float Volume_action::volume() { return m_volume; }

SGAL_END_NAMESPACE

#endif
