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
// $Source$
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_WRITTER_HPP
#define SGAL_WRITTER_HPP

/*! \file
 */

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph.hpp"
#include "SGAL/Group.hpp"
#include "SGAL/Transform.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Point_light.hpp"

SGAL_BEGIN_NAMESPACE

/*! A functor that Writes a scene graph to an output stream using a
 * parameterized formatter.
 */
class Writer {
private:
  // Copy constructor and assignment operator - not supported.
  Writer(const Writer &);
  Writer & operator=(const Writer &);

  /*! The scene graph to write */
  Scene_graph * m_scene_graph;

public:
  /*! Constructor */
  Writer(Scene_graph * scene_graph) : m_scene_graph(scene_graph) {}
  
  /*! Destructor */
  virtual ~Writer() {}

  /*! Write the scene graph */
  template <class Formatter>
  void operator() (Formatter & formatter) const
  {
    formatter.begin();
    Group * root = m_scene_graph->get_root();
    Transform * navigation_root = m_scene_graph->get_navigation_root();
    Point_light * head_light = m_scene_graph->get_head_light();
    Uint i;
    for (i = 0; i < root->get_child_count(); ++i) {
      Node * node = root->get_child(i);
      if ((node != navigation_root) && (node != head_light))
        formatter.write(node);
    }
    for (i = 0; i < navigation_root->get_child_count(); ++i) {
      Node * node = navigation_root->get_child(i);
      formatter.write(node);
    }
    formatter.end();
  }
};

SGAL_END_NAMESPACE

#endif
