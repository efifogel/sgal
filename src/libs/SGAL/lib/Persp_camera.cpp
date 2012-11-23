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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <assert.h>

#include "SGAL/basic.hpp"
#include "Types.h"
#include "Math_defs.h"
#include "Persp_camera.h"

using namespace SGAL;

/*! Constructor */
Persp_camera::Persp_camera() { init(); }

/*! Destructor */
Persp_camera::~Persp_camera() {}

/*!
 */
void Persp_camera::init()
{
  m_aspect_ratio = 1.333333f;
  m_horiz_fov = ENB_PI * 0.25f;
  m_vert_fov = m_horizFOV / m_aspect_ratio;
  m_horiz_fov_off = 0.0f;
  m_vert_fov_off = 0.0f;
  m_near_clip = 0.1f;
  m_far_clip = 32768.0f;
  m_aspect_mode = Frustum::CALC_VERT;

  m_base_frust.set_near(m_near_clip);
  m_base_frust.SetFar(m_far_clip);
  m_base_frust.set_aspect_ratio(m_aspect_ratio);
  m_base_frust.set_horiz_fov(m_horiz_fov);
  m_base_frust.set_aspect_mode(m_aspect_mode);
}

/*!
 */
void Persp_camera::set_horiz_fov(Float horiz_fov)
{
  m_horiz_fov = horiz_fov;
  m_base_frust.set_horiz_fov(horiz_fov);
}

/*!
 */
void Persp_camera::set_vert_fov(Float vert_fov)
{
  m_vert_fov = vert_fov;
  m_base_frust.set_vert_fov(vert_fov);
}

/*!
 */
void Persp_camera::set_horiz_fov_off(Float horiz_fov_off)
{
  assert(0);
  m_horiz_fov_off = horiz_fov_off;
}

/*!
 */
void Persp_camera::set_vert_fov_off(Float vert_fov_off)
{
  assert(0);
  m_vert_fov_off = vert_fov_off;
}

/*!
 */
void Persp_camera::set_near_clip(Float near_clip)
{
  m_near_clip = near_clip;
  m_base_frust.set_near(near_clip);
}

/*!
 */
void Persp_camera::set_far_clip(Float far_clip)
{
  m_far_clip = far_clip;
  m_base_frust.set_far(far_clip);
}

/*!
 */
void Persp_camera::set_aspect_mode(Frustum::Aspect_mode mode)
{
  m_aspect_mode = mode;
  m_base_frust.set_aspect_mode(mode);
}

/*!
 */
void Persp_camera::set_aspect_ratio(float aspect)
{
  m_aspect_ratio = aspect;
  m_base_frust.set_aspect_ratio(aspect);
}

