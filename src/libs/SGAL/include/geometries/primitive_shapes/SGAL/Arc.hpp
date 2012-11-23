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
// $Revision: 7263 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * An implementation of spherecal arc
 *
 * A geometry of type arc. The arc is specified by its radius assumed to
 * be centered at (0, 0, 0), and 4 angles. 2 Angles specify the lower left
 * corner, and 2 angles specify the arc dimensions.
 *
 * Inherits from Geometry.
 */

#ifndef SGAL_ARC_HPP
#define SGAL_ARC_HPP

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Arc : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    RADIUS,
    ALPHA,
    BETA,
    GAMMA,
    DELTA,
    SOLID,
    STACKS, 
    SLICES,
    LAST
  };

  /*! Constructor */
  Arc(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Arc();

  /* Construct the prototype */
  static Arc * prototype() {  return new Arc(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Arc(); }
  
  /*! Set the arc radius
   * \param radius the new radius
   */
  void set_radius(Float radius) { m_radius = radius; }

  /*! Obtain the arc radius
   * \return the arc radius
   */
  Float get_radius() const { return m_radius; }

  /*! Set the number of stacks
   * \param stacks the new number of stacks
   */
  void set_stacks(Uint stacks) { m_stacks = stacks; }

  /*! Obtain the number of stacks
   * \return the number of stacks
   */
  Uint get_stacks() const { return m_stacks; }

  /*! Set the number of slices
   * \param  the new number of slices
   */
  void set_slices(Uint slices) { m_slices = slices; }
  
  /*! Obtain the number of slices
   * \return the number of slices
   */
  Uint get_slices() const { return m_slices; }

  /*! Set the lower left corner (X and Z coordinates.
   * The lower left corner is the intersection of the sphere and a vector. The
   * vector is defined by 2 angles.
   * \param the angle between the vector and the Z axis in the XZ plane.
   */
  void set_alpha(Float alpha) { m_alpha = alpha; }

  /*! Obtain */
  Float get_alpha() const { return m_alpha; }

  /*! Set */
  void set_beta(Float beta) { m_beta = beta; }

  /*! Obtain */
  Float get_beta() const { return m_beta; }

  /*! Set */
  void set_gamma(Float gamma) { m_gamma = gamma; }

  /*! Obtain */
  Float get_gamma() const { return m_gamma; }

  /*! Set */
  void set_delta(Float delta) { m_delta = delta; }

  /*! Obtain */
  Float get_delta() const { return m_delta; }

  /*! Set */
  void set_solid(Boolean solid) { m_is_solid = solid; }

  /*! Obtain */
  Boolean is_solid() const;

  /*! Draw the arc
   * \param action the draw action
   */
  virtual void draw(Draw_action * action); 

  /*! Draw the arc in selection mode 
   * \param action the draw action
   */
  virtual void isect(Isect_action * action);

  /*! Calculare the sphere bound of the arc */
  virtual Boolean calculate_sphere_bound();
  
  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype
   * \return the node prototype
   */
  virtual Container_proto * get_prototype();

  // virtual void FieldChanged(short fieldId);

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
   */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Determine whether the geometry has color (as opposed to normal) */
  virtual Boolean has_color() const { return SGAL_FALSE; }

  /*! Is dirty? */
  // Boolean is_dirty() const { return m_dirty; }

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

  /*! Halftone stipple pattern for backfacing elements */
  static Ubyte s_halftone[];
  
private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The radius of the arc sphere */
  Float m_radius;
  
  /*! The number of stacks of the arc */
  Uint m_stacks;

  /*! The number of slices of the arc */
  Uint m_slices;

  Float m_alpha;

  Float m_beta;

  Float m_gamma;

  Float m_delta;

  /*! Indicates wether the object is a solid. If it is not a solid we need
   * to draw both sides of the polygons. 
   */
  Boolean m_is_solid;

  void clean();

private:
  /*! default values of arc */
  static const Float s_def_radius;
  static const Uint s_def_stacks;
  static const Uint s_def_slices;
  static const Float s_def_alpha;
  static const Float s_def_beta;
  static const Float s_def_gamma;
  static const Float s_def_delta;
  static const Boolean s_def_is_solid;
};

SGAL_END_NAMESPACE

#endif
