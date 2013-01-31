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
// $Id: Lower_envelope_geo.hpp 9188 2010-05-25 14:40:57Z efif $
// $Revision: 9188 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_LOWER_ENVELOPE_GEO_HPP
#define SGAL_LOWER_ENVELOPE_GEO_HPP

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure used as a base class to all lower envelope nodes.
 */

#include <CGAL/Cartesian.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>
#include <CGAL/envelope_3.h>

#include <string>
#include <vector>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Geometry.hpp"

#include "SCGAL/Exact_kernel.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Draw_action;
class Isect_action;

/*! A lower-envelope data structure */
class Lower_envelope_geo : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    VERTEX_RADIUS,
    EDGE_RADIUS,
    FACE_TRANSPARENCY,
    LAST
  };

  /*! Constructor */
  Lower_envelope_geo(Boolean proto = SGAL_FALSE);

  /*! Destructor */
  virtual ~Lower_envelope_geo();

  /*! Clone */
  virtual SGAL::Container * clone() = 0;

  /*! Draw the geometry of the lower envelope */
  virtual void draw(Draw_action * action);

  virtual void isect(Isect_action * action) {}

  /*! Determine whether the geometry has a color array. */
  virtual Boolean has_color() const { return m_color_array != NULL; }  
  
  /*! Determine whether the geometry has a textture coordinate array. */
  virtual Boolean has_tex_coord() const { return m_tex_coord_array != NULL; }  

  /*! Calculate the sphere bound of this geometry containter */
  virtual Boolean clean_sphere_bound();

  /*! Set the attributes of this node */
  virtual void set_attributes(SGAL::Element * elem);

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the prototype of this container */
  virtual void delete_prototype();

  /*! Obtain the prototype of this container */
  virtual Container_proto * get_prototype();

  /*! Print statistics */
  void print_stat();

  /*! Obtain the vertex radius */
  Float get_vertex_radius() const { return m_vertex_radius; }

  /*! Set the vertex radius */
  void set_vertex_radius(Float radius) { m_vertex_radius = radius; }

  /*! Obtain the edge radius */
  Float get_edge_radius() const { return m_edge_radius; }

  /*! Set the face radius */
  void set_edge_radius(Float radius) { m_edge_radius = radius; }

  /*! Obtain the face transparency */
  Float get_face_transparency() const { return m_face_transparency; }

  /*! Set the face transparency */
  void set_face_transparency(Float frac) { m_face_transparency = frac; }
  
protected:
  /*! Indicates whether the envelope has been built */
  Boolean m_dirty;

  typedef std::vector<Node *>                           Node_vector;
  typedef Node_vector::iterator                         Node_iter;
  
  /*! The surfaces the envelope is computed for */
  Node_vector m_surfaces;
  
  /*! The time is took to compute the envelope in seconds */
  Float m_time;

  /*! The radius of a diagram vertex */
  float m_vertex_radius;

  /*! The radius of an diagram edge */
  float m_edge_radius;

  /*! The transparency of a diagram face */
  float m_face_transparency;
  
  /*! Clean the representation */
  virtual void clean() { m_dirty = SGAL_FALSE; }

  /*! Clear the internal representation */
  virtual void clear() { m_dirty = SGAL_TRUE; }

  /*! Return true if the envelope is empty */
  virtual Boolean is_empty() const = 0;

  /*! Transform the coordinates of the SGM into spheres
   * \param spheres (o) the transformed coordinates
   */
  virtual void transform_coords(Approximate_sphere_vector & spheres) {}

  /*! Draw the envelope faces */
  virtual void draw_envelope_faces(Draw_action * action) {}

  /*! Draw the envelope edges */
  virtual void draw_envelope_edges(Draw_action * action) {}

  /*! Draw the envelope vertices */
  virtual void draw_envelope_vertices(Draw_action * action) {}

  /*! Determine whether the representation hasn't been cleaned
   * \return true if the representation hasn't been cleaned and false otherwise
   */
  Boolean is_dirty() const { return m_dirty; }
  
  /*! Transform the coordinates of the envelope into spheres
   * \param spheres (o) the transformed coordinates
   */
  template <typename Envelope>
  void transform_coords_impl(Envelope * envelope,
                             Approximate_sphere_vector & spheres)
  {
    if (!envelope || is_empty()) return;
  
    spheres.resize(envelope->number_of_vertices());
    Convert_approximate_sphere<Envelope> convert;
    std::transform(envelope->vertices_begin(), envelope->vertices_end(),
                   spheres.begin(), convert);
  }

private:
  /*! Converts an envelope point to a sphere */
  template <typename Envelope>
  struct Convert_approximate_sphere {
    Approximate_sphere_3
    operator()(const typename Envelope::Vertex & vertex) const
    {
      Vector2f vec = to_vector2f(vertex.point());
      Approximate_point_3 approximate_point(vec[0], vec[1], 0);
      return Approximate_sphere_3(approximate_point, 0);
    }
  };

  /*! The node prototype */
  static Container_proto * s_prototype;

  // Default Values:
  static float s_def_vertex_radius;
  static float s_def_edge_radius;
  static float s_def_face_transparency;
  
  /*! Draw the envelope */
  void draw_envelope(Draw_action * action);

  /*! Add a surfaces to the container of surfaces to process */
  void add_surface(Node * node) { m_surfaces.push_back(node); }
};

SGAL_END_NAMESPACE

#endif
