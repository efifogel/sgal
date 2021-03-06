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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

/*!
 * A node in the scene graph that maintains a polyhedron.
 * The the point of each vertex and the plane of each facet is in exact
 * representation. Each facet is extended with the normal of the facet-plane
 * in Vector3f (inexact) representation. The later is used for rendering.
 */

#ifndef SCGAL_NEF_GAUSSIAN_MAP_GEO_HPP
#define SCGAL_NEF_GAUSSIAN_MAP_GEO_HPP

/*! \file
 */

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/enum.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/HalfedgeDS_face_base.h>

#include "SGAL/basic.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Epec_polyhedron.hpp"
#include "SGAL/Polyhedron_geo_builder.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Nef_gaussian_map.hpp"
#include "SCGAL/gausian_map_to_polyhedron_3.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Isect_action;
class Color_array;
class Draw_action;
class Scene_graph;
class Sphere;

class SGAL_SCGAL_DECL Nef_gaussian_map_geo : public Mesh_set {
public:
  //  typedef Uint *                                        Coord_index_iter;

  typedef CGAL::Tag_true                                Tag_true;
  typedef CGAL::Tag_false                               Tag_false;

  typedef Epec_kernel                                   Kernel;
  typedef Epec_point_3                                  Point_3;
  typedef Epec_plane_3                                  Plane_3;
  typedef Epec_vector_3                                 Vector_3;

  // Inexact types:
  typedef CGAL::Min_sphere_of_spheres_d_traits_3<Inexact_kernel,Inexact_FT>
                                                        Min_sphere_traits;
  typedef CGAL::Min_sphere_of_spheres_d<Min_sphere_traits>
                                                        Min_sphere;
  typedef Min_sphere_traits::Sphere                     Inexact_sphere_3;
  typedef Inexact_kernel::Point_3                       Inexact_point_3;
  typedef Inexact_kernel::Vector_3                      Inexact_vector_3;

  typedef std::vector<Inexact_sphere_3>                 Inexact_sphere_vector;
  typedef Inexact_sphere_vector::const_iterator         Inexact_sphere_iter;

  typedef Epec_polyhedron                               Polyhedron;
  typedef Polyhedron::Facet_iterator                    Facet_iterator;
  typedef Polyhedron::Vertex_iterator                   Vertex_iterator;
  typedef Polyhedron::Point_iterator                    Point_iterator;
  typedef Polyhedron::HalfedgeDS                        HalfedgeDS;
  typedef Polyhedron::Halfedge_around_facet_circulator
    Halfedge_facet_circulator;
  typedef Polyhedron::Facet_const_handle                Facet_const_handle;
  typedef Polyhedron::Facet_handle                      Facet_handle;

  typedef Polyhedron::Vertex                            Vertex;

  // Nef stuff:
  // typedef CGAL::Nef_polyhedron_3<Kernel>                Nef_polyhedron_3;
  // typedef Nef_polyhedron_3::Halffacet_const_iterator    Nef_halffacet_iterator;

  // Nef Gaussian map
  typedef CGAL::Nef_gaussian_map<Kernel>                Nef_gaussian_map;

  typedef std::vector<int>                              Coord_index_vector;
  typedef Coord_index_vector::const_iterator            Coord_index_iter;

  // Shared pointer
  typedef boost::shared_ptr<Nef_gaussian_map_geo>
    Shared_nef_gaussian_map_geo;

  // List of pointers to Nef_gaussian_map_geo objects */
  typedef std::vector<Shared_nef_gaussian_map_geo>      Ngm_node_array;
  typedef Ngm_node_array::iterator                      Ngm_node_iter;

public:
  enum {
    FIRST = Mesh_set::LAST - 1,
    DRAW_DUAL,
    DRAW_DUAL_SPHERE,
    DRAW_DUAL_OPAQUE,
    DRAW_DUAL_HALOED,
    DUAL_LINE_WIDTH,
    DUAL_LINE_COLOR1,
    DUAL_LINE_COLOR2,
    TRUE_DRAW_PRIMAL,
    TRUE_DRAW_DUAL,
    INCREASE_VERTEX_INDEX,
    INCREASE_EDGE_INDEX,
    INCREASE_FACET_INDEX,
    GEOMETRIES,
    LAST
  };

  /*! Constructor. */
  Nef_gaussian_map_geo(Boolean proto = false);

  /*! Destructor. */
  ~Nef_gaussian_map_geo();

  /*! Construct the prototype. */
  static Nef_gaussian_map_geo* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Boolean* draw_dual_handle(const Field_info*) { return &m_draw_dual; }
  Boolean* draw_dual_sphere_handle(const Field_info*)
  { return &m_draw_dual_sphere; }
  Boolean* draw_primal_handle(const Field_info*) { return &m_draw_primal; }
  Boolean* draw_dual_opaque_handle(const Field_info*)
  { return &m_draw_dual_opaque; }
  Boolean* draw_dual_haloed_handle(const Field_info*)
  { return &m_draw_dual_haloed; }
  Boolean* increase_vertex_index_handle(const Field_info*)
  { return &m_increase_vertex_index; }
  Boolean* increase_edge_index_handle(const Field_info*)
  { return &m_increase_edge_index; }
  Boolean* increase_facet_index_handle(const Field_info*)
  { return &m_increase_facet_index; }
  Ngm_node_array* ngm_nodes_handle(const Field_info*) { return &m_ngm_nodes; }
  //@}

  /*! Draw the NGM geometry.
   */
  virtual void draw(Draw_action* action);

  /*! Draw the CGM geometry for selection.
   */
  virtual void isect(Isect_action* action);

  /*! Draw the CGM internal representation.
   */
  virtual void draw_geometry(Draw_action* action);

  /*! Clean the bounding sphere of the Nef Gaussian map. */
  virtual void clean_bounding_sphere();

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  /*! Print statistics. */
  void print_stat();

  /*! \brief sets the flag that indicates whether to draw the primal or dual. */
  void set_draw_dual(Boolean flag) { m_draw_dual = flag; }

  /*! \brief sets the flag that indicates whether to draw haloed lines. */
  void set_draw_dual_haloed(Boolean flag) { m_draw_dual_haloed = flag; }

  /*! \brief obtains the flag that indicates whether to draw haloed lines. */
  Boolean get_draw_dual_haloed() const { return m_draw_dual_haloed; }

  /*! Obtain he Nef_gaussian_map. */
  Nef_gaussian_map& get_ngm();

#if 0
  /*! Stores a point in inexact number type in the vertex. */
  struct Convert_inexact_point {
    void operator()(Vertex& vertex)
    {
      const Point_3& point = vertex.point();
      float x = CGAL::to_double(point.x());
      float y = CGAL::to_double(point.y());
      float z = CGAL::to_double(point.z());
      Inexact_point_3 inexact_point(x, y, z);
      vertex.set_inexact_point(inexact_point);
    }
  };
#endif

#if 0
  /*! Transforms a (planar) facet into a normal. */
  struct Normal_vector {
    template <typename Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      // Facet::Plane_3 is the normal vector type. We assume the
      // CGAL Kernel here and use its global functions.
#if 0
      const Point_3& x = h->vertex()->point();
      const Point_3& y = h->next()->vertex()->point();
      const Point_3& z = h->next()->next()->vertex()->point();
#endif
      Vector_3 normal =
        CGAL::cross_product(h->next()->vertex()->point() - h->vertex()->point(),
                            h->next()->next()->vertex()->point() -
                            h->next()->vertex()->point());
      Number_type sqr_length = normal.squared_length();
      double tmp = CGAL::to_double(sqr_length);
      return normal / CGAL::sqrt(tmp);
    }
  };
#endif

private:
  /*! Transforms a (planar) facet into a plane. */
  struct Plane_equation {
    template <typename Facet>
    typename Facet::Plane_3 operator()(Facet& f) {
      typename Facet::Halfedge_handle h = f.halfedge();
      typedef typename Facet::Plane_3 Plane;
      return Plane( h->vertex()->point(),
                    h->next()->vertex()->point(),
                    h->next()->next()->vertex()->point());
    }
  };

  /*! Extracts the inexact point from a polyhedron vertex. */

  struct Convert_inexact_sphere {
    Inexact_sphere_3 operator()(const Nef_gaussian_map::SVertex& vertex) const
    {
      const Point_3& point = vertex.point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      Inexact_point_3 inexact_point(x,y,z);
      return Inexact_sphere_3(inexact_point, 0.0);
    }

    Inexact_sphere_3 operator()(const Polyhedron::Vertex& vertex) const
    {
      const Point_3& point = vertex.point();
      float x = static_cast<float>(CGAL::to_double(point.x()));
      float y = static_cast<float>(CGAL::to_double(point.y()));
      float z = static_cast<float>(CGAL::to_double(point.z()));
      Inexact_point_3 inexact_point(x,y,z);
      return Inexact_sphere_3(inexact_point, 0.0);
    }
  };

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! Indicates whether the intermediate polyhedron has been built. */
  Boolean m_dirty_polyhedron;

  /*! Indicates whether the ngm data structure has been invalidated. */
  Boolean m_dirty_ngm;

  /*! The resulting polyhedron. */
  Polyhedron m_polyhedron;

  /*! The Nef Gaussian map representation. */
  Nef_gaussian_map m_nef_gaussian_map;

  /* Indicates that the bbox is set externally. */
  bool m_bb_is_pre_set;

  /*! The time is took to compute the minkowski sum in seconds. */
  float m_time;

  /*! Indicates whether to compute the minkowski sum. */
  bool m_minkowski_sum;

  /*! The minkowski sum operands. */
  Ngm_node_array m_ngm_nodes;

  /*! The shape of a marked vertex. */
  Sphere* m_sphere;

  /*! Indicates wether to draw the polyhedron or its dual. */
  Boolean m_draw_dual;

  /*! The opposite of the m_draw_dual flag. */
  Boolean m_draw_primal;

  /*! Indicates whether to draw the cube opaque. */
  Boolean m_draw_dual_opaque;

  /*! Indicates whether to draw haloed lines. */
  Boolean m_draw_dual_haloed;

  Float m_dual_line_width;

  Vector3f m_dual_line_color;

  Vector3f m_dual_sphere_color;

  Float m_delta_angle;

  /*! Indicates whether to draw the sphere. */
  Boolean m_draw_dual_sphere;

  /*! Indicates whether to draw the marked vertex. */
  Boolean m_draw_marked_vertex;

  /*! Indicates whether to draw the marked halfedge. */
  Boolean m_draw_marked_edge;

  /*! Indicates whether to draw the marked face. */
  Boolean m_draw_marked_facet;

  /*! The color of the marked vertex. */
  Vector3f m_marked_vertex_color;

  /*! The color of the marked edge. */
  Vector3f m_marked_edge_color;

  /*! The color of the marked face. */
  Vector3f m_marked_facet_color;

  /*! The scale of the radius of the geometry that represents a marked
   * vertex.
   */
  float m_vertex_radius_scale;

  /*! The scale of the radius of the geometry that represents a marked edge. */
  float m_edge_radius_scale;

  /*! The index of the marked vertex. */
  Uint m_marked_vertex_index;

  /*! The index of the marked edge. */
  Uint m_marked_edge_index;

  /*! The index of the marked face. */
  Uint m_marked_facet_index;

  /*! When trigerred the vertex index is increased. */
  Boolean m_increase_vertex_index;

  /*! When trigerred the edge index is increased. */
  Boolean m_increase_edge_index;

  /*! When trigerred the face index is increased. */
  Boolean m_increase_facet_index;

  // Default values:
  static const Boolean s_def_draw_dual;
  static const Boolean s_def_draw_dual_sphere;
  static const Boolean s_def_draw_dual_opaque;
  static const Boolean s_def_draw_dual_haloed;
  static const Float s_def_dual_line_width;
  static const Vector3f s_def_dual_line_color;
  static const Vector3f s_def_dual_sphere_color;
  static const Float s_def_delta_angle;
  static const Boolean s_def_draw_marked_vertex;
  static const Boolean s_def_draw_marked_edge;
  static const Boolean s_def_draw_marked_facet;
  static const Vector3f s_def_marked_vertex_color;
  static const Vector3f s_def_marked_edge_color;
  static const Vector3f s_def_marked_facet_color;
  static const float s_def_vertex_radius_scale;
  static const float s_def_edge_radius_scale;

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Clean the polyhedron data structure. */
  void clean_ngm();

  /*! Clear the internal representation. */
  void clear_ngm();

  /*! Determines whether the representation has been invalidated. */
  Boolean is_dirty_ngm() const;

  /*! Determine whether the representation is empty. */
  bool is_ngm_empty() const;

  /*! Update the polyhedron data structure. */
  void clean_polyhedron();

  /*! Raise the flag that indicates that the sphere bound changed. */
  void draw_changed(const Field_info* field_info = NULL);

  /*! Draw the intermediate polyhedron (for debugging purpose). */
  void draw_polyhedron(Draw_action* action);

  /*! \brief draws the dual representation of the polyhedron. */
  void draw_dual(Draw_action* action);

  /*! \brief draws the dual edges. */
  void draw_dual_edges();

  /*! \brief draws the dual representation opaque. */
  void draw_dual_opaque(Draw_action* action);

  /*! \brief draws the primal representation of the polyhedron. */
  void draw_primal(Draw_action* action);

  /*! Draw the dual marked facet, that is, the vertex. */
  void draw_dual_marked_facet();

  /*! Draw the dual marked edge. */
  void draw_dual_marked_edge(Vector3f& src, Vector3f& trg);

  /*! Draw the dual marked vertex, that is a face. */
  void draw_dual_marked_vertex();

  /*! Set the operand gausian maps of the minkowski sum. */
  void insert_ngm(Shared_nef_gaussian_map_geo ngm);

  /*! Set the flag that indicates whether to draw the sphere. */
  void set_draw_dual_sphere(Boolean draw_bg);

  /*! Set the width of the lines. */
  void set_dual_line_width(Float width);

  /*! Obtaint the width of the lines. */
  Float get_dual_line_width() const;

  /*! Set the line color. */
  void set_dual_line_color(const Vector3f& color);

  /*! Obtain the line color */
  const Vector3f& get_dual_line_color();

  /*! Set the sphere color. */
  void set_dual_sphere_color(const Vector3f& color);

  /*! Obtain the sphere color. */
  const Vector3f& get_dual_sphere_color() const;

  /*! Increase the vertex index. */
  void increase_vertex_index(const Field_info* field_info = NULL);

  /*! Increase the edge index. */
  void increase_edge_index(const Field_info* field_info = NULL);

  /*! Increase the face index. */
  void increase_facet_index(const Field_info* field_info = NULL);

  /*! Obtain the index of the marked vertex. */
  Uint marked_vertex_index() const { return m_marked_vertex_index; }

  /*! Obtain the index of the marked edge. */
  Uint marked_edge_index() const { return m_marked_edge_index; }

  /*! Obtain the index of the marked facet. */
  Uint marked_facet_index() const { return m_marked_facet_index; }

  /*! Set the index of the marked vertex. */
  void set_marked_vertex_index(Uint index) { m_marked_vertex_index = index; }

  /*! Set the index of the marked edge. */
  void set_marked_edge_index(Uint index) { m_marked_edge_index = index; }

  /*! Set the index of the marked facet. */
  void set_marked_facet_index(Uint index) { m_marked_facet_index = index; }

  /*! Set the color of the marked vertex. */
  void set_marked_vertex_color(const SGAL::Vector3f& color);

  /*! Set the color of the marked edge. */
  void set_marked_edge_color(const SGAL::Vector3f& color);

  /*! Sets the color of the marked facet. */
  void set_marked_facet_color(const SGAL::Vector3f& color);
};

//! \brief constructs the prototype.
inline Nef_gaussian_map_geo* Nef_gaussian_map_geo::prototype()
{ return new Nef_gaussian_map_geo(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Nef_gaussian_map_geo::create()
{ return new Nef_gaussian_map_geo(); }

//! \brief sets the operand gausian maps of the minkowski sum.
inline void Nef_gaussian_map_geo::insert_ngm(Shared_nef_gaussian_map_geo ngm)
{
  m_ngm_nodes.push_back(ngm);
  Observer observer(this, get_field_info(GEOMETRIES));
  ngm->register_observer(observer);
  m_dirty_bounding_sphere = true;
}

//! \brief sets the flag that indicates whether to draw the sphere.
inline void Nef_gaussian_map_geo::set_draw_dual_sphere(Boolean draw_bg)
{ m_draw_dual_sphere = draw_bg; }

//! \brief sets the width of the lines.
inline void Nef_gaussian_map_geo::set_dual_line_width(Float width)
{ m_dual_line_width = width; }

//! \brief obtaints the width of the lines.
inline Float Nef_gaussian_map_geo::get_dual_line_width() const
{ return m_dual_line_width; }

//! \brief sets the curve color.
inline void Nef_gaussian_map_geo::set_dual_line_color(const Vector3f& color)
{ m_dual_line_color = color; }

//! \brief obtains the curve color.
inline const Vector3f& Nef_gaussian_map_geo::get_dual_line_color()
{ return m_dual_line_color; }

//! \brief sets the sphere color.
inline void Nef_gaussian_map_geo::set_dual_sphere_color(const Vector3f& color)
{ m_dual_sphere_color = color; }

//! \brief obtains the sphere color.
inline const Vector3f& Nef_gaussian_map_geo::get_dual_sphere_color() const
{ return m_dual_sphere_color; }

//! \brief sets the color of the marked vertex.
inline void
Nef_gaussian_map_geo::set_marked_vertex_color(const Vector3f& color)
{ m_marked_vertex_color = color; }

//! \brief sets the color of the marked edge.
inline void
Nef_gaussian_map_geo::set_marked_edge_color(const Vector3f& color)
{ m_marked_edge_color = color; }

//! \brief sets the color of the marked facet.
inline void
Nef_gaussian_map_geo::set_marked_facet_color(const Vector3f& color)
{ m_marked_facet_color = color; }

//! \brief determiness whether the representation has been invalidated.
inline Boolean Nef_gaussian_map_geo::is_dirty_ngm() const
{ return m_dirty_ngm; }

//! \brief determines whether the representation is empty.
inline bool Nef_gaussian_map_geo::is_ngm_empty() const
{ return false; }
// { return m_nef_gaussian_map.empty(); }

SGAL_END_NAMESPACE

#endif
