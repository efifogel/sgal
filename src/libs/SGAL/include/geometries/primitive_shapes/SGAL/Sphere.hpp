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
// $Revision: 14223 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of the Box geometry.
 *
 * A geometry of type sphere. The sphere is specified by its radius.
 *
 * Inherits from Geometry.
 */

#ifndef SGAL_SPHERE_HPP
#define SGAL_SPHERE_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/glu.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;

class SGAL_CLASSDEF Sphere : public Geometry {
public:
  /*! Fields */
  enum {
    FIRST = Geometry::LAST - 1,
    CENTER,
    RADIUS,
    STACKS, 
    SLICES,
    LAST
  };

  /*! Constructor */
  Sphere(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Sphere();

  /* Construct the prototype */
  static Sphere* prototype() { return new Sphere(SGAL_TRUE); }

  /*! Clone */
  virtual Container* clone() { return new Sphere(); }

  /*! Set the sphere center */
  void set_center(const Vector3f& center) { m_center = center; }

  /*! Obtain the sphere center */
  const Vector3f& get_center() const { return m_center; }
  
  /*! Set the sphere radius */
  void set_radius(Float radius) { m_radius = radius; }

  /*! Obtain the sphere radius */
  Float get_radius() const { return m_radius; }

  /*! set the number of stacks that the sphere is made of */
  void set_stacks(Uint stacks) { m_stacks = stacks; }

  /*! Obtain the number of stacks that the sphere is made of */
  Uint get_stacks() const { return m_stacks; }

  /*! Set the number of slices that the sphere is made of */
  void set_slices(Uint slices) { m_slices = slices; }

  /*! Obtain the number of slices that the sphere is made of */
  Uint get_slices() const { return m_slices; }

  /*! Draw the sphere */
  virtual void draw(Draw_action* action); 

  /*! Draw the sphere in selection mode */
  virtual void isect(Isect_action* action);

  /*! Calculare the sphere bound of the sphere */
  virtual Boolean calculate_sphere_bound();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();
    
  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Return true if the geometry has color (as opposed to material) */
  virtual Boolean has_color() const { return SGAL_FALSE; }

  /*! Is dirty? */
  Boolean is_dirty() const { return m_dirty; }
  
protected:
  /*! Indicates whether the sphere has been initialized */
  Boolean m_dirty;
  
  /*! The utility instance */
  GLUquadricObj* m_sphere;

  /*! The sphere center */
  Vector3f m_center;
  
  /*! The sphere radius */
  Float m_radius;

  /*! The number of stacks */
  Uint m_stacks;

  /*! The number of slices */
  Uint m_slices;

  /*! Initialize the utility instance */
  void init();

  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Default values */
  static const Vector3f s_def_center;
  static const Float s_def_radius;
  static const Uint s_def_stacks;
  static const Uint s_def_slices;
};

SGAL_END_NAMESPACE

#endif
