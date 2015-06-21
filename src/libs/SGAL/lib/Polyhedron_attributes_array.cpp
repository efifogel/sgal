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
size_t Polyhedron_attributes_array::number_of_vertices() const
{
  size_t sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_number_of_vertices(); });
  return sum;
}

//! \brief Obtain the number of facets of all polyhedrons.
size_t Polyhedron_attributes_array::number_of_facets() const
{
  size_t sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_number_of_facets(); });
  return sum;
}

//! \brief Obtain the number of vertices of all polyhedrons.
Float Polyhedron_attributes_array::volume() const
{
  Float sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_volume(); });
  return sum;
}

//! \brief Obtain the number of vertices of all polyhedrons.
Float Polyhedron_attributes_array::surface_area() const
{
  Float sum(0);
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { sum += attrs.get_surface_area(); });
  return sum;
}

//! \brief Obtain the number of vertices of all polyhedrons.
const Bounding_box Polyhedron_attributes_array::bounding_box() const
{
  Bounding_box bbox;
  std::for_each(m_array.begin(), m_array.end(),
                  [&](const Polyhedron_attributes& attrs)
                  { bbox += attrs.get_bounding_box(); });
  return bbox;
}

//! \brief Obtain the number of vertices of all polyhedrons.
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

SGAL_END_NAMESPACE
