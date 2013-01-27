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

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
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

/*! \brief A representation of a shape node */
class SGAL_CLASSDEF Shape : public Node {
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

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the bounding sphere of the shape. */
  virtual Boolean clean_sphere_bound();

  virtual void cull(Cull_context& cull_context);

  /*! Draw the node while traversing the scene graph. */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  virtual void isect(Isect_action* isect_action);

  /*! Write this container. */
  virtual void write(Formatter* formatter);
  
  /*! Obtain the (const) appearance.
   * \return the appearance.
   */
  const Appearance* get_appearance() const;

  /*! Obtain the (non-const) appearance.
   * \return the appearance.
   */
  Appearance* get_appearance();

  /*! Set an appearance.
   * \param appearance the new appearance.
   */
  void set_appearance(Appearance* appearance, Boolean owned = false);

  /*! Obtain the geometry.
   * \retrun the geometry.
   */
  Geometry* get_geometry() const;

  /*! Set a geometry
   * \param geometry the new geometry.
   */
  void set_geometry(Geometry* geometry);

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

  void set_visible();

  void set_invisible();

  Boolean is_visible() const;

  Boolean is_background() const;

  void set_background(Boolean flag);

  Boolean is_text_object();

  /*! Obtain the rendering priority. */
  Float get_priority() const;

  /*! Set the rendering priority. */
  void set_priority(Float priority);

  /*! \todo rename to clean and m_dirty. */
  void init();

  virtual Boolean attach_context(Context* context); 

  virtual Boolean detach_context(Context* context = 0); 

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
  Appearance* m_appearance;

  /*! The alternative apperance attribute used when both texture and reflection
   * map are to be applied.
   */
  Appearance* m_alt_appearance;

  /*! The list of geometries. */
  Geometry* m_geometry;

  /*! if false the shape is not rendered. */
  Boolean m_is_visible;

  /*! a flag that indicates whether this is a background object. */
  Boolean m_is_background;

  /*! a flag that indicates if the object has been initialized. */
  Boolean m_is_initialized;

  /*! Indicates whether the appearance node has been constructed by default. */
  Boolean m_owned_appearance;

  /*! this is true when the geometry is a text object. */
  Boolean m_is_text_object;

  /*! The rendering priority. */
  Float m_priority;

  /* A flag that indicates whether backface drawing is required. */
  Boolean m_draw_backface;
  
  void draw_geometries(Draw_action* draw_action);
  void create_default_appearance();

  static const Boolean s_def_is_background;
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
inline const Appearance* Shape::get_appearance() const { return m_appearance; }

/*! \brief obtains the (non-const) appearance. */
inline Appearance* Shape::get_appearance() { return m_appearance; }

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

/*! \brief */
inline void Shape::set_visible() { m_is_visible = true; }

/*! \brief */
inline void Shape::set_invisible() { m_is_visible = false; }

/*! \brief */
inline Boolean Shape::is_visible() const { return m_is_visible; }

/*! \brief */
inline Boolean Shape::is_background() const { return m_is_background; }

/*! \brief */
inline void Shape::set_background(Boolean flag) { m_is_background = flag; }

/*! \brief Obtain the rendering priority. */
inline Float Shape::get_priority() const { return m_priority; }

/*! \brief Set the rendering priority. */
inline void Shape::set_priority(Float priority) { m_priority = priority; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Shape::get_tag() const { return s_tag; }

/*! \brief obtains the gepmetry. */
inline Geometry* Shape::get_geometry() const { return m_geometry; }

SGAL_END_NAMESPACE

#endif
