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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4800)
#endif

/*! \file
 * A group of assembly parts that comprise an assembly.
 */

#include <time.h>
#include <string>
#include <boost/type_traits.hpp>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_indexed_items.h>
#include <CGAL/convex_decomposition_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Epec_kernel.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Indexed_face_set.hpp"

#include "SCGAL/basic.hpp"
#include "SCGAL/Assembly_part.hpp"
#include "SCGAL/Exact_nef_polyhedron.hpp"
#include "SCGAL/compute_planes.hpp"
#include "SCGAL/merge_coplanar_facets.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Assembly_part::s_prototype(nullptr);
std::string Assembly_part::s_tag = "AssemblyPart";

REGISTER_TO_FACTORY(Assembly_part, "Assembly_part");

//! \brief constructor.
Assembly_part::Assembly_part(Boolean proto) :
  Group(proto),
  m_dirty(true),
  m_id(0xffffffff)
{}

//! \brief destructor.
Assembly_part::~Assembly_part() { clear(); }

//! \brief clones the container (virtual constructor) with deep-copy.
Container* Assembly_part::clone()
{
  if (m_dirty) clean();
  Container::clone();
}

//! \brief initializes the node prototype.
void Assembly_part::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Group::get_prototype());
}

//! \brief deletes the node prototype.
void Assembly_part::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the node prototype.
Container_proto* Assembly_part::get_prototype()
{
  if (!s_prototype) Assembly_part::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Assembly_part::set_attributes(Element* elem)
{ Group::set_attributes(elem); }

//! \brief prints information to an output stream.
void Assembly_part::print_info(std::ostream& out)
{ out << "Part id[" << m_id << "]" << std::endl; }

//! \brief clears the representation.
void Assembly_part::clear()
{
  m_sgm_geos.clear();
  m_sgm_apps.clear();
  m_dirty = true;
}

//! \brief cleans internal representation.
void Assembly_part::clean()
{
  clean_sgm_geos(this);
  m_dirty = false;
}

//! \brief obtains the container of the Sgm geometries that comprise this part.
Assembly_part::Sgm_geo_list& Assembly_part::get_sgm_geos()
{
  if (m_dirty) clean();
  return m_sgm_geos;
}

//! \brief obtains the container of the SGM appearances that comprise this part.
Assembly_part::Appearance_list& Assembly_part::get_sgm_apps()
{
  if (m_dirty) clean();
  return m_sgm_apps;
}

//! \brief constructs all the SGM's that comprise this part.
void Assembly_part::clean_sgm_geos(Group* /* group */)
{
  for (auto it = m_childs.begin(); it != m_childs.end(); ++it) {
    auto node = *it;
    clean_sgm_geos(&*node);
  }
}

//! \brief constructs all the SGM's that comprise this part.
void Assembly_part::clean_sgm_geos(Container* node)
{
  static float total_duration_time = 0;
  static size_t total_number_of_vertices(0);
  static size_t total_number_of_edges(0);
  static size_t total_number_of_facets(0);

  auto* shape = dynamic_cast<Shape*>(node);
  if (shape) {
    auto app = shape->get_appearance();
    auto geometry = shape->get_geometry();
    auto sgm_geo = boost::dynamic_pointer_cast<Sgm_geo>(geometry);
    if (sgm_geo) {
      m_sgm_geos.push_back(sgm_geo);
      m_sgm_apps.push_back(app);
      return;
    }

    auto ifs = boost::dynamic_pointer_cast<Indexed_face_set>(geometry);
    if (ifs) {
      typedef CGAL::Nef_polyhedron_3<Epec_kernel, CGAL::SNC_indexed_items>
                                                        Nef_polyhedron;
      typedef Nef_polyhedron::Volume_const_iterator     Volume_const_iterator;

      clock_t start_time = clock();
      ifs->set_polyhedron_type(Indexed_face_set::POLYHEDRON_EPEC);
      const auto& p_var = ifs->get_polyhedron();
      const auto& p_const = boost::get<Epec_polyhedron>(p_var);
      /*! \todo Allow passing a const polyhedron to the constructor of
       * Nef_polyhedron
       */
      auto& polyhedron = const_cast<Epec_polyhedron&>(p_const);
      std::cout << "# Piece: " << polyhedron.size_of_vertices() << ", "
                << polyhedron.size_of_halfedges() << ", "
                << polyhedron.size_of_facets()
                << std::endl;
      Nef_polyhedron nef_polyhedron = Nef_polyhedron(polyhedron);
      CGAL::convex_decomposition_3(nef_polyhedron);
      clock_t end_time = clock();
      float duration_time =
        static_cast<float>(end_time - start_time) / CLOCKS_PER_SEC;
      total_duration_time += duration_time;
      // The first volume is the outer volume, which is ignored in the
      // decomposition
      Volume_const_iterator ci = ++nef_polyhedron.volumes_begin();
      size_t number_of_vertices(0);
      size_t number_of_edges(0);
      size_t number_of_facets(0);
      for (; ci != nef_polyhedron.volumes_end(); ++ci) {
        if (!ci->mark()) continue;
        Sgm_geo::Polyhedron p;
        nef_polyhedron.convert_inner_shell_to_polyhedron(ci->shells_begin(), p);
        // std::cout << p << std::endl;
        typedef boost::is_same<Sgm_geo::Polyhedron::Plane_3, Epec_plane_3>
          Polyhedron_has_plane;
        // TBD: Use an existing kernel.
        Epec_kernel kernel;
        compute_planes(kernel, p, Polyhedron_has_plane());
        merge_coplanar_facets(kernel, p, Polyhedron_has_plane());
        number_of_vertices += p.size_of_vertices();
        number_of_edges += p.size_of_halfedges()/2;
        number_of_facets += p.size_of_facets();
        // std::cout << p << std::endl;
        auto sgm_geo = Shared_sgm_geo(new Sgm_geo);
        sgm_geo->set_polyhedron(&p);
        sgm_geo->set_aos_edge_color(app->get_material()->get_diffuse_color());
        // We need to clean, because the Polyhedron is local
        sgm_geo->clean_sgm();
        m_sgm_geos.push_back(sgm_geo);
        m_sgm_apps.push_back(app);
      }
      total_number_of_vertices += number_of_vertices;
      total_number_of_edges += number_of_edges;
      total_number_of_facets += number_of_facets;
      std::cout << "# pieces: " << m_sgm_apps.size()
                << " <" << number_of_vertices
                << ", " << number_of_edges
                << ", " << number_of_facets << ">"
                << " (<" << total_number_of_vertices
                << ", " << total_number_of_edges
                << ", " << total_number_of_facets << ">)"
                << ", " << duration_time << " (" << total_duration_time << ")"
                << std::endl;
      return;
    }

    auto nef_geo =
      boost::dynamic_pointer_cast<Exact_nef_polyhedron>(shape->get_geometry());
    if (nef_geo) {
      nef_geo->convex_decomposition();
      for (auto ci = nef_geo->volumes_begin(); ci != nef_geo->volumes_end();
           ++ci)
      {
        if (!ci->mark()) continue;

        Sgm_geo::Polyhedron polyhedron;
        nef_geo->convert_inner_shell_to_polyhedron(ci->shells_begin(),
                                                   polyhedron);
        auto sgm_geo = Shared_sgm_geo(new Sgm_geo);
        sgm_geo->set_polyhedron(&polyhedron);
        // We need to clean, because the Polyhedron is local
        sgm_geo->clean_sgm();
        m_sgm_geos.push_back(sgm_geo);
        m_sgm_apps.push_back(app);
      }
      return;
    }

    std::cerr << "Error: Unrecognized geometry attribute";
    if (!shape->get_name().empty())
      std::cerr << " (" << shape->get_name() << ")";
    std::cerr << "!" << std::endl;
    return;
  }

  auto* my_switch = dynamic_cast<Switch*>(node);
  if (my_switch) {
    auto choice = my_switch->get_choice();
    if (!choice) return;
    return clean_sgm_geos(&*choice);
  }

  auto* group = dynamic_cast<Group*>(node);
  if (group) return clean_sgm_geos(group);
}

SGAL_END_NAMESPACE
