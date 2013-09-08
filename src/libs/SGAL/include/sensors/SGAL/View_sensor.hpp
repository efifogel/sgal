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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_VIEW_SENSOR_HPP
#define SGAL_VIEW_SENSOR_HPP

/*! \file
 * The View Sensor senses the current scene view
 *
 * The translation and rotation fields are updated each application
 * cycle with the scene graph's current view (it is calculated
 * regarding both the navigation root transformation and the current camera).
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Action.hpp"
//! \todo #include "Event_filter.h"

SGAL_BEGIN_NAMESPACE

class Camera_pool;
class Transform;
class Navigation_sensor;
class Container_proto;
class Element;

class View_sensor : public Node /*! \todo , public Event_filter */ {
public:
  enum {
    FIRST = Node::LAST - 1,
    TRANSLATION,
    ROTATION,
    FOV,
    LOCALVIEWNAME,
    CURRENTVIEWNAME,
    LAST
  };

  /*! Constructor */
  View_sensor(Boolean proto = false);

  /*! Destructor */
  virtual ~View_sensor(){};

  /*! Construct the prototype. */
  static View_sensor* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();
  virtual void delete_prototyp();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /// \name field handlers
  //@{
  Vector3f* &m_translation;
  Rotation* &m_rotation;
  Float* (FOV, "fieldOfView",
                                         get_member_offset(&m_FOV)));

  // localViewName
  s_prototype->AddFieldInfo(new SF_string(LOCALVIEWNAME, "localViewName",
                                          get_member_offset(&m_local_view_name)));

  // currentViewName
  s_prototype->
    AddFieldInfo(new SF_string(CURRENTVIEWNAME, "currentViewName",
                               get_member_offset(&m_current_view_name)));
  //@}

  // Initialize the object
  void init(Camera_pool* camera_pool, Transform* navigation_root,
            Navigation_sensor* navigation_sensor = NULL);

  virtual Boolean update();

  virtual Action::Trav_directive draw(Draw_action* /* draw_action */);

protected :
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  Vector3f m_translation;
  Rotation m_rotation;
  Float m_fov;

  Camera_pool* m_camera_pool;
  Transform* m_navigation_root;
  Navigation_sensor* m_navigation_sensor;

  std::string m_local_view_name;
  std::string m_current_view_name;
  Camera* m_last_camera;
  Boolean m_local_view_name_set;
};

/*! \brief constructs the prototype. */
inline View_sensor* View_sensor::prototype() { return new View_sensor(true); }

/*! \brief clones. */
inline Container* View_sensor::clone() { return new View_sensor(); }

inline Action::Trav_directive View_sensor::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& View_sensor::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
