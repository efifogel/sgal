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
// $Source$
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of the cone geometry.
 *
 *  A geometry of type Cone. The Cone is specified by its
 *  radius and height. It is assumed to be centered at (0, 0, 0), has
 * a radius of 1 and a height of 2.
 *
 *  Inherits from Geometry.
 */

#ifndef SGAL_CONE_HPP
#define SGAL_CONE_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isec_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Cone : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    BOTTOM_RADIUS,
    HEIGHT,
    SIDE,
    BOTTOM,
    SLICES,
    STACKS,
    LAST
  };

  /*! Constructor */
  Cone(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Cone();

  /* Construct the prototype */
  static Cone* prototype() { return new Cone(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Cone(); }

  /*! Set the radius of the bottom disk */
  void set_bottom_radius(Float radius) { m_bottom_radius = radius; }

  /*! Obtain the radius of the bottom disk */
  Float get_bottom_radius() const { return m_bottom_radius; }

  /*! Set the height */
  void set_height(Float height) { m_height = height; }

  /*! Obtain the height */
  Float GetHeight() const { return m_height; }

  void set_stacks(Uint stacks) { m_stacks = stacks; }
  Uint get_stacks() const { return m_stacks; }
  void set_slices(Uint slices) { m_slices = slices; }
  Uint get_slices() const { return m_slices; }

  void set_is_bottom_visible(Boolean flag) { m_is_bottom_visible = flag; }
  Boolean get_is_bottom_visible() const { return m_is_bottom_visible; }
  void set_is_side_visible(Boolean flag) { m_is_side_visible = flag; }
  Boolean get_is_side_visible() const { return m_is_side_visible; }
  
  virtual void draw(Draw_action* action); 
  virtual void isect(Isect_action* action);
  virtual Boolean calculate_sphere_bound();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype(); 
  virtual Container_proto* get_prototype();

  virtual void set_attributes(Element* elem);
  // virtual Attribute_list get_attributes();

  /*! Return true if the geometry has color (as opposed to material) */
  virtual Boolean has_color() const { return SGAL_FALSE; }
  
  /*! Is dirty? */
  Boolean is_dirty() const { return m_dirty; }

protected:
  /*! Indicates whether the sphere has been initialized */
  Boolean m_dirty;

  /*! Radius of cone's base */
  Float m_bottom_radius;

  /*! Height of cone from center of the base to the apex */
  Float m_height;

  /*! The number of horizontal stacks used in rendering */
  Int m_stacks;

  /*! The number of vertical slices used in rendering */
  Int m_slices;

  /*! SGAL_TRUE if sides of cone are created */
  Boolean m_is_side_visible;

  /*! SGAL_TRUE if bottom cap of the cone is created */
  Boolean m_is_bottom_visible;

  /*! the OpenGL object used to draw sides of the cone */
  GLUquadricObj* m_cone;

  /*! the OpenGL object used to draw base of the cone */
  GLUquadricObj* m_cone_base;

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! default values for Cone.  */
  static const Float s_def_bottom_radius;
  static const Float s_def_height;
  static const Uint s_def_stacks;
  static const Uint s_def_slices;
  static const Boolean s_def_is_side_visible;
  static const Boolean s_def_is_bottom_visible;

  /*! Intialize */
  void init();

  Cone(const Cone& /* con */) {}
};

SGAL_END_NAMESPACE

#endif
