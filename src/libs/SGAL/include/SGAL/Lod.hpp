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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node in the scene graph that can choose for traversing one of child
 * objects.
 *
 * This is a level of detail node in the scene graph. It inherits
 * from Switch and therefore can have multiple child objects.
 */

#ifndef SGAL_LOD_HPP
#define SGAL_LOD_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph_defs.hpp"
#include "SGAL/Switch.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SGAL_SGAL_DECL Lod : public Switch {
public:
  enum {
    FIRST = Switch::LAST - 1,
    LAST
  };

  /*! Constructor */
  Lod(Boolean proto = false);

  /*! Destructor */
  virtual ~Lod();

  /*! Construct the prototype. */
  static Lod* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the container prototype. */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  virtual void cull(Cull_context & cull_context);

  virtual Action::Trav_directive draw(Draw_action* draw_action);

  virtual void isect(Isect_action* isect_action);

  virtual Node* get_choosen_node() const;

private:
  std::vector<float> m_range;
  Node* m_choosen_node;

  // LOD local center.
  Vector3f m_center;

  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Lod* Lod::prototype() { return new Lod(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Lod::create() { return new Lod(); }

SGAL_END_NAMESPACE

#endif
