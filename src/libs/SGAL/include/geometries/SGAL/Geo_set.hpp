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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_GEO_SET_HPP
#define SGAL_GEO_SET_HPP

/*! \file
 * A base class for mesh objects.
 *
 *  Inherits from Geometry.
 */

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <sys/types.h>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Bounding_box.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Normal_array;
class Index_array;
class Container_proto;
class Element;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! Geo_set describes geometry using a set of arrays that represent the
 * coordinates, normals, colors, and texture-coordinates of the vertices
 * of the geometry, and corresponding index arrays into the former resp.
 */
class SGAL_SGAL_DECL Geo_set : public Geometry {
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
    AT_NONE = 0x0,
    AT_PER_VERTEX = 0x1,
    AT_PER_PRIMITIVE = 0x2,
    AT_PER_MESH = 0x3
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

  /// \name field handlers
  //@{
  Shared_coord_array* coord_array_handle(const Field_info*)
  { return &m_coord_array; }
  Shared_normal_array* normal_array_handle(const Field_info*)
  { return &m_normal_array; }
  Shared_color_array* color_array_handle(const Field_info*)
  { return &m_color_array; }
  Shared_tex_coord_array* tex_coord_array_handle(const Field_info*)
  { return &m_tex_coord_array; }
  std::vector<Uint>* coord_indices_handle(const Field_info*)
  { return &m_coord_indices; }
  std::vector<Uint>* normal_indices_handle(const Field_info*)
  { return &m_normal_indices; }
  std::vector<Uint>* color_indices_handle(const Field_info*)
  { return &m_color_indices; }
  std::vector<Uint>* tex_coord_indices_handle(const Field_info*)
  { return &m_tex_coord_indices; }
  //@}

  /*! Set the attributes of the object extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values.
   */
  virtual void set_attributes(Element* elem);

  /*! Calculate the sphere bound.
   * \return true if the BS has changed since lst time this was called.
   * The implementation assumes that m_coord_array points at an object of
   * type Coord_array_3d. If this is not the case, this virtual member
   * function must be overriden!
   */
  virtual Boolean clean_sphere_bound();

  /*! Process change of data that causes a change to the sphere bound.
   *  (i) Indicate the change in the current node (through a call to the
   *      corresponding member of the base class).
   * (ii) Indicate the change in the parent nodes (through a call to
   *      field_changed(), which in turn appliesthe observers).
   * \param field_info (in) the information record of the field that caused
   *                   the change.
   */
  virtual void sphere_bound_changed(const Field_info* field_info = nullptr);

  /*! Determine whether the geometry has color.
   */
  virtual Boolean has_color();

  /*! Determine whether the geometry has texture coordinates.
   */
  virtual Boolean has_tex_coord();

  /*! Set the coordinate array.
   * \param coord_array (in) a pointer to a coordinate array
   */
  virtual void set_coord_array(Shared_coord_array coord_array);

  /*! Obtain the bounding box.
   */
  virtual Bounding_box bounding_box();

  /*! Obtain the (const) coordinate array.
   * \return the coordinate array.
   */
  const Shared_coord_array get_coord_array() const;

  /*! Obtain the (non-const) coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Set the normal array.
   * \param normal_array (in) the normal array.
   */
  virtual void set_normal_array(Shared_normal_array normal_array);

  /*! Obtain the (const) normal array.
   * \return the normal array.
   */
  const Shared_normal_array get_normal_array() const;

  /*! Obtain the (non-const) normal array.
   * \return the normal array.
   */
  virtual Shared_normal_array get_normal_array();

  /*! Set the texture-coordinate array.
   * \param tex_coord_array (in) the texture coordinate array.
   */
  virtual void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /*! Obtain the (const) texture-coordinate array.
   * \return the texture-coordinate array.
   */
  const Shared_tex_coord_array get_tex_coord_array() const;

  /*! Obtain the (non-const) texture-coordinate array.
   * \return the texture-coordinate array.
   */
  virtual Shared_tex_coord_array get_tex_coord_array();

  /*! Set the color field.
   * \param color_array (in) a pointer to a color array
   */
  virtual void set_color_array(Shared_color_array color_array);

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
  void set_coord_indices(const std::vector<Uint>& indices);

  /*! Obtain the (const) coordinate-index array.
   * \return the coordinate-index array.
   */
  const std::vector<Uint>& get_coord_indices() const;

  /*! Obtain the (non-const) coordinate-index array.
   * \return the coordinate-index array.
   */
  std::vector<Uint>& get_coord_indices();

  /*! Set the color-index array.
   * \param indices (in)
   */
  void set_color_indices(const std::vector<Uint>& indices);

  /*! Obtain the (const) color-index array.
   * \return the color-index array.
   */
  const std::vector<Uint>& get_color_indices() const;

  /*! Obtain the (non-const) color-index array.
   * \return the color-index array.
   */
  std::vector<Uint>& get_color_indices();

  /*! Set the normal-index array.
   * \param indices (in)
   */
  void set_normal_indices(const std::vector<Uint>& indices);

  /*! Obtain the (const) normal-index array.
   * \return the normal-index array.
   */
  const std::vector<Uint>& get_normal_indices() const;

  /*! Obtain the (non-const) normal-index array.
   * \return the normal-index array.
   */
  std::vector<Uint>& get_normal_indices();

  /*! Set the texture_coordinate-index array.
   * \param indices (in)
   */
  void set_tex_coord_indices(const std::vector<Uint>& indices);

  /*! Obtain the (const) texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  const std::vector<Uint>& get_tex_coord_indices() const;

  /*! Obtain the (non-const) texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  std::vector<Uint>& get_tex_coord_indices();

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

  /*! Obtain the ith coord index.
   * \return the ith coord index.
   */
  Uint get_coord_index(Uint i) const;

  /*! Obtain the ith normal index.
   * \return the ith normal index.
   */
  Uint get_normal_index(Uint i) const;

  /*! Obtain the ith color index.
   * \return the ith color index.
   */
  Uint get_color_index(Uint i) const;

  /*! Obtain the ith tex-coord index.
   * \return the ith tex-coord index.
   */
  Uint get_tex_coord_index(Uint i) const;

  /*! Determine whether the representation is empty. */
  Boolean is_empty() const;

  /*! Obtain the ith 3D coordinate.
   */
  virtual const Vector3f& get_coord_3d(Uint i) const;

  /*! Obtain a pointer to an element directly. */
  template <typename Array_T>
  const GLfloat* get(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the coord indices. */
  template <typename Array_T>
  const GLfloat* get_by_coord_index(Array_T array, Uint i) const;

  /*! Obtain apointer to an element through the normal indices. */
  template <typename Array_T>
  const GLfloat* get_by_normal_index(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the color indices. */
  template <typename Array_T>
  const GLfloat* get_by_color_index(Array_T array, Uint i) const;

  /*! Obtain a pointer to an element through the tex. coord indices. */
  template <typename Array_T>
  const GLfloat* get_by_tex_coord_index(Array_T array, Uint i) const;

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
  virtual void coord_changed(const Field_info* field_info);

  /*! Process change of normal field.
   * Generally process the change of normal field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void normal_changed(const Field_info* field_info);

  /*! Process change of color field.
   * Generally process the change of color field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void color_changed(const Field_info* field_info);

  /*! Process change of texture coordinate field.
   * Generally process the change of texture coordinate field.
   * We assume that any change requires the general processing of a field.
   * Notice that by default the general processing of a field of a container
   * (i.e., this container) consists of the processing of all observers of
   * that container.
   */
  virtual void tex_coord_changed(const Field_info* field_info);

  /*! Process change of field.
   */
  virtual void field_changed(const Field_info* field_info);

  /*! Assign the coord indices with the reverse of given indices.
   * \param indices the indices to reverse.
   */
  void set_reverse_coord_indices(const std::vector<Uint>& indices);

protected:
  /*! The number of primitives in this Geo_set. */
  Uint m_num_primitives;

  /*! Indicates how normals are bound.
   * By default, normals are bound per primitive (e.g., polygon, triangle,
   * and auad). Onserve that this is different than the specification of VRML,
   * where normalPerVertex is TRUE by default. This is rectified in
   * Indexed_face_set, where colorPerVertex, normalPerVertex, and creaseAngle
   * are introduced.
   */
  Attachment m_normal_attachment;

  /*! Indicates how normals are bound.
   * By default, colors are bound per primitive (e.g., polygon, triangle,
   * and auad). Onserve that this is different than the specification of VRML,
   * where colorPerVertex is TRUE by default. This is rectified in
   * Indexed_face_set, where colorPerVertex, normalPerVertex, and creaseAngle
   * are introduced.
   */
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
  std::vector<Uint> m_coord_indices;

  /*! An array of indices into the vertex-texture-coordinate array. */
  std::vector<Uint> m_normal_indices;

  /*! An array of indices into the vertex-color array. */
  std::vector<Uint> m_color_indices;

  /*! An array of indices into the vertex-normal array. */
  std::vector<Uint> m_tex_coord_indices;

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

  // Default values
  static const Primitive_type s_def_primitive_type;

  /*! Intialize. */
  void init();
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief determines whether colors are generated by the geometry.
inline Boolean Geo_set::has_color()
{ return (m_color_array && (m_color_array->size() != 0)); }

//! \brief determines whether the geometry has texture coordinates.
inline Boolean Geo_set::has_tex_coord()
{ return (m_tex_coord_array && (m_tex_coord_array->size() != 0)); }

//! \brief sets the coordinate-index array.
inline void Geo_set::set_coord_indices(const std::vector<Uint>& indices)
{ m_coord_indices = indices; }

//! \brief obtains the (const) coordinate-index array.
inline const std::vector<Uint>& Geo_set::get_coord_indices() const
{ return m_coord_indices; }

//! \brief obtains the (non-const) coordinate-index array.
inline std::vector<Uint>& Geo_set::get_coord_indices()
{ return m_coord_indices; }

//! \brief sets the color-index array.
inline void Geo_set::set_color_indices(const std::vector<Uint>& indices)
{ m_color_indices = indices; }

//! \brief obtains the (const) color-index array.
inline const std::vector<Uint>& Geo_set::get_color_indices() const
{ return m_color_indices; }

//! \brief obtains the (non-const) color-index array.
inline std::vector<Uint>& Geo_set::get_color_indices()
{ return m_color_indices; }

//! \brief sets the normal-index array.
inline void Geo_set::set_normal_indices(const std::vector<Uint>& indices)
{ m_normal_indices = indices; }

//! \brief obtains the (const) normal-index array.
inline const std::vector<Uint>& Geo_set::get_normal_indices() const
{ return m_normal_indices; }

//! \brief obtains the (non-const) normal-index array.
inline std::vector<Uint>& Geo_set::get_normal_indices()
{ return m_normal_indices; }

//! \brief sets the texture-coordinate-index array.
inline void Geo_set::set_tex_coord_indices(const std::vector<Uint>& indices)
{ m_tex_coord_indices = indices; }

//! \brief obtains the (const) texture-coordinate-index array.
inline const std::vector<Uint>& Geo_set::get_tex_coord_indices() const
{ return m_tex_coord_indices; }

//! \brief obtains the (non-const) texture-coordinate-index array.
inline std::vector<Uint>& Geo_set::get_tex_coord_indices()
{ return m_tex_coord_indices; }

//! \brief obtains the (const) coordinate-index vector.
inline const Uint* Geo_set::get_coord_indices_vector() const
{ return &(*(m_coord_indices.begin())); }

//! \brief obtains the (non-const) coordinate-index vector.
inline Uint* Geo_set::get_coord_indices_vector()
{ return &(*(m_coord_indices.begin())); }

//! \brief obtains the (const) color-index vector.
inline const Uint* Geo_set::get_color_indices_vector() const
{ return &(*(m_color_indices.begin())); }

//! \brief obtains the (non-const) color-index vector.
inline Uint* Geo_set::get_color_indices_vector()
{ return &(*(m_color_indices.begin())); }

//! \brief obtains the (const) normal-index vector.
inline const Uint* Geo_set::get_normal_indices_vector() const
{ return &(*(m_normal_indices.begin())); }

//! \brief obtains the (non-const) normal-index vector.
inline Uint* Geo_set::get_normal_indices_vector()
  { return &(*(m_normal_indices.begin())); }

//! \brief obtains the (const) texture-coordinate-index vector.
inline const Uint* Geo_set::get_tex_coord_indices_vector() const
{ return &(*(m_tex_coord_indices.begin())); }

//! \brief obtains the (non-const) texture-coordinate-index vector.
inline Uint* Geo_set::get_tex_coord_indices_vector()
{ return &(*(m_tex_coord_indices.begin())); }

//! \brief Obtain the i-th coord index.
inline Uint Geo_set::get_coord_index(Uint i) const
{ return m_coord_indices[i]; }

//! \brief obtains the ith normal index.
inline Uint Geo_set::get_normal_index(Uint i) const
{ return m_normal_indices[i]; }

//! \brief obtains the ith color index.
inline Uint Geo_set::get_color_index(Uint i) const
{ return m_color_indices[i]; }

//! \brief obtains the ith tex-coord index.
inline Uint Geo_set::get_tex_coord_index(Uint i) const
{ return m_tex_coord_indices[i]; }

/*! \brief resolvess the conflict between normal and colors. If the color
 * array is present, we use colors as source (disable lighting, etc.).
 */
inline Geo_set::Fragment_source Geo_set::resolve_fragment_source() const
{ return (m_color_array && m_color_array->size()) ? FS_COLOR : FS_NORMAL; }

//! \brief obtains the (const) coordinate array.
inline const Geo_set::Shared_coord_array Geo_set::get_coord_array() const
{ return m_coord_array; }

//! \brief obtains the (non-const) coordinate array.
inline Geo_set::Shared_coord_array Geo_set::get_coord_array()
{ return m_coord_array; }

//! \brief obtains the (const) normal array.
inline const Geo_set::Shared_normal_array Geo_set::get_normal_array() const
{ return m_normal_array; }

//! \brief obtains the (non-const) normal array.
inline Geo_set::Shared_normal_array Geo_set::get_normal_array()
{ return m_normal_array; }

//! \brief obtains the (const) texture-coordinate array.
inline const Geo_set::Shared_tex_coord_array Geo_set::get_tex_coord_array()
  const
{ return m_tex_coord_array; }

//! \brief obtains the (non-const) texture-coordinate array.
inline Geo_set::Shared_tex_coord_array Geo_set::get_tex_coord_array()
{ return m_tex_coord_array; }

//! \brief obtains the (const) normal array.
inline const Geo_set::Shared_color_array Geo_set::get_color_array() const
{ return m_color_array; }

//! \brief obtains the (non-const) normal array.
inline Geo_set::Shared_color_array Geo_set::get_color_array()
{ return m_color_array; }

//! \brief obtains a pointer to an element directly.
template <typename Array_T>
inline const GLfloat* Geo_set::get(Array_T array, Uint i) const
{ return array->datum(i); }

//! \brief obtains a pointer to an element through the coord indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_coord_index(Array_T array, Uint i) const
{ return array->datum(m_coord_indices[i]); }

//! \brief obtains a pointer to an element through the normal indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_normal_index(Array_T array, Uint i) const
{ return array->datum(m_normal_indices[i]); }

//! \brief obtains a pointer to an element through the color indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_color_index(Array_T array, Uint i) const
{ return array->datum(m_color_indices[i]); }

//! \brief obtains a pointer to an element through the tex. coord indices.
template <typename Array_T>
inline const GLfloat* Geo_set::get_by_tex_coord_index(Array_T array, Uint i)
  const
{ return array->datum(m_tex_coord_indices[i]); }

//! \brief sets the representation mode. */
inline void Geo_set::set_primitive_type(Primitive_type type)
{ m_primitive_type = type; }

//! \brief obtains the representation mode.
inline Geo_set::Primitive_type Geo_set::get_primitive_type() const
{ return m_primitive_type; }

//! \brief obtains the number of primitives.
inline Uint Geo_set::get_num_primitives() const { return m_num_primitives; }

//! \brief sets the number of primitives.
inline void Geo_set::set_num_primitives(Uint num) { m_num_primitives = num; }

SGAL_END_NAMESPACE

#endif
