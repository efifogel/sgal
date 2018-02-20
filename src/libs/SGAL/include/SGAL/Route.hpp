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

#ifndef SGAL_ROUTE_HPP
#define SGAL_ROUTE_HPP

/*! \file
 * Maintains a route between two containers.
 *
 * A route is a directional connection from an output field of one node to an
 * input field of another. The route itself is a node in the scene graph.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Draw_action;
class Field;
class Scene_graph;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Route : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    SSOURCEC_NODE,
    SSOURCEC_FIELD,
    DESTINATION_NODE,
    DESTINATION_FIELD,
    LAST
  };

  typedef boost::shared_ptr<Container>                  Shared_container;

  /*! Construct from prototype.
   */
  Route(Boolean proto = false);

  /*! Destruct. */
  virtual ~Route() {};

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Route* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /*! Obtain the source node.
   */
  Shared_container get_source_node();

  /*! Obtain the destination node.
   */
  Shared_container get_destination_node();

  /*! Obtain the source field within the source node.
   */
  Field* get_source_field();

  /*! Obtain the destination field within the destination node.
   */
  Field* get_destination_field();

  /*! Set the connection.
   */
  void set(Shared_container src_node, Field* src_field,
           Shared_container dst_node, Field* dst_field);

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The route source-node */
  Shared_container m_src_node;

  /*! The route source-field */
  Field* m_src_field;

  /*! The route destination-node */
  Shared_container m_dst_node;

  /*! The route destination-field */
  Field* m_dst_field;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Route* Route::prototype() { return new Route(true); }

//! \brief clones.
inline Container* Route::clone() { return new Route(); }

//! \brief obtains the source node.
inline Route::Shared_container Route::get_source_node() { return m_src_node; }

//! \brief obtains the destination node.
inline Route::Shared_container Route::get_destination_node()
{ return m_dst_node; }

//! \brief obtains the source field within the source node.
inline Field* Route::get_source_field() { return m_src_field; }

//! \brief obtains the destination field within the destination node.
inline Field* Route::get_destination_field() { return m_dst_field; }

SGAL_END_NAMESPACE

#endif
