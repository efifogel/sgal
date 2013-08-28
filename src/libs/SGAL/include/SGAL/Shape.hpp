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

#ifndef SGAL_SHAPE_HPP
#define SGAL_SHAPE_HPP

/*! \file
 * \brief A representation of a shape node.
 * A shape node holds a geometry and an appearance attributes
 *
 * A shape is a node in the scene graph that is holding references to one or
 * more geometries and a single apperance.
 *
 * Inherits from Node.
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector4ub.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Appearance;
class Container_proto;
class Element;
class Geometry;
class Formatter;
class Scene_graph;

/*! \brief A representation of a shape node */
class SGAL_SGAL_DECL Shape : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    APPEARANCE,
    GEOMETRY,
    ISVISIBLE,
    DRAW_DEPTH,
    TEST_DEPTH,
    DEPTH_RANGE,
    DEPTH_FUNCTION,
    COLOR_MASK,
    CULL_FACE,
    LAST
  };

  typedef boost::shared_ptr<Appearance>         Shared_appearance;
  typedef boost::shared_ptr<Geometry>           Shared_geometry;

  /*! Constructor */
  Shape(Boolean proto = false);

  /*! Destructor */
  virtual ~Shape();

  /*! Construct the prototype. */
  static Shape* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* is_visible_handle(Field_info*) { return &m_is_visible; }
  Shared_geometry* geometry_handle(Field_info*) { return &m_geometry; }
  Shared_appearance* appearance_handle(Field_info*) { return &m_appearance; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the bounding sphere of the shape.
   * \return true if the bounding sphere has changed since last call.
   */
  virtual Boolean clean_sphere_bound();

  virtual void cull(Cull_context& cull_context);

  /*! Draw the node while traversing the scene graph. */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  /*! Draw the shape for selection ignoring the appearance and using
   * a unique number that identifies the shape instead. The identifier is
   * encoded as color in the frame buffer.
   */
  virtual void isect(Isect_action* isect_action);

  /*! Write this container. */
  virtual void write(Formatter* formatter);

  /*! Add the container to the given scene. */
  void add_to_scene(Scene_graph* sg);

  /*! Obtain the (const) appearance.
   * \return the appearance.
   */
  const Shared_appearance get_appearance() const;

  /*! Obtain the (non-const) appearance.
   * \return the appearance.
   */
  Shared_appearance get_appearance();

  /*! Set an appearance.
   * \param appearance the new appearance.
   */
  void set_appearance(Shared_appearance appearance);

  /*! Obtain the geometry.
   * \retrun the geometry.
   */
  Shared_geometry get_geometry() const;

  /*! Set a geometry
   * \param geometry the new geometry.
   */
  void set_geometry(Shared_geometry geometry);

  /*! Obtain the flag that indicates whether to draw into the depth buffer.
   * \return true iff drawing into the depth buffer is enbaled.
   */
  Boolean get_draw_depth() const;

  /*! Set the flag that indicates whether to draw into the depth buffer.
   * \param draw_depth indicates whether to draw into the depth buffer.
   */
  void set_draw_depth(Boolean draw_depth);

  /*! Obtain the flag that indicates whether to test the depth buffer.
   * \return true iff testing the depth buffer is enabled.
   */
  Boolean get_test_depth() const;

  /*! Set the flag that indicates whether to test the depth buffer.
   * \param test_depth indicates whether to test the depth buffer.
   */
  void set_test_depth(Boolean test_depth);

  /*! Obtain the depth function.
   * \return the depth function.
   */
  Gfx::Depth_func get_depth_function() const;

  /*! Set the depth function.
   * \param depth_function the depth function.
   */
  void set_depth_function(Gfx::Depth_func depth_function);

  /*! Obtain the color mask.
   * \return the color mask.
   */
  Vector4ub get_color_mask() const;

  /*! Set the color mask.
   * \param color_mask the color mask.
   */
  void set_color_mask(Vector4ub color_mask);

  /*! Obtain the cull-face mode.
   * \return the cull-face mode.
   */
  Gfx::Cull_face get_cull_face() const;

  /*! Set the cull-face mode.
   * \param cull_face the cull-face mode.
   */
  void set_cull_face(Gfx::Cull_face cull_face);

  /*! Turn on the flag that indicates whether the shape should be rendered. */
  void set_visible();

  /*! Turn off the flag that indicates whether the shape should be rendered. */
  void set_invisible();

  /*! Set the flag that indicates whether the shape should be rendered. */
  void set_visible(Boolean flag);

  /*! Determine whether the shape should be rendered. */
  Boolean is_visible() const;

  /*! Obtain the rendering priority. */
  Float get_priority() const;

  /*! Set the rendering priority. */
  void set_priority(Float priority);

  /*! Clean the node before drawing. */
  void clean();

  /*! Process change of appearance. */
  void appearance_changed(Field_info* /* field_info. */);

  /*! Process change of geometry. */
  void geometry_changed(Field_info* /* field_info. */);

  virtual Boolean attach_context(Context* context);

  virtual Boolean detach_context(Context* context = 0);

  /*! Sets the flag that indicates whether to construct the appearance
   * material attribute when missing.
   */
  void set_override_material(Boolean flag);

  /*! Sets the flag that indicates whether to override the appearance texture
   * enable flag.
   */
  void set_override_tex_enable(Boolean flag);

  /*! Sets the flag that indicates whether to override the appearance texture
   * environment.
   */
  void set_override_tex_env(Boolean flag);

  /*! Sets the flag that indicates whether to override the appearance blend
   * functions.
   */
  void set_override_blend_func(Boolean flag);

  /*! Sets the flag that indicates whether to override the light model
   */
  void set_override_light_model(Boolean flag);

  /*! Sets the flag that indicates whether to override the appearance
   * texture-generation flag and construct the appearance texture-generation
   * attribute when missing.
   */
  void set_override_tex_gen(Boolean flag);

  /*! Sets the flag that indicates whether to override the appearance
   * light-enable flag.
   */
  void set_override_light_enable(Boolean flag);

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! Indicates whether to draw into the depth buffer. */
  Boolean m_draw_depth;

  /*! Indicates whether to test the depth buffer. */
  Boolean m_test_depth;

  /*! Specify mapping of depth values from normalized device coordinates to
   * window coordinates.
   */
  Vector2f m_depth_range;

  /*! The depth function of the shape. */
  Gfx::Depth_func m_depth_function;

  /*! The color mask of the shape. */
  Vector4ub m_color_mask;

  /*! The cull-face mode of the shape. */
  Gfx::Cull_face m_cull_face;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The apperance attribute. */
  Shared_appearance m_appearance;

  /*! The list of geometries. */
  Shared_geometry m_geometry;

  /*! Indicates whether the shape is visible and thus should be rendered. */
  Boolean m_is_visible;

  /*! The rendering priority. */
  Float m_priority;

  /* A flag that indicates whether backface drawing is required. */
  Boolean m_draw_backface;

  /*! Indicates whether the shape is dirty, and thus needs cleaning. */
  Boolean m_dirty;

  /*! Indicates whether the shape appearance is dirty, and thus needs
   * cleaning.
   */
  Boolean m_dirty_appearance;

  /*! Indicates whether the shape geometry is dirty, and thus needs cleaning. */
  Boolean m_dirty_geometry;

  /*! Indicates whether to apply texture mapping. */
  Boolean m_texture_map;

  /*! Indicates whether to construct the appearance material attribute when
   * missing.
   * Notice that when the appearance chages, the material attribute of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_material;

  /*! Indicates whether to override the appearance texture enebale flag. If
   * this flag is on, the (appearance) texture enable may be overriden.
   * Notice than when the appearance chages, the texture-enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_tex_enable;

  /*! Indicates whether to override the appearance texture environment. If
   * this flag is on, the (appearance) texture environment may be overriden.
   * Notice than when the appearance chages, the texture environment of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_tex_env;

  /*! Indicates whether to override the appearance blend functions. If
   * this flag is on, the (appearance) blend functions may be overriden.
   * Notice than when the appearance chages, the blend functions of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_blend_func;

  /*! Indicates whether to override the appearance light model. If
   * this flag is on, the (appearance) light model may be overriden.
   * Notice than when the appearance chages, the light model of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_light_model;

  /*! Indicates whether to override the appearance texture-generation flag and
   * construct the appearance texture-generation attribute when missing. If
   * this flag is on and the corresponding geometry does not generate
   * texture coordinates, the (appearance) texture-generation is enabled and
   * the (appearance) texture generation is created if missing.
   * Notice than when the appearance chages, the texture-generation flag (and
   * possibly attribute) of the previous appearance are not restored to their
   * original values.
   */
  Boolean m_override_tex_gen;

  /*! Indicates whether to override the appearance light-enable flag. If this
   * flag is on, the (appearance) light is enabled if the corresponding
   * geometry does not have color.
   * Notice than when the appearance chages, the light-enable flag of the
   * previous appearance is not restored to its original value.
   */
  Boolean m_override_light_enable;

  /*! Draw the geometry. */
  void draw_geometry(Draw_action* draw_action);

  /*! Cleane the appearance. */
  void clean_appearance();

  static const Vector2f s_def_depth_range;
  static const Vector4ub s_def_color_mask;
  static const Gfx::Depth_func s_def_depth_function;
  static const Gfx::Cull_face s_def_cull_face;

  static const Char* s_depth_function_names[];
  static const Char* s_cull_face_names[];
};

/*! \brief constructs the prototype. */
inline Shape* Shape::prototype() { return new Shape(true); }

/*! \brief clones. */
inline Container* Shape::clone() { return new Shape(); }

/*! \brief obtains the (const) appearance. */
inline const Shape::Shared_appearance Shape::get_appearance() const
{ return m_appearance; }

/*! \brief obtains the (non-const) appearance. */
inline Shape::Shared_appearance Shape::get_appearance() { return m_appearance; }

/*! \brief obtains the flag that indicates whether to draw into the depth
 * buffer.
 */
inline Boolean Shape::get_draw_depth() const { return m_draw_depth; }

/*! \brief sets the flag that indicates whether to draw into the depth buffer. */
inline void Shape::set_draw_depth(Boolean draw_depth)
{ m_draw_depth = draw_depth; }

/*! \brief obtains the flag that indicates whether to test the depth buffer. */
inline Boolean Shape::get_test_depth() const { return m_test_depth; }

/*! \brief sets the flag that indicates whether to test the depth buffer. */
inline void Shape::set_test_depth(Boolean test_depth)
{ m_test_depth = test_depth; }

/*! \brief obtains the depth function. */
inline Gfx::Depth_func Shape::get_depth_function() const
{ return m_depth_function; }

/*! \brief sets the depth function. */
inline void Shape::set_depth_function(Gfx::Depth_func depth_function)
{ m_depth_function = depth_function; }

/*! \brief obtains the color mask. */
inline Vector4ub Shape::get_color_mask() const { return m_color_mask; }

/*! \brief sets the color mask. */
inline void Shape::set_color_mask(Vector4ub color_mask)
{ m_color_mask = color_mask; }

/*! \brief obtains the cull-face mode. */
inline Gfx::Cull_face Shape::get_cull_face() const { return m_cull_face; }

/*! \brief sets the cull-face mode. */
inline void Shape::set_cull_face(Gfx::Cull_face cull_face)
{ m_cull_face = cull_face; }

/*! \brief determines whether the shape should be rendered. */
inline Boolean Shape::is_visible() const { return m_is_visible; }

/*! \brief obtains the rendering priority. */
inline Float Shape::get_priority() const { return m_priority; }

/*! \brief sets the rendering priority. */
inline void Shape::set_priority(Float priority) { m_priority = priority; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Shape::get_tag() const { return s_tag; }

/*! \brief obtains the gepmetry. */
inline Shape::Shared_geometry Shape::get_geometry() const
{ return m_geometry; }

/*! \brief sets the flag that indicates whether to construct the appearance
 * material attribute when missing.
 */
inline void Shape::set_override_material(Boolean flag)
{ m_override_material = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * texture enable flag.
 */
inline void Shape::set_override_tex_enable(Boolean flag)
{ m_override_tex_enable = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * texture environment.
 */
inline void Shape::set_override_tex_env(Boolean flag)
{ m_override_tex_env = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * blend functions.
 */
inline void Shape::set_override_blend_func(Boolean flag)
{ m_override_blend_func = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * light model.
 */
inline void Shape::set_override_light_model(Boolean flag)
{ m_override_light_model = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * texture-generation flag and construct the appearance texture-generation
 * attribute when missing.
 */
inline void Shape::set_override_tex_gen(Boolean flag)
{ m_override_tex_gen = flag; }

/*! \brief sets the flag that indicates whether to override the appearance
 * light-enable flag.
 */
inline void Shape::set_override_light_enable(Boolean flag)
{ m_override_light_enable = flag; }

SGAL_END_NAMESPACE

#endif
