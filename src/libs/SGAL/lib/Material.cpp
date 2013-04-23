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
// $Revision: 12384 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
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
#include "SGAL/Formatter.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Material::s_tag = "Material";
Container_proto* Material::s_prototype = 0;

// Default values:
Float Material::m_def_ambient_intensity = 0.2f;
Vector3f Material::m_def_diffuse_color(0.8f, 0.8f,0.8f);
Vector3f Material::m_def_emissive_color(0, 0, 0);
Float Material::m_def_shininess = 0.2f;
Vector3f Material::m_def_specular_color(0, 0, 0);
Float Material::m_def_transparency = 0;

REGISTER_TO_FACTORY(Material, "Material");

/*! Constructor */
Material::Material(Boolean proto) :
  Container(proto),
  m_ambient_intensity(m_def_ambient_intensity),
  m_diffuse_color(m_def_diffuse_color),
  m_specular_color(m_def_specular_color),
  m_emissive_color(m_def_emissive_color),
  m_shininess(m_def_shininess),
  m_transparency(m_def_transparency)
{}

/*! Destructor */
Material::~Material() {}

/*! \brief sets the transparency. */
void Material::set_transparency(Float transparency)
{
  Field_info* field_info = get_field_info(TRANSPARENCY);
  field_changed(field_info);
  m_transparency = transparency;
}

/*! \brief sets the ambient intensity. */
void Material::set_ambient_intensity(Float intensity)
{ m_ambient_intensity = intensity; }

/*! \brief sets the diffuse color. */
void Material::set_diffuse_color(Float v0, Float v1, Float v2)
{
  m_diffuse_color[0] = v0;
  m_diffuse_color[1] = v1;
  m_diffuse_color[2] = v2;
}

/*! \brief sets the specular color. */
void Material::set_specular_color(Float v0, Float v1, Float v2)
{
  m_specular_color[0] = v0;
  m_specular_color[1] = v1;
  m_specular_color[2] = v2;
}

/*! \brief sets the amissive color. */
void Material::set_emissive_color(Float v0, Float v1, Float v2)
{
  m_emissive_color[0] = v0;
  m_emissive_color[1] = v1;
  m_emissive_color[2] = v2;
}

/*! \brief sets the shininess factor. */
void Material::set_shininess(Float shininess)
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

  ambient.scale(m_ambient_intensity, m_diffuse_color);

  ambient[3] = diffuse[3] = specular[3] = emissive[3] = 1.0f - m_transparency;

  glMaterialfv(face, GL_AMBIENT, (Float *)&ambient);
  glMaterialfv(face, GL_DIFFUSE, (Float *)&diffuse);
  glMaterialfv(face, GL_SPECULAR, (Float *)&specular);
  glMaterialfv(face, GL_EMISSION, (Float *)&emissive);
  glMaterialf(face, GL_SHININESS, 128 * m_shininess);
}

/*! \brief the callback invoked when the material chages. */
void Material::material_changed(Field_info* field_info)
{
  set_rendering_required(field_info);
  field_changed(field_info);
}

/*! \brief initilalizes the prototype of this node. */
void Material::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance
  //! \todo s_prototype = new Container_proto(Geometry::get_prototype());
  s_prototype = new Container_proto();

  // Add the object fields to the prototype
  Execution_function exec_func =
    static_cast<Execution_function>(&Material::material_changed);
  s_prototype->add_field_info
    (new SF_float(AMBIENT_INTENSITY, "ambientIntensity",
                  get_member_offset(&m_ambient_intensity),
                  exec_func));

  exec_func = static_cast<Execution_function>(&Material::material_changed);
  s_prototype->
    add_field_info(new SF_vector3f(DIFFUSE_COLOR, "diffuseColor",
                                   get_member_offset(&m_diffuse_color),
                                   exec_func));

  exec_func = static_cast<Execution_function>(&Material::material_changed);
  s_prototype->add_field_info
    (new SF_vector3f(SPECULAR_COLOR, "specularColor",
                     get_member_offset(&m_specular_color),
                     exec_func));

  exec_func = static_cast<Execution_function>(&Material::material_changed);
  s_prototype->
    add_field_info(new SF_vector3f(EMISSIVE_COLOR, "emissiveColor",
                                   get_member_offset(&m_emissive_color),
                                   exec_func));

  exec_func = static_cast<Execution_function>(&Material::material_changed);
  s_prototype->add_field_info(new SF_float(SHININESS, "shininess",
                                           get_member_offset(&m_shininess),
                                           exec_func));

  exec_func = static_cast<Execution_function>(&Material::material_changed);
  s_prototype->add_field_info(new SF_float(TRANSPARENCY, "transparency",
                                           get_member_offset(&m_transparency),
                                           exec_func));
}

/*! \brief deletes the prototype node. */
void Material::delete_prototype()
{
  delete s_prototype;
  s_prototype = 0;
}

/*! \brief obtains the prototype. Initialize as nessary. */
Container_proto* Material::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of this object. */
void Material::set_attributes(Element* elem) 
{
  Container::set_attributes(elem);
  typedef Element::Str_attr_iter                Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "ambientIntensity") {
      set_ambient_intensity((Float) atof(value.c_str()));
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
      set_shininess((Float) atof(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "transparency") {
      set_transparency((Float) atof(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief writes this container. */
void Material::write(Formatter* formatter)
{
  formatter->container_begin(get_tag());
  formatter->single_vector3f("diffuseColor",
                             m_diffuse_color, m_def_diffuse_color);
  formatter->container_end();  
}

#if 0
/*! Get the attributes of the box. */
Attribute_list Material::get_attributes() 
{ 
  Attribute_list attribs; 
  Attribue attrib;
  char buf[32];
  Vector3f col;

  attribs = Container::get_attributes();

  if (m_ambient_intensity != m_def_ambient_intensity) {
    attrib.first = "ambientIntensity";
    sprintf(buf, "%g",   get_ambient_intensity());
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_diffuse_color != m_def_diffuse_color) {
    attrib.first = "diffuseColor";
    get_diffuse_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_specular_color != m_def_specular_color) {
    attrib.first = "specularColor";
    get_specular_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_emissive_color != m_def_emissive_color) {
    attrib.first = "emissiveColor";
    get_emissive_color(col);
    attrib.second = col.get_text();
    attribs.push_back(attrib);
  }
  if (m_shininess != m_def_shininess) {
    attrib.first = "shininess";
    sprintf(buf, "%g", (Float)get_shininess());
    attrib.second = buf;
    attribs.push_back(attrib);
  }
  if (m_transparency != m_def_transparency) {
    attrib.first = "transparency";
    sprintf(buf, "%f", (Float)get_transparency());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs; 
}

#endif

SGAL_END_NAMESPACE
