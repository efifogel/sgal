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

/*! \file
 *
 * Converts an image to a height map
 */

#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Image_to_height_map.hpp"
#include "SGAL/Coord_array_1d.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Scene_graph.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Image_to_height_map::s_tag("ImageToHeightMap");
Container_proto* Image_to_height_map::s_prototype(nullptr);

// Defaults values:

REGISTER_TO_FACTORY(Image_to_height_map, "Image_to_height_map");

//! \brief constructor.
Image_to_height_map::Image_to_height_map(Boolean proto) :
  Node(proto),
  m_trigger(false)
{}

//! \brief destructor.
Image_to_height_map::~Image_to_height_map() {}

//! \brief initializes the container prototype.
void Image_to_height_map::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  auto exec_func =
    static_cast<Execution_function>(&Image_to_height_map::trigger_changed);

  // trigger
  auto trigger_func =
    static_cast<Boolean_handle_function>(&Image_to_height_map::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          Field_rule::RULE_IN,
                                          trigger_func, exec_func));

  // image
  auto image_func = reinterpret_cast<Shared_container_handle_function>
    (&Image_to_height_map::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE,
                                                      "image",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      image_func));

  // heightMap
  auto height_map_func = reinterpret_cast<Shared_container_handle_function>
    (&Image_to_height_map::height_map_handle);
  s_prototype->add_field_info(new SF_shared_container(HEIGHT_MAP,
                                                      "heightMap",
                                                      Field_rule::RULE_OUT,
                                                      height_map_func));

  // xDimension
  auto x_dimension_func =
    static_cast<Uint_handle_function>(&Image_to_height_map::x_dimension_handle);
  s_prototype->add_field_info(new SF_uint(X_DIMENSION, "xDimension",
                                          Field_rule::RULE_OUT,
                                          x_dimension_func));

  // zDimension
  auto z_dimension_func =
    static_cast<Uint_handle_function>(&Image_to_height_map::z_dimension_handle);
  s_prototype->add_field_info(new SF_uint(Z_DIMENSION, "zDimension",
                                          Field_rule::RULE_OUT,
                                          z_dimension_func));
}

//! \brief deletes the container prototype.
void Image_to_height_map::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Image_to_height_map::get_prototype()
{
  if (!s_prototype) Image_to_height_map::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Image_to_height_map::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  // for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
  //   const auto& name = elem->get_name(ai);
  //   const auto& value = elem->get_value(ai);
  // }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "image") {
      set_image(boost::dynamic_pointer_cast<Image>(cont));
      elem->mark_delete(cai);
      continue;
    }
    if (name == "heightMap") {
      auto height_map = boost::dynamic_pointer_cast<Coord_array_1d>(cont);
      set_height_map(height_map);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the 2D array that represents the height map.
void Image_to_height_map::set_height_map(Shared_coord_array_1d height_map)
{ m_height_map = height_map; }

/*! \brief triggers the execution of the engine as a response to change in one
 * of the input fields.
 */
void Image_to_height_map::trigger_changed(const Field_info* /* field_info */)
{ execute(); }

//! \brief executes the engine.
void Image_to_height_map::execute()
{
  if (!m_image) return;

  auto size = m_image->get_width() * m_image->get_height();
  if (!m_height_map) {
    m_height_map.reset(new Coord_array_1d);
    SGAL_assertion(m_height_map);
  }
  auto& heights = m_height_map->get_array();
  heights.resize(size);

  if (m_image->is_dirty()) m_image->clean();
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  Uchar* pixels = static_cast<Uchar*>(m_image->get_pixels());

  // std::cout << "Width: " << width << std::endl;
  // std::cout << "Height: " << height << std::endl;
  // std::cout << "Format: " << format << std::endl;
  // std::cout << "Name: " << m_image->get_format_name(format) << std::endl;
  // std::cout << "# components: " << m_image->get_component_count() << std::endl;

  // Convert to height map:
  size_t j(0);
  Float factor = 16.0f/256.0f;
  switch (format) {
   case Image::kLuminance8:
    for (size_t i = 0; i < size; ++i) {
      heights[i] = static_cast<Float>(pixels[j]) * factor;
      ++j;
    }
    break;

   case Image::kRGB8_8_8:
    for (size_t i = 0; i < size; ++i) {
      float val(0);
      for (auto k = 0; k < 3; ++k) val += static_cast<Float>(pixels[j+k]);
      val /= 3.0;
      heights[i] = val * factor;
      j += 3;
    }
    break;

   case Image::kRGBA8_8_8_8:
    for (size_t i = 0; i < size; ++i) {
      float val(0);
      for (auto k = 0; k < 3; ++k) val += static_cast<Float>(pixels[j+k]);
      val /= 3.0;
      heights[i] = val * factor;
      j += 4;
    }
    break;

   default: throw(std::runtime_error("Error: unrecognized format!"));
  }

  auto* height_map_field = get_field(HEIGHT_MAP);
  if (height_map_field != nullptr) height_map_field->cascade();

  set_x_dimension(width);
  set_z_dimension(height);
}

//! \brief sets the number of grid points along the x-dimension.
void Image_to_height_map::set_x_dimension(Uint x_dimension)
{
  m_x_dimension = x_dimension;
  auto* field = get_field(X_DIMENSION);
  if (field != nullptr) field->cascade();
}

//! \brief sets the number of grid points along the z-dimension.
void Image_to_height_map::set_z_dimension(Uint z_dimension)
{
  m_z_dimension = z_dimension;
  auto* field = get_field(Z_DIMENSION);
  if (field != nullptr) field->cascade();
}

//! \breif writes a field of this container.
void Image_to_height_map::write(Formatter* formatter)
{
  auto* vrml_formatter = dynamic_cast<Vrml_formatter*>(formatter);
  if (!vrml_formatter) return;
  Node::write(vrml_formatter);
}

SGAL_END_NAMESPACE
