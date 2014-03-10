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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 * A geometry container that represents a quadric surface
 */

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Element.hpp"

#include "SCGAL/Quadric_geo.hpp"

#include "Quadric.hpp"

SGAL_BEGIN_NAMESPACE

std::string Quadric_geo::s_tag = "Quadric";
SGAL::Container_proto* Quadric_geo::s_prototype(NULL);

REGISTER_TO_FACTORY(Quadric_geo, "Quadric_geo");

/*! Constructor */
Quadric_geo::Quadric_geo(Boolean proto) :
  Geometry(proto),
  m_dirty(true)
{}

/*! Destructor */
Quadric_geo::~Quadric_geo() {}

/*! \brief initializes the container prototype */
void Quadric_geo::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());
}

/*! \brief deletes the container prototype */
void Quadric_geo::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the container prototype */
Container_proto* Quadric_geo::get_prototype()
{
  if (!s_prototype) Quadric_geo::init_prototype();
  return s_prototype;
}

/*! \brief sets the ellpsoid attributes */
void Quadric_geo::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(", \t\n\r");
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "coefficients") {
      tokenizer tokens(value, sep);
      Uint size = std::distance(tokens.begin(), tokens.end());
      if (size == 0) {
        m_coefficients.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_coefficients.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
        m_coefficients[i++] = boost::lexical_cast<AT::Integer>(*it);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

/*! \brief cleans the representation and prepares it for drawing */
void Quadric_geo::clean()
{
  m_quadric = Quadric_3(m_coefficients[0], m_coefficients[1],
                        m_coefficients[2], m_coefficients[3],
                        m_coefficients[4], m_coefficients[5],
                        m_coefficients[6], m_coefficients[7],
                        m_coefficients[8], m_coefficients[9]);

  m_dirty = false;
}

/*! \brief */
void Quadric_geo::cull(Cull_context & cull_context)
{
}

/*! \brief */
void Quadric_geo::isect(Isect_action * action)
{
}

/*! \brief */
Boolean Quadric_geo::clean_sphere_bound()
{
#define SQRT_3          1.732f

  if (!m_sphere_bound_dirty) return false;
  if (is_dirty()) clean();
  if (m_bb_is_pre_set) return true;

  //! \todo calculate the sphere bound
  m_sphere_bound.set_center(Vector3f(0, 0, 0));
  m_sphere_bound.set_radius(SQRT_3);

  m_sphere_bound_dirty = false;

  return true;
}

/*! \brief draws the geometry */
void Quadric_geo::draw(Draw_action * action)
{
  if (is_dirty()) clean();
  if (is_empty()) return;

  Boolean normalize = has_scale();
  if (normalize) glEnable(GL_NORMALIZE);
  draw_geometry(action);
  if (normalize) glDisable(GL_NORMALIZE);
}

/*! \brief clears the internal representation and auxiliary data structures
 */
void Quadric_geo::clear()
{
  //! \todo clear the quadric
  m_dirty = true;
}

/*! \brief is the representation empty ?
 */
Boolean Quadric_geo::is_empty() const
{
  //! \todo is the quadric empty?
  return false;
}

/*! \brief draws the arrangement on sphere */
void Quadric_geo::draw_geometry(Draw_action * action)
{
  Quadric quadric(m_quadric);
  quadric.draw();
}

SGAL_END_NAMESPACE
