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

#ifndef SGAL_MESH_SET_HPP
#define SGAL_MESH_SET_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Field_info;

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

  /// \name field handlers
  //@{
  Boolean* is_ccw_handle(const Field_info*) { return &m_is_ccw; }
  Boolean* is_solid_handle(const Field_info*) { return &m_is_solid; }
  Boolean* is_convex_handle(const Field_info*) { return &m_is_convex; }
  Float* crease_angle_handle(const Field_info*) { return &m_crease_angle; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Draw the mesh conditionaly. */
  virtual void draw(Draw_action* action);

  /*! Draw the mesh (unconditionaly). */
  virtual void draw_mesh(Draw_action* action);

  /*! Clean the mesh. */
  virtual void clean();

  /*! Calculate the sphere bound. */
  virtual Boolean clean_sphere_bound();

  /*! Draw the representation. */
  virtual void draw_geometry(Draw_action* action) = 0;

  /*! Clear the representation. */
  virtual void clear();

  /*! Determine whether the is dirty and thus needs cleaning. */
  virtual Boolean is_dirty() const;

  /*! Determine whether the representation is empty. */
  virtual Boolean is_empty() const = 0;

  /*! Clean the coord-index array. */
  virtual void clean_coord_indices();

  /*! Clean the normal-index array. */
  virtual void clean_normal_indices();

  /*! Clean the color-index array. */
  virtual void clean_color_indices();

  /*! Clean the texture coord-index array. */
  virtual void clean_tex_coord_indices();

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

  /*! Obtain the coordinate-index array. */
  virtual Array<Uint>& get_coord_indices();

  /*! Obtain the normal-index array. */
  virtual Array<Uint>& get_normal_indices();

  /*! Obtain the color-index array. */
  virtual Array<Uint>& get_color_indices();

  /*! Obtain the texture coordinate-index array. */
  virtual Array<Uint>& get_tex_coord_indices();

  /*! Set the flag that indicates whether to flatten the indices.
   * \param flag indicates whether to flatten the indices.
   */
  void set_flatten_indices(Boolean flag);

  /*! Set the flag that indicates whether the coordinate-index array is "flat".
   * \param flag indicates whether the coordinate-index array is "flat".
   */
  void set_coord_indices_flat(Boolean flag);

  /*! Determine whether the coordinate-index array is "flat". */
  Boolean are_coord_indices_flat() const;

  /*! Set the coordinate-index array. */
  void set_coord_indices(Array<Uint>& indices);

  /*! Set the flat coordinate-index array. */
  void set_flat_coord_indices(Array<Uint>& indices);

  /*! Reverse the coordinate indices. */
  void set_reverse_coord_indices(const Array<Uint>& indices);

  /*! Reverse the flat coordinate indices. */
  void set_reverse_flat_coord_indices(const Array<Uint>& indices);

  /*! Set the flag that indicates whether the normal-index array is "flat".
   * \param flag indicates whether the normal-index array is "flat".
   */
  void set_normal_indices_flat(Boolean flag);

  /*! Determine whether the normal-index array is "flat". */
  Boolean are_normal_indices_flat() const;

  /*! Set the normal-index array. */
  void set_normal_indices(Array<Uint>& indices);

  /*! Set the flat normal-index array. */
  void set_flat_normal_indices(Array<Uint>& indices);

  /*! Set the flag that indicates whether the color-index array is "flat".
   * \param flag indicates whether the color-index array is "flat".
   */
  void set_color_indices_flat(Boolean flag);

  /*! Determine whether the color-index array is "flat". */
  Boolean are_color_indices_flat() const;

  /*! Set the color-index array. */
  void set_color_indices(Array<Uint>& indices);

  /*! Set the flat color-index array. */
  void set_flat_color_indices(Array<Uint>& indices);

  /*! Set the flag that indicates whether the textuture coordinate-index array
   * is "flat".
   * \param flag indicates whether the texture coordinate-index array is "flat".
   */
  void set_tex_coord_indices_flat(Boolean flag);

  /*! Determine whether the texture coordinate-index array is "flat". */
  Boolean are_tex_coord_indices_flat() const;

  /*! Set the texture coordinate-index array. */
  void set_tex_coord_indices(Array<Uint>& indices);

  /*! Set the flat texture coordinate-index array. */
  void set_flat_tex_coord_indices(Array<Uint>& indices);

  /*! Set the coordinate indices from a range of elements that counts
   * the number of vertices per primitive.
   * \param begin The begin iterator of the range of numbers.
   * \param end The past0the-end iterator of the range of numbers.
   */
  template <typename InputIterator>
  void set_coord_indices(InputIterator begin, InputIterator end);

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

  /*! Indicates whether the coordinate-index array be cleaned. */
  Boolean m_dirty_coord_indices;

  /*! Indicates whether the normal-index array be cleaned. */
  Boolean m_dirty_normal_indices;

  /*! Indicates whether the color-index array be cleaned. */
  Boolean m_dirty_color_indices;

  /*! Indicates whether the texture coordinate-index array be cleaned. */
  Boolean m_dirty_tex_coord_indices;

  /*! Indicates whether to remove the -1 end-of-face markers in case of
   * triangles and quads.
   */
  Boolean m_flatten_indices;

  /*! Indicates whether the coordinate index array is "flat". In a "flat"
   * representation, in case of triangles and quads, the no -1 end-of-face
   * markers are not present. The number of indices is (m_num_primitives * 3)
   * or (m_num_primitives * 4) respectively. It is illegal to have a mixture
   * of flat and non-flat index arrays. Either all indices are flat or none is.
   */
  Boolean m_coord_indices_flat;

  /*! Indicates whether the normal index array is "flat". */
  Boolean m_normal_indices_flat;

  /*! Indicates whether the color index array is "flat". */
  Boolean m_color_indices_flat;

  /*! Indicates whether the texture coordinate index array is "flat". */
  Boolean m_tex_coord_indices_flat;

  /*! Clear the indices arrays, e.g., forces their cleaning. */
  void clear_indices();

  /*! Clear the cordinate indices, e.g., forces their cleaning. */
  void clear_coord_indices();

  /*! Clear the normal indices, e.g., forces their cleaning. */
  void clear_normal_indices();

  /*! Clear the color indices, e.g., forces their cleaning. */
  void clear_color_indices();

  /*! Clear the texture cordinate indices, e.g., forces their cleaning. */
  void clear_tex_coord_indices();

  /*! Determine whether the coordinate index array are dirty and thus should
   * be cleaned.
   */
  Boolean is_dirty_coord_indices() const;

  /*! Determine whether the normal index array are dirty and thus should
   * be cleaned.
   */
  Boolean is_dirty_normal_indices() const;

  /*! Determine whether the color index array are dirty and thus should
   * be cleaned.
   */
  Boolean is_dirty_color_indices() const;

  /*! Determine whether the texture coordinate index array are dirty and thus
   * should be cleaned.
   */
  Boolean is_dirty_tex_coord_indices() const;

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

//! \brief clears the representation.
inline void Mesh_set::clear() { m_dirty = true; }

//! \brief cleans the mesh.
inline void Mesh_set::clean() { m_dirty = false; }

//! \brief clears the cordinate indices, e.g., forces their cleaning.
inline void Mesh_set::clear_coord_indices() { m_dirty_coord_indices = true; }

//! \brief clears the normal indices, e.g., forces their cleaning.
inline void Mesh_set::clear_normal_indices() { m_dirty_normal_indices = true; }

//! \brief clears the color indices, e.g., forces their cleaning.
inline void Mesh_set::clear_color_indices() { m_dirty_color_indices = true; }

//! \brief clears the texture cordinate indices, e.g., forces their cleaning.
inline void Mesh_set::clear_tex_coord_indices()
{ m_dirty_tex_coord_indices = true; }

//! \brief determines whether the coordinate-index array hasn't been cleaned.
inline Boolean Mesh_set::is_dirty_coord_indices() const
{ return m_dirty_coord_indices; }

//! \brief determines whether the normal-index array hasn't been cleaned.
inline Boolean Mesh_set::is_dirty_normal_indices() const
{ return m_dirty_normal_indices; }

//! \brief determines whether the color-index array hasn't been cleaned.
inline Boolean Mesh_set::is_dirty_color_indices() const
{ return m_dirty_color_indices; }

//! \brief determines whether the texture coordinate-index hasn't been cleaned.
inline Boolean Mesh_set::is_dirty_tex_coord_indices() const
{ return m_dirty_tex_coord_indices; }

//! \brief determines whether the representation hasn't been cleaned.
inline Boolean Mesh_set::is_dirty() const { return m_dirty; }

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

//! \brief sets the flag that indicates whether to flatten the indices.
inline void Mesh_set::set_flatten_indices(Boolean flag)
{ m_flatten_indices = flag; }

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

/*! \brief sets the flag that indicates whether the color index array is
 * "flat".
 */
inline void Mesh_set::set_color_indices_flat(Boolean flag)
{ m_color_indices_flat = flag; }

/*! \brief sets the flag that indicates whether the texture coordinate index
 * array is "flat".
 */
inline void Mesh_set::set_tex_coord_indices_flat(Boolean flag)
{ m_tex_coord_indices_flat = flag; }

/*! \brief set the coordinate indices from a range of elements that counts
 * the number of vertices per primitive.
 */
template <typename InputIterator>
void Mesh_set::set_coord_indices(InputIterator begin, InputIterator end)
{
  Boolean tris(true);
  Boolean quads(true);
  std::vector<Uint>::iterator it;
  Uint size = 0;
  Uint num = 0;
  for (it = begin; it != end; ++it) {
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
    m_coord_indices.resize(size);
    for (Uint i = 0; i < size; ++i) m_coord_indices[i] = i;
    set_coord_indices_flat(true);
  }
  else {
    set_primitive_type(Geo_set::PT_POLYGONS);
    size += num;
    m_coord_indices.resize(size);
    Uint i = 0;
    Uint index = 0;
    for (it = begin; it != end; ++it) {
      for (Uint j = 0; j < *it; ++j) m_coord_indices[i++] = index++;
      m_coord_indices[i++] = -1;
    }
  }
  set_flatten_indices(false);
  set_num_primitives(num);
}

SGAL_END_NAMESPACE

#endif
