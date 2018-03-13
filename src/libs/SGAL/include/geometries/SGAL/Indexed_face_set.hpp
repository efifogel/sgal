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

#ifndef SGAL_INDEXED_FACE_SET_HPP
#define SGAL_INDEXED_FACE_SET_HPP

#include <boost/variant.hpp>
#include <boost/type_traits.hpp>

#include <CGAL/basic.h>

#include "SGAL/basic.hpp"
#include "SGAL/Boundary_set.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Epic_kernel.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Inexact_polyhedron.hpp"
#include "SGAL/Epic_polyhedron.hpp"
#include "SGAL/Epec_polyhedron.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Draw_action;
class Isect_action;
class Formatter;
class Scene_graph;
class Configuration;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Indexed_face_set Indexed_face_set.hpp
 * Boundary_set is a geometry node that represnts a mesh made of facets
 * with some additional attributes, efficient OpenGL rendering capabilities,
 * the generation of normals based on the angle neighboring facets, and a CGAL
 * polyhedral surface. The generation of normals utiles the polyhedral surface.
 */
class SGAL_SGAL_DECL Indexed_face_set : public Boundary_set {
public:
  enum {
    FIRST = Boundary_set::LAST - 1,
    VOLUME,
    SURFACE_AREA,
    LAST
  };

  enum Polyhedron_type {
    POLYHEDRON_INEXACT = 0,
    POLYHEDRON_EPIC,
    POLYHEDRON_EPEC
  };

  typedef boost::variant<Inexact_polyhedron, Epic_polyhedron, Epec_polyhedron>
                                                          Polyhedron;

  // typedef Inexact_kernel                               Kernel;
  // typedef Inexact_polyhedron                           Polyhedron;

  /*! Constructor */
  Indexed_face_set(Boolean proto = false);

  /*! Destructor */
  virtual ~Indexed_face_set();

  /* Construct the prototype. */
  static Indexed_face_set* prototype();

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  /*! Clone the container (virtual constructor) with deep copy.
   * \return a clone of this container.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  Float* volume_handle(const Field_info*) { return &m_volume; }
  Float* surface_area_handle(const Field_info*) { return &m_surface_area; }
  Polyhedron_type* polyhedron_type_handle(const Field_info*)
  { return &m_polyhedron_type; }
  //@}

  /*! Sets the attributes of this node extracted from the VRML or X3D file.
   * \param elem contains lists of attribute names and values
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Add the container to a given scene.
   * \param[in] scene_graph the given scene.
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Draw the polygons.
   */
  virtual void draw(Draw_action* action);

  /*! Export this container.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
  virtual void write(Formatter* formatter);

  /*! Export a field of this container.
   * \param[in] field_info The field information record.
   * \param[in] formatter The formatter to use for exporting, e.g., VRML.
   */
   virtual void write_field(const Field_info* field_info, Formatter* formatter);

  /*! Draw the polygons for selection.
   */
  virtual void isect(Isect_action* action);

  /*! Clean the sphere bound.
   */
  virtual void clean_bounding_sphere();

  /*! Obtain the coordinate array.
   * \return the coordinate array.
   */
  virtual Shared_coord_array get_coord_array();

  /*! Obtain the representation mode. */
  virtual Primitive_type get_primitive_type();

  /*! Obtain the number of primitives. */
  virtual Size get_num_primitives();

  /*! Clean the coordinate array. */
  virtual void clean_coords();

   /*! Clean (validate) the facet coordinate indices.
   * If the polyhedron got cleaned, extract the indices from the polyhedron;
   * otherwise, call the inherited member function.
   */
  virtual void clean_facet_coord_indices();

  /*! Repair the data structures, either the coordinates and the coordinate
   * indices, or the polyhedron.
   */
  void repair();

  /*! Repair the normals.
   */
  void repair_normals();

  /// \name Indices Change Reactors
  //@{
  /*! Respond to a change in the coordinate-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void coord_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the normal-index array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void normal_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the color-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void color_indices_changed(const Field_info* field_info);

  /*! Respond to a change in the texture-coordinate index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void tex_coord_indices_changed(const Field_info* field_info);
  //@}

  /// \name Facet Indices Change Reactors
  //@{
  /*! Respond to a change in the facet coordinate-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_coord_indices_changed();

  /*! Respond to a change in the facet normal-index array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void facet_normal_indices_changed();

  /*! Respond to a change in the facet color-index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_color_indices_changed();

  /*! Respond to a change in the facet texture-coordinate index array.
   * \param[in] field_info the information record of the field that caused
   *                       the change.
   */
  virtual void facet_tex_coord_indices_changed();
  //@}

  /*! Clear the coordinate array.
   */
  void clear_coord_array();

  /*! Determine whether the coordinate array has been invalidated, and thus
   * must be cleaned.
   */
  Boolean is_dirty_coord_array() const;

  /*! Clean the normal array and the normal indices.
   * If the creaseAngle field is greater than 0, a normal is calculated per
   * vertes. Otherwise a normal is calculated per polygon.
   */
  virtual void clean_normals();

  /*! Clean the polyhedron.
   */
  void clean_polyhedron();

  /*! Clean the repaired coordinates and the coordinate indices.
   */
  void clean_repaired_coords();

  /*! Clean the polyhedron without applying deformations (such as repairing)
   * that may require re-cleaning.
   */
  void clean_repaired_polyhedron();

  /*! Clear the polyhedron.
   */
  void clear_polyhedron();

  /*! Determine whether the polyhedron has been invalidated, and thus been
   * must be cleaned.
   */
  Boolean is_dirty_polyhedron() const;

  /*! Set the polyhedron data-structure.
   * \param[in] polyhedron the polyhedron data-structure.
   */
  void set_polyhedron(Polyhedron& polyhedron);

  /*! Obtain the polyhedron data-structure.
   * \param[in] with_planes indicates whether to clean the planes (as well
   *            as the polyhedron itself.
   * \return the polyhedron data-structure.
   */
  const Polyhedron& get_polyhedron(Boolean clean_facet_normals = false);

  /*! Determine whether the polyhedron representation is empty.
   */
  bool is_polyhedron_empty();

  /*! Obtain the number of connected components.
   */
  Size get_number_of_connected_components();

  /*! Determine whether the polyhedron is closed.
   */
  Boolean is_closed();

  /*! Obtain the number of border edges.
   */
  size_t get_number_of_border_edges();

  /*! Obtain the number of vertices.
   */
  Size get_number_of_vertices();

  /*! Obtain the number of edges.
   */
  Size get_number_of_edges();

  /*! Obtain the number of facets.
   */
  Size get_number_of_facets();

  /*! Clear (invalidate) the normal attributes m_creased and m_smooth.
   */
  void clear_normal_attributes();

  /*! Clear the polyhedron facets. (Invalidate their attributes.)
   */
  void clear_polyhedron_facet_normals();

  /// \name Change Recators
  //@{
  /*! Respond to a change in the coordinate array.
   * \param[in] field_info the information record of the field that caused
   *            the change.
   */
  virtual void coord_content_changed(const Field_info* field_info);
  //@}

  /*! Respond to a change in the polyhedron.
   */
  virtual void polyhedron_changed();

  /*! Calculate multiple normals per vertex for all vertices.
   * If the angle between the geometric normals of two adjacent faces is less
   * than the crease angle, calculate the normals so that the facets are
   * smooth-shaded across the edge. Otherwise, calculate the normals so that
   * the facets are faceted.
   */
  void calculate_multiple_normals_per_vertex();

  /*! Set the polyhedron type.
   */
  void set_polyhedron_type(Polyhedron_type type);

  /*! Obtain the polyhedron type.
   */
  Polyhedron_type get_polyhedron_type() const;

  /*! Determine whether to compute the convex hull of the coordinate set.
   */
  Boolean is_convex_hull() const;

  /*! Set the flag that indicates whether to compute the convex hull
   * of the coordinate set.
   */
  void set_convex_hull(Boolean flag);

  /*! Compute the orientation of a point relative to the polyhedron.
   */
  CGAL::Oriented_side oriented_side(const Epec_point_3& p);

  /*! Compute the volume of the convex hull of the polyhedron.
   */
  Float volume_of_convex_hull();

  /*! Compute the volume of the polyhedron.
   */
  Float volume();

  /*! Clear (invalidate) the volume.
   */
  void clear_volume();

  /*! Compute the surface area of the polyhedron.
   */
  Float surface_area();

  /*! Clear (invalidate) the surface_area.
   */
  void clear_surface_area();

  /*! Set the flag that determines wheather the mesh is repaired.
   * \param[in] flag indicates wheather the mesh is repaired.
   */
  void set_repaired(Boolean flag);

  /*! Determine wheather the mesh is repaired.
   * \return true if the the mesh is repaired and false otherwise.
   */
  Boolean is_repaired();

  /*! Set the flag that determines wheather the mesh is consistent.
   * \param[in] flag indicates wheather the mesh is consistent.
   */
  void set_consistent(Boolean flag);

  /*! Determine wheather the mesh is consistent.
   * \return true if the the mesh is consistent and false otherwise.
   */
  Boolean is_consistent();

  /*! Set the flag that determine whether the mesh has singular vertices.
   * \param[in] flag indicates whether the mesh has singular vertices.
   */
  Boolean set_has_singular_vertices(Boolean flag);

  /*! Print statistics.
   */
  void print_stat();

  /*! Determine whether the mesh has singular vertices.
    * \return true if the the mesh has singular vertices and false otherwise.
   */
  Boolean has_singular_vertices();

  /*! Set the flag that indicates whether to enable consistency repairing.
   * \param[in] indicates whether to enable consistency repairing.
   */
  void set_make_consistent(Boolean flag);

  /*! Set the flag that indicates whether to enable repairing procedures.
   * \param[in] triangulate_holes indicates whether to triangulate holes.
   * \param[in] repair_orientation indicates whether to repair orientation.
   */
  void set_repair(Boolean triangulate_holes, Boolean repair_orientation);

  /// \name Obtain empty polyhedrons
  //@{
  /*! Obtain the empty polyhedron. */
  Polyhedron& get_empty_polyhedron();

  /*! Obtain the empty inexact polyhedron. */
  Inexact_polyhedron& get_empty_inexact_polyhedron();

  /*! Obtain the empty epic  polyhedron. */
  Epic_polyhedron& get_empty_epic_polyhedron();

  /*! Obtain the empty epec  polyhedron. */
  Epec_polyhedron& get_empty_epec_polyhedron();
  //@}

protected:
  /*! Indicates whether to compute the convex hull. */
  Boolean m_convex_hull;

  /*! The volume of the polyhedron. */
  Float m_volume;

  /*! The surface area of the polyhedron. */
  Float m_surface_area;

  /*! The type of the polyhedrlal surface. */
  Polyhedron_type m_polyhedron_type;

  /*! Indicates wheather the mesh is consistent.
   * \return true if the mesh is consistent and false otherwise. An mesh is
   * inconsistent iff the construction of the polyderal surface failed.
   */
  Boolean m_consistent;

  /*! Indicates wheather the mesh has_singular vertices. */
  Boolean m_has_singular_vertices;

  //*! Indicates whether the geometry is reapired. */
  Boolean m_repaired;

  //*! Indicates whether the normals are reapired. */
  Boolean m_normals_repaired;

  /*! Indicates whether the volume is dirty and thus must be cleaned. */
  Boolean m_dirty_volume;

  /*! Indicates whether the surface area is dirty and thus must be cleaned. */
  Boolean m_dirty_surface_area;

  /*! Indicates whether the coordinate array has beeen invalidated. */
  Boolean m_dirty_coord_array;

  /*! The actual polyhedron object. */
  Polyhedron m_polyhedron;

  /*! Indicates whether the polyhedron has been invalidated, and thus must be
   * cleaned.
   */
  Boolean m_dirty_polyhedron;

  /*! Indicates whether the polyhedron facets are dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_polyhedron_facet_normals;

  /*! Indicates whether the normal attributes are dirty and thus should be
   * cleaned.
   */
  Boolean m_dirty_normal_attributes;

  /*! Obtain the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Obtain the ith 3D coordinate.
   */
  virtual const Vector3f& get_coord_3d(Uint i) const;

  /*! Clean the facet coordinate indices.
   */
  void clean_facet_coord_indices_from_polyhedron();

  /*! Clean the polyhedron facets. (Compute their attributes.)
   */
  void clean_polyhedron_facet_normals();

  /*! Clean (recompute) the normal attributes m_creased and m_smooth.
   */
  void clean_normal_attributes();

  /*! Clean (compute) the volume.
   */
  void clean_volume();

  /*! Clean (compute) the surface area.
   */
  void clean_surface_area();

  /*! Determine whether the angle between two given vectors is smooth.
   */
  Boolean is_smooth(const Vector3f& normal1, const Vector3f& normal2) const;

  /*! Initialize the polyhedron.
   */
  void init_polyhedron();

  /*! Is closed polyhedron visitor. */
  class Is_closed_polyhedron_visitor : public boost::static_visitor<Boolean> {
  public:
    template <typename Polyhedron_>
    Boolean operator()(const Polyhedron_& polyhedron) const
    { return polyhedron.is_closed(); }
  };

  /*! Polyhedron type visitor. */
  class Type_polyhedron_visitor : public boost::static_visitor<Primitive_type> {
  public:
    template <typename Polyhedron_>
    Primitive_type operator()(const Polyhedron_& polyhedron) const
    {
      Boolean triangles(true);
      Boolean quads(true);
      auto fit = polyhedron.facets_begin();
      for (; fit != polyhedron.facets_end(); ++fit) {
        auto hh = fit->facet_begin();
        size_t circ_size = CGAL::circulator_size(hh);
        if (circ_size != 3) triangles = false;
        if (circ_size != 4) quads = false;
      }
      SGAL_assertion(triangles && quads);

      return (triangles) ? PT_TRIANGLES : (quads) ? PT_QUADS : PT_POLYGONS;
    }
  };

  /*! Clean Polyhedron facets visitor. */
  class Clean_facet_normals_visitor : public boost::static_visitor<> {
  public:
    template <typename Polyhedron_>
    void operator()(Polyhedron_& polyhedron) const
    {
      // Compute the plane equations:
      std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                     polyhedron.planes_begin(), Plane_equation());

      // Compute the normal used only for drawing the polyhedron
      std::for_each(polyhedron.facets_begin(), polyhedron.facets_end(),
                    Plane_to_normal());

      // // Convert the exact points to approximate used for drawing the polyhedron
      // std::for_each(polyhedron.vertices_begin(), polyhedron.vertices_end(),
      //               Point_to_vector());
    }

    void operator()(Inexact_polyhedron& polyhedron) const
    {
      std::transform(polyhedron.facets_begin(), polyhedron.facets_end(),
                     polyhedron.planes_begin(), Facet_normal_calculator());
    }

  private:
    /*! A functor that calculates the normal of a given facet. */
    struct Facet_normal_calculator {
      template <typename Facet>
      typename Facet::Plane_3 operator()(Facet& f) {
        typename Facet::Halfedge_handle h = f.halfedge();
        // Facet::Plane_3 is the normal vector type. We assume the
        // CGAL Kernel here and use its global functions.
        Inexact_vector_3 normal =
          CGAL::cross_product(h->next()->vertex()->point() -
                              h->vertex()->point(),
                              h->next()->next()->vertex()->point() -
                              h->next()->vertex()->point());
        return normal / CGAL::sqrt(normal.squared_length());
      }
    };

    /*! Transform a (planar) facet into a plane. */
    struct Plane_equation {
      template <typename Facet>
      typename Facet::Plane_3 operator()(Facet& f) {
        typename Facet::Halfedge_handle h = f.halfedge();
        typedef typename Facet::Plane_3 Plane;
        return Plane(h->vertex()->point(),
                     h->next()->vertex()->point(),
                     h->next()->next()->vertex()->point());
      }
    };

    /*! Convert Plane_3 to normal in Vector3f representation. */
    struct Plane_to_normal {
      template <typename Facet>
      void operator()(Facet& facet) {
        auto normal = to_vector3f(facet.plane().orthogonal_vector());
        normal.normalize();
        facet.set_normal(normal);
      }
    };
  };

  /*! Clean normal attributes visitor. */
  class Clean_normal_attributes_visitor :
    public boost::static_visitor<std::pair<Boolean, Boolean> > {
  private:
    Float m_crease_angle;

  public:
    Clean_normal_attributes_visitor(Float crease_angle) :
      m_crease_angle(crease_angle)
    {}

    std::pair<Boolean, Boolean> operator()(Inexact_polyhedron& polyhedron) const
    { return clean_edges<Inexact_kernel>(polyhedron); }

    std::pair<Boolean, Boolean> operator()(Epic_polyhedron& polyhedron) const
    { return clean_edges<Epic_kernel>(polyhedron); }

    std::pair<Boolean, Boolean> operator()(Epec_polyhedron& polyhedron) const
    { return clean_edges<Epec_kernel>(polyhedron); }

  private:
    template <typename Kernel_, typename Polyhedron_>
    std::pair<Boolean, Boolean> clean_edges(Polyhedron_& polyhedron) const
    {
      typedef Kernel_                                   Kernel;
      typedef Polyhedron_                               Polyhedron;
      typedef typename Polyhedron::Halfedge             Halfedge;

      typedef boost::is_same<typename Polyhedron::Plane_3,
                             typename Kernel::Vector_3> Has_vector;

      Boolean creased(true);
      Boolean smooth(true);

      std::for_each(polyhedron.edges_begin(), polyhedron.edges_end(),
                    [&](Halfedge& edge)
                    {
                      if (edge.is_border_edge()) return;
                      Float angle = compute_angle(edge, Has_vector());
                      if (abs(angle) > m_crease_angle) {
                        edge.set_creased(true);
                        edge.opposite()->set_creased(true);
                        smooth = false;
                        return;
                      }
                      edge.set_creased(false);
                      edge.opposite()->set_creased(false);
                      creased = false;
                    });
      return std::make_pair(smooth, creased);
    }

    template <typename Halfedge_>
    Float compute_angle(Halfedge_& edge, boost::true_type) const
    {
      const auto& normal1 = edge.facet()->plane();
      const auto& normal2 = edge.opposite()->facet()->plane();
      return arccosf(normal1 * normal2);  // inner product
    }

    template <typename Halfedge_>
    Float compute_angle(Halfedge_& edge, boost::false_type) const
    {
      const auto& normal1 = edge.facet()->get_normal();
      const auto& normal2 = edge.opposite()->facet()->get_normal();
      return arccosf(normal1.dot(normal2));  // inner product
    }
  };

private:
  struct Vector_to_point {
    template <typename Vector3f>
    Epec_point_3 operator()(const Vector3f& vec)
    { return Epec_point_3(vec[0], vec[1], vec[2]); }
  };

  /*! Indicates whether to triangulate a hole thereby filling it. */
  Boolean m_triangulate_holes;

  /*! Indicates whether to refine the triangulation of a hole by applying
   * local averaging rules.
   */
  Boolean m_refine;

  /*! Indicates whether to smooth the triangulation of a hole to obtain
   * as-smooth-as-possible shape deformation.
   */
  Boolean m_fair;

  //! Indicates whether to repair the normals of facets..
  Boolean m_repair_normals;

  /*! Indicates whether to repair the orientation of facets of closed polyhedral
   * surfaces.
   */
  Boolean m_repair_orientation;

  /*! Indicates whether to try and orient the facets to make them consistent
   * and duplicate singular vertices (and edges).
   */
  Boolean m_make_consistent;

  /*! Computes the convex hull of the coordinate set. */
  void convex_hull();

  //! The time is took to compute the minkowski sum in seconds.
  float m_time;

  //! Indicates whether all edges are creased.
  Boolean m_creased;

  //! Indicates whether all edges are smooth.
  Boolean m_smooth;

  //! The tag that identifies this container type.
  static const std::string s_tag;

  //! The node prototype.
  static Container_proto* s_prototype;
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief constructs the prototype.
inline Indexed_face_set* Indexed_face_set::prototype()
{ return new Indexed_face_set(true); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Indexed_face_set::create() { return new Indexed_face_set(); }

/*! \brief determines whether the coordinate array has been invalidated,
 * and thus must be cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_coord_array() const
{ return m_dirty_coord_array; }

/*! \brief determine whether the polyhedron has been invalidated, and thus
 * must be cleaned.
 */
inline Boolean Indexed_face_set::is_dirty_polyhedron() const
{ return m_dirty_polyhedron; }

//! brief sets the flag that determine whether the mesh has singular vertices.
inline Boolean Indexed_face_set::set_has_singular_vertices(Boolean flag)
{ m_has_singular_vertices = flag; }

//! brief obtains the polyhedron type.
inline Indexed_face_set::Polyhedron_type Indexed_face_set::get_polyhedron_type()
  const
{ return m_polyhedron_type; }

//! \brief clears (invalidate) the volume.
inline void Indexed_face_set::clear_volume() { m_dirty_volume = true; }

//! \brief clears (invalidate) the surface_area.
inline void Indexed_face_set::clear_surface_area()
{ m_dirty_surface_area = true; }

//! \brief clears (invalidate) the normal attributes m_creased and m_smooth.
inline void Indexed_face_set::clear_normal_attributes()
{ m_dirty_normal_attributes = true; }

//! \brief clears the polyhedron facets. (Invalidate their attributes.)
inline void Indexed_face_set::clear_polyhedron_facet_normals()
{ m_dirty_polyhedron_facet_normals = true; }

//! \brief sets the flag that determines wheather the mesh is repaired.
inline void Indexed_face_set::set_repaired(Boolean flag) { m_repaired = flag; }

//! \brief determines wheather the mesh is repaired.
inline Boolean Indexed_face_set::is_repaired() { return m_repaired; }

//! \brief sets the flag that determines wheather the mesh is consistent.
inline void Indexed_face_set::set_consistent(Boolean flag)
{ m_consistent = flag; }

//! \brief determines whether to compute the convex hull of the coordinate set.
inline Boolean Indexed_face_set::is_convex_hull() const
{ return m_convex_hull; }

SGAL_END_NAMESPACE

#endif
