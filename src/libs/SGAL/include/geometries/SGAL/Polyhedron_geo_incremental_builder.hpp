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

#ifndef SCGAL_POLYHEDRON_GEO_INCREMENTAL_BUILDER_HPP
#define SCGAL_POLYHEDRON_GEO_INCREMENTAL_BUILDER_HPP

#include <vector>
#include <cstddef>

#include "SGAL/basic.hpp"

#include <CGAL/basic.h>
#include <CGAL/Random_access_adaptor.h>
#include <CGAL/HalfedgeDS_decorator.h>
#include <CGAL/Unique_hash_map.h>
#include <CGAL/IO/Verbose_ostream.h>

SGAL_BEGIN_NAMESPACE

// An Incremental Builder for Polyhedral Surfaces
// ----------------------------------------------
// DEFINITION
//
// Polyhedron_geo_incremental_builder<HDS> is an auxiliary class that
// supports the incremental construction of polyhedral surfaces. This is
// for example convinient when constructing polyhedral surfaces from
// files. The incremental construction starts with a list of all point
// coordinates and concludes with a list of all facet polygons. Edges are
// not explicitly specified. They are derived from the incidence
// information provided from the facet polygons. These are given as a
// sequence of vertex indices. The correct protocol of method calls to
// build a polyhedral surface can be stated as regular expression:
//
// `begin_surface (add_vertex | (begin_facet add_vertex_to_facet*
//  end_facet))* end_surface '
template <typename HalfedgeDS_>
class Polyhedron_geo_incremental_builder {
public:
  typedef HalfedgeDS_                                  HDS; // internal
  typedef HalfedgeDS_                                  HalfedgeDS;
  typedef typename HDS::Vertex                         Vertex;
  typedef typename HDS::Halfedge                       Halfedge;
  typedef typename HDS::Face                           Face;
  typedef typename HDS::Vertex_handle                  Vertex_handle;
  typedef typename HDS::Halfedge_handle                Halfedge_handle;
  typedef typename HDS::Face_handle                    Face_handle;
  typedef typename HDS::Face_handle                    Facet_handle;
  typedef typename Vertex::Base                        VBase;
  typedef typename Halfedge::Base                      HBase;
  typedef typename Vertex::Point                       Point_3;
  typedef typename HDS::size_type                      size_type;

  //! Operation types.
  enum { RELATIVE_INDEXING = 0, ABSOLUTE_INDEXING = 1};

  /*! Construct
   * `HDS' is the halfedge data structure used to represent the
   * polyhedral surface that is to be constructed.
   * stores a reference to the halfedge data structure `h' in the
   * internal state. The previous polyhedral surface in `h'
   * remains unchanged. The incremental builder adds the new
   * polyhedral surface to the old one.
   */
  Polyhedron_geo_incremental_builder(HDS& h, bool verbose = false);

  /*! Destruct. */
  ~Polyhedron_geo_incremental_builder();

  /*! Start the construction.
   * \param[in] v the number of new vertices to expect.
   * \param[in] f the number of new facets
   * \param[in] h the number of new halfedges.
   * If h is unspecified (`== 0') it is estimated using Euler equations (plus
   * 5% for the so far unkown holes and genus of the object).  These values
   * are used to reserve space in the polyhedron representation `HDS'. If the
   * representation supports insertion these values do not restrict the class
   * of readable polyhedrons. If the representation does not support insertion
   * the object must fit in the reserved sizes.  If `mode' is set to
   * ABSOLUTE_INDEXING the incremental builder uses absolute indexing and the
   * vertices of the old polyhedral surface can be used in new facets.
   * Otherwise relative indexing is used starting with new indices for the
   * new construction.
   */
  void begin_surface(std::size_t v, std::size_t f, std::size_t h = 0,
                     int mode = RELATIVE_INDEXING);

  /*! End the construction.
   */
  void end_surface();

  /*! Add a vertex to the vertex list.
   * \param p[in] the geometric empedding of the newly added vertex.
   */
  Vertex_handle add_vertex(const Point_3& p);

  /*! Test whether the facet described by the vertex indices in a given
   * range can be inserted without creating a non-manifold (and therefore
   * invalid) situation.
   */
  template <typename InputIterator>
  bool test_facet(InputIterator first, InputIterator beyond);

  /*! Synonym for begin_facet(), a call to add_vertex_to_facet() for each
   * value in the given range, and a call to end_facet().
   */
  template <typename InputIterator>
  Halfedge_handle add_facet(InputIterator first, InputIterator beyond);

  /*! Start a new facet
   * \eturns a the representative handle to the new facet.
   */
  Facet_handle begin_facet();

  /*! End a facet.
   */
  Halfedge_handle end_facet();

  /*! Add a vertex with a given index to the current facet. The first
   * point added with `add_vertex()' has the index 0.
   * \param[in] i the vertex index.
   */
  void add_vertex_to_facet(std::size_t i);

  /*! Undo all changes made to the halfedge data structure since the last
   * call to begin_surface() in relative indexing, and deletes the whole
   * surface in absolute indexing.
   */
  void rollback();

  /*! Remove all unconnected vertices or, in case removal is not supported,
   * indicates whether all unconnected vertices could be removed successfully.
   */
  bool remove_unconnected_vertices()
  { return remove_unconnected_vertices(Supports_removal()); }

  /*! Indicates whether unconnected vertices are detected.
   */
  bool check_unconnected_vertices();

protected:
  typedef typename HDS::Supports_vertex_halfedge       Supports_vertex_halfedge;
  typedef typename HDS::Supports_removal               Supports_removal;
  typedef typename HDS::Vertex_iterator                Vertex_iterator;
  typedef typename HDS::Halfedge_iterator              Halfedge_iterator;
  typedef CGAL::Random_access_adaptor<Vertex_iterator> Random_access_index;

  bool m_error;
  bool m_verbose;
  HDS& m_hds;

  //! Number of vertices to preserve upon rollback.
  size_type m_rollback_v;

  //! Number of facets to preserve upon rollback.
  size_type m_rollback_f;

  //! Number of halfedges to preserve upon rollback.
  size_type m_rollback_h;

  //! Number of new vertices.
  size_type m_new_vertices;

  //! Number of new facets.
  size_type m_new_faces;

  //! Number of new halfedges.
  size_type m_new_halfedges;

  Random_access_index m_index_to_vertex_map;
  std::vector<Halfedge_handle> m_vertex_to_edge_map;

  Face_handle current_face;

  Halfedge_handle g1;      // first halfedge, 0 denotes none.
  Halfedge_handle gprime;
  Halfedge_handle h1;      // current halfedge
  size_type w1;      // first vertex.
  size_type w2;      // second vertex.
  size_type v1;      // current vertex
  bool first_vertex;
  bool last_vertex;

  CGAL_assertion_code(int check_protocoll);  // use to check protocoll.
  // states for checking: 0 = created, 1 = constructing, 2 = make face.

  /*! Test whether the facet described by the indices of its vertices can be
   * inserted without creating a non-manifold (and therefore invalid) situation.
   */
  bool test_facet_indices(std::vector<std::size_t> indices);

  // Implement the m_vertex_to_edge_map either with an array or
  // the halfedge pointer in the vertices (if supported).
  // ----------------------------------------------------

  // Initialize the vertex maps
  void initialize_vertex_to_edge_map(size_type n, bool mode)
  { initialize_vertex_to_edge_map(n, mode, Supports_vertex_halfedge()); }

  // Push back into the vertex maps
  void push_back_vertex_to_edge_map(Halfedge_handle h)
  { push_back_vertex_to_edge_map(h, Supports_vertex_halfedge()); }

  // Obtain an item from the vertex maps
  Halfedge_handle get_vertex_to_edge_map(size_type i)
  { return get_vertex_to_edge_map(i, Supports_vertex_halfedge()); }

  // Set and entry in the vertex maps
  void set_vertex_to_edge_map(size_type i, Halfedge_handle h)
  { set_vertex_to_edge_map(i, h, Supports_vertex_halfedge()); }

  /*! Initialize vertex maps in case vertex-halfedge is supported.
   */
  void initialize_vertex_to_edge_map(size_type  n, bool mode, CGAL::Tag_true);

  /*! Initialize vertex maps in case vertex-halfedge is not supported.
   */
  void initialize_vertex_to_edge_map(size_type n, bool mode, CGAL::Tag_false);

  /*! Push back an item into vertex maps in case vertex-halfedge is supported.
   */
  void push_back_vertex_to_edge_map(Halfedge_handle h, CGAL::Tag_true);

  /*! Push back an item into vertex maps in case vertex-halfedge is not
   * supported.
   */
  void push_back_vertex_to_edge_map(Halfedge_handle h, CGAL::Tag_false);

  /*! Obtain an item from vertex maps in case vertex-halfedge is supported.
   */
  Halfedge_handle get_vertex_to_edge_map(size_type i, CGAL::Tag_true);

  /*! Obtain an item from vertex maps in case vertex-halfedge is not supported.
   * Use the self-managed array m_vertex_to_edge_map.
   */
  Halfedge_handle get_vertex_to_edge_map(size_type i, CGAL::Tag_false);

  /*! Set an entry of the vertex maps in case vertex-halfedge is supported.
   */
  void set_vertex_to_edge_map(size_type i, Halfedge_handle h, CGAL::Tag_true);

  /*! Set an entry of the vertex maps in case vertex-halfedge is not supported.
   */
  void set_vertex_to_edge_map(size_type i, Halfedge_handle h, CGAL::Tag_false);

  bool error() const { return m_error; }

  /*! Find the index of a vertex.
   */
  size_type find_vertex(Vertex_handle v);

  /*! Find the index of a facet.
   */
  size_type find_facet(Face_handle f);

  /*! Obtain the handle for the vertex of index i.
   */
  Vertex_handle vertex(std::size_t i)
  {
    if (i < m_new_vertices) return m_index_to_vertex_map[i];
    return Vertex_handle();
  }

  /*! Remove all unconnected vertices.
   * (In case removal is supported.)
   */
  bool remove_unconnected_vertices(CGAL::Tag_true);

  /*! Indicates whether all unconnected vertices could be removed successfully
   * (in case removal is not supported).
   */
  bool remove_unconnected_vertices(CGAL::Tag_false)
  { return ! check_unconnected_vertices(); }

  /*! Walk around a given vertex to find a hole in the facet structure.
   * Report an error if none exist.
   * \param[in] w the vertex being walked around.
   * \return the halfedge at this hole that points to the vertex.
   */
  Halfedge_handle lookup_hole(std::size_t w);

  /* Walk around a vertex pointed by a given halfedge to find a hole
   * in the facet structure. Report an error if none exist.
   * \param[in] e the halfedge that points to the vertex being walked around.
   * \return the halfedge at this hole that points to the vertex.
   */
  Halfedge_handle lookup_hole(Halfedge_handle e);

  /* Pre: 0 <= w,v < m_new_vertices
   * Case a: It exists an halfedge g from w to v:
   *     g must be a border halfedge and the facet of g->opposite()
   *     must be set and different from the current facet.
   *     Set the facet of g to the current facet. Return the
   *     halfedge pointing to g.
   * Case b: It exists no halfedge from w to v:
   *     Create a new pair of halfedges g and g->opposite().
   *     Set the facet of g to the current facet and g->opposite()
   *     to a border halfedge. Assign the vertex references.
   *     Set g->opposite()->next() to g. Return g->opposite().
   */
  Halfedge_handle lookup_halfedge(size_type w, size_type v);
};

//! \brief constructs
template <typename HDS>
Polyhedron_geo_incremental_builder<HDS>::
Polyhedron_geo_incremental_builder(HDS& h, bool verbose) :
  m_error(false),
  m_verbose(verbose),
  m_hds(h)
{ CGAL_assertion_code(check_protocoll = 0); }

//! \brief destructs.
template <typename HDS>
Polyhedron_geo_incremental_builder<HDS>::~Polyhedron_geo_incremental_builder()
{ CGAL_assertion(check_protocoll == 0); }

//! \brief starts the construction.
template <typename HDS>  CGAL_MEDIUM_INLINE
void Polyhedron_geo_incremental_builder<HDS>::
begin_surface(std::size_t v, std::size_t f, std::size_t h, int mode)
{
  CGAL_assertion(check_protocoll == 0);
  CGAL_assertion_code(check_protocoll = 1);
  CGAL_assertion(! m_error);
  if (mode == RELATIVE_INDEXING) {
    m_new_vertices = 0;
    m_new_faces = 0;
    m_new_halfedges = 0;
    m_rollback_v = m_hds.size_of_vertices();
    m_rollback_f = m_hds.size_of_faces();
    m_rollback_h = m_hds.size_of_halfedges();
  }
  else {
    m_new_vertices = m_hds.size_of_vertices();
    m_new_faces = m_hds.size_of_faces();
    m_new_halfedges = m_hds.size_of_halfedges();
    m_rollback_v = 0;
    m_rollback_f = 0;
    m_rollback_h = 0;
  }
  if (h == 0) {
    // Use the Eulerian equation for connected planar graphs. We do
    // not know the number of facets that are holes and we do not
    // know the genus of the surface. So we add 12 and a factor of
    // 5 percent.
    h = (std::size_t)((double)(v + f - 2 + 12) * 2.1);
  }
  m_hds.reserve(m_hds.size_of_vertices() + v,
                m_hds.size_of_halfedges() + h,
                m_hds.size_of_faces() + f);
  if (mode == RELATIVE_INDEXING) {
    m_index_to_vertex_map = Random_access_index(m_hds.vertices_end());
    m_index_to_vertex_map.reserve(v);
    initialize_vertex_to_edge_map(v, false);
  }
  else {
    m_index_to_vertex_map = Random_access_index(m_hds.vertices_begin(),
                                              m_hds.vertices_end());
    m_index_to_vertex_map.reserve(m_hds.size_of_vertices() + v);
    initialize_vertex_to_edge_map(m_hds.size_of_vertices() + v, true);
  }
}

//! \brief ends the construction.
template <typename HDS> CGAL_MEDIUM_INLINE
void Polyhedron_geo_incremental_builder<HDS>::end_surface()
{
  if (m_error) return;
  CGAL_assertion(check_protocoll == 1);
  CGAL_assertion_code(check_protocoll = 0);
}

//! \brief adds a vertex to the vertex list.
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Vertex_handle
Polyhedron_geo_incremental_builder<HDS>::add_vertex(const Point_3& p)
{
  CGAL_assertion(check_protocoll == 1);
  if (m_hds.size_of_vertices() >= m_hds.capacity_of_vertices()) {
    CGAL::Verbose_ostream verr(m_verbose);
    verr << " " << std::endl;
    verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::" << std::endl;
    verr << "add_vertex(): capacity error: more than " << m_new_vertices
         << " vertices added." << std::endl;
    m_error = true;
    return Vertex_handle();
  }
  CGAL::HalfedgeDS_decorator<HDS> decorator(m_hds);
  Vertex_handle v = decorator.vertices_push_back(Vertex(p));
  m_index_to_vertex_map.push_back(v);
  decorator.set_vertex_halfedge(v, Halfedge_handle());
  push_back_vertex_to_edge_map(Halfedge_handle());
  ++m_new_vertices;
  return v;
}

/*! \brief tests whether the facet described by the vertex indices in a given
 * range can be inserted without creating a non-manifold (and therefore
 * invalid) situation.
 * First, create a copy of the indices and close it cyclically
 */
template <typename HDS>
template <typename InputIterator>
bool Polyhedron_geo_incremental_builder<HDS>::test_facet(InputIterator first,
                                                         InputIterator beyond)
{
  std::vector<std::size_t> indices(first, beyond);
  if (indices.size() < 3) return false;
  indices.push_back(indices[0]);
  return test_facet_indices(indices);
}

/*! \brief synonym for begin_facet(), a call to add_vertex_to_facet() for
 * each value in the given range, and a call to end_facet().
 */
template <typename HDS>
template <typename InputIterator>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::add_facet(InputIterator first,
                                                   InputIterator beyond)
{
  begin_facet();
  for (; !m_error && first != beyond; ++first) add_vertex_to_facet(*first);
  if (m_error) return Halfedge_handle();
  return end_facet();
}

//! \brief starts a new facet.
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Facet_handle
Polyhedron_geo_incremental_builder<HDS>::begin_facet()
{
  // starts a facet.
  if (m_error) return Facet_handle();
  CGAL_assertion(check_protocoll == 1);
  CGAL_assertion_code(check_protocoll = 2;)
    if (m_hds.size_of_faces() >= m_hds.capacity_of_faces()) {
      CGAL::Verbose_ostream verr(m_verbose);
      verr << " " << std::endl;
      verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::" << std::endl;
      verr << "begin_facet(): capacity error: more than " << m_new_vertices
           << " facets added." << std::endl;
      m_error = true;
      return Facet_handle();
    }
  // initialize all status variables.
  first_vertex = true;  // denotes 'no vertex yet'
  g1 =  Halfedge_handle();  // denotes 'no halfedge yet'
  last_vertex = false;

  CGAL::HalfedgeDS_decorator<HDS> decorator(m_hds);
  current_face = decorator.faces_push_back(Face());
  return current_face;
}

//! \brief ends a facet.
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::end_facet()
{
  if (m_error) return Halfedge_handle();
  CGAL_assertion(check_protocoll == 2);
  CGAL_assertion(! first_vertex);
  // cleanup all static status variables
  add_vertex_to_facet(w1);
  if (m_error) return Halfedge_handle();
  last_vertex = true;
  add_vertex_to_facet(w2);
  if (m_error) return Halfedge_handle();
  CGAL_assertion(check_protocoll == 2);
  CGAL_assertion_code(check_protocoll = 1);
  CGAL::HalfedgeDS_items_decorator<HDS> decorator;
  Halfedge_handle h = get_vertex_to_edge_map(w1);
  decorator.set_face_halfedge(current_face, h);
  ++m_new_faces;
  return h;
}

/*! \brief undoes all changes made to the halfedge data structure since the
 * last call to begin_surface() in relative indexing, and deletes the whole
 * surface in absolute indexing. .
 */
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::rollback()
{
  CGAL_assertion(m_rollback_v <= m_hds.size_of_vertices());
  CGAL_assertion(m_rollback_h <= m_hds.size_of_halfedges());
  CGAL_assertion(m_rollback_f <= m_hds.size_of_faces());
  if ((m_rollback_v == 0) && (m_rollback_h == 0) && (m_rollback_f == 0)) {
    m_hds.clear();
  }
  else {
    while (m_rollback_v != m_hds.size_of_vertices()) m_hds.vertices_pop_back();
    CGAL_assertion(((m_hds.size_of_halfedges() - m_rollback_h) & 1) == 0);
    while (m_rollback_h != m_hds.size_of_halfedges()) m_hds.edges_pop_back();
    while (m_rollback_f != m_hds.size_of_faces()) m_hds.faces_pop_back();
  }
  m_error = false;
  CGAL_assertion_code(check_protocoll = 0);
}

//! \brief adds a vertex with a given index to the current facet.
template <typename HDS>
void
Polyhedron_geo_incremental_builder<HDS>::add_vertex_to_facet(std::size_t v2)
{
  if (m_error) return;
  CGAL_assertion(check_protocoll == 2);
  if (v2 >= m_new_vertices) {
    CGAL::Verbose_ostream verr(m_verbose);
    verr << " " << std::endl;
    verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::"
         << std::endl;
    verr << "add_vertex_to_facet(): vertex index " << v2
         << " is out-of-range [0," << m_new_vertices-1 << "]."
         << std::endl;
    m_error = true;
    return;
  }
  CGAL::HalfedgeDS_items_decorator<HDS> decorator;

  if (first_vertex) {
    CGAL_assertion(! last_vertex);
    w1 = v2;
    first_vertex = false;
    return;
  }
  if (g1 == Halfedge_handle()) {
    CGAL_assertion(! last_vertex);
    gprime  = lookup_halfedge(w1, v2);
    if (m_error) return;
    h1 = g1 = gprime->next();
    v1 = w2 = v2;
    return;
  }
  // g1, h1, v1, w1, w2 are set. Insert halfedge.
  // Lookup v1-->v2
  Halfedge_handle hprime;
  if (last_vertex) hprime = gprime;
  else {
    hprime = lookup_halfedge(v1, v2);
    if (m_error) return;
  }
  auto h2 = hprime->next();
  CGAL_assertion(! last_vertex || h2 == g1);
  auto prev = h1->next();
  h1->HBase::set_next(h2);
  decorator.set_prev(h2, h1);

  if (get_vertex_to_edge_map(v1) == Halfedge_handle()) {  // case 1:
    h2->opposite()->HBase::set_next(h1->opposite());
    decorator.set_prev(h1->opposite(), h2->opposite());
  }
  else {                                                  // case 2:
    bool b1 = h1->opposite()->is_border();
    bool b2 = h2->opposite()->is_border();
    if (b1 && b2) {
      auto hole = lookup_hole(v1);
      if (m_error) return;
      CGAL_assertion(hole != Halfedge_handle());
      h2->opposite()->HBase::set_next(hole->next());
      decorator.set_prev(hole->next(), h2->opposite());
      hole->HBase::set_next(h1->opposite());
      decorator.set_prev(h1->opposite(), hole);
    }
    else if (b2) {                                     // case 2.b:
      CGAL_assertion(prev->is_border());
      h2->opposite()->HBase::set_next(prev);
      decorator.set_prev(prev, h2->opposite());
    }
    else if (b1) {                                     // case 2.c:
      CGAL_assertion(hprime->is_border());
      hprime->HBase::set_next(h1->opposite());
      decorator.set_prev(h1->opposite(), hprime);
    }
    else if (h2->opposite()->next() == h1->opposite()) {// case 2.d:
      // f1 == f2
      CGAL_assertion(decorator.get_face(h1->opposite()) ==
                      decorator.get_face(h2->opposite()));
    }
    else {                                              // case 2.e:
      if (prev == h2) {                                // case _i:
        // nothing to be done, hole is closed.
      }
      else {                                          // case _ii:
        CGAL_assertion(prev->is_border());
        CGAL_assertion(hprime->is_border());
        hprime->HBase::set_next(prev);
        decorator.set_prev(prev, hprime);
        // Check whether the halfedges around v1 are connected.
        // It is sufficient to check it for h1 to prev.
        // Assert loop termination:
        CGAL_assertion_code(std::size_t k = 0);
          // Look for a hole in the facet complex starting at h1.
        Halfedge_handle hole;
        Halfedge_handle e = h1;
        do {
          if (e->is_border()) hole = e;
          e = e->next()->opposite();
          CGAL_assertion(k++ < m_hds.size_of_halfedges());
        } while (e->next() != prev && e != h1);
        if (e == h1) {
          // disconnected facet complexes
          if (hole != Halfedge_handle()) {
            // The complex can be connected with
            // the hole at hprime.
            hprime->HBase::set_next(hole->next());
            decorator.set_prev(hole->next(), hprime);
            hole->HBase::set_next(prev);
            decorator.set_prev(prev, hole);
          }
          else {
            CGAL::Verbose_ostream verr(m_verbose);
            verr << " " << std::endl;
            verr << "CGAL::Polyhedron_geo_incremental_builder<"
                 << "HDS>::" << std::endl;
            verr << "add_vertex_to_facet(): input error: "
                 << "disconnected facet complexes at vertex "
                 << v1 << ":" << std::endl;

            if (m_verbose && current_face != Face_handle()) {
              verr << "           involved facets are:";
              do {
                if (! e->is_border())
                  verr << " " << find_facet(decorator.get_face(e));
                e = e->next()->opposite();
              } while (e != h1);
              verr << " (closed cycle) and";
              e = hprime;
              do {
                if (! e->is_border())
                  verr << " " << find_facet(decorator.get_face(e));
              } while (e != hprime);
              verr << "." << std::endl;
            }
            m_error = true;
            return;
          }
        }
      }
    }
  }
  if (h1->vertex() == m_index_to_vertex_map[v1])
    set_vertex_to_edge_map(v1, h1);
  CGAL_assertion(h1->vertex() == m_index_to_vertex_map[v1]);
  h1 = h2;
  v1 = v2;
}

/*! \brief tests whether the facet described by the indices of its vertices can
 * be inserted without creating a non-manifold (and therefore invalid)
 * situation.
 */
template <typename HDS>
bool Polyhedron_geo_incremental_builder<HDS>::
test_facet_indices(std::vector<std::size_t> indices)
{
  std::cout << "testing "
            << indices[0] << ","
            << indices[1] << ","
            << indices[2] << ","
            << std::endl;
  typedef typename HDS::Supports_halfedge_vertex Supports_halfedge_vertex;
  Assert_compile_time_tag(Supports_halfedge_vertex(), CGAL::Tag_true());

  // indices are cyclically closed once.
  std::size_t n = indices.size() - 1;

  // Test whether a vertex is not twice in the indices
  for (std::size_t i = 0; i < n; ++i) {
    CGAL_precondition(indices[i] < m_new_vertices);
    // Check whether vertex indices[i] is already in the sequence [0..i-1]
    for (std::size_t k = 0; k < i; ++k) {
      if (indices[k] == indices[i]) {
        std::cerr << "Error: duplicate indices!" << std::endl;
        return false;
      }
    }
  }

  // Test non-manifold halfedges
  for (std::size_t i = 0; i < n; ++i) {
    // halfedge goes from vertex indices[i] to indices[i+1]
    // we know already that the halfedge is only once in the sequence
    // (otherwise the end-vertices would be twice in the sequence too)
    // check whether halfedge is already in the HDS and is not border halfedge
    auto v = get_vertex_to_edge_map(indices[i]);
    auto w = m_index_to_vertex_map[indices[i+1]];
    if (v != Halfedge_handle() &&
        get_vertex_to_edge_map(indices[i+1]) != Halfedge_handle())
    {
      // cycle through halfedge-loop and find edge to indices[i+1]
      Halfedge_handle vstart = v;
      do {
        v = v->next()->opposite();
      } while (v->next()->vertex() != w && v != vstart);
      if ((v->next()->vertex() == w) && ! v->next()->is_border()) {
        std::cerr << "Error: non-manifold halfedge!" << std::endl;
        return false;
      }
    }
  }

  // Test non-manifold vertices
  for (std::size_t i = 0; i < n; ++i) {
    // since we don't allow duplicates in indices[..] and we
    // tested for non-manifold halfedges already, we just need to check
    // if the vertex indices[i] is not a closed manifold yet.
    auto v = get_vertex_to_edge_map(indices[i]);
    if (v != Halfedge_handle()) {
      Halfedge_handle vstart = v;
      do {
        v = v->next()->opposite();
      } while (! v->is_border() && v != vstart);
      if (! v->is_border()) {
        std::cerr << "Error: non-manifold vertex!" << std::endl;
        return false;
      }
    }
  }

  //Test if all halfedges of the new face
  //are possibly consecutive border halfedges in the HDS.
  //Possibly because it may be not directly encoded in the HDS
  //(using next() function ). This situation can occur when one or
  //more facets share only a vertex: For example, the new facet we try to add
  //would make the vertex indices[i] a manifold but this should be forbidden
  //if a facet only incident to that vertex has already been inserted.
  //We check this for each vertex of the sequence.
  for (std::size_t i = 0; i < n; ++i) {
    std::size_t prev_index = indices[(i - 1 + n) % n];
    std::size_t next_index = indices[(i + 1) % n];
    auto previous_vertex = m_index_to_vertex_map[prev_index];
    auto next_vertex = m_index_to_vertex_map[next_index];

    auto v = get_vertex_to_edge_map(indices[i]);

    if (v == Halfedge_handle() ||
        get_vertex_to_edge_map(prev_index) == Halfedge_handle() ||
        get_vertex_to_edge_map(next_index) == Halfedge_handle())
      continue;

    Halfedge_handle start = v;
    //halfedges pointing to/running out from vertex indices[i]
    //and that need to be possibly consecutive
    auto previous= Halfedge_handle();
    auto next = Halfedge_handle();

    //look for a halfedge incident to vertex indices[i]
    //and which opposite is incident to previous_vertex
    do {
      if (v->opposite()->vertex() == previous_vertex) {
        previous = v;
        CGAL_precondition(previous->is_border());
        break;
      }
      v = v->next()->opposite();
    }
    while (v != start);

    if (previous != Halfedge_handle()){
      v = v->next()->opposite();
      //previous and next are already consecutive in the HDS
      if (v->opposite()->vertex() == next_vertex) continue;

      //look for a border halfedge which opposite is
      //incident to next_vertex: set next halfedge
      do {
        if (v->opposite()->vertex() == next_vertex) {
          next = v->opposite();
          break;
        }
        v = v->next()->opposite();
      }
      while (v != previous);
      if (next == Halfedge_handle()) continue;

      //check if no constraint prevents
      //previous and next to be adjacent:
      do {
        v = v->next()->opposite();
        if (v->opposite()->is_border() ) break;
      }
      while (v != previous);
      if (v == previous) {
        std::cerr << "Error: non-consecutive border!" << std::endl;
        return false;
      }
      start = v;
    }
  }

  return true;
}

//! \brief initializes vertex maps in case vertex-halfedge is supported.
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
initialize_vertex_to_edge_map(size_type  n, bool mode, CGAL::Tag_true)
{
  m_vertex_to_edge_map.clear();
  m_vertex_to_edge_map.resize(n);
  if (mode) {
    // go through all halfedges and keep a halfedge for each
    // vertex found in a hashmap.
    size_type i = 0;
    for (auto vi = m_hds.vertices_begin(); vi != m_hds.vertices_end(); ++vi) {
      set_vertex_to_edge_map(i, vi->halfedge());
      ++i;
    }
  }
}

//! \brief initialize vertex maps in case vertex-halfedge is not supported.
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
initialize_vertex_to_edge_map(size_type n, bool mode, CGAL::Tag_false)
{
  m_vertex_to_edge_map.clear();
  m_vertex_to_edge_map.resize(n);
  if (mode) {
    // go through all halfedges and keep a halfedge for each
    // vertex found in a hashmap.
    typedef CGAL::Unique_hash_map< Vertex_iterator, Halfedge_handle> V_map;
    Halfedge_handle hh;
    V_map v_map(hh, m_hds.size_of_vertices());
    for (auto hi = m_hds.halfedges_begin(); hi != m_hds.halfedges_end(); ++hi)
      v_map[hi->vertex()] = hi;
    size_type i = 0;
    for (auto vi = m_hds.vertices_begin(); vi != m_hds.vertices_end();  ++vi) {
      //set_vertex_to_edge_map(i, v_map[ m_index_to_vertex_map[i]]);
      set_vertex_to_edge_map(i, v_map[ vi]);
      ++i;
    }
  }
}

/*! \brief Pushes back an item into vertex maps in case vertex-halfedge is
 * supported.
 */
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
push_back_vertex_to_edge_map(Halfedge_handle h, CGAL::Tag_true)
{ push_back_vertex_to_edge_map(h, CGAL::Tag_false()); }

/*! \brief pushes back an item into vertex maps in case vertex-halfedge is not
 * supported.
 */
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
push_back_vertex_to_edge_map(Halfedge_handle h, CGAL::Tag_false)
{ m_vertex_to_edge_map.push_back(h); }

/*! \brief obtains an item from vertex maps in case vertex-halfedge is supported.
 */
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::get_vertex_to_edge_map(size_type i,
                                                                CGAL::Tag_true)
{
  // Use the halfedge pointer within the vertex.
  // CGAL_assertion(m_index_to_vertex_map[i]->halfedge() ==
  //                get_vertex_to_edge_map(i, CGAL::Tag_false()));
  return m_index_to_vertex_map[i]->halfedge();
}

/*! \brief obtains an item from vertex maps in case vertex-halfedge is not
 * supported.
 * Use the self-managed array m_vertex_to_edge_map.
 */
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::get_vertex_to_edge_map(size_type i,
                                                                CGAL::Tag_false)
{ return m_vertex_to_edge_map[i]; }

/*! \brief sets an entry of the vertex maps in case vertex-halfedge is
 * supported.
 */
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
set_vertex_to_edge_map(size_type i, Halfedge_handle h, CGAL::Tag_true)
{
  set_vertex_to_edge_map(i, h, CGAL::Tag_false());
  // Use the halfedge pointer within the vertex.
  m_index_to_vertex_map[i]->VBase::set_halfedge(h);
}

/*! \brief sets an entry of the vertex maps in case vertex-halfedge is not
 * supported.
 */
template <typename HDS>
void Polyhedron_geo_incremental_builder<HDS>::
set_vertex_to_edge_map(size_type i, Halfedge_handle h, CGAL::Tag_false)
{
  // Use the self-managed array m_vertex_to_edge_map.
  CGAL_assertion(i < m_vertex_to_edge_map.size());
  m_vertex_to_edge_map[i] = h;
}

template <typename HDS>
bool Polyhedron_geo_incremental_builder<HDS>::check_unconnected_vertices()
{
  if (m_error) return false;
  bool unconnected = false;
  CGAL::Verbose_ostream verr(m_verbose);
  for (std::size_t i = 0; i < m_new_vertices; ++i) {
    if (get_vertex_to_edge_map(i) == Halfedge_handle()) {
      verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::\n"
           << "check_unconnected_vertices(verb = true): "
           << "vertex " << i << " is unconnected." << std::endl;
      unconnected = true;
    }
  }
  return unconnected;
}

//! \brief removes all unconnected vertices.
template <typename HDS>
bool Polyhedron_geo_incremental_builder<HDS>::
remove_unconnected_vertices(CGAL::Tag_true)
{
  if (m_error) return true;
  for (std::size_t i = 0; i < m_new_vertices; ++i) {
    if (get_vertex_to_edge_map(i) == Halfedge_handle()) {
      m_hds.vertices_erase(m_index_to_vertex_map[i]);
    }
  }
  return true;
}

//! \brief
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::lookup_hole(std::size_t w)
{
  CGAL_assertion(w < m_new_vertices);
  return lookup_hole(get_vertex_to_edge_map(w));
}

//! \brief
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::lookup_hole(Halfedge_handle e)
{
  CGAL_assertion(e != Halfedge_handle());
  CGAL::HalfedgeDS_items_decorator<HDS> decorator;
  Halfedge_handle start_edge(e);
  do {
    if (e->next()->is_border()) return e;
    e = e->next()->opposite();
  } while (e != start_edge);

  CGAL::Verbose_ostream verr(m_verbose);
  verr << " " << std::endl;
  verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::" << std::endl;
  verr << "lookup_hole(): input error: at vertex "
       << find_vertex(e->vertex())
       << " a closed surface already exists and facet "
       << m_new_faces << " is nonetheless adjacent." << std::endl;
  if (m_verbose && current_face != Face_handle()) {
    verr << "             The closed cycle of facets is:";
    do {
      if (! e->is_border())
        verr << " " << find_facet(decorator.get_face(e));
      e = e->next()->opposite();
    } while (e != start_edge);
    verr << '.' << std::endl;
  }
  m_error = true;
  return Halfedge_handle();
}

//! \brief
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::Halfedge_handle
Polyhedron_geo_incremental_builder<HDS>::lookup_halfedge(size_type w,
                                                         size_type v)
{
  typedef typename HDS::Supports_halfedge_vertex Supports_halfedge_vertex;
  Assert_compile_time_tag(Supports_halfedge_vertex(), CGAL::Tag_true());
  CGAL_assertion(w < m_new_vertices);
  CGAL_assertion(v < m_new_vertices);
  CGAL_assertion(! last_vertex);
  CGAL::HalfedgeDS_items_decorator<HDS> decorator;
  Halfedge_handle e = get_vertex_to_edge_map(w);
  if (e != Halfedge_handle()) {
    CGAL_assertion(e->vertex() == m_index_to_vertex_map[w]);
    // check that the facet has no self intersections
    if (current_face != Face_handle() && current_face == decorator.get_face(e))
    {
      CGAL::Verbose_ostream verr(m_verbose);
      verr << " " << std::endl;
      verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::" << std::endl;
      verr << "lookup_halfedge(): input error: facet "
           << m_new_faces << " has a self intersection at vertex "
           << w << "." << std::endl;
      m_error = true;
      return Halfedge_handle();
    }
    Halfedge_handle start_edge(e);
    do {
      if (e->next()->vertex() == m_index_to_vertex_map[v]) {
        if (! e->next()->is_border()) {
          CGAL::Verbose_ostream verr(m_verbose);
          verr << " " << std::endl;
          verr << "CGAL::Polyhedron_geo_incremental_builder"
               << "<HDS>::" << std::endl;
          verr << "lookup_halfedge(): input error: facet "
               << m_new_faces << " shares a halfedge from "
               << "vertex " <<  w << " to vertex " << v << " with";
          if (m_verbose && current_face != Face_handle())
            verr << " facet "
                 << find_facet(decorator.get_face(e->next()))
                 << '.' << std::endl;
          else
            verr << " another facet." << std::endl;
          m_error = true;
          return Halfedge_handle();
        }
        CGAL_assertion(! e->next()->opposite()->is_border());
        if (current_face != Face_handle() &&
            current_face == decorator.get_face(e->next()->opposite()))
        {
          CGAL::Verbose_ostream verr(m_verbose);
          verr << " " << std::endl;
          verr << "CGAL::Polyhedron_geo_incremental_builder"
               << "<HDS>::" << std::endl;
          verr << "lookup_halfedge(): input error: facet "
               << m_new_faces << " has a self intersection "
               << "at the halfedge from vertex " << w
               << " to vertex " << v << "." << std::endl;
          m_error = true;
          return Halfedge_handle();
        }
        decorator.set_face(e->next(), current_face);
        // The following line prevents e->next() to be picked
        // by get_vertex_to_edge_map(v) in an upcoming call
        // of lookup_halfedge(v, *)
        set_vertex_to_edge_map(v, e->next()->next()->opposite());
        return e;
      }
      e = e->next()->opposite();
    } while (e != start_edge);
  }
  // create a new halfedge
  if (m_hds.size_of_halfedges() >= m_hds.capacity_of_halfedges()) {
    CGAL::Verbose_ostream verr(m_verbose);
    verr << " " << std::endl;
    verr << "CGAL::Polyhedron_geo_incremental_builder<HDS>::" << std::endl;
    verr << "lookup_halfedge(): capacity error: more than "
         << m_new_halfedges << " halfedges added while creating facet"
         << m_new_faces << '.' << std::endl;
    m_error = true;
    return Halfedge_handle();
  }
  e = m_hds.edges_push_back(Halfedge(), Halfedge());
  m_new_halfedges++;
  m_new_halfedges++;
  decorator.set_face(e, current_face);
  e->HBase::set_vertex(m_index_to_vertex_map[v]);
  e->HBase::set_next(Halfedge_handle());
  decorator.set_prev(e, e->opposite());
  e = e->opposite();
  e->HBase::set_vertex(m_index_to_vertex_map[w]);
  e->HBase::set_next(e->opposite());
  return e;
}

//! \brief finds the index of a vertex.
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::size_type
Polyhedron_geo_incremental_builder<HDS>::find_vertex(Vertex_handle v)
{
  // Returns 0 if v == NULL.
  if (v == Vertex_handle() ) return 0;
  size_type n = 0;
  auto it = m_hds.vertices_begin();
  while (it != v) {
    CGAL_assertion(it != m_hds.vertices_end());
    ++n;
    ++it;
  }
  n = n - m_rollback_v;
  return n;
}

//! \brief finds the index of a facet.
template <typename HDS>
typename Polyhedron_geo_incremental_builder<HDS>::size_type
Polyhedron_geo_incremental_builder<HDS>::find_facet(Face_handle f)
{
  // Returns 0 if f == NULL.
  if (f == Face_handle()) return 0;
  size_type n = 0;
  auto it = m_hds.faces_begin();
  while (it != f) {
    CGAL_assertion(it != m_hds.faces_end());
    ++n;
    ++it;
  }
  n = n - m_rollback_f;
  return n;
}

SGAL_END_NAMESPACE

#endif
