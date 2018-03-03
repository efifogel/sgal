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

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Geo_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_rule.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Utilities.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geo_set::s_prototype(nullptr);

const char* Geo_set::s_primitive_type_names[] = {
  "triangleStrip", "triangleFan", "triangles",
  "quadsStrip", "quads",
  "polygons",
  "lines", "lineStrips", "lineLoops"
};

const Char* Geo_set::s_attachment_names[] =
  { "none", "perVertex", "perPrimitive", "perMesh" };

// Default value:
const
Geo_set::Primitive_type Geo_set::s_def_primitive_type(Geo_set::PT_POLYGONS);

//! \brief constructor.
Geo_set::Geo_set(Boolean proto) :
  Geometry(proto),
  m_num_primitives(0),
  m_normal_attachment(AT_PER_PRIMITIVE),
  m_color_attachment(AT_PER_PRIMITIVE),
  m_coord_indices(),
  m_normal_indices(),
  m_color_indices(),
  m_tex_coord_indices(),
  m_primitive_type(s_def_primitive_type)
{}

//! \brief destructor.
Geo_set::~Geo_set()
{
  Observer observer(this, get_field_info(COORD_ARRAY));
  if (m_coord_array) m_coord_array->unregister_observer(observer);
}

//! \brief sets the attributes of this node.
void Geo_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  // Add the field-info records to the prototype:
  Execution_function exec_func;

  // coord
  exec_func = static_cast<Execution_function>(&Geo_set::coord_changed);
  auto coord_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_ARRAY, "coord",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      coord_array_func,
                                                      exec_func));

  // normal
  exec_func = static_cast<Execution_function>(&Geo_set::normal_changed);
  auto normal_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::normal_array_handle);
  s_prototype->add_field_info(new SF_shared_container(NORMAL_ARRAY, "normal",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      normal_array_func,
                                                      exec_func));

  // color
  exec_func = static_cast<Execution_function>(&Geo_set::color_changed);
  auto color_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::color_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COLOR_ARRAY, "color",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      color_array_func,
                                                      exec_func));

  // texCoord
  exec_func = static_cast<Execution_function>(&Geo_set::tex_coord_changed);
  auto tex_coord_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::tex_coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(TEX_COORD_ARRAY,
                                                      "texCoord",
                                                      Field_rule::RULE_EXPOSED_FIELD,
                                                      tex_coord_array_func,
                                                      exec_func));

  // coordIndex
  exec_func = static_cast<Execution_function>(&Geo_set::coord_indices_changed);
  auto coord_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::coord_indices_handle);
  s_prototype->add_field_info(new MF_uint(COORD_INDEX_ARRAY, "coordIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          coord_index_func, exec_func));

  // normalIndex
  exec_func = static_cast<Execution_function>(&Geo_set::normal_indices_changed);
  auto normal_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::normal_indices_handle);
  s_prototype->add_field_info(new MF_uint(NORMAL_INDEX_ARRAY, "normalIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          normal_index_func, exec_func));

  // colorIndex
  exec_func = static_cast<Execution_function>(&Geo_set::color_indices_changed);
  auto color_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::color_indices_handle);
  s_prototype->add_field_info(new MF_uint(COLOR_INDEX_ARRAY, "colorIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          color_index_func, exec_func));

  // texCoordIndex
  exec_func =
    static_cast<Execution_function>(&Geo_set::tex_coord_indices_changed);
  auto tex_coord_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::tex_coord_indices_handle);
  s_prototype->add_field_info(new MF_uint(TEX_COORD_INDEX_ARRAY,
                                          "texCoordIndex",
                                          Field_rule::RULE_EXPOSED_FIELD,
                                          tex_coord_index_func, exec_func));
}

//! \brief deletes the container prototype.
void Geo_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the container prototype.
Container_proto* Geo_set::get_prototype()
{
  if (s_prototype == nullptr) Geo_set::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of the object.
void Geo_set::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "colorAttachment") {
      auto num = sizeof(s_attachment_names) / sizeof(char*);
      const auto** found =
        std::find(s_attachment_names, &s_attachment_names[num], value);
      auto index = found - s_attachment_names;
      if (index < num) m_color_attachment = static_cast<Attachment>(index);
      else {
        std::cerr << "Unrecognized color attachment \"" << value << "\"!"
                  << std::endl;
      }
      elem->mark_delete(ai);
      continue;
    }

    if (name == "coordIndex") {
      m_num_primitives = 0;

      typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
      boost::char_separator<char> sep(", \t\n\r");
      tokenizer tokens(value, sep);
      Uint size = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
        ++size;
      if (size == 0) {
      err:
        m_coord_indices.clear();
        std::cerr << "Error!" << std::endl;
        //! todo issue an error
        elem->mark_delete(ai);
        continue;               // Advance to next attribute
      }

      m_coord_indices.resize(size);
      Uint i = 0;
      for (tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        m_coord_indices[i++] = static_cast<Uint>(boost::lexical_cast<int>(*it));
      }

      if (m_primitive_type == PT_POLYGONS) {
        Uint j = 0;
        Boolean tris = true, quads = true;
        for (i = 0; i < size; ++i) {
          if (m_coord_indices[i] == static_cast<Uint>(-1)) {
            m_num_primitives++;
            if (j != 3) tris = false;
            if (j != 4) quads = false;
            if (j < 3) goto err;
            j = 0;
          }
          else ++j;
        }
        SGAL_assertion(!tris || !quads);
        m_primitive_type =
          (tris) ? PT_TRIANGLES : (quads) ? PT_QUADS : PT_POLYGONS;
        //! \todo sg->get_stats().add_num_polygons(m_num_primitives);
      }
      else if (m_primitive_type == PT_LINES) {
        if ((size & 0x1) != 0) goto err;
        m_num_primitives = size >> 1;
      }
      else if (m_primitive_type == PT_LINE_STRIPS) {
        for (auto it = m_coord_indices.begin(); it != m_coord_indices.end();
             ++it)
          if (*it == static_cast<Uint>(-1)) ++m_num_primitives;
      }
      if (m_num_primitives == 0) goto err;
      coord_indices_changed();
      elem->mark_delete(ai);
      continue;
    }

    if (name == "colorIndex") {
      auto num_values = get_num_tokens(value);
      m_color_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_color_indices[i];
      color_indices_changed();
      elem->mark_delete(ai);
      continue;
    }

    if (name == "normalIndex") {
      auto num_values = get_num_tokens(value);
      m_normal_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_normal_indices[i];
      normal_indices_changed();
      elem->mark_delete(ai);
      continue;
    }

    if (name == "texCoordIndex") {
      auto num_values = get_num_tokens(value);
      m_tex_coord_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_tex_coord_indices[i];
      tex_coord_indices_changed();
      elem->mark_delete(ai);
      continue;
    }

    if (name == "primitiveType") {
      auto num = sizeof(s_primitive_type_names) / sizeof(char*);
      const auto** found =
        std::find(s_primitive_type_names, &s_primitive_type_names[num], value);
      auto index = found - s_primitive_type_names;
      if (index < num) set_primitive_type(static_cast<Primitive_type>(index));
      else
        std::cerr << "Illegal primitive type name (" << value << ")!"
                  << std::endl;
      elem->mark_delete(ai);
      continue;
    }
  }

  auto cai = elem->cont_attrs_begin();
  for (; cai != elem->cont_attrs_end(); ++cai) {
    const auto& name = elem->get_name(cai);
    auto cont = elem->get_value(cai);
    if (name == "coord") {
      auto coord_array = boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "normal") {
      auto normal_array = boost::dynamic_pointer_cast<Normal_array>(cont);
      set_normal_array(normal_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "color") {
      auto color_array = boost::dynamic_pointer_cast<Color_array>(cont);
      set_color_array(color_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "texCoord") {
      auto tex_coord_array = boost::dynamic_pointer_cast<Tex_coord_array>(cont);
      set_tex_coord_array(tex_coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

//! \brief responds to a change in the coordinate array.
void Geo_set::coord_content_changed(const Field_info* field_info)
{
  if (!field_info) field_info = get_field_info(COORD_ARRAY);
  bounding_sphere_changed(field_info);
}

//! \brief responds to a change in the normal array.
void Geo_set::normal_content_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the color array.
void Geo_set::color_content_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the texture coordinate array.
void Geo_set::tex_coord_content_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the coordinate-index array.
void Geo_set::coord_indices_changed(const Field_info* field_info)
{
  if (!field_info) field_info = get_field_info(COORD_INDEX_ARRAY);
  bounding_sphere_changed(field_info);
}

//! \brief responds to a change in the normal-index array.
void Geo_set::normal_indices_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the color-index array.
void Geo_set::color_indices_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the texture-coordinate index array.
void Geo_set::tex_coord_indices_changed(const Field_info* /* field_info */) {}

//! \brief responds to a change in the sphere bound.
void Geo_set::bounding_sphere_changed(const Field_info* field_info)
{
  if (!field_info) field_info = get_field_info(BOUNDING_SPHERE);
  Geometry::bounding_sphere_changed(field_info);
  Geometry::field_changed(field_info);
}

//! \brief sets the coordinate array.
void Geo_set::set_coord_array(Shared_coord_array coord_array)
{
  Observer observer(this, get_field_info(COORD_ARRAY));
  if (m_coord_array) m_coord_array->unregister_observer(observer);
  m_coord_array = coord_array;
  if (m_coord_array) m_coord_array->register_observer(observer);
  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief sets the normal array.
void Geo_set::set_normal_array(Shared_normal_array normal_array)
{
  m_normal_array = normal_array;
  normal_content_changed(get_field_info(NORMAL_ARRAY));
}

//! \brief sets the color array.
void Geo_set::set_color_array(Shared_color_array color_array)
{
  m_color_array = color_array;
  color_content_changed(get_field_info(COLOR_ARRAY));
}

//! \brief sets the texture-coordinate array.
void Geo_set::set_tex_coord_array(Shared_tex_coord_array tex_coord_array)
{
  m_tex_coord_array = tex_coord_array;
  tex_coord_content_changed(get_field_info(TEX_COORD_ARRAY));
}

//! \brief sets the coordinate-index array.
void Geo_set::set_coord_indices(const std::vector<Int32>& indices)
{
  m_coord_indices = indices;
  coord_indices_changed(get_field_info(COORD_INDEX_ARRAY));
}

//! \brief sets the normal-index array.
void Geo_set::set_normal_indices(const std::vector<Int32>& indices)
{
  m_normal_indices = indices;
  normal_indices_changed(get_field_info(NORMAL_INDEX_ARRAY));
}

//! \brief sets the color-index array.
void Geo_set::set_color_indices(const std::vector<Int32>& indices)
{
  m_color_indices = indices;
  color_indices_changed(get_field_info(COLOR_INDEX_ARRAY));
}

//! \brief sets the texture-coordinate-index array.
void Geo_set::set_tex_coord_indices(const std::vector<Int32>& indices)
{
  m_tex_coord_indices = indices;
  tex_coord_indices_changed(get_field_info(TEX_COORD_INDEX_ARRAY));
}

//! \brief processes change of coordinates.
void Geo_set::coord_changed(const Field_info* field_info)
{
  // Observe that the observer, being a pair of this container object and the
  // field_info argument, is automatically unregistered as an observer
  // of the previous value of the m_coord_array.
  if (m_coord_array) {
    Observer observer(this, field_info);
    m_coord_array->register_observer(observer);
  }
  coord_content_changed(get_field_info(COORD_ARRAY));
}

//! \brief processes change of normals.
void Geo_set::normal_changed(const Field_info* field_info)
{ normal_content_changed(get_field_info(NORMAL_ARRAY)); }

//! \brief processes change of colors.
void Geo_set::color_changed(const Field_info* field_info)
{ color_content_changed(get_field_info(COLOR_ARRAY)); }

//! \brief processes change of texture coordinates.
void Geo_set::tex_coord_changed(const Field_info* field_info)
{ tex_coord_content_changed(get_field_info(TEX_COORD_ARRAY)); }

//! \brief Process change of field.
void Geo_set::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case COORD_ARRAY: coord_content_changed(field_info); return;
   case NORMAL_ARRAY: normal_content_changed(field_info); return;
   case COLOR_ARRAY: color_content_changed(field_info); return;
   case TEX_COORD_ARRAY: tex_coord_content_changed(field_info); return;
   case COORD_INDEX_ARRAY: coord_indices_changed(field_info); return;
   case NORMAL_INDEX_ARRAY: normal_indices_changed(field_info); return;
   case COLOR_INDEX_ARRAY: color_indices_changed(field_info); return;
   case TEX_COORD_INDEX_ARRAY: tex_coord_indices_changed(field_info); return;

   default: break;
  }
  Geometry::field_changed(field_info);
}

//! \brief assigns the coord indices with the reverse of given indices.
void Geo_set::reverse_coord_indices(const std::vector<Int32>& indices)
{
  SGAL_assertion(indices != m_coord_indices);
  m_coord_indices.resize(indices.size());
  size_t i(0);
  auto rit = indices.rbegin();
  for (++rit; rit < indices.rend(); ++rit)
    m_coord_indices[i++] = *rit;
  m_coord_indices[i++] = static_cast<Uint>(-1);
}

//! \brief obtains the ith coordinate.
const Vector3f& Geo_set::get_coord_3d(Uint i) const
{
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);
  return (*coords)[i];
}

//! \brief cleans the sphere bound of the geometry set.
void Geo_set::clean_bounding_sphere()
{
  m_dirty_bounding_sphere = false;

  if (m_bb_is_pre_set) return;
  if (!m_coord_array || m_coord_array->empty()) return;
  auto coords = boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  if (!coords) return;
  m_bounding_sphere.set_around(coords->begin(), coords->end());
}

//! \brief obtains the bounding box.
Bounding_box Geo_set::bounding_box()
{
  auto it = m_coord_indices.begin();
  const Vector3f& v = get_coord_3d(*it);
  Bounding_box bbox(v[0], v[1], v[2], v[0], v[1], v[2]);
  for (++it; it != m_coord_indices.end(); ++it) {
    if (((Uint) -1) == *it) continue;
    const Vector3f& v = get_coord_3d(*it);
    Bounding_box tmp(v[0], v[1], v[2], v[0], v[1], v[2]);
    bbox += tmp;
  }
  return bbox;
}

SGAL_END_NAMESPACE
