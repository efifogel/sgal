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

#include "SGAL/basic.hpp"
#include "SGAL/Ortho_camera.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Ortho_camera::Ortho_camera() { init(); }

/*! Destructor */
Ortho_camera::~Ortho_camera() {}

/*!
 */
void Ortho_camera::init()
{
  m_width = 640.0f;
  m_height = 480.f;
  m_center.set(320.0f, 240.0f);
  m_near_clip = 0.0f;
  m_far_clip = 1.0f;
  m_aspect_mode = Frustum::CALC_VERT;
  m_aspect_ratio = 1.333333f;

  m_base_frust.set_near(m_near_clip);
  m_base_frust.set_far(m_far_clip);
  m_base_frust.make_ortho(0, m_width, 0, m_height);
}

/*!
 */
void Ortho_camera::set_width(Float width)
{
  m_width = width;
  float halm_width = width * 0.5f;
  m_base_frust.set_right(m_center[0] + halm_width);
  m_base_frust.set_left(m_center[0] - halm_width);
}

/*!
 */
void Ortho_camera::set_height(Float height)
{
  m_height = height;
  float halm_height = height * 0.5f;
  m_base_frust.SetTop(m_center[1] + halm_height);
  m_base_frust.set_bottom(m_center[1] - halm_height);
}

/*!
 */
void Ortho_camera::set_center(Float v0, Float v1)
{
  m_center.set(v0, v1);
  float halm_width = m_width * 0.5f;
  m_base_frust.set_right(v0 + halm_width);
  m_base_frust.set_left(v0 - halm_width);
  float halm_height = m_height * 0.5f;
  m_base_frust.set_top(v1 + halm_height);
  m_base_frust.set_bottom(v1 - halm_height);
}

/*!
 */
void Ortho_camera::set_near_clip(Float near_clip)
{
  m_near_clip = near_clip;
  m_base_frust.set_near(near_clip);
}

/*!
 */
void Ortho_camera::set_far_clip(Float far_clip)
{
  m_far_clip = far_clip;
  m_base_frust.set_far(far_clip);
}

/*!
 */
void Ortho_camera::set_aspect_mode(Frustum::Aspect_mode mode)
{
  m_aspect_mode = mode;
  m_base_frust.set_aspect_mode(mode);
}

/*!
 */
void Ortho_camera::set_aspect_ratio(Float aspect)
{
  m_aspect_ratio = aspect;
  m_base_frust.set_aspect_ratio(aspect);
}

SGAL_END_NAMESPACE
