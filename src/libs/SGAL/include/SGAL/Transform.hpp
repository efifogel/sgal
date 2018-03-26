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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TRANSFORM_HPP
#define SGAL_TRANSFORM_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Transform Transform.hpp
 * Transform is a node in the scene graph that represents a transformation
 * applied to geomteric objects. It inherits from Group and as such can have
 * multiple child nodes. It applies the transformation it represents to all
 * of its children. An object is first translated, then rotated around a
 * center, then scaled in the direction of a scaling orientation.
 *
 * When a an action (such as a drawing action) is applied to a transform node
 *   (i) the action matrix stack is pushed down, next
 *  (ii) the transform is applied to the top of the stack, next
 * (iii) the action is applied to all child nodes, finally
 *  (iv) action matrix stack is poped.
 */
class SGAL_SGAL_DECL Transform : public Group {
public:
  enum {
    FIRST = Group::LAST - 1,
    TRANSLATION,
    ROTATION,
    SCALE,
    SCALE_ORIENTATION,
    CENTER,
    RESET,
    LAST
  };

  /*! Construct.
   * \param proto (in) determines whether to construct a prototype.
   */
  Transform(Boolean proto = false);

  /*! Destruct. */
  virtual ~Transform();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Transform* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Clone the container (virtual constructor) with deep copy.
   * \return a clone of this container.
   */
  virtual Container* clone();

  /*! Apply a unary operation on every field of the container.
   * \param op[in] the unary operation to apply.
   */
  virtual void apply(Field_value_applier& op);

  /*! Transform every field of this container using a unary operation into
   * a corresponding field in a target container.
   * \param target[in] the target container.
   * \param op[in] the unary operation to apply.
   */
  virtual void transform(Container* target, Field_value_transformer& op);

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
  Vector3f* center_handle(const Field_info*) { return &m_center; }
  Vector3f* translation_handle(const Field_info*) { return &m_translation; }
  Rotation* rotation_handle(const Field_info*) { return &m_rotation; }
  Vector3f* scale_handle(const Field_info*) { return &m_scale; }
  Boolean* reset_handle(const Field_info*) { return &m_reset; }
  //@}

  /*! Set the attributes of the transform.
   * \param elem (in) contains lists of attribute name and value pairs.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  virtual void cull(Cull_context& cull_context);

  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the transformation node. */
  virtual void clean_bounding_sphere();

  /*! Set the translation.
   * \param translation (in) the new translation.
   */
  void set_translation(const Vector3f& translation);

  /*! Obtain the translation.
   * \return the translation.
   */
  const Vector3f& get_translation();

  /*! Set the rotation.
   * rotation (in) the new rotation.
   */
  void set_rotation(const Rotation& rotation);

  /*! Obtain the rotation.
   * \return the rotation.
   */
  const Rotation& get_rotation();

  void set_scale(const Vector3f& scale);

  /*! Obtain the scale.
   * \return the scale.
   */
  const Vector3f& get_scale();

  void set_scale_orientation(const Rotation& scaleOrientation);

  /*! Obtain the scale orientation.
   * \return the scale orientation.
   */
  const Rotation& get_scale_orientation();

  void set_center(const Vector3f& center);

  /*! Obtain the center of rotation.
   * \return the center of rotation.
   */
  const Vector3f& get_center();

  /*! Set the affine transformation 4x4 matrix.
   * \param matrix (in) the matrix.
   */
  void set_matrix(const Matrix4f& matrix);

  /*! Obtain the matrix.
   * \return the matrix
   */
  const Matrix4f& get_matrix();

  void set_translation(Float v0, Float v1, Float v2);

  void get_translation(Float& v0, Float& v1, Float& v2);

  void set_rotation(Float v0, Float v1, Float v2, Float v3);

  void get_rotation(Float& v0, Float& v1, Float& v2, Float& v3);

  void set_scale(Float v0, Float v1, Float v2);

  void get_scale(Float& v0, Float& v1, Float& v2);

  void set_scale_orientation(Float v0, Float v1, Float v2, Float v3);

  void get_scale_orientation(Float& v0, Float& v1, Float& v2, Float& v3);

  void set_center(Float v0, Float v1, Float v2);

  void get_center(Float& v0, Float& v1, Float& v2);

  /*! Set the flag that indicates whether the transformation took place.
   * \param transformed (in) indicates whether transformation took place.
   */
  void set_transformed(Boolean transformed);

  /*! Determine whether transformation took place.
   * \return true if transformation took place and false otherwise.
   */
  Boolean is_transformed() const;

  /*! Process change of parts.
   * \param field_info (in) the field information record of the particular
   *                   part that got changed.
   */
  void parts_changed(const Field_info* field_info = nullptr);

  /*! Copy only the local fields of a given container.
   */
  void copy_local(const Transform* other);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! The translation vector.
  Vector3f m_translation;

  //! The rotation vector.
  Rotation m_rotation;

  //! The scale vector.
  Vector3f m_scale;

  //! The scale oriantation vector.
  Rotation m_scale_orientation;

  //! The center of the transform.
  Vector3f m_center;

  //! The matrix representing the transform.
  Matrix4f m_matrix;

  //! The inverse of the matrix representing the transform.
  Matrix4f m_inverse_matrix;

  //! Indicates whether the matrix is not up-to-date.
  Boolean m_dirty_matrix;

  //! Indicates whether the transform should be reset.
  bool m_reset;

  /*! Indicates whether the inverse transform matrix is dirty and thus must
   * be cleaned.
   */
  Boolean m_dirty_inverse;

  /*! Indicates whether the individual parts are dirty and thus must be
   * cleaned. The parts become dirty if the matrix that represents the
   * transform is set, for example.
   */
  Boolean m_dirty_parts;

  /*! Indicates weather transformation took place.
   * This is used, for example, for billboarding. If transformation took place,
   * then the billboard transform matrix must be recalculated.
   */
  Boolean m_transformed;

  //! Default values.
  static const Vector3f s_def_translation;
  static const Rotation s_def_rotation;
  static const Vector3f s_def_scale;
  static const Rotation s_def_scale_orientation;
  static const Vector3f s_def_center;

  /*! Calculate the matrix out of the individual parts. */
  void clean_matrix();

  /*! Extract the individual Transformations (e.g., scale) from the matrix. */
  void clean_parts();

  /*! Reset all field to original values; e.g., set the matrix to identity. */
  void reset(const Field_info* field_info);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Transform* Transform::prototype() { return new Transform(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Transform::create()
{ return new Transform(); }

//! \brief obtains the tag (type) of the container.
inline const std::string& Transform::get_tag() const { return s_tag; }

//! \brief sets the flag that indicates whether the transformation took place.
inline void Transform::set_transformed(Boolean transformed)
{ m_transformed = transformed; }

//! \brief determines whether transformation took place.
inline Boolean Transform::is_transformed() const
{ return m_transformed; }

SGAL_END_NAMESPACE

#endif
