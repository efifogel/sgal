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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_POLYHEDRON_ATTRIBUTES_ACTION_HPP
#define SGAL_POLYHEDRON_ATTRIBUTES_ACTION_HPP

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Polyhedron_attributes_array.hpp"

SGAL_BEGIN_NAMESPACE

class Indexed_face_set;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Polyhedron_attributes_action Polyhedron_attributes_action.hpp
 */
class SGAL_SGAL_DECL Polyhedron_attributes_action : public Action {
public:
  typedef boost::shared_ptr<Node>                       Shared_node;
  typedef boost::shared_ptr<Indexed_face_set>           Shared_indexed_face_set;

  /*! Construct.
   */
  Polyhedron_attributes_action(Polyhedron_attributes_array& array);

  /*! Destruct. */
  virtual ~Polyhedron_attributes_action();

  /*! Apply the action to a given node.
   * \param[in] node  the node to apply the action on.
   */
  virtual Trav_directive apply(Shared_node node);

  /*! Add some volume to the accumulated volume.
   * \param[in] the volume to add.
   */
  void insert(Shared_indexed_face_set ifs);

  // /*! Obtain the polyhedron attributes array construct.
  //  */
  // const Polyhedron_attributes_array& get_array() const;

private:
  /*! The attributes of the polyhedrons. */
  Polyhedron_attributes_array& m_array;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs default.
inline Polyhedron_attributes_action::
Polyhedron_attributes_action(Polyhedron_attributes_array& array) :
  m_array(array)
{}

//! \brief destructs.
inline Polyhedron_attributes_action::~Polyhedron_attributes_action() {}

// //! \brief obtains the polyhedron attributes array construct.
// inline const Polyhedron_attributes_array&
// Polyhedron_attributes_action::get_array() const { return m_array; }

SGAL_END_NAMESPACE

#endif
