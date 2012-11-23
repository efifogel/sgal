// Copyright (c) 2008 Israel.
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
//
// Author(s)     : Boris Kozorovitzky         <zbzzn@hotmail.com>

/*!
 * An implementation of geodesic arc on a disc
 *
 * A geometry of type Geodesic. The arc is specified by two spherical coordinates.
 * Inherits from Geometry.
 */

#ifndef SGAL_GEODESIC_HPP
#define SGAL_GEODESIC_HPP

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"

#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Container_proto;
class Element;

class SGAL_CLASSDEF Geodesic : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    RADIUS,
    START,
    END,
    SOLID,
    STACKS, 
    BREAKS,
    COMPLEMENT,
    LAST
  };

  /*! Constructor */
  Geodesic(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Geodesic();

  /* Construct the prototype */
  static Geodesic * prototype() {  return new Geodesic(SGAL_TRUE); }

  /*! Clone */
  virtual Container * clone() { return new Geodesic(); }
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
  /*! Set the start point
       * \param point the new start point
      */
  void set_start(Vector2f start) { m_start = start; }

  /*! Obtain the start point of the geodesic
   * \return the start point
   */
  Vector2f get_start() const { return m_start; }

    /*! Set the end point
       * \param point the new end point
      */
  void set_end(Vector2f end) { m_end = end; }

  /*! Obtain the end point of the geodesic
   * \return the end point
   */
  Vector2f get_end() const { return m_end; }

  /*! Set the number of stacks
   * \param stacks the new number of stacks
   */
  void set_stacks(Uint stacks) { m_stacks = stacks; }

  /*! Obtain the number of stacks
   * \return the number of stacks
   */
  Uint get_stacks() const { return m_stacks; }

  /*! Set the number of breaks
   * \param  the new number of breaks
   */
  void set_breaks(Uint breaks) { m_breaks = breaks; }
  
  /*! Obtain the number of breaks
   * \return the number of breaks
   */
  Uint get_breaks() const { return m_breaks; }

   /*! Set */
  void set_solid(Boolean solid) { m_is_solid = solid; }

  /*! Obtain */
  Boolean is_solid() const;
  
   /*! Set */
  void set_complement(Boolean complement) { m_is_complement = complement; }

  /*! Obtain */
  Boolean is_complement() const;

  /*! Draw the geodesic
   * \param action the draw action
   */
  virtual void draw(Draw_action * action); 

  /*! Draw the geodesic in selection mode 
   * \param action the draw action
   */
  virtual void isect(Isect_action * action);

  /*! Calculare the sphere bound of the geodesic */
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

    /*! Determine whether the geometry has color (as opposed to normal) */
  virtual Boolean has_color() const { return SGAL_FALSE; }


protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The radius of the arc sphere */
  Float m_radius;
  
  /*! The start point of the geodesic */
  Vector2f m_start;

  /*! The start point of the geodesic */
  Vector2f m_end;
  
  /*! The number of stacks of the geodesic */
  Uint m_stacks;

  /*! The number of breaks of the geodesic */
  Uint m_breaks;

   /*! Indicates wether the object is a solid. If it is not a solid we need
   * to draw both sides of the polygons. 
   */
  Boolean m_is_solid;
  
  Boolean m_is_complement;

  void clean();

private:
  /*! default values of geodesic */
  static const Float s_def_radius;
  static const Vector2f s_def_start;
  static const Vector2f s_def_end;
  static const Uint s_def_stacks;
  static const Uint s_def_breaks;
  static const Boolean s_def_is_solid;
  static const Boolean s_def_is_complement;

};

SGAL_END_NAMESPACE

#endif
