// Copyright (c) 2012 Israel.
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

#ifndef SGAL_EGO_GEO_HPP
#define SGAL_EGO_GEO_HPP

/*! \file
 * A geometry container that represents a model decomposed into ego bricks.
 */

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <list>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"

#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels.hpp"
#include "SEGO/Ego_voxels_tiler.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Cull_context;
class Polyhedron_geo;
class Exact_polyhedron_geo;
class Scene_graph;
class Appearance;
class Coord_array;
class Normal_array;
class Tex_coord_array;
class Geo_set;

class SGAL_CLASSDEF Ego : public Transform {
public:
  enum {
    FIRST = Transform::LAST - 1,
    MODEL,
    VOXEL_WIDTH,
    VOXEL_LENGTH,
    VOXEL_HEIGHT,
    FIRST_TILE_PLACEMENT,
    TILING_STRATEGY,
    TILING_ROWS_DIRECTION,
    PARTS,
    STYLE,
    APPEARANCE,
    KNOB_SLICES,
    SPACE_FILLING,
    LAST
  };

  enum Style { STYLE_RANDOM_COLORS, STYLE_APPEARANCE, STYLE_DISCRETE_CUBE_MAP };
  
  /*! Constructor */
  Ego(Boolean proto = false);

  /*! Destructor */
  virtual ~Ego();

  /* Construct the prototype */
  static Ego* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the container prototype */
  virtual void init_prototype();
  
  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene.
   * \param scene_graph the given scene
   */  
  virtual void add_to_scene(Scene_graph* scene_graph);
  
  // virtual Attribute_list get_attributes();

  /*! Draw the geometry */
  virtual Action::Trav_directive draw(Draw_action* action);

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Calculate sphere bound of the node */
  virtual Boolean clean_sphere_bound();

  /*! Obtain the sphere bound */
  const Sphere_bound& get_sphere_bound();

  /*! Cleane the appearance. */
  void clean_appearance();

  /*! Create a random appearance. */
  Appearance* create_random_appearance();

  /*! Create the geometry of a brick. */
  Geometry* create_geometry(Boolean draw_knobs, Vector3f& center);
  
  /*! Create the geometry of a brick. */
  Geometry* create_geometry(Boolean draw_knobs);

  /*! Clean the voxels. */
  void clean_voxels();

  /*! Clean the tiling. */
  void clean_tiling();

  /*! (Re)generate the parts. */
  void clean_parts();

  /*! (Re)generate the part colors. */
  void clean_colors(Draw_action* action);
  
  /*! Clear the internal representation and auxiliary data structures */
  void clear();

  /*! Clear the parts */
  void clear_parts();
  
  /*! Is the representation empty ?
   */
  Boolean is_empty();

  /*! Determine whether the type of the model is 'Polyhedron'.
   * \return true if the type of the model is 'Polyhedron'.
   */
  Boolean is_model_polyhedron() const;

  /*! Determine whether the type of the model is 'Exact_polyhedron'.
   * \return true if the type of the model is 'Exact_polyhedron'.
   */
  Boolean is_model_exact_polyhedron() const;

  /*! Determine whether the base type of the model is 'Get_set'.
   * \return true if the base type of the model is 'Get_set'.
   */
  Boolean is_model_geo_set() const;

  /*! Set the model.
   * \param model the model.
   */
  void set_model(Polyhedron_geo* model);
  void set_model(Exact_polyhedron_geo* model);
  void set_model(Geo_set* model);

  /*! Obtain the (const) polyhedron model.
   * \return the model.
   */
  const Polyhedron_geo* get_polyhedron_model() const;

  /*! Obtain the (non-const) polyhedron model.
   * \return the model.
   */
  Polyhedron_geo* get_polyhedron_model();
  
  /*! Obtain the (const) exact polyhedron model.
   * \return the model.
   */
  const Exact_polyhedron_geo* get_exact_polyhedron_model() const;

  /*! Obtain the (non-const) exact polyhedron model.
   * \return the model.
   */
  Exact_polyhedron_geo* get_exact_polyhedron_model();

  /*! Obtain the (const) geometry-set model.
   * \return the model.
   */
  const Geo_set* get_geo_set_model() const;

  /*! Obtain the (non-const) geometry-set model.
   * \return the model.
   */
  Geo_set* get_geo_set_model();

  /*! Set the horizontal width of the voxel */
  void set_voxel_width(Float voxel_width) { m_voxel_width = voxel_width; }

  /*! Obtain the horizontal width of the voxel */
  Float get_voxel_width() const { return m_voxel_width; }

  /*! Set the horizontal length of the voxel */
  void set_voxel_length(Float voxel_length) { m_voxel_length = voxel_length; }

  /*! Obtain the horizontal length of the voxel */
  Float get_voxel_length() const { return m_voxel_length; }

  /*! Set the height of the voxel */
  void set_voxel_height(Float voxel_height) { m_voxel_height = voxel_height; }

  /*! Obtain the height of the voxel */
  Float get_voxel_height() const { return m_voxel_height; }
  
  void set_first_tile_placement(Ego_voxels_tiler::First_tile_placement p)
  { m_first_tile_placement = p; }

  void set_tiling_strategy(Ego_voxels_tiler::Strategy s)
  { m_tiling_strategy = s; }
  
  void set_tiling_rows_direction(Ego_voxels_tiler::Tiling_rows r)
  { m_tiling_rows_direction = r; }

  /*! Notify about a change in the model */
  void model_changed(Field_info* field_info = NULL);

  /*! Notify about a change in the voxels */
  void voxels_changed(Field_info* field_info = NULL);

  /*! Notify about a change in the tiling */
  void tiling_changed(Field_info* field_info = NULL);

  /*! Obtain the (const) appearance.
   * \return the appearance.
   */
  const Appearance* get_appearance() const;

  /*! Obtain the (non-const) appearance.
   * \return the appearance.
   */
  Appearance* get_appearance();

  /*! Set an appearance.
   * \param appearance the new appearance.
   */
  void set_appearance(Appearance* appearance);

  /*! Process change of appearance. */
  void appearance_changed(Field_info* /* field_info. */);

  /*! Obtain the style. */
  Style get_style() const;

  /*! Set the style. */
  void set_style(Style style);

  /*! Set the knob slicess number. */
  void set_knob_slices(Uint slices);

  /*! Obtain the knob slices number. */
  Uint get_knob_slices() const;

  /*! Determine whether the parts are space filling. */
  Boolean is_space_filling() const;

  /*! Set the flag that indicates whether the parts are space filling. */
  void set_space_filling(Boolean flag);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  void adjust_voxels_for_tiling();

  /*! The segments */
  boost::variant<Polyhedron_geo*, Exact_polyhedron_geo*, Geo_set*> m_model;

  /*! The horizontal voxel width */
  Float m_voxel_width;

  /*! The horizontal voxel length */
  Float m_voxel_length;

  /*! The voxel height */
  Float m_voxel_height;

  Ego_voxels m_voxels;

  Ego_voxels m_tiled_voxels;

  Exact_polyhedron_geo::Kernel::Point_3 m_tiled_voxels_origin;
  
  /*! The style */
  Style m_style;

  /*! The apperance attribute. */
  Appearance* m_appearance;

  /*! Indicates whether the parts are space filling. */
  Boolean m_space_filling;

  /*! Stores the pervious appearance. */
  Appearance* m_appearance_prev;

  /// Enums to control tiling.
  Ego_voxels_tiler::First_tile_placement m_first_tile_placement;
  Ego_voxels_tiler::Strategy m_tiling_strategy;
  Ego_voxels_tiler::Tiling_rows m_tiling_rows_direction;
  
  /*! Indicates whether the shape appearance is dirty, and thus needs
   * cleaning.
   */
  Boolean m_dirty_appearance;

  /*! Indicates whether the data structure must be cleaned */
  Boolean m_dirty_voxels;
  Boolean m_dirty_tiling;
  Boolean m_dirty_parts;
  Boolean m_dirty_colors;

  /*! Indicates whether the parts are "owned". If they are owned (as
   * the user hasn't provided any) the parts should be destructed when
   * Ego is destructed.
   */
  Boolean m_owned_parts;
  
  typedef boost::unordered_map<Uint, Appearance*>       Appearance_map;
  typedef Appearance_map::iterator                      Appearance_iter;
  Appearance_map m_appearances;

  typedef std::list<Material*>                          Material_list;
  typedef Material_list::iterator                       Material_iter;
  Material_list m_materials;

  typedef std::list<Ego_brick*>                         Ego_brick_list;
  typedef Ego_brick_list::iterator                      Ego_brick_iter;
  Ego_brick_list m_bricks;
  Ego_brick_list m_knobless_bricks;
  
  /*! The Scene_graph */
  Scene_graph* m_scene_graph;

  /*! The number of slices of a knob. */
  Uint m_knob_slices;

private:
  /*! Indicates whether the appearance is "owned". If it is owned (as the
   * user hasn't provided one) the appearance should be destructed when Ego
   * is destructed.
   */
  Boolean m_owned_appearance;

  /*! Indicates whether cleaning the color parts is in progress. 
   * Upon invocation of clean_parts(), if this flag is on, the function
   * immediately returns.
   */
  Boolean m_clean_colors_in_progress;
  
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! The array of style names. */
  static const char* s_style_names[];
  
  /*! Default values */
  static const Ego_voxels_tiler::First_tile_placement
    s_def_first_tile_placement;
  static const Ego_voxels_tiler::Strategy s_def_tiling_strategy;
  static const Ego_voxels_tiler::Tiling_rows s_def_tiling_rows_direction;
  static const Float s_def_voxel_width;
  static const Float s_def_voxel_length;
  static const Float s_def_voxel_height;
  static const Style s_def_style;
  static const Boolean s_def_space_filling;
};

/* \brief constructs the prototype. */
inline Ego* Ego::prototype() { return new Ego(true); }

/*! \brief clones. */
inline Container* Ego::clone() { return new Ego(); }

/*! \brief determines whether the type of the model is 'Polyhedron'. */
inline Boolean Ego::is_model_polyhedron() const
{  return (boost::get<Polyhedron_geo*> (&m_model) != NULL); }

/*! \brief determines whether the type of the model is 'Exact_polyhedron'. */
inline Boolean Ego::is_model_exact_polyhedron() const
{ return (boost::get<Exact_polyhedron_geo*> (&m_model) != NULL); }

/*! \brief determines whether the base type of the model is 'Get_set'. */
inline Boolean Ego::is_model_geo_set() const
{ return (boost::get<Geo_set*> (&m_model) != NULL); }

/*! \brief sets the model. */
inline void Ego::set_model(Polyhedron_geo* model) { m_model = model; }
inline void Ego::set_model(Exact_polyhedron_geo* model) { m_model = model; }
inline void Ego::set_model(Geo_set* model) { m_model = model; }

/*! \brief obtains the (const) appearance. */
inline const Appearance* Ego::get_appearance() const { return m_appearance; }

/*! \brief obtains the (non-const) appearance. */
inline Appearance* Ego::get_appearance() { return m_appearance; }

/*! \brief obtains the style. */
inline Ego::Style Ego::get_style() const { return m_style; }

/*! \brief determines whether the parts are space filling. */
inline Boolean Ego::is_space_filling() const { return m_space_filling; }

/*! \brief obtains the knob slices number. */
inline Uint Ego::get_knob_slices() const { return m_knob_slices; }

SGAL_END_NAMESPACE

#endif
