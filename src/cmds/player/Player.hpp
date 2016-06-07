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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "SGAL/sgal.hpp"
#include "SGAL/Polyhedron_attributes_array.hpp"

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"

class Player {
public:
  typedef std::vector<std::string>      Arguments;

  /*! Construct default. */
  Player();

  /*! Construct. */
  Player(int argc, char* argv[]);

  /*! Construct. */
  Player(Arguments args);

  /*! Destruct. */
  ~Player();

  /*! Initialize the player. */
  void init(int argc, char* argv[]);

  /*! Initialize the player. */
  void init(Arguments args);

  /*! Clean the player. */
  void clean();

  /*! Operator */
  void operator()();

  /*! Operator */
  void operator()(char* data, int size);

  void create();
  void create(char* data, int size);
  void visualize();
  void destroy();

  /*! Obtain the accumulated volume of all polyhedrons in the scene.
   */
  float volume();

  /*! Obtain the accumulated surface area of all polyhedrons in the scene.
   */
  float surface_area();

  /*! Obtain the attributes of all polyhedrons.
   */
  const SGAL::Polyhedron_attributes_array& get_polyhedron_attributes_array();

private:
  /*! The scene. */
  Player_scene m_scene;

  /*! The option parser. */
  Player_option_parser* m_option_parser;

  /*! The construct that holds the attributes of all polyhedrons. */
  SGAL::Polyhedron_attributes_array m_polyhedron_attributes_array;
};

#endif
