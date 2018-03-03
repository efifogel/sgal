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

#if defined(_WIN32)
#define NOMINMAX 1
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/Sphere.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Sphere::s_tag = "Sphere";
Container_proto* Sphere::s_prototype(nullptr);

// Default values:
const Vector3f Sphere::s_def_center(0,0,0);
const Float Sphere::s_def_radius(1);
const Uint Sphere::s_def_stacks(16);
const Uint Sphere::s_def_slices(16);

REGISTER_TO_FACTORY(Sphere, "Sphere");

//! \brief constructor.
Sphere::Sphere(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_sphere(nullptr),
  m_center(s_def_center),
  m_radius(s_def_radius),
  m_stacks(s_def_stacks),
  m_slices(s_def_slices)
{}

//! \brief destructor.
Sphere::~Sphere() { gluDeleteQuadric(m_sphere); }

//! \brief drawss the sphere.
void Sphere::draw(Draw_action* /* action */)
{
  if (is_dirty()) init();

  if (has_scale()) glEnable(GL_NORMALIZE);

  glPushMatrix();
  glTranslatef(m_center[0], m_center[1], m_center[2]);
  // When texture mapping is applied the orientation of the sphere is
  // significant. Rotate it 90 defrees to properly orient the sphere.
  glRotatef(-90, 1, 0, 0);
  gluQuadricTexture(m_sphere, (do_generate_tex_coord()) ? GL_TRUE : GL_FALSE);
  gluSphere(m_sphere, m_radius, m_slices, m_stacks);
  glPopMatrix();

  glDisable(GL_NORMALIZE);
}

//! \brief draws the object in selection mode.
void Sphere::isect(Isect_action* /* action */)
{
  if (is_dirty()) init();
  glPushMatrix();
  glTranslatef(m_center[0], m_center[1], m_center[2]);
  gluSphere(m_sphere, m_radius, m_stacks, m_slices);
  glPopMatrix();
}

//! Initialize the quadric object.
void Sphere::init()
{
  m_sphere = gluNewQuadric();
  gluQuadricOrientation(m_sphere, GLU_OUTSIDE);
  gluQuadricNormals(m_sphere, GLU_SMOOTH);
  gluQuadricDrawStyle(m_sphere, GLU_FILL);
  m_dirty = false;
}

/*! \brief cleans the sphere bound of the sphere.
 * Note that the sphere bound is identical to the sphere itself.
 */
void Sphere::clean_bounding_sphere()
{
  m_bounding_sphere.set_radius(m_radius);
  m_bounding_sphere.set_center(m_center);
  m_dirty_bounding_sphere = false;
}

//! \brief sets the attributes of this container.
void Sphere::set_attributes(Element * elem)
{
  Geometry::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "center") {
      Vector3f center(value);
      set_center(center);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "radius") {
      set_radius(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "stacks") {
      set_stacks(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "slices") {
      set_slices(boost::lexical_cast<Uint>(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief initilalizes the prototype object in the class.
void Sphere::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  // Bounding sphere changed
  auto exec_func =
    static_cast<Execution_function>(&Geometry::bounding_sphere_changed);
  auto radius_func = static_cast<Float_handle_function>(&Sphere::radius_handle);
  s_prototype->add_field_info(new SF_float(RADIUS, "radius",
                                           Field_rule::RULE_EXPOSED_FIELD,
                                           radius_func,
                                           exec_func));

  // Rendering required
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  auto stacks_func = static_cast<Uint_handle_function>(&Sphere::stacks_handle);
  s_prototype->add_field_info(new SF_uint(STACKS, "stacks",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          stacks_func));

  auto slices_func = static_cast<Uint_handle_function>(&Sphere::slices_handle);
  s_prototype->add_field_info(new SF_uint(SLICES, "slices",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          slices_func));
}

//! \brief deletes the sphere prototype.
void Sphere::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the sphere prototype.
Container_proto* Sphere::get_prototype()
{
  if (s_prototype == nullptr) Sphere::init_prototype();
  return s_prototype;
}

SGAL_END_NAMESPACE
