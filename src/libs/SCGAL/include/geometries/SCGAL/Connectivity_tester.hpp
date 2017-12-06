// Copyright (c) 2008 Israel.
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
// Author(s): Tzvika Geft <zvigreg@mail.tau.ac.il>

#ifndef SCGAL_CONNECTIVITY_TESTER_HPP
#define SCGAL_CONNECTIVITY_TESTER_HPP

/*! \file
 * A class template for strong connectivity testing of NDBG cells utlizing
 * lookahead. The implementation is based on the paper 'On certificates and lookahead in dynamic graph problems' [Khanna-Motwani-
Wilson ‘98] 
 */

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/optional.hpp>

#include "SCGAL/Graph_algorithms.hpp"

SGAL_BEGIN_NAMESPACE

template <typename Cell_const_handle>
class Connectivity_tester {
public:
  typedef boost::adjacency_matrix<boost::directedS>           Graph;
  typedef std::pair<Uint, Uint>                               Key;
  typedef std::vector<Key>                                    Delta;
  typedef std::tuple<boost::optional<Cell_const_handle>,
                     Delta, Delta, bool,
                     std::vector<Cell_const_handle>*>         Step;
  typedef std::vector<Step>                                   Lookahead;

  typedef std::vector<Uint>                                   Components;
  typedef std::pair<Uint, Components>                         Component_data;
  typedef std::pair<Cell_const_handle, Component_data>        Solution;
  typedef std::list<Solution*>                                Solutions;

private:
  Uint m_number_of_pieces;
  Uint m_number_of_tests;
  Graph m_graph;
  Solutions &m_solutions;
  Lookahead m_lookahead;
  bool m_use_naive_mult;
  
  // Number of tests below which recursion stops
  static const int base_number_of_tests = 8;
  // Reduce phase size by this factor for recursive calls
  static const int phase_reduction_factor = 8;


public:
  /*! Constructor */
  Connectivity_tester(Uint number_of_pieces, Graph g, Solutions &solutions,
                      Cell_const_handle c, bool use_naive_mult) :
    m_number_of_pieces(number_of_pieces),
    m_number_of_tests(0),
    m_graph(g),
    m_solutions(solutions),
    m_use_naive_mult(use_naive_mult)
  {
    Delta d;
    test(c, d, d);
  }

  void test(Cell_const_handle c, Delta add, Delta remove)
  {

    // If the graph is the same as in the last step (also makes sure prev step
    // wasn't an update)
    if (add.size() == 0 && remove.size() == 0 && m_lookahead.size()
        && std::get<0>(m_lookahead.back()))
    {
        auto prev_step = m_lookahead.back();
        if (std::get<4>(prev_step) == NULL){
          std::get<4>(prev_step) = new std::vector<Cell_const_handle>();
        }
        std::get<4>(prev_step)->push_back(c);
        return;
    }


    ++m_number_of_tests;

    std::vector<Cell_const_handle>* cells_with_same_sol = NULL;
    Step s = std::make_tuple(boost::optional<Cell_const_handle>{c}, add, remove,
                             false, cells_with_same_sol);
    m_lookahead.push_back(s);

    int phase_size = (m_number_of_pieces/phase_reduction_factor)
                     + (m_number_of_pieces % phase_reduction_factor != 0);

    if (m_number_of_tests == phase_size) {
      process_top_level_phase(m_number_of_tests);
    }
  }

  void update(Delta add, Delta remove)
  {
    if (add.size() == 0 && remove.size() == 0){
      return;
    }
    std::vector<Cell_const_handle>* cells_with_same_sol = NULL;
    Step s = std::make_tuple(boost::optional<Cell_const_handle>{}, add, remove,
                             false, cells_with_same_sol);
    m_lookahead.push_back(s);
  }

  void finish()
  {
    if (m_number_of_tests != 0){
      process_top_level_phase(m_number_of_tests);
    }
  }


private:
  void update_graph(Graph &g, Step &s)
  {
    Delta &add = std::get<1>(s), &remove = std::get<2>(s);

    for (auto key_it = add.begin(); key_it != add.end(); ++key_it) {
      boost::add_edge(key_it->second, key_it->first, g);
    }

    for (auto key_it = remove.begin(); key_it != remove.end(); ++key_it) {
      boost::remove_edge(key_it->second, key_it->first, g);
    }
  }

  bool is_connected(Graph& g)
  {
    Components components;
    components.resize(boost::num_vertices(g));
    Uint num_components;

  #if defined(_MSC_VER)
    num_components =
    boost::strong_components
    (g,
     boost::make_iterator_property_map(components.begin(),
                                       boost::get(boost::vertex_index, g),
                                       components[0]));
  #else
    num_components = boost::strong_components(g, &components[0]);
  #endif
    //std::cout << "Nump comp: " << num_components << std::endl;
    return num_components <= 1;
  }


  void compute_solution(Graph& g, Step& s)
  {
    Components components;
    components.resize(boost::num_vertices(g));
    Uint num_components;

  #if defined(_MSC_VER)
    num_components =
    boost::strong_components
    (g,
     boost::make_iterator_property_map(components.begin(),
                                       boost::get(boost::vertex_index, g),
                                       components[0]));
  #else
    num_components = boost::strong_components(g, &components[0]);
  #endif
    if (num_components > 1) {
      auto solution = new Solution;
      (solution->second).second = components;
      (solution->second).first = num_components;
      solution->first = *(std::get<0> (s));
      m_solutions.push_front(solution);

      auto cells_with_same_sol = std::get<4> (s);
      if (cells_with_same_sol != NULL) {
        for (auto cell_it = cells_with_same_sol->begin();
             cell_it != cells_with_same_sol->end(); ++cell_it) {
          auto solution = new Solution;
          (solution->second).second = components;
          (solution->second).first = num_components;
          solution->first = *(std::get<0> (s));
          m_solutions.push_front(solution);
        }
      }

    }
  }

  void process_top_level_phase(int phase_size)
  {
    // i.e. will the next call be the base case
    bool is_next_call_base = (phase_size <= base_number_of_tests);

    if (is_next_call_base) {
      // process_base_phase updates the graph while process_base does't
      process_base_phase(m_graph, 0, m_lookahead.size(), true);
    }
    else {
      process_phase(m_graph, 0, m_lookahead.size(), phase_size);
      for (auto step_it = m_lookahead.begin(); step_it != m_lookahead.end();
           ++step_it)
      {
        update_graph(m_graph, *step_it);

        bool is_connected_now = std::get<3> (*step_it);
        auto cell_handle = std::get<0> (*step_it);
        if (cell_handle && !is_connected_now) {
          compute_solution(m_graph, *step_it);
        }
      }
    }

    // Clean up
    for (auto step_it = m_lookahead.begin(); step_it != m_lookahead.begin();
         ++step_it)
    {
      if (std::get<4> (*step_it) != NULL) {
        delete std::get<4> (*step_it);
      }
    }
    m_number_of_tests = 0;
    m_lookahead.clear();
  }

  void process_phase(Graph &g, int start, int end, int phase_size)
  {
    std::vector<int> active_vertices(boost::num_vertices(g));

    // Mark active vertices for this phase
    for (auto step_it = m_lookahead.begin() + start;
         step_it != m_lookahead.begin() + end; ++step_it)
    {
      for (int i=1; i!=3; ++i){
        Delta& delta = (i==1) ? std::get<1> (*step_it) : std::get<2> (*step_it);
        for (auto key_it = delta.begin(); key_it != delta.end(); ++key_it) {
          active_vertices.at(key_it->first) = 1;
          active_vertices.at(key_it->second) = 1;
        }
      }
    }

    // Compute G+p (= Gp U E^A)
    std::vector<Uint> active_vert_indices;
    for (int i=0; i<active_vertices.size(); ++i) {
      if (active_vertices.at(i)) {
        active_vert_indices.push_back(i);
      }
    }

    Graph g_p_plus = g;
    for (auto vert_it = active_vert_indices.begin();
         vert_it !=active_vert_indices.end(); ++vert_it)
    {
      for (auto vert_it2 = active_vert_indices.begin();
           vert_it2 !=active_vert_indices.end(); ++vert_it2)
      {
        boost::add_edge(*vert_it, *vert_it2, g_p_plus);
      }
    }

    if (!is_connected(g_p_plus)) {
      // All the graphs in the phase are not connected (and they are already
      // marked as such)
      return;
    }

    // Compute G-p (= Gp \ E^A)
    Graph g_p_minus = g;
    for (auto vert_it = active_vert_indices.begin();
         vert_it != active_vert_indices.end(); ++vert_it)
    {
      for (auto vert_it2 = active_vert_indices.begin();
           vert_it2 != active_vert_indices.end(); ++vert_it2)
      {
        if (boost::edge(*vert_it, *vert_it2, g_p_minus).second) {
          boost::remove_edge(*vert_it, *vert_it2, g_p_minus);
        }
      }
    }

    transitive_closure(g_p_minus, m_use_naive_mult);

    // Compute H_p
    Graph h_p(active_vert_indices.size());
    for (int i=0; i<active_vert_indices.size(); ++i) {
      for (int j=0; j<active_vert_indices.size(); ++j) {
        if (boost::edge(active_vert_indices.at(i), active_vert_indices.at(j),
                        g_p_minus).second ||
            boost::edge(active_vert_indices.at(i),
                        active_vert_indices.at(j), g).second)
        {
          boost::add_edge(i, j, h_p);
        }
      }
    }

    // Update active_vertices to hold their indices in the subgraph
    for (int i = 0; i < active_vert_indices.size(); ++i) {
      active_vertices.at(active_vert_indices.at(i)) = i;
    }

    // Solve recursively by dividing into sub-phases
    int c = 0, prev_end = start;
    int rec_phase_size = (phase_size/phase_reduction_factor)
                         + (phase_size % phase_reduction_factor != 0);
    bool is_next_rec_call_base = (rec_phase_size <= base_number_of_tests);

    for (int j = start; j<end; ++j){
      // Adjust Delta keys to match subgraph indices (in H_p)
      for (int i=1; i!=3; ++i) {
        Delta& delta = (i==1) ? std::get<1>(m_lookahead.at(j)) :
          std::get<2> (m_lookahead.at(j));
        for (auto key_it = delta.begin(); key_it != delta.end(); ++key_it) {
          key_it->first = active_vertices.at(key_it->first);
          key_it->second = active_vertices.at(key_it->second);
        }
      }

      if (std::get<0> (m_lookahead.at(j))) {
        ++c;
        if (c == rec_phase_size){
          if (is_next_rec_call_base){
            process_base_phase(h_p, prev_end, j+1, false);
          }
          else {
            process_phase(h_p, prev_end, j+1, c);
          }

          // Prepare for next phase

          // Recursive call won't update H_p (for next phase) if it's not the
          // base call
          if (!is_next_rec_call_base) {
            for (; prev_end<=j; ++prev_end) {
               update_graph(h_p, m_lookahead.at(prev_end));
            }
          }
          else {
            prev_end = j+1;
          }

          c = 0;
        }
      }

      else if (j+1 == end) {
        if (c <= base_number_of_tests) {
          process_base_phase(h_p, prev_end, j+1, false);
        }
        else {
          process_phase(h_p, prev_end, j+1, c);
        }
      }
    }

    // Adjust Delta keys back
    for (int j = start; j<end; ++j) {
      for (int i = 1; i != 3; ++i){
        Delta& delta = (i == 1) ? std::get<1> (m_lookahead.at(j)) :
          std::get<2> (m_lookahead.at(j));
        for (auto key_it = delta.begin(); key_it != delta.end(); ++key_it) {
          key_it->first = active_vert_indices.at(key_it->first);
          key_it->second = active_vert_indices.at(key_it->second);
        }
      }
    }
  }

  void process_base_phase(Graph &g, int start, int end, bool get_solution)
  {
    for (auto step_it = m_lookahead.begin() + start;
         step_it != m_lookahead.begin() + end; ++step_it)
    {
      update_graph(g, *step_it);

      if (std::get<0>(*step_it)) {
        if (get_solution){
          compute_solution(g, *step_it);
        }

        // In this case the graph is part of a subproblem (for which we only
        // need to know connectivity and not find components)
        else if (is_connected(g)) {
          std::get<3> (*step_it) = true;
        }
      }
    }
  }
};

SGAL_END_NAMESPACE

#endif
