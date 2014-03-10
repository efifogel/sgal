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

#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container.hpp"

#include "SGAL/Accumulation.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Collision.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Cube_environment.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Font_style.hpp"
#include "SGAL/Halftone.hpp"
#include "SGAL/Image.hpp"
#include "SGAL/Image_texture.hpp"
#include "SGAL/Image_sphere_environment.hpp"
// #include "SGAL/Image_texture_WT.hpp"
#include "SGAL/Incrementor.hpp"
#include "SGAL/Navigation_info.hpp"
// #include "SGAL/Text_texture.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Route.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Sphere_environment.hpp"
#include "SGAL/Sphere_plane_intersection.hpp"
#include "SGAL/Switch.hpp"
// #include "SGAL/Lod.hpp"
#include "SGAL/Tex_gen.hpp"
#include "SGAL/Text.hpp"
#include "SGAL/Texture_2d.hpp"
#include "SGAL/Texture_transform.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/World_info.hpp"

#include "SGAL/Coord_transformer.hpp"
#include "SGAL/Coord_minkowski.hpp"
#include "SGAL/Color_array.hpp"
#include "SGAL/Coord_array.hpp"
#include "SGAL/Exporter.hpp"
#include "SGAL/Extrusion.hpp"
#include "SGAL/Indexed_face_set.hpp"
#include "SGAL/Indexed_line_set.hpp"
#include "SGAL/Indexed_geodesic_set.hpp"
#include "SGAL/Normal_array.hpp"
#include "SGAL/Piece.hpp"
#include "SGAL/Tex_coord_array_2d.hpp"
#include "SGAL/Tex_coord_array_3d.hpp"

#include "SGAL/Sphere.hpp"
#include "SGAL/Cylinder.hpp"
#include "SGAL/Cylindrical_patch.hpp"
#include "SGAL/Arc.hpp"
#include "SGAL/Box.hpp"
#include "SGAL/Cone.hpp"
#include "SGAL/Ellipsoid.hpp"
#include "SGAL/Torus.hpp"
#include "SGAL/Geodesic.hpp"

#include "SGAL/Point_light.hpp"
#include "SGAL/Simulation.hpp"
#include "SGAL/Spot_light.hpp"
#include "SGAL/Directional_light.hpp"

// #include "SGAL/Flow_sensor.hpp"
#include "SGAL/Key_sensor.hpp"
#include "SGAL/Navigation_sensor.hpp"
#include "SGAL/Proximity_sensor.hpp"
#include "SGAL/Single_key_sensor.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Time_sensor.hpp"

#include "SGAL/Color_interpolator.hpp"
// #include "SGAL/Normal_interpolator.hpp"
#include "SGAL/Coordinate_interpolator.hpp"
#include "SGAL/Position_interpolator.hpp"
#include "SGAL/Rotation_interpolator.hpp"
#include "SGAL/Scalar_interpolator.hpp"
// #include "SGAL/Vector3f_interpolator.hpp"

#include "SGAL/Background.hpp"
#include "SGAL/Environment_background.hpp"
#include "SGAL/Color_background.hpp"
#include "SGAL/Image_background.hpp"

// #include "SGAL/Position_relative_engine.hpp"
// #include "SGAL/Rotation_relative_engine.hpp"

// #include "SGAL/Comp_scalar_interpolator.hpp"
// #include "SGAL/Comp_vector3f_interpolator.hpp"
// #include "SGAL/Comp_position_interpolator.hpp"
// #include "SGAL/Comp_normal_interpolator.hpp"
// #include "SGAL/Comp_color_interpolator.hpp"
// #include "SGAL/Comp_rotation_interpolator.hpp"

// #include "SGAL/Prog_indexed_tri_set.hpp"
#include "SGAL/Script.hpp"
#include "SGAL/Snapshot.hpp"
// #include "SGAL/Movie_recorder.hpp"
// #include "SGAL/Field_def.hpp"

SGAL_BEGIN_NAMESPACE

Container_factory* Container_factory::m_instance = 0;

/*! Returns a pointer to the factory and makes sure only one instance exits */
Container_factory* Container_factory::get_instance()
{
  if (!m_instance) {
    m_instance = new Container_factory();
    //! \todo m_destroyer.set_singleton(m_instance);
  }
  return m_instance;
}

/*! \brief registers a container to the factory. */
void Container_factory::doregister(Container* container)
{
  container->init_prototype();
  m_map[container->get_tag()] = container;
}

/*! \brief creates a container according to the type. */
Shared_container Container_factory::create(const std::string& type)
{
  Cont_iter iter = m_map.find(type);
  if (iter != m_map.end()) return Shared_container(iter->second->clone());

  // try to add the "sgal" prefix:
  // The "sgal" prefix is added to containers that differ from the standard
  std::string alt_name("sgal");
  alt_name += type;
  iter = m_map.find(alt_name);
  if (iter != m_map.end()) return Shared_container(iter->second->clone());
  return Shared_container();
}

/*! \brief registers all containers. */
void Container_factory::initialize()
{
  REGISTER_OBJECT(Accumulation);
  REGISTER_OBJECT(Appearance);
  REGISTER_OBJECT(Arc);
  REGISTER_OBJECT(Background);
  REGISTER_OBJECT(Box);
  REGISTER_OBJECT(Camera);
  REGISTER_OBJECT(Collision);
  REGISTER_OBJECT(Color_background);
  REGISTER_OBJECT(Color_array);
  REGISTER_OBJECT(Coord_array);
  REGISTER_OBJECT(Configuration);
  REGISTER_OBJECT(Cone);
  REGISTER_OBJECT(Coord_minkowski);
  REGISTER_OBJECT(Coord_transformer);
  REGISTER_OBJECT(Cube_environment);
  REGISTER_OBJECT(Cylinder);
  REGISTER_OBJECT(Cylindrical_patch);
  REGISTER_OBJECT(Directional_light);
  REGISTER_OBJECT(Ellipsoid);
  REGISTER_OBJECT(Exporter);
  REGISTER_OBJECT(Extrusion);
  REGISTER_OBJECT(Environment_background);
  REGISTER_OBJECT(Font_style);
  REGISTER_OBJECT(Group);
  REGISTER_OBJECT(Geodesic);
  REGISTER_OBJECT(Halftone);
  REGISTER_OBJECT(Image);
  REGISTER_OBJECT(Image_background);
  REGISTER_OBJECT(Image_sphere_environment);
  REGISTER_OBJECT(Image_texture);
  REGISTER_OBJECT(Incrementor);
  REGISTER_OBJECT(Indexed_face_set);
  REGISTER_OBJECT(Indexed_geodesic_set);
  REGISTER_OBJECT(Indexed_line_set);
  REGISTER_OBJECT(Key_sensor);
  REGISTER_OBJECT(Material);
  REGISTER_OBJECT(Multisample);
  REGISTER_OBJECT(Navigation_info);
  REGISTER_OBJECT(Navigation_sensor);
  REGISTER_OBJECT(Normal_array);
  REGISTER_OBJECT(Piece);
  REGISTER_OBJECT(Point_light);
  REGISTER_OBJECT(Coordinate_interpolator);
  REGISTER_OBJECT(Position_interpolator);
  REGISTER_OBJECT(Proximity_sensor);
  REGISTER_OBJECT(Rotation_interpolator);
  REGISTER_OBJECT(Route);
  REGISTER_OBJECT(Shape);
  REGISTER_OBJECT(Simulation);
  REGISTER_OBJECT(Single_key_sensor);
  REGISTER_OBJECT(Snapshot);
  REGISTER_OBJECT(Sphere);
  REGISTER_OBJECT(Sphere_environment);
  REGISTER_OBJECT(Sphere_plane_intersection);
  REGISTER_OBJECT(Spot_light);
  REGISTER_OBJECT(Switch);
  REGISTER_OBJECT(Tex_coord_array_2d);
  REGISTER_OBJECT(Tex_coord_array_3d);
  REGISTER_OBJECT(Tex_gen);
  REGISTER_OBJECT(Text);
  REGISTER_OBJECT(Texture_2d);
  REGISTER_OBJECT(Texture_transform);
  REGISTER_OBJECT(Time_sensor);
  REGISTER_OBJECT(Touch_sensor);
  REGISTER_OBJECT(Torus);
  REGISTER_OBJECT(Transform);
  REGISTER_OBJECT(World_info);
  REGISTER_OBJECT(Scalar_interpolator);
  // REGISTER_OBJECT(Image_texture_WT);
  // REGISTER_OBJECT(Text_texture);
  // REGISTER_OBJECT(Lod);
  // REGISTER_OBJECT(Flow_sensor);
  REGISTER_OBJECT(Color_interpolator);
  // REGISTER_OBJECT(Normal_interpolator);
  // REGISTER_OBJECT(Vector3f_interpolator);
  // REGISTER_OBJECT(Position_relative_engine);
  // REGISTER_OBJECT(Rotation_relative_engine);
  // REGISTER_OBJECT(Comp_scalar_interpolator);
  // REGISTER_OBJECT(Comp_vector3f_interpolator);
  // REGISTER_OBJECT(Comp_position_interpolator);
  // REGISTER_OBJECT(Comp_normal_interpolator);
  // REGISTER_OBJECT(Comp_color_interpolator);
  // REGISTER_OBJECT(Comp_rotation_interpolator);
  // REGISTER_OBJECT(Prog_indexed_tri_set);
  REGISTER_OBJECT(Script);
  // REGISTER_OBJECT(Movie_recorder);
  // REGISTER_OBJECT(Field_def);
}

SGAL_END_NAMESPACE
