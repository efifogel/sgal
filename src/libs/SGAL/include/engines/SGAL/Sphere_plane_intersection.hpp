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

/*! \file
 *
 * Computes the intersection of a sphere and a plane
 */

#ifndef SGAL_SPHERE_PLANE_INTERSECTION_HPP
#define SGAL_SPHERE_PLANE_INTERSECTION_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Field_info;

class SGAL_SGAL_DECL Sphere_plane_intersection : public Node {
public:
  enum {
    FIRST = Node::LAST-1,
    SPHERE_RADIUS,
    PLANE,
    CIRCLE_TRANSLATION,
    CIRCLE_ROTATION,
    CIRCLE_RADIUS,
    TRIGGER,
    LAST
  };

  /*! Constructor. */
  Sphere_plane_intersection(Boolean proto = false);

  /*! Destructor. */
  virtual ~Sphere_plane_intersection();

  /*! Construct the prototype. */
  static Sphere_plane_intersection* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* sphere_radius_handle(const Field_info*) { return &m_sphere_radius; }
  Vector4f* plane_handle(const Field_info*) { return &m_plane; }
  Boolean* trigger_handle(const Field_info*) { return &m_trigger; }
  Vector3f* circle_translation_handle(const Field_info*)
  { return &m_circle_translation; }
  Rotation* circle_rotation_handle(const Field_info*)
  { return &m_circle_rotation; }
  Float* circle_radius_handle(const Field_info*) { return &m_circle_radius; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Trigger the execution of the engine as a response to change in one
   * of the input fields.
   */
  void trigger_changed(const Field_info* field_info);

  /*! The interpolation execution function.
   */
  void execute();

  /*! Obtain the sphere radius.
   */
  Float get_sphere_radius() const;

  /*! Set the sphere radius.
   */
  void set_sphere_radius(Float radius);

  /*! Obtain the plane coefficients.
   */
  const Vector4f& get_plane() const;

  /*! Set the plane coefficients.
   */
  void set_plane(const Vector4f& vec);

  /*! Obtain the circle translation.
   */
  const Vector3f& get_circle_translation() const;

  /*! Obtain the circle rotation.
   */
  const Rotation& get_circle_rotation() const;

  /*! Obtain the circle radius.
   */
  Float get_circle_radius() const;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! The sphere radius. */
  Float m_sphere_radius;

  /*! The plane coefficients */
  Vector4f m_plane;

  /*! The circle translation. */
  Vector3f m_circle_translation;

  /*! The circle rotation. */
  Rotation m_circle_rotation;

  /*! The circle radius. */
  Float m_circle_radius;

  /*! Trigger of the engine that makes the engine excute. */
  Boolean m_trigger;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  // Default values:
  static const Float s_def_sphere_radius;
  static const Vector4f s_def_plane;
};

//! \brief constructs the prototype.
inline Sphere_plane_intersection* Sphere_plane_intersection::prototype()
{ return new Sphere_plane_intersection(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Sphere_plane_intersection::create()
{ return new Sphere_plane_intersection(); }

//! \brief obtains the sphere radius.
inline Float Sphere_plane_intersection::get_sphere_radius() const
{ return m_sphere_radius; }

//! \brief sets the sphere radius.
inline void Sphere_plane_intersection::set_sphere_radius(Float radius)
{ m_sphere_radius = radius; }

//! \brief obtains the plane coefficients.
inline const Vector4f& Sphere_plane_intersection::get_plane() const
{ return m_plane; }

//! \brief sets the plane coefficients.
inline void Sphere_plane_intersection::set_plane(const Vector4f& vec)
{ m_plane = vec; }

//! \brief obtains the circle translation.
inline const Vector3f& Sphere_plane_intersection::get_circle_translation()
  const
{ return m_circle_translation; }

//! \brief obtains the circle rotation.
inline const Rotation& Sphere_plane_intersection::get_circle_rotation() const
{ return m_circle_rotation; }

//! \brief obtains the circle radius.
inline Float Sphere_plane_intersection::get_circle_radius() const
{ return m_circle_radius; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Sphere_plane_intersection::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE

#endif
