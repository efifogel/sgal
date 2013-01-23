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

SGAL_BEGIN_NAMESPACE

const std::string Shape::s_tag = "Shape";
Container_proto* Shape::s_prototype = 0;

// Default values:
const Boolean Shape::s_def_is_background(false);
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
  m_appearance(0),
  m_alt_appearance(0),
  m_geometry(0),
  m_is_visible(true),
  m_is_background(false),
  m_is_initialized(false),
  m_own_appearance(false),
  m_is_text_object(false),
  m_priority(0),
  m_draw_backface(false)
{}

/*! Destructor */
Shape::~Shape()
{
  if (m_own_appearance) {
    if (m_appearance) {
      delete m_appearance;
      m_appearance = NULL;
    }
    m_own_appearance = false;
  }
}

/*! \brief sets the appearance of the object.
 * @param app the appearance
 */
void Shape::set_appearance(Appearance* app)
{
  if (m_own_appearance) {
    if (m_appearance) {
      delete m_appearance;
      m_appearance = NULL;
    }
    m_own_appearance = false;
  }
  m_appearance = app;
}

/*! \brief adds a geometry to the shape at the end of the list.
 * @param geometry a pointer to the geomety added.
 */
void Shape::set_geometry(Geometry* geometry)
{
  Observer observer(this, get_field_info(SPHERE_BOUND));
  if (m_geometry) m_geometry->unregister_observer(observer);
  m_geometry = geometry;
  m_geometry->register_observer(observer);

  //! \todo
#if 0
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
 * \return true if the BS has changed since last call.
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

/*! The function returns the i-th  geometry in the list
 * @param i (in) which geometry to return
 * @return a pointer to a gepmetry. If i is greater than the number
 *  of geometries, returns 0.
 */
Geometry* Shape::get_geometry() { return m_geometry; }

/*! \brief */
Boolean Shape::is_text_object() { return m_is_text_object; }

/*! \brief draws the appearance and then all the geometries 
 * @param draw_action
 */
Action::Trav_directive Shape::draw(Draw_action* draw_action)
{
  if (!is_visible()) return Action::TRAV_CONT;
  if (!m_appearance) create_default_appearance();
  if (!m_is_initialized) init();

  // Boolean to_draw = false;
  Boolean is_transparent = false;
  // Boolean has_texture = false;
  int pass_no = draw_action->get_pass_no();

  if (m_appearance) {
    is_transparent = m_appearance->is_transparent() || is_text_object();
    // has_texture = (m_appearance->get_texture() != 0 );
  }

  if (!m_draw_backface && is_transparent && (pass_no == 0)) {
    draw_action->set_second_pass_required(true);
    return Action::TRAV_CONT;
  }

  if (m_geometry && m_appearance && !m_geometry->has_color())
    m_appearance->set_light_enable(true);

  // if ((!is_transparent && pass_no == 0) || (is_transparent && pass_no == 1))
  // {

  if (m_appearance) {
    if (is_transparent) {
      m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
      m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
    } else {
      m_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
      m_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
    }
    m_appearance->draw(draw_action);
  }
  draw_geometries(draw_action);

#if 0
  if (!m_env_map) {
    if (m_appearance) {
      if (is_transparent) {
        m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
        m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      } else {
        m_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
        m_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
      }

      m_appearance->draw(draw_action);
    }
    draw_geometries(draw_action);
  } else {
    if (has_texture || m_env_map->is_quality()) {
      if (is_transparent) {
        m_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
        m_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
        m_alt_appearance->get_material()->
          set_transparency(m_env_map->get_alpha()*
                           (1-m_appearance->get_material()->
                            get_transparency()));
      } else {
        m_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
        m_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
        m_alt_appearance->get_material()->
          set_transparency(m_env_map->get_alpha());
      }
      // double pass implementation using the stencil buffer (slower)
      // this is currently not used
      /*
      glClear(GL_STENCIL_BUFFER_BIT);
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 1, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
      m_appearance->draw(draw_action);
      draw_geometries(draw_action);
      glStencilFunc(GL_EQUAL, 1, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glColor4f(0, 0, 0, m_alt_appearance->get_material()->get_transparency());
      m_alt_appearance->draw(draw_action);
      draw_geometries(draw_action);
      glDisable(GL_STENCIL_TEST);
      */
      // double pass implementation using the depth buffer (faster)
      m_appearance->draw(draw_action);
      GLint depthFunc;
      glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
      draw_geometries(draw_action);
      glDepthFunc(GL_EQUAL);
      glColor4f(0, 0, 0, m_alt_appearance->get_material()->get_transparency());
      m_alt_appearance->draw(draw_action);
      draw_geometries(draw_action);
      glDepthFunc(depthFunc);
    } else {
      if (is_transparent) {
        m_alt_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
        m_alt_appearance->set_dst_blend_func(Gfx::ONE_MINUS_SRC_ALPHA_DBLEND);
      } else {
        m_alt_appearance->set_src_blend_func(Gfx::ONE_SBLEND);
        m_alt_appearance->set_dst_blend_func(Gfx::ZERO_DBLEND);
      }
      m_alt_appearance->draw(draw_action);
      draw_geometries(draw_action);
    }
  }
#endif
  
  return Action::TRAV_CONT;
}

/*! \brief culls the node if invisible and prepare for rendering. */
void Shape::cull(Cull_context& cull_context) { cull_context.add_shape(this); }

/*! \brief */
void Shape::draw_geometries(Draw_action* action) 
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

/*! \brief initializes the apperances in the shape according to whether texture
 * and environment mapping is to be applied on the shape.
 * There 3 cases:
 * # the object has no environment map. In this case, we do nothing. In the
 * render we call draw on the apperance and nothing else.
 * # the object has an environment mapping but no texture. In this case we
 * duplicate the original apperance. We set the texture to be the one of the
 * reflection and we make all the settings related to reflection. In the render
 * we use the new created apperance.
 * # the object has both environment mapping and texture. In this case we
 * create a new apperance that contains the texture of the environment and all
 * additional settings of the texture generation. In addition, we set the
 * blending functions according to the alpha value specified in the environment
 * object. In the render we first render with the original appearance and then
 * we render a second time with the newly created appearance.
 */
void Shape::init()
{
  m_is_initialized = true;
#if 0
  if (!m_env_map) return;

  Boolean has_texture = false;
  if (m_appearance)
    has_texture = (m_appearance->get_texture() != 0);

  if (!has_texture && !m_env_map->is_quality()) {
    m_alt_appearance = new Appearance();
    m_alt_appearance->set(m_appearance);
    m_alt_appearance->set_texture(m_env_map->get_texture());
    m_alt_appearance->set_tex_enable(true);
    Tex_gen* tex_gen = new Tex_gen();
    tex_gen->set_modes(Tex_gen::SPHERE_MAP);
    tex_gen->set_modet(Tex_gen::SPHERE_MAP);
    m_alt_appearance->set_tex_gen(tex_gen);
    m_alt_appearance->set_tex_gen_enable(true);
  } else {
    Tex_gen* tex_gen = new Tex_gen();
    tex_gen->set_modes(Tex_gen::SPHERE_MAP);
    tex_gen->set_modet(Tex_gen::SPHERE_MAP);

    m_alt_appearance = new Appearance();
    m_alt_appearance->set_tex_gen(tex_gen);
    m_alt_appearance->set_tex_gen_enable(true);
    m_alt_appearance->set_depth_func(Gfx::LEQUAL_DFUNC);
    m_alt_appearance->set_texture(m_env_map->get_texture());
    m_alt_appearance->set_tex_enable(true);
    m_alt_appearance->set_src_blend_func(Gfx::SRC_ALPHA_SBLEND);
    m_alt_appearance->set_dst_blend_func(Gfx::ONE_DBLEND);

    Material* mat = new Material();
    mat->set_diffuse_color(1, 1, 1);
    mat->set_transparency(m_env_map->get_alpha());
    mat->set_execution_coordinator(m_execution_coordinator);
    m_alt_appearance->set_material(mat);
    m_alt_appearance->set_back_material(mat);
    m_alt_appearance->set_tex_env(Gfx::DECAL_TENV);
    m_alt_appearance->set_light_enable(false);
  }

  if (m_alt_appearance) {
    m_alt_appearance->set_execution_coordinator(m_execution_coordinator);
  }

  if (m_env_map && m_alt_appearance) {
    /*
    Field* src = m_env_map->add_field(Environment_map::TEXTURE);
    Field* dst = m_alt_appearance->add_field(Appearance::TEXTURE);
    ASSERT(src);
    ASSERT(dst);
    if (src && dst) {
      src->connect(dst);
    }
    src = m_env_map->add_field(Environment_map::ALPHA);
    dst = m_alt_appearance->get_material()->add_field(Material::TRANSPARENCY);
    ASSERT(src);
    ASSERT(dst);
    if (src && dst) {
      src->connect(dst);
    }
    */
  }
#endif
}

/*! \brief */
void Shape::create_default_appearance() 
{
  m_appearance = new Appearance();
  m_own_appearance = true;
  m_is_initialized = true;

  //! \todo
#if 0
  Text* text = dynamic_cast<Text*>(m_geometry);
  if (text)
    text->set_appearance(m_appearance);
#endif
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
    if (name == "backgroundObject") {
      set_background(compare_to_true(value));
      elem->mark_delete(ai);
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

  if (m_is_background != s_def_is_background) {
    attrib.first = "backgroundObject";
    attrib.second = (m_is_background ? "TRUE" : "FLASE");
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

  // Add the object fields to the prototype:
  Execution_function exec_func = 
    static_cast<Execution_function>(&Node::sphere_bound_changed);
  s_prototype->add_field_info(new SF_bool(ISVISIBLE, "visible",
                                          get_member_offset(&m_is_visible),
                                          exec_func));    
}

/*! \brief deletes the node prototype */
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

SGAL_END_NAMESPACE
