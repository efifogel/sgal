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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * Interface to the Scene_graph that is used by the application  
 */

#ifndef SGAL_SCENE_GRAPH_INT_HPP
#define SGAL_SCENE_GRAPH_INT_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Event_filter;

#define LEVELS_URL_TAG  "LevelsURL"
#define TRUE_STR        "true"
#define FALSE_STR       "false"

class Node;
class Draw_action;
class Window_handle;
class Load_int;
class Execution_coordinator;
class SAI_services;
class Container;

const std::string g_navigation_root_name = "NAVIGATION_ROOT";
const std::string g_root_name = "ROOT";

class SGAL_SGAL_DECL Scene_graph_int {
public:
  Scene_graph_int() { }
  virtual ~Scene_graph_int() { }

  /*! creates the graphic context */
  virtual void create_context(Window_handle * win_handle) = 0;
  virtual void release_context() = 0;

  /*! Make the scene OGL context the current context */
  virtual void make_current() = 0;

#if 0
  /*! creates a node according to type. If the object is of type 
   * that can be loaded (GeoSet, Image) the node pointer refers 
   * to it as well. Otherwise the node pointer in NULL. 
   */
  virtual XML_entity * create_node(std::string tag, Load_int ** load_int = 0,
                                   Event_filter ** even_filter = 0) = 0;
#endif
  
  /*! creates the SAI object and returns a pointer to it */
  virtual SAI_services * create_sai() = 0;

  /*! dispose the given pointed SAI */
  virtual void dispose_sai(SAI_services * SAI) = 0;
  
  /*! signal the scene manager that an event cascade is about to start */
  virtual void signal_cascade_start() = 0;

  /*! signal the scene manager that an event cascade has just end */
  virtual void signal_cascade_end() = 0;

#if 0
  /*! add a node do the scene */
  virtual void add_to_scene(XML_entity * parent, XML_entity * node) = 0;
#endif
  
  /*! sets the root of the scene graph */
  virtual void set_root(Container * root) = 0;

  /*! sets the root of the navigation transform */
  virtual void set_navigation_root(Container * nr) = 0;

  /*! do all that is done prior to scene creation */
  virtual void init_creation() = 0;

  /*! tells the scene that it is ready for rendering */
  virtual void set_done() = 0;

  /*! render the scene graph */
  virtual void draw(Draw_action * draw_action) = 0;

  /*! traverse the tree in selection mode */
  virtual void isect(Uint x, Uint y) = 0;

  /*! return a pointer to the navigation sensor */
  virtual Event_filter * get_navigation_sensor() = 0;

  /*! get a pointer to the execution coordinator */
  virtual Execution_coordinator * get_execution_coordinator() = 0;

  /*! create the default event filter that catches the window related
   * evcents */
  virtual void set_default_event_filter(Event_filter * ef) = 0;

  /*! camera control **/
  virtual Boolean active_next_camera()  = 0;
  virtual Boolean active_prev_camera()  = 0;
  virtual Boolean is_last_camera_activated()  = 0;
  virtual Boolean is_first_camera_activated()  = 0;
  virtual Boolean has_more_then_one_camera() = 0;

#ifdef SGAL_MPEG_PROJECT
  virtual Container * get_Remote_keys() = 0;
#endif
};

SGAL_END_NAMESPACE

#endif
