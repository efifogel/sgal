// Copyright (c) 2018 Israel.
// All rights reserved to Xenia Optimal Ltd.

#include "SGAL/basic.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/add_triangle_indices.hpp"

#include "FPG/Column.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief cleans the geometry.
void Column::clean_geometry()
{
  typedef boost::shared_ptr<Indexed_face_set>       Shared_indexed_face_set;
  Shared_indexed_face_set ifs(new Indexed_face_set);
  SGAL_assertion(ifs);
  set_geometry(ifs);

  const auto& outer_boundary = get_outer_boundary();
  auto size = outer_boundary.size();
  auto num_vertices = 2 * size;
  auto num_triangles = 4 * (size - 1);

  // Construct coord array
  typedef boost::shared_ptr<Coord_array_3d>         Shared_coord_array_3d;
  auto* coords = new Coord_array_3d(num_vertices);
  Shared_coord_array_3d shared_coords(coords);
  size_t i(0);
  float z(0);
  for (auto it = outer_boundary.begin(); it != outer_boundary.end(); ++it) {
    auto x = (*it)[0];
    auto y = (*it)[1];
    (*coords)[i].set(x, y, z);
    (*coords)[i + size].set(x, y, z + get_height());
  }

  // Construct coord indices
  auto& indices = ifs->get_empty_triangle_coord_indices();
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

  // Bottom:
  size_t anchor(0);
  for (i = 0; i < size-2; ++i) {
    indices[k][0] = anchor;
    indices[k][1] = anchor + size - i - 1;
    indices[k][2] = anchor + size - i - 2;
    ++k;
  }

  // Top:
  anchor = size;
  for (i = 0; i < size-2; ++i) {
    indices[k][0] = anchor;
    indices[k][1] = anchor + i + 1;
    indices[k][2] = anchor + i + 2;
    ++k;
  }

  // Update the Indexed_face_set geometry node:
  ifs->set_primitive_type(Geo_set::PT_TRIANGLES);
  ifs->set_num_primitives(num_triangles);
  ifs->set_make_consistent(true);
  ifs->set_solid(true);
  ifs->set_coord_array(shared_coords);

  Shape::clean_geometry();
}

SGAL_END_NAMESPACE
