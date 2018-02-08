// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_2d.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_strip_triangle_indices.hpp"
#include "SGAL/io_vector2f.hpp"

#include "FPG/Wall.hpp"

SGAL_BEGIN_NAMESPACE

//! Refractor the triangulation code out of here.
struct Face_info {
  Face_info() {}
  int nesting_level;
  bool in_domain() { return nesting_level % 2 == 1; }
};

template <typename CDT>
void mark_domains(CDT& ct,
                  typename CDT::Face_handle start,
                  int index,
                  std::list<typename CDT::Edge>& border )
{
  if (start->info().nesting_level != -1) return;

  std::list<typename CDT::Face_handle> queue;
  queue.push_back(start);
  while (! queue.empty()) {
    typename CDT::Face_handle fh = queue.front();
    queue.pop_front();
    if (fh->info().nesting_level == -1){
      fh->info().nesting_level = index;
      for (int i = 0; i < 3; i++){
        typename CDT::Edge e(fh,i);
        typename CDT::Face_handle n = fh->neighbor(i);
        if(n->info().nesting_level == -1){
          if (ct.is_constrained(e)) border.push_back(e);
          else queue.push_back(n);
        }
      }
    }
  }
}

//explore set of facets connected with non constrained edges,
//and attribute to each such set a nesting level.
//We start from facets incident to the infinite vertex, with a nesting
//level of 0. Then we recursively consider the non-explored facets incident
//to constrained edges bounding the former set and increase the nesting level by 1.
//Facets in the domain are those with an odd nesting level.
template <typename CDT>
void mark_domains(CDT& cdt)
{
  for (auto it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
    it->info().nesting_level = -1;
  }
  std::list<typename CDT::Edge> border;
  mark_domains(cdt, cdt.infinite_face(), 0, border);
  while (! border.empty()) {
    typename CDT::Edge e = border.front();
    border.pop_front();
    typename CDT::Face_handle n = e.first->neighbor(e.second);
    if (n->info().nesting_level == -1) {
      mark_domains(cdt, n, e.first->info().nesting_level+1, border);
    }
  }
}

//! \brief processes change of structure.
void Wall::structure_changed(const Field_info* field_info)
{
  clear_coord_array();
  clear_facet_coord_indices();
  field_changed(field_info);
}

//! \brief cleans (generate) the coordinate array.
void Wall::clean_coords()
{
  // Calculate sizes:
  const auto outer_ccb = get_outer_ccb();
  if (! outer_ccb) return;

  auto outer_ccb_size = outer_ccb->size();
  if (0 == outer_ccb_size) return;

  auto ccb_size = outer_ccb_size;

  const auto& inner_ccbs = get_inner_ccbs();
  for (auto inner_ccb : inner_ccbs) ccb_size += inner_ccb->size();

  auto num_vertices = 2 * ccb_size;
  auto num_holes = inner_ccbs.size();
  auto num_horizontal_triangles = ccb_size - 2 + 2 * num_holes;
  auto num_triangles = num_vertices;
  if (get_top()) num_triangles += num_horizontal_triangles;
  if (get_bottom()) num_triangles += num_horizontal_triangles;

  // Construct coord array
  typedef boost::shared_ptr<Coord_array_3d>         Shared_coord_array_3d;
  auto* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);

  const auto& location = get_location();
  size_t i(0);
  float z(0);
  // Insert bottom coordinates:
  for (auto it = outer_ccb->begin(); it != outer_ccb->end(); ++it) {
    auto x = (*it)[0] + location[0];
    auto y = (*it)[1] + location[1];
    (*coords)[i].set(x, y, z);
    (*coords)[ccb_size + i++].set(x, y, z + get_height());
  }
  for (auto inner_ccb : inner_ccbs) {
    auto size = inner_ccb->size();
    for (auto it = inner_ccb->begin(); it != inner_ccb->end(); ++it) {
      auto size = inner_ccb->size();
      auto x = (*it)[0] + location[0];
      auto y = (*it)[1] + location[1];
      (*coords)[i].set(x, y, z);
      (*coords)[ccb_size + i++].set(x, y, z + get_height());
    }
  }
  SGAL_assertion(i == ccb_size);

  set_coord_array(shared_coords);
  coord_content_changed(get_field_info(COORD_ARRAY));
}

void Wall::clean_facet_coord_indices()
{
  m_dirty_coord_indices = true;
  m_dirty_facet_coord_indices = false;

  // Calculate sizes:
  const auto outer_ccb = get_outer_ccb();
  if (! outer_ccb) return;

  auto outer_ccb_size = outer_ccb->size();
  if (0 == outer_ccb_size) return;

  auto ccb_size = outer_ccb_size;

  const auto& inner_ccbs = get_inner_ccbs();
  for (auto inner_ccb : inner_ccbs) ccb_size += inner_ccb->size();

  auto num_vertices = 2 * ccb_size;
  auto num_holes = inner_ccbs.size();
  auto num_horizontal_triangles = ccb_size - 2 + 2 * num_holes;
  auto num_triangles = num_vertices;
  if (get_top()) num_triangles += num_horizontal_triangles;
  if (get_bottom()) num_triangles += num_horizontal_triangles;

  // Construct coord indices:
  Facet_indices facet_indices;
  auto& indices = boost::get<Triangle_indices>(facet_indices);
  indices.resize(num_triangles);

  // Outer surfaces:
  size_t k(0);
  k = add_strip_triangle_indices(indices, k, 0, ccb_size, outer_ccb_size);

  // Inner surfaces:
  size_t offset(outer_ccb_size);
  for (auto inner_ccb : inner_ccbs) {
    k = add_strip_triangle_indices(indices, k, offset, ccb_size + offset,
                                   inner_ccb->size());
    offset += inner_ccb->size();
  }

  // Bottom & top:
  if (get_top() || get_bottom()) {
  // Triangulation.
    typedef CGAL::Exact_predicates_exact_constructions_kernel   Kernel;
    typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, Kernel>
                                                                Vb;
    typedef CGAL::Triangulation_face_base_with_info_2<Face_info, Kernel>
                                                                Fbb;
    typedef CGAL::Constrained_triangulation_face_base_2<Kernel, Fbb>
                                                                Fb;
    typedef CGAL::Triangulation_data_structure_2<Vb, Fb>        Tds;
    typedef CGAL::Exact_predicates_tag                          Itag;
    typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, Tds, Itag>
                                                                CDT;

    CDT cdt;

    typedef CDT::Vertex_handle                            Vertex_handle;
    typedef CDT::Point                                    Point;

    // Insert outer ccb points:
    std::vector<Vertex_handle> vertex_handles(ccb_size);
    size_t i(0);
    for (auto it = outer_ccb->begin(); it != outer_ccb->end(); ++it) {
      Point p((*it)[0], (*it)[1]);
      vertex_handles[i] = cdt.insert(Point((*it)[0], (*it)[1]));
      vertex_handles[i]->info() = i;
      ++i;
    }
    // Insert inner ccbs points:
    for (auto inner_ccb : inner_ccbs) {
      for (auto it = inner_ccb->begin(); it != inner_ccb->end(); ++it) {
        Point p((*it)[0], (*it)[1]);
        vertex_handles[i] = cdt.insert(Point((*it)[0], (*it)[1]));
        vertex_handles[i]->info() = i;
        ++i;
      }
    }
    SGAL_assertion(cdt.number_of_vertices() == ccb_size);

    // Insert outer ccb segments:
    for (i = 0; i < outer_ccb->size()-1; ++i)
      cdt.insert_constraint(vertex_handles[i], vertex_handles[i+1]);
    cdt.insert_constraint(vertex_handles[i], vertex_handles[0]);

    // Insert inner ccbs segments:
    offset = outer_ccb_size;
    for (auto inner_ccb : inner_ccbs) {
      for (i = 0; i < inner_ccb->size()-1; ++i)
        cdt.insert_constraint(vertex_handles[offset+i], vertex_handles[offset+i+1]);
      cdt.insert_constraint(vertex_handles[offset+i], vertex_handles[offset]);
      offset += inner_ccb->size();
    }

    // Mark facets that are inside the domain bounded by the polygon
    mark_domains(cdt);

    // Traverse triangles
    for (auto it = cdt.finite_faces_begin(); it != cdt.finite_faces_end(); ++it) {
      if (! it->info().in_domain()) continue;

      Vertex_handle vh0 = it->vertex(0);
      Vertex_handle vh1 = it->vertex(1);
      Vertex_handle vh2 = it->vertex(2);

      // Bottom:
      indices[k][0] = vh2->info();
      indices[k][1] = vh1->info();
      indices[k][2] = vh0->info();
      ++k;

      // Top:
      indices[k][2] = vh2->info() + ccb_size;
      indices[k][1] = vh1->info() + ccb_size;
      indices[k][0] = vh0->info() + ccb_size;
      ++k;
    }
  }
  SGAL_assertion(k == indices.size());

  // Update the Indexed_face_set geometry base container:
  set_facet_coord_indices(std::move(facet_indices));
  set_primitive_type(Geo_set::PT_TRIANGLES);
  set_num_primitives(num_triangles);
  set_make_consistent(true);
  set_solid(get_top() && get_bottom());
}

SGAL_END_NAMESPACE
