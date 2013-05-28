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
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

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
class Mesh_set;
class Touch_sensor;
class Ego_voxels_filler_base;

class SGAL_CLASSDEF Ego : public Transform {
public:
  typedef boost::shared_ptr<Appearance>           Shared_appearance;
  typedef boost::shared_ptr<Material>             Shared_material;
  typedef boost::shared_ptr<Mesh_set>             Shared_mesh_set;
  typedef boost::shared_ptr<Ego_brick>            Shared_ego_brick;
  typedef boost::shared_ptr<Polyhedron_geo>       Shared_polyhedron_geo;
  typedef boost::shared_ptr<Exact_polyhedron_geo> Shared_exact_polyhedron_geo;
  
  enum {
    FIRST = Transform::LAST - 1,
    MODEL,
    VOXEL_WIDTH,
    VOXEL_LENGTH,
    VOXEL_HEIGHT,
    EVEN_LAYER_X,
    EVEN_LAYER_Y,
    ODD_LAYER_X,
    ODD_LAYER_Y,
    OFFSET_BETWEEN_ROWS,
    TILING_ROWS_DIRECTION,
    PARTS,
    STYLE,
    APPEARANCE,
    KNOB_SLICES,
    SPACE_FILLING,
    COLOR_SPACE,
    LAYER_X_VISIBILITY,
    LAYER_Y_VISIBILITY,
    LAYER_Z_VISIBILITY,
    BRICK_TYPES,
    SELECTION_ID,
    SMOOTH,
    LAST
  };

  enum Color_space {COLOR_SPACE_RGB, COLOR_SPACE_HSL};
  
  enum Style { STYLE_RANDOM_COLORS, STYLE_APPEARANCE, STYLE_DISCRETE_CUBE_MAP };

  enum Layer_visibility {
    LV_ALL, LV_ABOVE, LV_NOT_ABOVE, LV_BELOW, LV_NOT_BELOW, LV_ONLY, LV_NOT_ONLY
  };
  
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

  // virtual Attribute_list get_attributes();

  /*! Draw the geometry */
  virtual Action::Trav_directive draw(Draw_action* action);

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! Draw the Ego object in selection mode. */
  virtual void isect(Isect_action* action);

  /*! Calculate sphere bound of the node */
  virtual Boolean clean_sphere_bound();

  /*! Obtain the sphere bound */
  const Sphere_bound& get_sphere_bound();

  /*! Cleane the appearance. */
  void clean_appearance();

  /*! Create a random appearance. */
  Shared_appearance create_random_appearance();

  /*! Create an appearance. */
  Shared_appearance create_appearance(Uint hue_key, Uint saturation_key,
                                      Uint luminosity_key);
  
  /*! Create the geometry of a brick. */
  Shared_mesh_set create_geometry(Uint num0, Uint num1, Boolean draw_knobs,
                                  Vector3f& center);
  
  /*! Create the geometry of a brick. */
  Shared_mesh_set create_geometry(Uint num0, Uint num1, Boolean draw_knobs);

  /*! Clean the voxels. */
  void clean_voxels();

  /*! Clean the tiling. */
  void clean_tiling();

  /*! (Re)generate the parts. */
  void clean_parts();

  /*! Clean (set) the part visibility flags. */
  void clean_visibility();

  /*! Reset the part visibility flags. */
  void reset_visibility();
  
  /*! (Re)generate the part colors. */
  void clean_colors();

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
  Boolean is_model_mesh_set() const;

  /*! Set the model.
   * \param model the model.
   */
  void set_model(Shared_polyhedron_geo model);
  void set_model(Shared_exact_polyhedron_geo model);
  void set_model(Shared_mesh_set model);

  /*! Obtain the (const) polyhedron model.
   * \return the model.
   */
  const Shared_polyhedron_geo get_polyhedron_model() const;

  /*! Obtain the (non-const) polyhedron model.
   * \return the model.
   */
  Shared_polyhedron_geo get_polyhedron_model();
  
  /*! Obtain the (const) exact polyhedron model.
   * \return the model.
   */
  const Shared_exact_polyhedron_geo get_exact_polyhedron_model() const;

  /*! Obtain the (non-const) exact polyhedron model.
   * \return the model.
   */
  Shared_exact_polyhedron_geo get_exact_polyhedron_model();

  /*! Obtain the (const) geometry-set model.
   * \return the model.
   */
  const Shared_mesh_set get_mesh_set_model() const;

  /*! Obtain the (non-const) geometry-set model.
   * \return the model.
   */
  Shared_mesh_set get_mesh_set_model();

  /*! Set the horizontal width of the voxel */
  void set_voxel_width(Float voxel_width);

  /*! Obtain the horizontal width of the voxel */
  Float get_voxel_width() const;

  /*! Set the horizontal length of the voxel */
  void set_voxel_length(Float voxel_length);

  /*! Obtain the horizontal length of the voxel */
  Float get_voxel_length() const;

  /*! Set the height of the voxel */
  void set_voxel_height(Float voxel_height);

  /*! Obtain the height of the voxel */
  Float get_voxel_height() const;

  /*! */
  void set_even_layer_x(std::size_t x);

  /*! */
  void set_even_layer_y(std::size_t y);

  /*! */
  void set_odd_layer_x(std::size_t x);

  /*! */
  void set_odd_layer_y(std::size_t y);

  /*! */
  void set_offset_between_rows(std::size_t s);
  
  /*! */
  void set_tiling_rows_direction(Ego_voxels_tiler::Tiling_rows r);

  /*! Notify about a change in the model */
  void model_changed(Field_info* field_info = NULL);

  /*! Notify about a change in the voxels */
  void voxels_changed(Field_info* field_info = NULL);

  /*! Notify about a change in the tiling */
  void tiling_changed(Field_info* field_info = NULL);

  /*! Obtain the (const) appearance.
   * \return the appearance.
   */
  const Shared_appearance get_appearance() const;

  /*! Obtain the (non-const) appearance.
   * \return the appearance.
   */
  Shared_appearance get_appearance();

  /*! Set an appearance.
   * \param appearance the new appearance.
   */
  void set_appearance(Shared_appearance appearance);

  /*! Process change of appearance. */
  void appearance_changed(Field_info* /* field_info. */);

  /*! Process change of visibility scheme. */
  void visibility_changed(Field_info* /* field_info. */);

  /*! Process change of selected brick. */
  void selection_id_changed(Field_info* /* field_info. */);
  

  /*! Set a filler
   * \param filler the new filler.
   */
  void set_filler(Ego_voxels_filler_base* filler);

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

  /*! Obtain the color space. */
  Color_space get_color_space() const;

  /*! Set the style. */
  void set_color_space(Color_space color_space);

  /*! Obtain the layer-x index to use in the visibility computation. */
  Uint get_layer_x() const;

  /*! Set the layer-x index to use in the visibility computation. */
  void set_layer_x(Uint layer);

  /*! Obtain the layer-y index to use in the visibility computation. */
  Uint get_layer_y() const;

  /*! Set the layer-y index to use in the visibility computation. */
  void set_layer_y(Uint layer);

  /*! Obtain the layer-z index to use in the visibility computation. */
  Uint get_layer_z() const;

  /*! Set the layer-z index to use in the visibility computation. */
  void set_layer_z(Uint layer);
  
  /*! Obtain the layer-x visibility scheme. */
  Layer_visibility get_layer_x_visibility() const;

  /*! Set the layer-x visibility scheme. */
  void set_layer_x_visibility(Layer_visibility layer_visibility);

  /*! Obtain the layer-y visibility scheme. */
  Layer_visibility get_layer_y_visibility() const;

  /*! Set the layer-y visibility scheme. */
  void set_layer_y_visibility(Layer_visibility layer_visibility);

  /*! Obtain the layer-z visibility scheme. */
  Layer_visibility get_layer_z_visibility() const;

  /*! Set the layer-z visibility scheme. */
  void set_layer_z_visibility(Layer_visibility layer_visibility);
  
  /*! Determine whether the parts are smooth. */
  Boolean is_smooth() const;

  /*! Set the flag that indicates whether the parts are smooth. */
  void set_smooth(Boolean flag);

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const;

  Ego_voxels_tiler::Brick_types convert_types(const SGAL::Array<Vector3sh> &types);

  /*! Determine whether a given brick is visible with respect to a specific
   * layer.
   */
  Boolean is_visible(Layer_visibility lv, Uint layer_index, Uint brick_index);
  
  /*! The segments */
  boost::variant<Shared_polyhedron_geo, Shared_exact_polyhedron_geo,
                 Shared_mesh_set> m_model;

  /*! Find a child of certain type */
  template <class T>
  T* find() {
    for (Node_iterator it = m_childs.begin(); it != m_childs.end(); ++it) {
      T* res = dynamic_cast<T*>(*it);
      if (res != NULL) return res;
    }
    return NULL;
  }

  /*! The horizontal voxel width */
  Float m_voxel_width;

  /*! The horizontal voxel length */
  Float m_voxel_length;

  /*! The voxel height */
  Float m_voxel_height;

  Ego_voxels m_voxels;

  Exact_polyhedron_geo::Kernel::Point_3 m_voxels_center;
  
  /*! The style */
  Style m_style;

  /*! The apperance attribute. */
  Shared_appearance m_appearance;

  /*! Indicates whether the parts are space filling. */
  Boolean m_space_filling;

  Boolean m_owned_filler;
  Ego_voxels_filler_base* m_filler;

  /// Tiling parameter
  std::size_t m_even_layer_x;
  std::size_t m_even_layer_y;
  std::size_t m_odd_layer_x;
  std::size_t m_odd_layer_y;
  std::size_t m_offset_between_rows;
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

  Boolean m_dirty_visibility;

  /*! Indicates whether the parts are "owned". If they are owned (as
   * the user hasn't provided any) the parts should be destructed when
   * Ego is destructed.
   */
  Boolean m_owned_parts;

  /*! Indicates whether the touch sensor is "owned". If it is owned (as
   * the user hasn't provided any) the touch sensor should be destructed when
   * Ego is destructed.
   */
  Boolean m_owned_touch_sensor;

  typedef boost::unordered_map<Uint, Shared_appearance>  Appearance_map;
  typedef Appearance_map::iterator                       Appearance_iter;
  Appearance_map m_appearances;

  typedef std::list<Shared_material>                     Material_list;
  typedef Material_list::iterator                        Material_iter;
  Material_list m_materials;

  typedef std::pair<Uint, Uint>                          Ego_brick_key;
  typedef std::multimap<Ego_brick_key, Shared_ego_brick> Ego_brick_map;
  typedef Ego_brick_map::iterator                        Ego_brick_iter;
  Ego_brick_map m_bricks;
  Ego_brick_map m_knobless_bricks;

  typedef boost::tuple<std::size_t, std::size_t, std::size_t>
                                                         Voxel_signature;
  typedef std::vector<Voxel_signature>                   Voxel_signatures;
  typedef Voxel_signatures::iterator                     Voxel_signatures_iter;
  
  /*! The number of slices of a knob. */
  Uint m_knob_slices;

  /*! Indicates which color space to use to average color. */
  Color_space m_color_space;

  /*! Indicates the layer-x index to use in the visibility computation. */
  Uint m_layer_x;

  /*! Indicates the layer-y index to use in the visibility computation. */
  Uint m_layer_y;

  /*! Indicates the layer-z index to use in the visibility computation. */
  Uint m_layer_z;
  
  /*! Indicates the layer-x visibility scheme. */
  Layer_visibility m_layer_x_visibility;

  /*! Indicates the layer-y visibility scheme. */
  Layer_visibility m_layer_y_visibility;

  /*! Indicates the layer-z visibility scheme. */
  Layer_visibility m_layer_z_visibility;
  
  /*! Pieces with which it is OK to tile. */
  // I am not using Vector3u because there is none, and I don't have
  // the energy to create the right templates so it won't be repeatative.
  SGAL::Array<Vector3sh> m_brick_types;

  /*! A flag raised when the touch sensor senses a change. */
  Uint m_selection_id;
  
  /*! A sorted vector of voxel signatures. */
  Voxel_signatures m_voxel_signatures;

  /*! Indicates whether the parts are smooth. */
  Boolean m_smooth;
  
private:
  /*! Indicates whether cleaning the color parts is in progress. 
   * Upon invocation of clean_parts(), if this flag is on, the function
   * immediately returns.
   */
  Boolean m_clean_colors_in_progress;

  /*! The id of the first brick in the sequence of children of this group. */
  Uint m_start_brick;

  /*! The number bricks in the sequence of children of this group. */  
  Uint m_num_bricks;
  
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! The array of style names. */
  static const char* s_style_names[];

  /*! The array of style names. */
  static const char* s_color_space_names[];

  /*! The Layer visibility names */
  static const char* s_layer_visibility_names[];
  
  /*! Default values */
  static const std::size_t s_def_even_layer_x;
  static const std::size_t s_def_even_layer_y;
  static const std::size_t s_def_odd_layer_x;
  static const std::size_t s_def_odd_layer_y;
  static const std::size_t s_def_offset_between_rows;
  static const Ego_voxels_tiler::Tiling_rows s_def_tiling_rows_direction;
  static const Float s_def_voxel_width;
  static const Float s_def_voxel_length;
  static const Float s_def_voxel_height;
  static const Style s_def_style;
  static const Boolean s_def_space_filling;
  static const Color_space s_def_color_space;
  static const Layer_visibility s_layer_x_visibility;
  static const Layer_visibility s_layer_y_visibility;
  static const Layer_visibility s_layer_z_visibility;
  static const Boolean s_def_smooth;
};

/* \brief constructs the prototype. */
inline Ego* Ego::prototype() { return new Ego(true); }

/*! \brief clones. */
inline Container* Ego::clone() { return new Ego(); }

/*! \brief determines whether the type of the model is 'Polyhedron'. */
inline Boolean Ego::is_model_polyhedron() const
{  return (boost::get<Shared_polyhedron_geo>(&m_model)); }

/*! \brief determines whether the type of the model is 'Exact_polyhedron'. */
inline Boolean Ego::is_model_exact_polyhedron() const
{ return (boost::get<Shared_exact_polyhedron_geo>(&m_model)); }

/*! \brief determines whether the base type of the model is 'Get_set'. */
inline Boolean Ego::is_model_mesh_set() const
{ return (boost::get<Shared_mesh_set>(&m_model)); }

/*! \brief sets the model. */
inline void Ego::set_model(Shared_polyhedron_geo model) { m_model = model; }
inline void Ego::set_model(Shared_exact_polyhedron_geo model) { m_model = model; }
inline void Ego::set_model(Shared_mesh_set model) { m_model = model; }

/*! \brief obtains the (const) appearance. */
inline const Ego::Shared_appearance Ego::get_appearance() const
{ return m_appearance; }

/*! \brief obtains the (non-const) appearance. */
inline Ego::Shared_appearance Ego::get_appearance() { return m_appearance; }

/*! \brief obtains the style. */
inline Ego::Style Ego::get_style() const { return m_style; }

/*! \brief obtains the color space. */
inline Ego::Color_space Ego::get_color_space() const { return m_color_space; }

/*! \brief determines whether the parts are space filling. */
inline Boolean Ego::is_space_filling() const { return m_space_filling; }

/*! \brief obtains the knob slices number. */
inline Uint Ego::get_knob_slices() const { return m_knob_slices; }

/*! \brief sets the horizontal width of the voxel. */
inline void Ego::set_voxel_width(Float voxel_width)
{ m_voxel_width = voxel_width; }

/*! \brief obtains the horizontal width of the voxel. */
inline Float Ego::get_voxel_width() const { return m_voxel_width; }

/*! \brief sets the horizontal length of the voxel. */
inline void Ego::set_voxel_length(Float voxel_length)
{ m_voxel_length = voxel_length; }

/*! \brief obtains the horizontal length of the voxel. */
inline Float Ego::get_voxel_length() const { return m_voxel_length; }

/*! \brief sets the height of the voxel. */
inline void Ego::set_voxel_height(Float voxel_height)
{ m_voxel_height = voxel_height; }

/*! \brief obtains the height of the voxel. */
inline Float Ego::get_voxel_height() const { return m_voxel_height; }

/*! \brief  */
inline
void Ego::set_even_layer_x(std::size_t x)
{ m_even_layer_x = x; }

/*! \brief  */
inline
void Ego::set_even_layer_y(std::size_t y)
{ m_even_layer_y = y; }

/*! \brief  */
inline
void Ego::set_odd_layer_x(std::size_t x)
{ m_odd_layer_x = x; }

/*! \brief  */
inline
void Ego::set_odd_layer_y(std::size_t y)
{ m_odd_layer_y = y; }

/*! \brief  */
inline void Ego::set_offset_between_rows(std::size_t s)
{ m_offset_between_rows = s; }
  
/*! \brief  */
inline void Ego::set_tiling_rows_direction(Ego_voxels_tiler::Tiling_rows r)
{ m_tiling_rows_direction = r; }

/*! \brief obtains the layer-x index to use in the visibility computation. */
inline Uint Ego::get_layer_x() const {return m_layer_x; }

/*! \brief sets the layer-x index to use in the visibility computation. */
inline void Ego::set_layer_x(Uint layer) { m_layer_x = layer; }

/*! \brief obtains the layer-y index to use in the visibility computation. */
inline Uint Ego::get_layer_y() const {return m_layer_y; }

/*! \brief sets the layer-y index to use in the visibility computation. */
inline void Ego::set_layer_y(Uint layer) { m_layer_y = layer; }

/*! \brief obtains the layer-z index to use in the visibility computation. */
inline Uint Ego::get_layer_z() const {return m_layer_z; }

/*! \brief sets the layer-z index to use in the visibility computation. */
inline void Ego::set_layer_z(Uint layer) { m_layer_z = layer; }

/*! \brief obtains the layer-x visibility scheme. */
inline Ego::Layer_visibility Ego::get_layer_x_visibility() const
{ return m_layer_x_visibility; }

/*! \brief sets the layer-x visibility scheme. */
inline void Ego::set_layer_x_visibility(Layer_visibility layer_visibility)
{ m_layer_x_visibility = layer_visibility; }

/*! \brief obtains the layer-y visibility scheme. */
inline Ego::Layer_visibility Ego::get_layer_y_visibility() const
{ return m_layer_y_visibility; }

/*! \brief sets the layer-y visibility scheme. */
inline void Ego::set_layer_y_visibility(Layer_visibility layer_visibility)
{ m_layer_y_visibility = layer_visibility; }

/*! \brief obtains the layer-z visibility scheme. */
inline Ego::Layer_visibility Ego::get_layer_z_visibility() const
{ return m_layer_z_visibility; }

/*! \brief sets the layer-z visibility scheme. */
inline void Ego::set_layer_z_visibility(Layer_visibility layer_visibility)
{ m_layer_z_visibility = layer_visibility; }
  
/*! \brief obtains the tag (type) of the container */
inline const std::string& Ego::get_tag() const { return s_tag; }

/*! \brief determines whether the parts are smooth. */
inline Boolean Ego::is_smooth() const { return m_smooth; }

SGAL_END_NAMESPACE

#endif
