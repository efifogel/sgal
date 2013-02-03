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

#ifndef SGAL_GEOMETRY_HPP
#define SGAL_GEOMETRY_HPP

/*! \file
 * A base class for all geometries.
 *  
 * A geometry is a base (abstract) class. Currently it only
 * contains a unique id which is used as a display list id.
 *  
 * Inherits from Container
 */

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Sphere_bound.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Isect_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Geometry : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    SPHERE_BOUND,
    BOUND_MODE,
    GENERATED_TEX_COORD,
    LAST
  };

  enum Bound_mode {
    STATIC,
    DYNAMIC
  };

  /*! Constructor */
  Geometry(Boolean proto = false);

  /*! Destructor */
  virtual ~Geometry() {}

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  
  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();
  
  //void set_box_bound(const Box_bound & box_bound);
  //void get_box_bound(Box_bound & box_bound);

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action) = 0;

  /*! Draw the geometry in selection mode. */
  virtual void isect(Isect_action* action) = 0;

  /*! Calculare the sphere bound of the geometry. */
  virtual Boolean clean_sphere_bound() = 0;

  virtual const Sphere_bound* get_sphere_bound(bool& changed);

  const Sphere_bound* get_sphere_bound() {
    bool dummy;
    return get_sphere_bound(dummy);
  }

  /*! Set the flag that indicates that the sphere bound should be cleaned.*/
  void sphere_bound_changed(Field_info* field_info = NULL);
  
  /*! Return true if the current matrix contains scaling.
   * \todo has_scale() doesn't belong to Geometry!
   */
  virtual Boolean has_scale();

  /*! Determine whether colors are generated by the geometry. */
  virtual Boolean are_generated_color();

  /*! Set the flag that indicates whether colors are generated by the geometry.
   */
  void set_generated_color(Boolean flag);

  /*! Determine whether texture coordinates are generated by the geometry. */
  virtual Boolean are_generated_tex_coord();

  /*! Set the flag that indicates whether texture coordinates are generated
   * by the geometry.
   */
  void set_generated_tex_coord(Boolean flag);

protected:
  /*! The sphere bound of the node. */
  Sphere_bound m_sphere_bound;

  /*! Indicatres whether the bounding sphere is valid. */
  Boolean m_dirty_sphere_bound;

  /*! Indicates whether the bounding box is pewviously set. */
  Boolean m_bb_is_pre_set;

  /*! Indicates whether colors should be generated by the geometry. */
  Boolean m_generated_color;

  /*! Indicates whether texture coordinates should be generated by the
   * geometry (as oppsed to the texture generation capabilities of the
   * graphics system).
   */
  Boolean m_generated_tex_coord;

 private:
  /*! The prototype of this node. */
  static Container_proto* s_prototype;
};

/*! \brief determines whether colors are generated by the geometry. */
inline Boolean Geometry::are_generated_color() { return m_generated_color; }

/*! \brief sets the flag that indicates whether colors are generated by the
 * geometry.
 */
inline void Geometry::set_generated_color(Boolean flag)
{ m_generated_color = flag; }  

/*! \brief determines whether texture coordinates are generated by the geometry.
 */
inline Boolean Geometry::are_generated_tex_coord()
{ return m_generated_tex_coord; }

/*! \brief sets the flag that indicates whether texture coordinates are 
 * generated by the geometry.
 */
inline void Geometry::set_generated_tex_coord(Boolean flag)
{ m_generated_tex_coord = flag; }  

SGAL_END_NAMESPACE

#endif
