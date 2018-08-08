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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "Antialiasing.h"
#include "jitter.h"
#include "Camera.h"
#include "Draw_action.h"
#include "GL_error.h"
//! \todo #include "Time_check.h"

using namespace SGAL;

/*! Constructor */
Antialiasing::Antialiasing() :
  m_aalias_quality(8),
  m_accumulate(0),
  m_pass_no(0),
  m_is_in_process(false),
  m_delay_start_time(0.0f),
  m_is_in_delay(false),
  m_delay_duration(800),
  //! \todo m_ec(0),
  m_render(false)
{
  //! \todo m_timer = new Time_check(40);
}

/*! Destructor */
Antialiasing::~Antialiasing()
{
  TRACE_MSG(Tracer::DESTRUCTOR, "~Antialiasing ...");
  //! \todo delete m_timer;
  TRACE_MSG(Tracer::DESTRUCTOR, " completed\n");
}

void Antialiasing::set_delay_duration(Int dur)
{
  m_delay_duration = dur;
}

/*! Returns true if AA is in process, i.e., another pass of the
 * AA should be rendered.
 */
Boolean Antialiasing::is_in_process()
{
  /*! \todo
  if (!m_ec || !m_timer) {
    //ASSERT(0);
    return false;
  }

  if (!m_ec->is_loading_done()) {
    m_is_in_process = false;
    m_is_in_delay = false;
    return m_is_in_process;
  }

  if (m_ec->is_aa_interrupt() && m_is_in_process) {
    m_is_in_process = false;

  } else if (m_is_in_delay) {
    long int dt = m_timer->get_time() - m_delay_start_time;
    if (dt > m_delay_duration) {
      m_is_in_process = true;
      m_pass_no = 0;
      m_is_in_delay = false;
    } else {
      m_is_in_process = false;
    }
  } else if (m_pass_no == m_aalias_quality) {
    m_is_in_process = false;

  } else {
    m_is_in_process = true;
  }
  */

  return m_is_in_process;
}

/*! Prepering the accumulation buffer for anti aliasing.
 * Should be called befor the render call.
 */
void Antialiasing::pre_render(Draw_action * draw_action)
{
  if (!draw_action)
    return;

  if (m_pass_no == 0) {
    m_accumulate = 0.0;
    glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_ACCUM_BUFFER_BIT);
  }

  if (m_pass_no < m_aalias_quality) {
    Context *context = draw_action->get_context();
    if (!context)
      return;
    Camera *camera = context->get_active_camera();
    if (!camera)
      return;
    Frustum frust = camera->get_frustum();
    float nearClip = 0;
    float farClip = 0;
    float left = 0;
    float right = 0;
    float bottom = 0;
    float top = 0;

    frust.get_corners(left, right, bottom, top, nearClip,farClip);
    acc_frustum(left, right, bottom, top, nearClip, farClip,
                j8[m_pass_no].x, j8[m_pass_no].y, 0.0f, 0.0f, 1.0f);

  }
}

/**
  This is called after the scene has been rendered.
  It takes what was rendered, adds it to the accumulation
  buffer, and then dups it into the back buffer.
  This way we get progressive anti aliasing.
  */

void Antialiasing::post_render()
{
  if (m_aalias_quality == 0)
    return;

  bool blendMode = (glIsEnabled(GL_BLEND) == GL_TRUE);
  glDisable(GL_BLEND);

  m_accumulate += (1.0f / (float)m_aalias_quality);
  glAccum(GL_ACCUM, (1.0f / (float)m_aalias_quality));
  if (m_pass_no == m_aalias_quality/2-1 ||
    m_pass_no == m_aalias_quality-1)
  {
    glAccum(GL_RETURN, 1.0f / m_accumulate);
    m_render = true;
  } else {
    m_render = false;
  }
  if (blendMode)
    glEnable(GL_BLEND);
  m_pass_no++;
}

/*! Resets the delay start time to the current time.
 * Also sets the flag of the delay to be true.
 */
void Antialiasing::reset_delay_time()
{
  /*! \todo
  if (m_timer) {
    m_delay_start_time = m_timer->get_time();
    m_is_in_delay = true;
  }
  */
}

/*! The frame buffer should be updated twice onle. Once in the 4th pass (
 * assuming we are using 8 passes for anti aliasing), and once in the last
 * pass. This method returns true when we are doing one of these two passes.
 * Otherwise, it returns false.
 */
Boolean Antialiasing::is_render()
{
  return m_render;
}

void Antialiasing::set_quality(Antialiasing::Antialias_quality q)
{
  if (q == aqLow) {
    m_aalias_quality = 4;
  } else {
    m_aalias_quality = 8;
  }
}

/**
 * The first 6 arguments are identical to the glFrustum() call.
 *
 * pixdx and pixdy are anti-alias jitter in pixels.
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels.
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus.
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accFrustum() calls glTranslatef().  You will
 * probably want to insure that your ModelView matrix has been
 * initialized to identity before calling accFrustum().
 */
void Antialiasing::acc_frustum(double left, double right,
                               double bottom, double top,
                               double zNear, double zFar,
                               double pixdx, double pixdy,
                               double eyedx, double eyedy, double focus)
{
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   double xwsize = right - left;
   double ywsize = top - bottom;

   double dx = -(pixdx*xwsize/(double) viewport[2] + eyedx*zNear/focus);
   double dy = -(pixdy*ywsize/(double) viewport[3] + eyedy*zNear/focus);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(left + dx, right + dx, bottom + dy, top + dy, zNear, zFar);
   glMatrixMode(GL_MODELVIEW);
}

/*! The first 4 arguments are identical to the gluPerspective() call.
 * pixdx and pixdy are anti-alias jitter in pixels.
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels.
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus.
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accPerspective() calls accFrustum().
 */
void Antialiasing::acc_perspective(double fovy, double aspect,
                                   double zNear, double zFar,
                                   double pixdx, double pixdy,
                                   double eyedx, double eyedy,
                                   double focus)
{
   double halfFov, left, right, bottom, top;

   halfFov = fovy/2.0f;
   right = zNear * tan(halfFov);
   left = -right;
   top = right / aspect;
   bottom = -top;

   acc_frustum(left, right, bottom, top, zNear, zFar,
               pixdx, pixdy, eyedx, eyedy, focus);
}
