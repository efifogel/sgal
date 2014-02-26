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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_SCENE_GRAPH_HPP
#define SGAL_SCENE_GRAPH_HPP

/*! \file
 * \brief Maintain the scene graph of the model.
 *
 * The scene graph contains the following:
 *  - a pointer to the root of the scene graph which
 *    is of type Group and holds a default light
 *    (if no lights were specified in the scene).
 *  - a pointer to the navigation root of the scene
 *    graph. The navigation root is an Transform
 *    object that contains the navigation
 *    transformations.
 *  - a material pool
 *  - a texture pool
 *  - an apprearance pool
 *  - a node pool, which is a map of all nodes in
 *    the scene graph referenced by name
 *  - a touch sensor pool
 *  - a camera
 *  - a context
 *  - several flags to indicate the status of the
 *    scene graph
 */

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <list>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Navigation_info_types.hpp"
#include "SGAL/Bindable_stack.hpp"
#include "SGAL/File_format.hpp"

//! \todo #include "Event_filter.h"
//! \todo #include "Model_stats.h"
//! \todo #include "SG_JS_err_reporter.h"

namespace fi = boost::filesystem;

SGAL_BEGIN_NAMESPACE

class Navigation_info;
class Geometry;
class Camera;
class Touch_sensor;
class Time_sensor;
class Snapshot;
class Point_light;
class Viewpoint;
class Fog;
class Background;
class JSW_engine_int;
class Window_handle;
class Context;
class Light;
class Group;
class Transform;
class Stream;
class Draw_action;
class Execution_coordinator;
class Configuration;
class SAI;
// class View_sensor;
class Text_screen;
class Event_filter;
class Route;
class Isect_action;
class Simulation;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Scene_graph {
public:
  typedef boost::shared_ptr<Container>            Shared_container;
  typedef boost::shared_ptr<Group>                Shared_group;
  typedef boost::shared_ptr<Transform>            Shared_transform;
  typedef boost::shared_ptr<Point_light>          Shared_point_light;

  typedef std::list<Shared_container>             Container_list;
  typedef Container_list::iterator                Container_list_iter;

  typedef std::vector<Container*>                 Container_vector;
  typedef Container_vector::iterator              Container_vector_iter;

  typedef std::map<std::string, Shared_container> Container_map;
  typedef Container_map::iterator                 Container_map_iter;

  typedef std::list<Touch_sensor*>                Touch_sensor_list;
  typedef Touch_sensor_list::iterator             Touch_sensor_iter;

  typedef std::list<Time_sensor*>                 Time_sensor_list;
  typedef Time_sensor_list::iterator              Time_sensor_iter;

  typedef std::list<Snapshot*>                    Snapshot_list;
  typedef Snapshot_list::iterator                 Snapshot_iter;

  typedef std::list<Simulation*>                  Simulation_list;
  typedef Simulation_list::iterator               Simulation_iter;

  typedef std::list<fi::path>                     Path_list;
  typedef Path_list::iterator                     Path_iter;

  typedef std::pair<Uint, Uint>                   Selection_id_interval;
  typedef std::list<Selection_id_interval>        Selection_id_interval_list;
  typedef Selection_id_interval_list::iterator    Selection_id_interval_iter;

  /*! Constructor
   * \param syncronize indicates whether the creation and the rendering of the
   * scene graph must be syncronized. This is necessary when the scene graph
   * is created and rendered by two different threads.
   * In this case the method set_done() must be called by the creator to
   * indicate that the scene is ready for rendering
   */
  Scene_graph(bool syncronize = false);

  /*! Desctructor */
  virtual ~Scene_graph();

  /*! Attach the graphics state context. */
  Context* attach_context(Context* context);

  /*! Initialize the graphics state context. */
  void init_context();

  /*! Release the graphics state context. */
  void release_context();

  //! \todo void add_node(XML_entity* parent, XML_entity* node);

  /*! Obtain the context */
  Context* get_context() const { return m_context; }

  /*! Create default nodes and route them appropriately.
   * The set of such nodes consists of a navigation sensor node, a
   * configuration node, a camera node, and a light node. Also, connect
   * navigation sensor node to the navigation root node.
   */
  void create_defaults();

  /*! Destroy default (owned) nodes */
  void destroy_defaults();

  /*! Bind the bindable nodes */
  void bind();

  /*! Render the scene graph.
   * The scene is rendered only if the scene graph is complete and it has been
   * modified since the last render.
   * If a background node exits, we first render it. Otherwise we clear the
   * buffers. Then we render the camera and then we start rendering the scene
   * graph from the root. At the end we swap the buffers.
   * \param draw_action (in)
   */
  void draw(Draw_action* draw_action);

  void isect(Uint x, Uint y);

  //! \todo Event_filter* get_navigation_sensor() { return m_navigation_sensor; }

  /*! Process all snapshot nodes. */
  void process_snapshots(Draw_action* action);

  /*! Start simulation. */
  void start_simulation();

  void create_execution_coordinator();
  Execution_coordinator* get_execution_coordinator();

  void set_default_event_filter(Event_filter* ef)
  { m_default_event_filter = ef; }

  /*! Add a container to the name-less container-pool.
   * The name-lss container pool is maintained so that the name-less containers
   * can be deleted during distruction.
   */
  void add_container(Shared_container container);

  /*! Add a container to the instance container-pool.
   * In this pool every container has a name by which it can be retrieved.
   */
  void add_container(Shared_container container, const std::string & name);

  /*! Obtain a container by its instance name. If the node does not
   * exists, returns 0.
   * \param name (in) the name of the container.
   * \return a pointer to the container.
   */
  Shared_container get_container(const std::string & name);

  /*! Add a touch sensor node to the scene graph.
   * Pushes the touch sensor to the back of the list. It returns a
   * \param touch_sensor a pointer to the touch sensor object
   * unique selection id.
   * \return a unique selection id.
   */
  void add_touch_sensor(Touch_sensor* touch_Sensor);

  /*! Allocate an interval of selection ids, given the size of the interval
   * to allocate.
   */
  Uint allocate_selection_ids(Uint num);

  /*! Free an interval of selection ids, given the starting id and size of
   * the interval to free.
   */
  void free_selection_ids(Uint start, Uint num);

  /*! Add a time sensor node to the scene graph. */
  void add_time_sensor(Time_sensor* time_Sensor);

  /*! Return true iff time sensors are present. */
  Boolean has_time_sensors() const { return m_time_sensors.size() > 0; }

  /*! Add a snapshot node to the list of snapshots nodes. */
  void add_snaphot(Snapshot* snapshot);

  /*! Add a Simulation node to the list of Simulation nodes. */
  void add_simulation(Simulation* simulation);

  /*! Bindable node manipulations. */

  /*! Obtain the navigation info binable stack. */
  Bindable_stack* get_navigation_info_stack();

  /*! Obtain the background binable stack. */
  Bindable_stack* get_background_stack();

  /*! Obtain the camera stack. */
  Bindable_stack* get_camera_stack();

  /*! Route the Navigation_info node properly. */
  void route_navigation_info(Navigation_info* nav, Navigation_info_type type);

  /*! Set the context. */
  void set_context(Context* context);

  /*! \breif sets the Configuration node. */
  void set_configuration(Configuration* sconfig);

  /*! \breif obtains the Configuration node. */
  Configuration* get_configuration() const;

  /*! Obtain the active camera. */
  Camera* get_active_camera();

  /*! Obtain the active background. */
  Background* get_active_background();

  /*! Obtain the root of the scene graph.
   * \return the root of the scene graph.
   */
  Shared_group get_root() const;

  /*! Set the root of the scene graph.
   * The root of the Scene_graph is of type Group and contains the default light
   * source that is created in case there are no lights in the scene.
   * \param root (in) a pointer to the root node.
   */
  void set_root(Shared_group root);

  /*! Return the begin pointer of the containers */
  Container_list_iter containers_begin() { return m_containers.begin(); }

  /*! Return the past-the-end pointer of the containers */
  Container_list_iter containers_end() { return m_containers.end(); }

  /*! Return the begin pointer of the containers */
  Container_map_iter instances_begin() { return m_instances.begin(); }

  /*! Return the past-the-end pointer of the containers */
  Container_map_iter instances_end() { return m_instances.end(); }

  /*! Obtain the navigation root. It's a node of type Transform */
  Shared_transform get_navigation_root() { return m_navigation_root; }

  /*! Set the navigation root. The navigation root node is a child
   * of the root of the scene graph. It is a node of type Transform.
   * \param nav_root (in) a pointer to the navigation root node
   */
  void set_navigation_root(Shared_transform nr);

  /*! Perform pre cascade activity. */
  void signal_cascade_start();

  /*! Perform post cascade activity. */
  void signal_cascade_end();

  /*! Returns true if the scene creation is complete */
  bool is_scene_done() { return m_is_scene_done; }

  /*! Raises the flag that indicates whether the scene creation is complete */
  void set_done() { m_is_scene_done = true; }

  void set_have_lights(Boolean flag);

  Boolean does_have_lights();

  Int get_unique_light_id();

  Float get_fps() { return m_fps; }

  /*! \breif sets the active key-sensor */
  void set_active_key_sensor(Key_sensor* se) { m_active_key_sensor = se; }

  /*! \breif obtains the active key-sensor */
  Key_sensor* get_active_key_sensor() const { return m_active_key_sensor; }

  /*! todo sai
  JSW_engine_int* get_jsw_engine();
  SAI* get_scripts_sai();
  */

  // Text screen operations.
  // Clear all text lines.
  void clear_text_screen();

  void put_text_string(int line,const std::string & str);

  void add_text_string(const std::string & str);

  /*! Obtain the head light. */
  Shared_point_light get_head_light() const { return m_head_light; };

  /*! \bried computes the speed factor required by the sensors */
  float compute_speed_factor() const;

  /*! Route the connection */
  bool route(const std::string& src_node_str,
             const std::string& src_field_str,
             const std::string& dts_node_str,
             const std::string& dst_field_str,
             Route* route);

  /*! Route the connection */
  bool route(Container* src_node, const char* src_field_str,
             Container* dts_node, const char* dst_field_str,
             Route* route);

  /*! Add a directory to the directory-search list */
  void add_data_dir(const fi::path & dir) { m_data_dirs.push_back(dir); }

  /*! Set the directory-search structure */
  void set_data_dirs(const Path_list & dirs) { m_data_dirs = dirs; }

  /*! Obtain the directory-search structure */
  const Path_list & get_data_dirs() const { return m_data_dirs; }

  /*! Find containers of a particular type and inserts them into a given
   * container
   */
  template <typename T, typename OutputIterator>
  OutputIterator find(OutputIterator oi)
  {
    Container_list_iter ci;
    for (ci = containers_begin(); ci != containers_end(); ++ci) {
      T* tmp = dynamic_cast<T*>(*ci);
      if (tmp) *oi++ = tmp;
    }
    Container_map_iter ii;
    for (ii = instances_begin(); ii != instances_end(); ++ii) {
      T* tmp = dynamic_cast<T*>((*ii).second);
      if (tmp) *oi++ = tmp;
    }
    return oi;
  }

  /*! Obtain the input file format.
   * \return The input file format.
   */
  File_format::Id get_input_format_id() const;

  /*! Set the input file format.
   * \param format The input file format.
   */
  void set_input_format_id(File_format::Id format_id);

  /// \name Writters
  //@{
  /*! Write the scene to a file in VRML format.
   * \param filename The file name.
   */
  void write_vrml(const std::string& filename);

  /*! Write the scene to a file in STL format.
   * \param filename The file name.
   */
  void write_stl(const std::string& filename);
  //@}

private:
  /*! Binadable stacks */

  /*! The viewpoint stack */
  Bindable_stack m_viewpoint_stack;

  /*! The navigation info binable stack */
  Bindable_stack m_navigation_info_stack;

  /*! The fog stack */
  Bindable_stack m_fog_stack;

  /*! The camera stack */
  Bindable_stack m_camera_stack;

  /*! The background stack */
  Bindable_stack m_background_stack;

  /*! The root of the scene graph */
  Shared_group m_root;

  /*! the default light. */
  Shared_point_light m_head_light;

  /*! The default camera */
  Camera* m_camera;

  /*! The default navigation info */
  Navigation_info* m_navigation_info;

  /*! A list of objects that are children of the root of the scene */
  Container_vector m_root_objects;

  /*! The navigation transfor (a child of the root) */
  Shared_transform m_navigation_root;

  /*! The list of all containers. Used mainly for deletion. */
  Container_list m_containers;

  /*! All containers that may be referenced (have the DEF attribute). */
  Container_map m_instances;

  /*! the current context. */
  Context* m_context;

  /*! indicates whether the scene is ready for rendering. */
  Boolean m_is_scene_done;

  /*! indicates whether there were lights specified in the scene. */
  Boolean m_does_have_lights;

  /*! An index for the last used light. Used to generate unique light ids. */
  Int m_current_light_id;

  /*! an action used to intersect the scene graph with a given (x,y) point
   * on the screen.
   */
  Isect_action* m_isect_action;

  /*! A vector of touch-sensor pointers. */
  Touch_sensor_list m_touch_sensors;

  /*! The list of intervals of free selection ids. */
  Selection_id_interval_list m_free_selection_ids;

  /*! A list of time sensors */
  Time_sensor_list m_time_sensors;

  /*! an array of snapshot objects to be processed at the end of the rendering
   * sequence.
   */
  Snapshot_list m_snapshots;

  /*! A list of Simulation containetrs. */
  Simulation_list m_simulations;

  /*! the execution coordinator responsible for holding status information
   * about rendering.
   */
  Execution_coordinator* m_execution_coordinator;

  /*! an event filter that catches all events that are specific
   * to the context, such as resize.
   */
  Event_filter* m_default_event_filter;

  /*! a pointer to the configuration object. */
  Configuration* m_configuration;

  /*! a pointer to the view sensor object. */
  // View_sensor* m_view_sensor;

  /*! The active key sensor. */
  Key_sensor* m_active_key_sensor;

  /*! the rendering frames per second. */
  float m_fps;
  short m_fps_counter;
  time_t m_fps_start_time;

  /*! Last rendering time. */
  Uint m_last_render_time;

  static Uint s_min_redraw_period;

  /*! a pointer to the statistics object. */
  //! \todo Model_stats m_statistics;

  /*! a pointer to the JScript Interpreter engine. */
  JSW_engine_int* m_jsw_engine;

  /*! a pointer to SAI that serves the script nodes. */
  SAI* m_scripts_sai;

  /*! an object that is used to display the FPS on the screen. */
  Text_screen* m_text_screen;

  /*! */
  //! \todo SG_JS_err_reporter m_js_error_reporter;

  /*! a critical section used in rendering and context creation. */
  //! \todo static Critical_section s_render_cs;

  /*! used to avoid multiple Isect executions in a single frame. */
  Boolean m_is_isect_required;

  /*! Indicates whether the camera frustum is set to contain the bounding
   * sphere
   */
  Boolean m_is_camera_in_focus;

  /*! A collection of directories to search files in. */
  Path_list m_data_dirs;

  /*! Indicates whether the configuration node is owned. */
  Boolean m_owned_configuration;

  /*! Indicates whether the navigation info node is owned. */
  Boolean m_owned_navigation_info;

  /*! Indicates whether the camera node is owned. */
  Boolean m_owned_camera;

  //! The id of the input format.
  File_format::Id m_input_format_id;

  void set_head_light(Configuration* config);

  /*! Set the camera and draws the background. Any other rendering
   * calls that should not me done for each pass of the anti aliasing
   * should go in this method.
   */
  void initialize_rendering(Draw_action* draw_action);

  /*! Render the scene graph (all passes). All rendering that should be
   * done for each anti alias pass should go in this method.
   */
  void render_scene_graph(Draw_action* draw_action);

  /*! Obtain the active navigation-info node. */
  Navigation_info* get_active_navigation_info();

  /*! Copy constructor. */
  Scene_graph(const Scene_graph &) {}
};

#if (defined _MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the scene configuration container.
inline Configuration* Scene_graph::get_configuration() const
{ return m_configuration; }

//! \brief obtains the navigation info bindable stack.
inline Bindable_stack* Scene_graph::get_navigation_info_stack()
{ return &m_navigation_info_stack; }

//! \brief obtains the background bindable stack.
inline Bindable_stack* Scene_graph::get_background_stack()
{ return &m_background_stack; }

//! \brief obtains the camera bindable stack.
inline Bindable_stack* Scene_graph::get_camera_stack()
{ return &m_camera_stack; }

//! \brief obtains the root of the scene graph.
inline Scene_graph::Shared_group Scene_graph::get_root() const
{ return m_root; }

//! \brief obtains the input file format.
inline File_format::Id Scene_graph::get_input_format_id() const
{ return m_input_format_id; }

//! \brief sets the input file format.
inline void Scene_graph::set_input_format_id(File_format::Id format_id)
{ m_input_format_id = format_id; }

SGAL_END_NAMESPACE

#endif
