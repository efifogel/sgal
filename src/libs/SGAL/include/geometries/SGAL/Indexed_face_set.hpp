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

#ifndef SGAL_INDEXED_FACE_SET_HPP
#define SGAL_INDEXED_FACE_SET_HPP

#include <vector>
#include <boost/unordered_map.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/IO/Polyhedron_iostream.h>

#include "SGAL/basic.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector2f.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Vector4f.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Indexed_face_set Indexed_face_set.hpp
 * Indexed_face_set is a geometry node that represnts a mesh made of facets
 * with some additional attributes and efficient OpenGL rendering capabilities.
 *
 * There are several data structures that compose the input. They can be
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
 * 3. Vertex flat index arrays (defined in the base class Mesh_set).
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
 *    is different than the flat coordinate indices array and the latter is
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
 * and the cordinate indices. In that case the following must be done:
 * 1. init_prototype()
 *   Recall that the execution function is the call back invoked after a
 *   change to a field.
 *   - For every field, the change of which should trigger the regeneration of
 *     the coordinates, the execution function must call
 *     Indexed_face_set::clear(), which in turns calls Mesh_set::clear(),
 *     which in turns set the m_dirty flag on.
 *   - For every field, the change of which should trigger the regeneration of
 *     the coordinate indices, the execution function must call
 *     Indexed_face_set::clear_indices(), which in turns calls
 *     Mesh_set::clear(), which in turns set the m_dirty_indices flag on.
 * 2. set_<field>()
 *   If the change of <field> should trigger the regeneration of the
 *   coordinates or coordinate indices, the function must also call
 *   Indexed_face_set::clear() or Indexed_face_set::clear_indices(),
 *   respectively.
 * 3. Container fields and field_changed()
 *   - Every field that is a container and the change of which should
 *     trigger the regeneration of the coordinates or coordinate indices must
 *     register the derived class as an observer.
 *   - The field_changed() msut call either Indexed_face_set::clear() or
 *     Indexed_face_set::clear_indices() as a response to a change in the
 *     respective field.
 */
class SGAL_SGAL_DECL Indexed_face_set : public Mesh_set {
public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    COLOR_PER_VERTEX,
    NORMAL_PER_VERTEX,
    LAST
  };

  typedef CGAL::Cartesian<Float>                         Kernel;
  typedef Kernel::Point_3                                Point;
  typedef Kernel::Vector_3                               Vector;
  typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
  typedef CGAL::Polyhedron_3<Traits>                     Polyhedron;
  typedef Polyhedron::Facet_iterator                     Facet_iterator;
  typedef Polyhedron::Vertex_iterator                    Vertex_iterator;
  typedef Polyhedron::Halfedge_around_facet_circulator   Halfedge_facet_circ;
  typedef Polyhedron::HalfedgeDS                         HalfedgeDS;
  typedef Polyhedron::Halfedge_around_facet_circulator
    Halfedge_facet_circulator;

  /*! Constructor */
  Indexed_face_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Indexed_face_set();

  /* Construct the prototype. */
  static Indexed_face_set* prototype();

  /*! Clone. */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Boolean* normal_per_vertex_handle(const Field_info*)
  { return &m_normal_per_vertex; }
  Boolean* color_per_vertex_handle(const Field_info*)
  { return &m_color_per_vertex; }
  //@}

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  // Drawing & representation:

#define FRAGMENT_SOURCE_        0
#define FRAGMENT_SOURCE         0x00000001
#define FRAGMENT_INDEXED_       1
#define FRAGMENT_INDEXED        0x00000002
#define FRAGMENT_ATTACHMENT_    2
#define FRAGMENT_ATTACHMENT     0x0000000c
#define TEXTURE_ENABLED_        4
#define TEXTURE_ENABLED         0x00000010
#define TEXTURE_INDEXED_        5
#define TEXTURE_INDEXED         0x00000020
#define PRIMITIVE_TYPE_         6
#define PRIMITIVE_TYPE          0x000001c0
#define VERTEX_ARRAY_           9
#define VERTEX_ARRAY            0x00000200
#define NUM_DRAWS               0x400

  /*! Calculate a single normal per vertex for all vertices.
   * For each vertex compute the weighted normal based on the normals of
   * the vertex incident facets and the receiprocal of the square distance
   * from the facet center to the vertex. (An alternative could be the facet
   * area.)
   */
  void calculate_single_normal_per_vertex();

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

  /*! Calculate a single normal per polygon for all polygons. */
  void calculate_normal_per_polygon();

  /*! Calculate a single normal per polygon for all polygons. */
  void calculate_normal_per_polygon(Normal_array& normals);

  /*! Clean the polyhedron. */
  virtual void clean_polyhedron();

  /*! Clean the facets. */
  virtual void clean_facets();

  /*! Calculate the normals in case they are invalidated.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

  /*! Determine whether the representation of the normals hasn't been
   * cleaned.
   */
  Boolean is_dirty_normals() const;

  /*! Calculate the texture coordinates in case they are invalidated. */
  virtual void clean_tex_coords();

  /*! Determine whether the representation of the normals hasn't been
   * cleaned.
   */
  Boolean is_dirty_tex_coords() const;

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

  /*! Draw the polygons for selection. */
  virtual void isect(Isect_action* action);

  virtual int create_display_list(Draw_action* action);

  /*! Return true if the representation is empty. */
  virtual Boolean is_empty() const;

  /*! Draw the polygons. */
  virtual void draw(Draw_action* action);

  /*! Draw the representation.
   * \param action action.
   */
  virtual void draw_geometry(Draw_action* action);

  /*! Determine whether colors are generated by the geometry. */
  virtual Boolean are_generated_color();

  /*! Determine whether texture coordinates are generated by the geometry. */
  virtual Boolean are_generated_tex_coord();

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

  void draw_FSNO_FINO_FAPM_TENO_MOPO_VANO();
  void draw_FSCO_FINO_FAPM_TENO_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOPO_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOPO_VANO();         invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOPO_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOPO_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TINO_MOPO_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOPO_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOPO_VANO();         invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOPO_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOPO_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOPO_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOPO_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOPO_VANO();         invalid

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

  void draw_FSNO_FINO_FAPM_TENO_MOTR_VANO();
  void draw_FSCO_FINO_FAPM_TENO_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOTR_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOTR_VANO();         invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTR_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTR_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TINO_MOTR_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTR_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTR_VANO();         invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTR_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTR_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTR_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTR_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTR_VANO();         invalid

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

  void draw_FSNO_FINO_FAPM_TENO_MOQU_VANO();
  void draw_FSCO_FINO_FAPM_TENO_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOQU_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOQU_VANO();         invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOQU_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOQU_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TINO_MOQU_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOQU_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOQU_VANO();         invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOQU_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOQU_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOQU_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOQU_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOQU_VANO();         invalid

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

  void draw_FSNO_FINO_FAPM_TENO_MOTS_VANO();
  void draw_FSCO_FINO_FAPM_TENO_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TENO_MOTS_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TENO_MOTS_VANO();         invalid

  // Texture enabled:
  void draw_FSNO_FINO_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TINO_MOTS_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TINO_MOTS_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TINO_MOTS_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TINO_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TINO_MOTS_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TINO_MOTS_VANO();         invalid

  void draw_FSNO_FINO_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FINO_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSNO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FIYE_FAPV_TEYE_TIYE_MOTS_VANO();

  void draw_FSNO_FINO_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FINO_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSNO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FIYE_FAPT_TEYE_TIYE_MOTS_VANO();

  void draw_FSNO_FINO_FAPM_TEYE_TIYE_MOTS_VANO();
  void draw_FSCO_FINO_FAPM_TEYE_TIYE_MOTS_VANO();
  // void draw_FSNO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO();         invalid
  // void draw_FSCO_FIYE_FAPM_TEYE_TIYE_MOTS_VANO();         invalid

  // Vertex Buffer:
  // Triangles:
  // Texture disabled:
  void draw_FAPV_VAYE();
  void draw_FAPT_VAYE();
  void draw_FAPM_VAYE();

  static void (Indexed_face_set::*draws[NUM_DRAWS])();
  static Boolean m_draws_initialized;

  /*! Set the coordinate array.
   * \param coord_array (in) a pointer to a coordinate array
   */
  virtual void set_coord_array(Shared_coord_array coord_array);

  /*! Set the normal array.
   * \param normal_array (in) the normal array.
   */
  virtual void set_normal_array(Shared_normal_array normal_array);

  /*! Set the color field.
   * \param color_array (in) a pointer to a color array
   */
  virtual void set_color_array(Shared_color_array color_array);

  /*! Set the texture-coordinate array.
   * \param tex_coord_array (in) the texture coordinate array.
   */
  virtual void set_tex_coord_array(Shared_tex_coord_array tex_coord_array);

  /*! Process change of field. */
  virtual void field_changed(const Field_info* field_info);

  /*! Process change of coordinate points. */
  virtual void coord_point_changed();

protected:
  // weight, facet-index
  typedef std::pair<Uint, Float>                Vertex_facet_info;

  typedef std::list<Vertex_facet_info>          Vertex_info;
  typedef Vertex_info::const_iterator           Vertex_info_const_iter;
  typedef std::vector<Vertex_info>              Vertices_info;
  typedef Vertices_info::const_iterator         Vertices_info_const_iter;

  /*! The maximal rank of vertices in the graph */
  Uint m_max_rank;

  /*! The number of polygons in the last decoded level */
  Int m_curr_num_polygons;

  /*! number of (top) levels for which smooth normals are computed */
  Int m_num_of_levels_to_smooth;

  /*! Indicates whether normals are bound per vertex */
  Boolean m_normal_per_vertex;

  /*! Indicates whether colors are bound per vertex */
  Boolean m_color_per_vertex;

  /*! An array that holds the lengths of all tristrips. The first
   * number in the array is the number of tri strips.
   */
  Uint* m_tri_strip_lengths;

  /*! true if there are multiple uv coordinates per vertex. */
  Boolean m_is_multiple_uv;

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

  /*! Indicates whether the indices have been converted to fixed size */
  Boolean m_indices_dirty;

  // data structures to hold the low level geometry
  Int m_tri_strip_lengths_size;

  /* if the bbox is read from the xml file, this is true. */
  Boolean m_bb_is_pre_set;

  Boolean m_is_progressive;

  /*! The actual polyhedron object. */
  Polyhedron m_polyhedron;

  /*! Indicates whether the polyhedron is dirty and thus should be cleaned. */
  Boolean m_dirty_polyhedron;

  /*! Indicates whether the polyhedron facets are dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_facets;

  /*! Indicates that the normals have been invalidated. */
  Boolean m_dirty_normals;

  /*! Indicates that the normals have been cleaned. */
  Boolean m_normals_cleaned;

  /*! Indicates that the texture coordinatea have been invalidated. */
  Boolean m_dirty_tex_coords;

  /*! Indicates that the texture coordinatea  have been cleaned. */
  Boolean m_tex_coords_cleaned;

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

  /*! Clean the local vertex buffers. */
  void clean_local_vertex_buffers();

  /*! Destroy the data structure of the vertex buffer object. */
  void destroy_vertex_buffers();

  /*! Destroy the data structure of the display_list. */
  void destroy_display_list();

  /*! Clear the vertex arrays. */
  void clear_vertex_arrays();

  /*! Clear the local vertex buffers. */
  void clear_local_vertex_buffers();

  /*! Isect direct drawing-mode */
  void isect_direct();

  /*! Dispatch the appropriate drawing routine. */
  void draw_dispatch(Draw_action* action);

  /*! Determine whether the conditions allow for the use of openGl vertex array.
   */
  Boolean use_vertex_array() const;

  /*! Compute the normalized normal to a triangle.
   * \param j The starting index of the triangular facet in the coord indices
   *     array.
   * \param normal The resulting normal.
   */
  void compute_triangle_normal(Uint j, Vector3f& normal) const;

  /*! Compute the center point of a triangle.
   * \param j The starting index of the triangular facet in the coord indices
   *     array.
   * \param center The resulting center.
   */
  void compute_triangle_center(Uint j, Vector3f& center) const;

  /*! Compute the vertex information for the three vertices of a triangule
   * \param j The starting index of the triangular facet in the coord
   *     indices array.
   * \param center The triangle center.
   * \param normal The triangle normal.
   * \param vertices_info The vertex information array.
   */
  void compute_triangle_vertex_info(Uint j, Uint facet_index,
                                    const Vector3f& center,
                                    Vertices_info& vertices_info) const;

  /*! Compute the normalized normal to a quadrilateral.
   * \param j The starting index of the quadrilateral facet in the coord
   *     indices array.
   * \param normal The resulting normal
   */
  void compute_quad_normal(Uint j, Vector3f& normal) const;

  /*! Compute the center point of a quadrilateral.
   * \param j The starting index of the quadrilateral facet in the coord
   *     indices array.
   * \param center The resulting center
   */
  void compute_quad_center(Uint j, Vector3f& center) const;

  /*! Compute the vertex information for the four vertices of a quadrilateral
   * \param j The starting index of the quadrilateral facet in the coord
   *     indices array.
   * \param center The triangle center.
   * \param normal The triangle normal.
   * \param vertices_info The vertex information array.
   */
  void compute_quad_vertex_info(Uint j, Uint facet_index,
                                const Vector3f& center,
                                Vertices_info& vertices_info) const;

  /*! Compute the normalized normal to a polygon.
   * \param j The starting index of the polygonal facet in the coord indices
   *     array.
   * \param normal The resulting normal
   */
  void compute_polygon_normal(Uint j, Vector3f& normal) const;

  /*! Compute the center point of a polygon.
   * \param j The starting index of the polygonal facet in the coord indices
   *    array.
   * \param center the resulting center
   * \return the number of vertices of the polygon
   */
  Uint compute_polygon_center(Uint j, Vector3f& center) const;

  /*! Compute the vertex information for the all vertices of a polygon
   * \param j The starting index of the polygonal facet in the coord
   *     indices array.
   * \param k The number of vertices of the polygon
   * \param center The polygon center.
   * \param normal The polygon normal.
   * \param vertices_info The vertex information array.
   */
  void compute_polygon_vertex_info(Uint j, Uint facet_index, Uint k,
                                   const Vector3f& center,
                                   Vertices_info& vertices_info) const;

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
    boost::shared_ptr<Coord_array_3d> coord_array =
      boost::static_pointer_cast<Coord_array_3d>(m_coord_array);
    SGAL_assertion(coord_array);

    // Compute the receiprocal of the square distance from the facet center
    Vector3f& v = (*coord_array)[vertex_index];
    Float weight = 1.0f / v.sqr_distance(center);
    *info++ = Vertex_facet_info(facet_index, weight);
    return info;
  }

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

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
  const GLfloat* local_color_data() const;

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

  /*! Obtain the flat coordinate indices data.
   * \return the flat coordinate indices data.
   */
  const GLvoid* flat_coord_indicaes_data() const;

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

  /*! Obtain the number of tex ture coordinates.
   * \return the number of tex ture coordinates.
   */
  Uint num_tex_coordinates() const;

private:
  /*! The key for the utility map, which maps a tuple of 3 ids, namely,
   * coordinate id, normal/color id, and texture coordinate id, to a single
   * id.
   */
  typedef std::tuple<Uint, Uint, Uint> Id_key;

  /*! The hash specialized function.
   * It maps 3 ids to their sum.
   */
  struct Id_hash {
    std::size_t operator()(Id_key const& key) const
    { return std::get<0>(key) + std::get<1>(key) + std::get<2>(key); }
  };

  /*! The type Id_map maps from tuples of 3 ids to ids. */
  typedef boost::unordered_map<Id_key, Uint, Id_hash> Id_map;

  /*! The coordinates vertex array. */
  std::vector<Vector3f> m_local_coord_buffer;

  /*! The normals vertex array. */
  std::vector<Vector3f> m_local_normal_buffer;

  /*! The colors vertex array. */
  std::vector<Vector3f> m_local_color_buffer;

  /*! The texture coordinates vertex array. */
  std::vector<Vector2f> m_local_tex_coord_buffer_2d;
  std::vector<Vector3f> m_local_tex_coord_buffer_3d;
  std::vector<Vector4f> m_local_tex_coord_buffer_4d;

  /*! The index vertex array. */
  std::vector<Uint> m_local_indices;

  /*! The map from tuples of 3 ids to ids. */
  Id_map m_id_map;

  /*! A functor that computes the normal of a given facet. */
  struct Normal_vector {
    template <typename Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      // Facet::Plane_3 is the normal vector type. We assume the
      // CGAL Kernel here and use its global functions.
      Vector normal = CGAL::cross_product(h->next()->vertex()->point() -
                                          h->vertex()->point(),
                                          h->next()->next()->vertex()->point() -
                                          h->next()->vertex()->point());
      return normal / CGAL::sqrt(normal.squared_length());
    }
  };

  /*! The builder. */
  Polyhedron_geo_builder<HalfedgeDS> m_surface;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Default values */
  static const Boolean s_def_normal_per_vertex;
  static const Boolean s_def_color_per_vertex;

  /*! Calculate vertex information. Used to compute a single normal per vertex.
   * \param vertices_info (out) the container of the resulting information.
   */
  void calculate_vertices_info(Vertices_info& vertices_info);
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Indexed_face_set* Indexed_face_set::prototype()
{ return new Indexed_face_set(true); }

//! \brief clones.
inline Container* Indexed_face_set::clone() { return new Indexed_face_set(); }

/*! \brief determines whether the conditions allow for the use of openGl vertex
 * array.
 * Configuration specifies VERTEX_ARRAY, and
 * Primitive types supported so far: QUADS and TRIANGLES, and
 * Attachment or color/normal is PER_VERTEX.
 */
inline Boolean Indexed_face_set::use_vertex_array() const
{
  Fragment_source fragment_source = resolve_fragment_source();
  return ((m_drawing_mode == Configuration::GDM_VERTEX_ARRAY) &&
          ((m_primitive_type == PT_QUADS) ||
           (m_primitive_type == PT_TRIANGLES)) &&
          ((fragment_source == FS_COLOR) ?
           (m_color_attachment == PER_VERTEX) :
           (m_normal_attachment == PER_VERTEX)));
}

/*! \brief determines whether the representation of the normals hasn't been
 * cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_normals() const
{ return m_dirty_normals; }

/*! \brief determines whether the representation of the normals hasn't been
 * cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_tex_coords() const
{ return m_dirty_tex_coords; }

//! \brief obtains the normal per vertex mode.
inline Boolean Indexed_face_set::get_normal_per_vertex() const
{ return m_normal_per_vertex; }

//! \brief obtains the color per vertex mode.
inline Boolean Indexed_face_set::get_color_per_vertex() const
{ return m_color_per_vertex; }

//! \brief obtains local coordinate data.
inline const GLfloat* Indexed_face_set::local_coord_data() const
{
  SGAL_assertion(! m_local_coord_buffer.empty());
  return (GLfloat*)(&(*(m_local_coord_buffer.begin())));
}

//! \brief obtains local normal data.
inline const GLfloat* Indexed_face_set::local_normal_data() const
{
  SGAL_assertion(! m_local_normal_buffer.empty());
  return (GLfloat*)(&(*(m_local_normal_buffer.begin())));
}

//! \brief obtains local color data.
inline const GLfloat* Indexed_face_set::local_color_data() const
{
  SGAL_assertion(! m_local_color_buffer.empty());
  return (GLfloat*)(&(*(m_local_color_buffer.begin())));
}

//! \brief obtains local 2d teture coordinate data.
inline const GLfloat* Indexed_face_set::local_tex_coord_2d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_2d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_2d.begin())));
}

//! \brief obtains local 3d teture coordinate data.
inline const GLfloat* Indexed_face_set::local_tex_coord_3d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_3d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_3d.begin())));
}

//! \brief obtains local 4d teture coordinate data.
inline const GLfloat* Indexed_face_set::local_tex_coord_4d_data() const
{
  SGAL_assertion(! m_local_tex_coord_buffer_4d.empty());
  return (GLfloat*)(&(*(m_local_tex_coord_buffer_4d.begin())));
}

//! \brief obtains the local indices data.
inline const GLvoid* Indexed_face_set::local_indices_data() const
{
  SGAL_assertion(! m_local_indices.empty());
  return (GLvoid*)(&(*(m_local_indices.begin())));
}

//! Obtain the flat coordinate indices data.
inline const GLvoid* Indexed_face_set::flat_coord_indicaes_data() const
{ return (GLvoid*)(&(*(m_flat_coord_indices.begin()))); }

//! \brief obtains coordinate data..
inline const GLfloat* Indexed_face_set::coord_data() const
{
  return m_local_coord_buffer.empty() ? m_coord_array->data() :
    local_coord_data();
}

//! \brief obtains normal data.
inline const GLfloat* Indexed_face_set::normal_data() const
{
  return m_local_normal_buffer.empty() ? m_normal_array->data() :
    local_normal_data();
}

//! \brief obtains color data.
inline const GLfloat* Indexed_face_set::color_data() const
{
  return m_local_color_buffer.empty() ? m_color_array->data() :
    local_color_data();
}

//! \brief obtains teture coordinate data.
inline const GLfloat* Indexed_face_set::tex_coord_data() const
{
  return
    (! m_local_tex_coord_buffer_2d.empty()) ? local_tex_coord_2d_data() :
    ((! m_local_tex_coord_buffer_3d.empty()) ? local_tex_coord_3d_data() :
     ((! m_local_tex_coord_buffer_4d.empty()) ? local_tex_coord_4d_data() :
      m_tex_coord_array->data()));
}

//! Obtain the indices.
inline const GLvoid* Indexed_face_set::indices_data() const
{
  return m_local_indices.empty() ? flat_coord_indicaes_data() :
    local_indices_data();
}

//! \brief obtains the number of tex ture coordinates.
inline Uint Indexed_face_set::num_tex_coordinates() const
{
  return
    (! m_local_tex_coord_buffer_2d.empty()) ? 2 :
    ((! m_local_tex_coord_buffer_3d.empty()) ? 3 :
     ((! m_local_tex_coord_buffer_4d.empty()) ? 4 :
      ((m_tex_coord_array != nullptr) ?
       m_tex_coord_array->num_coordinates() : 0)));
}

SGAL_END_NAMESPACE

#endif
