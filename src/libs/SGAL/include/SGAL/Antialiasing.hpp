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

#ifndef SGAL_ANTIALIASING_HPP
#define SGAL_ANTIALIASING_HPP

/*! \file
 * An implementation of the full scene anti-aliasing.
 * Use an OpenGL accomulation buffer and jitter.h for the jittering process.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class Frustum;
class Draw_action;
//! \todo class Execution_coordinator;
//! \todo class Time_check;

class SGAL_SGAL_DECL Antialiasing {
public:
  /*! Constructor */
  Antialiasing();

  /*! Destructor */
  virtual ~Antialiasing();

  enum Antialias_quality { aqHigh, aqLow };

  void set_delay_duration(int dur);
  Boolean is_in_process();
  void pre_render(Draw_action * draw_action);
  void post_render();
  void reset_delay_time();
  void set_in_process(Boolean in_process) { m_is_in_process = in_process; }
  Boolean is_render();
  void set_quality(Antialias_quality q);
  //! \todo void set_ec(Execution_coordinator * ec) { m_ec = ec; }

private:
  /*! the max number of levels used for antialiasing */
  int m_aalias_quality;

  /*! the current accumulate factor used */
  float m_accumulate;

  /*! the current pass number in the anti aliasing (0-m_AAliasQuality) */
  int m_pass_no;

  /*! SGAL_TRUE if anti aliasing calculations are in progress. */
  Boolean m_is_in_process;

  /*! the starting time of the delay */
  long int m_delay_start_time;

  /*! SGAL_TRUE if thr anti aliasing process is delayed */
  Boolean m_is_in_delay;

  /*! the delay duration before anti aliasing starts (in ms) */
  int m_delay_duration;

  /*! a timer to get current time */
  //! \todo Time_check * m_timer;

  /*! a pointer to the execution coordinator. Used to check for interrupts in
   * the anti aliasing
   */
  //! \todo Execution_coordinator * m_ec;

  /*! true after a call to glAccum(GL_RENDER....) */
  Boolean m_render;

  void acc_frustum(double left, double right,
                   double bottom, double top,
                   double zNear, double zFar,
                   double pixdx, double pixdy,
                   double eyedx, double eyedy, double focus);

  void acc_perspective(double fovy, double aspect, 
                       double zNear, double zFar,
                       double pixdx, double pixdy, 
                       double eyedx, double eyedy, double focus);
};

SGAL_END_NAMESPACE

#endif
