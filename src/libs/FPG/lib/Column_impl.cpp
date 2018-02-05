// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"
#include "SGAL/io_vector2f.hpp"

#include "FPG/Column.hpp"

SGAL_BEGIN_NAMESPACE

//! Associate the node engine with a scene graph.
void Column::add_to_scene(Scene_graph* scene_graph)
{ m_scene_graph = scene_graph; }

//! \brief cleans the geometry.
void Column::clean_geometry()
{
  typedef boost::shared_ptr<Indexed_face_set>       Shared_indexed_face_set;
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  set_geometry(ifs);

  const auto& location = get_location();
  const auto& outer_boundary = get_outer_boundary();
  auto size = outer_boundary.size();
  if (0 == size) {
    Shape::clean_geometry();
    return;
  }
  auto num_vertices = 2 * size;
  auto num_triangles = 4 * (size - 1);

  // Construct coord array
  typedef boost::shared_ptr<Coord_array_3d>         Shared_coord_array_3d;
  auto* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  size_t i(0);
  float z(0);
  for (auto it = outer_boundary.begin(); it != outer_boundary.end(); ++it) {
    auto x = (*it)[0] + location[0];
    auto y = (*it)[1] + location[1];
    (*coords)[i].set(x, y, z);
    (*coords)[size + i++].set(x, y, z + get_height());
  }

  // Construct coord indices
  Facet_indices facet_indices;
  auto& indices = boost::get<Triangle_indices>(facet_indices);
  indices.resize(num_triangles);

  // Side:
  size_t k(0);
  for (i = 0; i < size-1; ++i) {
    auto ll = i;
    auto ul = ll + size;
    auto lr = ll + 1;
    auto ur = ul + 1;
    k = add_triangle_indices(k, indices, ll, lr, ur, ul);
  }
  auto ll = i;
  auto ul = ll + size;
  auto lr = 0;
  auto ur = size;
  k = add_triangle_indices(k, indices, ll, lr, ur, ul);

  // Bottom & top:
  // Triangulation.

  typedef CGAL::Exact_predicates_exact_constructions_kernel     Kernel;
  typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, Kernel>
                                                                Vb;
  typedef CGAL::Constrained_triangulation_face_base_2<Kernel>   Fb;
  typedef CGAL::Triangulation_data_structure_2<Vb, Fb>          Tds;
  typedef CGAL::Exact_predicates_tag                            Itag;
  typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, Tds, Itag>
    Triangulation;

  Triangulation tri;

  typedef Triangulation::Vertex_handle          Vertex_handle;
  typedef Triangulation::Point                  Point;

  // Insert outer boundary points:
  std::vector<Vertex_handle> vertex_handles(size);
  i = 0;
  for (auto it = outer_boundary.begin(); it != outer_boundary.end(); ++it) {
    Point p((*it)[0], (*it)[1]);
    vertex_handles[i] = tri.insert(Point((*it)[0], (*it)[1]));
    vertex_handles[i]->info() = i;
    ++i;
  }
  SGAL_assertion(tri.number_of_vertices() != 0);

  // Insert outer boundary segments:
  for (i = 0; i < size-1; ++i)
    tri.insert_constraint(vertex_handles[i], vertex_handles[i+1]);
  tri.insert_constraint(vertex_handles[i++], vertex_handles[0]);

  // Traverse triangles
  for (auto it = tri.finite_faces_begin(); it != tri.finite_faces_end(); ++it) {
    Vertex_handle vh0 = it->vertex(0);
    Vertex_handle vh1 = it->vertex(1);
    Vertex_handle vh2 = it->vertex(2);

    // Bottom:
    indices[k][0] = vh2->info();
    indices[k][1] = vh1->info();
    indices[k][2] = vh0->info();
    ++k;

    // Top:
    indices[k][2] = vh2->info() + size;
    indices[k][1] = vh1->info() + size;
    indices[k][0] = vh0->info() + size;
    ++k;
  }
  SGAL_assertion(k == indices.size());

  // Update the Indexed_face_set geometry node:
  ifs->add_to_scene(m_scene_graph);
  ifs->set_facet_coord_indices(std::move(facet_indices));
  ifs->set_coord_array(shared_coords);
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(num_triangles);
  ifs->set_make_consistent(true);
  ifs->set_solid(true);

  Shape::clean_geometry();
}

SGAL_END_NAMESPACE
