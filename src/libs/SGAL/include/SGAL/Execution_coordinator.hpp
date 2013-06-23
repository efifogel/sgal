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

#ifndef SGAL_EXECUTION_COORDINATOR_HPP
#define SGAL_EXECUTION_COORDINATOR_HPP

/*! \file
 * Handles coordination functions and flags of the execution of the player.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/App_error_handle_int.hpp"

SGAL_BEGIN_NAMESPACE

class App_error_handle_int;

enum Cursor_type {
  ctUNDEFINED = -1,
  ctARROW = 0,
  ctHAND = 1,
  ctGRABHAND = 2,
  ctMOVE = 3,
  ctZOOM = 4
};

class Key_sensor;
class Flow_sensor;

class SGAL_SGAL_DECL Execution_coordinator {
public:
  Execution_coordinator();
  virtual ~Execution_coordinator(){};

  /*! performs reset of flags that shuld be reset every application work cycle
   */
  void cycle_reset()
  {
    m_is_mouse_over = SGAL_FALSE;
  }

  /*!
   */
  Boolean is_bounding_sphere_modified() const
  {
    return m_bounding_sphere_modified;
  }

  /*!
   */
  Boolean is_rendering_required() const
  {
    return m_rendering_required;
  }

  /*!
   */
  Boolean is_dragging_locked() const
  {
    return m_dragging_locked;
  }

  /*!
   */
  Boolean is_scope_locked() const
  {
    return m_scope_locked;
  }

  /*!
   */
  void set_bounding_sphere_modified(Boolean bounding_sphere_modified)
  {
    m_bounding_sphere_modified = bounding_sphere_modified;
  }

  /*!
   */
  void set_rendering_required()
  {
    m_rendering_required = SGAL_TRUE;
    m_is_aa_interrupt = SGAL_TRUE;
  }

  /*!
   */
  void reset_rendering_required()
  {
    m_rendering_required = SGAL_FALSE;
  }

  /*!
   */
  void lock_draging()
  {
    m_dragging_locked = SGAL_TRUE;
  }

  /*!
   */
  void unlock_dragging()
  {
    m_dragging_locked = SGAL_FALSE;
  }

  /*!
   */
  void lock_scope()
  {
    m_scope_locked = SGAL_TRUE;
  }

  /*!
   */
  void unlock_scope()
  {
    m_scope_locked = SGAL_FALSE;
  }

  /*!
   */
  static Scene_time get_scene_time();
  
  /*!
   */
  void set_cursor_type(Cursor_type ct) 
  {
    m_current_cursor_id = ct;
  }

  Cursor_type get_cursor_type() 
  {
    return m_current_cursor_id;
  }

  /*!
   */
  void set_is_cursor_on() 
  {
    m_is_cursor_on = SGAL_TRUE;
  }

  /*!
   */
  void reset_is_cursor_on() 
  {
    m_is_cursor_on = SGAL_FALSE;
  }

  /*!
   */
  Boolean is_cursor_on() 
  {
    return m_is_cursor_on;
  }

  /*!
   */
  void set_mouse_over() 
  {
    m_is_mouse_over = SGAL_TRUE;
  }

  /*!
   */
  Boolean is_mouse_over() 
  {
    return m_is_mouse_over;
  }

  /*!
   */
  void set_loading_done(Boolean flag = SGAL_TRUE);

  /*!
   */
  void signal_level_0rendered();

  /*!
   */
  void signal_last_level_rendered();

  /*!
   */
  void signal_snapshot_done();

  /*!
   */
  Boolean is_loading_done() 
  {
    return m_is_loading_done;
  }

  /*!
   */
  void set_current_view_calculation_required(Boolean flag = SGAL_TRUE)
  {
    m_current_view_calculation_required = flag;
  }

  /*!
   */
  Boolean is_current_view_calculation_required()
  {
    return m_current_view_calculation_required;
  }

  // Set and get functions for the active key sensor
  // (only one key sensor can be active in a given time)
  void set_active_key_sensor(Key_sensor * sensor)
  {
    m_active_key_sensor = sensor;
  }

  Key_sensor * get_active_key_sensor()
  {
    return m_active_key_sensor;
  }

  Boolean is_aa_interrupt();
  void set_frame_rate(float rate) { m_frame_rate = rate; }
  float get_frame_rate() const { return m_frame_rate; }
  void set_min_frame_rate(float rate) { m_min_frame_rate = rate; }
  float get_min_frame_rate() const { return m_min_frame_rate;}
  bool lower_level() { return m_min_frame_rate > m_frame_rate; }

  void set_flow_sensor(Flow_sensor * flow_sensor);
  void set_error_handle(App_error_handle_int * handle)
  { m_error_handle = handle; }
  void send_error_message(Error_type type, std::string & error);

  void add_comp_interpolator() 
  { 
    m_compressed_interpolators_count++; 
    m_is_update_time_active = SGAL_FALSE;            
  }
    
  void update_comp_interpolator(); 

  Boolean is_update_time_active() 
  {
    return m_is_update_time_active;
  }

  void set_scene_bounding_sphere_radius(Float radius)
  {
    m_scene_bounding_sphere_radius = radius;
  }

  Float get_scene_bounding_sphere_radius()
  {
    return m_scene_bounding_sphere_radius;
  }

  void inc_frame_counter() { m_frame_counter++; }
  int get_frame_counter() { return m_frame_counter; }

  void add_polygons(const int n);
  void add_memory(const int n);

private :
  /*! true if need to render the scene */
  Boolean m_rendering_required;

  /*! true if draggin is on */
  Boolean m_dragging_locked;

  /*! true if scope of cursor is on */
  Boolean m_scope_locked;

  /*! true if need to caclulate the bounding sphere of the scene */
  Boolean m_bounding_sphere_modified;

  /*! returns an index into the cursor array */
  Cursor_type m_current_cursor_id;

  /*! true if the cursor is not the default cursor */
  Boolean m_is_cursor_on;

  /*! true if the mouse is over a touch sensor */
  Boolean m_is_mouse_over;

  /*! true after all levels of all mesh objects have been loaded */
  Boolean m_is_loading_done;

  /*! true when anti aliasing should be interrupted */
  Boolean m_is_aa_interrupt;

  /*! the current key sensor */
  Key_sensor * m_active_key_sensor;

  /*! the last calculated frame rate */
  float m_frame_rate;

  /*! The minimum frame rate that below it we switch to low level rendering
   * in motion
   */
  float m_min_frame_rate;

  /*! SGAL_TRUE when the current view should be calculated */
  Boolean m_current_view_calculation_required;

  /*! a pointer to the flow sensor */
  Flow_sensor * m_flow_sensor;

  /*! the error handler */
  App_error_handle_int * m_error_handle;

  /*! the error handler */
  int m_compressed_interpolators_count;

  /*! Activation of time sensors - used by compressd interpolators */
  Boolean m_is_update_time_active;

  /*! Scene bounding sphere radius - used by interpolators encoders*/
  Float m_scene_bounding_sphere_radius;

  /*! Frame counter */
  Ulong m_frame_counter;
};

SGAL_END_NAMESPACE

#endif
