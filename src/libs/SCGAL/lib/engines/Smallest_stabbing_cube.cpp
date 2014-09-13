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

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <string>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>

#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Coord_transformer.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Smallest_stabbing_cube.hpp"
#include "SCGAL/compute_planes.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Smallest_stabbing_cube::s_prototype(nullptr);
const std::string Smallest_stabbing_cube::s_tag = "SmallestStabbingCube";

Boolean Smallest_stabbing_cube::s_def_enabled(true);

/*! Register to the container factory. This will enable automatic creation
 * through the name provided as a parameter.
 */
REGISTER_TO_FACTORY(Smallest_stabbing_cube, "Smallest_stabbing_cube");

//! \brief constructor from prototype.
Smallest_stabbing_cube::Smallest_stabbing_cube(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_changed(false),
  m_execute(false)
{}

//! \brief sets the attributes of this object.
void Smallest_stabbing_cube::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  for(auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      m_enabled = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Sets the multi-container attributes of this node:
  for (auto mcai = elem->multi_cont_attrs_begin();
       mcai != elem->multi_cont_attrs_end(); ++mcai)
  {
    const std::string& name = elem->get_name(mcai);
    auto& cont_list = elem->get_value(mcai);
    if (name == "geometries") {
      for (auto ci = cont_list.begin(); ci != cont_list.end(); ci++) {
        Element::Shared_container cont = *ci;
        auto coords = boost::dynamic_pointer_cast<Coord_transformer>(cont);
        if (coords) m_coord_nodes.push_back(coords);
        else
          std::cerr << "Invalid " << s_tag << " geometry nodes!" << std::endl;
      }

      elem->mark_delete(mcai);
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the attributes of this node.
void Smallest_stabbing_cube::init_prototype()
{
  if (s_prototype)
    return;

  s_prototype = new Container_proto();

  Execution_function exec_func =
    static_cast<Execution_function>(&Smallest_stabbing_cube::execute);

  // Add the field-info records to the prototype:
  // enabled
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>
    (&Smallest_stabbing_cube::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled",
                                          RULE_EXPOSED_FIELD,
                                          enabled_func,
                                          s_def_enabled, exec_func));

  // changed
  Boolean_handle_function changed_func =
    static_cast<Boolean_handle_function>
    (&Smallest_stabbing_cube::changed_handle);
  s_prototype->add_field_info(new SF_bool(CHANGED, "changed",
                                          RULE_EXPOSED_FIELD, changed_func));

  // execute
  Boolean_handle_function execute_func =
    static_cast<Boolean_handle_function>
    (&Smallest_stabbing_cube::execute_handle);
  s_prototype->add_field_info(new SF_bool(EXECUTE, "execute",
                                          RULE_EXPOSED_FIELD, execute_func,
                                          exec_func));

  // geometries
  Shared_container_array_handle_function geometries_func =
    reinterpret_cast<Shared_container_array_handle_function>
    (&Smallest_stabbing_cube::coord_nodes_handle);
  s_prototype->add_field_info(new MF_shared_container(GEOMETRIES, "geometries",
                                                      RULE_EXPOSED_FIELD,
                                                      geometries_func,
                                                      exec_func));

  // coord
  Shared_container_handle_function coord_changed_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Smallest_stabbing_cube::result_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_CHANGED,
                                                      "coord_changed",
                                                      RULE_EXPOSED_FIELD,
                                                      coord_changed_func));
}

//! \brief deletes the node prototype.
void Smallest_stabbing_cube::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Smallest_stabbing_cube::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! \brief transforms the input vertices and store the results in the output
 * vertices
 */
void Smallest_stabbing_cube::execute(const Field_info* /* field_info */)
{
  if (!m_enabled) return;

  if (m_coord_nodes.size() == 0) return;

  // const int X = 0;
  // const int Y = 1;
  // const int Z = 2;
  const int W = 3;

  Program lp(CGAL::SMALLER, true, 0, false, 0);
  lp.set_c(W, 1);
  int count = 0;

  //Create a kernel
  Smallest_stabbing_cube::Kernel kernel;

  //Build unit cube 1
  Polyhedron cube1;
  std::vector<Point_3> cube1Points;
  cube1Points.push_back(Point_3(-1,-1,-1));
  cube1Points.push_back(Point_3(-1,-1,1));
  cube1Points.push_back(Point_3(-1,1,-1));
  cube1Points.push_back(Point_3(-1,1,1));
  cube1Points.push_back(Point_3(1,-1,-1));
  cube1Points.push_back(Point_3(1,-1,1));
  cube1Points.push_back(Point_3(1,1,-1));
  cube1Points.push_back(Point_3(1,1,1));
  CGAL::convex_hull_3(cube1Points.begin(), cube1Points.end(), cube1);
  compute_planes(kernel, cube1, boost::true_type());
  merge_coplanar_facets(kernel, cube1, boost::true_type());

  //Build unit cube 2
  Polyhedron cube2;
  std::vector<Point_3> cube2Points;
  cube2Points.push_back(Point_3(-2,-2,-2));
  cube2Points.push_back(Point_3(-2,-2,2));
  cube2Points.push_back(Point_3(-2,2,-2));
  cube2Points.push_back(Point_3(-2,2,2));
  cube2Points.push_back(Point_3(2,-2,-2));
  cube2Points.push_back(Point_3(2,-2,2));
  cube2Points.push_back(Point_3(2,2,-2));
  cube2Points.push_back(Point_3(2,2,2));
  CGAL::convex_hull_3(cube2Points.begin(), cube2Points.end(), cube2);
  compute_planes(kernel, cube2, boost::true_type());
  merge_coplanar_facets(kernel, cube2, boost::true_type());

  // std::cout << "Starting stabbing cube..." << std::endl;
  // auto start = boost::posix_time::microsec_clock::local_time();

  // Create a polyhedron from each of our coordinates
#if (_MSC_VER <= 1600)
  BOOST_FOREACH(auto& obj, m_coord_nodes)
#else
  for(auto& obj : m_coord_nodes)
#endif
  {
    Polyhedron polyhedron;
    std::vector<Point_3> points;

    //std::cout << "Adding polyhedron..." << std::endl;
    if (obj->get_coord_array_changed()) {
      Shared_coord_array tmp = obj->get_coord_array_changed();
      boost::shared_ptr<Coord_array_3d> coord_array_changed =
        boost::static_pointer_cast<Coord_array_3d>(tmp);
      SGAL_assertion(coord_array_changed);

#if (_MSC_VER <= 1600)
      BOOST_FOREACH(auto& point, *coord_array_changed)
#else
      for (auto& point : *coord_array_changed)
#endif
      {
        auto p = Point_3(point.get(0), point.get(1), point.get(2));
        //std::cout << "Adding point: " << p << std::endl;
        points.push_back(p);
      }
    }
    else {
      Shared_coord_array tmp = obj->get_coord_array();
      boost::shared_ptr<Coord_array_3d> coord_array =
        boost::static_pointer_cast<Coord_array_3d>(tmp);
      SGAL_assertion(coord_array);

#if (_MSC_VER <= 1600)
      BOOST_FOREACH(auto& point, *coord_array)
#else
      for (auto& point : *coord_array)
#endif
      {
        auto p = Point_3(point.get(0), point.get(1), point.get(2));
        //std::cout << "Adding point: " << p << std::endl;
        points.push_back(p);
      }
    }

    CGAL::convex_hull_3(points.begin(), points.end(), polyhedron);
    compute_planes(kernel, polyhedron, boost::true_type());
    merge_coplanar_facets(kernel, polyhedron, boost::true_type());

    //Calculate minkowski sum with the two cubes
    auto sum1Tet1 = calculateSum(cube1, polyhedron);
    auto sum2Tet1 = calculateSum(cube2, polyhedron);

    //Add constraints for the linear programming
    addConstraints(sum1Tet1, sum2Tet1, lp, count);
  }

  //Now solve the linear programming
  Solution s = CGAL::solve_linear_program(lp, CGAL::Gmpq());

  //Get solution point
  auto p = s.variable_values_begin();
  auto x = CGAL::to_double(*p);
  auto y = CGAL::to_double(*(++p));
  auto z = CGAL::to_double(*(++p));
  auto dist = CGAL::to_double(*(++p));
  //auto dist = CGAL::to_double(s.objective_value());

  if ((s.status() == CGAL::QP_INFEASIBLE) || (dist <= 0)) {
    Coord_array_3d* coords = new Coord_array_3d((Uint)4);
    m_result = Shared_coord_array(coords);
    (*coords)[0].set(0, 0, 0);
    (*coords)[1].set(0.01f, 0, 0);
    (*coords)[2].set(0, 0.01f, 0);
    (*coords)[3].set(0, 0, 0.01f);
  }
  else {
    Coord_array_3d* coords = new Coord_array_3d((Uint)8);
    m_result = Shared_coord_array(coords);
    Float xplus = static_cast<Float>(x + dist);
    Float xminus = static_cast<Float>(x - dist);
    Float yplus = static_cast<Float>(y + dist);
    Float yminus = static_cast<Float>(y - dist);
    Float zplus = static_cast<Float>(z + dist);
    Float zminus = static_cast<Float>(z - dist);
    (*coords)[0].set(xplus, yplus, zplus);
    (*coords)[1].set(xplus, yplus, zminus);
    (*coords)[2].set(xplus, yminus, zplus);
    (*coords)[3].set(xplus, yminus, zminus);
    (*coords)[4].set(xminus, yplus, zplus);
    (*coords)[5].set(xminus, yplus, zminus);
    (*coords)[6].set(xminus, yminus, zplus);
    (*coords)[7].set(xminus, yminus, zminus);
  }

  // std::cout << "Finished stabbing cube." << std::endl;
  // std::cout << "Time: "
  //           << boost::posix_time::microsec_clock::local_time() - start
  //           << std::endl;

  //Send all events
  Field* coord_changed_field = get_field(COORD_CHANGED);
  if (coord_changed_field)
    coord_changed_field->cascade();

  m_changed = true;
  Field* changed_field = get_field(CHANGED);
  if (changed_field)
    changed_field->cascade();

  m_result->process_content_changed();
}

Smallest_stabbing_cube::Polyhedron
Smallest_stabbing_cube::calculateSum(const Polyhedron& p1,
                                     const Polyhedron& p2) const
{
  Exact_kernel kernel;

  Polyhedron p1Tmp = p1;
  Spherical_gaussian_map_for_ssc sgm1;
  Sgm_initializer_for_ssc initializer1(sgm1);
  initializer1(p1Tmp);

  Polyhedron p2Tmp = p2;
  Spherical_gaussian_map_for_ssc sgm2;
  Sgm_initializer_for_ssc initializer2(sgm2);
  initializer2(p2Tmp);

  Spherical_gaussian_map_for_ssc sgmSum;
  Sgm_overlay_traits_for_ssc sgmOverlay;
  sgmSum.minkowski_sum(sgm1, sgm2, sgmOverlay);

  //Calculate the sum as a polyhedron
  std::vector<Point_3> sumPoints;
  for(auto vit = sgmSum.vertices_begin(); vit != sgmSum.vertices_end(); ++vit)
  {
    // Vertices with boundary conditions may have degree 2. Skip them:
    if (vit->degree() < 3)
      continue;

    auto hec = vit->incident_halfedges();
    auto begin_hec = hec;
    do
    {
      sumPoints.push_back((*hec).face()->point());
      ++hec;
    } while (hec != begin_hec);
  }

  Polyhedron sum;
  CGAL::convex_hull_3(sumPoints.begin(), sumPoints.end(), sum);
  compute_planes(kernel, sum, boost::true_type());
  merge_coplanar_facets(kernel, sum, boost::true_type());

  return sum;
}

void Smallest_stabbing_cube::addConstraints(const Polyhedron& minkCube1,
                                            const Polyhedron& minkCube2,
                                            Program& lp, int& count)
{
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  const int W = 3;

  //std::cout << "Planes of cube1: " << std::endl;
  std::vector<Plane_3> sum1Planes;
  for(auto it = minkCube1.facets_begin(); it != minkCube1.facets_end(); ++it)
  {
    sum1Planes.push_back(it->plane());
    //std::cout << "Plane: " << it->plane() << std::endl;
  }

  //std::cout << "Planes of cube2: " << std::endl;
  std::vector<Plane_3> sum2Planes;
  for(auto it = minkCube2.facets_begin(); it != minkCube2.facets_end(); ++it)
  {
    sum2Planes.push_back(it->plane());
    //std::cout << "Plane: " << it->plane() << std::endl;
  }

  for(size_t i = 0; i < sum1Planes.size(); ++i)
  {
    auto a1 = sum1Planes[i].a();
    auto b1 = sum1Planes[i].b();
    auto c1 = sum1Planes[i].c();
    auto d1 = sum1Planes[i].d();

    //Look for the matching index in the other vector
    int found = 0;
    int res(0);
    for(size_t j = 0; j < sum2Planes.size(); ++j)
    {
      auto a2 = sum2Planes[j].a();
      auto b2 = sum2Planes[j].b();
      auto c2 = sum2Planes[j].c();
      auto d2 = sum2Planes[j].d();

      //Find first non zero coefficient in each
      CGAL::Gmpq norm1;
      if (a1 != 0)
        norm1 = a1;
      else if (b1 != 0)
        norm1 = b1;
      else
        norm1 = c1;

      CGAL::Gmpq norm2;
      if (a2 != 0)
        norm2 = a2;
      else if (b2 != 0)
        norm2 = b2;
      else
        norm2 = c2;

      //Compare
      auto a1norm = a1 / norm1;
      auto b1norm = b1 / norm1;
      auto c1norm = c1 / norm1;
      auto d1norm = d1 / norm1;
      auto a2norm = a2 / norm2;
      auto b2norm = b2 / norm2;
      auto c2norm = c2 / norm2;
      auto d2norm = d2 / norm2;

      if (a1norm == a2norm &&
          b1norm == b2norm &&
          c1norm == c2norm)
      {
        //Get a point on first plane
        CGAL::Gmpq x, y, z;
        if (a1norm != 0)
        {
          y = 0;
          z = 0;
          x = -d1norm;
        }
        else if (b1norm != 0)
        {
          x = 0;
          z = 0;
          y = -d1norm;
        }
        else
        {
          x = 0;
          y = 0;
          z = -d1norm;
        }

        //Check if some transformation of it is on the other plane
        if (((x + 1)*a2norm + (y + 1)*b2norm + (z + 1)*c2norm + d2norm == 0) ||
            ((x + 1)*a2norm + (y + 1)*b2norm + (z - 1)*c2norm + d2norm == 0) ||
            ((x + 1)*a2norm + (y - 1)*b2norm + (z + 1)*c2norm + d2norm == 0) ||
            ((x + 1)*a2norm + (y - 1)*b2norm + (z - 1)*c2norm + d2norm == 0) ||
            ((x - 1)*a2norm + (y + 1)*b2norm + (z + 1)*c2norm + d2norm == 0) ||
            ((x - 1)*a2norm + (y + 1)*b2norm + (z - 1)*c2norm + d2norm == 0) ||
            ((x - 1)*a2norm + (y - 1)*b2norm + (z + 1)*c2norm + d2norm == 0) ||
            ((x - 1)*a2norm + (y - 1)*b2norm + (z - 1)*c2norm + d2norm == 0))
        {
          found++;
          res = j;
        }
      }
    }

    assert(found == 1);

    //Calculate constraint
    auto a2 = sum2Planes[res].a();
    auto b2 = sum2Planes[res].b();
    auto c2 = sum2Planes[res].c();
    auto d2 = sum2Planes[res].d();

    assert((a1 != 0 && a2 != 0) || (b1 != 0 && b2 != 0) ||
           (c1 != 0 && c2 != 0));

    CGAL::Gmpq ratio;
    if (a1 != 0)
    {
      ratio = a2 / a1;
    }
    else if (b1 != 0)
    {
      ratio = b2 / b1;
    }
    else
    {
      ratio = c2 / c1;
    }

    a1 *= ratio;
    b1 *= ratio;
    c1 *= ratio;
    d1 *= ratio;

    assert(a1 == a2 && b1 == b2 && c1 == c2);

    auto constraintD = d2 - d1;
    auto constraintE = 2 * d1 - d2;

    //cout << "A: " << a1 << endl;
    //cout << "B: " << b1 << endl;
    //cout << "C: " << c1 << endl;
    //cout << "D: " << constraintD << endl;
    //cout << "E: " << constraintE << endl;

    lp.set_a(X, count, a1);
    lp.set_a(Y, count, b1);
    lp.set_a(Z, count, c1);
    lp.set_a(W, count, constraintD);
    lp.set_b(count, -1 * constraintE);

    ++count;
  }
}

/* \brief constructs the prototype. */
Smallest_stabbing_cube* Smallest_stabbing_cube::prototype()
{ return new Smallest_stabbing_cube(true); }

/*! \brief clones. */
Container* Smallest_stabbing_cube::clone()
{ return new Smallest_stabbing_cube(); }

/*! \brief obtains the tag (type) of the container. */
const std::string& Smallest_stabbing_cube::get_tag() const
{ return s_tag; }

SGAL_END_NAMESPACE
