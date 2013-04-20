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
// $Revision: 6205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef PLAYER2_SCENE_HPP
#define PLAYER2_SCENE_HPP

/*! \file
 */

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "Player_scene.hpp"

SGAL_BEGIN_NAMESPACE

#if defined(USE_CGM)
class Cubical_gaussian_map_geo;
#endif

class Spherical_gaussian_map_base_geo;
class Switch;

SGAL_END_NAMESPACE

class Player2_option_parser;

class Player2_scene : public Player_scene {
public:
#if defined(USE_CGM)
  /*! The CGM geometry */
  typedef boost::shared_ptr<SGAL::Cubical_gaussian_map_geo>
    Shared_cubical_gaussian_map_geo;
#endif
  typedef boost::shared_ptr<SGAL::Spherical_gaussian_map_base_geo>
    Shared_spherical_gaussian_map_base_geo;

  /*! Constructor */
  Player2_scene(Player2_option_parser* option_parser);

  /*! Destructor */
  virtual ~Player2_scene();

  /*! \brief initializes the secene */
  virtual void init_scene();

  /*! Draw into a window of the scene
   * It is assumed that the window context is the current context
   * \param window_item the window to draw
   * \param dont_accumulate indicates that no accumulation should be performed
   */
  virtual void draw_window(SGAL::Window_item* window_item,
                           SGAL::Boolean dont_accumulate);

private:
  Player2_option_parser* m_option_parser;
  
#if defined(USE_CGM)
  /*! The CGM geometry */
  Shared_cubical_gaussian_map_geo m_cgm_geo;
#endif

  /*! The SGM geometry */
  Shared_spherical_gaussian_map_base_geo m_sgm_geo;
};

#endif
