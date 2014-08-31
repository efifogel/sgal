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

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

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

  /*! Traverse the node hierarchy and insert each node to the appropriate
   * drawing list.
   * \param node (in) a pointer to the node to draw.
   */
  virtual void cull(Node* node, Camera* camera);

  /*! Draw the collected nodes (after culling).
   */
  virtual void draw(Draw_action* draw_action);

  /*! Add a shape node.
   */
  void add_shape(Shape* shape);

  /*! Add a light node.
   * \param light a pointer to the Light.
   */
  void add_light(Light* light);

  /*! Set the head light. */
  void set_head_light(Shared_light light);

  /*! Push the transform matrix. */
  void push_matrix(const Matrix4f& mat);

  /*! Pop the transform matrix. */
  void pop_matrix();

  Camera* get_camera() const;

  const Matrix4f& get_current_wtm();

  void set_current_lod(Int lod);

  Int get_current_lod() const;

  /*! Set the flag that indicates whether the node hierarchy has been
   * transformed.
   * \param transformed (in) indicates whether the node hierarchy has been
   *                    transformed.
   */
  void set_transformed(Boolean transformed);

  /*! Determine whether the node hierarchy has been transformed.
   * \return true if the node hierarchy has been transformed and false
   *         otherwise.
   */
  Boolean is_transformed() const;

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

  /*! Indicates whether sorting is needed. */
  Boolean m_sort;

  /*! Indicates whether the node hierarchy must be transformed.
   * This is used, for example, for billboarding. If the node hierarchy has
   * been transformed, then the billboard transform matrix must be recalculated.
   */
  Boolean m_transformed;
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

inline Camera* Cull_context::get_camera() const { return m_camera; }

inline const Matrix4f& Cull_context::get_current_wtm() { return m_world_tm; }

inline void Cull_context::set_current_lod(Int lod) { m_current_lod = lod; }

inline Int Cull_context::get_current_lod() const { return m_current_lod; }

/*! \brief sets the flag that indicates whether the node hierarchy has been
 * transformed.
 */
inline void Cull_context::set_transformed(Boolean transformed)
{ m_transformed = transformed; }

//! \brief determines whether the node hierarchy has been transformed.
inline Boolean Cull_context::is_transformed() const { return m_transformed; }

SGAL_END_NAMESPACE

#endif
