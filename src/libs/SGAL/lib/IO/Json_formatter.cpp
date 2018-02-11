// Copyright (c) 2018 Israel.
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
 */

#include <iterator>
#include <utility>

#include "SGAL/basic.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Json_formatter.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Tex_coord_array.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Json_formatter::Json_formatter(const std::string& filename) :
  Text_formatter(filename)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
}

//! \brief constructs an output formatter.
Json_formatter::Json_formatter(const std::string& filename, std::ostream& os) :
  Text_formatter(filename, os)
{
  Shared_matrix4f mat(new Matrix4f);
  m_matrices.push(mat);
  m_uuid = boost::uuids::random_generator()();
}

//! \brief constructs an input formatter.
Json_formatter::Json_formatter(const std::string& filename, std::istream& is) :
  Text_formatter(filename, is)
{}

//! \brief destruct.
Json_formatter::~Json_formatter()
{
  m_geometries.clear();
  m_apperances.clear();
}

//! \brief writes the begin statement.
void Json_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  object_begin();
}

//! \brief writes the end statement.
void Json_formatter::end()
{
  single_object("metadata",
                [&]() {
                  attribute("version", "1.0");
                  object_separator();
                  attribute("type", "Object");
                  object_separator();
                  attribute ("generator", "SGAL");
                });
  object_separator();
  multiple_object("geometries",
                  [&](){
                    for (auto item : m_geometries) {
                      auto geom_op =
                        std::bind(&Json_formatter::export_geometry, this,
                                  item.first, item.second);
                      single_object_body(geom_op);
                      object_separator();
                    }
                  });
  object_separator();
  multiple_object("materials",
                  [&](){
                    for (auto item : m_apperances) {
                      auto material_op =
                        std::bind(&Json_formatter::export_material, this,
                                  item.first, item.second);
                      single_object_body(material_op);
                      object_separator();
                    }
                  });
  object_separator();
  auto object_op = std::bind(&Json_formatter::export_object, this);
  single_object("object", object_op);
  new_line();
  object_end();
  new_line();
}

//! \brief writes a vertex.
void Json_formatter::vertex(const Vector3f& p, bool compact)
{
  if (compact) {
    out() << p[0] << "," << p[1] << "," << p[2];
    return;
  }
  indent();
  out() << p[0] << "," << p[1] << "," << p[2];
  new_line();
}

//! \brief writes a triangular facet.
void Json_formatter::facet(size_t i1, size_t i2, size_t i3)
{ out() << i1 << "," << i2 << "," << i3; }

//! \brief writes a scene-graph container.
void Json_formatter::write(Shared_container container)
{
  auto transform = boost::dynamic_pointer_cast<Transform>(container);
  if (transform) {
    // Push the transform matrix
    const auto& curr_mat = transform->get_matrix();
    auto last_mat = m_matrices.top();
    Shared_matrix4f next_mat(new Matrix4f);
    next_mat->mult(*last_mat, curr_mat);
    m_matrices.push(next_mat);

    // Process the children.
    transform->write(this);

    // Pop the transform matrix
    m_matrices.pop();
    return;
  }
  auto shape = boost::dynamic_pointer_cast<Shape>(container);
  if (shape) {
    // Observe that we push the shape even if it is not visible.
    m_shapes.push_back(std::make_pair(shape, m_matrices.top()));
    return;
  }
  container->write(this);
}

void Json_formatter::out_string(const std::string& name)
{ out() << '\"' << name << '\"'; }

void Json_formatter::name_value_separator() { out() << ": "; }

void Json_formatter::object_separator(bool compact)
{
  if (compact) {
    out() << ",";
    return;
  }
  out() << ",";
  new_line();
}

void Json_formatter::object_begin()
{
  indent();
  out() << "{";
  new_line();
  push_indent();
}

void Json_formatter::object_end()
{
  new_line();
  pop_indent();
  indent();
  out() << "}";
}

void Json_formatter::array_begin(bool compact)
{
  if (compact) {
    out() << "[";
    return;
  }
  indent();
  out() << "[";
  new_line();
  push_indent();
}

void Json_formatter::array_end(bool compact)
{
  if (compact) {
    out() << "]";
    return;
  }
  new_line();
  pop_indent();
  indent();
  out() << "]";
}

void Json_formatter::export_object()
{
  attribute("uuid", boost::uuids::to_string(m_uuid));
  object_separator();
  attribute("type", "Scene");
  object_separator();
  // Matrix4f identity;
  multiple_object("matrix",
                  [&]() { out() << "1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1"; }, true);
  object_separator();
  multiple_object("children",
                  [&]() {
                    for (auto world_shape : m_shapes) {
                      auto shape = world_shape.first;
                      auto matrix = world_shape.second;
                      const auto& name = shape->get_name();
                      if (! name.empty()) {
                        attribute("name", name);
                        object_separator();
                      }
                      attribute("uuid", boost::uuids::to_string(m_uuid));

                      // "matrix": [1,0,0,0,0,0,-1,0,-0,1,0,0,-2e-06,-6.71175,3.01972,1],
                      // "visible": true,
                      attribute("type", "Mesh");
                      object_separator();
                      auto geometry = shape->get_geometry();
                      auto git = m_geometries.find(geometry);
                      attribute("geometry", git->second);
                      auto app = shape->get_appearance();
                      auto ait = m_apperances.find(app);
                      attribute("material", ait->second);
                    }
                  });
}

Uint Json_formatter::to_hex(const Vector3f& color)
{
  auto red = static_cast<size_t>(color[0] * 255);;
  auto green = static_cast<size_t>(color[1] * 255);;
  auto blue = static_cast<size_t>(color[2] * 255);;

  Uint r = (red << 16) & 0xff0000;
  Uint g = (green << 8) & 0x00ff00;
  Uint b = (blue << 0) & 0x0000ff;
  return r | g | b;
}

void Json_formatter::export_material(Shared_apperance appearance, String& id)
{
  attribute("uuid", id);
  object_separator();
  attribute("type", "MeshPhongMaterial");
  object_separator();
  const auto& name = appearance->get_name();
  if (! name.empty()) {
    attribute("name", name);
    object_separator();
  }
  //"vertexColors": false,
  //"depthTest": true,
  //"depthWrite": true,
  auto material = appearance->get_material();
  if (material) {
    attribute("color", to_hex(material->get_diffuse_color()));
    object_separator();
    attribute("specular", to_hex(material->get_specular_color()));
    object_separator();
    attribute("emissive", to_hex(material->get_emissive_color()));
    object_separator();
    attribute("ambient", to_hex(material->get_ambient_color()));
    object_separator();
    attribute("shininess", material->get_shininess());
    object_separator();
  }
  attribute("blending", "NormalBlending");
}

void Json_formatter::export_geometry(Shared_geometry geometry, String& id)
{
  attribute("uuid", id);
  object_separator();
  attribute("type", "Geometry");
  object_separator();
  auto geometry_data_op =
    std::bind(&Json_formatter::export_geometry_data, this, geometry);
  single_object("data", geometry_data_op);
}

void Json_formatter::export_geometry_data(Shared_geometry geometry)
{
  auto mesh_set = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  if (mesh_set) {
    const auto& name = geometry->get_name();
    if (! name.empty()) {
      attribute("name", name);
      object_separator();
    }
    //! \todo Ensure that all attachments are per-vertex.
    //! \todo Ensure that the mesh is represented by triangles.
    auto coord_array =
      boost::dynamic_pointer_cast<Coord_array_3d>(mesh_set->get_coord_array());
    const auto& indices = mesh_set->get_facet_coord_indices();
    if (!coord_array || coord_array->empty() || mesh_set->empty_facet_indices(indices))
    {
      single_object("metadata",
                    [&]() {
                      attribute("version", "1.0");
                      object_separator();
                      attribute("vertices", 0);
                    });
      return;
    }

    auto normal_array = mesh_set->get_normal_array();
#if 0
    auto color_array = mesh_set->get_normal_array();
    auto tex_coord_array = mesh_set->get_tex_coord_array();
#endif

    single_object("metadata",
                  [&]() {
                    attribute("version", "1.0");
                    object_separator();
                    attribute("vertices", coord_array->size());
                    if (normal_array && ! normal_array->empty()) {
                      object_separator();
                      attribute ("normals", normal_array->size());
                    }
                    // object_separator();
                    //attribute ("uvs", tex_coord_array->size());
                    object_separator();
                    attribute ("faces", mesh_set->get_num_primitives());
                  });
    object_separator();
    multiple_object("vertices",
                    [&]() {
                      auto it = coord_array->begin();
                      vertex(*it, true);
                      for (++it; it != coord_array->end(); ++it) {
                        object_separator(true);
                        vertex(*it, true);
                      }
                    },
                    true);
    if (normal_array && ! normal_array->empty()) {
      object_separator();
      multiple_object("normals",
                      [&]() {
                        auto it = coord_array->begin();
                        vertex(*it, true);
                        for (++it; it != coord_array->end(); ++it) {
                          object_separator(true);
                          vertex(*it, true);
                        }
                      },
                      true);
    }
    object_separator();
    multiple_object("faces",
                    [&]() {
                      const auto& tris = mesh_set->triangle_coord_indices();
                      size_t i(0);
                      facet(tris[i][0], tris[i][1], tris[i][2]);
                      for (++i; i < mesh_set->get_num_primitives(); ++i) {
                        object_separator(true);
                        facet(tris[i][0], tris[i][1], tris[i][2]);
                      }
                    },
                    true);
  }
}

void Json_formatter::pre_process(const std::list<Shared_container>& containers,
                                 const std::map<std::string, Shared_container>& instances)
{
  for (auto container : containers) {
    auto geom = boost::dynamic_pointer_cast<Geometry>(container);
    if (geom) {
      m_geometries[geom] = boost::uuids::to_string(m_uuid);
      continue;
    }

    auto app = boost::dynamic_pointer_cast<Appearance>(container);
    if (app) {
      m_apperances[app] = boost::uuids::to_string(m_uuid);
      continue;
    }
  }

  for (auto instance : instances) {
    auto geom = boost::dynamic_pointer_cast<Geometry>(instance.second);
    if (geom) {
      m_geometries[geom] = boost::uuids::to_string(m_uuid);
      continue;
    }

    auto app = boost::dynamic_pointer_cast<Appearance>(instance.second);
    if (app) {
      m_apperances[app] = boost::uuids::to_string(m_uuid);
      continue;
    }
  }

}

SGAL_END_NAMESPACE
