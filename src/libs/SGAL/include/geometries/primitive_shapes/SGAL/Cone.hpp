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
// $Revision: 11857 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of the cone geometry.
 *
 * The Cone is specified by its radius and height.
 * The cone is centered at (0, 0, 0), has radius 1, and height 2.
 *
 * Inherits from Geometry.
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
  Cone(Boolean proto = false);

  /*! Destructor */
  virtual ~Cone();

  /* Construct the prototype */
  static Cone* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the radius of the bottom disk */
  void set_bottom_radius(Float radius);

  /*! Obtain the radius of the bottom disk */
  Float get_bottom_radius() const;

  /*! Set the height */
  void set_height(Float height);

  /*! Obtain the height */
  Float get_height() const;

  void set_stacks(Uint stacks);

  Uint get_stacks() const;

  void set_slices(Uint slices);

  Uint get_slices() const;

  void set_is_bottom_visible(Boolean flag);

  Boolean is_bottom_visible() const;

  void set_is_side_visible(Boolean flag);

  Boolean is_side_visible() const;
  
  virtual void draw(Draw_action* action); 

  virtual void isect(Isect_action* action);

  virtual Boolean clean_sphere_bound();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  virtual void delete_prototype(); 

  virtual Container_proto* get_prototype();

  virtual void set_attributes(Element* elem);
  // virtual Attribute_list get_attributes();

  /*! Determine whether the geometry has color (as opposed to material). */
  virtual Boolean has_color() const;
  
  /*! Determine whether the geometry is dirty. */
  Boolean is_dirty() const;

protected:
  /*! Indicates whether the sphere has been initialized. */
  Boolean m_dirty;

  /*! Radius of cone's base. */
  Float m_bottom_radius;

  /*! Height of cone from center of the base to the apex. */
  Float m_height;

  /*! The number of horizontal stacks used in rendering. */
  Int m_stacks;

  /*! The number of vertical slices used in rendering. */
  Int m_slices;

  /*! Determines whether the sides of cone are visible. */
  Boolean m_side_visible;

  /*! tDetermines whether the bottom cap of the cone is visible. */
  Boolean m_bottom_visible;

  /*! The OpenGL object used to draw sides of the cone. */
  GLUquadricObj* m_cone;

  /*! The OpenGL object used to draw base of the cone. */
  GLUquadricObj* m_cone_base;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

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
  static const Boolean s_def_side_visible;
  static const Boolean s_def_bottom_visible;

  /*! Clean the cone. */
  void clean();

  Cone(const Cone& /* cone */) {}
};

/* \brief constructs the prototype. */
inline Cone* Cone::prototype() { return new Cone(true); }

/*! \brief clones. */
inline Container* Cone::clone() { return new Cone(); }

/*! \brief sets the radius of the bottom disk. */
inline void Cone::set_bottom_radius(Float radius)
{
  m_bottom_radius = radius;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the radius of the bottom disk. */
inline Float Cone::get_bottom_radius() const { return m_bottom_radius; }

/*! \brief sets the height. */
inline void Cone::set_height(Float height)
{
  m_height = height;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the height. */
inline Float Cone::get_height() const { return m_height; }

inline void Cone::set_stacks(Uint stacks) { m_stacks = stacks; }

/*! \brief obtains the stacks. */
inline Uint Cone::get_stacks() const { return m_stacks; }

inline void Cone::set_slices(Uint slices) { m_slices = slices; }

/*! \brief obtains the slices. */
inline Uint Cone::get_slices() const { return m_slices; }

inline void Cone::set_is_bottom_visible(Boolean flag)
{ m_bottom_visible = flag; }

inline Boolean Cone::is_bottom_visible() const { return m_bottom_visible; }

inline void Cone::set_is_side_visible(Boolean flag) { m_side_visible = flag; }

inline Boolean Cone::is_side_visible() const { return m_side_visible; }

/*! \brief determines whether the geometry has color. */
inline Boolean Cone::has_color() const { return false; }
  
/*! \brief determines whether the geometry is dirty. */
inline Boolean Cone::is_dirty() const { return m_dirty; }

/*! \brief obtains the tag (type) of the container */
inline const std::string& Cone::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
