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
    FIELDOFVIEW,
    DESCRIPTION,
    TYPE,
    RADIUS_SCALE,
    FAR_PLANE_SCALE,
    POSITION_TRANSLATION,
    LAST
  };

  /*! Constructor */
  Camera(Boolean proto = false);

  /*! Destructor */
  virtual ~Camera();

  /*! Construct the prototype. */
  static Camera* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Protoype handling. */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  // position
  Vector3f* position_handle(const Field_info*) { return &m_position; }
  Rotation* orientation_handle(const Field_info*) { return &m_orientation; }
  Float* fov_handle(const Field_info*) { return &m_field_of_view; }
  std::string* description_handle(const Field_info*) { return &m_description; }
  Float* radius_scale_handle(const Field_info*) { return &m_radius_scale; }
  //@}

  /*! Set the attributes of this container. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Set the camera position. */
  void set_position(const Vector3f& position);

  /*! Set the camera position. */
  void set_position(Float x, Float y, Float z);

  /*! Obtain the camera position. */
  const Vector3f& get_position() const;

  /*! Set the camera orientation. */
  void set_orientation(const Rotation& orientation);

  /*! Set the camera orientation. */
  void set_orientation(Float v0, Float v1, Float v2, Float v3);

  /*! Obtain the camera orientation. */
  const Rotation& get_orientation() const;

  /*! Set the fiewd-of-view of the camera. */
  void set_field_of_view( float fov );

  /*! Obtain the fiewd-of-view of the camera. */
  float get_field_of_view();

  void update_field_of_view(const Field_info* info);

  /*! Set the textual description of the camera. */
  void set_description(const std::string& description);

  /*! Obtain the textual description of the camera. */
  const std::string& get_description() const;

  /*! Obtain the (non-const) frustum. */
  Frustum& get_frustum();

  /*! Obtain the (const) frustum. */
  const Frustum& get_frustum() const;

  const Matrix4f& get_view_mat();

  void update_matrix_requiered(const Field_info* info);

  void set_clipping_planes(float near_plane, float far_plane);

  void get_clipping_planes(float& near_plane, float& far_plane);

  /*! Set the clipping planes so that the frustum contains the
   * bounding-sphere.
   * \param bb_center (in) the center of the bounding sphere
   * \param bb_radius (in) the radius of the bounding sphere
   */
  void set_clipping_planes(const Vector3f& bb_center, float bb_radius);

  void set_dynamic_clipping_planes();

  /*! Initialize some camera parameters. Cannot be called from
   * the constructor, but does not require a scene graph nor a context.
   */
  void utilize();

  /*! Update the aspect ratio based on the context. */
  void set_aspect_ratio(const Context* context);

  /*! Initialize the camera based on the given context. */
  void init(const Context* context);

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

  /*! Set the scene graph. */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  void set_is_dynamic(bool flag) { m_is_dynamic = flag; }

  bool get_is_dynamic() const { return m_is_dynamic; }

  void set_radius_scale(float scale) { m_radius_scale = scale; }

  float get_radius_scale() const { return m_radius_scale; }

  void set_far_plane_scale(float scale) { m_far_plane_scale = scale; }

  float get_far_plane_scale() const { return m_far_plane_scale; }

protected:
  /*! The Scene_graph. */
  Scene_graph* m_scene_graph;

  /*! Indicates whether the cliping planes are set dynamically. */
  bool m_is_dynamic;

  Vector3f m_position;

  Rotation m_orientation;

  Matrix4f m_view_mat;

  Frustum m_frustum;

  float m_field_of_view;

  bool m_dirty_matrix;

  int m_id;

  float m_nearest_clipping_plane;

  /*! The scale factor the radius of the bounding sphere is extended by. */
  float m_radius_scale;

  /*! The scale value the far plane is extended by. */
  float m_far_plane_scale;

  /*! The translational vector to adjust the initial position. */
  Vector3f m_position_translation;

  /*! The textual description of the camera. (Used by VRML.) */
  std::string m_description;

  /*! Obtains the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  static Container_proto* s_prototype;

  // Defaults values
  static const Vector3f s_def_position;
  static const Rotation s_def_orientation;
  static const float s_def_field_of_view;
  static Frustum s_def_frustum;
  static const std::string s_def_description;
  static const float s_def_radius_scale;
  static const float s_def_far_plane_scale;

  void clean_matrix();

  /*! Apply the camera. */
  void draw();

  // static Isect_action * m_picker;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief constructs the prototype. */
inline Camera* Camera::prototype() { return new Camera(true); }

/*! \brief clones. */
inline Container* Camera::clone() { return new Camera(); }

/*! \brief sets the camera position. */
inline void Camera::set_position(Float x, Float y, Float z)
{ set_position(Vector3f(x, y, z)); }

/*! \brief sets the camera orientation. */
inline void Camera::set_orientation(Float v0, Float v1, Float v2, Float v3)
{ set_orientation(Rotation(v0, v1, v2, v3)); }

/*! \brief obtains the camera position. */
inline const Vector3f& Camera::get_position() const { return m_position; }

/*! \brief obtains the camera orientation. */
inline const Rotation& Camera::get_orientation() const { return m_orientation; }

/*! \brief sets the textual description of the camera. */
inline void Camera::set_description(const std::string& description)
{ m_description = description; }

/*! \brief obtains the textual description of the camera. */
inline const std::string& Camera::get_description() const
{ return m_description; }

/*! \brief obtains the (non-const) frustum. */
inline Frustum& Camera::get_frustum() { return m_frustum; }

/*! \brief obtains the (const) frustum. */
inline const Frustum& Camera::get_frustum() const { return m_frustum; }

SGAL_END_NAMESPACE

#endif
