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

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

/*! This class describes a general mesh made of facets */
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

  /* Construct the prototype */
  static Indexed_face_set* prototype()
  { return new Indexed_face_set(true); }

  /*! Clone */
  virtual Container* clone() { return new Indexed_face_set(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the node prototype */
  virtual void delete_prototype();

  /*! Obtain the node prototype */  
  virtual Container_proto* get_prototype();
  
  /*! Sets the attributes of this node */
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

  /*! Calculate a single normal per vertex for all vertices */
  void calculate_normals_per_vertex();

  /*! Calculate a single normal per polygon for all polygons */
  void calculate_normals_per_polygon();
  
  /*! Allocate the normal array in case it is not present.
   * If the creaseAngle field is greater than 0, a normal is allocated per
   * vertes. Otherwise a normal is allocated per polygon.
   */
  void allocate_normals();

  /*! Calculate the normals in case they are invalidated.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  void clean_normals();

  /*! Clear the representation of the normals */
  virtual void clear_normals() { m_dirty_normals = true; }
  
  /*! Determine whether the representation of the normals hasn't been updated */
  Boolean is_dirty_normals() const { return m_dirty_normals; }
  
  /*! Allocate the texture coordinate array in case it is not present */
  void allocate_tex_coords();
  
  /*! Calculate the texture coordinates in case they are invalidated */
  void clean_tex_coords();

  /*! Clear the representation of the normals */
  void clear_tex_coords() { m_dirty_tex_coords = true; }
  
  /*! Determine whether the representation of the normals hasn't been updated */
  Boolean is_dirty_tex_coords() const { return m_dirty_tex_coords; }

  void set_normal_per_vertex(Boolean normal_per_vertex);
  Boolean get_normal_per_vertex() { return m_normal_per_vertex; }

  void set_color_per_vertex(Boolean color_per_vertex);
  Boolean get_color_per_vertex() { return m_color_per_vertex; }
  
  virtual void isect(Isect_action* action); 
  virtual int create_display_list(Draw_action* action);

  /*! Clear the representation */
  virtual void clear();
  
  /*! Return true if the representation is empty */
  virtual Boolean is_empty() const;

  /*! Draw the mesh conditionaly */
  virtual void draw(Draw_action* action);

  /*! Draw the representation */
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
  Int* m_tri_strip_lengths;

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

  /*! Indicates whether space was allocated to accomodate default normals */
  Boolean m_own_normal_array;

  /*! Indicates whether space was allocated to accomodate default texture
   * coordinates
   */
  Boolean m_own_tex_coord_array;

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

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Default value */
  static const Boolean s_def_normal_per_vertex;
  static const Boolean s_def_color_per_vertex;
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
