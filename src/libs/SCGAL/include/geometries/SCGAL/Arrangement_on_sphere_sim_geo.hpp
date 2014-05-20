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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that simulates the construction of an arrangement
 * induced by arcs of great circles embeded on a sphere.
 */

#ifndef SCGAL_ARRANGEMENT_ON_SPHERE_SIM_GEO_HPP
#define SCGAL_ARRANGEMENT_ON_SPHERE_SIM_GEO_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Agent.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Arrangement_on_sphere_base_geo.hpp"
#include "SCGAL/Arrangement_extended_direction.hpp"
#include "SCGAL/Arrangement_on_sphere_labeled.hpp"

SGAL_BEGIN_NAMESPACE

class Field_info;
class Tick_event;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere
 */
class SGAL_SCGAL_DECL Arrangement_on_sphere_sim_geo :
  public Agent,
  public Arrangement_on_sphere_base_geo
{
public:
  enum {
    FIRST = Arrangement_on_sphere_base_geo::LAST - 1,
    TIME,
    RESUME,
    SUSPEND,
    LABEL,
    INCREASE_LABEL,
    VERTEX_LABEL,
    EDGE_LABEL,
    FACE_LABEL,
    INCREASE_VERTEX_LABEL,
    INCREASE_EDGE_LABEL,
    INCREASE_FACE_LABEL,
    LAST
  };

  typedef Aos_labeled_geom_traits               Geom_traits;
  typedef Aos_labeled_topol_traits              Topol_traits;
  typedef Arrangement_on_sphere_labeled         Arrangement_on_surface;

protected:
  typedef Arrangement_on_sphere_labeled         Aos_labeled;
  typedef Aos_labeled::Geometry_traits_2        Geometry_traits;

private:
  /* An observer that labels the vertices, edges, and faces in the order they
   * are created.
   */
  class Label_observer : public CGAL::Arr_observer<Aos_labeled> {
  private:
    //! The running label
    Uint m_label;

  public:
    /*! Empty constructor */
    Label_observer() : CGAL::Arr_observer<Aos_labeled>(), m_label(0) {}

    /*! Notification after the creation of a new vertex.
     * \param v A handle to the created vertex.
     */
    virtual void after_create_vertex(Vertex_handle v)
    {
      // std::cout << "after_create_vertex: " << m_label << std::endl;
      v->set_label(m_label++);
    }

    /*! Notification after the creation of a new vertex at infinity.
     * \param v A handle to the created vertex.
     */
    virtual void after_create_boundary_vertex (Vertex_handle v)
    {
      // std::cout << "after_create_boundary_vertex: " << m_label << std::endl;
      v->set_label(m_label++);
    }

    /*! Notification after the creation of a new edge.
     * \param e A handle to one of the twin halfedges that were created.
     */
    virtual void after_create_edge(Halfedge_handle e)
    {
      // std::cout << "after_create_edge: " << m_label << std::endl;
      e->twin()->set_label(m_label);
      e->set_label(m_label++);
    }
  };

public:
  /*! Constructor */
  Arrangement_on_sphere_sim_geo(Boolean proto = false);

  /*! Destructor */
  virtual ~Arrangement_on_sphere_sim_geo();

  /* Construct the prototype */
  static Arrangement_on_sphere_sim_geo* prototype()
  { return new Arrangement_on_sphere_sim_geo(true); }

  /*! Clone */
  virtual Container* clone()
  { return new Arrangement_on_sphere_sim_geo(); }

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Float* time_handle(const Field_info*) { return &m_time; }
  Boolean* resume_handle(const Field_info*) { return &m_resume; }
  Boolean* suspend_handle(const Field_info*) { return &m_suspend; }
  Boolean* increase_vertex_label_handle(const Field_info*)
  { return &m_increase_vertex_label; }
  Boolean* increase_edge_label_handle(const Field_info*)
  { return &m_increase_edge_label; }
  Boolean* increase_face_label_handle(const Field_info*)
  { return &m_increase_face_label; }
  //@}

  /*! Set the ellpsoid attributes */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the representation */
  virtual void clean();

  /*! */
  virtual void cull(Cull_context& cull_context) {}

  /*! Clear the internal representation and auxiliary data structures */
  virtual void clear();

  /*! Is the representation empty ? */
  virtual Boolean is_empty() const { return m_aos->is_empty(); }

  /*! Obrain the arrangement */
  Aos_labeled* get_aos();

  /*! Set the arrangement */
  void set_aos(Aos_labeled* aos);

  /*! Print statistics */
  void print_stat() { Arrangement_on_surface_geo::print_stat(this); }

  /*! Obtain the the global label */
  Uint get_label() const { return m_label; }

  /*! Set the global label */
  void set_label(Uint label) { m_label = label; }

  /*! Obtain the the vertex label */
  Uint get_vertex_label() const { return m_vertex_label; }

  /*! Obtain the edge label */
  Uint get_edge_label() const { return m_edge_label; }

  /*! Obtain the face label */
  Uint get_face_label() const { return m_face_label; }

  /*! Set the vertex label */
  void set_vertex_label(Uint label) { m_vertex_label = label; }

  /*! Set the edge label */
  void set_edge_label(Uint label) { m_edge_label = label; }

  /*! Set the face label */
  void set_face_label(Uint label) { m_face_label = label; }

protected:
  /*! A function object that renders the vertices with color */
  class Sphere_sim_colored_vertices_renderer :
    public Arrangement_renderer::Renderer
  {
  public:
    /*! The arrangement geometry */
    Arrangement_on_sphere_sim_geo& m_geo;

  public:
    typedef Arrangement_on_sphere_sim_geo               Geometry;

    /*! Constructor */
    Sphere_sim_colored_vertices_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the edges */
    virtual void operator()(Draw_action* action);

  private:
    // Assignment operator cannot be generated.
    Sphere_sim_colored_vertices_renderer&
    operator=(const Sphere_sim_colored_vertices_renderer&);

    // In C++11, the following is supported:
    // Sphere_sim_colored_vertices_renderer&
    //   operator=(const Sphere_sim_colored_vertices_renderer&) = delete;
  };

  /*! A function object that renders the edges with color */
  class Sphere_sim_colored_edges_renderer :
    public Arrangement_renderer::Renderer
  {
  public:
    /*! The arrangement geometry */
    Arrangement_on_sphere_sim_geo& m_geo;

  public:
    typedef Arrangement_on_sphere_sim_geo               Geometry;

    /*! Constructor */
    Sphere_sim_colored_edges_renderer(Geometry& geo) : m_geo(geo) {}

    /*! Render the edges */
    virtual void operator()(Draw_action* action);

  private:
    // Assignment operator cannot be generated.
    Sphere_sim_colored_edges_renderer&
    operator=(const Sphere_sim_colored_edges_renderer&);

    // In C++11, the following is supported:
    // Sphere_sim_colored_edges_renderer&
    //   operator=(const Sphere_sim_colored_edges_renderer&) = delete;
  };

  typedef SGAL::Line_colored_edges_renderer
    <Sphere_sim_colored_edges_renderer>
    Sphere_sim_line_colored_edges_renderer;
  typedef SGAL::Point_colored_vertices_renderer
    <Sphere_sim_colored_vertices_renderer>
    Sphere_sim_point_colored_vertices_renderer;
  typedef SGAL::Ring_colored_vertices_renderer
    <Sphere_sim_colored_vertices_renderer>
    Sphere_sim_ring_colored_vertices_renderer;

  typedef Vertices_renderer<Self>
    Sphere_sim_vertices_renderer;
  typedef Isolated_vertices_renderer<Self>
    Sphere_sim_isolated_vertices_renderer;
  typedef Edges_renderer<Self>
    Sphere_sim_edges_renderer;

  typedef Colored_isolated_vertices_renderer
    <Sphere_sim_isolated_vertices_renderer>
    Sphere_sim_colored_isolated_vertices_renderer;

  typedef Point_colored_isolated_vertices_renderer
    <Sphere_sim_colored_isolated_vertices_renderer>
    Sphere_sim_point_colored_isolated_vertices_renderer;
  typedef Ring_colored_isolated_vertices_renderer
    <Sphere_sim_colored_isolated_vertices_renderer>
    Sphere_sim_ring_colored_isolated_vertices_renderer;

  typedef Inflated_line_edges_renderer<Sphere_sim_edges_renderer>
    Sphere_sim_inflated_line_edges_renderer;
  typedef Inflated_strip_edges_renderer<Sphere_sim_edges_renderer>
    Sphere_sim_inflated_strip_edges_renderer;
  typedef Inflated_tube_edges_renderer<Sphere_sim_edges_renderer>
    Sphere_sim_inflated_tube_edges_renderer;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Indicates whether the aos data structure is owned, i.e., explicitly
   * allocated. If the aos data structure is owned, it should be destructed
   * when this geometry node is destructed.
   */
  Boolean m_owned_aos;

  /*! The arrangement of great-circle arcs on a sphere */
  Aos_labeled* m_aos;

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The container prototype */
  static Container_proto* s_prototype;

  /*! An observer that label the arrangement vertices and edges */
  Label_observer m_observer;

  /*! The simulation time */
  Float m_time;

  /*! Triggers the resume of simulation */
  Boolean m_resume;

  /*! Triggers the suspension of simulation */
  Boolean m_suspend;

  /*! Indicates whether to draw the labeled vertex */
  Boolean m_draw_labeled_vertex;

  /*! Indicates whether to draw the labeled halfedge */
  Boolean m_draw_labeled_edge;

  /*! Indicates whether to draw the labeled face */
  Boolean m_draw_labeled_face;

  /*! The global label */
  Uint m_label;

  /*! The vertex label */
  Uint m_vertex_label;

  /*! The edge label */
  Uint m_edge_label;

  /*! The face label */
  Uint m_face_label;

  /*! The number of labels */
  Uint m_num_labels;

  /*! The color of the labeled vertex */
  Vector3f m_labeled_vertex_color;

  /*! The color of the labeled edge */
  Vector3f m_labeled_edge_color;

  /*! The color of the labeled face */
  Vector3f m_labeled_face_color;

  /*! When trigerred the vertex label is increased */
  Boolean m_increase_vertex_label;

  /*! When trigerred the edge label is increased */
  Boolean m_increase_edge_label;

  /*! When trigerred the face label is increased */
  Boolean m_increase_face_label;

  /*! Default values */
  static const Boolean s_def_draw_labeled_vertex;
  static const Boolean s_def_draw_labeled_edge;
  static const Boolean s_def_draw_labeled_face;
  static const Vector3f s_def_labeled_vertex_color;
  static const Vector3f s_def_labeled_edge_color;
  static const Vector3f s_def_labeled_face_color;

  /*! Draw the arrangement vertices
   * \param action
   */
  virtual void draw_aos_vertices(Draw_action* action);

  /*! Draw the arrangement isolated vertices
   * \param action
   */
  virtual void draw_aos_isolated_vertices(Draw_action* action);

  /*! Draw the arrangement edges
   * \param action
   */
  virtual void draw_aos_edges(Draw_action* action);

  /*! Handle tick events
   * \param event
   */
  virtual void handle(Tick_event* event);

  /*! Print out the name of this agent (for debugging purposes) */
  virtual void identify();

  /*! Process change of simulation time */
  void time_changed(const Field_info* field_info);

  /*! Resume the simulation */
  void resume(const Field_info* field_info);

  /*! Suspend the simulation */
  void suspend(const Field_info* field_info);

  /*! Draw the labeled vertex */
  void draw_labeled_vertex();

  /*! Draw the labeled edge */
  void draw_labeled_edge();

  /*! Draw the labeled face */
  void draw_labeled_face();

  /*! Increase the vertex label
   * \param field_info
   */
  void increase_vertex_label(const Field_info* field_info = NULL);

  /*! Increase the edge label
   * \param field_info
   */
  void increase_edge_label(const Field_info* field_info = NULL);

  /*! Increase the face label
   * \param field_info
   */
  void increase_face_label(const Field_info* field_info = NULL);

  /*! Create the renderers */
  void create_renderers();

  /*! Detsroy the renderers */
  void destroy_renderers();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief draws the arrangement vertices
inline void Arrangement_on_sphere_sim_geo::
draw_aos_vertices(Draw_action* action)
{ my_draw_aos_vertices(m_aos, action); }

//! \brief draws the arrangement isolated vertices
inline void Arrangement_on_sphere_sim_geo::
draw_aos_isolated_vertices(Draw_action* action)
{ my_draw_aos_isolated_vertices(m_aos, action); }

//! \brief draws the arrangement edges
inline void Arrangement_on_sphere_sim_geo::
draw_aos_edges(Draw_action* action)
{ my_draw_aos_edges(m_aos, action); }

SGAL_END_NAMESPACE

#endif
