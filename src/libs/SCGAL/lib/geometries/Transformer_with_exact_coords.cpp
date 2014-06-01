// Copyright (c) 2014 Israel.
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

#include <string>
#include <vector>

#include "SGAL/Container_factory.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"

#include "SCGAL/Transformer_with_exact_coords.hpp"
#include "SCGAL/Exact_coord_array_3d.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Transformer_with_exact_coords::s_tag =
  "TransformerWithExactCoordinates";

//! The node prototype.
Container_proto* Transformer_with_exact_coords::s_prototype(nullptr);

//! Register to the container factory.
REGISTER_TO_FACTORY(Transformer_with_exact_coords,
                    "Transformer_with_exact_coords");

//! \brief initializes the node prototype.
void Transformer_with_exact_coords::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Coord_transformer::get_prototype());
}

//! \brief deletes the node prototype.
void Transformer_with_exact_coords::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! Obtain the node prototype.
Container_proto* Transformer_with_exact_coords::get_prototype()
{
  if (!s_prototype) Transformer_with_exact_coords::init_prototype();
  return s_prototype;
}

//! Sets the attributes of the object extracted from the VRML or X3D file.
void Transformer_with_exact_coords::set_attributes(Element* elem)
{ Coord_transformer::set_attributes(elem); }

//! \brief applies the transformation.
void Transformer_with_exact_coords::apply()
{
  if (!m_coord_array) return;
  if (!m_coord_array_changed) return;

  boost::shared_ptr<Coord_array_3d> coords_changed =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array_changed);
  SGAL_assertion(coords_changed);
  boost::shared_ptr<Exact_coord_array_3d> exact_coords =
    boost::dynamic_pointer_cast<Exact_coord_array_3d>(m_coord_array);

  if (exact_coords) {
    const std::vector<Vector3f>& coords = exact_coords->get_inexact_coords();
    if (do_reflect())
      reflect(coords.begin(), coords.end(), coords_changed->begin());
    else transform(coords.begin(), coords.end(), coords_changed->begin());
  }
  else {
    boost::shared_ptr<Coord_array_3d> coords =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    SGAL_assertion(coords);
    if (do_reflect())
      reflect(coords->begin(), coords->end(), coords_changed->begin());
    else transform(coords->begin(), coords->end(), coords_changed->begin());
  }
}

SGAL_END_NAMESPACE
