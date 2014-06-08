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

#include "SGAL/Geo_set.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geo_set::s_prototype(NULL);

const char* Geo_set::s_primitive_type_names[] = {
  "triangleStrip", "triangleFan", "triangles",
  "quadsStrip", "quads",
  "polygons",
  "lines", "lineStrips", "lineLoops"
};

const Char* Geo_set::s_attachment_names[] =
  { "perVertex", "perPrimitive", "perMesh" };

// Default value:
const
Geo_set::Primitive_type Geo_set::s_def_primitive_type(Geo_set::PT_POLYGONS);

//! \brief constructor.
Geo_set::Geo_set(Boolean proto) :
  Geometry(proto),
  m_num_primitives(0),
  m_normal_attachment(PER_PRIMITIVE),
  m_color_attachment(PER_PRIMITIVE),
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
  Shared_container_handle_function coord_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COORD_ARRAY, "coord",
                                                      RULE_EXPOSED_FIELD,
                                                      coord_array_func,
                                                      exec_func));

  // normal
  exec_func = static_cast<Execution_function>(&Geo_set::normal_changed);
  Shared_container_handle_function normal_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::normal_array_handle);
  s_prototype->add_field_info(new SF_shared_container(NORMAL_ARRAY, "normal",
                                                      RULE_EXPOSED_FIELD,
                                                      normal_array_func,
                                                      exec_func));

  // color
  exec_func = static_cast<Execution_function>(&Geo_set::color_changed);
  Shared_container_handle_function color_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::color_array_handle);
  s_prototype->add_field_info(new SF_shared_container(COLOR_ARRAY, "color",
                                                      RULE_EXPOSED_FIELD,
                                                      color_array_func,
                                                      exec_func));

  // texCoord
  exec_func = static_cast<Execution_function>(&Geo_set::tex_coord_changed);
  auto tex_coord_array_func =
    reinterpret_cast<Shared_container_handle_function>
    (&Geo_set::tex_coord_array_handle);
  s_prototype->add_field_info(new SF_shared_container(TEX_COORD_ARRAY,
                                                      "texCoord",
                                                      RULE_EXPOSED_FIELD,
                                                      tex_coord_array_func,
                                                      exec_func));

  // coordIndex
  exec_func = static_cast<Execution_function>(&Geo_set::field_changed);
  auto coord_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::coord_indices_handle);
  s_prototype->add_field_info(new MF_uint(COORD_INDEX_ARRAY, "coordIndex",
                                          RULE_EXPOSED_FIELD,
                                          coord_index_func, exec_func));

  // normalIndex
  auto normal_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::normal_indices_handle);
  s_prototype->add_field_info(new MF_uint(NORMAL_INDEX_ARRAY, "normalIndex",
                                          RULE_EXPOSED_FIELD,
                                          normal_index_func, exec_func));

  // colorIndex
  auto color_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::color_indices_handle);
  s_prototype->add_field_info(new MF_uint(COLOR_INDEX_ARRAY, "colorIndex",
                                          RULE_EXPOSED_FIELD,
                                          color_index_func, exec_func));

  // texCoordIndex
  auto tex_coord_index_func =
    reinterpret_cast<Uint_array_handle_function>
    (&Geo_set::tex_coord_indices_handle);
  s_prototype->add_field_info(new MF_uint(TEX_COORD_INDEX_ARRAY,
                                          "texCoordIndex",
                                          RULE_EXPOSED_FIELD,
                                          tex_coord_index_func, exec_func));
}

//! \brief deletes the container prototype.
void Geo_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

//! \brief obtains the container prototype.
Container_proto* Geo_set::get_prototype()
{
  if (s_prototype == NULL) Geo_set::init_prototype();
  return s_prototype;
}

//! \brief sets the coordinate array.
void Geo_set::set_coord_array(Shared_coord_array coord_array)
{
  Observer observer(this, get_field_info(COORD_ARRAY));
  if (m_coord_array) m_coord_array->unregister_observer(observer);
  m_coord_array = coord_array;
  if (m_coord_array) m_coord_array->register_observer(observer);
  m_dirty_sphere_bound = true;
}

//! \brief sets the normal array.
void Geo_set::set_normal_array(Shared_normal_array normal_array)
{ m_normal_array = normal_array; }

//! \brief sets the texture-coordinate array.
void Geo_set::set_tex_coord_array(Shared_tex_coord_array tex_coord_array)
{ m_tex_coord_array = tex_coord_array; }

//! \brief sets the color array.
void Geo_set::set_color_array(Shared_color_array color_array)
{ m_color_array = color_array; }

//! \brief returns true if the representation is empty.
Boolean Geo_set::is_empty() const
{ return (!m_coord_array || (m_coord_array->size() == 0) ); }

//! \brief calculates the sphere bound of the geometry set.
Boolean Geo_set::clean_sphere_bound()
{
  if (!m_dirty_sphere_bound) return false;

  if (!m_bb_is_pre_set && m_coord_array) {
    // We assume that m_coord_array points at an object of type
    // Coord_array_3d type.
    boost::shared_ptr<Coord_array_3d> coords =
      boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
    SGAL_assertion(coords);
    m_sphere_bound.set_around(coords->begin(), coords->end());
  }
  m_dirty_sphere_bound = false;
  return true;
}

//! \brief sets the attributes of the object.
void Geo_set::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);

    if (name == "colorAttachment") {
      Uint num = sizeof(s_attachment_names) / sizeof(char*);
      const char** found = std::find(s_attachment_names,
                                     &s_attachment_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_attachment_names;
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
        m_coord_indices[i++] =
          static_cast<Uint>(boost::lexical_cast<int>(*it));
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
          } else j++;
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
        for (i = 0; i < size; ++i) {
          if (m_coord_indices[i] == static_cast<Uint>(-1)) m_num_primitives++;
        }
      }
      if (m_num_primitives == 0) goto err;
      elem->mark_delete(ai);
      continue;
    }

    if (name == "colorIndex") {
      Uint num_values = get_num_tokens(value);
      m_color_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i)
        svalue >> m_color_indices[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "normalIndex") {
      Uint num_values = get_num_tokens(value);
      m_normal_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i)
        svalue >> m_normal_indices[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "texCoordIndex") {
      Uint num_values = get_num_tokens(value);
      m_tex_coord_indices.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i)
        svalue >> m_tex_coord_indices[i];
      elem->mark_delete(ai);
      continue;
    }
    if (name == "primitiveType") {
      Uint num = sizeof(s_primitive_type_names) / sizeof(char*);
      const char** found = std::find(s_primitive_type_names,
                                     &s_primitive_type_names[num],
                                     strip_double_quotes(value));
      Uint index = found - s_primitive_type_names;
      if (index < num)
        set_primitive_type(static_cast<Primitive_type>(index));
      else
        std::cerr << "Illegal primitive type name (" << value << ")!"
                  << std::endl;
      elem->mark_delete(ai);
      continue;
    }
  }

  typedef Element::Cont_attr_iter       Cont_attr_iter;
  Cont_attr_iter cai;
  for (cai = elem->cont_attrs_begin(); cai != elem->cont_attrs_end(); ++cai) {
    const std::string& name = elem->get_name(cai);
    Shared_container cont = elem->get_value(cai);
    if (name == "coord") {
      Shared_coord_array coord_array =
        boost::dynamic_pointer_cast<Coord_array>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "normal") {
      Shared_normal_array normal_array =
        boost::dynamic_pointer_cast<Normal_array>(cont);
      set_normal_array(normal_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "color") {
      Shared_color_array color_array =
        boost::dynamic_pointer_cast<Color_array>(cont);
      set_color_array(color_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "texCoord") {
      Shared_tex_coord_array tex_coord_array =
        boost::dynamic_pointer_cast<Tex_coord_array>(cont);
      set_tex_coord_array(tex_coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
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
  field_changed(field_info);
}

//! \brief processes change of normals.
void Geo_set::normal_changed(const Field_info* field_info)
{ field_changed(field_info); }

//! \brief processes change of colors.
void Geo_set::color_changed(const Field_info* field_info)
{ field_changed(field_info); }

//! \brief processes change of texture coordinates.
void Geo_set::tex_coord_changed(const Field_info* field_info)
{ field_changed(field_info); }

//! \brief Process change of field.
void Geo_set::field_changed(const Field_info* field_info)
{
  switch (field_info->get_id()) {
   case COORD_ARRAY: m_dirty_sphere_bound = true; break;
   default: break;
  }
  Container::field_changed(field_info);
}

//! \brief assigns the coord indices with the reverse of given indices.
void Geo_set::set_reverse_coord_indices(const std::vector<Uint>& indices)
{
  m_coord_indices.resize(indices.size());
  Uint i = 0;
  std::vector<Uint>::const_reverse_iterator rit = indices.rbegin();
  for (++rit; rit < indices.rend(); ++rit)
    m_coord_indices[i++] = *rit;
  m_coord_indices[i++] = (Uint) -1;
}

//! \brief obtains the ith coordinate.
const Vector3f& Geo_set::get_coord_3d(Uint i) const
{
  boost::shared_ptr<Coord_array_3d> coords =
    boost::dynamic_pointer_cast<Coord_array_3d>(m_coord_array);
  SGAL_assertion(coords);
  return (*coords)[i];
}

SGAL_END_NAMESPACE
