// Copyright (c) 2016 Israel.
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

#ifndef SGAL_CONNECTED_COMPONENTS_SPLITTER_HPP
#define SGAL_CONNECTED_COMPONENTS_SPLITTER_HPP

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/variant.hpp>

#include <CGAL/basic.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Inexact_polyhedron.hpp"
#include "SGAL/Epic_polyhedron.hpp"
#include "SGAL/Epec_polyhedron.hpp"

SGAL_BEGIN_NAMESPACE

namespace PMP = CGAL::Polygon_mesh_processing;

class Field_info;
class Container_proto;
class Mesh_set;

/*! \class Connected_components_splitter Connected_components_splitter.hpp
 * Connected_components_splitter is an engine node in the scene graph that splits
 * connected components of a single geometry node into separate geometry nodes.
 */
class SGAL_SGAL_DECL Connected_components_splitter : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    OPERAND,
    RESULT,
    TRIGGER,
    LAST
  };

  typedef boost::shared_ptr<Mesh_set>               Shared_mesh_set;

  /*! Construct.
   */
  Connected_components_splitter(Boolean proto = false);

  /*! Destruct.
   */
  virtual ~Connected_components_splitter();

  /* Construct the prototype */
  static Connected_components_splitter* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Shared_mesh_set* operand_handle(const Field_info*) { return &m_operand; }
  std::vector<Shared_mesh_set>* result_handle(const Field_info*)
  { return &m_result; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Export this container.
   * \param[in] formatter the formatter to use for exporting; e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! triggers the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! Execute the engine. */
  void execute();

  /*! Set the input operand, which is a mesh that might be open (may have holes).
   * \param operand the input operand.
   */
  void set_operand(Shared_mesh_set operand);

  /*! Obtain the input operand, which is a mesh that might be open (may have
   * holes).
   * \return the input operand.
   */
  Shared_mesh_set get_operand() const;

  /*! Set the resulting geometry.
   * \param[in] result the resulting geomery.
   */
  void set_result(const std::vector<Shared_mesh_set>& result);

  /*! Obtain the resulting geometry.
   * \param[in] result the resulting geomery.
   */
  const std::vector<Shared_mesh_set>& get_result() const;

  /*! The input polygonal mesh, which might be open. */
  Shared_mesh_set m_operand;

  /*! The resulting polygonal closed mesh. */
  std::vector<Shared_mesh_set> m_result;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! Trigger of the engine that makes the engine excute. */
  Boolean m_trigger;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The prototype of this node. */
  static Container_proto* s_prototype;

  /*! Default values. */
};

//! \brief constructs the prototype.
inline Connected_components_splitter* Connected_components_splitter::prototype()
{ return new Connected_components_splitter(true); }

//! \brief clones.
inline Container* Connected_components_splitter::clone() { return new Connected_components_splitter(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Connected_components_splitter::get_tag() const
{ return s_tag; }

//! \brief obtains the input operand, which is a mesh that might be open.
inline Connected_components_splitter::Shared_mesh_set
Connected_components_splitter::get_operand() const { return m_operand; }

//! \brief sets the input operand, which is a mesh that might be open.
inline void Connected_components_splitter::set_operand(Shared_mesh_set operand)
{ m_operand = operand; }

//! \brief sets the resulting geometry.
inline void Connected_components_splitter::
set_result(const std::vector<Shared_mesh_set>& result) { m_result = result; }

//! \brief obtains the resulting geometry.
inline const std::vector<Connected_components_splitter::Shared_mesh_set>&
Connected_components_splitter::get_result() const { return m_result; }

SGAL_END_NAMESPACE

#endif
