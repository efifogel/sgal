// Copyright (c) 2014 Israel.
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

#ifndef SGAL_BILLBOARD_HPP
#define SGAL_BILLBOARD_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Field_info;

/*! \class Billboard Billboard.hpp
 * Billboard is a grouping node in the scene graph; that is, it has children,
 * which may be other grouping or leaf nodes. It modifies its local coordinate
 * system so that the z-axis turns to point at the viewer.
 */
class SGAL_SGAL_DECL Billboard : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    AXIS_OF_ROTATION,
    LAST
  };

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Billboard(Boolean proto = false);

  /*! Destructor. */
  virtual ~Billboard();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Billboard* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Vector3f* axis_of_rotation_handle(const Field_info*)
  { return &m_axis_of_rotation; }
  //@}

  /*! Set the attributes of the transform.
   * \param elem contains lists of attribute name and value pairs.
   */
  virtual void set_attributes(Element* elem);

  virtual void cull(Cull_context& cull_context);

  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the transformation node. */
  virtual Boolean clean_sphere_bound();

  /*! Set the axis used to perform the rotation.
   * \param axis_of_rotation (in) the axis or rotation.
   */
  void set_axis_of_rotation(const Vector3f& axis_of_rotation);

  /*! Obtain the axis used to perform the rotation.
   * \return the axis or rotation.
   */
  const Vector3f& get_axis_of_rotation() const;

  /*! Process change of axis of rotation.
   * \param field_info (in) the field information record of the axis of
   *                   rotation field.
   */
  void axis_of_rotation_changed(const Field_info* field_info = nullptr);

  /*! Set the flag that indicates whether the transformation took place.
   * \param transformed (in) indicates whether transformation took place.
   */
  void set_transformed(Boolean transformed);

  /*! Determine whether transformation took place.
   * \return true if transformation took place and false otherwise.
   */
  Boolean is_transformed() const;

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Specifies which axis used to perform the rotation.
   * This axis is defined in the local coordinates of the Billboard node.
   * The default (0,1,0) is useful for objects such as images of trees and
   * lamps positioned on a ground plane. But when an object is oriented at an
   * angle, for example, on the incline of a mountain, then the
   * axis_of_rotation may also need to be oriented at a similar angle.
   *
   * A special case of billboarding is screen-alignment---the object rotates
   * so that it stays aligned with the viewer even when the viewer elevates,
   * pitches and rolls. This special case is distinguished by setting the
   * axis_of_rotation to (0, 0, 0).
   */
  Vector3f m_axis_of_rotation;

  /*! The matrix representing the billboard. */
  Matrix4f m_matrix;

  /*! Indicates whether the matrix is not up-to-date. */
  Boolean m_dirty_matrix;

  /*! Indicates weather transformation took place.
   * This is used, for example, for billboarding. If transformation took place,
   * then the billboard transform matrix must be recalculated.
   */
  Boolean m_transformed;

  /*! Default values */
  static const Vector3f s_def_axis_of_rotation;

  /*! Clean the transform matrix. */
  void clean_matrix(const Matrix4f& matrix);
};

//! \brief constructs the prototype.
inline Billboard* Billboard::prototype() { return new Billboard(true); }

//! \brief clone.
inline Container* Billboard::clone() { return new Billboard(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Billboard::get_tag() const { return s_tag; }

//! \brief obtains the axis used to perform the rotation.
inline const Vector3f& Billboard::get_axis_of_rotation() const
{ return m_axis_of_rotation; }

//! \brief sets the flag that indicates whether the transformation took place.
inline void Billboard::set_transformed(Boolean transformed)
{ m_transformed = transformed; }

//! \brief determines whether transformation took place.
inline Boolean Billboard::is_transformed() const
{ return m_transformed; }

SGAL_END_NAMESPACE

#endif

