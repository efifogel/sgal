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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ROUTE_HPP
#define SGAL_ROUTE_HPP

/*! \file
 * Maintains a route between two containers.
 *
 * A route is a directional connection from an output field of one node to an
 * input field of another. The route itself is a node in the scene graph.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Draw_action;
class Field;
class Scene_graph;

class SGAL_SGAL_DECL Route : public Container {
public:
  /*! Constructor */
  Route(Boolean proto = false);
  
  /*! Destructor */
  virtual ~Route() {};

  /*! Construct the prototype */
  static Route* prototype() { return new Route(true); }

  /*! Clone */
  virtual Container* clone() { return new Route(); }

  /*! Set the connection */
  void set(Container* src_node, Field* src_field,
           Container* dst_node, Field* dst_field);
  
  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The route source-node */
  Container* m_src_node;

  /*! The route source-field */
  Field* m_src_field;

  /*! The route destination-node */
  Container* m_dst_node;

  /*! The route destination-field */
  Field* m_dst_field;
};

SGAL_END_NAMESPACE

#endif
