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

#ifndef SGAL_SCEME_HPP
#define SGAL_SCEME_HPP

/*! \file
 * A concrete scene type derived from this pure scene type manage the
 * scene. Typically there is a single scene per application.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Agent.hpp"

SGAL_BEGIN_NAMESPACE

class Window_item;

class SGAL_SGAL_DECL Scene : public SGAL::Agent {
public:
  /*! Destruct.
   */
  virtual ~Scene();

  /*! Create the scene.
   */
  virtual void create_scene();

  /*! Destroy the scene.
   */
  virtual void destroy_scene();

  /*! Initialize the scene.
   */
  virtual void init_scene() = 0;

  /*! Clear the scene.
   */
  virtual void clear_scene() = 0;

  /*! Initialize a window.
   * \param[in] window_item the window to initialize
   */
  virtual void init_window(Window_item* window_item, Uint width, Uint height);

  /*! Clear a window.
   * \param[in] window_item the window to initialize
   */
  virtual void clear_window(Window_item* window_item);

  /*! Reshape the viewport of a window of the scene
   * It is assumed that the window context is the current context
   * \param[in] window_item the window to reshape
   * \param[in] width the new width of the window
   * \param[in] height the new height of the window
   */
  virtual void reshape_window(Window_item* window_item, Uint width, Uint height);

  /*! Draw into a window of the scene
   * It is assumed that the window context is the current context
   * \param[in] window_item the window to draw
   * \param[in] dont_accumulate indicates whether accumulation not be performed.
   */
  virtual void draw_window(Window_item* window_item, Boolean dont_accumulate);

  /*! Print the scene information.
   */
  virtual void print_scene_help(void);
};

//! \brief destruct.
inline Scene::~Scene() {}

//! \brief creates the scene.
inline void Scene::create_scene() {}

//! \brief destroys the scene.
inline void Scene::destroy_scene() {}

//! \brief initializes a window. Typically used to create a context.
inline void Scene::init_window(Window_item* /* window_item */,
                               Uint /* width */, Uint /* height */)
{}

//! \brief clears a window.
inline void Scene::clear_window(Window_item* /* window_item */) {}

//! \brief reshapes the viewport of a window of the scene.
inline void Scene::reshape_window(Window_item* /* window_item */,
                                  Uint /* width */, Uint /* height */)
{}

//! \brief draws into a window of the scene
inline void Scene::draw_window(Window_item* /* window_item */,
                               Boolean /* dont_accumulate */)
{}

//! \brief prints the scene information */
inline void Scene::print_scene_help(void) {}

SGAL_END_NAMESPACE

#endif
