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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CAMERA_HPP
#define SGAL_CAMERA_HPP

/*! \file
 * A camera class.
 *
 *  Inherits from Container
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Rotation.hpp"
#include "SGAL/Bindable_node.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Camera_pool;
class Context;
class Container_proto;
class Element;
class Scene_graph;
class Container;
class Bindable_stack;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Camera : public Bindable_node {
public:
  enum {
    FIRST = Bindable_node::LAST - 1,
    POSITION,
    ORIENTATION,
    FIELD_OF_VIEW,
    ASPECT_RATIO,
    DESCRIPTION,
    OFFSET,
    RADIUS_SCALE,
    FAR_PLANE_SCALE,
    LAST
  };

  /*! Constructor */
  Camera(Boolean proto = false);

  /*! Destructor */
  virtual ~Camera();

  /*! Construct the prototype. */
  static Camera* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Protoype handling. */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  // position
  // Vector3f* position_handle(const Field_info*) { return &m_position; }
  Rotation* orientation_handle(const Field_info*) { return &m_orientation; }
  Float* fov_handle(const Field_info*) { return &m_field_of_view; }
  Float* aspect_ratio_handle(const Field_info*) { return &m_aspect_ratio; }
  std::string* description_handle(const Field_info*) { return &m_description; }
  Vector3f* offset_handle(const Field_info*) { return &m_offset; }
  Float* radius_scale_handle(const Field_info*) { return &m_radius_scale; }
  //@}

  /*! Set the attributes of this container. */
  virtual void set_attributes(Element* elem);

  /*! Apply the camera. */
  virtual void draw(Draw_action* action);

  /*! Apply the camera. */
  virtual void draw(Isect_action* action);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  //! \todo what for? static const std::string m_static_tag;

  /*! Obtain the bindable stack. */
  virtual Bindable_stack* get_stack();

  /*! Enable the bindable node. */
  virtual void enable();

  /*! Process change of field-of-view.
   * \param field_info
   */
  void field_of_view_changed(const Field_info* info);

  /*! Process change of viewing components.
   * \param field_info
   */
  void components_changed(const Field_info* info);

  /*! Set the camera relative position. */
  void set_offset(const Vector3f& position);

  /*! Set the camera relative position. */
  void set_offset(Float x, Float y, Float z);

  /*! Obtain the camera relative position. */
  const Vector3f& get_offset() const;

  /*! Obtain the camera (eye) position. */
  Vector3f get_position() const;

  /*! Obtain the normalized "right" vector.
   */
  const Vector3f& get_right() const;

  /*! Obtain the normalized "up" vector.
   */
  const Vector3f& get_up() const;

  /*! Obtain the normalized "forward" vector.
   */
  const Vector3f& get_forward() const;

  /*! Set the camera relative orientation. */
  void set_orientation(const Rotation& orientation);

  /*! Set the camera relative orientation. */
  void set_orientation(Float v0, Float v1, Float v2, Float v3);

  /*! Obtain the camera relative orientation. */
  const Rotation& get_orientation() const;

  /*! Set the fiewd-of-view of the camera. */
  void set_field_of_view(float fov);

  /*! Obtain the fiewd-of-view of the camera. */
  float get_field_of_view();

  /*! Set the textual description of the camera. */
  void set_description(const std::string& description);

  /*! Obtain the textual description of the camera. */
  const std::string& get_description() const;

  /*! Set the viewpoint line-of-sight and up default vectors.
   * \param[in] line_of_sight
   * \param[in] up
   */
  void set_viewpoint(const Vector3f& line_of_sight, const Vector3f& up);

  /*! Obtain the frustum.
   */
  Frustum& get_frustum();

  /*! Obtain the viewing matrix.
   */
  const Matrix4f& get_view_mat();

  /*! Set The near and far clipping planes of the frustum. */
  void set_clipping_planes(float near_plane, float far_plane);

  /*! Obtain The near clipping plane. */
  float get_nearest_clipping_plane() const;

  /*! Obtain The near clipping plane. */
  float get_furthest_clipping_plane() const;

  /*! Set the camera viewing transformation.
   * \param[in] eye the position of the eye point.
   * \param[in] yaw
   * \param[in] pitch
   */
  void view(const Vector3f& eye, Float yaw, Float pitch);

  /*! Update some camera parameters based on the graphics context.
   */
  void update();

  /*! Initialize the camera. */
  void init();

  /*! Set the scene graph. */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  void set_radius_scale(float scale);

  float get_radius_scale() const { return m_radius_scale; }

  void set_far_plane_scale(float scale);

  float get_far_plane_scale() const { return m_far_plane_scale; }

protected:
  //! The Scene_graph.
  Scene_graph* m_scene_graph;

  //! The offset of the camera.
  // Vector3f m_position;

  //! The rotation relative to the local coordinate system.
  Rotation m_orientation;

  //! The camera line of sight.
  Vector3f m_line_of_sight;

  //! The up vector.
  Vector3f m_up;

  //! The eye position.
  Vector3f m_eye;

  //! The normalized "right" vector.
  Vector3f m_xaxis;

  //! The normalized "up" vector.
  Vector3f m_yaxis;

  //! The normalized "forward" vector.
  Vector3f m_zaxis;

  //! The camera viewong matrix.
  Matrix4f m_view_mat;

  //! The camera frustum.
  Frustum m_frustum;

  //! The camera field of view.
  float m_field_of_view;

  //! The camera aspect ratio.
  float m_aspect_ratio;

  //! The nearest clipping plane.
  float m_nearest_clipping_plane;

  //! The furthest clipping plane.
  float m_furthest_clipping_plane;

  //! The distance betwen the camera eye position and its target.
  Float m_distance;

  //! The effective radius of the bounding sphere.
  Float m_radius;

  //! The offset of the camera.
  Vector3f m_offset;

  //! The scale factor the radius of the bounding sphere is extended by.
  float m_radius_scale;

  //! The scale value the far plane is extended by.
  float m_far_plane_scale;

  //! Indicates whether the camera axes are dirty and thus must be cleaned.
  bool m_dirty_axes;

  //! Indicates whether the modelview matrix is durty and thus must be cleaned.
  bool m_dirty_matrix;

  //! Indicates whether the frustum is dirty and thus must be cleaned.
  bool m_dirty_frustum;

  /*! Indicates whether the distance between the camera eye position and its
   * target is dirty and thus must be cleaned.
   */
  bool m_dirty_distance;

  //! The textual description of the camera. (Used by VRML.)
  std::string m_description;

  /*! Obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  static Container_proto* s_prototype;

  // Defaults values
  static const Vector3f s_def_line_of_sight;
  static const Vector3f s_def_up;
  static const Vector3f s_def_offset;
  static const Rotation s_def_orientation;
  static const float s_def_field_of_view;
  static Frustum s_def_frustum;
  static const float s_def_nearest_clipping_plane;
  static const float s_def_furthest_clipping_plane;
  static const std::string s_def_description;
  static const float s_def_radius_scale;
  static const float s_def_far_plane_scale;

  /*! Clean the distance. */
  void clean_distance();

  /*! Clean the axes. */
  void clean_axes();

  /*! Clean the camera viewing matrix. */
  void clean_matrix();

  /*! Clean the frustum so that the frustum contains the bounding-sphere. */
  void clean_frustum();

  /*! Apply the camera. */
  void draw();

  // static Isect_action * m_picker;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Camera* Camera::prototype() { return new Camera(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Camera::create()
{ return new Camera(); }

//! \brief obtains The near clipping plane. */
inline float Camera::get_nearest_clipping_plane() const
{ return m_nearest_clipping_plane; }

//! obtains The near clipping plane. */
inline float Camera::get_furthest_clipping_plane() const
{ return m_furthest_clipping_plane; }

//! \brief sets the camera offset.
inline void Camera::set_offset(Float x, Float y, Float z)
{ set_offset(Vector3f(x, y, z)); }

//! \brief sets the camera orientation. */
inline void Camera::set_orientation(Float v0, Float v1, Float v2, Float v3)
{ set_orientation(Rotation(v0, v1, v2, v3)); }

//! \brief obtains the camera offset.
inline const Vector3f& Camera::get_offset() const { return m_offset; }

//! \brief obtains the camera orientation.
inline const Rotation& Camera::get_orientation() const { return m_orientation; }

//! \brief obtains the normalized "right" vector.
inline const Vector3f& Camera::get_right() const { return m_xaxis; }

//! \brief obtain the normalized "up" vector.
inline const Vector3f& Camera::get_up() const { return m_yaxis; }

//! \brief obtain the normalized "forward" vector.
inline const Vector3f& Camera::get_forward() const { return m_zaxis; }

//! \brief sets the textual description of the camera.
inline void Camera::set_description(const std::string& description)
{ m_description = description; }

//! \brief obtains the textual description of the camera.
inline const std::string& Camera::get_description() const
{ return m_description; }

SGAL_END_NAMESPACE

#endif
