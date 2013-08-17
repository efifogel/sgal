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
// $Id: Sphere_plane_intersection.cpp 7204 2009-01-24 21:43:15Z efif $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 *
 * Computes the intersection of a sphere and a plane
 */

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Sphere_plane_intersection.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Sphere_plane_intersection::s_tag = "SpherePlaneIntersection";
Container_proto* Sphere_plane_intersection::s_prototype(NULL);

// Defaults values:
  // Default values:
const Float Sphere_plane_intersection::s_def_sphere_radius(1);
const Vector4f Sphere_plane_intersection::s_def_plane(0, 0, 1, 0);

REGISTER_TO_FACTORY(Sphere_plane_intersection, "Sphere_plane_intersection");

/*! \brief constructor */
Sphere_plane_intersection::Sphere_plane_intersection(Boolean proto) :
  Node(proto),
  m_sphere_radius(s_def_sphere_radius),
  m_plane(s_def_plane),
  m_trigger(false)
{}

/*! \brief destructor */
Sphere_plane_intersection::~Sphere_plane_intersection() {}

/*! \brief initializes the container prototype */
void Sphere_plane_intersection::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Sphere_plane_intersection::execute);

  // sphereRadius
  s_prototype->add_field_info(new SF_float(SPHERE_RADIUS, "sphereRadius",
                                           get_member_offset(&m_sphere_radius),
                                           exec_func));

  // plane
  s_prototype->add_field_info(new SF_vector4f(PLANE, "plane",
                                           get_member_offset(&m_plane),
                                           exec_func));

  // trigger
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          get_member_offset(&m_trigger),
                                          exec_func));

  // circleTranslation
  s_prototype->
    add_field_info(new SF_vector3f(CIRCLE_TRANSLATION, "circleTranslation",
                                   get_member_offset(&m_circle_translation)));

  // circleRotation
  s_prototype->
    add_field_info(new SF_rotation(CIRCLE_ROTATION, "circleRotation",
                                   get_member_offset(&m_circle_rotation)));

  // circleRadius
  s_prototype->
    add_field_info(new SF_float(CIRCLE_RADIUS, "circleRadius",
                                get_member_offset(&m_circle_radius)));
}

/*! \brief deletes the container prototype */
void Sphere_plane_intersection::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto* Sphere_plane_intersection::get_prototype()
{
  if (!s_prototype) Sphere_plane_intersection::init_prototype();
  return s_prototype;
}

/*! \brief sets the attributes of the object extracted from the input file. */
void Sphere_plane_intersection::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;
  typedef Element::Cont_attr_iter         Cont_attr_iter;

  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "sphereRadius") {
      m_sphere_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "plane") {
      Vector4f plane(value);
      set_plane(plane);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "circleTranslation") {
      Vector3f vec(value);
      m_circle_translation = vec;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "circleRotation") {
      Rotation rot(value);
      m_circle_rotation = rot;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "circleRadius") {
      m_circle_radius = boost::lexical_cast<Float>(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "trigger") {
      m_trigger = boost::lexical_cast<Boolean>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief executes the engine */
void Sphere_plane_intersection::execute(Field_info* /*! field_info */)
{
  Field* circle_trans = get_field(CIRCLE_TRANSLATION);
  Field* circle_rot = get_field(CIRCLE_ROTATION);
  Field* circle_rad = get_field(CIRCLE_RADIUS);

  float a = m_plane[0];
  float b = m_plane[1];
  float c = m_plane[2];
  float d = m_plane[3];

  float length_square = a*a + b*b + c*c;
  float length_reciprocal = 1.0f / sqrtf(length_square);
  float radius_sqaure = m_sphere_radius * m_sphere_radius;
  m_circle_radius =
    sqrtf(radius_sqaure * length_square - d*d) * length_reciprocal;
  float x = a * length_reciprocal;
  float y = b * length_reciprocal;
  float z = c * length_reciprocal;
  Vector3f normal(x, y, z);
  Vector3f ref(0, 1, 0);

  m_circle_rotation.make(ref, normal);
  m_circle_translation.set(0, -d * length_reciprocal, 0);

  if (circle_trans != NULL) circle_trans->cascade();
  if (circle_rot != NULL) circle_rot->cascade();
  if (circle_rad != NULL) circle_rad->cascade();
}

SGAL_END_NAMESPACE
