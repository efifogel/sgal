// Copyright (c) 2004  Israel.
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

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Texture_transform.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Texture_transform::s_prototype(nullptr);
const std::string Texture_transform::s_tag = "TextureTransform";

// Default values
const Vector2f Texture_transform::s_def_center(0, 0);
const Float Texture_transform::s_def_rotation(0);
const Vector2f Texture_transform::s_def_scale(1, 1);
const Vector2f Texture_transform::s_def_translation(0, 0);

REGISTER_TO_FACTORY(Texture_transform, "Textture_transform");

//! Constructor
Texture_transform::Texture_transform(Boolean proto) :
  Container(proto),
  m_center(s_def_center),
  m_rotation(s_def_rotation),
  m_scale(s_def_scale),
  m_translation(s_def_translation),
  m_dirty_matrix(false),
  m_dirty_parts(false)
{}

//! Destructor
Texture_transform::~Texture_transform() {}

//! Initialize the node prototype.
void Texture_transform::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Texture_transform::parts_changed);

  // center
  Vector2f_handle_function center_func =
    static_cast<Vector2f_handle_function>(&Texture_transform::center_handle);
  s_prototype->add_field_info(new SF_vector2f(CENTER, "center",
                                              RULE_EXPOSED_FIELD,
                                              center_func,
                                              s_def_center, exec_func));

  // rotation
  Float_handle_function rotation_func =
    static_cast<Float_handle_function>(&Texture_transform::rotation_handle);
  s_prototype->add_field_info(new SF_float(ROTATION, "rotation",
                                           RULE_EXPOSED_FIELD,
                                           rotation_func,
                                           s_def_rotation, exec_func));

  // scale
  Vector2f_handle_function scale_func =
    static_cast<Vector2f_handle_function>(&Texture_transform::scale_handle);
  s_prototype->add_field_info(new SF_vector2f(SCALE, "scale",
                                              RULE_EXPOSED_FIELD,
                                              scale_func,
                                              s_def_scale, exec_func));

  // translation
  Vector2f_handle_function translation_func =
    static_cast<Vector2f_handle_function>
    (&Texture_transform::translation_handle);
  s_prototype->add_field_info(new SF_vector2f(TRANSLATION, "translation",
                                              RULE_EXPOSED_FIELD,
                                              translation_func,
                                              s_def_translation, exec_func));
}

//! Delete the node prototype.
void Texture_transform::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! Obtain the node prototype.
Container_proto* Texture_transform::get_prototype()
{
  if (!s_prototype) Texture_transform::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this container.
void Texture_transform::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ai++) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "center") {
      Vector2f vec(value);
      set_center(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "rotation") {
      set_rotation(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "scale") {
      Vector2f vec(value);
      set_scale(vec);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "translation") {
      Vector2f vec(value);
      set_translation(vec);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
};

//! \brief sets the center point of the rotation.
void Texture_transform::set_center(const Vector2f& center)
{
  m_center = center;
  m_dirty_matrix = true;
}

//! \brief obtains the center point of the rotation.
const Vector2f& Texture_transform::get_center()
{
  return m_center;
  if (m_dirty_parts) clean_parts();
}

//! \brief sets the rotation angle.
void Texture_transform::set_rotation(Float rotation)
{
  m_rotation = rotation;
  m_dirty_matrix = true;
}

//! \brief obtains the rotation angle.
Float Texture_transform::get_rotation()
{
  return m_rotation;
  if (m_dirty_parts) clean_parts();
}

//! \brief sets the a non-uniform scale about an arbitrary center point.
void Texture_transform::set_scale(const Vector2f& scale)
{
  m_scale = scale;
  m_dirty_matrix = true;
}

//! \brief obtains the non-uniform scale about an arbitrary center point.
const Vector2f& Texture_transform::get_scale()
{
  return m_scale;
  if (m_dirty_parts) clean_parts();
}

//! \brief sets the translation.
void Texture_transform::set_translation(const Vector2f& translation)
{
  m_translation = translation;
  m_dirty_matrix = true;
}

//! \brief obtains the translation.
const Vector2f& Texture_transform::get_translation()
{
  return m_translation;
  if (m_dirty_parts) clean_parts();
}

/*! \brief raises the matrix dirty flag, and sets the flag that indicates that
 * rendering is required.
 */
void Texture_transform::parts_changed(const Field_info* /* field_info */)
{ m_dirty_matrix = true; }

//! \brief calculates the matrix out of the individual parts.
void Texture_transform::clean_matrix()
{
  float mySin = sinf(m_rotation);
  float myCos = cosf(m_rotation);

  m_matrix[0][0] = myCos * m_scale[0];
  m_matrix[0][1] = mySin * m_scale[0];
  m_matrix[0][2] = 0;
  m_matrix[0][3] = 0;

  m_matrix[1][0] = -mySin * m_scale[1];
  m_matrix[1][1] = myCos * m_scale[1];
  m_matrix[1][2] = 0;
  m_matrix[1][3] = 0;

  m_matrix[2][0] = 0;
  m_matrix[2][1] = 0;
  m_matrix[2][2] = 1;
  m_matrix[2][3] = 0;

  m_matrix[3][0] = m_translation[0] + m_center[0] -
    m_center[0] * myCos + m_center[1] * mySin;
  m_matrix[3][1] = m_translation[1] + m_center[1] -
    m_center[0] * mySin - m_center[1] * myCos;
  m_matrix[3][2] = 0;
  m_matrix[3][3] = 1;

  m_dirty_matrix = false;
}

//! \brief applies the texture transform.
void Texture_transform::draw(Context* context)
{
  if (m_dirty_matrix) clean_matrix();
  GLint matrix_mode;
  glGetIntegerv(GL_MATRIX_MODE, &matrix_mode);
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf((GLfloat*) &m_matrix);
  glMatrixMode(matrix_mode);  // restore matrix mode so modelview
  // operations can continue as before
}

//! \brief extracts the individual Transformations from the matrix.
void Texture_transform::clean_parts()
{
  m_dirty_parts = false;
}

SGAL_END_NAMESPACE
