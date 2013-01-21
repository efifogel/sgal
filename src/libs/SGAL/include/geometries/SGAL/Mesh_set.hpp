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
    LAST
  };

  /*! Constructor */
  Mesh_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Mesh_set() {}

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */  
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly. */
  virtual void draw(Draw_action* action);

  /*! Draw the mesh (unconditionaly). */
  virtual void draw_mesh(Draw_action* action);
  
  /*! Calculate the sphere bound. */
  virtual Boolean clean_sphere_bound();
  
  /*! Draw the representation. */
  virtual void draw_geometry(Draw_action* action) = 0;

  /*! Clean the representation. */
  virtual void clean();

  /*! Clear the representation. */
  virtual void clear();
  
  /*! Determine whether the representation hasn't been cleaned. */
  virtual Boolean is_dirty() const;

  /*! Determine whether the representation is empty. */
  virtual Boolean is_empty() const = 0;

  /*! Set the counter-clockwise flag. */
  void set_ccw(Boolean ccw);

  /*! Determine whether the surface polygons are counter-clockwise. */
  Boolean is_ccw() const;

  /*! Set the flag that indicates whether the mesh represents is solid
   * (water-proof) polytope. In case the mesh represents a solid polytope,
   * the back faces can be culled out during rendering, and the front faces
   * can be rendered single sided.
   */
  void set_solid(Boolean solid);

  /*! Determines whether the mesh represents a solid (water-proof) polytope. */
  Boolean is_solid() const;

  /*! Set the flag that indicates whether the mesh represents a convex
   * polytope.
   */
  void set_convex(Boolean convex);

  /*! Determines whether the mesh represents a convex polytope. */
  Boolean is_convex() const;

  /*! Set the crease angle. */
  void set_crease_angle(Float crease_angle);

  /*! Obtain the creas_angle. */
  Float get_crease_angle() const;

  /*! Process change of coordinates. */
  virtual void coord_changed(SGAL::Field_info* field_info);

protected:
  /*! Indicates whether the mesh must be cleaned. */
  Boolean m_dirty;

  /*! Indicates the direction of the vertices in a face. The default is
   * counter clock wise.
   */
  Boolean m_is_ccw;

  /*! Indicates wether the object is a solid. If it is not a solid we need
   * to draw both sides of the polygons. 
   */
  Boolean m_is_solid;

  /*! Indicates whether all faces are convex. */
  Boolean m_is_convex;

  /*! The crease angle. */
  Float m_crease_angle;

  /*! The polygon offset factor. */
  Float m_polygon_offset_factor;
  
  /*! Indicates whether the mesh must be cleaned. */
  Boolean m_dirty_indices;

  /*! Indicates whether to remove the -1 end-of-face markers in case of
   * triangles and quads.
   */
  Boolean m_flatten_indices;

  /*! Indicates whether the indices structure is "flat". In a "flat"
   * representation, in case of triangles and quads, the no -1 end-of-face
   * markers are not present. The number of indices is (m_num_primitives * 3)
   * or (m_num_primitives * 4) respectively. It is illegal to have a mixture
   * of flat and non-flat index arrays. Either all indices are flat or none is.
   */
  Boolean m_are_indices_flat;
  
  /*! Proces the indices. */
  void flatten_indices();

  /*! Clean the indices. */
  void clean_indices();

  /*! Determine whether the indices hasn't been cleaned. */
  Boolean is_dirty_indices() const;
  
  /*! Proces the indices. */
  void flatten_indices(Uint* src, Uint* dst, Uint num);

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default value. */
  static const Boolean s_def_is_ccw;
  static const Boolean s_def_is_solid;
  static const Boolean s_def_is_convex;
  static const Float s_def_crease_angle;
  static const Float s_def_polygon_offset_factor;
};

/*! \brief determines whether the indices hasn't been cleaned. */
inline Boolean Mesh_set::is_dirty_indices() const { return m_dirty_indices; }

/*! \brief clears the representation. */
inline void Mesh_set::clear() { m_dirty = true; }
  
/*! \brief determines whether the representation hasn't been cleaned. */
inline Boolean Mesh_set::is_dirty() const { return m_dirty; }

/*! \brief processes change of coordinates. */
inline void Mesh_set::coord_changed(SGAL::Field_info* /* field_info */)
{ m_dirty_sphere_bound = true; }

/*! \brief sets the counter-clockwise flag. */
inline void Mesh_set::set_ccw(Boolean ccw) { m_is_ccw = ccw; }

/*! \brief returns true if the surface polygons are counter-clockwise. */
inline Boolean Mesh_set::is_ccw() const { return m_is_ccw; }

/*! \brief sets the solid flag. */
inline void Mesh_set::set_solid(Boolean solid) { m_is_solid = solid; }

/*! \brief returns true if the polyhedron is solid, and false otherwise. */
inline Boolean Mesh_set::is_solid() const { return m_is_solid; }

/*! \brief sets the convex flag. */
inline void Mesh_set::set_convex(Boolean convex) { m_is_convex = convex; }

/*! \brief returns true if all faces of the polyhedron are convex. */
inline Boolean Mesh_set::is_convex() const { return m_is_convex; }

/*! \brief sets the crease angle of the polyhedron.*/
inline void Mesh_set::set_crease_angle(Float crease_angle)
{ m_crease_angle = crease_angle; }

/*! \brief obtains the creas_angle. */
inline Float Mesh_set::get_crease_angle() const { return m_crease_angle; }

SGAL_END_NAMESPACE

#endif
