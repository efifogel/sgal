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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TRANSFORM_HPP
#define SGAL_TRANSFORM_HPP

/*! \file
 * A class representing transformation in the scene graph.
 * 
 * This is a transformation node in the scene graph. It inherits 
 * from Group and therefore can have multiple child objects. It
 * contains a tranlation vector, a rotation and a scale vector. It
 * also contains a matrix representation of the transformation.
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Rotation.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Formatter;

class SGAL_CLASSDEF Transform : public Group {
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

  /*! Constructor */
  Transform(Boolean proto = false);

  /*! Destructor */
  virtual ~Transform();

  /* Construct the prototype */
  static Transform* prototype();

  /*! Clone */
  virtual Container* clone();

  void set_translation(const Vector3f& translation);
  void get_translation(Vector3f& translation);
  void set_rotation(const Rotation& rotation);
  void get_rotation(Rotation& rotation);
  void set_scale(const Vector3f& scale);
  void get_scale(Vector3f& scale);
  void set_scale_orientation(const Rotation& scaleOrientation);
  void get_scale_orientation(Rotation& scaleOrientation);
  void set_center(const Vector3f& center);
  void get_center(Vector3f& center);

  void set_matrix(const Matrix4f& matrix);

  /*! Obtain (a copy of) the matrix */
  void get_matrix(Matrix4f& matrix);

  /*! Obtain (a const reference of) the matrix */  
  const Matrix4f& get_matrix();
  
  void set_translation(Float v0, Float v1, Float v2);
  void get_translation(Float* v0, Float* v1, Float* v2);
  void set_rotation(Float v0, Float v1, Float v2, Float v3);
  void get_rotation(Float* v0, Float* v1, Float* v2, Float* v3);
  void set_scale(Float v0, Float v1, Float v2);
  void get_scale(Float* v0, Float* v1, Float* v2);
  void set_scale_orientation(Float v0, Float v1, Float v2, Float v3);
  void get_scale_orientation(Float* v0, Float* v1, Float* v2, Float* v3);
  void set_center(Float v0, Float v1, Float v2);
  void get_center(Float* v0, Float* v1, Float* v2);

  void parts_changed(Field_info* field_info = NULL);

  virtual void cull(Cull_context& cull_context);
  virtual void isect(Isect_action* isect_action);

  /*! Clean the bounding sphere of the transformation node. */
  virtual Boolean clean_sphere_bound();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! \bried writes this container. */
  virtual void write(Formatter* formatter);
  
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  
protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The translation vector. */
  Vector3f m_translation;

  /*! The rotation vector. */
  Rotation m_rotation;

  /*! The scale vector. */
  Vector3f m_scale;

  /*! The scale oriantation vector. */
  Rotation m_scale_orientation;

  /*! The center of the transform. */
  Vector3f m_center;

  /*! The matrix representing of the transformation. */
  Matrix4f m_matrix;

  /*! The inverse of the matrix representing the transformation. */
  Matrix4f m_inverse_matrix;

  /*! Indicates whether the matrix is not up-to-date. */
  Boolean m_dirty_matrix;

  /*! Indicates whether the transform should be reset. */
  bool m_reset;
  
  Boolean m_dirty_inverse;

  Boolean m_dirty_parts;

  /*! Default values */
  static Vector3f s_def_translation;
  static Rotation s_def_rotation;
  static Vector3f s_def_scale;
  static Rotation s_def_scale_orientation;
  static Vector3f s_def_center;
  static Vector3f s_def_bbox_center;
  static Vector3f s_def_bbox_size;

  void clean_parts();

  void calc_matrix();

  void clean_matrix();

  void reset(Field_info* field_info);
};

/* \brief constructs the prototype. */
inline Transform* Transform::prototype() { return new Transform(true); }

/*! \brief clones. */
inline Container* Transform::clone() { return new Transform(); }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Transform::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
