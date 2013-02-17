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
// $Revision: 14220 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <iterator>
#include <stdio.h>
#include <assert.h>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Plane.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! The frustum types (prespective, orthogonal, etc) */
const char* Frustum::s_type_strings[] = {"SIMPLE", "ORTHOGONAL", "PERSPECTIVE"};

/*! Constructor */
Frustum::Frustum() :
  m_type(Frustum::SIMPLE),
  m_near_dist(0.1f), 
  m_far_dist(1000.0f),
  m_left(-30.0f), 
  m_right(30.0f), 
  m_top(22.5f), 
  m_bottom(-22.5f),
  m_horiz_fov(m_vert_fov / m_aspect_ratio), 
  m_vert_fov(0.785398f),        //! \todo SGAL_PI * 0.25f), 
  m_xcenter(0.0f), 
  m_ycenter(0.0f), 
  m_aspect_mode(Frustum::CALC_HORIZ),
  m_aspect_ratio(1.333333f), 
  m_dirty_corners(true),
  m_dirty_planes(true),
  m_x_perturbation_scale(0), m_y_perturbation_scale(0)
{}

/*! Destructor */
Frustum::~Frustum() {}

/*! \brief clones. */
void Frustum::copy(const Frustum* src)
{
  m_type = src->m_type;
  m_near_dist = src->m_near_dist;
  m_far_dist = src->m_far_dist;
  m_xcenter = src->m_xcenter;
  m_ycenter = src->m_ycenter;
  m_aspect_mode = src->m_aspect_mode;
  m_aspect_ratio = src->m_aspect_ratio;
  m_horiz_fov = src->m_horiz_fov;
  m_vert_fov = src->m_vert_fov;
  
  int i;
  if (!m_dirty_corners) {
    for (i = 0; i < Frustum::NUM_CORNERS; i++) {
      m_corners[i] = src->m_corners[i];
    }
  }

  if (!m_dirty_planes) {
    for (i = 0; i < Frustum::NUM_PLANES; i++) {
      m_facets[i] = src->m_facets[i];
    }
  }
}

/*! \brief sets the distance from the origin to the near plane. */
void Frustum::set_near(Float near_dist)
{
  if (m_near_dist == near_dist) return;
  m_near_dist = near_dist;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief sets the distance from the origin to the far plane. */
void Frustum::set_far(Float far_dist)
{
  if (m_far_dist == far_dist) return;
  m_far_dist = far_dist;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief obtains the distances from the origin to the near plane and to
 * the far plane.
 */
void Frustum::get_near_far(Float& near_dist, Float& far_dist)
{
  if (m_dirty_corners) clean_corners(); 
  near_dist = m_near_dist; 
  far_dist = m_far_dist; 
}

/*! \brief sets the aspect recalculation mode. */
void Frustum::set_aspect_mode(Frustum::Aspect_mode mode)
{
  if (m_aspect_mode == mode)
    return;
  m_aspect_mode = mode;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief sets the aspect ratio of the frustum dimensions. */
void Frustum::set_aspect_ratio(float ratio)
{
  if (m_aspect_ratio == ratio) return;
  m_aspect_ratio = ratio;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_fov(float fov)
{
  set_vert_fov(fov);
  set_horiz_fov(fov * m_aspect_ratio);
//  set_horiz_fov(fov);
//  set_vert_fov(fov / m_aspect_ratio);
}

/*! \brief */
Float Frustum::get_fov()
{
  // FIX - what to do here?
  return m_horiz_fov;
}

/*! \brief */
void Frustum::set_horiz_fov(Float horiz_fov)
{
  if (m_horiz_fov == horiz_fov) return;
  m_horiz_fov = horiz_fov;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_vert_fov(Float vert_fov)
{
  if (m_vert_fov == vert_fov) return;
  m_vert_fov = vert_fov;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_right(Float right)
{
  m_right = right;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_left(Float left)
{
  m_left = left;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_top(Float top)
{
  m_top = top;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief */
void Frustum::set_bottom(Float bottom)
{
  m_bottom = bottom;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief makes this frustum orthogonal. */
void Frustum::make_ortho(float left, float right, float bottom, float top)
{
  m_left = left;
  m_right = right;
  m_top = top;
  m_bottom = bottom;
  m_type = Frustum::ORTHOGONAL;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief makes this frustum orthogonal. */
void Frustum::make_ortho(Float left, Float right, Float bottom, Float top,
                         Float near_dist, Float far_dist)
{
  m_left = left;
  m_right = right;
  m_top = top;
  m_bottom = bottom;
  m_near_dist = near_dist;
  m_far_dist = far_dist;
  m_type = Frustum::ORTHOGONAL;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief makes this frustum perspective. */
void Frustum::make_persp(Float left, Float right, Float bottom, Float top)
{
  m_left = left;
  m_right = right;
  m_top = top;
  m_bottom = bottom;
  m_type = Frustum::PERSPECTIVE;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief makes this frustum perspective. */
void Frustum::make_persp(Float left, Float right, Float bottom, Float top,
                         Float near_dist, Float far_dist)
{
  m_left = left;
  m_right = right;
  m_top = top;
  m_bottom = bottom;
  m_near_dist = near_dist;
  m_far_dist = far_dist;
  m_type = Frustum::PERSPECTIVE;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief gets the corners of the near plane. */
void Frustum::get_near(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur)
{
  if (m_dirty_corners) clean_corners(); 
  ll = m_corners[Frustum::NEAR_LL];
  lr = m_corners[Frustum::NEAR_LR];
  ul = m_corners[Frustum::NEAR_UL];
  ur = m_corners[Frustum::NEAR_UR];
}

/*! \brief gets the corners of the far plane. */
void Frustum::get_far(Vector3f& ll, Vector3f& lr, Vector3f& ul, Vector3f& ur)
{
  if (m_dirty_corners) clean_corners(); 
  ll = m_corners[Frustum::FAR_LL];
  lr = m_corners[Frustum::FAR_LR];
  ul = m_corners[Frustum::FAR_UL];
  ur = m_corners[Frustum::FAR_UR];
}

/*! \brief obtains the six coordinates defining the two diagonal corners of
 * the frustum. The two corners are (left, bottom, near) and (right, top, far).
 */
void Frustum::get_diag_corners(float& left, float& right, float& bottom,
                               float& top, float& near_dist, float& far_dist)
{
  if (m_dirty_corners) clean_corners(); 
  left = m_corners[Frustum::NEAR_LL][0];
  bottom = m_corners[Frustum::NEAR_LL][1];
  near_dist = m_corners[Frustum::NEAR_LL][2];

  right = m_corners[Frustum::FAR_UR][0];
  top = m_corners[Frustum::FAR_UR][1];
  far_dist = m_corners[Frustum::FAR_UR][2];
}

/*! \brief calculates a matrix suitable for use as the GL_PROJECTION matrix
 * in OpenGL, projecting the contents of the frustum onto the near plane.
 */
void Frustum::get_gl_proj_mat(Matrix4f& mat)
{
  if (m_dirty_corners) clean_corners(); 
  float left = m_corners[Frustum::NEAR_LL][0];
  float right = m_corners[Frustum::NEAR_UR][0];
  float bottom = m_corners[Frustum::NEAR_LL][1];
  float top = m_corners[Frustum::NEAR_UR][1];
      
  mat[0][0] = 2.0f * m_near_dist / (right - left);
  mat[0][1] = 0.0f;
  mat[0][2] = (right + left) / (right - left);
  mat[0][3] = 0.0f;

  mat[1][0] = 0.0f;
  mat[1][1] = 2.0f * m_near_dist / (top - bottom);
  mat[1][2] = (top + bottom) / (top - bottom);
  mat[1][3] = 0.0f;

  mat[2][0] = 0.0f;
  mat[2][1] = 0.0f;
  mat[2][2] = -(m_far_dist + m_near_dist) / (m_far_dist - m_near_dist);
  mat[2][3] = -2.0f * m_far_dist * m_near_dist / (m_far_dist - m_near_dist);
      
  mat[3][0] = 0.0f;
  mat[3][1] = 0.0f;
  mat[3][2] = -1.0f;
  mat[3][3] = 0.0f;
}

/*! \brief */
void Frustum::get_corners(float& left, float& right,
                          float& bottom, float& top,
                          float& near_dist, float& far_dist)
{
  if (m_dirty_corners) clean_corners(); 
  left = m_corners[Frustum::NEAR_LL][0];
  right = m_corners[Frustum::NEAR_LR][0];
  bottom = m_corners[Frustum::NEAR_LL][1];
  top = m_corners[Frustum::NEAR_UL][1];
  near_dist = m_near_dist;
  far_dist  = m_far_dist;
}

/*! \brief */
Plane* Frustum::get_facets() 
{
  if (m_dirty_planes) clean_planes();
  return m_facets;
}

/*! \brief */
void Frustum::clean_corners() 
{
  /* in case the near and far clipping planes are the same, nothing is to be
   * rendered. We shift the far plane a bit to avoid invalid values in OpenGL.
   */
  if (m_near_dist == m_far_dist)
    m_far_dist += 1.0;
  float left = 0, right = 0, top = 0, bottom = 0;

  switch(m_aspect_mode) {
   case Frustum::CALC_HORIZ:
    top = tanf(0.5f * m_vert_fov) * m_near_dist;
    right = top * m_aspect_ratio;
    left = -right;
    bottom = -top;
    break;

   case Frustum::CALC_VERT:
    right = tanf(0.5f * m_horiz_fov) * m_near_dist;
    top = right / m_aspect_ratio;
    left = -right;
    bottom = -top;
    break;

#if 0
    //! \todo Implement this option
   case Frustum::CALC_????:
    top = tanf(0.5f * m_vert_fov) * m_near_dist;
    right = tanf(0.5f * m_horiz_fov) * m_near_dist;
    left = -right;
    bottom = -top;
    break;
#endif
    
   case Frustum::CALC_NONE:
    left = m_left;
    right = m_right;
    top = m_top;
    bottom = m_bottom;
    break;
    
   default: fprintf(stderr, "Illegal aspect mode %d", m_aspect_mode);
  }
  
  m_corners[Frustum::NEAR_LL][0] = left;
  m_corners[Frustum::NEAR_LL][1] = bottom;
  m_corners[Frustum::NEAR_LL][2] = -m_near_dist;

  m_corners[Frustum::NEAR_LR][0] = right;
  m_corners[Frustum::NEAR_LR][1] = bottom;
  m_corners[Frustum::NEAR_LR][2] = -m_near_dist;

  m_corners[Frustum::NEAR_UL][0] = left;
  m_corners[Frustum::NEAR_UL][1] = top;
  m_corners[Frustum::NEAR_UL][2] = -m_near_dist;
        
  m_corners[Frustum::NEAR_UR][0] = right;
  m_corners[Frustum::NEAR_UR][1] = top;
  m_corners[Frustum::NEAR_UR][2] = -m_near_dist;

  // Set far corners:
  if (m_type != Frustum::ORTHOGONAL) {
    float ratio = m_far_dist / m_near_dist;
    left *= ratio;
    right *= ratio;
    top *= ratio;
    bottom *= ratio;
  }

  // -m_far_dist specifies the location of the far clipping plane.
  m_corners[Frustum::FAR_LL][0] = left;
  m_corners[Frustum::FAR_LL][1] = bottom;
  m_corners[Frustum::FAR_LL][2] = -m_far_dist;
        
  m_corners[Frustum::FAR_LR][0] = right;
  m_corners[Frustum::FAR_LR][1] = bottom;
  m_corners[Frustum::FAR_LR][2] = -m_far_dist;
        
  m_corners[Frustum::FAR_UL][0] = left;
  m_corners[Frustum::FAR_UL][1] = top;
  m_corners[Frustum::FAR_UL][2] = -m_far_dist;
        
  m_corners[Frustum::FAR_UR][0] = right;
  m_corners[Frustum::FAR_UR][1] = top;
  m_corners[Frustum::FAR_UR][2] = -m_far_dist;

  m_dirty_corners = false;
}

/*! \brief */
Uint Frustum::contains(const Vector3f& /* pt */) const
{
  assert(0);
  return 0;
}

/*! \brief */
Uint Frustum::contains(const Sphere_bound* /* sphere */) const
{
  assert(0);
  return 0;
}

/*! \brief */
Uint Frustum::contains(const Box_bound* /* box */) const
{
  assert(0);
  return 0;
}

/*! \brief */
void Frustum::apply()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (m_dirty_corners) clean_corners();

  float xdim = m_corners[Frustum::NEAR_UR][0] - m_corners[Frustum::NEAR_LL][0];
  float ydim = m_corners[Frustum::NEAR_UR][1] - m_corners[Frustum::NEAR_LL][1];
  float dx = xdim * m_x_perturbation_scale;
  float dy = ydim * m_y_perturbation_scale;
  if (m_type == Frustum::ORTHOGONAL) {
    glOrtho(m_corners[Frustum::NEAR_LL][0] + dx,
            m_corners[Frustum::NEAR_UR][0] + dx,
            m_corners[Frustum::NEAR_LL][1] + dy,
            m_corners[Frustum::NEAR_UR][1] + dy,
            m_near_dist, m_far_dist);
  } else {
    glFrustum(m_corners[Frustum::NEAR_LL][0] + dx,
              m_corners[Frustum::NEAR_UR][0] + dx,
              m_corners[Frustum::NEAR_LL][1] + dy,
              m_corners[Frustum::NEAR_UR][1] + dy,
              m_near_dist, m_far_dist);
  }
}

/*! \brief */
void Frustum::clean_planes()
{
  if (m_dirty_corners) clean_corners(); 

  Vector3f origin;
  origin.set(0.0f, 0.0f, 0.0f);

  // EFEF: The order looks flipped.
  if (m_type == Frustum::ORTHOGONAL) {
    m_facets[Frustum::LEFT_PLANE].make_pts(m_corners[Frustum::NEAR_LL],
                                           m_corners[Frustum::FAR_LL],
                                           m_corners[Frustum::FAR_UL]);
    m_facets[Frustum::RIGHT_PLANE].make_pts(m_corners[Frustum::NEAR_UR],
                                            m_corners[Frustum::FAR_UR],
                                            m_corners[Frustum::FAR_LR]);
    m_facets[Frustum::NEAR_PLANE].make_pts(m_corners[Frustum::NEAR_LL],
                                           m_corners[Frustum::NEAR_UL],
                                          m_corners[Frustum::NEAR_UR]);
    m_facets[Frustum::BOTTOM_PLANE].make_pts(m_corners[Frustum::NEAR_LR],
                                             m_corners[Frustum::FAR_LR],
                                             m_corners[Frustum::FAR_LL]);
    m_facets[Frustum::TOP_PLANE].make_pts(m_corners[Frustum::NEAR_UL],
                                          m_corners[Frustum::FAR_UL],
                                          m_corners[Frustum::FAR_UR]);
    m_facets[Frustum::FAR_PLANE].make_pts(m_corners[Frustum::FAR_UR],
                                          m_corners[Frustum::FAR_UL],
                                          m_corners[Frustum::FAR_LL]);
  } else {
    m_facets[Frustum::LEFT_PLANE].make_pts(origin,
                                           m_corners[Frustum::NEAR_LL],
                                           m_corners[Frustum::NEAR_UL]);
    m_facets[Frustum::RIGHT_PLANE].make_pts(origin,
                                            m_corners[Frustum::NEAR_UR],
                                            m_corners[Frustum::NEAR_LR]);
    m_facets[Frustum::NEAR_PLANE].make_pts(m_corners[Frustum::NEAR_LL],
                                           m_corners[Frustum::NEAR_UL],
                                           m_corners[Frustum::NEAR_UR]);
    m_facets[Frustum::BOTTOM_PLANE].make_pts(origin,
                                             m_corners[Frustum::NEAR_LR],
                                             m_corners[Frustum::NEAR_LL]);
    m_facets[Frustum::TOP_PLANE].make_pts(origin,
                                          m_corners[Frustum::NEAR_UL],
                                          m_corners[Frustum::NEAR_UR]);
      
    m_facets[Frustum::FAR_PLANE].make_pts(m_corners[Frustum::FAR_UR],
                                          m_corners[Frustum::FAR_UL],
                                          m_corners[Frustum::FAR_LL]);
  }

  // std::cout << "near: normal: " << m_facets[Frustum::NEAR_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::NEAR_PLANE].get_offset()
  //           << std::endl;
  // std::cout << "far: normal: " << m_facets[Frustum::FAR_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::FAR_PLANE].get_offset()
  //           << std::endl;
  // std::cout << "left: normal: " << m_facets[Frustum::LEFT_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::LEFT_PLANE].get_offset()
  //           << std::endl;
  // std::cout << "right: normal: " << m_facets[Frustum::RIGHT_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::RIGHT_PLANE].get_offset()
  //           << std::endl;
  // std::cout << "top: normal: " << m_facets[Frustum::TOP_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::TOP_PLANE].get_offset()
  //           << std::endl;
  // std::cout << "bottom: normal: " << m_facets[Frustum::BOTTOM_PLANE].get_normal()
  //           << ", offset: " << m_facets[Frustum::BOTTOM_PLANE].get_offset()
  //           << std::endl;
  m_dirty_planes = false;
}

/*! \brief sets the frustum type (orthogonal, perspective, etc). */
void Frustum::set_type(Frustum_type type)
{
  m_type = type;
  m_dirty_corners = true;
  m_dirty_planes = true;
}

/*! \brief sets the attributes of this object. */
void Frustum::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "type") {
      Uint num = sizeof(s_type_strings) / sizeof(char*);
      const char** found = std::find(s_type_strings, &s_type_strings[num],
                                     strip_double_quotes(value));
      Uint index = found - s_type_strings;
      if (index < num)
        m_type = static_cast<Frustum_type>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "left") {
      set_left(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "right") {
      set_right(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "top") {
      set_top(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bottom") {
      set_bottom(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "near") {
      set_near(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "far") {
      set_far(atoff(value.c_str()));
      m_aspect_mode = CALC_NONE;
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the deleted attributes:
  elem->delete_marked();
}

SGAL_END_NAMESPACE
