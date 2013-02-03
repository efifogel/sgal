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
// $Revision: 12554 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <iostream>
#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Formatter.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Configuration.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Shape::s_tag = "Shape";
Container_proto* Shape::s_prototype = 0;

// Default values:
const Vector2f Shape::s_def_depth_range(0, 1);
const Vector4ub Shape::s_def_color_mask(0xff, 0xff, 0xff, 0xff);
const Gfx::Depth_func Shape::s_def_depth_function(Gfx::LESS_DFUNC);
const Gfx::Cull_face Shape::s_def_cull_face(Gfx::NO_CULL);

const Char* Shape::s_depth_function_names[] = {
  "never", "less", "equal", "lequal", "greater", "notequal", "gequal", "always"
};

const Char* Shape::s_cull_face_names[] = { "no", "front", "back", "both" };

REGISTER_TO_FACTORY(Shape, "Shape");

/* Constructor */
Shape::Shape(Boolean proto) :
  Node(proto), 
  m_draw_depth(true),
  m_test_depth(true),
  m_depth_range(s_def_depth_range),
  m_depth_function(s_def_depth_function),
  m_color_mask(s_def_color_mask),
  m_cull_face(s_def_cull_face),
  m_appearance(NULL),
  m_geometry(NULL),
  m_is_visible(true),
  m_owned_tex_gen(false),
  m_is_text_object(false),
  m_priority(0),
  m_draw_backface(false),
  m_owned_appearance(false),
  m_dirty(true),
  m_dirty_appearance(true),
  m_dirty_geometry(true),
  m_appearance_prev(NULL),
  m_geometry_prev(NULL),
  m_override_light_enable(Configuration::s_def_override_light_enable),
  m_override_blend_func(Configuration::s_def_override_blend_func),
  m_override_tex_gen(Configuration::s_def_override_tex_gen)
{}

/*! Destructor */
Shape::~Shape()
{
  if (m_owned_appearance) {
    if (m_appearance) {
      delete m_appearance;
      m_appearance = NULL;
    }
    m_owned_appearance = false;
  }
}

/*! \brief sets the appearance of the object. */
void Shape::set_appearance(Appearance* app)
{
  m_appearance = app;
  m_dirty = true;
  m_dirty_appearance = true;
}

/*! \brief adds a geometry to the shape at the end of the list. */
void Shape::set_geometry(Geometry* geometry)
{
  m_geometry = geometry;
  m_dirty = true;
  m_dirty_geometry = true;
  
#if 0
  //! \todo
  // the text object needs to pass a pointer to the appearance 
  // for the FontTexture object
  Text* text = dynamic_cast<Text*>(geometry);
  if (text) {
    text->set_appearance(m_appearance);
    m_is_text_object = true;
  }
#endif
}

/*! \brief calculates the bounding sphere of all geometries in the shape.
 * \return true if the bounding sphere has changed since last call.
 */
Boolean Shape::clean_sphere_bound()
{
  if (!is_visible()) {
    if (m_sphere_bound.get_radius() == 0) return false;
    m_sphere_bound.set_radius(0);
    return true;
  }

  Boolean changed = false;
  if (m_geometry)
    m_sphere_bound = *m_geometry->get_sphere_bound(changed);

  return changed;
}

/*! \brief determines whether the geometry is text. */
Boolean Shape::is_text_object() { return m_is_text_object; }

/*! \brief draws the appearance and then all the geometries. */
Action::Trav_directive Shape::draw(Draw_action* draw_action)
{
  if (!m_geometry || !is_visible()) return Action::TRAV_CONT;
  if (m_dirty) clean();

  int pass_no = draw_action->get_pass_no();
  Boolean is_transparent = m_appearance->is_transparent() || is_text_object();
  if (!m_draw_backface && is_transparent && (pass_no == 0)) {
    draw_action->set_second_pass_required(true);
    return Action::TRAV_CONT;
  }
  
  m_appearance->draw(draw_action);
  draw_geometry(draw_action);

  return Action::TRAV_CONT;
}

/*! \brief culls the node if invisible and prepare for rendering. */
void Shape::cull(Cull_context& cull_context) { cull_context.add_shape(this); }

/*! \brief draws the geometry. */
void Shape::draw_geometry(Draw_action* action) 
{
  if (m_geometry) {
    Context* context = action->get_context();
    if (!m_draw_depth) context->draw_depth_mask(false);
    if (!m_test_depth) context->draw_depth_enable(false);
    if (m_depth_range != s_def_depth_range)
      glDepthRange(m_depth_range[0], m_depth_range[1]);
    if (m_depth_function != s_def_depth_function)
      context->draw_depth_func(m_depth_function);
    if (m_color_mask != s_def_color_mask)
      context->draw_color_mask(m_color_mask);
    if (m_cull_face != s_def_cull_face)
      context->draw_cull_face(m_cull_face);

    m_geometry->draw(action);

    if (m_cull_face != s_def_cull_face)
      context->draw_cull_face(s_def_cull_face);
    if (m_color_mask != s_def_color_mask)
      context->draw_color_mask(s_def_color_mask);
    if (m_depth_function != s_def_depth_function)
      context->draw_depth_func(s_def_depth_function);
    if (m_depth_range != s_def_depth_range)
      glDepthRange(s_def_depth_range[0], s_def_depth_range[1]);
    if (!m_test_depth) context->draw_depth_enable(true);
    if (!m_draw_depth) context->draw_depth_mask(true);  }
}

/*! \brief draws the shape in a compact way that includes only rendering the 
 * geometry and not the appearance. The color used is an identifier
 * of the touch sensor that is attaced to the shape. We assume there are 
 * no more than 256 touch sensors and we use the 4 most significant bits 
 * of the red and green to set the color.
 * @param isect_action
 */
void Shape::isect(Isect_action* isect_action)
{  
  if (!is_visible()) return;
  if (!m_geometry) return;

  Uint id = isect_action->get_id();
  if (id != 0) {
    Uint rgb[3];
    isect_action->get_color(id, rgb);
    glColor3ui(rgb[0], rgb[1], rgb[2]);
  } else {
    glColor3ui(0, 0, 0);
  }

  m_geometry->isect(isect_action);
}

/*! \brief cleans the apperances. */
void Shape::clean()
{
  if (!m_dirty_appearance && !m_dirty_geometry) {
    m_dirty = false;
    return;
  }

  if (m_dirty_appearance) clean_appearance();

  // If the geometry has no color coordinates, enabled the light by default.
  if (m_override_light_enable)
    if (!m_geometry->are_generated_color())
      m_appearance->set_light_enable(true);

  if (m_override_blend_func) {
    /*! Text geometry are transparent by default. */
    Boolean is_transparent =
      m_appearance->is_transparent() || is_text_object();
    if (is_transparent) {
      m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
      m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    }
    else {
      m_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
      m_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
    }
  }

  if (m_override_tex_gen) {
    // Enable texture generation if texture is enabled, and the geometry does
    // not have a texture-coordinate array.
    if (m_appearance->get_tex_enable() &&
        !m_geometry->are_generated_tex_coord())
    {
      m_appearance->set_tex_gen_enable(true);
      clean_tex_gen();
    }
  }
  
  if (m_dirty_appearance) {
    m_appearance_prev = m_appearance;
    m_dirty_appearance = false;
  }
  
  if (m_dirty_geometry) {
    Observer observer(this, get_field_info(SPHERE_BOUND));
    if (m_geometry_prev) m_geometry_prev->unregister_observer(observer);
    if (m_geometry) m_geometry->register_observer(observer);
    m_geometry_prev = m_geometry;
    m_dirty_geometry = false;
  }
    
#if 0
  //! \todo
  Text* text = dynamic_cast<Text*>(m_geometry);
  if (text) {
    text->set_appearance(m_appearance);
    m_is_text_object = true;
  }
#endif
  
  m_dirty = false;
}

/*! breif cleans the apperance. */
void Shape::clean_appearance()
{
  // Construct a new owned appearance if needed, and delete the previously
  // constructed owned appearance if not needed any more.
  if (m_owned_appearance) {
    if (!m_appearance) m_appearance = m_appearance_prev;
    else if (m_appearance != m_appearance_prev) {
      delete m_appearance_prev;
      m_appearance_prev = NULL;
      m_owned_appearance = false;
    }
  }
  else {
    if (!m_appearance) {
      m_appearance = new Appearance;
      SGAL_assertion(m_appearance);
      m_owned_appearance = true;
    }
  }
}

/*! \brief cleans the texture generation attribute. */
void Shape::clean_tex_gen()
{
  // Construct a new owned texture generation attribute if needed, and delete
  // the previously constructed owned texture generation attribute if not
  // needed any more.
  SGAL_assertion(m_appearance);
  if (m_owned_tex_gen) {
    SGAL_assertion(m_appearance_prev);
    Tex_gen* tex_gen_prev = m_appearance_prev->get_tex_gen();
    Tex_gen* tex_gen = m_appearance->get_tex_gen();
    if (!tex_gen) m_appearance->set_tex_gen(tex_gen_prev);
    else if (tex_gen != tex_gen_prev) {
      delete tex_gen_prev;
      m_appearance_prev->set_tex_gen(NULL);
      m_owned_tex_gen = false;
    }
  }
  else {
    if (!m_appearance->get_tex_gen()) {
      Tex_gen* tex_gen = new Tex_gen();
      SGAL_assertion(tex_gen);
      m_appearance->set_tex_gen(tex_gen);
      m_owned_tex_gen = true;
    }
  }

  // Setup the textute-generation functions.
  Texture* texture = m_appearance->get_texture();
  if (dynamic_cast<Texture_2d*>(texture)) {
    // Setup standard texture map if requested:
#if 0
    m_appearance->get_tex_gen()->set_mode_s(Tex_gen::EYE_LINEAR);
    m_appearance->get_tex_gen()->set_mode_t(Tex_gen::EYE_LINEAR);
#else
    m_appearance->get_tex_gen()->set_mode_s(Tex_gen::OBJECT_LINEAR);
    m_appearance->get_tex_gen()->set_mode_t(Tex_gen::OBJECT_LINEAR);
#endif
  }
  else if (dynamic_cast<Sphere_environment*>(texture)) {
    // Setup sphere environment map if requested:
    m_appearance->get_tex_gen()->set_mode_s(Tex_gen::SPHERE_MAP);
    m_appearance->get_tex_gen()->set_mode_t(Tex_gen::SPHERE_MAP);
  }
  else if (dynamic_cast<Cube_environment*>(texture)) {
    // Setup cube environment map if requested:
#if 0
    m_appearance->get_tex_gen()->set_mode_s(Tex_gen::NORMAL_MAP);
    m_appearance->get_tex_gen()->set_mode_t(Tex_gen::NORMAL_MAP);
    m_appearance->get_tex_gen()->set_mode_r(Tex_gen::NORMAL_MAP);
#else
    m_appearance->get_tex_gen()->set_mode_s(Tex_gen::REFLECTION_MAP);
    m_appearance->get_tex_gen()->set_mode_t(Tex_gen::REFLECTION_MAP);
    m_appearance->get_tex_gen()->set_mode_r(Tex_gen::REFLECTION_MAP);
#endif
  }
}

/*! \brief sets the attributes of the shape */
void Shape::set_attributes(Element* elem)
{
  typedef Element::Str_attr_iter          Str_attr_iter;
  Node::set_attributes(elem);
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "cullFace") {
      Uint num = sizeof(s_cull_face_names) / sizeof(char*);
      const char** found = std::find(s_cull_face_names,
                                     &s_cull_face_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_cull_face_names;
      if (index < num)
        m_cull_face = static_cast<Gfx::Cull_face>(index);
      elem->mark_delete(ai);
      continue;
    }    
    if (name == "depthFunction") {
      Uint num = sizeof(s_depth_function_names) / sizeof(char*);
      const char** found = std::find(s_depth_function_names,
                                      &s_depth_function_names[num],
                                      strip_double_quotes(value));
      Uint index = found - s_depth_function_names;
      if (index < num)
        m_depth_function = static_cast<Gfx::Depth_func>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "visible") {
      if (!compare_to_true(value)) set_invisible();
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawDepth") {
      m_draw_depth = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "testDepth") {
      m_test_depth = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "depthRange") {
      m_depth_range = Vector2f(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "colorMask") {
      m_color_mask = Vector4ub(value);
      elem->mark_delete(ai);
      continue;
    }    
    if (name == "priority") {
      set_priority(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "drawBackface") {
      m_draw_backface = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter         Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "appearance") {
      Appearance* app = dynamic_cast<Appearance*>(cont);
      set_appearance(app);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "geometry") {
      Geometry* geo = dynamic_cast<Geometry*>(cont);
      set_geometry(geo);
      elem->mark_delete(cai);      
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

#if 0
/*! \brief Obtain a list of attributes (called in the save process). */
Attribute_list Shape::get_attributes() 
{ 
  Attribute_list attrs;
  attrs = Node::get_attributes();
  Attribue attrib;

  if (m_is_visible != true) {
    attrib.first = "visible";
    attrib.second = "FALSE";
    attrs.push_back(attrib);
  }

  return attrs; 
}
#endif

/*! \brief writes this container. */
void Shape::write(Formatter* formatter)
{
  formatter->container_begin(get_tag());

  formatter->single_container_begin("appearance");
  formatter->write(m_appearance);
  formatter->single_container_end();

  formatter->single_container_begin("geometry");
  formatter->write(m_geometry);
  formatter->single_container_end();

  formatter->container_end();
}

/*! \brief initializes the node prototype. */
void Shape::init_prototype()
{
  if (s_prototype) return;

  // Allocate a prototype instance:
  s_prototype = new Container_proto(Node::get_prototype());

  Execution_function exec_func = 
    static_cast<Execution_function>(&Node::sphere_bound_changed);
  s_prototype->add_field_info(new SF_bool(ISVISIBLE, "visible",
                                          get_member_offset(&m_is_visible),
                                          exec_func));    

  exec_func = static_cast<Execution_function>(&Shape::geometry_changed);
  s_prototype->add_field_info(new SF_container(GEOMETRY, "geometry",
                                               get_member_offset(&m_geometry),
                                               exec_func));    

  exec_func = static_cast<Execution_function>(&Shape::appearance_changed);
  s_prototype->add_field_info(new SF_container(APPEARANCE, "appearance",
                                               get_member_offset(&m_appearance),
                                               exec_func));    
}

/*! \brief deletes the node prototype. */
void Shape::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Shape::get_prototype() 
{  
  if (s_prototype == NULL) Shape::init_prototype();
  return s_prototype;
}

/*! \brief */
Boolean Shape::attach_context(Context* context)
{
  Boolean result = Node::attach_context(context);
  Appearance* app = get_appearance();
  if (app) result &= app->attach_context(context);  
  return result;
}

/*! \brief */
Boolean Shape::detach_context(Context* context)
{
  Boolean result = Node::detach_context(context);
  Appearance* app = get_appearance();
  if (app) result &= app->detach_context(context);
  return result;
}

/*! \brief processes change of appearance. */
void Shape::appearance_changed(Field_info* /* field_info. */)
{
  m_dirty = true;
  m_dirty_appearance = true;
}

/*! \brief processes change of geometry. */
void Shape::geometry_changed(Field_info* /* field_info. */)
{
  m_dirty = true;
  m_dirty_geometry = true;
}

SGAL_END_NAMESPACE
