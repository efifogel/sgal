// Copyright (c) 2007 Israel.
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
// $Id: Geodesic_voronoi_on_sphere_geo.cpp 7204 2009-01-24 21:43:15Z efif $
// $Revision: 7204 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if defined(_WIN32)
#pragma warning( disable : 4146)
#pragma warning( disable : 4244)
#pragma warning( disable : 4390)
#pragma warning( disable : 4503)
#pragma warning( disable : 4800)
#pragma warning( disable : 4996)
#endif

/*! \file
 * A geometry container that represents an arrangement induced by arcs of
 * great circles embeded on a sphere.
 */

#include <boost/lexical_cast.hpp>

#include <CGAL/Cartesian.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include <CGAL/envelope_3.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Sphere.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Context.hpp"

#include "SCGAL/Geodesic_voronoi_on_sphere_geo.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geodesic_voronoi_on_sphere_geo::s_prototype = NULL;

/*! Default values */
const Boolean Geodesic_voronoi_on_sphere_geo::s_def_draw_sites(SGAL_TRUE);
const Vector3f Geodesic_voronoi_on_sphere_geo::s_def_site_color(0, 0, 0);

/*! Constructor */
Geodesic_voronoi_on_sphere_geo::
Geodesic_voronoi_on_sphere_geo(Boolean proto) :
  Arrangement_on_sphere_base_geo(proto),
  m_draw_sites(s_def_draw_sites),
  m_site_color(s_def_site_color)
{}

/*! Destructor */
Geodesic_voronoi_on_sphere_geo::~Geodesic_voronoi_on_sphere_geo() {}

/*! \brief initializes the container prototype. */
void Geodesic_voronoi_on_sphere_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype =
    new Container_proto(Arrangement_on_sphere_base_geo::get_prototype());

  //! Container execution function
  typedef void (Container::* Execution_function)(Field_info*);
  
  s_prototype->add_field_info(new SF_bool(DRAW_SITES, "drawSites",
                                          get_member_offset(&m_draw_sites)));
}

/*! \brief deletes the container prototype. */
void Geodesic_voronoi_on_sphere_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype. */
Container_proto* Geodesic_voronoi_on_sphere_geo::get_prototype()
{
  if (!s_prototype) Geodesic_voronoi_on_sphere_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes. */
 void Geodesic_voronoi_on_sphere_geo::set_attributes(Element* elem)
{
  Arrangement_on_sphere_base_geo::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "siteIndex") {
      tokenizer tokens(value, sep);      
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_site_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error        
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_site_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_site_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
      }
      elem->mark_delete(ai);
      continue;
    }

    if (name == "drawSites") {
      m_draw_sites = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "siteColor") {
      Vector3f col(value);
      set_site_color(col);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief */
void Geodesic_voronoi_on_sphere_geo::cull(Cull_context& cull_context) {}

SGAL_END_NAMESPACE
