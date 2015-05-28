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

#ifndef SGAL_MESH_SET_HPP
#define SGAL_MESH_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Field_info;
class Formatter;
class Stl_formatter;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Mesh_set : public Geo_set {
public:
  enum {
    FIRST = Geo_set::LAST - 1,
    CCW,
    SOLID,
    CONVEX,
    CREASE_ANGLE,
    LAST
  };

  /* The type Index_array is used to store an array of unsigned int indices.
   * Arrays of such type are used as arguments in OpenGL calls, where the
   * type of the index, namely unsigned int, is hard coded.
   */
  typedef unsigned int                                  Index_type;
  typedef std::vector<Index_type>                       Index_array;

  /*! Constructor.
   */
  Mesh_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Mesh_set();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* is_ccw_handle(const Field_info*) { return &m_is_ccw; }
  Boolean* is_solid_handle(const Field_info*) { return &m_is_solid; }
  Boolean* is_convex_handle(const Field_info*) { return &m_is_convex; }
  Float* crease_angle_handle(const Field_info*) { return &m_crease_angle; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly.
   */
  virtual void draw(Draw_action* action);

  /*! Draw the mesh (unconditionaly).
   */
  virtual void draw_mesh(Draw_action* action);

  /*! Draw the representation. */
  virtual void draw_geometry(Draw_action* action) = 0;

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Obtain the bounding box.
   */
  virtual Bounding_box bounding_box();

  /// \name Field Getters and Setters
  //@{
  /*! Set the counter-clockwise flag.
   */
  void set_ccw(Boolean ccw);

  /*! Determine whether the surface polygons are counter-clockwise.
   */
  Boolean is_ccw() const;

  /*! Set the flag that indicates whether the mesh represents is solid
   * (watertight) polytope. In case the mesh represents a solid polytope,
   * the back faces can be culled out during rendering, and the front faces
   * can be rendered single sided.
   */
  void set_solid(Boolean solid);

  /*! Determines whether the mesh represents a solid (watertight) polytope. */
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
  //@}

  /// \name Index-Arrays Getters
  //@{
  /*! Obtain the coordinate-index array.
   * \return the coordinate-index array.
   */
  virtual std::vector<Int32>& get_coord_indices();

  /*! Obtain the normal-index array.
   * \return the normal-index array.
   */
  virtual std::vector<Int32>& get_normal_indices();

  /*! Obtain the color-index array.
   * \return the color-index array.
   */
  virtual std::vector<Int32>& get_color_indices();

  /*! Obtain the texture-coordinate-index array.
   * \return the texture-coordinate-index array.
   */
  virtual std::vector<Int32>& get_tex_coord_indices();
  //@}

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the normal-index array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void normal_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the color-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the texture-coordinate index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_indices_changed(const Field_info* field_info);
  //@}

  /// \name Cleaners
  //@{
  /*! Validate (clean) the coordinate-index array. */
  void clean_coord_indices();

  /*! Validate (clean) the normal-index array. */
  void clean_normal_indices();

  /*! Validate (clean) the color-index array. */
  void clean_color_indices();

  /*! Validate (clean) the texture coordinate-index array. */
  void clean_tex_coord_indices();

  /*! Clean the bounding sphere of the mesh set. */
  virtual void clean_sphere_bound();
  //@}

  /// \name Clearers
  //@{
  /*! Invalidate (clear) the coordinate-index array. */
  void clear_coord_indices();

  /*! Invalidate (clear) the normal-index array. */
  void clear_normal_indices();

  /*! Invalidate (clear) the color-index array. */
  void clear_color_indices();

  /*! Invalidate (clear) the texture coordinate-index array. */
  void clear_tex_coord_indices();
  //@}

  /// \name Index-Array Predicates
  //@{
  /*! Determine whether the coordinate-index array has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the coordinate-index array
   *         is dirty.
   */
  Boolean is_dirty_coord_indices() const;

  /*! Determine whether the normal-index array has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the normal-index array is
   *         dirty.
   */
  Boolean is_dirty_normal_indices() const;

  /*! Determine whether the color-index array has been invalidated
   * (is dirty).
   * \return a Boolean flag that indicates whether the color-index array is
   *         dirty.
   */
  Boolean is_dirty_color_indices() const;

  /*! Determine whether the texture coordinate-index array has been
   * invalidated (is dirty).
   * \return a Boolean flag that indicates whether the texture coordinate-index
   *         array is dirty.
   */
  Boolean is_dirty_tex_coord_indices() const;
  //@}

  /// \name Flat Index Arrays
  //@{
  /*! Obtain the flat coordinate-index array.
   */
  Index_array& get_flat_coord_indices();

  /*! Obtain the flat normal-index array.
   */
  Index_array& get_flat_normal_indices();

  /*! Obtain the flat color-index array.
   */
  Index_array& get_flat_color_indices();

  /*! Obtain the flat texture coordinate-index array.
   */
  Index_array& get_flat_tex_coord_indices();

  /*! Set the flat coordinate-index array.
   */
  void set_flat_coord_indices(const Index_array& indices);

  /*! Set the flat normal-index array.
   */
  void set_flat_normal_indices(const Index_array& indices);

  /*! Set the flat color-index array.
   */
  void set_flat_color_indices(const Index_array& indices);

  /*! Set the flat texture coordinate-index array.
   */
  void set_flat_tex_coord_indices(const Index_array& indices);

   /*! Validate (clean) the flat coordinate-index array.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the index array. This operation changes the structure of the
   * index array, and must be reflected in the drawing routines.
   */
  virtual void clean_flat_coord_indices();

  /*! Validate (clean) the flat normal-index array.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the index array. This operation changes the structure of the
   * index array, and must be reflected in the drawing routines.
   */
  virtual void clean_flat_normal_indices();

  /*! Validate (clean) the flat color-index array.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the index array. This operation changes the structure of the
   * index array, and must be reflected in the drawing routines.
   */
  virtual void clean_flat_color_indices();

  /*! Validate (clean) the flat texture coord-index array.
   * In case of triangles or quads, remove the '-1' end-of-polygon indication
   * from the index array. This operation changes the structure of the
   * index array, and must be reflected in the drawing routines.
   */
  virtual void clean_flat_tex_coord_indices();

  /*! Invalidate (clear) the flat index arrays, i.e., coordinate-index array,
   * normal-index array, color-index array, and texture coordinate index arrays.
   */
  void clear_flat_indices();

  /*! Invalidate (clear) the flat cordinate-index array.
   */
  void clear_flat_coord_indices();

  /*! Invalidate (clear) the flat normal-index array.
   */
  void clear_flat_normal_indices();

  /*! Invalidate (clear) the flat color-index array.
   */
  void clear_flat_color_indices();

  /*! Invalidate (clear) the flat texture cordinate-index array.
   */
  void clear_flat_tex_coord_indices();

  /*! Determine whether the coordinate-index array has been invalidated
   * (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_flat_coord_indices() const;

  /*! Determine whether the normal-index array has been invalidated (is dirty)
   * thus must be cleaned.
   */
  Boolean is_dirty_flat_normal_indices() const;

  /*! Determine whether the color-index array has been invalidated (is dirty)
   * and thus must be cleaned.
   */
  Boolean is_dirty_flat_color_indices() const;

  /*! Determine whether the texture coordinate-index array has been
   * invalidated (is dirty) and thus must be cleaned.
   */
  Boolean is_dirty_flat_tex_coord_indices() const;
  //@}

  /*! Set the flag that indicates whether the coordinate-index array is "flat".
   * \param flag indicates whether the coordinate-index array is "flat".
   */
  void set_coord_indices_flat(Boolean flag);

  /*! Determine whether the coordinate-index array is "flat".
   */
  Boolean are_coord_indices_flat() const;

  /*! Assign the coord indices with the reverse of given indices.
   * \param indices the indices to reverse.
   */
  void set_reverse_coord_indices(const std::vector<Int32>& indices);

  /*! Assign the flat coord indices with the reverse of given indices.
   * \param indices the indices to reverse.
   */
  void set_reverse_flat_coord_indices(const Index_array& indices);

  /*! Determine whether the normal-index array is "flat".
   */
  Boolean are_normal_indices_flat() const;

  /*! Set the flag that indicates whether the color-index array is "flat".
   * \param flag indicates whether the color-index array is "flat".
   */
  void set_color_indices_flat(Boolean flag);

  /*! Determine whether the color-index array is "flat".
   */
  Boolean are_color_indices_flat() const;

  /*! Set the flag that indicates whether the normal-index array is "flat".
   * \param flag indicates whether the normal-index array is "flat".
   */
  void set_normal_indices_flat(Boolean flag);

  /*! Set the flag that indicates whether the textuture coordinate-index array
   * is "flat".
   * \param flag indicates whether the texture coordinate-index array is "flat".
   */
  void set_tex_coord_indices_flat(Boolean flag);

  /*! Determine whether the texture coordinate-index array is "flat".
   */
  Boolean are_tex_coord_indices_flat() const;

  /*! Convert non-flat indices (VRML style) to flat indices.
   * \param src the non-flat indices.
   * \param dst the flat indices.
   * \pre the primitive type is either triangle or quad.
   */
  void flatten_indices(const std::vector<Int32>& src, Index_array& dst);

  /*! Convert flat indices to flat non-indices (VRML style).
   * \param dst the non-flat indices.
   * \param src the flat indices.
   * \pre the primitive type is either triangle or quad.
   */
  void  deflatten_indices(const Index_array& src, std::vector<Int32>& dst);

  /*! Obtain the ith flat coord index.
   * \return the ith flat coord index.
   */
  Index_type get_flat_coord_index(Uint i) const;

  /*! Obtain the ith flat normal index.
   * \return the ith flat normal index.
   */
  Index_type get_flat_normal_index(Uint i) const;

  /*! Obtain the ith flat color index.
   * \return the ith flat color index.
   */
  Index_type get_flat_color_index(Uint i) const;

  /*! Obtain the ith flat tex-coord index.
   * \return the ith flat tex-coord index.
   */
  Index_type get_flat_tex_coord_index(Uint i) const;

  /*! Set the coordinate indices from a range of elements that counts
   * the number of vertices per primitive.
   * \param begin The begin iterator of the range of numbers.
   * \param end The past0the-end iterator of the range of numbers.
   */
  template <typename InputIterator>
  void set_coord_indices_from_range(InputIterator begin, InputIterator end);

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates();

  /*! Obtain a pointer to an element through the coord indices. */
  template <typename Vector_T>
  const GLfloat* get_by_flat_coord_index(Vector_T array, Index_type i) const;

  /*! Obtain apointer to an element through the normal indices. */
  template <typename Vector_T>
  const GLfloat* get_by_flat_normal_index(Vector_T array, Index_type i) const;

  /*! Obtain a pointer to an element through the color indices. */
  template <typename Vector_T>
  const GLfloat* get_by_flat_color_index(Vector_T array, Index_type i) const;

  /*! Obtain a pointer to an element through the tex. coord indices. */
  template <typename Vector_T>
  const GLfloat* get_by_flat_tex_coord_index(Vector_T array, Index_type i) const;

protected:
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

  /*! An array of indices into the vertex-coordinate array. */
  Index_array m_flat_coord_indices;

  /*! An array of indices into the vertex-texture-coordinate array. */
  Index_array m_flat_normal_indices;

  /*! An array of indices into the vertex-color array. */
  Index_array m_flat_color_indices;

  /*! An array of indices into the vertex-normal array. */
  Index_array m_flat_tex_coord_indices;

  /*! Indicates whether the flat coordinate-index array should be cleaned. */
  Boolean m_dirty_flat_coord_indices;

  /*! Indicates whether the flat normal-index array should be cleaned. */
  Boolean m_dirty_flat_normal_indices;

  /*! Indicates whether the flat color-index array should be cleaned. */
  Boolean m_dirty_flat_color_indices;

  /*! Indicates whether the flat texture coordinate-index array should be
   * cleaned.
   */
  Boolean m_dirty_flat_tex_coord_indices;

  /*! Indicates whether the coordinate index array is "flat". In a "flat"
   * representation, in case of triangles and quads, the no -1 end-of-face
   * markers are not present. The number of indices is (m_num_primitives * 3)
   * or (m_num_primitives * 4) respectively. It is illegal to have a mixture
   * of flat and non-flat index arrays. Either all indices are flat or none is.
   */
  Boolean m_coord_indices_flat;

  /*! Indicates whether the normal index array is "flat".
   */
  Boolean m_normal_indices_flat;

  /*! Indicates whether the color index array is "flat".
   */
  Boolean m_color_indices_flat;

  /*! Indicates whether the texture coordinate index array is "flat".
   */
  Boolean m_tex_coord_indices_flat;

  /*! Indicates whether the coordinate-index array should be cleaned. */
  Boolean m_dirty_coord_indices;

  /*! Indicates whether the normal-index array should be cleaned. */
  Boolean m_dirty_normal_indices;

  /*! Indicates whether the color-index array should be cleaned. */
  Boolean m_dirty_color_indices;

  /*! Indicates whether the texture coordinate-index array should be cleaned. */
  Boolean m_dirty_tex_coord_indices;

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates(std::vector<Int32>& indices);

  /*! Colapse identical coordinates.
   */
  void collapse_identical_coordinates(Index_array& indices);

  /*! Export a triangular facet.
   * \param[in] p1 the point of the first vertex.
   * \param[in] p2 the point of the second vertex.
   * \param[in] p3 the point of the third vertex.
   * \param[in] stl_formater the formatter.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2,
                   const Vector3f& p3, Formatter* formatter);

  /*! Export a quadrilateral facet.
   * \param[in] p1 the point of the first vertex.
   * \param[in] p2 the point of the second vertex.
   * \param[in] p3 the point of the third vertex.
   * \param[in] p4 the point of the forth vertex.
   * \param[in] stl_formater the formatter.
   */
  void write_facet(const Vector3f& p1, const Vector3f& p2,
                   const Vector3f& p3, const Vector3f& p4,
                   Formatter* formatter);

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

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief sets the counter-clockwise flag. */
inline void Mesh_set::set_ccw(Boolean ccw) { m_is_ccw = ccw; }

//! \brief returns true if the surface polygons are counter-clockwise.
inline Boolean Mesh_set::is_ccw() const { return m_is_ccw; }

//! \brief sets the solid flag.
inline void Mesh_set::set_solid(Boolean solid) { m_is_solid = solid; }

//! \brief returns true if the polyhedron is solid, and false otherwise.
inline Boolean Mesh_set::is_solid() const { return m_is_solid; }

//! \brief sets the convex flag.
inline void Mesh_set::set_convex(Boolean convex) { m_is_convex = convex; }

//! \brief returns true if all faces of the polyhedron are convex.
inline Boolean Mesh_set::is_convex() const { return m_is_convex; }

//! \brief sets the crease angle of the polyhedron.
inline void Mesh_set::set_crease_angle(Float crease_angle)
{ m_crease_angle = crease_angle; }

//! \brief obtains the creas_angle.
inline Float Mesh_set::get_crease_angle() const { return m_crease_angle; }

//! \brief determines whether the flat coordinate-index array is dirty.
inline Boolean Mesh_set::is_dirty_flat_coord_indices() const
{ return m_dirty_flat_coord_indices; }

//! \brief determines whether the flat normal-index array is dirty.
inline Boolean Mesh_set::is_dirty_flat_normal_indices() const
{ return m_dirty_flat_normal_indices; }

//! \brief determines whether the flat color-index array is dirty.
inline Boolean Mesh_set::is_dirty_flat_color_indices() const
{ return m_dirty_flat_color_indices; }

//! \brief determines whether the flat texture coordinate-index array is dirty.
inline Boolean Mesh_set::is_dirty_flat_tex_coord_indices() const
{ return m_dirty_flat_tex_coord_indices; }

//! \brief determines whether the coordinate index array is "flat".
inline Boolean Mesh_set::are_coord_indices_flat() const
{ return m_coord_indices_flat; }

//! \brief determines whether the normal index array is "flat".
inline Boolean Mesh_set::are_normal_indices_flat() const
{ return m_normal_indices_flat; }

//! \brief determines whether the color index array is "flat".
inline Boolean Mesh_set::are_color_indices_flat() const
{ return m_color_indices_flat; }

//! \brief determines whether the texture coordinate index array is "flat".
inline Boolean Mesh_set::are_tex_coord_indices_flat() const
{ return m_tex_coord_indices_flat; }

/*! \brief sets the flag that indicates whether the coordinate index array is
 * "flat".
 */
inline void Mesh_set::set_coord_indices_flat(Boolean flag)
{ m_coord_indices_flat = flag; }

/*! \brief sets the flag that indicates whether the normal index array is
 * "flat".
 */
inline void Mesh_set::set_normal_indices_flat(Boolean flag)
{ m_normal_indices_flat = flag; }

//! \brief sets the flag that indicates whether the color index array is "flat".
inline void Mesh_set::set_color_indices_flat(Boolean flag)
{ m_color_indices_flat = flag; }

/*! \brief sets the flag that indicates whether the texture coordinate index
 * array is "flat".
 */
inline void Mesh_set::set_tex_coord_indices_flat(Boolean flag)
{ m_tex_coord_indices_flat = flag; }

//! \brief obtains a pointer to an element through the coord indices.
template <typename Vector_T>
inline const GLfloat*
Mesh_set::get_by_flat_coord_index(Vector_T array, Index_type i) const
{ return array->datum(m_flat_coord_indices[i]); }

/*! \brief obtains a pointer to an element through the normal indices. */
template <typename Vector_T>
inline const GLfloat*
Mesh_set::get_by_flat_normal_index(Vector_T array, Index_type i)
  const
{ return array->datum(m_flat_normal_indices[i]); }

//! \brief obtains a pointer to an element through the color indices.
template <typename Vector_T>
inline const GLfloat*
Mesh_set::get_by_flat_color_index(Vector_T array, Index_type i) const
{ return array->datum(m_flat_color_indices[i]); }

//! \brief obtains a pointer to an element through the tex. coord indices.
template <typename Vector_T>
inline const GLfloat*
Mesh_set::get_by_flat_tex_coord_index(Vector_T array, Index_type i) const
{ return array->datum(m_flat_tex_coord_indices[i]); }

//! \brief obtains the ith flat coord index.
inline Mesh_set::Index_type Mesh_set::get_flat_coord_index(Uint i) const
{ return m_flat_coord_indices[i]; }

//! \brief obtains the ith flat normal index.
inline Mesh_set::Index_type Mesh_set::get_flat_normal_index(Uint i) const
{ return m_flat_normal_indices[i]; }

//! \brief obtains the ith flat color index.
inline Mesh_set::Index_type Mesh_set::get_flat_color_index(Uint i) const
{ return m_flat_color_indices[i]; }

//! \brief obtains the ith flat tex-coord index.
inline Mesh_set::Index_type Mesh_set::get_flat_tex_coord_index(Uint i) const
{ return m_flat_tex_coord_indices[i]; }

/*! \brief set the coordinate indices from a range of elements that counts
 * the number of vertices per primitive.
 */
template <typename InputIterator>
void Mesh_set::set_coord_indices_from_range(InputIterator begin,
                                            InputIterator end)
{
  Boolean tris(true);
  Boolean quads(true);
  Uint size = 0;
  Uint num = 0;
  for (auto it = begin; it != end; ++it) {
    ++num;
    size += *it;
    if (*it == 3) quads = false;
    else if (*it == 4) tris = false;
    else {
      quads = false;
      tris = false;
    }
  }
  SGAL_assertion(tris && quads);

  if (tris || quads) {
    if (tris)
      set_primitive_type(Geo_set::PT_TRIANGLES);
    else // quads
      set_primitive_type(Geo_set::PT_QUADS);
    m_flat_coord_indices.resize(size);
    for (Uint i = 0; i < size; ++i) m_flat_coord_indices[i] = i;
    set_coord_indices_flat(true);
    m_dirty_coord_indices = true;
  }
  else {
    set_primitive_type(Geo_set::PT_POLYGONS);
    size += num;
    m_coord_indices.resize(size);
    Uint i = 0;
    Uint index = 0;
    for (auto it = begin; it != end; ++it) {
      for (Uint j = 0; j < *it; ++j) m_coord_indices[i++] = index++;
      m_coord_indices[i++] = -1;
    }
    set_coord_indices_flat(false);
  }
  set_num_primitives(num);
  m_dirty_flat_coord_indices = false;
}

//! \brief determines whether the coordinate-index array is dirty.
inline Boolean Mesh_set::is_dirty_coord_indices() const
{ return m_dirty_coord_indices; }

//! \brief determines whether the normal-index array is dirty.
inline Boolean Mesh_set::is_dirty_normal_indices() const
{ return m_dirty_normal_indices; }

//! \brief determines whether the color-index array is dirty.
inline Boolean Mesh_set::is_dirty_color_indices() const
{ return m_dirty_color_indices; }

//! \brief determines whether the texture coordinate-index array is dirty.
inline Boolean Mesh_set::is_dirty_tex_coord_indices() const
{ return m_dirty_tex_coord_indices; }

SGAL_END_NAMESPACE

#endif
