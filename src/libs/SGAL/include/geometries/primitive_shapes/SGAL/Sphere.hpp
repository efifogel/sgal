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
  Sphere(Boolean proto = false);

  /*! Destructor */
  virtual ~Sphere();

  /* Construct the prototype */
  static Sphere* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Set the sphere center */
  void set_center(const Vector3f& center);

  /*! Obtain the sphere center */
  const Vector3f& get_center() const;
  
  /*! Set the sphere radius */
  void set_radius(Float radius);

  /*! Obtain the sphere radius */
  Float get_radius() const;

  /*! set the number of stacks that the sphere is made of */
  void set_stacks(Uint stacks);

  /*! Obtain the number of stacks that the sphere is made of */
  Uint get_stacks() const;

  /*! Set the number of slices that the sphere is made of */
  void set_slices(Uint slices);

  /*! Obtain the number of slices that the sphere is made of */
  Uint get_slices() const;

  /*! Draw the sphere */
  virtual void draw(Draw_action* action); 

  /*! Draw the sphere in selection mode */
  virtual void isect(Isect_action* action);

  /*! Calculare the sphere bound of the sphere */
  virtual Boolean clean_sphere_bound();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtains the node prototype */
  virtual Container_proto* get_prototype();
    
  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Is dirty? */
  Boolean is_dirty() const;
  
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

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;
  
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

/* C\brief onstructs the prototype. */
inline Sphere* Sphere::prototype() { return new Sphere(true); }

/*! \brief clones. */
inline Container* Sphere::clone() { return new Sphere(); }

/*! \brief sets the sphere center. */
inline void Sphere::set_center(const Vector3f& center)
{
  m_center = center;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the sphere center. */
inline const Vector3f& Sphere::get_center() const { return m_center; }
  
/*! \brief sets the sphere radius. */
inline void Sphere::set_radius(Float radius)
{
  m_radius = radius;
  m_dirty_sphere_bound = true;
}

/*! \brief obtains the sphere radius. */
inline Float Sphere::get_radius() const { return m_radius; }

/*! \brief sets the number of stacks that the sphere is made of. */
inline void Sphere::set_stacks(Uint stacks) { m_stacks = stacks; }

/*! \brief obtains the number of stacks that the sphere is made of. */
inline Uint Sphere::get_stacks() const { return m_stacks; }

/*! \brief sets the number of slices that the sphere is made of. */
inline void Sphere::set_slices(Uint slices) { m_slices = slices; }

/*! \brief obtains the number of slices that the sphere is made of. */
inline Uint Sphere::get_slices() const { return m_slices; }

/*! \brief determines whether the sphere is dirty. */
inline Boolean Sphere::is_dirty() const { return m_dirty; }

/*! \brief obtains the tag (type) of the container. */
inline const std::string& Sphere::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
