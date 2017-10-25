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

#include <boost/lexical_cast.hpp>

#if (defined _MSC_VER)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>
#include <algorithm>

#include "SGAL/basic.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Material::s_tag = "Material";
Container_proto* Material::s_prototype(nullptr);

// Default values:
const float Material::s_def_ambient_intensity = 0.2f;
const Vector3f Material::s_def_ambient_color(0.16f, 0.16f, 0.16f);
const Vector3f Material::s_def_diffuse_color(0.8f, 0.8f, 0.8f);
const Vector3f Material::s_def_emissive_color(0, 0, 0);
const float Material::s_def_shininess = 0.2f;
const Vector3f Material::s_def_specular_color(0, 0, 0);
const float Material::s_def_transparency = 0;

REGISTER_TO_FACTORY(Material, "Material");

//! \brief constructor
Material::Material(Boolean proto) :
  Container(proto),
  m_ambient_intensity(s_def_ambient_intensity),
  m_ambient_color(s_def_ambient_color),
  m_diffuse_color(s_def_diffuse_color),
  m_specular_color(s_def_specular_color),
  m_emissive_color(s_def_emissive_color),
  m_shininess(s_def_shininess),
  m_transparency(s_def_transparency),
  m_dirty_ambient_intensity(false),
  m_dirty_ambient_color(false)
{}

//! \brief Destructor
Material::~Material() {}

//! \brief sets the transparency.
void Material::set_transparency(float transparency)
{
  const Field_info* field_info = get_field_info(TRANSPARENCY);
  field_changed(field_info);
  m_transparency = transparency;
}

//! \brief obtains the ambient intensity.
float Material::get_ambient_intensity()
{
  if (m_dirty_ambient_intensity) {
    m_ambient_intensity =(m_diffuse_color[0] / m_ambient_color[0] +
                          m_diffuse_color[1] / m_ambient_color[1] +
                          m_diffuse_color[2] / m_ambient_color[2]) * 0.333;
    m_dirty_ambient_intensity = false;
  }
  return m_ambient_intensity;
}

//! \brief sets the ambient intensity.
void Material::set_ambient_intensity(float intensity)
{
  m_ambient_intensity = intensity;
  m_dirty_ambient_intensity = false;
  m_dirty_ambient_color = true;
}

//! \brief sets the ambient color.
void Material::set_ambient_color(float red, float green, float blue)
{
  m_ambient_color[0] = red;
  m_ambient_color[1] = green;
  m_ambient_color[2] = blue;
  m_dirty_ambient_color = false;
  m_dirty_ambient_intensity = true;
}

//! \brief obtains the ambient color.
const Vector3f& Material::get_ambient_color()
{
  if (m_dirty_ambient_color) {
    m_ambient_color.scale(m_ambient_intensity, m_diffuse_color);
    m_dirty_ambient_color = false;
  }
  return m_ambient_color;
}

//! \brief sets the diffuse color.
void Material::set_diffuse_color(float red, float green, float blue)
{
  m_diffuse_color[0] = red;
  m_diffuse_color[1] = green;
  m_diffuse_color[2] = blue;
}

//! \brief sets the specular color.
void Material::set_specular_color(float red, float green, float blue)
{
  m_specular_color[0] = red;
  m_specular_color[1] = green;
  m_specular_color[2] = blue;
}

//! \brief sets the amissive color.
void Material::set_emissive_color(float red, float green, float blue)
{
  m_emissive_color[0] = red;
  m_emissive_color[1] = green;
  m_emissive_color[2] = blue;
}

//! \brief sets the shininess factor.
void Material::set_shininess(float shininess)
{
  if (shininess > 1) m_shininess = 1;
  else m_shininess = shininess;
}

/*! \brief calls the ogl functions to set the current material.
 * we assume that if this method is called, all
 * material parameters need to be set.
 */
void Material::draw(Face which_face, Context* /* context */)
{
  static GLenum s_face_tokens[] = {GL_FRONT, GL_BACK, GL_FRONT_AND_BACK};
  GLenum face = s_face_tokens[which_face];

  Vector4f diffuse(m_diffuse_color);
  Vector4f specular(m_specular_color);
  Vector4f emissive(m_emissive_color);
  Vector4f ambient;
  SGAL_assertion(m_dirty_ambient_color && m_dirty_ambient_intensity);
  if (m_dirty_ambient_intensity) ambient = m_ambient_color;
  else ambient.scale(m_ambient_intensity, m_diffuse_color);

  ambient[3] = diffuse[3] = specular[3] = emissive[3] = 1.0f - m_transparency;

  glMaterialfv(face, GL_AMBIENT, (float*)&ambient);
  glMaterialfv(face, GL_DIFFUSE, (float*)&diffuse);
  glMaterialfv(face, GL_SPECULAR, (float*)&specular);
  glMaterialfv(face, GL_EMISSION, (float*)&emissive);
  glMaterialf(face, GL_SHININESS, 128 * m_shininess);
}

//! \brief the callback invoked when the material chages.
void Material::material_changed(const Field_info* field_info)
{
  set_rendering_required(field_info);
  field_changed(field_info);
}

//! \brief initilalizes the prototype of this node.
void Material::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  // ambientIntensity
  Execution_function exec_func =
    static_cast<Execution_function>(&Material::material_changed);
  Float_handle_function ambient_intensity_func =
    static_cast<Float_handle_function>(&Material::ambient_intensity_handle);
  s_prototype->add_field_info(new SF_float(AMBIENT_INTENSITY,
                                           "ambientIntensity",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           ambient_intensity_func,
                                           s_def_ambient_intensity,
                                           exec_func));

  // ambientColor
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Vector3f_handle_function ambient_color_func =
    static_cast<Vector3f_handle_function>(&Material::ambient_color_handle);
  s_prototype->add_field_info(new SF_vector3f(AMBIENT_COLOR,
                                              "ambientColor",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              ambient_color_func,
                                              s_def_ambient_color, exec_func));

  // diffuseColor
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Vector3f_handle_function diffuse_color_func =
    static_cast<Vector3f_handle_function>(&Material::diffuse_color_handle);
  s_prototype->add_field_info(new SF_vector3f(DIFFUSE_COLOR,
                                              "diffuseColor",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              diffuse_color_func,
                                              s_def_diffuse_color, exec_func));

  // specularColor
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Vector3f_handle_function specular_color_func =
    static_cast<Vector3f_handle_function>(&Material::specular_color_handle);
  s_prototype->add_field_info(new SF_vector3f(SPECULAR_COLOR,
                                              "specularColor",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              specular_color_func,
                                              s_def_specular_color, exec_func));

  // emissiveColor
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Vector3f_handle_function emissive_color_func =
    static_cast<Vector3f_handle_function>(&Material::emissive_color_handle);
  s_prototype->add_field_info(new SF_vector3f(EMISSIVE_COLOR,
                                              "emissiveColor",
                                              Field_info::RULE_EXPOSED_FIELD,
                                              emissive_color_func,
                                              s_def_emissive_color, exec_func));

  // shininess
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Float_handle_function shininess_func =
    static_cast<Float_handle_function>(&Material::shininess_handle);
  s_prototype->add_field_info(new SF_float(SHININESS,
                                           "shininess",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           shininess_func,
                                           s_def_shininess, exec_func));

  // transparency
  exec_func = static_cast<Execution_function>(&Material::material_changed);
  Float_handle_function transparency_func =
    static_cast<Float_handle_function>(&Material::transparency_handle);
  s_prototype->add_field_info(new SF_float(TRANSPARENCY,
                                           "transparency",
                                           Field_info::RULE_EXPOSED_FIELD,
                                           transparency_func,
                                           s_def_transparency, exec_func));
}

//! \brief deletes the prototype node.
void Material::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype. Initialize as nessary.
Container_proto* Material::get_prototype()
{
  if (!s_prototype) Material::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Material::set_attributes(Element* elem)
{
  Container::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "ambientIntensity") {
      set_ambient_intensity(boost::lexical_cast<float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "ambientColor") {
      Vector3f col(value);
      set_ambient_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "diffuseColor") {
      Vector3f col(value);
      set_diffuse_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "specularColor") {
      Vector3f col(value);
      set_specular_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "emissiveColor") {
      Vector3f col(value);
      set_emissive_color(col);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "shininess") {
      set_shininess(boost::lexical_cast<float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "transparency") {
      set_transparency(boost::lexical_cast<float>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
//! Get the attributes of the box.
Attribute_list Material::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  char buf[32];
  Vector3f col;

  attribs = Container::get_attributes();

  if (m_ambient_intensity != s_def_ambient_intensity) {
    attrib.first = "ambientIntensity";
    sprintf(buf, "%g",   get_ambient_intensity());
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_diffuse_color != s_def_diffuse_color) {
    attrib.first = "diffuseColor";
    get_diffuse_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_specular_color != s_def_specular_color) {
    attrib.first = "specularColor";
    get_specular_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_emissive_color != s_def_emissive_color) {
    attrib.first = "emissiveColor";
    get_emissive_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_shininess != s_def_shininess) {
    attrib.first = "shininess";
    sprintf(buf, "%g", (float)get_shininess());
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_transparency != s_def_transparency) {
    attrib.first = "transparency";
    sprintf(buf, "%f", (float)get_transparency());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs;
}

#endif

SGAL_END_NAMESPACE
