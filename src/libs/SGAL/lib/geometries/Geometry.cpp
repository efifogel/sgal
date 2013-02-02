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
// $Source: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geometry::s_prototype = 0;

/*! Constructor */
Geometry::Geometry(Boolean proto) :
  Container(proto),
  m_dirty_sphere_bound(true),
  m_bb_is_pre_set(false)
{}

/*! \brief initializes the geometry prototype. */
void Geometry::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  // Execution_function exec_func;
  SF_sphere_bound* sphere_bound_fi;
  SF_bool* bool_field;
  
  // exec_func = static_cast<Execution_function>(&Transform::parts_changed);
  sphere_bound_fi = new SF_sphere_bound(SPHERE_BOUND, "sphereBound",
                                        get_member_offset(&m_sphere_bound));
  s_prototype->add_field_info(sphere_bound_fi);

  bool_field = new SF_bool(GENERATED_TEX_COORD, "generatedTexCoord",
                           get_member_offset(&m_generated_tex_coord));
  s_prototype->add_field_info(bool_field);
}

/*! \brief deletes the geometry prototype. */
void Geometry::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the geometry prototype. */
Container_proto* Geometry::get_prototype() 
{  
  if (!s_prototype) Geometry::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the geometric object. */
void Geometry::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;

  Container::set_attributes(elem);

  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "bboxCenter") {
      m_sphere_bound.set_center(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bboxSize") {
      Vector3f vec(value);
      float radius = vec.length();
      m_sphere_bound.set_radius(radius);
      m_bb_is_pre_set = SGAL_TRUE;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "generatedTexCoord") {
      set_generated_tex_coord(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief sets the flag that indicates that the sphere bound should be
 * cleaned.
 */
void Geometry::sphere_bound_changed(Field_info* /* field_info */)
{ m_dirty_sphere_bound = true; }


/*! Returns a pointer to the sphere bound. 
 * changed is true if the BS has changed since last call.
 */
const Sphere_bound* Geometry::get_sphere_bound(bool& changed)
{
  changed = false;
  if (m_dirty_sphere_bound) {
    clean_sphere_bound();
    changed = true;
  }
  return &m_sphere_bound;
}

#if 0
/*! \brief obtains the attributes of the box */
Attribute_list Geometry::get_attributes() 
{ 
  Attribute_list attribs; 
  attribs = Container::get_attributes();
  return attribs;
}

/*! \brief obtains the parent node from the scene graph and add the geometry
 * to it.
 * \param sg a pointer to the scene graph.
 * \param parentName the name of the parent object.
 */
void Geometry::add_to_scene(Scene_graph* sg, XML_entity* parent) 
{ 
  Container::add_to_scene(sg, parent);
  Shape *shape = dynamic_cast<Shape *>(parent);
  if (shape)
    shape->set_geometry(this);
  
  // insert the geometry to the geometry pool 
  sg->add_container(this);
}
#endif

/*! \brief returns true if the current matrix contains scaling.
 * We get the current matrix from the matrix stack, multiply it
 * by a unit vector and check its length after the transformation.
 */
Boolean Geometry::has_scale()
{
  Vector4f v1(0.5773502f, 0.5773502f, 0.5773502f, 0.0f);
  float epsilon = 0.000001f;

  float modelview_mat[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview_mat); 
  Matrix4f m;
  m.set(modelview_mat);

  float pre_length = v1.length();
  v1.xform(v1, m);
  float post_length = v1.length();
#if 0
  std::cout << "post_length: " << post_length
            << ", pre_length: " << pre_length
            << std::endl;
#endif
  if (abs(post_length - pre_length) > epsilon) return SGAL_TRUE;
  return SGAL_FALSE;
}

SGAL_END_NAMESPACE
