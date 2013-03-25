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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#if (defined _MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <sstream>
#include <time.h>
#include <iostream>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Math_defs.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Context.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Navigation_info.hpp"
#include "SGAL/Navigation_info_types.hpp"
#include "SGAL/Field.hpp"
#include "SGAL/Load_int.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Record_action.hpp"
#include "SGAL/Execution_coordinator.hpp"
#include "SGAL/Sphere_bound.hpp"
#include "SGAL/Touch_sensor.hpp"
#include "SGAL/Time_sensor.hpp"
#include "SGAL/Configuration.hpp"
#include "SGAL/View_sensor.hpp"
#include "SGAL/Text_screen.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/GL_error.hpp"
#include "SGAL/Flow_sensor.hpp"
#include "SGAL/Scene_graph_int.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Route.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Snapshot.hpp"
#include "SGAL/Point_light.hpp"
#include "SGAL/Bindable_stack.hpp"
#include "SGAL/Camera.hpp"
#include "SGAL/Simulation.hpp"
#include "SGAL/Key_sensor.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Multisample.hpp"
#include "SGAL/Gl_wrapper.hpp"

SGAL_BEGIN_NAMESPACE

unsigned int Scene_graph::s_min_redraw_period = 500;
//! \todo Critical_section Scene_graph::s_render_cs;

/*! A constructor.
 * \param syncronize indicates whether the creation and the rendering of the
 * scene graph must be syncronized. This is necessary when the scene graph
 * is created and rendered by two different threads
 */
Scene_graph::Scene_graph(bool syncronize) : 
  m_root(0), 
  m_head_light(NULL),
  m_camera(NULL),
  m_navigation_info(NULL),
  m_navigation_root(NULL),
  m_context(NULL),
  m_is_scene_done(!syncronize), 
  m_does_have_lights(false),
  m_current_light_id(0),
  m_isect_action(NULL),
  m_execution_coordinator(0),
  m_default_event_filter(0),
  m_configuration(NULL),
  m_view_sensor(NULL),
  m_active_key_sensor(NULL),
  m_fps(0),
  m_fps_counter(0),
  m_fps_start_time(0),
  m_last_render_time(0),
  m_jsw_engine(0),
  //! \todo m_scripts_sai(0),
  //! \todo m_text_screen(0),
  m_is_isect_required(true),
  m_is_camera_in_focus(false),
  m_owned_configuration(false),
  m_owned_navigation_info(false),
  m_owned_camera(false),
  m_owned_head_light(false)
{
  m_isect_action = new Isect_action();
  m_touch_sensors.clear();
  m_time_sensors.clear();

  /*! \todo container factory initialization.
   * During initialization of the container factory an instance of every
   * container is constructed and registered. This is done automatically
   * through the REGISTER_TO_FACTORY() macro (provided in
   * Container_factory.h). An alternative way is to call the initialize()
   * method of the factory explicitly, in case allocation is desired to be
   * concentrated elsewhere.
   *
   * Container_factory* factory = Container_factory::get_instance();
   * factory->initialize();
   */
}

/*! Destructor */
Scene_graph::~Scene_graph()
{
  // \todo sai
  // if (m_scripts_sai != NULL) delete m_scripts_sai;
  
  for (unsigned int i = 0 ; i < m_containers.size(); ++i)
    delete m_containers[i];
  m_containers.clear();

  Container_map_iter iter;
  for (iter = m_instances.begin(); iter != m_instances.end(); ++iter) {
    Container* c = iter->second;
    delete c;
  }
  m_instances.clear();

  delete m_isect_action;
  delete m_execution_coordinator;
  //! \todo delete m_text_screen;

#ifdef JAVA_SCRIPT
  if (m_jsw_engine != NULL) m_jsw_engine->destroy();
#endif 

  Navigation_info* nav = get_active_navigation_info();
  if (nav) nav->unregister_events();

  destroy_defaults();
  
  //! \todo destroy stacks
}

/*! \brief obtains a pointer to the JScript Interpreter engine.
 * If needed - creates the engine (if it is the first call first time).
 */
//! \todo sai
// JSW_engine_int* Scene_graph::get_jsw_engine()
// {
// #ifdef JAVA_SCRIPT
//   if (m_jsw_engine == NULL)
//     m_jsw_engine = JSW_engine_int::create(&m_js_error_reporter);
// #endif
//   return m_jsw_engine;
// }

/*! \brief obtains a pointer to the SAI serving the script nodes
 * If needed - allocate a new instance
 */
//! \todo sai
// SAI * Scene_graph::get_scripts_sai()
// {
//   if (m_scripts_sai == 0) m_scripts_SAI = new SAI(this);
//   return m_scripts_SAI;
// }

/*! \brief sets the context in the scene graph and in all relevant
 * nodes accessible from the scene graph.
 */
void Scene_graph::set_context(Context* context)
{
  m_context = context;
  if (m_isect_action) m_isect_action->set_context(context);
  m_free_selection_ids.push_back(std::make_pair(1, (0x1 << 24) - 1));
}

/*! \brief intializes the context in the scene grpha and in all relevant
 * nodes accessible from the scene graph.
 */
void Scene_graph::init_context()
{
  m_context->set_depth_enable(true);
  m_context->set_cull_face(Gfx::BACK_CULL);
  m_context->draw_app(0);
  glReadBuffer(GL_BACK);

  // Activate seamless cube map if supported.
  // \todo The GL_TEXTURE_CUBE_MAP_SEAMLESS state should be handled by the
  //       context.
  SGAL_assertion(m_configuration);
  if (Gfx_conf::get_instance()->is_seamless_cube_map_supported() &&
      m_configuration->is_seamless_cube_map())
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

#if 0
  Container_vector_iter ci;
  for (ci = containers_begin(); ci != containers_end(); ++ci)
    if (*ci) (*ci)->attach_context(m_context);
#endif
}

/*! \brief releases the context. */
void Scene_graph::release_context()
{
#if 0
  Container_vector_iter it;
  for (it = containers_begin(); it != containers_end(); ++it)
    if (*it) (*it)->detach_context(m_context);
#endif
}

/*! \brief renders the scene graph. */
void Scene_graph::draw(Draw_action* draw_action) 
{
  //! \todo Auto_lock lock(&s_render_CS);
  if (!m_context) return;

  //! \todo should the user set this?
  draw_action->set_context(m_context);
  
  if (!m_is_scene_done) {
    // this is to clear the scereen with no scene graph loaded
    m_context->make_current();
    m_context->clear_color_depth_buffer();
    return;
  }

  Accumulation* acc = NULL;
  Gfx::Poly_mode poly_mode = Gfx::FILL_PMODE;
  Configuration* config = draw_action->get_configuration();
  if (config) {
    acc = config->get_accumulation();
    poly_mode = config->get_poly_mode();
    m_context->set_poly_mode(poly_mode);
  }
  bool accumulation_enabled = acc && acc->is_enabled();
  if (poly_mode != Gfx::FILL_PMODE) accumulation_enabled = false;

  if (m_execution_coordinator && !m_execution_coordinator->is_loading_done()) {
    // If Loading not done, redraw scene at least every MinRedrawTime.
    Uint t;
#if (defined _MSC_VER)
    t = ::GetTickCount();
#else
    t = 0;      //! \todo GetTickCount
#endif
    if (t - m_last_render_time > s_min_redraw_period) {
      m_execution_coordinator->set_rendering_required();
    }
  }

  //! \todo
#if 0
  if (m_execution_coordinator &&
      m_execution_coordinator->is_rendering_required())
  {
  }
#endif

#if (defined _MSC_VER)
  m_last_render_time = ::GetTickCount();
#else
  m_last_render_time = 0;     //! \todo GetTickCount
#endif
  if (poly_mode != Gfx::FILL_PMODE) m_context->set_cull_face(Gfx::NO_CULL);
  initialize_rendering(draw_action);
  if (accumulation_enabled && acc->is_active()) acc->pre_render(draw_action);
  render_scene_graph(draw_action);
  if (accumulation_enabled && acc->is_active()) acc->post_render(draw_action);
  if (poly_mode != Gfx::FILL_PMODE) m_context->set_cull_face(Gfx::BACK_CULL);

  if (accumulation_enabled) {   
    if (acc->is_done() && draw_action->get_snap()) {
      draw_action->set_snap_from_front(false);
      process_snapshots(draw_action);
    }
  }
  else if (draw_action->get_snap()) {
    draw_action->set_snap_from_front(false);
    process_snapshots(draw_action);
  }
  
  if (acc && !acc->is_active()) acc->reset_delay_time();
  
  // temporary code to get FPS
  if (m_fps_counter == 0) m_fps_start_time = clock();
  else if (m_fps_counter == 20) {
    time_t t2 = clock();
    float elapsed = (float)(t2-m_fps_start_time); 
    if (elapsed != 0) {
      m_fps = m_fps_counter / (elapsed/(float)CLOCKS_PER_SEC);
    }   
    //! \todo m_execution_coordinator->set_frame_rate(m_fps);
    m_fps_counter = -1;
    
    if (config && config->is_display_fps()) {
      std::cout << "fps: " << m_fps << std::endl;
      std::ostringstream stream_fps;
      stream_fps << m_fps;
      put_text_string(0, stream_fps.str());
    }
    else clear_text_screen();
  }
    
  ++m_fps_counter;

  // since this is the end of the current frame, we reset the isect.
  m_is_isect_required = true;
}

/*! \brief sets the camera and draws the background.
 * \todo make it so that it is not called for each pass of the accumulation!
 */
void Scene_graph::initialize_rendering(Draw_action* draw_action) 
{
  if (!m_context) return;

  draw_action->reset_pass_no();
  m_context->make_current();

  Camera* camera = get_active_camera();
  m_context->set_active_camera(camera);

#if 0
  // Set the clipping planes around the bounding sphere. 
  if (!m_is_camera_in_focus && camera->get_is_dynamic()) {
    const Sphere_bound& sb = m_navigation_root->get_sphere_bound();
    camera->set_dynamic_clipping_planes(sb.get_center(), sb.get_radius());
    m_is_camera_in_focus = true;
  }
#endif
  
  set_head_light(draw_action->get_configuration());

  if (draw_action->get_clear()) {
    Background* bg = get_active_background();
    if (bg) bg->draw(draw_action);
    else m_context->clear(draw_action->get_clear());
  }

  if (draw_action->get_apply_camera()) camera->draw(draw_action);

  /*! \todo move to Context! */
  Configuration* config = draw_action->get_configuration();
  if (config) {
    Multisample* ms = config->get_multisample();
    if (ms && ms->is_enabled()) glEnable(GL_MULTISAMPLE);
  }
}

/*! \brief renders the scene graph (all passes). */
void Scene_graph::render_scene_graph(Draw_action* draw_action) 
{
  /*
    draw_action->Apply(m_root);
    if (draw_action->IsSecondPassRequired()) 
    {
    draw_action->IncreasePassNo();
    draw_action->Apply(m_root);
    }
  */
  
  //! \todo m_execution_coordinator->inc_frame_counter();
  Camera* act_camera = get_active_camera();
  Cull_context cull_context;
  cull_context.set_head_light(get_head_light());
  cull_context.cull(m_root, act_camera);
  cull_context.draw(draw_action);
  
  // Draw Text screen.
#if 0
  if (m_text_screen)
    m_text_screen->draw(draw_action);
#endif
  
  //! \todo m_execution_coordinator->reset_rendering_required();
}

/*! This method is called whenever there is a need to see if the mouse 
 * is positioned on an object that has a touch sensor.
 * See isect_action for more details.
 */
void Scene_graph::isect(Uint x, Uint y)
{
  if (!m_is_isect_required || m_touch_sensors.empty()) return;

  //! \todo Auto_lock lock(&s_render_cs);

  m_is_isect_required = false;
  if (!m_is_scene_done || !m_context) return;
  
  Camera* act_camera = get_active_camera();
  /*! \todo this is all wrong!
   * Instead of the scene graph calling Camera::draw(Isect_action) , the
   * following public functions entry points should be implemented to apply
   * picking in Camera:
   *
   * virtual int pick (csNode* node, int x, int y, csHit& hit);
   *
   * virtual int pick (csNode* node, int x, int y, csHit& hit, int xo, int
   *                   yo, int xs, int ys);
   *
   * Applies a csIsectAction to node using the ray formed by the viewpoint
   * and window coordinates x and y, and stores the result in hit. The first
   * form gets the window from the current context's viewport, the second
   * form has the window origin passed in as xo and yo and the window size
   * passed in as xs and ys
   *
   * This currently works, because the action is not used in Camera::draw()
   */
  act_camera->draw(m_isect_action);
  
  Uint x0 = 0, y0 = 0, width = 0, height = 0;
  m_context->get_viewport(x0, y0, width, height);
  // if the viewport is not initialized return here
  if (width == 0) return;
    
  m_context->make_current();
  //! \todo should be done in make_current

  glViewport(x0, y0, width, height);
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_NORMALIZE);
  glDisable(GL_DITHER);
  glShadeModel(GL_FLAT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  m_context->clear_color_depth_buffer();
  m_navigation_root->isect(m_isect_action);
  glPopAttrib();

  Image_base::Format format = Image_base::kRGB8_8_8;
  GLenum gl_format = Image_base::get_format_format(format);
  GLenum gl_type = Image_base::get_format_type(format);
  SGAL_assertion_code(Uint bits = Image_base::get_format_size(format));
  SGAL_assertion(get_num_selection_ids() < (0x1 << bits));
  // Ideally, we would declare pixel[] as an array of size
  // Image_base::get_format_components(format) (Uchar pixel[num_components]).
  // Unfortunately, MSVC does not allow defining an array of non-constant
  // size, so we define it to be of the maximum (of 3 and 4).
#if (_MSC_VER <= 1600)
  Uchar pixel[4];
#else
  Uchar pixel[Image_base::get_format_components(format)];
#endif
  glReadPixels(x, y, 1, 1, gl_format, gl_type, pixel);
  Uint index = m_isect_action->get_index(pixel);
  Touch_sensor_iter it;
  for (it = m_touch_sensors.begin(); it != m_touch_sensors.end(); ++it) {
    Boolean is_over = (*it)->is_in_range(index);
    (*it)->set_selection_id(is_over ? index : 0);
  }
}

/*! \brief processes all snapshot nodes. */
void Scene_graph::process_snapshots(Draw_action* action) 
{
  for (Snapshot_iter i = m_snapshots.begin(); i != m_snapshots.end(); ++i) {
    Snapshot* snapshot = *i;
    snapshot->draw(action);
  }
}

/*! \brief adds a Simulation node to the list of Simulation nodes. */
void Scene_graph::add_simulation(Simulation* simulation)
{ m_simulations.push_back(simulation); }

/*! Start simulation */
void Scene_graph::start_simulation()
{
  Simulation_iter i;
  for (i = m_simulations.begin(); i != m_simulations.end(); ++i) {
    Simulation* simulation = *i;
    simulation->start();
  }
}

/*! \brief creates the execution coordinator for this scene graph. It is later
 * passed on to each node.
 */
void Scene_graph::create_execution_coordinator()
{
  /*! \todo   
  if (!m_execution_coordinator) {
    m_execution_coordinator = new Execution_coordinator();
    m_antialiasing.set_ec(m_execution_coordinator);
  }
  */
}

/*! \brief adds a container to the name-less container-pool. */
void Scene_graph::add_container(Container* container)
{
  if (!container) return;
  m_containers.push_back(container);
}

/*! \brief adds a container to the instance container-pool. */
void Scene_graph::add_container(Container* container, const std::string& name)
{
  if (!container) return;
  container->set_name(name);
  m_instances[name] = container;
}

/*! \brief adds a touch sensor to the scene graph. */
void Scene_graph::add_touch_sensor(Touch_sensor* touch_sensor) 
{ m_touch_sensors.push_back(touch_sensor); }

/*! \brief allocates an interval of selection ids, given the size of the
 * interval to allocate.
 */
Uint Scene_graph::allocate_selection_ids(Uint num)
{
  std::cout << "Scene_graph::allocate_selection_ids: " << num << std::endl;
  Selection_id_interval_iter it = m_free_selection_ids.begin();
  for (; it != m_free_selection_ids.end(); ++it) {
    Selection_id_interval& interval = *it;
    if (num > interval.second) continue;
    Uint start = interval.first;
    if (num == interval.second) m_free_selection_ids.erase(it);
    else interval = std::make_pair(interval.first+num, interval.second-num);
    return start;
  }
  std::cerr << "Failed to allocate interval of " << num << " ids!"
            << std::endl;
  return 0;
}

/*! \brief frees an interval of selection ids, given the starting id and size
 * of the interval to free.
 */
void Scene_graph::free_selection_ids(Uint start, Uint num)
{
  // If the list of free intervals is empty, simply create a new free interval
  // and insert it into the list of free intervals:
  if (m_free_selection_ids.empty()) {
    Selection_id_interval new_interval = std::make_pair(start, num);
    m_free_selection_ids.push_front(new_interval);
    return;
  }


  // Iterate over the list of free intervals:
  Selection_id_interval_iter it;
  for (; it != m_free_selection_ids.end(); ++it) {
    Selection_id_interval& interval = *it;
    Uint interval_end = interval.first + interval.second;

    // If the new free interval starts efter the current interval ends,
    // continue.
    if (interval_end < start) continue;

    // If the new free interval starts where the current interval ends,
    // enlarge the current interval
    if (interval_end == start) {
      interval.second += num;
      // If the next interval starts where the enlarged current interval
      // ends, further enlarge the current interval:
      Selection_id_interval_iter next = it;
      ++next;
      if (next == m_free_selection_ids.end()) return;
      Selection_id_interval& next_interval = *next;
      interval_end += num;
      if (interval_end == next_interval.first) {
        interval.second += next_interval.second;
        m_free_selection_ids.erase(next);
      }
      return;
    }
    // The new interval ends before the current interval starts. Insert
    // the new interval right before the current interval.
    SGAL_assertion(last < interval.first);
    Selection_id_interval new_interval = std::make_pair(start, num);
    m_free_selection_ids.insert(it, new_interval);
    return;
  }

  // All free intervals are strictly smaller than the new one.
  // Create a new free interval and insert it into the list of free intervals:
  Selection_id_interval new_interval = std::make_pair(start, num);
  m_free_selection_ids.push_back(new_interval);
}

/*! \brief adds a time sensor node to the scene graph. */
void Scene_graph::add_time_sensor(Time_sensor* time_sensor)
{ m_time_sensors.push_back(time_sensor); } 

/*! \brief adds a snapshot node to the list of snapshots nodes. */
void Scene_graph::add_snaphot(Snapshot* snapshot)
{ m_snapshots.push_back(snapshot); }

/*! \brief routes the Navigation_info node properly. */
void Scene_graph::route_navigation_info(Navigation_info* nav,
                                        Navigation_info_type type)
{
  switch (type) {
   case SGAL::EXAMINE:
    {
      // Create the navigation_info fields:
      nav->add_field(Navigation_sensor::TRANSLATION);
      nav->add_field(Navigation_sensor::ROTATION);
      nav->add_field(Navigation_sensor::MIN_ZOOM_DISTANCE);

      // Connect the Navigation_info (sensor) to the Transform:
      Field* pos_field = nav->get_field(Navigation_sensor::TRANSLATION);
      Field* orient_field = nav->get_field(Navigation_sensor::ROTATION);
      SGAL_assertion(pos_field);
      SGAL_assertion(orient_field);
  
      Transform* navigat_root = get_navigation_root();
      Field* sg_pos_field = navigat_root->get_field(Transform::TRANSLATION);
      Field* sg_orient_field = navigat_root->get_field(Transform::ROTATION);
      SGAL_assertion(sg_pos_field);
      SGAL_assertion(sg_orient_field);
  
      pos_field->connect(sg_pos_field);
      orient_field->connect(sg_orient_field);
    }
    break;
    
   case SGAL::FLY: SGAL_warning_msg(0, "\"FLY\" not supported yet!"); break;
   case SGAL::WALK: SGAL_warning_msg(0, "\"WALK\" not supported yet!"); break;

   case SGAL::NONE:
   case SGAL::TRANSFORM: break;
   case NUM_TYPES: SGAL_assertion(0); return;
  }
}

/*! \brief sets the scene configuration container. */
void Scene_graph::set_configuration(Configuration* config) 
{
  if (m_owned_configuration) {
    if (m_configuration) delete m_configuration;
    m_owned_configuration = false;
  }
  m_configuration = config;
  //! \todo m_execution_coordinator->set_min_frame_rate(sconfig->get_min_frame_rate());
}

/*! \brief obtains a container by its instance name. */
Container* Scene_graph::get_container(const std::string& name)
{
  Container_map_iter ni = m_instances.find(name);
  if (ni != m_instances.end()) return ni->second;
  return 0;
}

/*! \todo Model_stats& Scene_graph::get_stats() */
// { return m_statistics; }

/*! Return a pointer to the execution executor
 * @return a pointer to the execution coordinator.
 */
Execution_coordinator* Scene_graph::get_execution_coordinator()
{ return m_execution_coordinator; }

/*! The function sets the flag that indicates whether a light source was
 * specified in the input file.
 * @param flag true if a light source was specified.
 */
void Scene_graph::set_have_lights(Boolean flag) { m_does_have_lights = flag; }

/*! \brief returns a flag that indicates whether a light source was
 * specified in the input file.
 * @return true if a light source was specified.
 */
Boolean Scene_graph::does_have_lights() { return m_does_have_lights; }

/*! \brief returns a unique id (0-7) for a light source. We are assuming
 * that there will not be more than 8 light sources in the scene and that light
 * sources can not be reused.
 * @return a uniqu id for light.
 */
Int Scene_graph::get_unique_light_id()
{
  int tmp = m_current_light_id;
  ++m_current_light_id;
  return tmp;
}

/*! \brief sets the root of the scene graph. */
void Scene_graph::set_root(Group* root) { m_root = root; }

/*! \brief sets the navigation root. */
void Scene_graph::set_navigation_root(Transform* nav_root)
{
  SGAL_assertion(nav_root);
  m_navigation_root = nav_root;

  // Create the navigation_sensor fields:
  nav_root->add_field(Transform::TRANSLATION);
  nav_root->add_field(Transform::ROTATION);
}

/*! \brief */
void Scene_graph::set_head_light(Configuration* config)
{
  if (!m_head_light || (config && !config->is_fixed_head_light())) return;

#if 0
  const Sphere_bound& sb = m_navigation_root->get_sphere_bound();
  const Vector3f& center = sb.get_center();  

  Vector3f org;
  Vector3f loc;
  if (center == org) loc[2] = 1;
  else loc.negate(center);
  loc.scale(1000);
  m_head_light->set_location(loc);
#endif
  m_head_light->set_location(Vector3f(0, 0, 1000));
  /*
    Vector3f direction(0, 0, -1);
    EMatrix4f m;
    m_camera_pool.get_active_camera()->GetViewMat(m);
    m.Transpose();
    direction.XformVec(direction,m); 
    m_head_light->SetDirection(direction);
  */
}

/*! \brief performs pre cascade activity. */
void Scene_graph::signal_cascade_start()
{
  /*! \todo if (m_scripts_sai != NULL)
   * m_scripts_sai->signal_cascade_start();
   */
}
 
/*! \brief performs post cascade activity. */
void Scene_graph::signal_cascade_end()
{
  if (m_view_sensor != NULL)
    m_view_sensor->update();

  /*! \todo if (m_scripts_sai != NULL)
    m_scripts_sai->signal_cascade_end();
  */
}
 
/* \brief destroys default (owned) nodes. */
void Scene_graph::destroy_defaults()
{
  if (m_owned_configuration) {
    if (m_configuration) {
      delete m_configuration;
      m_configuration = NULL;
    }
    m_owned_configuration = false;
  }

  if (m_owned_navigation_info) {
    if (m_navigation_info) {
      delete m_navigation_info;
      m_navigation_info = NULL;
    }
    m_owned_navigation_info = false;
  }

  if (m_owned_camera) {
    if (m_camera) {
      delete m_camera;
      m_camera = NULL;
    }
    m_owned_camera = false;
  }

  if (m_owned_head_light) {
    if (m_head_light) {
      delete m_head_light;
      m_head_light = NULL;
    }
    m_owned_head_light = false;
  }
}
 
/*! \brief creates default nodes and route them appropriately. */
void Scene_graph::create_defaults()
{
  // The default Configuration container:
  if (!m_configuration) {
    m_configuration = new Configuration();
    SGAL_assertion(m_configuration);
    m_owned_configuration = true;
  }

  // The default navaigation info:
  if (!get_active_navigation_info()) {
    m_navigation_info = new Navigation_info;
    SGAL_assertion(m_navigation_info);
    m_owned_navigation_info = true;
    m_navigation_info->set_scene_graph(this);
    m_navigation_info_stack.insert(m_navigation_info);
    route_navigation_info(m_navigation_info, EXAMINE);
  }
    
  // The default camera:
  if (!get_active_camera()) {
    m_camera = new Camera;
    SGAL_assertion(m_camera);
    m_owned_camera = true;
    m_camera->set_scene_graph(this);
    m_camera_stack.insert(m_camera);
  }

  // The default light:
  if (!does_have_lights()) {
    m_head_light = new Point_light();
    SGAL_assertion(m_head_light);
    m_owned_head_light = true;
    set_head_light(m_configuration);
    m_head_light->set_ambient_intensity(1);
    get_root()->add_child(m_head_light);
    set_have_lights(true);
  }

  // Zoom distance:
  Field* sc_mzd_field =
    m_configuration->get_field(Configuration::MIN_ZOOM_DISTANCE);
  if (!sc_mzd_field)
    sc_mzd_field = m_configuration->add_field(Configuration::MIN_ZOOM_DISTANCE);

  // Connect the Configuration fields:
  Field* sg_mzd_field =
    m_configuration->get_field(Configuration::MIN_ZOOM_DISTANCE);
  SGAL_assertion(sg_mzd_field);

  sg_mzd_field->connect(sc_mzd_field);

  // Set the default center of rotation:
  const Sphere_bound& sb = m_navigation_root->get_sphere_bound();
  m_navigation_root->set_center(sb.get_center());
  
#if 0
  //! \todo Execution_coordinator* ec = get_execution_coordinator();
  if (ec) {
    const Sphere_bound& sb = m_navigation_root->get_sphere_bound();
    ec->set_scene_bounding_sphere_radius(sb.get_radius());
  }
#endif
}

/*! \brief binds the bindable nodes and activate the key_sensor. */
void Scene_graph::bind()
{
  m_navigation_info_stack.bind_top();
  m_camera_stack.bind_top();
  m_background_stack.bind_top();
  if (m_active_key_sensor != NULL) m_active_key_sensor->activate();
}

/*! \brief */
void Scene_graph::clear_text_screen()
{
#if 0
  if (m_text_screen) {
    m_text_screen->clear();
  }
#endif
}

/*! \brief */
void Scene_graph::put_text_string(int /* line */, const std::string& /* str */)
{
#if 0
  if (! m_text_screen) {
    m_text_screen = new Text_screen;
  }

  if (m_text_screen) {
    m_text_screen->put_string(line, str);
  }
#endif
}
	
/*! \brief */
void Scene_graph::add_text_string(const std::string& /* str */)
{
#if 0
  if (m_text_screen) {
    m_text_screen->add_string(str);
  }
#endif
}

/*! \bried computes the speed factor required by the sensors */
float Scene_graph::compute_speed_factor() const
{
  SGAL_assertion(m_navigation_root);
  const Sphere_bound& sb = m_navigation_root->get_sphere_bound();
  SGAL_assertion(m_configuration);
  float speed_factor = m_configuration->get_speed_factor();
  return (sb.get_radius() / speed_factor);
}

/*! \brief routes the connection. */
bool Scene_graph::route(const std::string& src_node_str,
                        const std::string& src_field_name,
                        const std::string& dst_node_str,
                        const std::string& dst_field_name,
                        Route* route)
{
  // Get the containers from the scene graph:
  Container* src_node = get_container(src_node_str);
  if (!src_node) return false;
  
  Container* dst_node = get_container(dst_node_str);
  if (!dst_node) return false;
  
  // Get the source and destination fields from the container:
  Field* src_field = src_node->get_source_field(src_field_name);
  if (!src_field) return false;

  Field* dst_field = dst_node->get_destination_field(dst_field_name);
  if (!dst_field) return false;

  route->set(src_node, src_field, dst_node, dst_field);
             
  return true;
}

/*! \brief routes the connection. */
bool Scene_graph::route(Container* src_node, const char* src_field_name,
                        Container* dst_node, const char* dst_field_name,
                        Route* route)
{
  if (!src_node) return false;
  if (!dst_node) return false;
  
  // Get the source and destination fields from the container:
  Field* src_field = src_node->get_source_field(src_field_name);
  if (!src_field) return false;

  Field* dst_field = dst_node->get_destination_field(dst_field_name);
  if (!dst_field) return false;

  route->set(src_node, src_field, dst_node, dst_field);
             
  return true;
}

/*! \brief obtains the active navigation-info node. */
Navigation_info* Scene_graph::get_active_navigation_info()
{ return static_cast<Navigation_info*>(m_navigation_info_stack.top()); }

/*! \brief obtains the active camera. */
Camera* Scene_graph::get_active_camera()
{ return static_cast<Camera*>(m_camera_stack.top()); }

/*! \brief obtains the active background. */
Background* Scene_graph::get_active_background()
{ return static_cast<Background*>(m_background_stack.top()); }

SGAL_END_NAMESPACE
