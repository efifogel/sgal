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
#include <bitset>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/version.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Coord_array_3d.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Json_formatter.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Mesh_set.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Vector3f.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Frustum.hpp"
#include "SGAL/Spot_light.hpp"
#include "SGAL/Point_light.hpp"
#include "SGAL/Directional_light.hpp"

SGAL_BEGIN_NAMESPACE

//! \brief constructs.
Json_formatter::Json_formatter(const std::string& filename) :
  Text_formatter(filename),
  m_separated(true),
  m_camera(nullptr)
{ m_identity_matrix.make_identity(); }

//! \brief constructs an output formatter.
Json_formatter::Json_formatter(const std::string& filename, std::ostream& os) :
  Text_formatter(filename, os),
  m_separated(true),
  m_camera(nullptr)
{ m_identity_matrix.make_identity(); }

//! \brief constructs an input formatter.
Json_formatter::Json_formatter(const std::string& filename, std::istream& is) :
  Text_formatter(filename, is),
  m_separated(true),
  m_camera(nullptr)
{
  m_identity_matrix.make_identity();
}

//! \brief destruct.
Json_formatter::~Json_formatter()
{
  m_geometries.clear();
  m_apperances.clear();
}

//! \brief pre-processes the formatter.
void Json_formatter::
pre_process(Camera* camera,
            const std::list<Shared_container>& containers,
            const std::map<std::string, Shared_container>& instances)
{
  m_camera = camera;

  for (auto container : containers) {
    // std::cout << container->get_tag() << std::endl;
    auto geom = boost::dynamic_pointer_cast<Geometry>(container);
    if (geom) {
      auto uuid = boost::uuids::random_generator()();
      m_geometries[geom] = boost::uuids::to_string(uuid);
      continue;
    }

    auto app = boost::dynamic_pointer_cast<Appearance>(container);
    if (app) {
      auto uuid = boost::uuids::random_generator()();
      m_apperances[app] = boost::uuids::to_string(uuid);
      continue;
    }
  }

  for (auto instance : instances) {
    // std::cout << instance.second->get_tag() << std::endl;
    auto geom = boost::dynamic_pointer_cast<Geometry>(instance.second);
    if (geom) {
      auto uuid = boost::uuids::random_generator()();
      m_geometries[geom] = boost::uuids::to_string(uuid);
      continue;
    }

    auto app = boost::dynamic_pointer_cast<Appearance>(instance.second);
    if (app) {
      auto uuid = boost::uuids::random_generator()();
      m_apperances[app] = boost::uuids::to_string(uuid);
      continue;
    }
  }

}

//! \brief converts a color to its hexadecimal representation.
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

//! \brief writes the begin statement.
void Json_formatter::begin()
{
  SGAL_assertion(m_out != nullptr);
  // m_old_out_mode = get_mode(*m_out);
  // set_ascii_mode(*m_out);

  object_begin();
  attribute_single("metadata",
                   [&]() {
                     attribute("version", "1.0");
                     attribute("type", "Object");
                     attribute("generator", "SGAL");
                   });
  attribute_multiple("geometries",
                     [&](){
                       for (auto item : m_geometries) {
                         auto geom_op =
                           std::bind(&Json_formatter::export_geometry, this,
                                     item.first, item.second);
                         single_object(geom_op);
                       }
                     });
  attribute_multiple("materials",
                     [&](){
                       for (auto item : m_apperances) {
                         auto material_op =
                           std::bind(&Json_formatter::export_material, this,
                                     item.first, item.second);
                         single_object(material_op);
                       }
                     });

  object_separator();
  indent();
  out_string("object");
  name_value_separator();
  object_begin();
  auto uuid = boost::uuids::random_generator()();
  attribute("uuid", boost::uuids::to_string(uuid));
  attribute("type", "Scene");
  attribute_multiple("matrix", [&](){ export_matrix(m_identity_matrix); }, true);
  object_separator();
  indent();
  out_string("children");
  name_value_separator();
  array_begin();
}

//! \brief writes the end statement.
void Json_formatter::end()
{
  array_end();
  object_end();
  object_end();
  new_line();
}

//! \brief exports a 3D point.
void Json_formatter::vertex(const Vector3f& p, bool compact)
{
  object_separator(compact);
  if (!compact) indent();
  out() << p[0] << "," << p[1] << "," << p[2];
  if (!compact) new_line();
  m_separated = false;
}

//! brief exports a 2D point.
void Json_formatter::vertex(const Vector2f& p, bool compact)
{
  object_separator(compact);
  if (!compact) indent();
  out() << p[0] << "," << p[1];
  if (!compact) new_line();
  m_separated = false;
}

//! \brief writes a triangular facet.
void Json_formatter::facet(Uint flags, const std::array<Index_type, 3>& tri,
                           Uint i, bool compact)
{
  object_separator(compact);
  if (!compact) indent();
  out() << flags << "," << tri[0] << "," << tri[1] << "," << tri[2] << "," << i;
  if (!compact) new_line();
  m_separated = false;
}

//! \brief writes a scene-graph container.
void Json_formatter::write(Shared_container container)
{
  // std::cout << "Json_formatter::write(): " << container->get_tag() << std::endl;
  auto group = boost::dynamic_pointer_cast<Group>(container);
  if (group) {
    single_object([&](){ export_group(group); });
    return;
  }

  auto shape = boost::dynamic_pointer_cast<Shape>(container);
  if (shape) {
    single_object([&](){ export_mesh(shape); });

    return;
  }

  auto light = boost::dynamic_pointer_cast<Light>(container);
  if (light) {
    single_object([&](){ export_light(light); });
    return;
  }

  container->write(this);
}

void Json_formatter::out_string(const char* str)
{ out() << '\"' << str << '\"'; }

//! \brief exports a separator between the name of an attribute and its value.
void Json_formatter::name_value_separator() { out() << ": "; }

//! \brief exports a separator between consecutive objects.
void Json_formatter::object_separator(bool compact)
{
  if (m_separated) return;
  out() << ",";
  if (! compact) new_line();
  m_separated = true;
}

void Json_formatter::object_begin()
{
  indent();
  out() << "{";
  new_line();
  push_indent();
  m_separated = true;
}

void Json_formatter::object_end()
{
  new_line();
  pop_indent();
  indent();
  out() << "}";
  m_separated = false;
}

void Json_formatter::array_begin(bool compact)
{
  if (compact) {
    out() << "[";
    m_separated = true;
    return;
  }
  indent();
  out() << "[";
  m_separated = true;
  new_line();
  push_indent();
}

void Json_formatter::array_end(bool compact)
{
  if (compact) {
    out() << "]";
    m_separated = false;
    return;
  }
  new_line();
  pop_indent();
  indent();
  out() << "]";
  m_separated = false;
}

//! Export a matrix.
void Json_formatter::export_matrix(const Matrix4f& matrix)
{
  for (auto i = 0; i != 4; ++i)
    for (auto j = 0; j != 4; ++j)
      single_value(matrix.get(i, j), true);
}

//! exports the camera.
void Json_formatter::export_camera()
{
  auto uuid = boost::uuids::random_generator()();
  attribute("uuid", boost::uuids::to_string(uuid));
  const auto& name = m_camera->get_name();
  if (! name.empty()) attribute("name", name);

  auto& frustum = m_camera->get_frustum();

  // The viewing matrix of the camera is the OpenGL matrix, which seems to be
  // different than the one we need, so build it from scratch:

  Matrix4f mat;
  mat.make_identity();
  mat.set_col(0, m_camera->get_right());
  mat.set_col(1, m_camera->get_up());
  mat.set_col(2, m_camera->get_forward());

  // rotation:
  auto rotation = m_camera->get_orientation();
  const auto& newz = rotation.get_axis();
  const auto angle = rotation.get_angle();
  mat.post_rot(mat, newz[0], newz[1], newz[2], angle);

  // Position
  Vector3f translation = m_camera->get_position();
  translation.xform_pt(translation, mat);
  mat.set_row(3, translation);
  attribute_multiple("matrix", [&](){ export_matrix(mat); }, true);
  auto type = frustum.get_type();
  attribute("type", ((Frustum::ORTHOGONAL == type) ?
                     "OrthogonalCamera" : "PerspectiveCamera"));
  attribute("fov", rad2deg(frustum.get_fov()));
  attribute("aspect", frustum.get_aspect_ratio());
  Float near_dist, far_dist;
  frustum.get_near_far(near_dist, far_dist);
  attribute("near", near_dist);
  attribute("far", far_dist);
}

//! exports a group.
void Json_formatter::export_group(Shared_group group)
{
  auto uuid = boost::uuids::random_generator()();
  attribute("uuid", boost::uuids::to_string(uuid));
  attribute("type", group->get_tag());
  auto transform = boost::dynamic_pointer_cast<Transform>(group);
  const auto& mat = (transform) ? transform->get_matrix() : m_identity_matrix;
  attribute_multiple("matrix", [&](){ export_matrix(mat); }, true);
  attribute_multiple("children", [&](){ group->write(this); });
}

//! \brief exports a light source.
void Json_formatter::export_light(Shared_light light)
{
  auto uuid = boost::uuids::random_generator()();
  attribute("uuid", boost::uuids::to_string(uuid));
  const auto& name = light->get_name();
  if (! name.empty()) attribute("name", name);
  attribute("intensity", light->get_intensity());
  attribute("color", to_hex(light->get_color()));
  attribute("type", light->get_tag());

  Matrix4f mat;
  mat.make_identity();
  auto spot_light = boost::dynamic_pointer_cast<Spot_light>(light);
  if (spot_light) {
    const auto& direction = spot_light->get_direction();
    //! \todo do something with the direction.
    Vector3f translation(spot_light->get_location());
    translation.xform_pt(translation, mat);
    mat.set_row(3, translation);
  }
  attribute_multiple("matrix", [&](){ export_matrix(mat); }, true);
  light->write(this);
}

//! \brief exports a mesh.
void Json_formatter::export_mesh(Shared_shape shape)
{
  auto uuid = boost::uuids::random_generator()();
  attribute("uuid", boost::uuids::to_string(uuid));
  const auto& name = shape->get_name();
  if (! name.empty()) attribute("name", name);

  Matrix4f mat;
  mat.make_identity();
  attribute_multiple("matrix", [&](){ export_matrix(mat); }, true);
  attribute("visible", shape->is_visible());
  attribute("type", "Mesh");
  auto geometry = shape->get_geometry();
  auto git = m_geometries.find(geometry);
  if (git == m_geometries.end())
    std::cerr << "ERROR: Failed to find geometry!" << std::endl;
  else attribute("geometry", git->second);
  auto app = shape->get_appearance();
  auto ait = m_apperances.find(app);
  if (ait == m_apperances.end())
    std::cerr << "ERROR: Failed to find material!" << std::endl;
  else attribute("material", ait->second);
  shape->write(this);
}

//! \brief exports the main scene object.
void Json_formatter::export_scene()
{
  // auto uuid = boost::uuids::random_generator()();
  // attribute("uuid", boost::uuids::to_string(uuid));
  // attribute("type", "Scene");
  // // Matrix4f identity;
  // attribute_multiple("matrix",
  //                    [&](){ out() << "1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1"; }, true);
  // attribute_multiple("children",
  //                    [&](){
  //                      if (m_camera) {
  //                        auto camera_op =
  //                          std::bind(&Json_formatter::export_camera, this);
  //                        single_object(camera_op);
  //                      }
  //                      for (auto world_shape : m_shapes) {
  //                        auto shape_op =
  //                          std::bind(&Json_formatter::export_mesh, this,
  //                                    world_shape);
  //                        single_object(shape_op);
  //                      }
  //                    });
}

void Json_formatter::export_material(Shared_apperance appearance, String& id)
{
  attribute("uuid", id);
  attribute("type", "MeshPhongMaterial");
  const auto& name = appearance->get_name();
  if (! name.empty()) attribute("name", name);
  //"vertexColors": false,
  //"depthTest": true,
  //"depthWrite": true,
  auto material = appearance->get_material();
  if (material) {
    attribute("color", to_hex(material->get_diffuse_color()));
    attribute("specular", to_hex(material->get_specular_color()));
    attribute("emissive", to_hex(material->get_emissive_color()));
    attribute("ambient", to_hex(material->get_ambient_color()));
    attribute("shininess", material->get_shininess());
  }
  attribute("blending", "NormalBlending");
}

//! \brief exports the attributes of a geometry object.
void Json_formatter::export_geometry(Shared_geometry geometry, String& id)
{
  attribute("uuid", id);
  attribute("type", "Geometry");
  auto geometry_data_op =
    std::bind(&Json_formatter::export_geometry_data, this, geometry);
  attribute_single("data", geometry_data_op);
}

void Json_formatter::export_geometry_data(Shared_geometry geometry)
{
  auto mesh_set = boost::dynamic_pointer_cast<Mesh_set>(geometry);
  if (mesh_set) {
    const auto& name = geometry->get_name();
    if (! name.empty()) attribute("name", name);
    //! \todo Ensure that all attachments are per-vertex.
    //! \todo Ensure that the mesh is represented by triangles.
    auto coord_array =
      boost::dynamic_pointer_cast<Coord_array_3d>(mesh_set->get_coord_array());
    const auto& indices = mesh_set->get_facet_coord_indices();
    if (!coord_array || coord_array->empty() ||
        mesh_set->empty_facet_indices(indices))
    {
      attribute_single("metadata",
                       [&]() {
                         attribute("version", "1.0");
                         attribute("vertices", 0);
                       });
      return;
    }

    auto normal_array = mesh_set->get_normal_array();
    auto color_array = mesh_set->get_normal_array();

    auto tex_coord_array = mesh_set->get_tex_coord_array();
    auto tex_coord_array_2d =
      boost::dynamic_pointer_cast<Tex_coord_array_2d>(tex_coord_array);
    auto has_uvs = tex_coord_array_2d && ! tex_coord_array_2d->empty();

    attribute_single("metadata",
                     [&]() {
                       attribute("version", "1.0");
                       attribute("vertices", coord_array->size());
                       if (normal_array && ! normal_array->empty())
                         attribute("normals", normal_array->size());
                       if (has_uvs)
                         attribute ("uvs", tex_coord_array_2d->size());
                       attribute("faces", mesh_set->get_num_primitives());
                     });
    attribute_multiple("vertices",
                       [&]() {
                         for (const auto& v : *coord_array) vertex(v, true);
                       },
                       true);

    // Export normals:
    if (normal_array && ! normal_array->empty())
      attribute_multiple("normals",
                         [&]() {
                           for (const auto& n : *normal_array) vertex(n, true);
                         },
                      true);
    // Export texture coordinates:
    if (has_uvs)
      attribute_multiple("uvs",
                         [&]() {
                           for (const auto& v : *tex_coord_array_2d)
                             vertex(v, true);
                         },
                         true);

    std::bitset<F_SIZE> flags;
    switch (mesh_set->get_primitive_type()) {
     case Geo_set::PT_TRIANGLES: break;
     case Geo_set::PT_QUADS: flags[F_IS_TRI] = 1; break;

     case Geo_set::PT_POLYGONS:
     default: std::cerr << "ERROR: general polygons are not supported!"
                        << std::endl;
    }

    // We do not use the had_material option.

    // todo check indices for indexing
    flags[F_HAS_UVS] = has_uvs;

    auto fragment_source = mesh_set->resolve_fragment_source();
    if (fragment_source == Geo_set::FS_NORMAL) {
      const auto& normal_indices = mesh_set->get_facet_normal_indices();
      if (! mesh_set->empty_facet_indices(normal_indices)) {
        //! \todo use normal index
      }
      switch (mesh_set->get_normal_attachment()) {
       case Geo_set::AT_PER_VERTEX: flags[F_FACE_VERTEX_NORMAL] = 1; break;
       case Geo_set::AT_PER_PRIMITIVE: flags[F_FACE_NORMAL] = 1; break;

       case Geo_set::AT_PER_MESH:
       default: SGAL_error();
      }
    }
    else {
      const auto& color_indices = mesh_set->get_facet_color_indices();
      if (! mesh_set->empty_facet_indices(color_indices)) {
        //! \todo use color index
      }
      switch (mesh_set->get_color_attachment()) {
       case Geo_set::AT_PER_VERTEX: flags[F_FACE_VERTEX_COLOR] = 1; break;
       case Geo_set::AT_PER_PRIMITIVE: flags[F_FACE_COLOR] = 1; break;
       case Geo_set::AT_PER_MESH: flags[F_FACE_COLOR] = 1; break;
       default: SGAL_error();
      }
    }
    std::cout << "flags: " << flags << std::endl;

    attribute_multiple("faces",
                       [&]() {
                         Uint i(0);
                         const auto& tris = mesh_set->triangle_coord_indices();
                         for (const auto& t : tris) facet(flags.to_ulong(), t, i++, true);
                       },
                       true);
  }
}

SGAL_END_NAMESPACE
