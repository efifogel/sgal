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

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
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

class SGAL_CLASSDEF Group : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    ISVISIBLE,
    CHILDREN,
    LAST
  };

  typedef std::list<Node *>             Node_list;
  typedef Node_list::iterator           Node_iterator;
  typedef Node_list::const_iterator     Node_const_iterator;

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
  
  /*! Obtain the number of children.
 * \return number of children in the group.
   */
  unsigned int get_child_count();

  /*! Obtain a child according to its position in the children sequence.
   * \param index the index of the child.
   * \return a pointer to the child object.
   */
  Node* get_child(unsigned int index);

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
  void add_child(Node* node);

  /* Remove a given child from the sequence of children of the group.
   * \param node (in) the child object
   */
  void remove_child(Node* node);

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

  /*! Add a touch sensor to the group. Also, set the selection id.
   * The selection id is used as a color to draw the object in selection mode.
   */
  void add_touch_sensor(Touch_sensor* touch_sensor);

  /*! Remove a touch sensor from the group. */
  void remove_touch_sensor(Touch_sensor* touch_sensor);

  /*! Determine whether the group has a touch sensor. */
  Boolean has_touch_sensor() const;

  /*! Obtain the selection id of the attached touch sensor if exists */
  unsigned int get_selection_id() const;

  /*! Add a light source to the group. */
  void add_light(Light* light);

  /*! Remove a light source from the group. */
  void remove_light(Light* light);

  /*! Determine whether the group has light sources. */
  Boolean has_lights() const;
  
  /*! Set the attributes of the group  extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Write this container */
  virtual void write(Formatter* formatter);

  /*! Write the children */
  void write_children(Formatter* formatter);

  /// \name Protoype handling
  //@{

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  //@}
  
  virtual Boolean attach_context(Context* context ); 

  virtual Boolean detach_context(Context* context = 0); 

  bool is_dynamic_loaded() { return false; }

protected:
  /*! Indicates whether the group is visible and thus should be rendered. */
  Boolean m_is_visible;

  /*! The list of child objects. */
  Node_list m_childs;

  /*! The unique id used when doing picking with the colour buffer. */
  Uint m_selection_id;

  /*! Indicates whether the group has a touch sensor. */
  Boolean m_has_touch_sensor;

  /*! The number of light sources in the group. */
  Uint m_num_lights;
    
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that represents the container. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;
};

/*! \brief constructs the prototype. */
inline Group* Group::prototype() { return new Group(true); }

/*! \brief clone. */
inline Container* Group::clone() { return new Group(); }
  
/*! \brief obtains the beginning iterator of the children. */
inline Group::Node_iterator Group::children_begin() { return m_childs.begin(); }
 
/*! \brief obtains the pass-the-end iterator of the children. */
inline Group::Node_iterator Group::children_end() { return m_childs.end(); }

/*! \brief obtains the selection id of the attached touch sensor if exists. */
inline Uint Group::get_selection_id() const { return m_selection_id; }

/*! \brief determines whether the group is visible */
inline Boolean Group::is_visible() const { return m_is_visible; }

/*! \brief determines whether the group has light sources. */
inline Boolean Group::has_lights() const { return (m_num_lights != 0); } 

/*! \brief determines whether the group has a touch sensor. */
inline Boolean Group::has_touch_sensor() const { return m_has_touch_sensor; } 

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Group::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
