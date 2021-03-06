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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "SGAL/Polyhedron_attributes_array.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs default
Polyhedron_attributes_array::Polyhedron_attributes_array() {}

//! \brief destructs default
Polyhedron_attributes_array::~Polyhedron_attributes_array()
{ m_array.clear(); }

//! \brief obtains the (mutable) array of polyhedron attributes.
std::vector<Polyhedron_attributes>&
Polyhedron_attributes_array::get_array()
{ return m_array; }

//! \brief obtains the (const) array of polyhedron attributes.
const std::vector<Polyhedron_attributes>&
Polyhedron_attributes_array::get_array() const
{ return m_array; }

//! \brief obtains the number of vertices of all polyhedrons.
Size Polyhedron_attributes_array::number_of_vertices() const
{
  Size sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_number_of_vertices(); });
  return sum;
}

//! \brief obtains the number of edges of all polyhedrons.
Size Polyhedron_attributes_array::number_of_edges() const
{
  Size sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_number_of_edges(); });
  return sum;
}

//! \brief obtains the number of facets of all polyhedrons.
Size Polyhedron_attributes_array::number_of_facets() const
{
  Size sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_number_of_facets(); });
  return sum;
}

//! \brief obtains the total volume of all polyhedrons.
Float Polyhedron_attributes_array::volume() const
{
  Float sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_volume(); });
  return sum;
}

//! \brief obtains the surface area of all polyhedrons.
Float Polyhedron_attributes_array::surface_area() const
{
  Float sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                [&](const Polyhedron_attributes& attrs)
                { sum += attrs.get_surface_area(); });
  return sum;
}

//! \brief obtains the number of connected components of all polyhedrons.
Size Polyhedron_attributes_array::number_of_connected_components() const
{
  Size sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                [&](const Polyhedron_attributes& attrs)
                { sum += attrs.get_number_of_connected_components(); });
  return sum;
}

//! \brief obtains the number of vertices of all polyhedrons.
const Bounding_box Polyhedron_attributes_array::bounding_box() const
{
  auto it = m_array.begin();
  if (it == m_array.end()) return Bounding_box();
  Bounding_box bbox(it++->get_bounding_box());
  std::for_each(it, m_array.end(),
                [&](const Polyhedron_attributes& attrs)
                { bbox += attrs.get_bounding_box(); });
  return bbox;
}

//! \brief obtains the number of vertices of all polyhedrons.
const Bounding_sphere Polyhedron_attributes_array::bounding_sphere() const
{
  std::vector<const Bounding_sphere*> spheres;
  std::for_each(m_array.begin(), m_array.end(),
                [&](const Polyhedron_attributes& attrs)
                {
                  const auto& bs = attrs.get_bounding_sphere();
                  if (bs.get_radius() == 0) return;
                  spheres.push_back(&bs);
                });

  Bounding_sphere bs;
  bs.set_around(spheres.begin(), spheres.end());
  return bs;
}

//! \brief determines wheather all polyhedra are valid.
Boolean Polyhedron_attributes_array::is_valid() const
{
  for (auto it = m_array.begin(); it != m_array.end(); ++it)
    if (!(it->is_valid())) return false;
  return true;
}

SGAL_END_NAMESPACE
