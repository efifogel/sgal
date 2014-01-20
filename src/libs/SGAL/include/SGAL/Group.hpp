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
// $Revision: 12512 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GROUP_HPP
#define SGAL_GROUP_HPP

/*! \file
 * A node in the scene graph that can have child objects.
 *
 * A group is a node in the scene graph that can have one or more
 * child objects in the scene graph. There are several types of
 * group objects that inherit from this class. e.g. Transform.
 *
 * The child objects of a group can be of any node type. Among
 * these are light objects, engines, shapes and groups. In order
 * to get the right result, the child objects have to be stored
 * in a particular order. In the head of the list of child objects
 * wee need to store the light objects. Then the engines (not
 * implemented yet) and finally the rest of the group/shape objects.
 * The sorting is done in the process of adding a new child to the
 * group.
 *
 * Inherits from Node.
 */

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Cull_context;
class Draw_action;
class Isect_action;
class Container_proto;
class Element;
class Formatter;
class Touch_sensor;
class Light;
class Scene_graph;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Group : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    ISVISIBLE,
    CHILDREN,
    LAST
  };

  typedef boost::shared_ptr<Node>               Shared_node;
  typedef boost::shared_ptr<Light>              Shared_light;
  typedef boost::shared_ptr<Touch_sensor>       Shared_touch_sensor;

  typedef std::list<Shared_node>                Node_list;
  typedef Node_list::iterator                   Node_iterator;
  typedef Node_list::const_iterator             Node_const_iterator;

  /*! Constructor */
  Group(Boolean proto = false);

  /*! Copy constructor */
  Group(const Group& groop);

  /*! Destructor */
  virtual ~Group();

  /*! Construct the prototype. */
  static Group* prototype();

  /*! Clone. */
  virtual Container* clone();

  /// \name Protoype handling
  //@{

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  //@}

  /// \name field handlers
  //@{
  Boolean* is_visible_handle(const Field_info*) { return &m_is_visible; }
  //@}

  /*! Set the attributes of the group  extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the node while traversing the scene graph. */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Cull the node if invisible and prepare for rendering. */
  virtual void cull(Cull_context& cull_context);

  /*! Prepare the node for selection.
   * \param isect_action
   */
  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the group.
   * \return true iff the bounding sphere has changed during the clean.
   */
  virtual Boolean clean_sphere_bound();

  /*! Add the touch sensor object to a given scene.
   * This includes adding it to the container of touch sensors in the
   * scene graph.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Write this container */
  virtual void write(Formatter* formatter);

  /*! Write the children */
  void write_children(Formatter* formatter);

  virtual Boolean attach_context(Context* context );

  virtual Boolean detach_context(Context* context = 0);

  /*! Obtain the number of children.
   * \return number of children in the group.
   */
  Uint children_size();

  /*! Obtain a child according to its position in the children sequence.
   * \param index the index of the child.
   * \return a pointer to the child object.
   */
  Shared_node get_child(Uint index);

  /*! Obtain the beginning iterator of the children. */
  Node_iterator children_begin();

  /*! Obtain the pass-the-end iterator of the children. */
  Node_iterator children_end();

  /*! Turn on the flag that indicates whether the shape should be rendered. */
  void set_visible();

  /*! Turn off the flag that indicates whether the shape should be rendered. */
  void set_invisible();

  /*! Set the flag that indicates whether the shape should be rendered. */
  void set_visible(Boolean flag);

  /*! Determine whether the group is visible */
  Boolean is_visible() const;

  /* Add a child to the sequence of children of the group.
   * Lights appear in front of the sequence followed by all the rest.
   * \param node (in) the child object
   */
  void add_child(Shared_node node);

  /* Remove a given child from the sequence of children of the group.
   * \param node (in) the child object
   */
  void remove_child(Shared_node node);

  /*! Add a touch sensor to the group. */
  void add_touch_sensor(Shared_touch_sensor touch_sensor);

  /*! Remove a touch sensor from the group. */
  void remove_touch_sensor(Shared_touch_sensor touch_sensor);

  /*! Determine whether the group has a touch sensor. */
  Boolean has_touch_sensor() const;

  /*! Add a light source to the group. */
  void add_light(Shared_light light);

  /*! Remove a light source from the group. */
  void remove_light(Shared_light light);

  /*! Determine whether the group has light sources. */
  Boolean has_lights() const;

  bool is_dynamic_loaded() { return false; }

protected:
  /*! Indicates whether the group is visible and thus should be rendered. */
  Boolean m_is_visible;

  /*! The list of child objects. */
  Node_list m_childs;

  /*! Indicates whether the group has a touch sensor. */
  Shared_touch_sensor m_touch_sensor;

  /*! The number of light sources in the group. */
  Uint m_num_lights;

  /*! The start id of a unique range of color ids used for selection */
  Uint m_start_selection_id;

  /*! The number of ids of a unique range of color ids used for selection */
  Uint m_num_selection_ids;

  /*! The scene graph */
  Scene_graph* m_scene_graph;

  /*! Allocate the selection ids for this group. */
  void allocate_selection_ids();

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that represents the container. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Group* Group::prototype() { return new Group(true); }

/*! \brief clone. */
inline Container* Group::clone() { return new Group(); }

/*! \brief obtains the beginning iterator of the children. */
inline Group::Node_iterator Group::children_begin() { return m_childs.begin(); }

/*! \brief obtains the pass-the-end iterator of the children. */
inline Group::Node_iterator Group::children_end() { return m_childs.end(); }

/*! \brief obtains the number of children of the group. */
inline unsigned int Group::children_size() { return m_childs.size(); }

/*! \brief determines whether the group is visible */
inline Boolean Group::is_visible() const { return m_is_visible; }

/*! \brief determines whether the group has light sources. */
inline Boolean Group::has_lights() const { return (m_num_lights != 0); }

/*! \brief determines whether the group has a touch sensor. */
inline Boolean Group::has_touch_sensor() const { return (m_touch_sensor); }

/*! \brief adds the touch sensor to a given scene. */
inline void Group::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Group::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
