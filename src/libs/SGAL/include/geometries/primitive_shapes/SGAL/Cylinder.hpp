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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of the cylinder geometry.
 *
 * A geometry of type Cylinder. The Cylinder is specified by its
 * radius and height. It is assumed to be centered at (0, 0, 0), has
 * a radius of 1 and a height of 2.
 *
 * Inherits from Geometry.
 */

#ifndef SGAL_CYLINDER_HPP
#define SGAL_CYLINDER_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_SGAL_DECL Cylinder : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    RADIUS,
    HEIGHT,
    SIDE,
    BOTTOM,
    TOP,
    SLICES,
    STACKS,
    LAST
  };

  /*! Constructor */
  Cylinder(Boolean proto = false);

  /*! Destructor */
  virtual ~Cylinder();

  /* Construct the prototype. */
  static Cylinder* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* radius_handle(Field_info*) { return &m_radius; }
  Float* height_handle(Field_info*) { return &m_height; }
  Boolean* is_body_visible_handle(Field_info*) { return &m_is_body_visible; }
  Boolean* is_bottom_visible_handle(Field_info*)
    { return &m_is_bottom_visible; }
  Boolean* is_top_visible_handle(Field_info*) { return &m_is_top_visible; }
  Uint* slices_handle(Field_info*) { return &m_slices; }
  Uint* stacks_handle(Field_info*) { return &m_stacks; }
  //@}

  virtual void set_attributes(Element* elem);
  // virtual Attribute_list get_attributes();

  /*! Draw the cylinder.
   * \param draw_action the draw action
   */
  virtual void draw(Draw_action* action);

  /*! Draw the cylinder in selection mode.
   * \param action
   */
  virtual void isect(Isect_action* action);

  virtual Boolean clean_sphere_bound();

  /*! Set the radius of the cylinder. */
  void set_radius(Float radius);

  /*! Obtain the radius of the cylinder. */
  Float get_radius() const;

  /*! Set the height of the cylinder. */
  void set_height(Float height);

  /*! Obtain the height of the cylinder. */
  Float get_height() const;

  /*! Set the number of stacks that the cylinder is made of. */
  void set_stacks(Uint stacks);

  /*! Obtain the number of stacks that the cylinder is made of. */
  Uint get_stacks() const;

  /*! Set the number of slices that the cylinder is made of. */
  void set_slices(Uint slices);

  /*! Obtain the number of slices that the cylinder is made of. */
  Uint get_slices() const;

  /*! Set the flag that determines whether the bottom is visible. */
  void set_is_bottom_visible(Boolean flag);

  /*! Obtain the flag that determines whether the bottom is visible. */
  Boolean is_bottom_visible() const;

  /*! Set the flag that determines whether the top is visible. */
  void set_is_top_visible(Boolean flag);

  /*! Obtain the flag that determines whether the top is visible. */
  Boolean is_top_visible() const;

  /*! Set the flag that determines whether the body is visible. */
  void set_is_body_visible(Boolean flag);

  /*! Obtain the flag that determines whether the body is visible */
  Boolean is_body_visible() const;

  /*! Determine whether the cylinder is dirty. */
  Boolean is_dirty() const { return m_dirty; }

protected:
  /*! Indicates whether the sphere has been initialized. */
  Boolean m_dirty;

  /*! The radius of the cylinder. */
  Float m_radius;

  /*! The height of the cylinder. */
  Float m_height;

  /*! The number of horizontal stacks used in rendering. */
  Uint m_stacks;

  /*! The number if vertical slices used in rendering. */
  Uint m_slices;

  /*! true if the bottom is showing. */
  Boolean m_is_bottom_visible;

  /*! true if the top is showing. */
  Boolean m_is_top_visible;

  /*! true if the cylinder is showing. */
  Boolean m_is_body_visible;

  /*! The OpenGL object used to draw the cylinder. */
  GLUquadricObj* m_cylinder;

  /*! The OpenGL object used to draw the base of the cylinder. */
  GLUquadricObj* m_cylinder_base;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default values. */
  static const Float s_def_radius;
  static const Float s_def_height;
  static const Uint s_def_stacks;
  static const Uint s_def_slices;
  static const Boolean s_def_is_bottom_visible;
  static const Boolean s_def_is_top_visible;
  static const Boolean s_def_is_body_visible;

  Cylinder(const Cylinder& cylinder) {}

  /*! Clean the cylinder. */
  void clean();
};

/* \brief constructs the prototype. */
inline Cylinder* Cylinder::prototype() { return new Cylinder(true); }

/*! \brief clones. */
inline Container* Cylinder::clone() { return new Cylinder(); }

/*! \brief sets the radius of the cylinder. */
inline void Cylinder::set_radius(Float radius)
{
  m_radius = radius;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the radius of the cylinder. */
inline Float Cylinder::get_radius() const { return m_radius; }

/*! \brief sets the height of the cylinder. */
inline void Cylinder::set_height(Float height)
{
  m_height = height;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the height of the cylinder. */
inline Float Cylinder::get_height() const { return m_height; }

/*! \brief sets the number of stacks that the cylinder is made of. */
inline void Cylinder::set_stacks(Uint stacks) { m_stacks = stacks; }

/*! \brief obtains the number of stacks that the cylinder is made of. */
inline Uint Cylinder::get_stacks() const { return m_stacks; }

/*! \brief sets the number of slices that the cylinder is made of. */
inline void Cylinder::set_slices(Uint slices) { m_slices = slices; }

/*! \brief obtains the number of slices that the cylinder is made of. */
inline Uint Cylinder::get_slices() const { return m_slices; }

/*! \brief Set the flag that determines whether the bottom is visible .*/
inline void Cylinder::set_is_bottom_visible(Boolean flag)
{ m_is_bottom_visible = flag; }

/*! \brief obtains the flag that determines whether the bottom is visible. */
inline Boolean Cylinder::is_bottom_visible() const
{ return m_is_bottom_visible; }

/*! \brief sets the flag that determines whether the top is visible. */
inline void Cylinder::set_is_top_visible(Boolean flag)
{ m_is_top_visible = flag; }

/*! \brief obtains the flag that determines whether the top is visible. */
inline Boolean Cylinder::is_top_visible() const { return m_is_top_visible; }

/*! \brief sets the flag that determines whether the body is visible. */
inline void Cylinder::set_is_body_visible(Boolean flag)
{ m_is_body_visible = flag; }

/*! \brief obtains the flag that determines whether the body is visible. */
inline Boolean Cylinder::is_body_visible() const { return m_is_body_visible; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Cylinder::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
