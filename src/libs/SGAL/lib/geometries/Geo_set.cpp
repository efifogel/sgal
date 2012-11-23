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
// $Source$
// $Revision: 14198 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "SGAL/Geo_set.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Field_infos.hpp"

SGAL_BEGIN_NAMESPACE

Container_proto* Geo_set::s_prototype = NULL;

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

/*! Constructor */
Geo_set::Geo_set(Boolean proto) :
  Geometry(proto),
  m_dirty(true),
  m_num_primitives(0),
  m_normal_attachment(PER_VERTEX),
  m_color_attachment(PER_VERTEX),
  m_coord_array(NULL),
  m_normal_array(NULL),
  m_tex_coord_array(NULL),
  m_color_array(NULL),
  m_coord_indices(),
  m_tex_coord_indices(),
  m_normal_indices(),
  m_color_indices(),
  m_primitive_type(s_def_primitive_type),
  m_flatten_indices(false),
  m_are_indices_flat(false)
{
}

/*! Destructor */
Geo_set::~Geo_set() {}

/*! sets the attributes of this node */
void Geo_set::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Geometry::get_prototype());

  //! Container execution function
  typedef void (Container::*Execution_function)(Field_info*);

  // Add the field-info records to the prototype:
  Execution_function exec_func;

  exec_func = static_cast<Execution_function>(&Geo_set::coord_changed);
  SF_container* field;
  field = new SF_container(COORD_ARRAY, "coord",
                           get_member_offset(&m_coord_array), exec_func);
  s_prototype->add_field_info(field);
}

/*! */
void Geo_set::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! */
Container_proto* Geo_set::get_prototype() 
{  
  if (s_prototype == NULL) Geo_set::init_prototype();
  return s_prototype;
}

/*! Set the coordinate array
 * \param coord_array (in) a pointer to a coordinate array
 */
void Geo_set::set_coord_array(Coord_array* coord_array)
{
  Observer observer(this, get_field_info(COORD_ARRAY));
  if (m_coord_array) m_coord_array->unregister_observer(observer);
  m_coord_array = coord_array;
  m_coord_array->register_observer(observer);
  m_is_sphere_bound_dirty = true;
}

/*! Set the normal array
 * \param coord_array (in) a pointer to a normal array
 */
void Geo_set::set_normal_array(Normal_array* normal_array)
{ m_normal_array = normal_array; }

/*! Set the texture-coordinate array
 * \param tex_coord_array (in) a pointer to a texture-coordinate array
 */
void Geo_set::set_tex_coord_array(Tex_coord_array* tex_coord_array)
{ m_tex_coord_array = tex_coord_array; }

/*! Set the color array
 * \param color_array (in) a pointer to a color array
 */
void Geo_set::set_color_array(Color_array* color_array)
{ m_color_array = color_array; }

/*! Return true if the representation is empty */
Boolean Geo_set::is_empty() const
{
  return (!m_coord_array || (m_coord_array->size() == 0) );
}

/*! Calculate the sphere bound of the geometry set. Returns true if the BS has
 * changed since lst time this was called.
 */
Boolean Geo_set::calculate_sphere_bound()
{
  if (!m_is_sphere_bound_dirty) return false;

  if (!m_bb_is_pre_set && m_coord_array)
    m_sphere_bound.set_around(m_coord_array->begin(), m_coord_array->end());
  m_is_sphere_bound_dirty = false;
  return true;
}

/*! Proces the indices (in places) */
void Geo_set::flatten_indices(Uint* src, Uint* dst, Uint num)
{
  if (m_primitive_type == PT_TRIANGLES) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < num; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      k++;
    }
  }
  if (m_primitive_type == PT_QUADS) {
    Uint i, j, k;
    for (j = 0, i = 0, k = 0; j < num; ++j) {
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      dst[i++] = src[k++];
      k++;
    }
  }
}

/*! Process the indices
 * In case of triangles or quads remove the '-1' end-of-polygon indication
 * from the index buffers. This operation changes the the structure of the
 * index buffers, and must be reflected in the drawing routines.
 */
void Geo_set::flatten_indices()
{
  Uint size = (m_primitive_type == PT_TRIANGLES) ? m_num_primitives * 3 :
    (m_primitive_type == PT_QUADS) ? m_num_primitives * 4 : 0;
  if (!size) return;

  Uint* indices = m_coord_indices.get_vector();
  flatten_indices(indices, indices, m_num_primitives);
  m_coord_indices.resize(size);

  if (m_tex_coord_indices.size()) {
    Uint* indices = m_tex_coord_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_tex_coord_indices.resize(size);
  }

  if (m_normal_indices.size() && m_normal_attachment == PER_VERTEX) {
    Uint* indices = m_normal_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_normal_indices.resize(size);
  }

  if (m_color_indices.size() && m_color_attachment == PER_VERTEX) {
    Uint* indices = m_color_indices.get_vector();
    flatten_indices(indices, indices, m_num_primitives);
    m_color_indices.resize(size);
  }
  m_are_indices_flat = true;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 */
void Geo_set::set_attributes(Element* elem)
{
  Geometry::set_attributes(elem);

  typedef Element::Str_attr_iter        Str_attr_iter;
  typedef Element::Cont_attr_iter       Cont_attr_iter;

  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
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
      } else if (m_primitive_type == PT_LINES) {
        if ((size & 0x1) != 0) goto err;
        m_num_primitives = size >> 1;
      } else if (m_primitive_type == PT_LINE_STRIPS) {
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

  for (Cont_attr_iter cai = elem->cont_attrs_begin();
       cai != elem->cont_attrs_end(); cai++)
  {
    const std::string& name = elem->get_name(cai);
    Container* cont = elem->get_value(cai);
    if (name == "coord") {
      Coord_array* coord_array = dynamic_cast<Coord_array*>(cont);
      set_coord_array(coord_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "normal") {
      Normal_array* normal_array = dynamic_cast<Normal_array*>(cont);
      set_normal_array(normal_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "color") {
      Color_array* color_array = dynamic_cast<Color_array*>(cont);
      set_color_array(color_array);
      elem->mark_delete(cai);
      continue;
    }
    if (name == "texCoord") {
      Tex_coord_array* tex_coord_array = dynamic_cast<Tex_coord_array*>(cont);
      set_tex_coord_array(tex_coord_array);
      elem->mark_delete(cai);
      continue;
    }
  }

  // Remove all the deleted attributes:
  elem->delete_marked();
}

void Geo_set::clean()
{
  if (!m_are_indices_flat && m_flatten_indices) flatten_indices();
  m_dirty = false;
}

SGAL_END_NAMESPACE
