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
class Indexed_face_set;
class Scene_graph;

/*! \class Connected_components_splitter Connected_components_splitter.hpp
 * Connected_components_splitter is an engine node in the scene graph that splits
 * connected components of a single geometry node into separate geometry nodes.
 */
class SGAL_SGAL_DECL Connected_components_splitter : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    OPERAND,
    COMPONENTS,
    TRIGGER,
    LAST
  };

  typedef boost::shared_ptr<Indexed_face_set>   Shared_indexed_face_set;

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
  Shared_indexed_face_set* operand_handle(const Field_info*)
  { return &m_operand; }
  std::vector<Shared_indexed_face_set>* components_handle(const Field_info*)
  { return &m_components; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

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
  void set_operand(Shared_indexed_face_set operand);

  /*! Obtain the input operand, which is a mesh that might be open (may have
   * holes).
   * \return the input operand.
   */
  Shared_indexed_face_set get_operand() const;

  /*! Set the resulting components.
   * \param[in] components the resulting components.
   */
  void set_components(const std::vector<Shared_indexed_face_set>& components);

  /*! Obtain the resulting components.
   * \param[in] components the resulting components.
   */
  const std::vector<Shared_indexed_face_set>& get_components() const;

  /*! The input polygonal mesh, which might be open. */
  Shared_indexed_face_set m_operand;

  /*! The resulting components. */
  std::vector<Shared_indexed_face_set> m_components;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

protected:
  /*! The scene graph. */
  Scene_graph* m_scene_graph;

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
inline Connected_components_splitter::Shared_indexed_face_set
Connected_components_splitter::get_operand() const { return m_operand; }

//! \brief sets the input operand, which is a mesh that might be open.
inline void Connected_components_splitter::set_operand(Shared_indexed_face_set operand)
{ m_operand = operand; }

//! \brief sets the resulting components.
inline void Connected_components_splitter::
set_components(const std::vector<Shared_indexed_face_set>& components)
{ m_components = components; }

//! \brief obtains the resulting components.
inline const std::vector<Connected_components_splitter::Shared_indexed_face_set>&
Connected_components_splitter::get_components() const { return m_components; }

SGAL_END_NAMESPACE

#endif
