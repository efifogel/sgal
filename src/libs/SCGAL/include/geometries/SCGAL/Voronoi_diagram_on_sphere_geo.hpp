// Copyright (c) 2007 Israel.
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
// $Id: Voronoi_diagram_on_sphere_geo.hpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SGAL_VORONOI_DIAGRAM_ON_SPHERE_GEO_HPP
#define SGAL_VORONOI_DIAGRAM_ON_SPHERE_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>

#include <CGAL/basic.h>
#include <CGAL/envelope_voronoi_2.h>
#include <CGAL/Envelope_voronoi_traits_2/Spherical_voronoi_diagram_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"

#include "SCGAL/Geodesic_voronoi_on_sphere_geo.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

class Coord_array;
class Index_array;
class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;
class Sphere;

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */
class SGAL_CLASSDEF Voronoi_diagram_on_sphere_geo :
  public Geodesic_voronoi_on_sphere_geo
{
public:
  enum {
    FIRST = Geodesic_voronoi_on_sphere_geo::LAST - 1,
    SITE_STYLE,
    SITE_STYLE_ID,
    SITE_RADIUS,
    SITE_POINT_SIZE,
    SITE_DELTA_ANGLE,
    LAST
  };

  typedef Arrangement_renderer::Vertex_shape       Site_shape;
  typedef Site_shape::Style                        Site_style;
  
private:
  typedef CGAL::Spherical_voronoi_diagram_traits_2<Kernel>
    Geom_traits;
  typedef CGAL::Envelope_voronoi_2::Spherical_voronoi_diagram_2<Geom_traits>
    Voronoi_on_sphere;

  typedef Geom_traits::Point_2                     Point;
  typedef Geom_traits::X_monotone_curve_2          X_monotone_curve;
  typedef Geom_traits::Curve_2                     Curve;
  typedef Geom_traits::Site_2                      Site;

  typedef Voronoi_on_sphere                        Vos;
  
  typedef Vos::Vertex                              Vos_vertex;
  typedef Vos::Halfedge                            Vos_halfedge;
  typedef Vos::Face                                Vos_face;

  typedef Vos::Vertex_handle                       Vos_vertex_handle;
  typedef Vos::Halfedge_handle                     Vos_halfedge_handle;
  typedef Vos::Face_handle                         Vos_face_handle;

  typedef Vos::Vertex_const_handle                 Vos_vertex_const_handle;
  typedef Vos::Halfedge_const_handle               Vos_halfedge_const_handle;
  typedef Vos::Face_const_handle                   Vos_face_const_handle;
  
  typedef Vos::Vertex_iterator                     Vos_vertex_iterator;
  typedef Vos::Halfedge_iterator                   Vos_halfedge_iterator;
  typedef Vos::Edge_iterator                       Vos_edge_iterator;
  typedef Vos::Face_iterator                       Vos_face_iterator;

  typedef Vos::Vertex_const_iterator               Vos_vertex_const_iterator;
  typedef Vos::Edge_const_iterator                 Vos_edge_const_iterator;
  typedef Vos::Halfedge_const_iterator             Vos_halfedge_const_iterator;
  typedef Vos::Face_const_iterator                 Vos_face_const_iterator;

  typedef Vos::Halfedge_around_vertex_circulator
    Vos_halfedge_around_vertex_circulator;
  typedef Vos::Halfedge_around_vertex_const_circulator
    Vos_halfedge_around_vertex_const_circulator;

public:
  /*! Constructor. */
  Voronoi_diagram_on_sphere_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Voronoi_diagram_on_sphere_geo();

  /* Construct the prototype. */
  static Voronoi_diagram_on_sphere_geo* prototype()
  { return new Voronoi_diagram_on_sphere_geo(true); }

  /*! Clone. */
  virtual Container* clone()
  { return new Voronoi_diagram_on_sphere_geo(); }

  /*! Initialize the container prototype. */
  virtual void init_prototype();

  /*! Delete the container prototype. */
  virtual void delete_prototype(); 

  /*! Obtain the container prototype. */
  virtual Container_proto* get_prototype();

  /*! Set the node attributes. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! */
  virtual void cull(Cull_context& cull_context);

  /*! Determine whether the representation empty. */
  virtual Boolean is_empty() const { return m_vos->is_empty(); }

  /*! Clean the representation. */
  virtual void clean();

  /*! Clear the internal representation and auxiliary data structures. */
  virtual void clear();

  /*! Clean the renderer. */
  virtual void clean_renderer();

  /*! Indicates whether the vos data structure is owned, i.e., explicitly
   * allocated. If the vos data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_vos;

  /*! Obrain the Voronoi diagram. */
  Voronoi_on_sphere* get_vos() { return m_vos; }

  /*! Obtain the flag that indicates whether to draw the sites. */
  Boolean get_draw_sites() const { return m_draw_sites; }

  /*! Obtain the site shape style. */
  Vertex_style get_site_style() const { return m_site_style; }
  
  /*! Obtain the site point size. */
  Float get_site_point_size() const { return m_site_point_size; }

protected:
  typedef Vertices_renderer<Self>          Voronoi_vertices_renderer;
  typedef Edges_renderer<Self>             Voronoi_edges_renderer;
  
  typedef Colored_vertices_renderer<Voronoi_vertices_renderer>
                                           Voronoi_colored_vertices_renderer;
  typedef Colored_edges_renderer<Voronoi_edges_renderer>
                                           Voronoi_colored_edges_renderer;

  typedef Line_colored_edges_renderer<Voronoi_colored_edges_renderer>
                                           Voronoi_line_colored_edges_renderer;
  typedef Point_colored_vertices_renderer<Voronoi_colored_vertices_renderer>
    Voronoi_point_colored_vertices_renderer;
  typedef Ring_colored_vertices_renderer<Voronoi_colored_vertices_renderer>
    Voronoi_ring_colored_vertices_renderer;

  typedef Inflated_line_edges_renderer<Voronoi_edges_renderer>
    Voronoi_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Voronoi_edges_renderer>
    Voronoi_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Voronoi_edges_renderer>
    Voronoi_inflated_tube_edges_renderer;

  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! A functor that draws the sites. */
  class Site_renderer : public Arrangement_renderer::Renderer {
  private:
    /*! The arrangement geometry. */
    Voronoi_diagram_on_sphere_geo& m_geo;
    
  public:
    /*! Constructor. */
    Site_renderer(Voronoi_diagram_on_sphere_geo& geo) : m_geo(geo) {}
    
    /*! Draw the sites. */
    virtual void operator()(Draw_action* action) { m_geo.draw_sites(action); }
  };

  class Site_other_renderer : public Arrangement_renderer::Renderer {
  private:
    /*! The arrangement geometry. */
    Voronoi_diagram_on_sphere_geo& m_geo;
    
  public:
    /*! Constructor. */
    Site_other_renderer(Voronoi_diagram_on_sphere_geo& geo) : m_geo(geo) {}
    
    /*! Drawer operator. */
    virtual void operator()(Draw_action* action);
  };
  
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! The arrangement of great-circle arcs on a sphere. */
  Voronoi_on_sphere* m_vos;

  /*! The site shape style. */
  Vertex_style m_site_style;

  /*! The site radius. */
  Float m_site_radius;

  /*! The site point size (when drawn as a point). */
  Float m_site_point_size;

  /*! The angle of a single triangle in the fan drawing of a site. */
  Float m_site_delta_angle;

  /*! The site renderer. */
  Site_renderer* m_site_renderer;

  /*! The non-flat site renderer. */
  Site_other_renderer* m_site_other_renderer;
  
  /*! Default values */
  static const Vertex_style s_def_site_style;
  static const Float s_def_site_radius;
  static const Float s_def_site_point_size;
  static const Float s_def_site_delta_angle;
  
  /*! Draw the arrangement on sphere opaque.
   * \param action
   */
  virtual void draw_opaque(Draw_action* action);

  /*! Draw the sites.
   * \param action
   */
  void draw_sites(Draw_action* action);
  
  /*! Draw a site.
   * \param action
   * \param point
   */
  void draw_site(Draw_action* action, Exact_point_3& point);

  /*! Draw the arrangement vertices.
   * \param action
   */
  void draw_aos_vertices(Draw_action* action)
  { my_draw_aos_vertices(m_vos, action); }
  
  /*! Draw the arrangement edges.
   * \param action
   */
  void draw_aos_edges(Draw_action* action)
  { my_draw_aos_edges(m_vos, action); }

  /*! Create the renderers. */
  void create_renderers();

  /*! Detsroy the renderers. */
  void destroy_renderers();
};

SGAL_END_NAMESPACE

#endif
