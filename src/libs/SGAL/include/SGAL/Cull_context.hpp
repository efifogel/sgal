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

#ifndef SGAL_CULL_CONTEXT_HPP
#define SGAL_CULL_CONTEXT_HPP

/*! \file
 * Cull_context is a class used to traverse the scene graph before rendering
 * and gather visible nodes, and then rendering them in specified order.
 */

#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Node;
class Camera;
class Shape;
class Light;
class Draw_action;

class SGAL_SGAL_DECL Cull_context {
public:
  class Render_node {
  public:
    Shape* node;
    float priority;
    Matrix4f wtm;       // Object to World space transformation.
    int lod;            // Level of detail for node.
  };

  class Light_node {
  public:
    Light* light;
    Matrix4f wtm;       // Object to World space transformation.
  };

  typedef boost::shared_ptr<Light>            Shared_light;

  /*! Constructor */
  Cull_context();

  /*! Destructor */
  virtual ~Cull_context();

  /*! Traverse node hierarchy seacrhing for transform nodes and adding
   * them to draw list.
   * \param node a pointer to the node to draw
   */
  virtual void cull(Node* node, Camera* camera);

  virtual void draw(Draw_action* draw_action);

  void add_shape(Shape* shape);

  /*! 
   * \param light a pointer to the Light.
   */
  void add_light(Light* light);

  /*! */
  void set_head_light(Shared_light light);
  
  void push_matrix(const Matrix4f& mat);

  void pop_matrix();

  Camera* get_camera() const { return m_camera; };

  const Matrix4f& get_current_wtm() { return m_world_tm; }

  void set_current_lod(int lod) { m_current_lod = lod; };

  Int get_current_lod() const { return m_current_lod; };

protected:
  void draw_node(Draw_action* draw_action, const Render_node& rn);

  typedef std::vector<Render_node>      Render_node_vector;
  typedef Render_node_vector::iterator  Render_node_iter;
  
  typedef std::vector<Light_node>       Light_vector;
  typedef Light_vector::iterator        Light_iter;

  typedef std::vector<Matrix4f>         Matrix_stack;

private:
  float compute_distance(const Cull_context::Render_node& rn);
  
  Render_node_vector m_nodes;

  Render_node_vector m_2ndpass;

  Light_vector m_lights;

  Camera* m_camera;

  Shared_light m_head_light;

  // Matrix stack (similar to opengl).
  Matrix_stack m_matrix_stack;
  
  // Current world to view transformation matrix (Camera matrix).
  // Matrix4f m_viewTM;

  // Current object to world transformation matrix.
  Matrix4f m_world_tm;

  int m_current_lod;

  /*! A flag indicating whether sort is needed */
  Boolean m_sort;
};

SGAL_END_NAMESPACE

#endif
