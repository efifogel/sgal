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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Sphere.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Node.hpp"

SGAL_BEGIN_NAMESPACE

std::string Sphere::s_tag = "Sphere";
Container_proto * Sphere::s_prototype = 0;

// Default values:
const Vector3f Sphere::s_def_center(0,0,0);
const Float Sphere::s_def_radius(1);
const Uint Sphere::s_def_stacks(16);
const Uint Sphere::s_def_slices(16);

REGISTER_TO_FACTORY(Sphere, "Sphere");

/*! Constructor */
Sphere::Sphere(Boolean proto) :
  Geometry(proto),
  m_dirty(SGAL_TRUE),
  m_sphere(NULL),
  m_center(s_def_center),
  m_radius(s_def_radius), 
  m_stacks(s_def_stacks),
  m_slices(s_def_slices)
{
}

/*! Destructor */
Sphere::~Sphere()
{
  gluDeleteQuadric(m_sphere);
}

/*! Draws the sphere
 * @param draw_action the draw action
 */
void Sphere::draw(Draw_action* /* action */) 
{
  if (is_dirty()) init();

  if (has_scale()) glEnable(GL_NORMALIZE);

  glPushMatrix();
  glTranslatef(m_center[0], m_center[1], m_center[2]);
  // glRotatef(-90, 1, 0, 0);

  gluQuadricTexture(m_sphere, GL_TRUE);
  gluSphere(m_sphere, m_radius, m_slices, m_stacks);
  glPopMatrix();

  glDisable(GL_NORMALIZE);
}

/*! Draws the object in selection mode 
 * @param action
 */
void Sphere::isect(Isect_action* /* action */) 
{
  if (is_dirty()) init();
  glPushMatrix();
  glTranslatef(m_center[0], m_center[1], m_center[2]);
  gluSphere(m_sphere, m_radius, m_stacks, m_slices);
  glPopMatrix();
}

/*! Initialize the quadric object */
void Sphere::init()
{
  m_sphere = gluNewQuadric();
  gluQuadricOrientation(m_sphere, GLU_OUTSIDE);
  gluQuadricNormals(m_sphere, GLU_SMOOTH); 
  gluQuadricDrawStyle(m_sphere, GLU_FILL); 
  m_dirty = SGAL_FALSE;
}

/*! Calculare the sphere bound of the sphere. In this case, the sphere
 * bound is identical to the sphere itself.
 */
Boolean Sphere::calculate_sphere_bound()
{
  if (m_is_sphere_bound_dirty) {
    m_sphere_bound.set_radius(m_radius);
    m_sphere_bound.set_center(m_center);
    return SGAL_TRUE;
  }
  return SGAL_FALSE;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Sphere::set_attributes(Element * elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++) {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "center") {
      Vector3f center(value);
      m_center = center;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "radius") {
      set_radius(atoff(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stacks") {
      set_stacks(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "slices") {
      set_slices(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    } 
  }

  // Remove all the deleted attributes:
  elem->delete_marked();

  calculate_sphere_bound();
}

#if 0
/*! Get a list of attributes for the object.
 * The list is of name=value pairs.
 * @return a list of pairs of strings
 */
Attribute_list Sphere::get_attributes() 
{ 
  Attribute_list attribs; 
  // attribs = Geometry::get_attributes();
  Attribue attrib;
  char buf[32];

  attribs = Geometry::get_attributes();

  if (m_radius != m_def_radius) {
    attrib.first = "radius";
    sprintf(buf, "%g", get_radius());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_slices != m_def_slices)
  {
    attrib.first = "slices";
    sprintf(buf, "%d", get_slices());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_stacks != m_def_stacks)
  {
    attrib.first = "stacks";
    sprintf(buf, "%d", get_stacks());
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

/*! initilalizes the prototype object in the class */
void Sphere::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  //! type definition of a container execution function - used with engines
  typedef void (Container::* Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Node::set_sphere_bound_modified);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           get_member_offset(&m_radius),
                                           exec_func));
   
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          get_member_offset(&m_stacks),
                                          exec_func));

  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          get_member_offset(&m_slices),
                                          exec_func));
}

/*! Delete the node prototype */
void Sphere::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! Obtain the node prototype */
Container_proto * Sphere::get_prototype() 
{  
  if (s_prototype == NULL) Sphere::init_prototype();
  return s_prototype;
} 

SGAL_END_NAMESPACE
