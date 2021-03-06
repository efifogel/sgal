// Copyright (c) 2014 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CALCULATE_MULTIPLE_NORMALS_PER_VERTEX_HPP
#define SGAL_CALCULATE_MULTIPLE_NORMALS_PER_VERTEX_HPP

#include <vector>
#include <boost/type_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/variant.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Set_index_facet_indices_visitor.hpp"

SGAL_BEGIN_NAMESPACE

template <typename FacetHandle>
Vector3f get_normal_impl(FacetHandle f, boost::true_type)
{ return Vector3f(f->plane().x(), f->plane().y(), f->plane().z()); }

template <typename FacetHandle>
Vector3f get_normal_impl(FacetHandle f, boost::false_type)
{ return f->get_normal(); }

template <typename Polyhedron_>
Vector3f get_normal(typename Polyhedron_::Facet_const_handle f)
{
  typedef Polyhedron_                                   Polyhedron;
  typedef boost::is_same<typename Polyhedron::Plane_3, Inexact_kernel::Vector_3>
                                                        Polyhedron_has_vector;
  return get_normal_impl(f, Polyhedron_has_vector());
}

/*! Calculate multiple normal per vertex polyhedron visitor. */
class Calculate_multiple_normals_per_vertex_visitor :
  public boost::static_visitor<>
{
private:
  typedef boost::shared_ptr<Normal_array>       Shared_normal_array;

  Shared_normal_array m_normal_array;
  Facet_indices& m_normal_indices;

public:
  Calculate_multiple_normals_per_vertex_visitor
  (Shared_normal_array normal_array, Facet_indices& normal_indices) :
    m_normal_array(normal_array),
    m_normal_indices(normal_indices)
  {}

  template <typename Polyhedron_>
  void operator()(Polyhedron_& polyhedron) const
  {
    calculate_multiple_normals_per_vertex(polyhedron, m_normal_array,
                                          m_normal_indices);
  }
};

template <typename Polyhedron_>
void calculate_multiple_normals_per_vertex
(Polyhedron_& polyhedron,
 boost::shared_ptr<Normal_array> normal_array, Facet_indices& normal_indices)
{
  typedef Polyhedron_                                   Polyhedron;
  typedef typename Polyhedron::Facet                    Facet;
  typedef typename Polyhedron::Facet_const_handle       Facet_const_handle;
  typedef typename Polyhedron::Halfedge_around_facet_const_circulator
    Halfedge_around_facet_const_circulator;
  typedef typename Polyhedron::Halfedge_around_vertex_const_circulator
    Halfedge_around_vertex_const_circulator;

  struct Normal_hash {
    std::size_t operator()(Vector3f const& key) const
    {
      std::size_t seed = 0;
      boost::hash_combine(seed, key[0]);
      boost::hash_combine(seed, key[1]);
      boost::hash_combine(seed, key[2]);
      return seed;
    }
  };
  typedef boost::unordered_map<Vector3f, Uint, Normal_hash> Normal_map;
  Normal_map normal_map;

  auto it = polyhedron.facets_begin();
  for (; it != polyhedron.facets_end(); ++it) {
    const Facet& facet = *it;
    Halfedge_around_facet_const_circulator start = facet.facet_begin();
    Halfedge_around_facet_const_circulator he = start;
    do {
      auto index = he->id();

      // Go backwards around the vertex until a creased edge is encountered.
      Halfedge_around_vertex_const_circulator hev = he->vertex_begin();
      Halfedge_around_vertex_const_circulator startv = hev;
      while (!hev->is_creased()) {
        if (--hev == startv) break;
      }

      // Go forwards and collect the normals.
      size_t cnt = 1;
      Facet_const_handle f = hev->facet();
      Vector3f prev_normal = get_normal<Polyhedron>(f);
      Vector3f normal_res(prev_normal);
      startv = hev++;
      while ((hev != startv) && !hev->is_creased()) {
        if (hev->is_border()) {
          ++hev;
          continue;
        }
        Facet_const_handle f = hev->facet();
        Vector3f normal = get_normal<Polyhedron>(f);
        if (normal != prev_normal) {
          normal_res.add(normal);
          ++cnt;
        }
        prev_normal = normal;
        ++hev;
      }
      normal_res.scale(1.0f/static_cast<Float>(cnt));
      typename Normal_map::const_iterator got = normal_map.find(normal_res);
      if (got != normal_map.end()) {
        Set_index_facet_indices_visitor visitor(index, got->second);
        boost::apply_visitor(visitor, normal_indices);
      }
      else {
        Uint id = normal_array->size();
        Set_index_facet_indices_visitor visitor(index, id);
        boost::apply_visitor(visitor, normal_indices);
        normal_map[normal_res] = id;
        normal_res.normalize();
        normal_array->push_back(normal_res);
      }

    } while (++he != start);
  }
  normal_map.clear();
}

SGAL_END_NAMESPACE

#endif
