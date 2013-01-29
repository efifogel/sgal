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
// $Id: Knot_scene.hpp 7205 2009-01-24 22:20:34Z efif $
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef KNOT_SCENE_HPP
#define KNOT_SCENE_HPP

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <string.h>

#include <boost/unordered_set.hpp>

#include "SGAL/Types.hpp"
#include "SGAL/Scene.hpp"
#include "SGAL/Piece.hpp"
#include "SGAL/Vector3f.hpp"

#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Time_sensor.hpp"
#include "SGAL/Position_interpolator.hpp"
#include "SGAL/Route.hpp"

class Knot_option_parser;

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Context;
class Transform;
class Group;
class Position_interpolator;
class Time_sensor;
class Route;
class Tick_event;

#if (defined USE_GLUT)
class Glut_window_manager;
class Glut_window_item;
#elif defined(_WIN32)
class Windows_window_manager;
class Windows_window_item;
#else
class X11_window_manager;
class X11_window_item;
#endif

SGAL_END_NAMESPACE

class Knot_scene : public SGAL::Scene {
public:
#if (defined USE_GLUT)
  typedef SGAL::Glut_window_manager             Window_manager;
  typedef SGAL::Glut_window_item                Window_item;
#elif defined(_WIN32)
  typedef SGAL::Windows_window_manager          Window_manager;
  typedef SGAL::Windows_window_item             Window_item;
#else
  typedef SGAL::X11_window_manager              Window_manager;
  typedef SGAL::X11_window_item                 Window_item;
#endif

  typedef SGAL::Uint                    Uint;
  typedef SGAL::Int                     Int;
  typedef SGAL::Boolean                 Boolean;
  typedef SGAL::Scene_time              Scene_time;
  
  /*! The piece colors (ids) */
  enum Color {
    INVALID = -1, ORANGE, BLUE, RED, YELLOW, PURPLE, GREEN, NUMBER_OF_COLORS
  };

#define NUMBER_OF_DIRECTIONS            6

  struct indent {
    Uint m_depth;
    indent(Uint depth): m_depth(depth) {};
  };

  typedef SGAL::Byte                   Piece_position[3];
  struct Piece_state {
    SGAL::Ubyte m_active;
    Piece_position m_position;
  };
  typedef Piece_state                   State[NUMBER_OF_COLORS];

  enum Error_id { FILE_NOT_FOUND, FILE_CANNOT_OPEN, ILLEGAL_EXTENSION };
  class Illegal_input : public std::exception {
  public:
    Illegal_input(Error_id err, const std::string& msg,
                  const std::string& filename) throw() :
      m_error(err),
      m_msg(msg),
      m_filename(filename)
    {}

    // This declaration is not useless:
    // http://gcc.gnu.org/onlinedocs/gcc-3.0.2/gcc_6.html#SEC118
    virtual ~Illegal_input() throw()
    {
      m_msg.clear();
      m_filename.clear();
    }

    virtual const char* what () const throw ()
    {
      std::string tmp(m_msg + " (" + m_filename + ")!");
      return tmp.c_str();
    }
    Error_id m_error;
    std::string m_msg;
    std::string m_filename;
  };

  /*! Constructor */
  Knot_scene(Knot_option_parser& option_parser);

  /*! Destructor */
  virtual ~Knot_scene(void);

  /*! \brief creates the scene */
  virtual void create_scene();

  /*! \brief destroys the scene */
  virtual void destroy_scene();
  
  /*! \brief initializes the secene */
  virtual void init_scene();

  /*! \brief clears the scene */  
  virtual void clear_scene();

  /*! Reshape the viewport of a window of the scene
   * It is assumed that the window context is the current context
   * \param window_item the window to reshape
   * \param width the new width of the window
   * \param height the new height of the window
   */
  virtual void reshape_window(SGAL::Window_item* window_item,
                              SGAL::Uint width, SGAL::Uint height);

  /*! Draw the scene into a window.
   * It is assumed that the window context is the current context.
   * \param window_item the window to draw.
   * \param dont_accumulate indicates that no accumulation should be performed.
   */
  virtual void draw_window(SGAL::Window_item* window_item,
                           SGAL::Boolean dont_accumulate);

  /*! Print out the name of this agent (for debugging purposes). */
  virtual void identify(void);

  /*! Handle tick events. */
  virtual void handle(SGAL::Tick_event* tick_event);

  /*! Set the window manager */
  template <typename Window_manager>
  void set_window_manager(Window_manager* manager)
  { m_window_manager = manager; }

  /*! \brief obtains the scene scene-graph */
  SGAL::Scene_graph* get_scene_graph() { return m_scene_graph; }

  /*! \brief returns true iff the scene does simulate something */
  Boolean is_simulating(void) const { return true; }

  /*! \brief clears the scene */
  void clear();

  /*! Obtain the name of a given color */
  static const char* get_color_name(Uint color) { return s_color_names[color]; }
  
private:
  typedef SGAL::Array<SGAL::Vector3f>   Vector3f_array;
  typedef SGAL::Array<SGAL::Float>      Float_array;
  typedef SGAL::Array<Uint>             Uint_array;
  
  /*! The window manager */
  Window_manager* m_window_manager;

  /*! The window item */
  Window_item* m_window_item;
  
  /*! The scene graph */
  SGAL::Scene_graph* m_scene_graph;

  /*! The root of the scene graph */
  SGAL::Group* m_root;

  /*! The scene navigation root */
  SGAL::Transform* m_navigation;
  
  /*! The context of the scene */
  SGAL::Context* m_context;
  
  /*! Option parser */
  Knot_option_parser& m_option_parser;

  /*! A vector of directions to move the pieces while attempting to solve */
  static Piece_position s_directions[NUMBER_OF_DIRECTIONS] ;

  /*! COlor names for debuggin */
  static const char* s_color_names[NUMBER_OF_COLORS];

  /*! Pointers to the piece geometries */
  SGAL::Piece* m_pieces[NUMBER_OF_COLORS];

  /*! Pointers to the piece transforms */
  SGAL::Transform* m_transforms[NUMBER_OF_COLORS];
  
  /*! The width of the volume where all the pieces reside */
  Uint m_volume_width;

  /*! The height of the volume where all the pieces reside */
  Uint m_volume_height;

  /*! The depth of the volume where all the pieces reside */
  Uint m_volume_depth;

  /*! The head padding of the pieces within the volume along the x-coord */
  Uint m_head_pad_x;

  /*! The head padding of the pieces within the volume along the y-coord */
  Uint m_head_pad_y;

  /*! The head padding of the pieces within the volume along the z-coord */
  Uint m_head_pad_z;

  /*! The tail padding of the pieces within the volume along the x-coord */
  Uint m_tail_pad_x;

  /*! The tail padding of the pieces within the volume along the y-coord */
  Uint m_tail_pad_y;

  /*! The tail padding of the pieces within the volume along the z-coord */
  Uint m_tail_pad_z;
  
  /*! The volume transform */
  SGAL::Transform* m_volume_trans;

  /*! The volume piece */
  SGAL::Piece* m_volume;

  /*! \brief hashes a given state --- map to size_t */
  static inline size_t my_hash(const State state);

  /*! The hash_multiset's key equality function object */
  class Hash_comparer {
  public:

    /*! \brief compares 2 hash keys */
    inline bool operator( )(const Piece_state* key1, const Piece_state* key2)
      const;
  };
  
  /*! Stores all visited states. Used to prevent visiting the same state
   * more than once
   */
  struct Hasher {
    inline size_t operator()(const Piece_state* state) const
    { return Knot_scene::my_hash(state); }
  };

  boost::unordered_multiset<Piece_state *, Hasher, Hash_comparer> m_visited;

  typedef std::pair<Uint,Uint>                  Step;
  typedef std::list<Step>                       Solution;
  typedef Solution::iterator                    Solution_iter;

  /*! The solution */
  Solution                                      m_solution;

  /*! Pointer to the next step in the solution */
  Solution::iterator                            m_solution_iterator;
  
  typedef std::list<State*>                     List_state_blocks;
  typedef List_state_blocks::iterator           List_states_iter;
  
  /*! A link list of free state */
  List_state_blocks m_state_blocks;

  #define BLOCK_SIZE                            65536

  /*! Next free state index */
  Uint m_next_free_state_index;
  
  /*! Next free state block */
  State* m_next_free_state_block;
  
  /*! Number of times solve() is invoked */
  Uint m_num_invocations;

  /*! The level of recursion */
  Uint m_max_level;

  /*! The time sensors to trigger the interpolations respectively. */
  SGAL::Time_sensor* m_time_sensors[3];

  /*! The position interpolators to animate the solution */
  SGAL::Position_interpolator* m_pos_interpolators[3];

  /*! The time sensor routers */
  SGAL::Route* m_time_routers[3];

  /*! The position interpolator routers */
  SGAL::Route* m_pos_interpolator_routers[3];

  /*! Encode 1 color */
  Uint encode(Uint color1) const { return color1+1; }

  /*! Encode 2 colors */
  Uint encode(Uint color1, Uint color2) const
  { return (color2+1) + (NUMBER_OF_COLORS+1)* encode(color1); }

  /*! Encode 3 colors */
  Uint encode(Uint color1, Uint color2, Uint color3) const
  { return (color3+1) + (NUMBER_OF_COLORS+1)* encode(color1, color2); }
  
  /*! \brief indulges user requests from the command line */
  void indulge_user();

  /*! \brief solves the puzzle */
  Boolean solve(State state, Uint level);

  /*! \brief updates the volume */
  void update(const State state);
  
  /*! \brief reduces the number of pieces if possible */
  Boolean reduce(State state);

  /*! \brief advances one piece (one step), if possible */
  Boolean advance1(State state, Uint color, Uint dir, State& next_state);

  /*! \brief advances two pieces (one step), if possible */
  Boolean advance2(State state, Uint color1, Uint color2, Uint dir,
                   State& next_state);

  /*! \brief advances three pieces (one step), if possible */
  Boolean advance3(State state, Uint color1, Uint color2, Uint color3,
                   Uint dir, State& next_state);
  
  /*! \brief does the expected move cause a conflict? */
  Boolean conflict(State state, Uint color, Uint dir);

  /*! \brief checks whether the state is empty */
  Boolean empty(State state) const;

  /*! \brief marks a given state and its equivalent states as visited */
  void mark_all(State state);

  /*! \brief marks a given state as visited */
  void mark(State state);

  /*! \brief finds the minimum coordinate values */
  static void find_min(const State state,
                       Uint& min_x, Uint& min_y, Uint& min_z);

  /*! \brief compares lexicographically two states */
  static Int compare(const State state1, const State state2);

  /*! \brief initializes the nodes required for animations */
  void init_animation();

  /*! \brief animates */
  void animate(Scene_time cur_time, Uint color, Uint dir, Uint route);
};

/*! Exporter */
inline std::ostream& operator<<(std::ostream& os,
                                 const Knot_scene::State state)
{
  Knot_scene::Uint color;
  for (color = 0; color < Knot_scene::NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    int x = state[color].m_position[0];
    int y = state[color].m_position[1];
    int z = state[color].m_position[2];
    os << Knot_scene::get_color_name(color) << ":"
       << x << "," << y << "," << z;
    break;
  }
  for (color++; color < Knot_scene::NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    int x = state[color].m_position[0];
    int y = state[color].m_position[1];
    int z = state[color].m_position[2];
    os << ";" << Knot_scene::get_color_name(color) << ":"
       << x << "," << y << "," << z;
  }
  
  return os;
}

/*! Indenter */
inline std::ostream& operator<<(std::ostream& os, Knot_scene::indent const& in)
{
  for (Knot_scene::Uint i = 0; i != in.m_depth; ++i) os << " ";
  return os;
}

/*! Compare 2 hash keys */
inline bool Knot_scene::Hash_comparer::operator()(const Piece_state* key1,
                                                  const Piece_state* key2)
  const
{
  return (compare(key1, key2) == 0);
}

/*! Hash a given state --- map to size_t */
inline size_t Knot_scene::my_hash(const State state)
{
  Uint min_x, min_y, min_z;
  find_min(state, min_x, min_y, min_z);         // find the min values

  size_t hash_val = 0;
  Uint color;
  for (color = 0; color < NUMBER_OF_COLORS; ++color) {
    if (!state[color].m_active) continue;
    hash_val += state[color].m_position[0] + state[color].m_position[1] +
      state[color].m_position[2];
  }
  return hash_val - ((min_x + min_y + min_z) * NUMBER_OF_COLORS);
}

#endif
