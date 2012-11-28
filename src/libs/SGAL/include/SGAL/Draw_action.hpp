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
// $Revision: 14220 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DRAW_ACTION_HPP
#define SGAL_DRAW_ACTION_HPP

/*! \file
 * Draw_action is a class used while traversing the scene graph for rendering.
 * It holds information about which pass is being rendered and an indication
 * whether more passes are required.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Configuration;

class SGAL_CLASSDEF Draw_action : public Isect_action {
public:
  /*! Constructor */
  Draw_action(Configuration* configuration = NULL); 

  /*! Destructor */
  virtual ~Draw_action();

  virtual Trav_directive apply(Node* node);

  Boolean is_second_pass_required();
  void set_second_pass_required(Boolean flag);

  void inc_pass_no();
  int get_pass_no();
  void reset_pass_no();
  Boolean is_first_pass();
  
  void set_current_lod( int lod ) { m_current_lod = lod; };
  int get_current_lod() const { return m_current_lod; };

  void set_current_wtm(const Matrix4f* wtm) { m_current_wtm = wtm; };
  const Matrix4f* get_current_wtm() const { return m_current_wtm; };
  
  void set_snap_from_front(Boolean flag) { m_snap_from_front = flag; }
  Boolean is_snap_from_front() const { return m_snap_from_front; }

  void set_clear(Uint mask) { m_clear = mask; }
  Uint get_clear() const { return m_clear; }

  void set_snap(Boolean flag) { m_snap = flag; }
  Boolean get_snap() const { return m_snap; }  

  void set_apply_camera(Boolean flag) { m_apply_camera = flag; }
  Boolean get_apply_camera() const { return m_apply_camera; }

  /*! Set the configuration container */
  void set_configuration(Configuration* conf) { m_configuration = conf; }

  /*! Obtain the configuration container */
  Configuration* get_configuration() const { return m_configuration; }
  
private:
  /*! Indicates which pass is being rendered, 0 being the first pass. */
  Int m_pass_no;

  /*! Indicates whether a second pass is required for transparent objects */
  Boolean m_second_pass_required;

  /*! Indicates current level of detail for shapes. */
  Int m_current_lod;

  /*! The world transformation matrix */
  const Matrix4f* m_current_wtm;
    
  /*! indicates for the snapshot from which buffer to take the image from */
  Boolean m_snap_from_front;

  /*! Indicates whether the frame buffers should be cleared */
  Uint m_clear;

  /*! Indicates whether to campture a snapshot */
  Boolean m_snap;

  /*! Indicates whether to apply the active camera */
  Boolean m_apply_camera;

  /*! Configuration \todo move to action? */
  Configuration* m_configuration;
};

SGAL_END_NAMESPACE

#endif
