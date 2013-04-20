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
#include <boost/shared_ptr.hpp>

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

  typedef boost::shared_ptr<Coord_array>                Shared_coord_array;
  typedef boost::shared_ptr<Normal_array>               Shared_normal_array;
  typedef boost::shared_ptr<Color_array>                Shared_color_array;
  typedef boost::shared_ptr<Tex_coord_array>            Shared_tex_coord_array;

  /*! Constructor. */
  Geo_set(Boolean proto = false);

  /*! Destructor. */
  virtual ~Geo_set();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Calculate the sphere bound.
   * \return true if the BS has changed since lst time this was called.
   */
  virtual Boolean clean_sphere_bound();
  
  /*! Determine whether colors are generated by the geometry. */
  virtual Boolean are_generated_color();
  
  /*! Determine whether texture coordinates are generated by the geometry. */
  virtual Boolean are_generated_tex_coord();

  /*! Set the coordinate array.
   * \param coord_array (in) a pointer to a coordinate array
   */
  void set_coord_array(Shared_coord_array coord_array);

  /*! Obtain the (const) coordinate array.
   * \return the coordinate array.
   */
  const Shared_coord_array get_coord_array() const;

  /*! Obtain the (non-const) coordinate array.
   * \return the coordinate array.
   */
  Shared_coord_array get_coord_array();

  /*! Set the normal array.
   * \param normal_array (in) the normal array.
   */
  void set_normal_array(Shared_normal_array normal_array);

  /*! Obtain the (const) normal array.
   * \return the normal array.
   */
  const Shared_normal_array get_normal_array() const;

  /*! Obtain the (non-const) normal array.
   * \return the normal array.
   */
  Shared_normal_array get_normal_array();

  /*! Set the texture-coordinate array.
   * \param tex_coord_array (in) the texture coordinate array.
   */
  void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /*! Obtain the (const) texture-coordinate array.
   * \return the texture-coordinate array.
   */
  const Shared_tex_coord_array get_tex_coord_array() const;

  /*! Obtain the (non-const) texture-coordinate array.
   * \return the texture-coordinate array.
   */
  Shared_tex_coord_array get_tex_coord_array();

  /*! Set the color field.
   * \param color_array (in) a pointer to a color array
   */
  void set_color_array(Shared_color_array color_array);

  /*! Obtain the (const) normal array.
   * \return the color array.
   */
  const Shared_color_array get_color_array() const;

  /*! Obtain the (non-const) normal array.
   * \return the color array.
   */
  Shared_color_array get_color_array();

  /*! Set the coordinate-index array.
   * \param indices (in)
   */
  void set_coord_indices(const Array<Uint>& indices);

  /*! Obtain the (const) coordinate-index array.
   * \return the coordinate-index array.
   */
  const Array<Uint>& get_coord_indices() const;

  /*! Obtain the (non-const) coordinate-index array.
   * \return the coordinate-index array.
   */
  Array<Uint>& get_coord_indices();

  /*! Set the color-index array.
   * \param indices (in)
   */
  void set_color_indices(const Array<Uint>& indices);

  /*! Obtain the (const) color-index array.
   * \return the color-index array.
   */
  const Array<Uint>& get_color_indices() const;

  /*! Obtain the (non-const) color-index array.
   * \return the color-index array.
   */
  Array<Uint>& get_color_indices();

  /*! Set the normal-index array.
   * \param indices (in)
   */
  void set_normal_indices(const Array<Uint>& indices);

  /*! Obtain the (const) normal-index array.
   * \return the normal-index array.
   */
  const Array<Uint>& get_normal_indices() const;

  /*! Obtain the (non-const) normal-index array.
   * \return the normal-index array.
   */
  Array<Uint>& get_normal_indices();

  /*! Set the texture_coordinate-index array.
   * \param indices (in)
   */
  void set_tex_coord_indices(const Array<Uint>& indices);

  /*! Obtain the (const) texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  const Array<Uint>& get_tex_coord_indices() const;

  /*! Obtain the (non-const) texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  Array<Uint>& get_tex_coord_indices();

  /*! Obtain the (const) coordinate-index vector.
   * \return the coordinate-index array.
   */
  const Uint* get_coord_indices_vector() const;

  /*! Obtain the (non-const) coordinate-index vector.
   * \return the coordinate-index array.
   */
  Uint* get_coord_indices_vector();
  
  /*! Obtain the (const) color-index vector.
   * \return the color-index array.
   */
  const Uint* get_color_indices_vector() const;

  /*! Obtain the (non-const) color-index vector.
   * \return the color-index array.
   */
  Uint* get_color_indices_vector();

  /*! Obtain the (const) normal-index vector.
   * \return the normal-index array.
   */
  const Uint* get_normal_indices_vector() const;

  /*! Obtain the (non-const) normal-index vector.
   * \return the normal-index array.
   */
  Uint* get_normal_indices_vector();

  /*! Obtain the (const) texture-coordinate-index vector.
   * \return the texture-coordinate-index array.
   */
  const Uint* get_tex_coord_indices_vector() const;

  /*! Obtain the (non-const) texture-coordinate-index vector.
   * \return the texture-coordinate-index array.
   */
  Uint* get_tex_coord_indices_vector();

  /*! Obtain the i-th coord index.
   */
  Uint get_coord_index(Uint i) const;
  
  /*! Determine whether the representation is empty. */
  Boolean is_empty() const;

  /*! Obtain a pointer to an element directly. */
  template <typename T_Vector>
  GLfloat* get(T_Vector& array, Uint i) const;

  /*! Obtain a pointer to an element through the coord indices. */
  template <typename T_Vector>
  GLfloat* get_by_coord_index(T_Vector& array, Uint i) const;

  /*! Obtain apointer to an element through the normal indices. */
  template <typename T_Vector>
  GLfloat* get_by_normal_index(T_Vector& array, Uint i) const;
  
  /*! Obtain a pointer to an element through the color indices. */
  template <typename T_Vector>
  GLfloat* get_by_color_index(T_Vector& array, Uint i) const;

  /*! Obtain a pointer to an element through the tex. coord indices. */
  template <typename T_Vector>
  GLfloat* get_by_tex_coord_index(T_Vector & array, Uint i) const;

  /*! Set the representation mode. */
  void set_primitive_type(Primitive_type type);

  /*! Obtain the representation mode. */
  Primitive_type get_primitive_type() const;
  
  /*! Obtain the number of primitives. */
  Uint get_num_primitives() const;

  /*! Set the number of primitives. */
  void set_num_primitives(Uint num);

  /*! Draw the geometry. */
  virtual void draw(Draw_action* action) = 0;

  /*! Process change of coordinate field.
   * Generally process the change of coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void coord_changed(Field_info* field_info);

  /*! Process change of normal field.
   * Generally process the change of normal field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void normal_changed(Field_info* field_info);

  /*! Process change of color field.
   * Generally process the change of color field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void color_changed(Field_info* field_info);

  /*! Process change of texture coordinate field.
   * Generally process the change of texture coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void tex_coord_changed(Field_info* field_info);
  
protected:
  /*! The number of primitives in this Geo_set. */
  Uint m_num_primitives;

  /*! Indicates how normals are bound. */
  Attachment m_normal_attachment;

  /*! Indicates how normals are bound. */
  Attachment m_color_attachment;

  /*! An array of vertex ccordinates. */
  Shared_coord_array m_coord_array;

  /*! An array of normals. */
  Shared_normal_array m_normal_array;

  /*! An array of vertex texture-ccordinates. */
  Shared_tex_coord_array m_tex_coord_array;

  /*! An array if vertex colors. */
  Shared_color_array m_color_array;
  
  /*! An array of indices into the vertex-coordinate array. */
  SGAL::Array<Uint> m_coord_indices;

  /*! An array of indices into the vertex-normal array. */
  SGAL::Array<Uint> m_tex_coord_indices;

  /*! An array of indices into the vertex-texture-coordinate array. */
  SGAL::Array<Uint> m_normal_indices;

  /*! An array of indices into the vertex-color array. */
  SGAL::Array<Uint> m_color_indices;

  /*! Indicates the geometry mode (triangles, quads, or general polygons). */
  Primitive_type m_primitive_type;

  /*! Resolve the conflict between normal and colors. */
  Fragment_source resolve_fragment_source() const;

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The name of the representation modes. */
  static const Char* s_primitive_type_names[];

  /*! The name of the attachments. */
  static const Char* s_attachment_names[];
  
  static const Primitive_type s_def_primitive_type;

  /*! Intialize. */
  void init();
};

/*! \brief determines whether colors are generated by the geometry. */
inline Boolean Geo_set::are_generated_color()
{ return (m_generated_color && m_color_array); }  
  
/*! \brief determines whether texture coordinates are generated by the geometry.
 */
inline Boolean Geo_set::are_generated_tex_coord()
{ return (m_generated_tex_coord && m_tex_coord_array); }  

/*! \brief sets the coordinate-index array. */
inline void Geo_set::set_coord_indices(const Array<Uint>& indices)
{ m_coord_indices = indices; }

/*! \brief obtains the (const) coordinate-index array. */
inline const SGAL::Array<Uint>& Geo_set::get_coord_indices() const
{ return m_coord_indices; }

/*! \brief obtains the (non-const) coordinate-index array. */
inline SGAL::Array<Uint>& Geo_set::get_coord_indices()
{ return m_coord_indices; }

/*! \brief sets the color-index array. */
inline void Geo_set::set_color_indices(const Array<Uint>& indices)
{ m_color_indices = indices; }

/*! \brief obtains the (const) color-index array. */
inline const SGAL::Array<Uint>& Geo_set::get_color_indices() const
{ return m_color_indices; }

/*! \brief obtains the (non-const) color-index array. */
inline SGAL::Array<Uint>& Geo_set::get_color_indices()
{ return m_color_indices; }

/*! \brief sets the normal-index array. */
inline void Geo_set::set_normal_indices(const Array<Uint>& indices)
{ m_normal_indices = indices; }

/*! \brief obtains the (const) normal-index array. */
inline const SGAL::Array<Uint>& Geo_set::get_normal_indices() const
{ return m_normal_indices; }

/*! \brief obtains the (non-const) normal-index array. */
inline SGAL::Array<Uint>& Geo_set::get_normal_indices()
{ return m_normal_indices; }

/*! \brief sets the texture-coordinate-index array. */
inline void Geo_set::set_tex_coord_indices(const Array<Uint>& indices)
{ m_tex_coord_indices = indices; }

/*! \brief obtains the (const) texture-coordinate-index array. */
inline const SGAL::Array<Uint>& Geo_set::get_tex_coord_indices() const
{ return m_tex_coord_indices; }

/*! \brief obtains the (non-const) texture-coordinate-index array. */
inline SGAL::Array<Uint>& Geo_set::get_tex_coord_indices()
{ return m_tex_coord_indices; }

/*! \brief obtains the (const) coordinate-index vector. */
inline const Uint* Geo_set::get_coord_indices_vector() const
{ return m_coord_indices.get_vector(); }

/*! \brief obtains the (non-const) coordinate-index vector. */
inline Uint* Geo_set::get_coord_indices_vector()
{ return m_coord_indices.get_vector(); }

/*! \brief obtains the (const) color-index vector. */
inline const Uint* Geo_set::get_color_indices_vector() const
{ return m_color_indices.get_vector(); }

/*! \brief obtains the (non-const) color-index vector. */
inline Uint* Geo_set::get_color_indices_vector()
{ return m_color_indices.get_vector(); }

/*! \brief obtains the (const) normal-index vector. */
inline const Uint* Geo_set::get_normal_indices_vector() const
{ return m_normal_indices.get_vector(); }

/*! \brief obtains the (non-const) normal-index vector. */
inline Uint* Geo_set::get_normal_indices_vector()
{ return m_normal_indices.get_vector(); }

/*! \brief obtains the (const) texture-coordinate-index vector. */
inline const Uint* Geo_set::get_tex_coord_indices_vector() const
{ return m_tex_coord_indices.get_vector(); }

/*! \brief obtains the (non-const) texture-coordinate-index vector. */
inline Uint* Geo_set::get_tex_coord_indices_vector()
{ return m_tex_coord_indices.get_vector(); }

/*! \brief Obtain the i-th coord index. */
inline Uint Geo_set::get_coord_index(Uint i) const
{ return m_coord_indices[i]; }
  
/*! \brief resolvess the conflict between normal and colors. If the color
 * array is present, we use colors as source (disable lighting, etc.).
 */
inline Geo_set::Fragment_source Geo_set::resolve_fragment_source() const
{ return (m_color_array && m_color_array->size()) ? FS_COLOR : FS_NORMAL; }

/*! \brief obtains the (const) coordinate array. */
inline const Geo_set::Shared_coord_array Geo_set::get_coord_array() const
{ return m_coord_array; }

/*! \brief obtains the (non-const) coordinate array. */
inline Geo_set::Shared_coord_array Geo_set::get_coord_array()
{ return m_coord_array; }

/*! \brief obtains the (const) normal array. */
inline const Geo_set::Shared_normal_array Geo_set::get_normal_array() const
{ return m_normal_array; }

/*! \brief obtains the (non-const) normal array. */
inline Geo_set::Shared_normal_array Geo_set::get_normal_array()
{ return m_normal_array; }

/*! \brief obtains the (const) texture-coordinate array. */
inline const Geo_set::Shared_tex_coord_array Geo_set::get_tex_coord_array()
  const
{ return m_tex_coord_array; }

/*! \brief obtains the (non-const) texture-coordinate array. */
inline Geo_set::Shared_tex_coord_array Geo_set::get_tex_coord_array()
{ return m_tex_coord_array; }

/*! \brief obtains the (const) normal array. */
inline const Geo_set::Shared_color_array Geo_set::get_color_array() const
{ return m_color_array; }

/*! \brief obtains the (non-const) normal array. */
inline Geo_set::Shared_color_array Geo_set::get_color_array()
{ return m_color_array; }

/*! \brief obtains a pointer to an element directly. */
template <typename T_Vector>
inline GLfloat* Geo_set::get(T_Vector& array, Uint i) const
{ return ((GLfloat *) &(array)[i]); }

/*! \brief obtains a pointer to an element through the coord indices. */
template <typename T_Vector>
inline GLfloat* Geo_set::get_by_coord_index(T_Vector& array, Uint i) const
{ return ((GLfloat *) &(array)[m_coord_indices[i]]); }

/*! \brief obtains a pointer to an element through the normal indices. */
template <typename T_Vector>
inline GLfloat* Geo_set::get_by_normal_index(T_Vector& array, Uint i) const
{ return ((GLfloat *) &(array)[m_normal_indices[i]]); }
  
/*! \brief obtains a pointer to an element through the color indices. */
template <typename T_Vector>
inline GLfloat* Geo_set::get_by_color_index(T_Vector& array, Uint i) const
{ return ((GLfloat *) &(array)[m_color_indices[i]]); }

/*! \brief obtains a pointer to an element through the tex. coord indices. */
template <typename T_Vector>
inline GLfloat* Geo_set::get_by_tex_coord_index(T_Vector & array, Uint i) const
{ return ((GLfloat *) &(array)[m_tex_coord_indices[i]]); }

/*! \brief Set the representation mode. */
inline void Geo_set::set_primitive_type(Primitive_type type)
{ m_primitive_type = type; }

/*! \brief Obtain the representation mode. */
inline Geo_set::Primitive_type Geo_set::get_primitive_type() const
{ return m_primitive_type; }

/*! \brief obtains the number of primitives. */
inline Uint Geo_set::get_num_primitives() const { return m_num_primitives; }

/*! \brief sets the number of primitives. */
inline void Geo_set::set_num_primitives(Uint num) { m_num_primitives = num; }

SGAL_END_NAMESPACE

#endif
