#include "SEGO/Ego_voxels_vertex_list_graph_utils.hpp"
#include "SEGO/Ego_voxels.hpp"

#include <boost/tuple/tuple_comparison.hpp>

SGAL_BEGIN_NAMESPACE

bool
Is_inside_voxels::operator() (const vertex_descriptor& point) const {
  if (point.get<0>() < 0)
    return false;
  if (point.get<1>() < 0)
    return false;
  if (point.get<2>() < 0)
    return false;
  
  if (point.get<0>() >= long(m_dim.get<0>()))
    return false;
  if (point.get<1>() >= long(m_dim.get<1>()))
    return false;
  if (point.get<2>() >= long(m_dim.get<2>()))
    return false;
  
  return true;
}

bool Is_inside_voxels::operator() (const edge_descriptor& edge) const {
  return (*this)(edge.first) && (*this)(edge.second);
}

bool Is_filling_identical::operator()
  (const edge_descriptor& edge) const {
  
  return (m_voxels->is_filled(edge.first) ==
          m_voxels->is_filled(edge.second));
}

bool Not_self_edge::operator() (const edge_descriptor& edge) const {
  return (edge.first != edge.second);
}

bool Is_same_level::operator() (const edge_descriptor& edge) const {
  return (edge.first.get<2>() == edge.second.get<2>());
}

bool Is_orthogonal::operator() (const edge_descriptor& edge) const {
  bool x_identical = (edge.first.get<0>() == edge.second.get<0>());
  bool y_identical = (edge.first.get<1>() == edge.second.get<1>());
  bool z_identical = (edge.first.get<2>() == edge.second.get<2>());

  if (x_identical && y_identical) return true;
  if (y_identical && z_identical) return true;
  if (z_identical && x_identical) return true;

  return false;
}

neighborhood_iterator::neighborhood_iterator()
    : m_center(0, 0, 0), m_null(true) {
}

neighborhood_iterator::neighborhood_iterator(const vertex_descriptor& c)
    : m_center(c), m_null(false) {
  long x = c.get<0>();
  long y = c.get<1>();
  long z = c.get<2>();
  
  long x_b = std::max(x-1, 0L);
  long y_b = std::max(y-1, 0L);
  long z_b = std::max(z-1, 0L);
  
  m_begin = vertex_iterator(x_b, x+2, y_b, y+2, z_b, z+2);
  m_end = vertex_iterator(x+2, x+2, y+2, y+2, z+2, z+2);
}

neighborhood_iterator::Facade::reference
neighborhood_iterator::dereference() const {
  return edge_descriptor (m_center, *m_begin);
}

bool neighborhood_iterator::equal(const neighborhood_iterator& o) const {

  if (this->is_end() && o.is_end())
    return true;
  
  return (this->m_center == o.m_center) &&
    (this->m_begin == o.m_begin) &&
    (this->m_end == o.m_end);
}

void neighborhood_iterator::increment() {
  ++m_begin;
}

bool neighborhood_iterator::is_end() const {
  if (m_null)
    return true;

  return (m_begin == m_end);
}



SGAL_END_NAMESPACE

