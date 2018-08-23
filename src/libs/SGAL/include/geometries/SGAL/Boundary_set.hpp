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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BOUNDARY_SET_HPP
#define SGAL_BOUNDARY_SET_HPP

#include <vector>
#include <boost/unordered_map.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array_3d.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Texture.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;
class Formatter;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Boundary_set Boundary_set.hpp
 * Boundary_set is a geometry node that represnts a mesh made of facets
 * with some additional attributes and efficient OpenGL rendering capabilities.
 *
 * There are several data structures that compose the structure. They can be
 * divided into sets as follows:
 *
 * 1. Vertex arrays (defined in the base class Geo_set).
 *   a. m_coord_array - a field that comprises a vector of 3D points.
 *   b. m_normal_array - a field that comprises a vector of 3D normals.
 *   c. m_color_array - a field that comprises a vector of (3D) colors.
 *   d. m_tex_coord_array - a field that comprises a vector of 2D, 3D, or 4D
 *                      texture coordinates.
 *
 * 2. Vertex index arrays (defined in the base class Geo_set).
 *   a. m_coord_indices - a field that comprises a vector of indices into
 *                      m_coord_array.
 *   b. m_normal_indices - a field that comprises a vector of indices into
 *                      m_normal_array.
 *   c. m_color_indices - a field that comprises a vector of indices into
 *                      m_color_array.
 *   d. m_tex_coord_indices - a field that comprises a vector of indices into
 *                      m_tex_coord_array.
 *
 * 3. Vertex flat index arrays (defined in the base class Mesh_set, which
 *   derives from Geo_set).
 *   These are not fields but rather utility arrays. These arrays can be used
 *   only when all facets have the exact same number of vertices. Currently,
 *   only triangles and quadruples are supported.
 *   a. m_flat_coord_indices - a vector of indices into m_coord_array.
 *   b. m_flat_normal_indices - a vector of indices into m_normal_array.
 *   c. m_flat_color_indices - a vector of indices into m_color_array.
 *   d. m_flat_tex_coord_indices - a vector of indices into m_tex_coord_array.
 *
 * 4. Local vertex buffers (defined here). These arrays are mirros of
 *    the buffers used as part of the OpenGL VERTEX BUFFER OBBJECT
 *    mechanism or VERTEX ARRAY mechanism. They are used when the arrays
 *    from set (1) plus m_flat_coord_indices from set (3) cnnot be used due
 *    to non-identical index arrays (e.g., the flat coordinate indices array
 *    is different than the flat normal indices array and the latter is
 *    not empty.
 *   a. m_local_coord_buffer - a vector of 3D points.
 *   b. m_local_normal_buffer - a vector of 3D normals.
 *   c. m_local_color_buffer - a vector of (3D) colors.
 *   d. m_local_tex_coord_buffer_2d - a vector of 2D texture coordinates.
 *   e. m_local_tex_coord_buffer_3d - a vector of 2D texture coordinates.
 *   f. m_local_tex_coord_buffer_4d - a vector of 2D texture coordinates.
 *   g. m_local_indices - a vector of indices into
 *     (i) m_local_coord_buffer,
 *    (ii) either m_local_normal_buffer or m_local_color_buffer, and optionally
 *   (iii) one of the local texture buffers.
 *
 * 5. OpenGL vertex buffer objects.
 *   a. m_coord_buffer_id
 *   b. m_normal_buffer_id
 *   c. m_color_buffer_id
 *   d. m_tex_coord_buffer_id
 *
 * A class that derives from this class, typically generates the coordinates
 * and the cordinate indices. A change of a field that should trigger the
 * regeneration of the coordinates or the coordinate indices must be
 * accompanied by the clearing of the coordinates or the coordinate
 * indices. The clearing will, in turn, triger their regeneration.
 *
 * Observe that there are three different ways a field, say <field>, may change:
 * 1. directly through the call to the respective setter (set_<field>()),
 * 2. indirectly as a result of a change to another field routed to <field>, or
 * 3. remotely, as a result of a change to another field in a container being
 * observed by the container that contains <field>.  In particular, if the
 * change to a field should trigger the regeneration of the coordinates or the
 * coordinate indices, the following must be applied according to the three
 * cases above.
 * 1. set_<field>()
 *   If the change to <field> should trigger the regeneration of the
 *   coordinates, the function set_<field>() must call
 *   Indexed_face_set::clear_coord_array(), which clears the coordinate
 *   array (sets the m_dirty_coord_array flag on). If the change should also
 *   trigger the regeneration of the coordinate indices, the function
 *   set_<field>() must also call Indexed_face_set::clear_coord_indices(),
 *   which clears the coordinate index array (sets the
 *   m_dirty_coord_indices flag on) or
 *   Indexed_face_set::clear_flat_coord_indices(),
 *   which clears the flat coordinate index array (sets the
 *   m_dirty_coord_indices flag on) or both.
 * 2. Init_prototype()
 *   Denote the callback invoked after a change to a field is made
 *   <field_changed(). This is set in the Init_prototype() function of the
 *   container class that contains the field. This function must follow the
 *   the instruction above.
 * 3. The entry point for this case is the function field_changed() of the
 *   container class that contains the field. Let A be the container class
 *   that contains the field <field>, and let B be a container observed by A.
 *   Assume that B changes. The function B::field_changed() is invoked().
 *   As part of the iterations of all observers of B the function
 *   A::field_changed() is invoked.
 */
class SGAL_SGAL_DECL Boundary_set : public Mesh_set {
public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    COLOR_PER_VERTEX,
    NORMAL_PER_VERTEX,
    LAST
  };

  // Drawing & representation:

#define SGAL_BO_FRAG_SOURCE_        0
#define SGAL_BO_FRAG_SOURCE         0x00000001
#define SGAL_BO_FRAG_INDEXED_       1
#define SGAL_BO_FRAG_INDEXED        0x00000002
#define SGAL_BO_FRAG_ATTACHMENT_    2
#define SGAL_BO_FRAG_ATTACHMENT     0x0000000c
#define SGAL_BO_TEXTURE_ENABLED_    4
#define SGAL_BO_TEXTURE_ENABLED     0x00000010
#define SGAL_BO_TEXTURE_INDEXED_    5
#define SGAL_BO_TEXTURE_INDEXED     0x00000020
#define SGAL_BO_PRIM_TYPE_          6
#define SGAL_BO_PRIM_TYPE           0x000001c0
#define SGAL_BO_VERTEX_ARRAY_       9
#define SGAL_BO_VERTEX_ARRAY        0x00000200
#define SGAL_NUM_BO_DRAWS           0x400

  /*! Constructor */
  Boundary_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Boundary_set();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype.
   */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Boolean* normal_per_vertex_handle(const Field_info*)
  { return &m_normal_per_vertex; }
  Boolean* color_per_vertex_handle(const Field_info*)
  { return &m_color_per_vertex; }
  Vector3f* center_handle(const Field_info*){ return &m_center; }
  //@}

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Draw the polygons.
   */
  virtual void draw(Draw_action* action);

  /*! Draw the polygons for selection.
   */
  virtual void isect(Isect_action* action);

  /// \name Indices Change Reactors
  //@{
  /*! Respond to a change in the coordinate array.
   * \param field_info (in) the information record of the field that caused
   *                   the change.
   */
  virtual void coord_content_changed(const Field_info* field_info);

  /*! Respond to a change in the normal array.
   * \param field_info (in) the information record of the field that caused
   *                   the change.
   */
  virtual void normal_content_changed(const Field_info* field_info);

  /*! Respond to a change in the color array.
   * \param field_info (in) the information record of the field that caused
   *                   the change.
   */
  virtual void color_content_changed(const Field_info* field_info);

  /*! Respond to a change in the texture coordinate array.
   * \param field_info (in) the information record of the field that caused
   *                   the change.
   */
  virtual void tex_coord_content_changed(const Field_info* field_info);
  //@}

  /// \name Array Getters
  //@{
  /*! Obtain the normal array.
   * \return the normal array.
   */
  virtual Shared_normal_array get_normal_array();

  /*! Obtain the texture-coordinate array.
   * \return the texture-coordinate array.
   */
  virtual Shared_tex_coord_array get_tex_coord_array();
  //@}

  /// \name Array Cleaners
  //@{
  /*! Clean the normal array and the normal indices.
   * A normal is calculated either per vertex or per polygon depending on the
   * attachment attributes.
   */
  virtual void clean_normals();

  /*! Clean the default texture-mapping coordinate array and coordinate indices
   * using the shape bounding-box.
   * \param target (in) the texture target, which, for example, implies
   *               the number of texture coordinates (2, 3, or 4).
   */
  virtual void clean_tex_coords(Texture::Target target);

  /*! Clean the default 2D texture-mapping coordinate array and coordinate
   * indices.
   */
  virtual void clean_tex_coords_2d();

  /*! Calculate the default 3D texture-mapping coordinate array and coordinate
   * indices.
   */
  virtual void clean_tex_coords_3d();
  //@}

  /// \name Facet Indices Change Recators
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

  /// \name Facet Indices Change Recators
  //@{
  /*! Respond to a change in the facet coordinate-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_coord_indices_changed();

  /*! Respond to a change in the facet normal-index array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void facet_normal_indices_changed();

  /*! Respond to a change in the facet color-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_color_indices_changed();

  /*! Respond to a change in the facet texture-coordinate index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_tex_coord_indices_changed();
  //@}

  /// \name Array Predicates
  //@{
  /*! Determine whether the have been invalidated, and thus must be cleaned.
   */
  Boolean is_dirty_normal_array() const;

  /*! Determine whether the texture-mapping coordinate array has been
   * invalidated, and thus needs cleaning.
   */
  Boolean is_dirty_tex_coord_array() const;
  //@}

  /*! Clean the center of the geometric brick. */
  virtual void clean_center();

  /*! Set the center of the geometric object.
   */
  void set_center(const Vector3f& center);

  /*! Obtain the center of the geometric object.
   */
  Vector3f& get_center();

  /*! Determine whether the center has been invalidated, and thus needs
   * cleaning.
   */
  Boolean is_dirty_center() const;

  /*! Add the container to a given scene.
   * \param[in] scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Calculate a single normal per vertex for all vertices.
   * For each vertex compute the weighted normal based on the normals of
   * the vertex incident facets and the receiprocal of the square distance
   * from the facet center to the vertex. (An alternative could be the facet
   * area.)
   */
  void calculate_single_normal_per_vertex();

  /*! Calculate a single normal per polygon for all polygons. */
  void calculate_normal_per_facet();

  /*! Calculate a single normal per polygon for all polygons. */
  void calculate_normal_per_facet(Normal_array& normals);

  /* Set the flag that indicates whether normals are bound per vertex or per
   * face.
   * \param normal_per_vertex true if normals are bound per vertex
   */
  void set_normal_per_vertex(Boolean normal_per_vertex);

  /*! Obtain the normal per vertex mode.
   */
  Boolean get_normal_per_vertex() const;

  /* Set the flag that indicates whether colors are bound per vertex or per
   * face.
   * @param color_per_vertex true if normals are bound per vertex
   */
  void set_color_per_vertex(Boolean color_per_vertex);

  /*! Obtain the color per vertex mode.
   */
  Boolean get_color_per_vertex() const;

  virtual int create_display_list(Draw_action* action);

  /*! Draw the representation.
   * \param action action.
   */
  virtual void draw_geometry(Draw_action* action);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
   virtual void write_field(const Field_info* field_info, Formatter* formatter);

  // Drawing:
#include "SGAL/Indexed_face_set_mask.hpp"

  // Draw routines:
  void draw_invalid();

  // Polygons (general):
  // Texture disabled:
  void draw_FSNO_FINO_FAPV_TENO_MOPO_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOPO_VANO();

  void draw_FSNO_FINO_FAPT_TENO_MOPO_VANO();
  void draw_FSCO_FINO_FAPT_TENO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPT_TENO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPT_TENO_MOPO_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOPO_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOPO_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOPO_VANO(); invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VANO(); invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO(); invalid

  // Triangles:
  // Texture disabled:
  void draw_FSNO_FINO_FAPV_TENO_MOTR_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOTR_VANO();

  void draw_FSNO_FINO_FAPT_TENO_MOTR_VANO();
  void draw_FSCO_FINO_FAPT_TENO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPT_TENO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPT_TENO_MOTR_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOTR_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOTR_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOTR_VANO(); invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VANO(); invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO(); invalid

  // Quads:
  // Texture disabled:
  void draw_FSNO_FINO_FAPV_TENO_MOQU_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOQU_VANO();

  void draw_FSNO_FINO_FAPT_TENO_MOQU_VANO();
  void draw_FSCO_FINO_FAPT_TENO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPT_TENO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPT_TENO_MOQU_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOQU_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOQU_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOQU_VANO(); invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VANO(); invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO(); invalid

  // Triangle strips:
  // Texturedisabled:
  void draw_FSNO_FINO_FAPV_TENO_MOTS_VANO();
  void draw_FSCO_FINO_FAPV_TENO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPV_TENO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPV_TENO_MOTS_VANO();

  void draw_FSNO_FINO_FAPT_TENO_MOTS_VANO();
  void draw_FSCO_FINO_FAPT_TENO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPT_TENO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPT_TENO_MOTS_VANO();

  // void draw_FSNO_FINO_FAPM_TENO_MOTS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TENO_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOTS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOTS_VANO(); invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VANO(); invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO();

  // void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO(); invalid
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO(); invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO(); invalid

  // Vertex Buffer:
  // Triangles:
  // Texture disabled:
  void draw_FAPV_VAYE();
  void draw_FAPT_VAYE();
  void draw_FAPM_VAYE();

  static void (Boundary_set::*m_draws[SGAL_NUM_BO_DRAWS])();
  static Boolean m_draws_initialized;

protected:
  // weight, facet-index
  typedef std::pair<Uint, Float>                Vertex_facet_info;

  typedef std::list<Vertex_facet_info>          Vertex_info;
  typedef Vertex_info::const_iterator           Vertex_info_const_iter;
  typedef std::vector<Vertex_info>              Vertices_info;
  typedef Vertices_info::const_iterator         Vertices_info_const_iter;

  /*! Indicates whether normals are bound per vertex */
  Boolean m_normal_per_vertex;

  /*! Indicates whether colors are bound per vertex */
  Boolean m_color_per_vertex;

  /*! An array that holds the lengths of all tristrips. The first
   * number in the array is the number of tri strips.
   */
  Uint* m_tri_strip_lengths;

  /*! The id of the display list. */
  Uint m_display_list_id;

  /*! The id of the coord buffer. */
  Uint m_coord_buffer_id;

  /*! The id of the color buffer. */
  Uint m_color_buffer_id;

  /*! The id of the normal buffer. */
  Uint m_normal_buffer_id;

  /*! The id of the tex. coord buffer. */
  Uint m_tex_coord_buffer_id;

  /*! The geometry drawing-mode {direct, display list, or vertex array */
  Configuration::Geometry_drawing_mode m_drawing_mode;

  /*! Indicates whether vertex buffer object is supported */
  Boolean m_use_vertex_buffer_object;

  /*! data structures to hold the low level geometry. */
  Uint m_tri_strip_lengths_size;

  /*! Indicates that the normal array has been invalidated. */
  Boolean m_dirty_normal_array;

  /*! Indicates that the normals array has been cleaned. */
  Boolean m_normal_array_cleaned;

  /*! Indicates that the texture coordinate array has been invalidated.
   */
  Boolean m_dirty_tex_coord_array;

  /*! Indicates that the texture coordinatea array has been cleaned.
   */
  Boolean m_tex_coord_array_cleaned;

  /*! Indicates that the vetex coordinate buffer is dirty. */
  Boolean m_dirty_coord_buffer;

  /*! Indicates that the vetex normal buffer is dirty. */
  Boolean m_dirty_normal_buffer;

  /*! Indicates that the vetex color buffer is dirty. */
  Boolean m_dirty_color_buffer;

  /*! Indicates that the vetex texture coordinate buffer is dirty. */
  Boolean m_dirty_tex_coord_buffer;

  /*! Indicates that the local vetex buffers are dirty */
  Boolean m_dirty_local_vertex_buffers;

  /*! Indicates whether the center is dirty and thus needs cleaning. */
  Boolean m_dirty_center;

  /*! The center of the geometric object. The generated texture coordinate of
   * a vertex v is the displacement of v relative to m_center (v - m_center).
   */
  Vector3f m_center;

  /*! Clean the data structure of the vertex coordinate buffer object.
   */
  void clean_vertex_coord_buffer(Uint size, const GLfloat* data);

  /*! Clean the data structure of the vertex normal buffer object.
   */
  void clean_vertex_normal_buffer(Uint size, const GLfloat* data);

  /*! Clean the data structure of the vertex color buffer object.
   */
  void clean_vertex_color_buffer(Uint size, const GLfloat* data);

  /*! Clean the data structure of the vertex texture coordinate buffer object.
   */
  void clean_vertex_tex_coord_buffer(Uint size, const GLfloat* data);

  /*! Clean the local coordinates, normals, color, and texture coordinates
   * vertex buffers.
   */
  void clean_local_cnct_vertex_buffers();

  /*! Clean the local coordinates, normals, and color, vertex buffers. */
  void clean_local_cnc_vertex_buffers();

  /*! Clean the local coordinates, normals, and texture coordinates vertex
   * buffers.
   */
  void clean_local_cnt_vertex_buffers();

  /*! Clean the local coordinates and normals vertex buffers. */
  void clean_local_cn_vertex_buffers();

  /*! Clean the local coordinates, colors, and texture coordinates vertex
   * buffers.
   */
  void clean_local_cct_vertex_buffers();

  /*! Clean the local coordinates and colors vertex buffers. */
  void clean_local_cc_vertex_buffers();

  /*! Clean the local coordinates and texture coordinates vertex buffers. */
  void clean_local_ct_vertex_buffers();

  /*! Destroy the data structure of the vertex buffer object. */
  void destroy_vertex_buffers();

  /*! Destroy the data structure of the display_list. */
  void destroy_display_list();

  /*! Clear the local vertex buffers. */
  void clear_local_vertex_buffers();

  /*! Isect direct drawing-mode */
  void isect_direct();

  /*! Dispatch the appropriate drawing routine. */
  void draw_dispatch(Draw_action* action);

  /*! Determine whether it is possible using openGl vertex array.
   */
  Boolean use_vertex_array() const;

  /*! Determine whether it is possible using openGl vertex buffer object.
   */
  Boolean use_vertex_buffer_object() const;

  /*! Calculate a single normal per triangle for all triangles. */
  void calculate_normal_per_triangle(Normal_array& normals);

  /*! Calculate a single normal per quad for all quads. */
  void calculate_normal_per_quad(Normal_array& normals);

  /*! Calculate a single normal per polygon for all polygons. */
  void calculate_normal_per_polygon(Normal_array& normals);

  /*! Calculate vertex information per triangle for all triangles. */
  void calculate_vertex_info_per_triangle(Vertices_info& vertices_info);

  /*! Calculate vertex information per quad for all quads. */
  void calculate_vertex_info_per_quad(Vertices_info& vertices_info);

  /*! Calculate vertex information per polygon for all polygons. */
  void calculate_vertex_info_per_polygon(Vertices_info& vertices_info);

  // /*! Compute the normalized normal to a triangle.
  //  * \param j (in) The starting index of the triangular facet in the coord
  //             indices array.
  //  * \param normal The resulting normal.
  //  */
  // void compute_triangle_normal(Uint j, Vector3f& normal) const;

  // /*! Compute the center point of a triangle.
  //  * \param j The starting index of the triangular facet in the coord indices
  //  *     array.
  //  * \param center The resulting center.
  //  */
  // void compute_triangle_center(Uint j, Vector3f& center) const;

  // /*! Compute the vertex information for the three vertices of a triangule
  //  * \param j The starting index of the triangular facet in the coord
  //  *     indices array.
  //  * \param center The triangle center.
  //  * \param normal The triangle normal.
  //  * \param vertices_info The vertex information array.
  //  */
  // void compute_triangle_vertex_info(Uint j, Uint facet_index,
  //                                   const Vector3f& center,
  //                                   Vertices_info& vertices_info) const;

  // /*! Compute the normalized normal to a quadrilateral.
  //  * \param j (in) The starting index of the quadrilateral facet in the coord
  //  *          indices array.
  //  * \param normal The resulting normal
  //  */
  // void compute_quad_normal(Uint j, Vector3f& normal) const;

  // /*! Compute the center point of a quadrilateral.
  //  * \param j The starting index of the quadrilateral facet in the coord
  //  *     indices array.
  //  * \param center The resulting center
  //  */
  // void compute_quad_center(Uint j, Vector3f& center) const;

  // /*! Compute the vertex information for the four vertices of a quadrilateral
  //  * \param j The starting index of the quadrilateral facet in the coord
  //  *     indices array.
  //  * \param center The triangle center.
  //  * \param normal The triangle normal.
  //  * \param vertices_info The vertex information array.
  //  */
  // void compute_quad_vertex_info(Uint j, Uint facet_index,
  //                               const Vector3f& center,
  //                               Vertices_info& vertices_info) const;

  // /*! Compute the normalized normal to a polygon.
  //  * \param j (in) The starting index of the polygonal facet in the coord
  //  *          indices array.
  //  * \param normal The resulting normal
  //  */
  // void compute_polygon_normal(Uint j, Vector3f& normal) const;

  // /*! Compute the center point of a polygon.
  //  * \param j The starting index of the polygonal facet in the coord indices
  //  *    array.
  //  * \param center the resulting center
  //  * \return the number of vertices of the polygon
  //  */
  // Uint compute_polygon_center(Uint j, Vector3f& center) const;

  // /*! Compute the vertex information for the all vertices of a polygon
  //  * \param j The starting index of the polygonal facet in the coord
  //  *     indices array.
  //  * \param k The number of vertices of the polygon
  //  * \param center The polygon center.
  //  * \param normal The polygon normal.
  //  * \param vertices_info The vertex information array.
  //  */
  // void compute_polygon_vertex_info(Uint j, Uint facet_index, Uint k,
  //                                  const Vector3f& center,
  //                                  Vertices_info& vertices_info) const;

  /*! Compute the vertex information for a facet vertex
   * \param vertex_index The vertex index
   * \param venter The facet center
   * \param normal The facet normal
   * \param info The output iterator associated with the vertex info container
   */
  template <typename OutputIterator>
  OutputIterator compute_vertex_info(Uint vertex_index,
                                     Uint facet_index,
                                     const Vector3f& center,
                                     OutputIterator info) const
  {
    // Compute the receiprocal of the square distance from the facet center
    const Vector3f& v = get_coord_3d(vertex_index);
    Float weight = 1.0f / v.sqr_distance(center);
    *info++ = Vertex_facet_info(facet_index, weight);
    return info;
  }

  /*! Determine whether the angle between two given vectors is smooth.
   */
  Boolean is_smooth(const Vector3f& normal1, const Vector3f& normal2) const;

  /*! Calculate a single normal per vertex for all vertices.
   * For each vertex compute the weighted normal based on the normals of
   * the vertex incident facets and the receiprocal of the square distance
   * from the facet center to the vertex. (An alternative could be the facet
   * area.)
   * \param array (out) the array of the resulting normals.
   */
  void calculate_single_normal_per_vertex(Shared_normal_array normal_array);

  /*! Obtain local coordinate data.
   * \return local coordinate data.
   */
  const GLfloat* local_coord_data() const;

  /*! Obtain local normal data.
   * \return local normal data.
   */
  const GLfloat* local_normal_data() const;

  /*! Obtain local color data.
   * \return local color data.
   */
  const GLfloat* local_color_3d_data() const;

  /*! Obtain local color data.
   * \return local color data.
   */
  const GLfloat* local_color_4d_data() const;

  /*! Obtain local 2d teture coordinate data.
   * \return local 2d teture coordinate data.
   */
  const GLfloat* local_tex_coord_2d_data() const;

  /*! Obtain local 3d teture coordinate data.
   * \return local 3d teture coordinate data.
   */
  const GLfloat* local_tex_coord_3d_data() const;

  /*! Obtain local 4d teture coordinate data.
   * \return local 3d teture coordinate data.
   */
  const GLfloat* local_tex_coord_4d_data() const;

  /*! Obtain the local indices data.
   * \return the local indices data.
   */
  const GLvoid* local_indices_data() const;

  /*! Visitor */
  class Data_visitor : public boost::static_visitor<> {
  public:
    GLvoid* data;

    /*! Constructor. */
    Data_visitor() : data(NULL) {}

    void operator()(const Triangle_indices& indices)
    { data = (GLvoid*) &indices[0][0]; }

    void operator()(const Quad_indices& indices)
    { data = (GLvoid*) &indices[0][0]; }

    void operator()(const Polygon_indices& indices)
    { data = (GLvoid*) &indices[0][0]; }

    void operator()(const Flat_indices& indices)
    { data = (GLvoid*) &indices[0]; }
  };

  /*! Obtain the facet coordinate indices data.
   * \return the facet coordinate indices data.
   */
  const GLvoid* facet_coord_indicaes_data() const;

  /*! Obtain coordinate data.
   * \return coordinate data.
   */
  const GLfloat* coord_data() const;

  /*! Obtain normal data.
   * \return normal data.
   */
  const GLfloat* normal_data() const;

  /*! Obtain color data.
   * \return color data.
   */
  const GLfloat* color_data() const;

  /*! Obtain teture coordinate data.
   * \return teture coordinate data.
   */
  const GLfloat* tex_coord_data() const;

  /*! Obtain the indices data.
   * \return the indices data.
   */
  const GLvoid* indices_data() const;

  /*! Obtain color data size.
   * \return color data size.
   */
  size_t color_data_size() const;

  /*! Obtain teture coordinate data size.
   * \return teture coordinate data size.
   */
  size_t tex_coord_data_size() const;

  /*! Obtain the number of tex ture coordinates.
   * \return the number of tex ture coordinates.
   */
  Uint num_tex_coordinates() const;

  /*! Compute flat indices for the normals or for the colors.
   * This function is invoked when the attachment is per-primitive.
   */
  void compute_flat_indices_per_primitive(const Facet_indices& source,
                                          Flat_indices& target);

  /*! Compute flat indices for the colors.
   * This function is invoked when the attachment is per-mesh.
   */
  void compute_flat_indices_per_mesh(const Facet_indices& source,
                                     Flat_indices& target);

  void compute_flat_tex_coords_2d(size_t num_verts);
  void compute_polygon_tex_coords_2d();

private:
  /*! The key for the utility map, which maps a tuple of 3 ids, namely,
   * coordinate id, normal/color id, and texture coordinate id, to a single
   * id.
   */
  typedef std::tuple<Uint, Uint>                Id_key_2d;
  typedef std::tuple<Uint, Uint, Uint>          Id_key_3d;
  typedef std::tuple<Uint, Uint, Uint, Uint>    Id_key_4d;

  /*! The hash specialized function, which maps maps 2 ids to a unique value.
   */
  struct Id_hash_2d {
    std::size_t operator()(Id_key_2d const& key) const
    {
      std::size_t seed = 0;
      boost::hash_combine(seed, std::get<0>(key));
      boost::hash_combine(seed, std::get<1>(key));
      return seed;
    }
  };
  /*! The hash specialized function, which maps maps 3 ids to a unique value.
   */
  struct Id_hash_3d {
    std::size_t operator()(Id_key_3d const& key) const
    {
      std::size_t seed = 0;
      boost::hash_combine(seed, std::get<0>(key));
      boost::hash_combine(seed, std::get<1>(key));
      boost::hash_combine(seed, std::get<2>(key));
      return seed;
    }
  };

  /*! The hash specialized function, which maps maps 4 ids to a unique value.
   */
  struct Id_hash_4d {
    std::size_t operator()(Id_key_4d const& key) const
    {
      std::size_t seed = 0;
      boost::hash_combine(seed, std::get<0>(key));
      boost::hash_combine(seed, std::get<1>(key));
      boost::hash_combine(seed, std::get<2>(key));
      boost::hash_combine(seed, std::get<2>(key));
      return seed;
    }
  };

  //! The type Id_map maps from tuples of ids to ids.
  typedef boost::unordered_map<Id_key_2d, Uint, Id_hash_2d> Id_map_2d;
  typedef boost::unordered_map<Id_key_3d, Uint, Id_hash_3d> Id_map_3d;
  typedef boost::unordered_map<Id_key_4d, Uint, Id_hash_4d> Id_map_4d;

  //! The coordinates vertex array.
  std::vector<Vector3f> m_local_coord_buffer;

  //! The normals vertex array.
  std::vector<Vector3f> m_local_normal_buffer;

  //! The colors vertex array.
  std::vector<Vector3f> m_local_color_buffer_3d;
  std::vector<Vector4f> m_local_color_buffer_4d;

  //! The texture coordinates vertex array.
  std::vector<Vector2f> m_local_tex_coord_buffer_2d;
  std::vector<Vector3f> m_local_tex_coord_buffer_3d;
  std::vector<Vector4f> m_local_tex_coord_buffer_4d;

  //! The index vertex array.
  Index_array m_local_indices;

  //! The node prototype.
  static Container_proto* s_prototype;

  //! Default values.
  static const Boolean s_def_normal_per_vertex;
  static const Boolean s_def_color_per_vertex;

  /*! Calculate vertex information. Used to compute a single normal per vertex.
   * \param vertices_info (out) the container of the resulting information.
   */
  void calculate_vertices_info(Vertices_info& vertices_info);

  /*! Clean the local vertex buffers.
   */
  template <typename Array, typename Buffer, typename InputIterator>
  void clean_local_2d_vertex_buffers(Array array2, Buffer& buffer2,
                                     InputIterator it2)
  {
    m_local_coord_buffer.clear();
    buffer2.clear();

    m_local_indices.resize(size_facet_indices(m_facet_coord_indices));

    Id_map_2d id_map;
    Uint id1 = 0;
    Uint id2 = 0;

    auto it1 = begin_facet_indices(m_facet_coord_indices);
    size_t index = 0;
    for (; it1 != end_facet_indices(m_facet_coord_indices); ++it1, ++it2) {
      id1 = *it1;
      id2 = *it2;
      auto key = std::make_tuple(id1, id2);
      Id_map_2d::const_iterator got = id_map.find(key);
      if (got != id_map.end()) {
        m_local_indices[index++] = got->second;
        continue;
      }
      size_t new_id = m_local_coord_buffer.size();
      m_local_coord_buffer.push_back(get_coord_3d(id1));
      buffer2.push_back((*array2)[id2]);
      id_map[key] = new_id;
      m_local_indices[index++] = new_id;
    }
    id_map.clear();

    m_dirty_local_vertex_buffers = false;
    m_dirty_coord_buffer = true;
  }

  /*! Clean the local vertex buffers.
   */
  template <typename Array2, typename Buffer2, typename InputIterator2,
            typename Array3, typename Buffer3, typename InputIterator3>
  void clean_local_3d_vertex_buffers(Array2 array2, Buffer2& buffer2,
                                     InputIterator2 it2,
                                     Array3 array3, Buffer3& buffer3,
                                     InputIterator3 it3)
  {
    m_local_coord_buffer.clear();
    buffer2.clear();
    buffer3.clear();

    m_local_indices.resize(size_facet_indices(m_facet_coord_indices));

    Id_map_3d id_map;
    Uint id1 = 0;
    Uint id2 = 0;
    Uint id3 = 0;

    auto it1 = begin_facet_indices(m_facet_coord_indices);
    size_t index = 0;
    for (; it1 != end_facet_indices(m_facet_coord_indices); ++it1, ++it2, ++it3)
    {
      id1 = *it1;
      id2 = *it2;
      id3 = *it3;
      auto key = std::make_tuple(id1, id2, id3);
      Id_map_3d::const_iterator got = id_map.find(key);
      if (got != id_map.end()) {
        m_local_indices[index++] = got->second;
        continue;
      }
      size_t new_id = m_local_coord_buffer.size();
      m_local_coord_buffer.push_back(get_coord_3d(id1));
      buffer2.push_back((*array2)[id2]);
      buffer3.push_back((*array3)[id3]);
      id_map[key] = new_id;
      m_local_indices[index++] = new_id;
    }
    id_map.clear();

    m_dirty_local_vertex_buffers = false;
    m_dirty_coord_buffer = true;
  }

  /*! Clean the local vertex buffers.
   */
  template <typename Array2, typename Buffer2, typename InputIterator2,
            typename Array3, typename Buffer3, typename InputIterator3,
            typename Array4, typename Buffer4, typename InputIterator4>
  void clean_local_4d_vertex_buffers(Array2 array2, Buffer2& buffer2,
                                     InputIterator2 it2,
                                     Array3 array3, Buffer3& buffer3,
                                     InputIterator3 it3,
                                     Array4 array4, Buffer4& buffer4,
                                     InputIterator4 it4)
  {
    m_local_coord_buffer.clear();
    buffer2.clear();
    buffer3.clear();
    buffer4.clear();

    m_local_indices.resize(size_facet_indices(m_facet_coord_indices));

    Id_map_4d id_map;
    Uint id1 = 0;
    Uint id2 = 0;
    Uint id3 = 0;
    Uint id4 = 0;

    auto it1 = begin_facet_indices(m_facet_coord_indices);
    size_t index = 0;
    for (; it1 != end_facet_indices(m_facet_coord_indices); ++it1, ++it2, ++it3, ++it4)
    {
      id1 = *it1;
      id2 = *it2;
      id3 = *it3;
      id4 = *it4;
      auto key = std::make_tuple(id1, id2, id3, id4);
      Id_map_4d::const_iterator got = id_map.find(key);
      if (got != id_map.end()) {
        m_local_indices[index++] = got->second;
        continue;
      }
      size_t new_id = m_local_coord_buffer.size();
      m_local_coord_buffer.push_back(get_coord_3d(id1));
      buffer2.push_back((*array2)[id2]);
      buffer3.push_back((*array3)[id3]);
      buffer4.push_back((*array4)[id4]);
      id_map[key] = new_id;
      m_local_indices[index++] = new_id;
    }
    id_map.clear();

    m_dirty_local_vertex_buffers = false;
    m_dirty_coord_buffer = true;
  }
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! \brief determines whether the conditions allow for the use of openGl vertex
 * array.
 * Configuration specifies VERTEX_ARRAY, and
 * Primitive types supported so far: QUADS and TRIANGLES, and
 * Notice that when the attachment of the color/normal is not PER_VERTEX (e.g.,
 * (PER_POLYGON), the vertex data must be properly computed to use vertex array.
 * This is done by the functions that clean the local vertex buffers.
 */
inline Boolean Boundary_set::use_vertex_array() const
{
  Fragment_source fragment_source = resolve_fragment_source();
  return ((m_drawing_mode == Configuration::GDM_VERTEX_ARRAY) &&
          ((m_primitive_type == PT_QUADS) ||
           (m_primitive_type == PT_TRIANGLES)));
}

/*! \brief determines whether the normals have been invalidated,
 * and thus been must be cleaned.
 */
inline Boolean Boundary_set::is_dirty_normal_array() const
{ return m_dirty_normal_array; }

/*! \brief determines whether the texture coordinates have been invalidated,
 * and thus been must be cleaned.
 */
inline Boolean Boundary_set::is_dirty_tex_coord_array() const
{ return m_dirty_tex_coord_array; }

//! \brief obtains the normal per vertex mode.
inline Boolean Boundary_set::get_normal_per_vertex() const
{ return m_normal_per_vertex; }

//! \brief obtains the color per vertex mode.
inline Boolean Boundary_set::get_color_per_vertex() const
{ return m_color_per_vertex; }

//! \brief obtains local coordinate data.
inline const GLfloat* Boundary_set::local_coord_data() const
{
  SGAL_assertion(! m_local_coord_buffer.empty());
  return (GLfloat*)(&(*(m_local_coord_buffer.begin())));
}

//! \brief obtains local normal data.
inline const GLfloat* Boundary_set::local_normal_data() const
{
  SGAL_assertion(! m_local_normal_buffer.empty());
  return (GLfloat*)(&(*(m_local_normal_buffer.begin())));
}

//! \brief obtains local color data.
inline const GLfloat* Boundary_set::local_color_3d_data() const
{
  SGAL_assertion(! m_local_color_buffer_3d.empty());
  return (GLfloat*)(&(*(m_local_color_buffer_3d.begin())));
}

//! \brief obtains local color data.
inline const GLfloat* Boundary_set::local_color_4d_data() const
{
  SGAL_assertion(! m_local_color_buffer_4d.empty());
  return (GLfloat*)(&(*(m_local_color_buffer_4d.begin())));
}

//! \brief obtains local 2d teture coordinate data.
inline const GLfloat* Boundary_set::local_tex_coord_2d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_2d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_2d.begin())));
}

//! \brief obtains local 3d teture coordinate data.
inline const GLfloat* Boundary_set::local_tex_coord_3d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_3d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_3d.begin())));
}

//! \brief obtains local 4d teture coordinate data.
inline const GLfloat* Boundary_set::local_tex_coord_4d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_4d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_4d.begin())));
}

//! \brief obtains the local indices data.
inline const GLvoid* Boundary_set::local_indices_data() const
{
  SGAL_assertion(! m_local_indices.empty());
  return (GLvoid*)(&(*(m_local_indices.begin())));
}

//! Obtain the flat coordinate indices data.
inline const GLvoid* Boundary_set::facet_coord_indicaes_data() const
{
  Data_visitor visitor;
  boost::apply_visitor(visitor, m_facet_coord_indices);
  return visitor.data;
}

//! \brief obtains coordinate data..
inline const GLfloat* Boundary_set::coord_data() const
{
  return m_local_coord_buffer.empty() ? m_coord_array->data() :
    local_coord_data();
}

//! \brief obtains normal data.
inline const GLfloat* Boundary_set::normal_data() const
{
  return m_local_normal_buffer.empty() ? m_normal_array->data() :
    local_normal_data();
}

//! \brief obtains color data.
inline const GLfloat* Boundary_set::color_data() const
{
  return
    (! m_local_color_buffer_3d.empty()) ? local_color_3d_data() :
    ((! m_local_color_buffer_4d.empty()) ? local_color_4d_data() :
     m_color_array->data());
}

//! \brief obtains teture coordinate data.
inline const GLfloat* Boundary_set::tex_coord_data() const
{
  return
    (! m_local_tex_coord_buffer_2d.empty()) ? local_tex_coord_2d_data() :
    ((! m_local_tex_coord_buffer_3d.empty()) ? local_tex_coord_3d_data() :
     ((! m_local_tex_coord_buffer_4d.empty()) ? local_tex_coord_4d_data() :
      m_tex_coord_array->data()));
}

//! \brief obtains color size.
inline size_t Boundary_set::color_data_size() const
{
  return
    (! m_local_color_buffer_3d.empty()) ?
    (m_local_color_buffer_3d.size() * sizeof(Vector3f)) :
    ((! m_local_color_buffer_4d.empty()) ?
     (m_local_color_buffer_4d.size() * sizeof(Vector4f)) : 0);
}

//! \brief obtains teture coordinate size.
inline size_t Boundary_set::tex_coord_data_size() const
{
  return
    (! m_local_tex_coord_buffer_2d.empty()) ?
    (m_local_tex_coord_buffer_2d.size() * sizeof(Vector2f)) :
    ((! m_local_tex_coord_buffer_3d.empty()) ?
     (m_local_tex_coord_buffer_3d.size() * sizeof(Vector3f)) :
     ((! m_local_tex_coord_buffer_4d.empty()) ?
      (m_local_tex_coord_buffer_4d.size() * sizeof(Vector4f)) : 0));
}

//! Obtain the indices.
inline const GLvoid* Boundary_set::indices_data() const
{
  return m_local_indices.empty() ? facet_coord_indicaes_data() :
    local_indices_data();
}

//! \brief obtains the number of tex ture coordinates.
inline Uint Boundary_set::num_tex_coordinates() const
{
  return
    (! m_local_tex_coord_buffer_2d.empty()) ? 2 :
    ((! m_local_tex_coord_buffer_3d.empty()) ? 3 :
     ((! m_local_tex_coord_buffer_4d.empty()) ? 4 :
      ((m_tex_coord_array != nullptr) ?
       m_tex_coord_array->num_coordinates() : 0)));
}

//! \brief determines whether the center has been invalidated.
inline Boolean Boundary_set::is_dirty_center() const { return m_dirty_center; }

SGAL_END_NAMESPACE

#endif
