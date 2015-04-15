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
 * Converts an image to a elevation_grid
 */

#include <boost/lexical_cast.hpp>
//! \todo remove this
#include <boost/filesystem/path.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Field_info.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Image_to_elevation_grid.hpp"
#include "SGAL/Elevation_grid.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Image_to_elevation_grid::s_tag = "ImageToElavationGrid";
Container_proto* Image_to_elevation_grid::s_prototype(nullptr);

// Defaults values:

REGISTER_TO_FACTORY(Image_to_elevation_grid, "Image_to_elevation_grid");

//! \brief constructor.
Image_to_elevation_grid::Image_to_elevation_grid(Boolean proto) :
  Node(proto),
  m_trigger(false)
{}

//! \brief destructor.
Image_to_elevation_grid::~Image_to_elevation_grid() {}

//! \brief initializes the container prototype.
void Image_to_elevation_grid::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Image_to_elevation_grid::execute);

  // trigger
  Boolean_handle_function trigger_func =
    static_cast<Boolean_handle_function>
    (&Image_to_elevation_grid::trigger_handle);
  s_prototype->add_field_info(new SF_bool(TRIGGER, "trigger",
                                          RULE_IN,
                                          trigger_func, exec_func));

  // elevationGrid
  exec_func = static_cast<Execution_function>
    (&Image_to_elevation_grid::elevation_grid_changed);
  Shared_container_handle_function elevation_grid_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Image_to_elevation_grid::elevation_grid_handle);
  s_prototype->add_field_info(new SF_shared_container(ELEVATION_GRID,
                                                      "elevationGrid",
                                                      RULE_EXPOSED_FIELD,
                                                      elevation_grid_func,
                                                      exec_func));

  // image
  Shared_container_handle_function image_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Image_to_elevation_grid::image_handle);
  s_prototype->add_field_info(new SF_shared_container(IMAGE,
                                                      "image",
                                                      RULE_EXPOSED_FIELD,
                                                      image_func));
}

//! \brief deletes the container prototype.
void Image_to_elevation_grid::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Image_to_elevation_grid::get_prototype()
{
  if (!s_prototype) Image_to_elevation_grid::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object extracted from the input file.
void Image_to_elevation_grid::set_attributes(Element* elem)
{
  Node::set_attributes(elem);
  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "trigger") {
      m_trigger = boost::lexical_cast<Boolean>(value);
      elem->mark_delete(ai);
      continue;
    }
  }

  for (auto cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end();
       ++cai)
  {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "image") {
      set_image(boost::dynamic_pointer_cast<Image>(cont));
      {
        //! \todo remove this
        boost::filesystem::path dir(".");
        m_image->add_dir(dir);
      }
      elem->mark_delete(cai);
      continue;
    }
    if (name == "elevationGrid") {
      auto eg = boost::dynamic_pointer_cast<Elevation_grid>(cont);
      set_elevation_grid(eg);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief sets the 2D array that represents the elevation_grid above a grid.
void Image_to_elevation_grid::
set_elevation_grid(Shared_elevation_grid elevation_grid)
{ m_elevation_grid = elevation_grid; }

//! \brief executes the engine.
void Image_to_elevation_grid::execute(const Field_info* /*! field_info */)
{
  if (!m_image) return;

  auto size = m_image->get_width() * m_image->get_height();
  if (!m_elevation_grid) {
    m_elevation_grid.reset(new Elevation_grid);
    SGAL_assertion(m_elevation_grid);
  }
  auto& heights = m_elevation_grid->get_height();
  heights.resize(size);

  if (m_image->is_dirty()) m_image->clean();
  Uint width = m_image->get_width();
  Uint height = m_image->get_height();
  Image::Format format = m_image->get_format();
  Uchar* pixels = static_cast<Uchar*>(m_image->get_pixels());

  // std::cout << "Width: " << width << std::endl;
  // std::cout << "Height: " << height << std::endl;
  // std::cout << "Format: " << format << std::endl;
  size_t j(0);
  Float factor = 16.0f/256.0f;
  for (size_t i = 0; i < size; ++i) {
    heights[i] = static_cast<Float>(pixels[j]) * factor;
    j += 4;
  }
  m_elevation_grid->set_x_dimension(width);
  m_elevation_grid->set_z_dimension(height);
  Field* elevation_grid_field = get_field(ELEVATION_GRID);
  if (elevation_grid_field != nullptr) elevation_grid_field->cascade();
}

//! \brief processes change of elevation_grid.
void Image_to_elevation_grid::
elevation_grid_changed(const Field_info* /* field_info. */)
{
  //m_dirty = true;
  //m_dirty_elevation_grid = true;
}

SGAL_END_NAMESPACE
