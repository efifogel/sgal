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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <algorithm>
#include <assert.h>

#include "SGAL/basic.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Text.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Point_light.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

/*! Constructor */
Cull_context::Cull_context() :
  m_camera(0),
  m_head_light(0),
  m_current_lod(-1),
  m_sort(false)
{}

/*! Destructor */
Cull_context::~Cull_context() {}

/*! \brief Compare node priority.
 * Used to sort nodes by their distance fromt he camera.
 */
inline bool compare_render_nodes(const Cull_context::Render_node& n1,
                                 const Cull_context::Render_node& n2)
{ return n1.priority > n2.priority; }

/*! \brief adds a shape node. */
void Cull_context::add_shape(Shape* node)
{
  Render_node rn;
  rn.node = node;
  rn.wtm = m_world_tm;
  rn.lod = m_current_lod;
  rn.priority = node->get_priority();
  if (rn.priority > 0) m_sort = true;
  m_nodes.push_back(rn);
}

/*! \brief adds a light node. */
void Cull_context::add_light(Light* light)
{
  if (light != m_head_light) {
    Light_node ln;
    ln.light = light;
    ln.wtm = m_world_tm;
    m_lights.push_back(ln);
  }
}

/*! \brief traverses node hierarchy and adds each node to the appropriate list.
 */
void Cull_context::cull(Node* node, Camera* camera)
{
  m_sort = false;
  m_nodes.clear();
  m_lights.clear();
  m_camera = camera;
  if (!m_camera) return;
  // m_camera->get_view_mat(m_view_tm);
  node->cull(*this);
}

/*! \brief draws a single node. */
void Cull_context::draw_node(Draw_action* draw_action, const Render_node& rn)
{
  glPushMatrix();
  glMultMatrixf((float*)&(rn.wtm));
  draw_action->set_current_lod(rn.lod);
  draw_action->set_current_wtm(&(rn.wtm));
  rn.node->draw(draw_action);
  glPopMatrix();
}

/*! \brief */
float Cull_context::compute_distance(const Cull_context::Render_node& rn)
{
  // Get world position of node.
  const Sphere_bound& sphere_bound = rn.node->get_sphere_bound();

  // the center of the sphere bound in world coordinate system
  Vector3f w_center; 
  w_center.xform_pt(sphere_bound.get_center(), rn.wtm);

  // Extract the scale factor from the matrix:
  //! \todo Elliminate the calculation of the scale, as the scale
  // already resides in the original transform object.
  Vector3f scale;
        
  Vector3f ustar;
  rn.wtm.get_row(0, ustar);
  scale[0] = ustar.length();
  SGAL_assertion(scale[0]);
        
  ustar.scale(1.0f / scale[0], ustar);
      
  // v* = (v - (v . u*)u*)/|v - (v . u*)u*|
  Vector3f v;
  rn.wtm.get_row(1, v);
  Vector3f tmp;
  tmp.scale(v.dot(ustar), ustar);
  Vector3f vstar;
  vstar.sub(v, tmp);
  scale[1] = vstar.length();
  SGAL_assertion(scale[1]);
  vstar.scale(1.0f / scale[1], vstar);

  // w* =(w - (w . u*)u* - (w . v*)v*) / |(w - (w . u*)u* - (w . v*)v*)|
  Vector3f w;
  rn.wtm.get_row(2, w);
  Vector3f tmp0;
  tmp0.scale(w.dot(ustar), ustar);
  Vector3f tmp1;
  tmp1.scale(w.dot(vstar), vstar);
  Vector3f wstar;
  tmp.add(tmp0, tmp1);
  wstar.sub(w, tmp);
  scale[2] = wstar.length();
  SGAL_assertion(scale[2]);
 
  float factor = scale.get_max_comp();
  float radius = factor * sphere_bound.get_radius();
        
  // calculate the distance between the camera and the center ofthe object
  Vector3f p;
  p.sub(w_center, m_camera->get_position());
  return (p.length() - radius);
}

/*! \brief draws. */
void Cull_context::draw(Draw_action* draw_action)
{
  glMatrixMode(GL_MODELVIEW);

  // Draw head light outside of view transform.
  if (m_head_light) {
    glPushMatrix();
    glLoadIdentity();
    m_head_light->draw(draw_action);
    glPopMatrix();
  }
  
  // Draw lights.
  for (Light_iter lit = m_lights.begin(); lit != m_lights.end(); ++lit) {
    Light_node& ln = *lit;
    glPushMatrix();
    glMultMatrixf((GLfloat*)&(ln.wtm));
    ln.light->draw(draw_action);
    glPopMatrix();
  }

  m_2ndpass.clear();
  draw_action->set_second_pass_required(false);

  if (m_sort)
    std::sort(m_nodes.begin(), m_nodes.end(), compare_render_nodes);
  
  // Draw nodes in first pass.
  for (Render_node_iter nit = m_nodes.begin(); nit != m_nodes.end(); ++nit) {
    Render_node& rn = *nit;
    draw_node(draw_action, rn);
    if (draw_action->is_second_pass_required()) {
      draw_action->set_second_pass_required(false);

      // This node must be rendered in second pass.
      m_2ndpass.push_back(rn);

      //! \todo Should Text nodes be assigned the highest priority?
      if (rn.priority == 0) rn.priority = compute_distance(rn);      
    }
  }

  // Draw nodes in 2nd pass
  if (!m_2ndpass.empty()) {
    // Sort nodes in 2nd path by distance from camera.
    std::sort(m_2ndpass.begin(), m_2ndpass.end(), compare_render_nodes);

    draw_action->inc_pass_no();

    // Draw nodes in second pass.
    for (Render_node_iter nit = m_2ndpass.begin();
         nit != m_2ndpass.end(); ++nit) 
    {
      Render_node& rn = *nit;
      draw_node(draw_action, rn);
    }
  }
}

/*! \brief pushes the transform matrix. */
void Cull_context::push_matrix(const Matrix4f& mat)
{
  m_matrix_stack.push_back(m_world_tm);

  Matrix4f wtm;
  if (!m_matrix_stack.empty()) {
    wtm = m_matrix_stack.back();
  }
  m_world_tm.mult(wtm, mat);
}
  
/*! \brief pops the transform matrix. */
void Cull_context::pop_matrix()
{
  SGAL_assertion((!m_matrix_stack.empty()));
  m_world_tm = m_matrix_stack.back();
  m_matrix_stack.pop_back();
}

/*! \brief sets the head light. */
void Cull_context::set_head_light(Light* light) { m_head_light = light; }

SGAL_END_NAMESPACE
