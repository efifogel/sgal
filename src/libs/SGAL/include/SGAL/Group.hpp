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

#ifndef SGAL_GROUP_HPP
#define SGAL_GROUP_HPP

#include <string>
#include <vector>
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

/*! \class Group Group.hpp
 * Group is a node in the scene graph that has zero or more child nodes.
 * Actions applied to a group node are typically passed on to all of its
 * children. The bounding sphere of a group node contains the bounding
 * spheres of all its children.
 *
 * There are several types of group objects (types that inherit from this
 * type), e.g., Transform and Switch.
 *
 * A group node is associated with a single scene graph.
 * Light nodes and touch sensor nodes require special handling.
 * A group node may have at most one touch sensor node. The touch sensor
 * (if exists as a child node of the group) must be registered with the
 * scene graph.
 */
class SGAL_SGAL_DECL Group : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    IS_VISIBLE,
    CHILDREN,
    BBOX_CENTER,
    BBOX_SIZE,
    LAST
  };

  typedef boost::shared_ptr<Container>          Shared_container;
  typedef boost::shared_ptr<Node>               Shared_node;
  typedef boost::shared_ptr<Light>              Shared_light;
  typedef boost::shared_ptr<Touch_sensor>       Shared_touch_sensor;

  typedef std::vector<Shared_container>         Container_array;
  typedef Container_array::iterator             Container_iterator;
  typedef Container_array::const_iterator       Container_const_iterator;

  /*! Constructor.
   * \param proto determines whether to construct a prototype.
   */
  Group(Boolean proto = false);

  /*! Destructor. */
  virtual ~Group();

  /*! Construct the prototype.
   * \return the prototype.
   */
  static Group* prototype();

  /*! Clone this object.
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

  /// \name field handlers
  //@{
  Boolean* is_visible_handle(const Field_info*) { return &m_is_visible; }
  Container_array* childs_handle(const Field_info*) { return &m_childs; }
  Vector3f* bbox_center_handle(const Field_info*) { return &m_bbox_center; }
  Vector3f* bbox_size_handle(const Field_info*) { return &m_bbox_size; }
  //@}

  /*! Set the attributes of the group extracted, for example, from a VRML file.
   * \param elem contains lists of attribute name and value pairs.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Traverse the children of the group.
   * \param[in] action The traversal action record.
   */
  virtual Action::Trav_directive traverse(Action* action);

  /*! Draw the node while traversing the scene graph.
   * \param[in] draw_action The drawing action record.
   */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Cull the node if invisible and prepare for rendering.
   * \param cull_context The culling context record.
   */
  virtual void cull(Cull_context& cull_context);

  /*! Prepare the node for selection.
   * \param isect_action The selection action record.
   */
  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the group.
   */
  virtual void clean_bounding_sphere();

  /*! Associate the group node with a scene graph, so that the touch sensor
   * node, if exists, is registered with the scene graph.
   * \param[in] scene_graph The scene graph.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Process change of field.
   * \param field_info The information record of the field that changed.
   */
  virtual void field_changed(const Field_info* field_info);

  virtual Boolean attach_context(Context* context );

  virtual Boolean detach_context(Context* context = 0);

  /*! Clean the children in case they are dirty.
   */
  virtual void clean_childs();

  /*! Obtain the number of children.
   * \return number of children in the group.
   */
  Uint children_size();

  /*! Obtain a child according to its position in the children array.
   * \param index the index of the child.
   * \return the child node.
   */
  Shared_container get_child(Uint index);

  /*! Obtain the begin iterator of the children.
   * \return The begin iterator of the children.
   */
  Container_iterator children_begin();

  /*! Obtain the pass-the-end iterator of the children.
   * \return The pass-the-end iterator of the children.
   */
  Container_iterator children_end();

  /*! Turn on the flag that indicates whether the shape should be rendered. */
  void set_visible();

  /*! Turn off the flag that indicates whether the shape should be rendered. */
  void set_invisible();

  /*! Set the flag that indicates whether the shape should be rendered.
   * \param[in] flag The input flag.
   */
  void set_visible(Boolean flag);

  /*! Determine whether the group is visible.
   * \return a Boolean flag that indicates whether the group is visible.
   */
  Boolean is_visible() const;

  /* Add a child to the array of children of the group.
   * Lights appear in front of the sequence followed by all the rest.
   * \param[in] node the child node to add.
   */
  void add_child(Shared_container node);

  /*! Set the center of a bounding box that encloses the children of the
   * group.
   * \param bbox_center the bounding-box center.
   */
  void set_bbox_center(const Vector3f& bbox_center);

  /*! Obtain the center of a bounding box that encloses the children of the
   * group.
   * \return the bounding-box center.
   */
  const Vector3f& get_bbox_center() const;

  /*! Set the size of a bounding box that encloses the children of the
   * group.
   * \param bbox_size the bounding-box size.
   */
  void set_bbox_size(const Vector3f& bbox_size);

  /*! Obtain the size of a bounding box that encloses the children of the
   * group.
   * \return the bounding-box size.
   */
  const Vector3f& get_bbox_size() const;

  template <typename UnaryPredicate>
  void remove_children_if(UnaryPredicate pred)
  {
    const auto* field_info = get_field_info(BOUNDING_SPHERE);
    Observer observer(this, field_info);
    auto it = std::remove_if(m_childs.begin(), m_childs.end(), pred);
    m_childs.erase(it, m_childs.end());
    m_dirty_bounding_sphere = true;
    field_changed(field_info);
  }

  /* Remove a given child from the sequence of children of the group.
   * \param[in] node the child node to remove.
   */
  void remove_child(Shared_container node);

  /*! Add a touch sensor to the group.
   * \param[in] touch_sensor The touch sensor node to add.
   */
  void add_touch_sensor(Shared_touch_sensor touch_sensor);

  /*! Remove a touch sensor from the group.
   * \param[in] touch_sensor The touch sensor node to remove.
   */
  void remove_touch_sensor(Shared_touch_sensor touch_sensor);

  /*! Determine whether the group has a touch sensor.
   * \return a Boolean that indicates whether the group has a touch sensor.
   */
  Boolean has_touch_sensor() const;

  /*! Add a light source to the group.
   * \param[in] light The ligh node to add.
   */
  void add_light(Shared_light light);

  /*! Remove a light source from the group. */
  void remove_light(Shared_light light);

  /*! Determine whether the group has light sources. */
  Boolean has_lights() const;

  bool is_dynamic_loaded() { return false; }

  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

protected:
  //! Indicates whether the group is visible and thus should be rendered.
  Boolean m_is_visible;

  //! The list of child objects.
  Container_array m_childs;

  //! Indicates whether the group has a touch sensor.
  Shared_touch_sensor m_touch_sensor;

  //! The number of light sources in the group.
  Uint m_num_lights;

  //! The start id of a unique range of color ids used for selection.
  Uint m_start_selection_id;

  //! The number of ids of a unique range of color ids used for selection.
  Uint m_num_selection_ids;

  //! The scene graph.
  Scene_graph* m_scene_graph;

  //! Indicates whether the children are dirty and must be cleaned.
  Boolean m_dirty_childs;

  /*! Allocate the selection ids for this group.
   */
  void allocate_selection_ids();

private:
  //! The tag that represents the container.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! The center of a bounding box that encloses the children of the group.
  Vector3f m_bbox_center;

  //! The size of a bounding box that encloses the children of the group.
  Vector3f m_bbox_size;

  //! Default values.
  static const Vector3f s_def_bbox_center;
  static const Vector3f s_def_bbox_size;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Group* Group::prototype() { return new Group(true); }

//! \brief clone.
inline Container* Group::clone() { return new Group(); }

//! \brief obtains the beginning iterator of the children.
inline Group::Container_iterator Group::children_begin()
{ return m_childs.begin(); }

//! \brief obtains the pass-the-end iterator of the children.
inline Group::Container_iterator Group::children_end()
{ return m_childs.end(); }

//! \brief obtains the number of children of the group.
inline Uint Group::children_size() { return m_childs.size(); }

//! \brief determines whether the group is visible.
inline Boolean Group::is_visible() const { return m_is_visible; }

//! \brief determines whether the group has light sources.
inline Boolean Group::has_lights() const { return (m_num_lights != 0); }

//! \brief determines whether the group has a touch sensor.
inline Boolean Group::has_touch_sensor() const { return bool(m_touch_sensor); }

//! \brief adds the touch sensor to a given scene.
inline void Group::add_to_scene(Scene_graph* sg) { m_scene_graph = sg; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Group::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
