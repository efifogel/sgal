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

#include "SEGO/Ego_brick.hpp"
#include "SEGO/Ego_voxelizer.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/variant.hpp>

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Element;
class Cull_context;
class Exact_polyhedron_geo;

class SGAL_CLASSDEF Ego_geo : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    MODEL,
    PARTS,
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

  /*! */
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
    return (boost::get<Exact_polyhedron_geo*> (&m_model) != NULL);
  }

  bool is_model_geo_set() const {
    return (boost::get<Geo_set*> (&m_model) != NULL);
  }

  /*! Set the model.
   * \param model the model.
   */
  void set_model(Exact_polyhedron_geo* model) { m_model = model; }
  void set_model(Geo_set* model) { m_model = model; }

  /*! Obtain the model.
   * \return the model.
   */
  const Exact_polyhedron_geo* get_polyhedron_model() const;
  const Geo_set* get_geo_set_model() const;

  /*! Set the scale.
   * \param scale the scale.
   */
  void set_scale(Float scale) { m_scale = scale; }

protected:
  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! The segments */
  boost::variant<Exact_polyhedron_geo*, Geo_set*> m_model;

  std::vector<Ego_brick> m_parts;

  Ego_voxels m_voxels;
  
  /*! Indicates whether the data structure must be cleaned */
  Boolean m_dirty;

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
};

SGAL_END_NAMESPACE

#endif
