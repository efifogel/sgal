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
// $Revision: $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_INDEXED_FACE_SET_HPP
#define SGAL_INDEXED_FACE_SET_HPP

#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Indexed_face_set_geometry.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Normal_array.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

/*! This class describes a general mesh made of facets. */
class SGAL_CLASSDEF Indexed_face_set : public Mesh_set {
public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    COLOR_PER_VERTEX,
    NORMAL_PER_VERTEX,
    LAST
  };

  /*! Constructor */
  Indexed_face_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Indexed_face_set();

  /* Construct the prototype. */
  static Indexed_face_set* prototype()
  { return new Indexed_face_set(true); }

  /*! Clone. */
  virtual Container* clone() { return new Indexed_face_set(); }

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */  
  virtual Container_proto* get_prototype();
  
  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   * \param sg a pointer to the scene graph
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
  
  /*! Calculate the normals in case they are invalidated.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  void clean_normals();

  /*! Clear the representation of the normals. */
  virtual void clear_normals() { m_dirty_normals = true; }
  
  /*! Determine whether the representation of the normals hasn't been
   * cleaned.
   */
  Boolean is_dirty_normals() const { return m_dirty_normals; }
  
  /*! Calculate the texture coordinates in case they are invalidated. */
  void clean_tex_coords();

  /*! Clear the representation of the normals. */
  void clear_tex_coords() { m_dirty_tex_coords = true; }
  
  /*! Determine whether the representation of the normals hasn't been
   * cleaned.
   */
  Boolean is_dirty_tex_coords() const { return m_dirty_tex_coords; }

  /* Set the flag that indicates whether normals are bound per vertex or per
   * face.
   * \param normal_per_vertex true if normals are bound per vertex
   */
  void set_normal_per_vertex(Boolean normal_per_vertex);
  Boolean get_normal_per_vertex() { return m_normal_per_vertex; }

  /* Set the flag that indicates whether colors are bound per vertex or per
   * face.
   * @param color_per_vertex true if normals are bound per vertex
   */
  void set_color_per_vertex(Boolean color_per_vertex);

  Boolean get_color_per_vertex() { return m_color_per_vertex; }

  /*! Draw the polygons for selection. */
  virtual void isect(Isect_action* action); 

  virtual int create_display_list(Draw_action* action);

  /*! Clear the representation. */
  virtual void clear();
  
  /*! Return true if the representation is empty. */
  virtual Boolean is_empty() const;

  /*! Draw the polygons. */
  virtual void draw(Draw_action* action);

  /*! Draw the representation.
   * \param action action.
   */
  virtual void draw_geometry(Draw_action* action);
  
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

  /*! Process change of coordinates */
  virtual void coord_changed(SGAL::Field_info* field_info);
  
  /*! Process change of field */
  virtual void field_changed(Field_info* field_info);
  
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

  /*! The id of the display list */
  Uint m_display_list_id;

  /*! The id of the coord array */
  Uint m_vertex_coord_id;

  /*! The id of the color array */
  Uint m_vertex_color_id;

  /*! The id of the normal array */
  Uint m_vertex_normal_id;

  /*! The id of the tex. coord array */
  Uint m_vertex_tex_coord_id;

  /*! The geometry drawing-mode {direct, display list, or vertex array */
  Configuration::Geometry_drawing_mode m_drawing_mode;

  /*! Indicates whether the indices have been converted to fixed size */
  Boolean m_indices_dirty;
  
  // data structures to hold the low level geometry
  Int m_tri_strip_lengths_size;

  /* if the bbox is read from the xml file, this is true */
  Boolean m_bb_is_pre_set;

  Boolean m_is_progressive;

  /*! Indicates that the normal array has been invalidated */
  Boolean m_dirty_normals;

  /*! Indicates that the texture coordinate array has been invalidated */
  Boolean m_dirty_tex_coords;
  
  /*! Indicates that the vertex buffer objects have been created */
  Boolean m_vertex_buffer_object_created;

  /*! Create the data structure of the vertex buffer object */
  void create_vertex_buffer_object();

  /*! Destroy the data structure of the vertex buffer object */
  void destroy_vertex_buffer_object();

  /*! Destroy the data structure of the display_list */
  void destroy_display_list();

  /*! Clear the vertex arrays */
  void clear_vertex_arrays();

  /*! Destroy the vertex-index arrays */
  void destroy_vertex_index_arrays();
  
  /*! Isect direct drawing-mode */
  void isect_direct();

  /*! Dispatch the appropriate drawing routine */
  void draw_dispatch(Draw_action* action);

  /*! Do the conditions allow for the use of openGl vertex array? */
  Boolean use_vertex_array() const;

  /*! Compute the normalized normal to a facet from 3 points lying on the
   * facet.
   */
  void compute_normal(const Vector3f& v1, const Vector3f& v2,
                      const Vector3f& v3, Vector3f& normal) const;

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
    // Compute the receiprocal of the square distance from the facet center
    Vector3f& v = (*m_coord_array)[vertex_index];
    Float weight = 1.0f / v.sqr_distance(center);
    *info++ = Vertex_facet_info(facet_index, weight);
    return info;
  }

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Determines whether the angle between two given vectors is smooth.
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
  
private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Default value */
  static const Boolean s_def_normal_per_vertex;
  static const Boolean s_def_color_per_vertex;

  /*! Calculate vertex information. Used to compute a single normal per vertex.
   * \param vertices_info (out) the container of the resulting information.
   */
  void calculate_vertices_info(Vertices_info& vertices_info);
};

/*! Do the conditions allow for the use of openGl vertex array?
 * Configuration specifies VERTEX_ARRAY, and
 * Primitive types supported so far: QUADS and TRIANGLES, and
 * Attachment or color/normal is PER_VERTEX, and
 * Texture is not indexed, and
 * color/normal is not indexed.
 */
inline Boolean Indexed_face_set::use_vertex_array() const
{
  Fragment_source fragment_source = resolve_fragment_source();
  return ((m_drawing_mode == Configuration::GDM_VERTEX_ARRAY) &&
          ((m_primitive_type == PT_QUADS) ||
           (m_primitive_type == PT_TRIANGLES)) &&
          ((fragment_source == FS_COLOR) ?
           (m_color_attachment == PER_VERTEX) :
           (m_normal_attachment == PER_VERTEX)) &&
          (m_tex_coord_indices.size() == 0) &&
          ((fragment_source == FS_COLOR) ?
           (m_color_indices.size() == 0) : (m_normal_indices.size() == 0))
          );
}

SGAL_END_NAMESPACE

#endif
