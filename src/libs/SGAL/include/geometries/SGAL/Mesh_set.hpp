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
// $Revision: 14184 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_MESH_SET_HPP
#define SGAL_MESH_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;

class Mesh_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    CCW,
    SOLID,
    CONVEX,
    CREASE_ANGLE,
    INVALIDATE,
    LAST
  };

  /*! Constructor */
  Mesh_set(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Mesh_set() {}

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */  
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly */
  virtual void draw(Draw_action* action);

  /*! Draw the mesh (unconditionaly) */
  virtual void draw_mesh(Draw_action* action);
  
  /*! Calculate the sphere bound */
  virtual Boolean calculate_sphere_bound();
  
  /*! Draw the representation */
  virtual void draw_geometry(Draw_action* action) = 0;

  /*! Clean the representation */
  virtual void clean();

  /*! Clear the representation */
  virtual void clear() { m_dirty = true; }
  
  /*! Determine whether the representation hasn't been cleaned */
  virtual Boolean is_dirty() const { return m_dirty; }

  /*! Determine whether the representation is empty */
  virtual Boolean is_empty() const = 0;

  /*! Set the counter-clockwise flag */
  void set_ccw(Boolean ccw);

  /*! Determine whether the surface polygons are counter-clockwise */
  Boolean is_ccw() const;

  void set_solid(Boolean solid);
  Boolean is_solid() const;

  void set_convex(Boolean convex);
  Boolean is_convex() const;

  void set_crease_angle(Float crease_angle);
  Float get_crease_angle() const;

  /*! Process change of coordinates */
  virtual void coord_changed(SGAL::Field_info* /* field_info */) {}

  /*! Process change of points */
  virtual void point_changed() {}

protected:
  /*! Indicates whether the mesh must be cleaned */
  Boolean m_dirty;

  /*! Indicates the direction of the vertices in a face. The default is
   * counter clock wise.
   */
  Boolean m_is_ccw;

  /*! Indicates wether the object is a solid. If it is not a solid we need
   * to draw both sides of the polygons. 
   */
  Boolean m_is_solid;

  /*! Indicates whether all faces are convex */
  Boolean m_is_convex;

  /*! The crease angle */
  Float m_crease_angle;

  /*! The polygon offset factor */
  Float m_polygon_offset_factor;
  
private:
  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! Default value */
  static const Boolean s_def_is_ccw;
  static const Boolean s_def_is_solid;
  static const Boolean s_def_is_convex;
  static const Float s_def_crease_angle;
  static const Float s_def_polygon_offset_factor;
};

/*! Sets the counter-clockwise flag */
inline void Mesh_set::set_ccw(Boolean ccw) { m_is_ccw = ccw; }

/*! Returns true if the surface polygons are counter-clockwise */
inline Boolean Mesh_set::is_ccw() const { return m_is_ccw; }

/*! sets the solid flag */
inline void Mesh_set::set_solid(Boolean solid) { m_is_solid = solid; }

/*! returns true if the polyhedron is solid, and false otherwise */
inline Boolean Mesh_set::is_solid() const { return m_is_solid; }

/*! sets the convex flag */
inline void Mesh_set::set_convex(Boolean convex) { m_is_convex = convex; }

/*! returns true if all faces of the polyhedron are convex */
inline Boolean Mesh_set::is_convex() const { return m_is_convex; }

/*! sets the crease angle of the polyhedron.*/
inline void Mesh_set::set_crease_angle(Float crease_angle)
{ m_crease_angle = crease_angle; }

/*! obtains the creas_angle */
inline Float Mesh_set::get_crease_angle() const { return m_crease_angle; }

SGAL_END_NAMESPACE

#endif
