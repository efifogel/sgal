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

#include "SGAL/basic.hpp"
#include "SGAL/Geometry.hpp"

#include "SCGAL/Polyhedron_geo.hpp"
#include "SCGAL/Exact_polyhedron_geo.hpp"

#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxels.hpp"
#include "SEGO/Ego_voxels_tiler.hpp"

#include <boost/variant.hpp>

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Cull_context;
class Polyhedron_geo;
class Exact_polyhedron_geo;

class SGAL_CLASSDEF Ego_geo : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    MODEL,
    PARTS,
    VOXEL_WIDTH,
    VOXEL_LENGTH,
    VOXEL_HEIGHT,
    FIRST_TILE_PLACEMENT,
    TILING_STRATEGY,
    TILING_ROWS_DIRECTION,
    LAST
  };

  /*! Constructor */
  Ego_geo(Boolean proto = false);

  /*! Destructor */
  virtual ~Ego_geo();

  /* Construct the prototype */
  static Ego_geo* prototype()
  { return new Ego_geo(true); }

  /*! Clone */
  virtual Container* clone() { return new Ego_geo(); }

  /*! Initialize the container prototype */
  virtual void init_prototype();
  

  /*! Delete the container prototype */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! */
  virtual void isect(Isect_action* action);

  /*! Calculate sphere bound of the node */
  virtual Boolean calculate_sphere_bound();

  /*! Determine whether the geometry has color (as opposed to material)
   */
  virtual Boolean has_color() const { return false; }  

  /*! Draw the geometry */
  virtual void draw(Draw_action* action);

  /*! Clear the internal representation and auxiliary data structures
   */
  virtual void clear();

  /*! Determine whether the representation hasn't been updated
   */
  virtual Boolean is_dirty() const { return m_dirty; }
  
  /*! Is the representation empty ?
   */
  virtual Boolean is_empty();

  /*! Get the type of the model.
   * \return true if this is the type of the model.
   */
  bool is_model_polyhedron() const {
    return (boost::get<Polyhedron_geo*> (&m_model) != NULL);
  }

  bool is_model_exact_polyhedron() const {
    return (boost::get<Exact_polyhedron_geo*> (&m_model) != NULL);
  }

  bool is_model_geo_set() const {
    return (boost::get<Geo_set*> (&m_model) != NULL);
  }

  /*! Set the model.
   * \param model the model.
   */
  void set_model(Polyhedron_geo* model) { m_model = model; }
  void set_model(Exact_polyhedron_geo* model) { m_model = model; }
  void set_model(Geo_set* model) { m_model = model; }

  /*! Obtain the model.
   * \return the model.
   */
  const Polyhedron_geo* get_polyhedron_model() const;
  const Exact_polyhedron_geo* get_exact_polyhedron_model() const;
  const Geo_set* get_geo_set_model() const;

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
  
  /*! Set the scale.
   * \param scale the scale.
   */
  void set_scale(Float scale) { m_scale = scale; }

  void set_first_tile_placement(Ego_voxels_tiler::First_tile_placement p) {
    m_first_tile_placement = p;
  }

  void set_tiling_strategy(Ego_voxels_tiler::Strategy s) {
    m_tiling_strategy = s;
  }
  
  void set_tiling_rows_direction(Ego_voxels_tiler::Tiling_rows r) {
    m_tiling_rows_direction = r;
  }

  void tiling_changed(Field_info * field_info = NULL);

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

  std::vector<Ego_brick> m_parts;

  Ego_voxels m_voxels;
  Ego_voxels m_tiled_voxels;
  Exact_polyhedron_geo::Kernel::Point_3 m_tiled_voxels_origin;
  
  /// Enums to control tiling.
  Ego_voxels_tiler::First_tile_placement m_first_tile_placement;
  Ego_voxels_tiler::Strategy m_tiling_strategy;
  Ego_voxels_tiler::Tiling_rows m_tiling_rows_direction;
  
  /*! Indicates whether the data structure must be cleaned */
  Boolean m_dirty;
  Boolean m_voxels_dirty;
  Boolean m_tiling_dirty;

  /*! Scale of the model compared to the ego bricks. */
  Float m_scale;

  /*! These are temporary members used to expedite rendering */
  Ego_brick m_ego_brick;
  Ego_brick m_ego_brick_without_knobs;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! Default values */
  static const Float s_def_scale;
  static const Ego_voxels_tiler::First_tile_placement s_def_first_tile_placement;
  static const Ego_voxels_tiler::Strategy s_def_tiling_strategy;
  static const Ego_voxels_tiler::Tiling_rows s_def_tiling_rows_direction;
  static const Float s_def_voxel_width;
  static const Float s_def_voxel_length;
  static const Float s_def_voxel_height;
};

SGAL_END_NAMESPACE

#endif
