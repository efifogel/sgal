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
// $Id: Knot_scene.cpp 12379 2011-12-27 20:12:34Z efif $
// $Revision: 12379 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! \file
 */

#if defined(_WIN32)
#pragma warning ( disable : 4146 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4800 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <limits.h>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Appearance.hpp"
#include "SGAL/Material.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Writer.hpp"
#include "SGAL/Vrml_formatter.hpp"
#include "SGAL/Piece.hpp"
#include "SGAL/Array.hpp"
#include "SGAL/Time_sensor.hpp"
#include "SGAL/Position_interpolator.hpp"
#include "SGAL/Route.hpp"
#include "SGAL/Tick_event.hpp"

#if (defined USE_GLUT)
#include "SGLUT/Glut_window_item.hpp"
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_item.hpp"
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_item.hpp"
#include "SGAL/X11_window_manager.hpp"
#endif

#include "Knot_scene.hpp"
#include "Knot_option_parser.hpp"

/*! COlor names for debugging */
const char* Knot_scene::s_color_names[] = {
  "Orange", "Blue", "Red", "Yellow", "Purple", "Green"
};

// Initialize possible tranlational movements:
Knot_scene::Piece_position Knot_scene::s_directions[] = {
  { 1,  0,  0},
  { 0,  1,  0},
  { 0,  0,  1},
  {-1,  0,  0},
  { 0, -1,  0},
  { 0,  0, -1}
};

#define WIDTH                   7
#define HEIGHT                  5
#define DEPTH                   1

struct My_piece {
  SGAL::Short m_position[3];
  unsigned int m_compisition[DEPTH][HEIGHT][WIDTH];
};

struct Puzzle {
  My_piece* m_pieces[Knot_scene::NUMBER_OF_COLORS];
};

My_piece orange = {
  {0,1,2},
  {
    {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,0,0,0,1,1},
      {1,0,0,0,0,0,1},
      {1,1,0,1,1,1,1}
    }
  }
};

My_piece blue = {
  {0,1,4},
  {
    {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,0,0,0,1,1},
      {1,0,1,0,0,0,1},
      {1,1,1,1,0,1,1}
    }
  }
};

My_piece red = {
  {1,2,0},
  {
    {
      {1,1,0,1,0,1,1},
      {1,0,0,1,0,0,1},
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,0,1,1}
    }
  }
};

My_piece yellow = {
  {1,4,0},
  {
    {
      {1,1,0,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,0,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,1}
    }
  }
};

My_piece purple = {
  {2,0,1},
  {
    {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,0,0,0,1,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,0,1,1}
    }
  }
};

My_piece green = {
  {4,0,1},
  {
    {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,1}
    }
  }
};

Puzzle puzzle;

/*! Constructor */
Knot_scene::Knot_scene(Knot_option_parser& option_parser) :
  m_window_manager(NULL),
  m_window_item(NULL),
  m_scene_graph(NULL),
  m_root(NULL),
  m_navigation(NULL),
  m_context(NULL),
  m_option_parser(option_parser),
  m_volume_width(0),
  m_volume_height(0),
  m_volume_depth(0),
  m_head_pad_x(0), m_head_pad_y(0), m_head_pad_z(0),
  m_tail_pad_x(0), m_tail_pad_y(0), m_tail_pad_z(0),
  m_volume_trans(NULL),
  m_volume(NULL),
  m_next_free_state_index(0),
  m_next_free_state_block(NULL),
  m_num_invocations(0),
  m_max_level(0)
{
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    m_pieces[color] = NULL;
    m_transforms[color] = NULL;
  }

  Uint i;
  for (i = 0; i < 3; ++i) {
    m_time_sensors[i] = NULL;
    m_pos_interpolators[i] = NULL;
    m_time_routers[i] = NULL;
    m_pos_interpolator_routers[i] = NULL;
  }
}

/*! Destructor */
Knot_scene::~Knot_scene(void) {}

/*! \brie destroys the scene. */
void Knot_scene::destroy_scene()
{
  if (m_context) {
    delete m_context;
    m_context = NULL;
  }

  if (m_scene_graph) {
    delete m_scene_graph;
    m_scene_graph = NULL;
  }

  if (m_root) {
    delete m_root;
    m_root = NULL;
  }

  if (m_navigation) {
    delete m_navigation;
    m_navigation = NULL;
  }
  
  m_solution.clear();

  Uint i;
  for (i = 0; i < 3; ++i) {
    delete m_time_sensors[i];
    delete m_pos_interpolators[i];
    delete m_time_routers[i];
    delete m_pos_interpolator_routers[i];
  }
}
  
/*! \brie creates the scene. */
void Knot_scene::create_scene()
{
  puzzle.m_pieces[ORANGE] = &orange;
  puzzle.m_pieces[BLUE] = &blue;
  puzzle.m_pieces[RED] = &red;
  puzzle.m_pieces[YELLOW] = &yellow;
  puzzle.m_pieces[PURPLE] = &purple;
  puzzle.m_pieces[GREEN] = &green;
     
  std::vector<SGAL::Vector3f> colors;
  colors.resize(NUMBER_OF_COLORS);
  colors[ORANGE].set(1, 0.65f, 0);
  colors[BLUE].set(0, 0, 1);
  colors[RED].set(1, 0, 0);
  colors[YELLOW].set(1, 1, 0);
  colors[PURPLE].set(0.7f, 0, 0.7f);
  colors[GREEN].set(0, 1, 0);
  
  // Construct a Scene_graph:
  m_scene_graph = new SGAL::Scene_graph;
  m_root = new SGAL::Group;
  m_scene_graph->set_root(m_root);
  m_navigation = new SGAL::Transform;
  m_scene_graph->set_navigation_root(m_navigation);
  m_root->add_child(m_navigation);
  unsigned int n;
#if (defined _MSC_VER)
  Uint max_size = max(WIDTH, HEIGHT);
  max_size = max(max_size, static_cast<SGAL::Uint>(DEPTH));
#else
  Uint max_size = std::max(WIDTH, HEIGHT);
  max_size = std::max(max_size, static_cast<Uint>(DEPTH));
#endif
  m_head_pad_x = max_size;
  m_head_pad_y = max_size;
  m_head_pad_z = max_size;

  m_tail_pad_x = max_size;
  m_tail_pad_y = max_size;
  m_tail_pad_z = max_size;

  m_option_parser.get_head_pad_x(m_head_pad_x);
  m_option_parser.get_head_pad_y(m_head_pad_y);
  m_option_parser.get_head_pad_z(m_head_pad_z);
  
  m_option_parser.get_tail_pad_x(m_tail_pad_x);
  m_option_parser.get_tail_pad_y(m_tail_pad_y);
  m_option_parser.get_tail_pad_z(m_tail_pad_z);
  
  for (n = 0; n < NUMBER_OF_COLORS; ++n) {
    puzzle.m_pieces[n]->m_position[0] += m_head_pad_x;
    puzzle.m_pieces[n]->m_position[1] += m_head_pad_y;
    puzzle.m_pieces[n]->m_position[2] += m_head_pad_z;
  }
  
  m_volume_width = m_head_pad_x + max_size + m_tail_pad_x;
  m_volume_height = m_head_pad_y + max_size + m_tail_pad_y;
  m_volume_depth = m_head_pad_z + max_size + m_tail_pad_z;

  for (n = 0; n < NUMBER_OF_COLORS; ++n) {
    SGAL::Transform* transform = new SGAL::Transform;
    m_transforms[n] = transform;
    transform->
      set_translation(static_cast<float>(puzzle.m_pieces[n]->m_position[0]),
                      static_cast<float>(puzzle.m_pieces[n]->m_position[1]),
                      static_cast<float>(puzzle.m_pieces[n]->m_position[2]));
    m_navigation->add_child(transform);

    SGAL::Touch_sensor* touch_sensor = new SGAL::Touch_sensor;
    transform->add_child(touch_sensor);
    touch_sensor->add_to_scene(m_scene_graph);
    
    SGAL::Shape* shape = new SGAL::Shape;
    transform->add_child(shape);
    SGAL::Appearance* app = new SGAL::Appearance;
    shape->set_appearance(app);
    SGAL::Material* mat = new SGAL::Material;
    app->set_material(mat);
    mat->set_diffuse_color(colors[n]);
    SGAL::Piece* piece = new SGAL::Piece;
    m_pieces[n] = piece;
    Uint size = WIDTH * HEIGHT * DEPTH;
    SGAL::Array<Uint>& composition = piece->get_composition();
    composition.resize(size);

    Uint i, j, k;
    Uint width, height, depth;
    if (n < 2) {
      width = WIDTH;
      height = HEIGHT;
      depth = DEPTH;
    }
    else if (n < 4) {
      width = HEIGHT;
      height = DEPTH;
      depth = WIDTH;
    }
    else {
      width = DEPTH;
      height = WIDTH;
      depth = HEIGHT;
    }

    piece->set_width(width);
    piece->set_height(height);
    piece->set_depth(depth);
    
    for (k = 0; k < DEPTH; ++k) {
      for (j = 0; j < HEIGHT; ++j) {
        for (i = 0; i < WIDTH; ++i) {
          if (n < 2) {
            Uint l = i + WIDTH * (j + HEIGHT * k);
            composition[l] = puzzle.m_pieces[n]->m_compisition[k][j][i];
          } else if (n < 4) {
            Uint l = j + HEIGHT * (k + DEPTH * i);
            composition[l] = puzzle.m_pieces[n]->m_compisition[k][j][i];
          } else {
            Uint l = k + DEPTH * (i + WIDTH * j);
            composition[l] = puzzle.m_pieces[n]->m_compisition[k][j][i];
          }
        }
      }
    }
    shape->set_geometry(piece);
  }

  if (m_option_parser.solve()) {
    SGAL::Shape* shape = new SGAL::Shape;
#if 0
    // Add the volume to the scene graph:
    SGAL::Transform* transform = new SGAL::Transform;
    m_volume_trans = transform;
    transform->set_translation(10,0,0);
    m_navigation->add_child(transform);
    transform->add_child(shape);
#endif
    SGAL::Appearance* app = new SGAL::Appearance;
    shape->set_appearance(app);
    SGAL::Material* mat = new SGAL::Material;
    app->set_material(mat);
    mat->set_diffuse_color(0.5, 0.5, 0.5);
    m_volume = new SGAL::Piece;
    Uint_array& composition = m_volume->get_composition();
    Uint size = m_volume_width * m_volume_height * m_volume_depth;
    composition.resize(size);
    Uint* volume_vec = composition.get_vector();
    memset(volume_vec, 0, size * sizeof(Uint));
    m_volume->set_width(m_volume_width);
    m_volume->set_height(m_volume_height);
    m_volume->set_depth(m_volume_depth);
    shape->set_geometry(m_volume);

    // Initialize the starting state:
    State state;
    Uint color;
    for (color = 0; color < NUMBER_OF_COLORS; ++color) {
      state[color].m_active = 1;                     // mark active
      state[color].m_position[0] = puzzle.m_pieces[color]->m_position[0];
      state[color].m_position[1] = puzzle.m_pieces[color]->m_position[1];
      state[color].m_position[2] = puzzle.m_pieces[color]->m_position[2];
    }

    clock_t start_ticks = clock();
    Boolean success = solve(state, 0);
    clock_t end_ticks = clock();
    clock_t interval_ticks = end_ticks - start_ticks;
    time_t interval_secs = interval_ticks / CLOCKS_PER_SEC;
    std::cout << ((success) ? "Success!" : "Failure!")
              << ", " << interval_secs << " seconds"
              << ", Max. level: " << m_max_level
              << ", No. invocations: " << m_num_invocations
              << std::endl;

    // Clear:
    List_state_blocks::iterator bi;
    for (bi = m_state_blocks.begin(); bi != m_state_blocks.end(); ++bi)
      delete [] (*bi);
    m_visited.clear();
      
    if (success) {
      if (m_option_parser.get_verbosity_level() > 2) {
        Solution_iter si;
        for (si = m_solution.begin(); si != m_solution.end(); ++si) {
          Uint color1 = (*si).first % (NUMBER_OF_COLORS+1);
          Uint tmp = (*si).first / (NUMBER_OF_COLORS+1);
          Uint color2 = tmp % (NUMBER_OF_COLORS+1);
          Uint color3 = tmp / (NUMBER_OF_COLORS+1);
          std::cout << color1;
          if (color2 > 0) std::cout << "," << color2-1;
          if (color3 > 0) std::cout << "," << color3;
          std::cout << ":" << (*si).second << std::endl;
        }
      }

      // Construct a time sensor and add to the scene graph:
      init_animation();
    }
    m_solution_iterator = m_solution.begin();
  }
}

/*! \brie initializes the nodes required for animations. */
void Knot_scene::init_animation()
{
  Uint i;
  for (i = 0; i < 3; ++i) {
    // Construct a rime sensor and add to the scene graph:
    m_time_sensors[i] = new SGAL::Time_sensor;
    m_root->add_child(m_time_sensors[i]);
    m_time_sensors[i]->
      set_cycle_interval(m_option_parser.get_cycle_interval());
    m_scene_graph->add_time_sensor(m_time_sensors[i]);

    // Construct a position interpolator and add to the scene graph:
    m_pos_interpolators[i] = new SGAL::Position_interpolator;
    m_root->add_child(m_pos_interpolators[i]);
    Float_array& keys = m_pos_interpolators[i]->get_keys();
    Vector3f_array& values = m_pos_interpolators[i]->get_values();
    keys.resize(2);
    values.resize(2);
    keys[0] = 0;
    keys[1] = 1;

    // ROUTE TIME.fraction_changed TO POS_INTERPOLATOR.set_fraction
    m_time_routers[i] = new SGAL::Route;
    if (!m_scene_graph->route(m_time_sensors[i], "fraction_changed",
                              m_pos_interpolators[i], "set_fraction",
                              m_time_routers[i]))
      std::cerr << "Route 1 failed!" << std::endl;

    // Construct the position-interpolator router:
    m_pos_interpolator_routers[i] = new SGAL::Route;
  }
}

/*! \brie updates the volume. */
void Knot_scene::update(const State state)
{
  m_volume->clear();
  Uint_array& volume_composition = m_volume->get_composition();
  Uint* volume_vec = volume_composition.get_vector();
  memset(volume_vec, 0, volume_composition.size() * sizeof(Uint));
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    SGAL::Piece* piece = m_pieces[color];
    Uint_array& composition = piece->get_composition();
    Uint width = piece->get_width();
    Uint height = piece->get_height();
    Uint depth = piece->get_depth();
    Uint x = state[color].m_position[0];
    Uint y = state[color].m_position[1];
    Uint z = state[color].m_position[2];

    Uint i, j, k;
    for (k = 0; k < depth; ++k) {
      for (j = 0; j < height; ++j) {
        for (i = 0; i < width; ++i) {
          Uint l1 = i + width * (j + height * k);
          if (!composition[l1]) continue;
          Uint l2 = (x+i) + m_volume_width * ((y+j) + m_volume_height * (z+k));
          volume_composition[l2] = color + 1;
        }
      }
    }
  }
}

/*! \brie reduces the number of pieces if possible. */
Knot_scene::Boolean Knot_scene::reduce(State state)
{
  update(state);
  
  // Check whether any piece is apart from the rest
  Boolean ret_value = false;
  Uint_array& volume_composition = m_volume->get_composition();
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    bool free = true;
    SGAL::Piece* piece = m_pieces[color];
    Uint_array& composition = piece->get_composition();
    Uint width = piece->get_width();
    Uint height = piece->get_height();
    Uint depth = piece->get_depth();
    SGAL::Short x = state[color].m_position[0];
    SGAL::Short y = state[color].m_position[1];
    SGAL::Short z = state[color].m_position[2];
    Uint i, j, k;
    for (k = 0; k < depth; ++k) {
      if (!free) break;
      for (j = 0; j < height; ++j) {
        if (!free) break;
        for (i = 0; i < width; ++i) {
          Uint l1 = i + width * (j + height * k);
          if (composition[l1]) continue;
          Uint l2 = (x+i) + m_volume_width * ((y+j) + m_volume_height * (z+k));
          if (volume_composition[l2]) {
            free = false;
            break;
          }
        }
      }
    }
    if (free) {
      state[color].m_active = 0;
      ret_value = true;
    }
  }
  return ret_value;
}

/*! \brief determines whether the expected move cause a conflict. */
SGAL::Boolean Knot_scene::conflict(State state, Uint color, Uint dir)
{
  Uint_array& volume_composition = m_volume->get_composition();
  SGAL::Piece* piece = m_pieces[color];
  Uint_array& composition = piece->get_composition();
  Uint width = piece->get_width();
  Uint height = piece->get_height();
  Uint depth = piece->get_depth();
  SGAL::Byte x = s_directions[dir][0] + state[color].m_position[0];
  if ((x < 0) || (m_volume_width <= (x + width))) return true;
  SGAL::Byte y = s_directions[dir][1] + state[color].m_position[1];
  if ((y < 0) || (m_volume_height <= (y + height))) return true;
  SGAL::Byte z = s_directions[dir][2] + state[color].m_position[2];
  if ((z < 0) || (m_volume_depth <= (z + depth))) return true;

  Uint i, j, k;
  for (k = 0; k < depth; ++k) {
    for (j = 0; j < height; ++j) {
      for (i = 0; i < width; ++i) {
        Uint l1 = i + width * (j + height * k);
        if (!composition[l1]) continue;
        Uint l2 = (x+i) + m_volume_width * ((y+j) + m_volume_height * (z+k));
        if (volume_composition[l2]) return true;
      }
    }
  }
  return false;
}

/*! \brie advances 1 piece 1 step if possible. */
SGAL::Boolean Knot_scene::advance1(State state, Uint color, Uint dir,
                                   State& next_state)
{
  SGAL_assertion(state[color].m_active);

  // Update the state without the moving pieces:
  state[color].m_active = 0;
  update(state);
  state[color].m_active = 1;

  // If there is a conflict, return false:
  if (conflict(state, color, dir)) return false;
  
  // Construct the next state:
  memcpy(next_state, state, sizeof(State));
  next_state[color].m_position[0] += s_directions[dir][0];
  next_state[color].m_position[1] += s_directions[dir][1];
  next_state[color].m_position[2] += s_directions[dir][2];

  // If the state has been visited already, return false:
  if (m_visited.count(next_state)) return false;

  return true;
}

/*! \brie advance 2 pieces 1 step if possible. */
SGAL::Boolean Knot_scene::advance2(State state, Uint color1, Uint color2, 
                                   Uint dir, State& next_state)
{
  SGAL_assertion(state[color1].m_active);
  SGAL_assertion(state[color2].m_active);

  // Update the state without the moving pieces:
  state[color1].m_active = 0;
  state[color2].m_active = 0;
  update(state);
  state[color1].m_active = 1;
  state[color2].m_active = 1;

  // If there is a conflict, return false:
  if (conflict(state, color1, dir)) return false;
  if (conflict(state, color2, dir)) return false;
  
  // Construct the next state:
  memcpy(next_state, state, sizeof(State));
  next_state[color1].m_position[0] += s_directions[dir][0];
  next_state[color1].m_position[1] += s_directions[dir][1];
  next_state[color1].m_position[2] += s_directions[dir][2];
  next_state[color2].m_position[0] += s_directions[dir][0];
  next_state[color2].m_position[1] += s_directions[dir][1];
  next_state[color2].m_position[2] += s_directions[dir][2];

  // If the state has been visited already, return false:
  if (m_visited.count(next_state)) return false;

  return true;
}

/*! \brie advances 2 pieces 1 step if possible. */
SGAL::Boolean Knot_scene::advance3(State state, Uint color1, Uint color2,
                                   Uint color3, Uint dir, State& next_state)
{
  SGAL_assertion(state[color1].m_active);
  SGAL_assertion(state[color2].m_active);

  // Update the state without the moving pieces:
  state[color1].m_active = 0;
  state[color2].m_active = 0;
  state[color3].m_active = 0;
  update(state);
  state[color1].m_active = 1;
  state[color2].m_active = 1;
  state[color3].m_active = 1;

  // If there is a conflict, return false:
  if (conflict(state, color1, dir)) return false;
  if (conflict(state, color2, dir)) return false;
  if (conflict(state, color3, dir)) return false;
  
  // Construct the next state:
  memcpy(next_state, state, sizeof(State));
  next_state[color1].m_position[0] += s_directions[dir][0];
  next_state[color1].m_position[1] += s_directions[dir][1];
  next_state[color1].m_position[2] += s_directions[dir][2];
  next_state[color2].m_position[0] += s_directions[dir][0];
  next_state[color2].m_position[1] += s_directions[dir][1];
  next_state[color2].m_position[2] += s_directions[dir][2];
  next_state[color3].m_position[0] += s_directions[dir][0];
  next_state[color3].m_position[1] += s_directions[dir][1];
  next_state[color3].m_position[2] += s_directions[dir][2];

  // If the state has been visited already, return false:
  if (m_visited.count(next_state)) return false;

  return true;
}

/*! \brie checks whether the state is empty. */
SGAL::Boolean Knot_scene::empty(State state) const
{
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color)
    if (state[color].m_active) return false;
  return true;
}

/*! \brie solves the puzzle. */
SGAL::Boolean Knot_scene::solve(State state, Uint level)
{
  // std::cout << m_num_invocations << ": " << state << std::endl;
  if (m_option_parser.get_verbosity_level() > 1)
    std::cout << indent(level) << state << std::endl;
  ++m_num_invocations;
  if (level > m_max_level) m_max_level = level;
  
  mark(state);                      // mark state as visited
  if (reduce(state)) mark(state);   // remove loose pieces    
  if (empty(state)) return true;    // if empty, we are done

  // Move one at a time:
  Uint dir;
  for (dir = 0; dir < NUMBER_OF_DIRECTIONS; ++dir) {
    Uint color1;
    for (color1 = 0; color1 < NUMBER_OF_COLORS; ++color1) {
      if (!state[color1].m_active) continue;
      State next_state;
      if (advance1(state, color1, dir, next_state))
        if (solve(next_state, level+1)) {
          m_solution.push_front(Step(encode(color1), dir));
          return true;
        }
        
      Uint color2;
      for (color2 = color1+1; color2 < NUMBER_OF_COLORS; ++color2) {
        if (!state[color2].m_active) continue;
        State next_state;
        if (advance2(state, color1, color2, dir, next_state))
          if (solve(next_state, level+1)) {
            m_solution.push_front(Step(encode(color1, color2), dir));
            return true;
          }

        Uint color3;
        for (color3 = color2+1; color3 < NUMBER_OF_COLORS; ++color3) {
          if (!state[color3].m_active) continue;
          State next_state;
          if (advance3(state, color1, color2, color3, dir, next_state))
            if (solve(next_state, level+1)) {
              m_solution.push_front(Step(encode(color1, color2, color3), dir));
              return true;
            }
        }
      }
    }
  }
  return false;
}

/*! \brie initializes the secene. */
void Knot_scene::init_scene()
{
  m_window_item = m_window_manager->create_window_item();
  m_window_item->set_title("Knot");
  m_window_manager->create_window(m_window_item);
  
  m_scene_graph->create_defaults();
  indulge_user();

  m_context = new SGAL::Context();
  m_scene_graph->set_context(m_context);
  m_scene_graph->init_context();
  m_scene_graph->start_simulation();
  m_scene_graph->bind();

  if (m_option_parser.export_vrml()) {
    SGAL::Writer writer(m_scene_graph);
    SGAL::Vrml_formatter formatter(std::cout);
    writer(formatter);
  }

  if (m_option_parser.solve()) SGAL::Tick_event::doregister(this);

  m_window_item->show();
}

/*! \brief indulges user requests from the command line. */
void Knot_scene::indulge_user()
{ m_option_parser.configure(m_scene_graph); }

/*! \brief clears the scene. */
void Knot_scene::clear_scene()
{
  if (m_option_parser.solve()) SGAL::Tick_event::unregister(this);
  m_scene_graph->release_context();
  m_scene_graph->destroy_defaults();
}

/*! \brief identifies the agent. */
void Knot_scene::identify(void)
{ std::cout << "Agent: Knot_scene" << std::endl; }

/*! \brief draws the scene. */
void Knot_scene::draw_window(SGAL::Window_item* window_item,
                             SGAL::Boolean dont_accumulate)
{
  SGAL_assertion(m_scene_graph);
  SGAL::Configuration* conf = m_scene_graph->get_configuration();
  SGAL::Draw_action draw_action(conf);
  draw_action.set_context(m_context);
  draw_action.set_snap(false);
  m_scene_graph->draw(&draw_action);
  
  // m_context->swap_buffers();
  window_item->swap_buffers();
}

/*! \brief reshapes the viewport. */
void Knot_scene::reshape_window(SGAL::Window_item* window_item,
                                SGAL::Uint width, SGAL::Uint height)
{
  SGAL::Context* context = m_scene_graph->get_context();
  context->set_viewport(0, 0, width, height);
  SGAL::Camera* camera = m_scene_graph->get_active_camera();
  SGAL_assertion(camera);
  camera->init(context);
}

/*! \brief marks a given state as visited (insert into the hash data structure)
 */
void Knot_scene::mark(State state)
{
  if (!m_next_free_state_block) {
    m_next_free_state_index = 0;
    m_next_free_state_block = new State[BLOCK_SIZE];
    m_state_blocks.push_back(m_next_free_state_block);
  }
  Piece_state* new_state = m_next_free_state_block[m_next_free_state_index++];
  if (m_next_free_state_index == BLOCK_SIZE) m_next_free_state_block = NULL;
  memcpy(new_state, state, sizeof(State));
  m_visited.insert(&new_state[0]);
}

/*! \brief finds the minimum coordinate values. */
void Knot_scene::find_min(const State state,
                          Uint& min_x, Uint& min_y, Uint& min_z)
{
  min_x = min_y = min_z = UINT_MAX;
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    Uint x = state[color].m_position[0];
    Uint y = state[color].m_position[1];
    Uint z = state[color].m_position[2];
    if (x < min_x) min_x = x;
    if (y < min_y) min_y = y;
    if (z < min_z) min_z = z;
  }
}

/*! \brief compares lexicographically two states. */
Knot_scene::Int Knot_scene::compare(const State state1, const State state2)
{
  Uint min_x1, min_y1, min_z1;
  find_min(state1, min_x1, min_y1, min_z1);     // find the min values
  Uint min_x2, min_y2, min_z2;
  find_min(state2, min_x2, min_y2, min_z2);     // find the min values

  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    if (!state1[color].m_active && state2[color].m_active) return -1;
    else if (state1[color].m_active && !state2[color].m_active) return 1;

    Uint x1 = state1[color].m_position[0] - min_x1;
    Uint x2 = state2[color].m_position[0] - min_x2;
    if (x1 < x2) return -1;
    else if (x1 > x2) return 1;

    Uint y1 = state1[color].m_position[1] - min_y1;
    Uint y2 = state2[color].m_position[1] - min_y2;
    if (y1 < y2) return -1;
    else if (y1 > y2) return 1;

    Uint z1 = state1[color].m_position[2] - min_z1;
    Uint z2 = state2[color].m_position[2] - min_z2;
    if (z1 < z2) return -1;
    else if (z1 > z2) return 1;
  }
  return 0;
}

/*! \brief marks a given state and its equivalent states as visited. */
void Knot_scene::mark_all(State state)
{
  Uint min_x, min_y, min_z;
  find_min(state, min_x, min_y, min_z);         // find the min values
  
  // Set the active flag:
  State eqv_state;
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color)
    eqv_state[color].m_active = state[color].m_active;
  
  // Iterate over equivalent states:
  Uint i, j, k;
  Boolean overflow_z = false;
  for (k = 0; k < m_volume_depth; ++k) {
    for (color = 0; color < NUMBER_OF_COLORS; ++color) {
      if (!state[color].m_active) continue;
      SGAL::Piece* piece = m_pieces[color];
      Uint depth = piece->get_depth();
      Uint z = state[color].m_position[2] - min_z + k;
      if (z + depth >= m_volume_depth) {
        overflow_z = true;
        break;
      }
      eqv_state[color].m_position[2] = z;
    }
    if (overflow_z) break;
    
    Boolean overflow_y = false;
    for (j = 0; j < m_volume_height; ++j) {
      for (color = 0; color < NUMBER_OF_COLORS; ++color) {
        if (!state[color].m_active) continue;
        SGAL::Piece* piece = m_pieces[color];
        Uint height = piece->get_height();
        Uint y = state[color].m_position[1] - min_y + j;
        if (y + height >= m_volume_height) {
          overflow_y = true;
          break;
        }
        eqv_state[color].m_position[1] = y;
      }
      if (overflow_y) break;

      Boolean overflow_x = false;
      for (i = 0; i < m_volume_width; ++i) {
        for (color = 0; color < NUMBER_OF_COLORS; ++color) {
          if (!state[color].m_active) continue;
          SGAL::Piece* piece = m_pieces[color];
          Uint width = piece->get_width();
          Uint x = state[color].m_position[0] - min_x + i;
          if (x + width >= m_volume_width) {
            overflow_x = true;
            break;
          }
          eqv_state[color].m_position[0] = x;
        }
        if (overflow_x) break;

        mark(eqv_state);
      }
    }
  }
}

/*! \brief handles tick events. */
void Knot_scene::handle(SGAL::Tick_event* event)
{
  if (m_solution_iterator == m_solution.end()) {
    // \todo disable the time sensor, and unregister the tick event
    return;
  }

  SGAL::Scene_time cur_time = event->get_sim_time() / CLOCKS_PER_SEC;
  if (cur_time > m_time_sensors[0]->get_stop_time()) {
    Step step = *m_solution_iterator++;
    Uint color1 = step.first % (NUMBER_OF_COLORS+1);
    Uint tmp = step.first / (NUMBER_OF_COLORS+1);
    Uint color2 = tmp % (NUMBER_OF_COLORS+1);
    Uint color3 = tmp / (NUMBER_OF_COLORS+1);
    Uint dir = step.second;
    if (color1 > 0) animate(cur_time, color1-1, dir, 0);
    if (color2 > 0) animate(cur_time, color2-1, dir, 1);
    if (color3 > 0) animate(cur_time, color3-1, dir, 2);
  }
}

/*! \brief animates. */
void Knot_scene::animate(Scene_time cur_time, Uint color, Uint dir, Uint route)
{
  // Setup the interpolator:
  Vector3f_array& values = m_pos_interpolators[route]->get_values();
  Uint x = puzzle.m_pieces[color]->m_position[0];
  Uint y = puzzle.m_pieces[color]->m_position[1];
  Uint z = puzzle.m_pieces[color]->m_position[2];
  values[0].set(x, y, z);
  values[1].set(x+s_directions[dir][0], y+s_directions[dir][1],
                z+s_directions[dir][2]);
  puzzle.m_pieces[color]->m_position[0] += s_directions[dir][0];
  puzzle.m_pieces[color]->m_position[1] += s_directions[dir][1];
  puzzle.m_pieces[color]->m_position[2] += s_directions[dir][2];
    
  // Route the interpolator to the transform:
  if (!m_scene_graph->route(m_pos_interpolators[route], "value_changed",
                            m_transforms[color], "set_translation",
                            m_pos_interpolator_routers[route]))
    std::cerr << "Route 2 failed!" << std::endl;

  // Start the animation:
  m_time_sensors[route]->set_start_time(cur_time);
  m_time_sensors[route]->
    set_stop_time(cur_time + m_option_parser.get_cycle_interval());
}
