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

#ifndef SCGAL_LOWER_ENVELOPE_GEO_HPP
#define SCGAL_LOWER_ENVELOPE_GEO_HPP

/*! \file
 * A geometry node in the scene graph that maintains a CGAL::Lower_envelope_3
 * data structure used as a base class to all lower envelope nodes.
 */

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>
#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Geometry.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Min_sphere.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Draw_action;
class Isect_action;
class Node;

/*! A lower-envelope data structure. */
class SGAL_SCGAL_DECL Lower_envelope_geo : public Geometry {
public:
  typedef boost::shared_ptr<Node>               Shared_node;

  enum {
    FIRST = Geometry::LAST - 1,
    VERTEX_RADIUS,
    EDGE_RADIUS,
    FACE_TRANSPARENCY,
    LAST
  };

  /*! Constructor. */
  Lower_envelope_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Lower_envelope_geo();

  /*! Clone. */
  virtual Container* clone() = 0;

  /*! Initialize the node prototype */
  virtual void init_prototype();

  /*! Delete the prototype of this container */
  virtual void delete_prototype();

  /*! Obtain the prototype of this container */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* vertex_radius_handle(const Field_info*) { return &m_vertex_radius; }
  Float* edge_radius_handle(const Field_info*) { return &m_edge_radius; }
  Float* face_transparency_handle(const Field_info*)
  { return &m_face_transparency; }
  //@}

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  /*! Draw the geometry of the lower envelope. */
  virtual void draw(Draw_action* action);

  /* Draw the envelope for selection. */
  virtual void isect(Isect_action* action) {}

  /*! Clean the sphere bound of the lower envelope containter */
  virtual void clean_bounding_sphere();

  /*! Print statistics */
  void print_stat();

  /*! Obtain the vertex radius. */
  Float get_vertex_radius() const;

  /*! Set the vertex radius. */
  void set_vertex_radius(Float radius);

  /*! Obtain the edge radius. */
  Float get_edge_radius() const;

  /*! Set the face radius. */
  void set_edge_radius(Float radius);

  /*! Obtain the face transparency. */
  Float get_face_transparency() const;

  /*! Set the face transparency. */
  void set_face_transparency(Float frac);

protected:
  /*! Indicates whether the envelope is dirty and thus should be cleaned. */
  Boolean m_dirty;

  typedef std::vector<Shared_node>                      Node_vector;
  typedef Node_vector::iterator                         Node_iter;

  /*! The surfaces the envelope is computed for. */
  Node_vector m_surfaces;

  /*! The time is took to compute the envelope in seconds. */
  Float m_time;

  /*! The radius of a diagram vertex. */
  float m_vertex_radius;

  /*! The radius of an diagram edge. */
  float m_edge_radius;

  /*! The transparency of a diagram face. */
  float m_face_transparency;

  /*! Clean the representation. */
  virtual void clean();

  /*! Clear the internal representation. */
  virtual void clear();

  /*! Return true if the envelope is empty. */
  virtual Boolean is_empty() const = 0;

  /*! Transform the coordinates of the SGM into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  virtual void transform_coords(Inexact_sphere_vector& spheres) {}

  /*! Draw the envelope faces. */
  virtual void draw_envelope_faces(Draw_action* action) {}

  /*! Draw the envelope edges. */
  virtual void draw_envelope_edges(Draw_action* action) {}

  /*! Draw the envelope vertices. */
  virtual void draw_envelope_vertices(Draw_action* action) {}

  /*! Determine whether the envelope is dirty and thus needs cleaning.
   * \return true if the representation hasn't been cleaned and false otherwise.
   */
  Boolean is_dirty() const;

  /*! Transform the coordinates of the envelope into spheres.
   * \param spheres (o) the transformed coordinates.
   */
  template <typename Envelope>
  void transform_coords_impl(Envelope* envelope,
                             Inexact_sphere_vector& spheres)
  {
    if (!envelope || is_empty()) return;

    spheres.resize(envelope->number_of_vertices());
    Convert_approximate_sphere<Envelope> convert;
    std::transform(envelope->vertices_begin(), envelope->vertices_end(),
                   spheres.begin(), convert);
  }

private:
  /*! Converts an envelope point to a sphere. */
  template <typename Envelope>
  struct Convert_approximate_sphere {
    Inexact_sphere_3
    operator()(const typename Envelope::Vertex& vertex) const
    {
      Vector2f vec = to_vector2f(vertex.point());
      Inexact_point_3 approximate_point(vec[0], vec[1], 0.0f);
      return Inexact_sphere_3(approximate_point, 0.0f);
    }
  };

  /*! The node prototype */
  static Container_proto* s_prototype;

  // Default Values:
  static float s_def_vertex_radius;
  static float s_def_edge_radius;
  static float s_def_face_transparency;

  /*! Draw the envelope. */
  void draw_envelope(Draw_action* action);

  /*! Add a surfaces to the container of surfaces to process. */
  void add_surface(Shared_node node) { m_surfaces.push_back(node); }
};

//! \brief obtains the vertex radius.
inline Float Lower_envelope_geo::get_vertex_radius() const
{ return m_vertex_radius; }

//! \brief sets the vertex radius.
inline void Lower_envelope_geo::set_vertex_radius(Float radius)
{ m_vertex_radius = radius; }

//! \brief obtains the edge radius.
inline Float Lower_envelope_geo::get_edge_radius() const
{ return m_edge_radius; }

//! \brief sets the face radius.
inline void Lower_envelope_geo::set_edge_radius(Float radius)
{ m_edge_radius = radius; }

//! \brief obtains the face transparency.
inline Float Lower_envelope_geo::get_face_transparency() const
{ return m_face_transparency; }

//! \brief sets the face transparency.
inline void Lower_envelope_geo::set_face_transparency(Float frac)
{ m_face_transparency = frac; }

//! \brief cleans the representation.
inline void Lower_envelope_geo::clean() { m_dirty = false; }

//! \brief clears the internal representation.
inline void Lower_envelope_geo::clear() { m_dirty = true; }

//! \brief determines whether the envelope is dirty and thus needs cleaning.
inline Boolean Lower_envelope_geo::is_dirty() const { return m_dirty; }

SGAL_END_NAMESPACE

#endif
