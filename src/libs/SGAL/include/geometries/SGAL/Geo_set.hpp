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

#ifndef SGAL_GEO_SET_HPP
#define SGAL_GEO_SET_HPP

/*! \file
 * A base class for mesh objects.
 *
 *  Inherits from Geometry.
 */

#include <sys/types.h>
#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Color_array.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Normal_array;
class Tex_coord_array;
class Index_array;
class Container_proto;
class Element;

/*! Geo_set describes geometry using a set of arrays that represent the
 * coordinates, normals, colors, and texture-coordinates of the vertices
 * of the geometry, and corresponding index arrays into the former resp.
 */
class SGAL_CLASSDEF Geo_set : public Geometry {
public:
  /*! Fields */
  enum {
    FIRST = Geometry::LAST-1,
    NUM_PRIMITIVES,
    NORMAL_ATTACHMENT,
    COLOR_ATTACHMENT,
    COORD_ARRAY,
    NORMAL_ARRAY,
    COLOR_ARRAY,
    TEX_COORD_ARRAY,
    COORD_INDEX_ARRAY,
    NORMAL_INDEX_ARRAY,
    COLOR_INDEX_ARRAY,
    TEX_COORD_INDEX_ARRAY,
    LAST
  };

  enum Attachment {
    PER_VERTEX = 0x0,
    PER_PRIMITIVE = 0x1,
    PER_MESH = 0x2
  };

  enum Primitive_type {
    PT_TRIANGLE_STRIP = 0x0,
    PT_TRIANGLE_FAN = 0x1,
    PT_TRIANGLES = 0x2,
    PT_QUAD_STRIP = 0x3,
    PT_QUADS = 0x4,
    PT_POLYGONS = 0x5,
    PT_LINES = 0x6,
    PT_LINE_STRIPS = 0x7,
    PT_LINE_LOOPS = 0x8,
    NUM_PRIMITIVE_TYPES = 9
  };

  enum Fragment_source {
    FS_NORMAL = 0x0,
    FS_COLOR = 0x1
  };

  /*! Constructor */
  Geo_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Geo_set();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype
   * \return the node prototype
   */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of the object */
  virtual void set_attributes(Element* elem);

  /*! Calculate the sphere bound */
  virtual Boolean calculate_sphere_bound();
  
  /*! */
  void set_coord_array(Coord_array* coord_array);

  /*! Obtain the coordinate array */
  Coord_array* get_coord_array() const { return m_coord_array; }

  /*! */
  void set_normal_array(Normal_array* normal_array);

  /*! Obtain the normal array */
  Normal_array* get_normal_array() const { return m_normal_array; }

  /*! */
  void set_tex_coord_array(Tex_coord_array* tex_coord_array);

  /*! Obtain the texture-coordinate array */
  Tex_coord_array* get_tex_coord_array() const { return m_tex_coord_array; }

  /*! Set the color field */
  void set_color_array(Color_array* color_array);

  /*! Obtain the normal array */
  Color_array* get_color_array() const { return m_color_array; }

  /*! Obtain the coord-index array */
  const SGAL::Array<Uint>& get_coord_indices() const;

  /*! Obtain the color-index array */
  const SGAL::Array<Uint>& get_color_indices() const;

  /*! Obtain the normal-index array */
  const SGAL::Array<Uint>& get_normal_indices() const;

  /*! Obtain the texture-coord-index array */
  const SGAL::Array<Uint>& get_tex_coord_indices() const;

  /*! Obtain the coord-index array */
  const Uint* get_coord_indices_vector() const;
  
  /*! Obtain the color-index array */
  const Uint* get_color_indices_vector() const;

  /*! Obtain the normal-index array */
  const Uint* get_normal_indices_vector() const;

  /*! Obtain the texture-coord-index array */
  const Uint* get_tex_coord_indices_vector() const;

  /*! Obtain the i-th coord index */
  Uint get_coord_index(Uint i) const { return m_coord_indices[i]; }
  
  /*! Set the representation mode */
  void set_primitive_type(Primitive_type type) { m_primitive_type = type; }

  /*! Obtain the representation mode */
  Primitive_type get_primitive_type() const { return m_primitive_type; }
  
  /*! Determine whether the representation is empty */
  Boolean is_empty() const;

  /*! Determine whether the geometry has color (as opposed to material) */
  virtual Boolean has_color() const { return m_color_array != NULL; }  
  
  /*! Draw the geometry */
  virtual void draw(Draw_action* action) = 0;

  /*! Clean the representation */
  virtual void clean();

  /*! Determine whether the representation hasn't been cleaned */
  virtual Boolean is_dirty() const { return m_dirty; }
  
  /*! Obtain a pointer to an element directly */
  template <class T_Vector>
  GLfloat* get(T_Vector& array, Uint i) const
  { return ((GLfloat *) &(array)[i]); }

  /*! Obtain a pointer to an element through the coord indices */
  template <class T_Vector>
  GLfloat* get_by_ci(T_Vector& array, Uint i) const
  { return ((GLfloat *) &(array)[m_coord_indices[i]]); }

  /*! Obtain apointer to an element through the normal indices */
  template <class T_Vector>
  GLfloat* get_by_ni(T_Vector& array, Uint i) const
  { return ((GLfloat *) &(array)[m_normal_indices[i]]); }
  
  /*! Obtain apointer to an element through the color indices */
  template <class T_Vector>
  GLfloat* get_by_color_index(T_Vector& array, Uint i) const
  { return ((GLfloat *) &(array)[m_color_indices[i]]); }

  /*! Obtain a pointer to an element through the tex. coord indices */
  template <class T_Vector>
  GLfloat* get_by_ti(T_Vector & array, Uint i) const
  { return ((GLfloat *) &(array)[m_tex_coord_indices[i]]); }

  /*! Obtain the number of primitives */
  Uint get_num_primitives() const { return m_num_primitives; }

  /*! Set the number of primitives */
  void set_num_primitives(Uint num) { m_num_primitives = num; }

  /*! Process change of coordinates */
  virtual void coord_changed(Field_info* /* field_info */) {}

  /*! Process change of points */
  virtual void point_changed() {}
  
protected:
  /*! Indicates whether the mesh must be cleaned */
  Boolean m_dirty;

  /*! The number of primitives in this Geo_set */
  Uint m_num_primitives;

  /*! Indicates how normals are bound */
  Attachment m_normal_attachment;

  /*! Indicates how normals are bound */
  Attachment m_color_attachment;

  /*! An array of vertex ccordinates */
  Coord_array* m_coord_array;

  /*! An array if vertex normals */
  Normal_array* m_normal_array;

  /*! An array of vertex texture-ccordinates */
  Tex_coord_array* m_tex_coord_array;

  /*! An array if vertex colors */
  Color_array* m_color_array;
  
  /*! An array of indices into the vertex-coordinate array */
  SGAL::Array<Uint> m_coord_indices;

  /*! An array of indices into the vertex-normal array */
  SGAL::Array<Uint> m_tex_coord_indices;

  /*! An array of indices into the vertex-texture-coordinate array */
  SGAL::Array<Uint> m_normal_indices;

  /*! An array of indices into the vertex-color array */
  SGAL::Array<Uint> m_color_indices;

  /*! Indicates the geometry mode (triangles, quads, or general polygons) */
  Primitive_type m_primitive_type;

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
  
  /*! Proces the indices */
  void flatten_indices();

  /*! Resolve the conflict between normal and colors */
  Fragment_source resolve_fragment_source() const;

private:
  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The name of the representation modes */
  static const Char* s_primitive_type_names[];

  /*! The name of the attachments */
  static const Char* s_attachment_names[];
  
  static const Primitive_type s_def_primitive_type;

  /*! Intialize */
  void init();

  /*! Proces the indices */
  void flatten_indices(Uint* src, Uint* dst, Uint num);
};

/*! Obtain the color-index array */
inline const SGAL::Array<Uint>& Geo_set::get_coord_indices() const
{ return m_coord_indices; }

/*! Obtain the color-index array */
inline const SGAL::Array<Uint>& Geo_set::get_color_indices() const
{ return m_color_indices; }

/*! Obtain the normal-index array */
inline const SGAL::Array<Uint>& Geo_set::get_normal_indices() const
{ return m_normal_indices; }

/*! Obtain the texture-coord-index array */
inline const SGAL::Array<Uint>& Geo_set::get_tex_coord_indices() const
{ return m_tex_coord_indices; }

/*! Obtain the coord-index array */
inline const Uint* Geo_set::get_coord_indices_vector() const
{ return m_coord_indices.get_vector(); }

/*! Obtain the color-index array */
inline const Uint* Geo_set::get_color_indices_vector() const
{ return m_color_indices.get_vector(); }

/*! Obtain the normal-index array */
inline const Uint* Geo_set::get_normal_indices_vector() const
{ return m_normal_indices.get_vector(); }

/*! Obtain the texture-coord-index array */
inline const Uint* Geo_set::get_tex_coord_indices_vector() const
{ return m_tex_coord_indices.get_vector(); }

/*! Resolves the conflict between normal and colors. If the color array is
 * present, we use colors as source (disable lighting, etc.).
 */
inline Geo_set::Fragment_source Geo_set::resolve_fragment_source() const
{
  return (m_color_array && m_color_array->size()) ? FS_COLOR : FS_NORMAL;
}
  
SGAL_END_NAMESPACE

#endif
