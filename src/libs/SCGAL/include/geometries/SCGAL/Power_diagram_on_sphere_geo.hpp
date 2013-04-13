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
// $Id: Power_diagram_on_sphere_geo.hpp 7729 2009-06-18 08:47:21Z efif $
// $Revision: 7729 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */

#ifndef SGAL_POWER_DIAGRAM_ON_SPHERE_GEO_HPP
#define SGAL_POWER_DIAGRAM_ON_SPHERE_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>

#include <CGAL/basic.h>
#include <CGAL/envelope_voronoi_2.h>
#include <CGAL/Envelope_voronoi_traits_2/Spherical_power_diagram_traits_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"

#include "SCGAL/Geodesic_voronoi_on_sphere_geo.hpp"
#include "SCGAL/Exact_plane_array.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_renderer.hpp"

SGAL_BEGIN_NAMESPACE

class Index_array;
class Draw_action;
class Isect_action;
class Cull_context;
class Container_proto;
class Scene_graph;
class Element;
class Sphere;

/*! A geometry container that represents a power diagram os a sphere.
 */
class SGAL_CLASSDEF Power_diagram_on_sphere_geo :
  public Geodesic_voronoi_on_sphere_geo
{
public:
  enum {
    FIRST = Geodesic_voronoi_on_sphere_geo::LAST - 1,
    PLANE_ARRAY,
    SITE_ENABLED,
    SITE_STYLE_ID,
    SITE_COUNT_ID,
    SITE_DIRECTED,
    SITE_RADIUS,
    SITE_LINE_WIDTH,
    SITE_DELTA_ANGLE,
    LAST
  };

  typedef Arrangement_renderer::Edge_shape      Site_shape;
  typedef Site_shape::Style                     Site_style;
  
private:
  typedef Exact_plane_array                        Coeff_array;
  
  typedef CGAL::Spherical_power_diagram_traits_2<Kernel>
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

  typedef boost::shared_ptr<Coeff_array>           Shared_coeff_array;
  typedef boost::shared_ptr<Exact_plane_array>     Shared_exact_plane_array;
  
public:
  /*! Constructor. */
  Power_diagram_on_sphere_geo(Boolean proto = false);

  /*! Destructor. */
  virtual ~Power_diagram_on_sphere_geo();

  /* Construct the prototype. */
  static Power_diagram_on_sphere_geo* prototype()
  { return new Power_diagram_on_sphere_geo(true); }

  /*! Clone. */
  virtual Container* clone()
  { return new Power_diagram_on_sphere_geo(); }

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
  
  /*! Set the coordinate array. */
  void set_coeff_array(Shared_coeff_array coeff_array);

  /*! Obtain the coordinate array. */
  Shared_coeff_array get_coeff_array() const;

  /*! Obrain the power diagram. */
  Voronoi_on_sphere* get_vos() { return m_vos; }

  /*! Obtain the flag that indicates whether to draw the sites. */
  Boolean get_draw_sites() const { return m_draw_sites; }

  /*! Enable edge rendering. */
  void enable_site() { m_site_enabled = true; }

  /*! Disable edge rendering. */
  void disable_site() { m_site_enabled = false; }

  /*! Determine whether edge rendering is enabled. */
  Boolean is_site_enabled() const { return m_site_enabled; }

  /*! Obtain the edge shape style. */
  Site_style get_site_style() const { return m_site_style; }

  /*! Set the edge shape style. */
  void set_site_style(Site_style style) { m_site_style = style; }
  
  /*! Obtain the edge shape type. */
  Int get_site_count() const { return m_site_count; }

  /*! Set the edge shape type. */
  void set_site_count(Int count) { m_site_count = count; }
    
  /*! Determine whether edges are rendered directed. */
  Boolean get_site_directed() const { return m_site_directed; }

  /*! Set the flag that determines whether edges are rendered directed. */
  void set_site_directed(Boolean dir) { m_site_directed = dir; }

  /*! Obtain the edge radius. */
  Float get_site_radius() const { return m_site_radius; }

  /*! Set the edge radius. */
  void set_site_radius(Float radius) { m_site_radius = radius; }

  /*! Obtain the edge line width. */
  Float get_site_line_width() const { return m_site_line_width; }

  /*! Set the edge line width. */
  void set_site_line_width(Float width) { m_site_line_width = width; }
  
protected:
  typedef Vertices_renderer<Self>          Pd_vertices_renderer;
  typedef Edges_renderer<Self>             Pd_edges_renderer;
  
  typedef Colored_vertices_renderer<Pd_vertices_renderer>
                                           Pd_colored_vertices_renderer;
  typedef Colored_edges_renderer<Pd_edges_renderer>
                                           Pd_colored_edges_renderer;

  typedef Line_colored_edges_renderer<Pd_colored_edges_renderer>
                                           Pd_line_colored_edges_renderer;
  typedef Point_colored_vertices_renderer<Pd_colored_vertices_renderer>
    Pd_point_colored_vertices_renderer;
  typedef Ring_colored_vertices_renderer<Pd_colored_vertices_renderer>
    Pd_ring_colored_vertices_renderer;

  typedef Inflated_line_edges_renderer<Pd_edges_renderer>
    Pd_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Pd_edges_renderer>
    Pd_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Pd_edges_renderer>
    Pd_inflated_tube_edges_renderer;

  class Inflated_site_renderer : public Arrangement_renderer::Renderer {
  private:
    /*! The arrangement geometry. */
    Power_diagram_on_sphere_geo& m_geo;
    
  public:
    /*! Constructor. */
    Inflated_site_renderer(Power_diagram_on_sphere_geo& geo) : m_geo(geo) {}
    
    /*! Drawer operator. */
    virtual void operator()(Draw_action* action);
  };

  /*! A functor that draws the sites. */
  class Site_renderer : public Arrangement_renderer::Renderer {
  private:
    /*! The arrangement geometry. */
    Power_diagram_on_sphere_geo& m_geo;
    
  public:
    /*! Constructor. */
    Site_renderer(Power_diagram_on_sphere_geo& geo) : m_geo(geo) {}
    
    /*! Draw the sites. */
    virtual void operator()(Draw_action* action) { m_geo.draw_sites(action); }
  };

  class Site_other_renderer : public Arrangement_renderer::Renderer {
  private:
    /*! The arrangement geometry. */
    Power_diagram_on_sphere_geo& m_geo;
    
  public:
    /*! Constructor. */
    Site_other_renderer(Power_diagram_on_sphere_geo& geo) : m_geo(geo) {}
    
    /*! Drawer operator. */
    virtual void operator()(Draw_action* action);
  };
    
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! Indicates whether the vos data structure is owned, i.e., explicitly
   * allocated. If the vos data structure is owned, it should be destructed.
   * when this geometry node is destructed.
   */
  Boolean m_owned_vos;

  /*! The arrangement of great-circle arcs on a sphere. */
  Voronoi_on_sphere* m_vos;

  /*! An array of direction ccordinates. */
  Shared_coeff_array m_coeff_array;

  /*! Indicates whether the rendering of edges is enabled or not. */
  Boolean m_site_enabled;

  /*! The edge rendering style. */
  Site_style m_site_style;

  /*! The edge rendering type. */
  Int m_site_count;

  /*! Indicates whether edges are rendered directed or not. */
  Boolean m_site_directed;
  
  /*! The site radius. */
  Float m_site_radius;

  /*! The site line width (when drawn as a line) */
  Float m_site_line_width;

  /*! The angle of a single triangle in the fan drawing of a site. */
  Float m_site_delta_angle;
  
  /*! The site renderer. */
  Inflated_site_renderer* m_inflated_site_renderer;

  /*! The site renderer. */
  Site_renderer* m_site_renderer;
  
  /*! The non-flat site renderer. */
  Site_other_renderer* m_site_other_renderer;
  
  /*! Default values. */
  static const Boolean s_def_site_enabled;
  static const Site_style s_def_site_style;
  static const Int s_def_site_count;
  static const Boolean s_def_site_directed;
  static const Float s_def_site_radius;
  static const Float s_def_site_line_width;
  static const Float s_def_site_delta_angle;
  
  /*! Draw the power diagram on sphere opaque.
   * \param action
   */
  virtual void draw_opaque(Draw_action* action);

  /*! Draw the sites.
   * \param action
   */
  void draw_sites(Draw_action* action);
  
  /*! Draw a site
   * \param action
   * \param point
   */
  void draw_site(Draw_action* action, Exact_plane_3& plane);

  /*! Draw the arrangement vertices.
   * \param action
   */
  void draw_aos_vertices(Draw_action* action);
  
  /*! Draw the arrangement edges.
   * \param action
   */
  void draw_aos_edges(Draw_action* action);

  /*! Create the renderers. */
  void create_renderers();

  /*! Detsroy the renderers. */
  void destroy_renderers();
};

/*! \brief obtains the coordinate array. */
inline Power_diagram_on_sphere_geo::Shared_coeff_array
Power_diagram_on_sphere_geo::get_coeff_array() const { return m_coeff_array; }

SGAL_END_NAMESPACE

#endif
