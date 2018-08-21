// Copyright (c) 2015 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_CONSTRUCT_TRIANGULATION_HPP
#define SGAL_CONSTRUCT_TRIANGULATION_HPP

#include <list>

#include "SGAL/basic.hpp"
#include "SGAL/Face_nesting_level.hpp"

SGAL_BEGIN_NAMESPACE

/*! Construct a triangulation from an outline.
 * \param tri (out) the triangulation.
 * \param begin (in) the outline begin iterator.
 * \param end (in) the outline end iterator.
 * \param k (in)
 */
template <typename Triangulation_, typename InputIterator>
Uint construct_triangulation(Triangulation_& tri,
                             InputIterator begin, InputIterator end, Uint k)
{
  typedef Triangulation_                Triangulation;

  auto pit = begin;
  typename Triangulation::Point p((*pit)[0], (*pit)[1]);
  auto start = tri.insert(p);
  start->info().m_index = k++;
  auto prev = start;
  for (++pit; pit != end; ++pit) {
    typename Triangulation::Point p((*pit)[0], (*pit)[1]);
    auto next = tri.insert(p);
    if (static_cast<size_t>(-1) == next->info().m_index)
      next->info().m_index = k++;
    tri.insert_constraint(prev, next);
    prev = next;
  }
  tri.insert_constraint(prev, start);
  return k;
}

/*! Mark facets in a triangulation that are inside the domain bounded by
 * the polygon.
 * \param tri (in/out) the triangulation.
 */
template <typename Triangulation_>
void mark_domains(Triangulation_& tri,
                  typename Triangulation_::Face_handle start,int index,
                  std::list<typename Triangulation_::Edge>& border)
{
  typedef Triangulation_                Triangulation;

  if (start->info().m_nesting_level != -1) return;
  std::list<typename Triangulation::Face_handle> queue;
  queue.push_back(start);
  while (! queue.empty()) {
    auto fh = queue.front();
    queue.pop_front();
    if (fh->info().m_nesting_level == -1) {
      fh->info().m_nesting_level = index;
      for (int i = 0; i < 3; i++) {
        typename Triangulation::Edge e(fh,i);
        auto n = fh->neighbor(i);
        if (n->info().m_nesting_level == -1) {
          if (tri.is_constrained(e)) border.push_back(e);
          else queue.push_back(n);
        }
      }
    }
  }
}

/*! \brief marks facets in a triangulation that are inside the domain.
 * Explores set of facets connected with non constrained edges,
 * and attribute to each such set a nesting level.
 * We start from facets incident to the infinite vertex, with a nesting
 * level of 0. Then we recursively consider the non-explored facets incident
 * to constrained edges bounding the former set and increase the nesting
 * level by 1.
 * Facets in the domain are those with an odd nesting level.
 */
template <typename Triangulation_>
void mark_domains(Triangulation_& tri)
{
  typedef Triangulation_                Triangulation;

  for (auto it = tri.all_faces_begin(); it != tri.all_faces_end(); ++it)
    it->info().m_nesting_level = -1;

  std::list<typename Triangulation::Edge> border;
  mark_domains(tri, tri.infinite_face(), 0, border);
  while (! border.empty()) {
    auto e = border.front();
    border.pop_front();
    auto n = e.first->neighbor(e.second);
    if (n->info().m_nesting_level == -1)
      mark_domains(tri, n, e.first->info().m_nesting_level+1, border);
  }
}

SGAL_END_NAMESPACE

#endif
