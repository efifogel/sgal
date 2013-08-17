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
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_BACKGROUND_HPP
#define SGAL_BACKGROUND_HPP

/*! \file
 * Scene background
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Bindable_node.hpp"

SGAL_BEGIN_NAMESPACE

class Scene_graph;
class Draw_action;
class Bindable_stack;
class Container_proto;

/*! Virtual base class for backgrounds */
class SGAL_SGAL_DECL Background : public Bindable_node {
public:
  enum {
    FIRST = Bindable_node::LAST - 1,
    CLEAR_COLOR,
    CLEAR_DEPTH,
    CLEAR_STENCIL,
    LAST
  };

  /*! Constructor */
  Background(Boolean proto = false);

  /*! Destructor */
  virtual ~Background();

  /*! Construct the prototype */
  static Background* prototype() { return new Background(true); }

  /*! Clone */
  virtual Container* clone() { return new Background(); }

  /*! Initialize the protoype */
  virtual void init_prototype();

  /*! Delete the protoype */
  virtual void delete_prototype();

  /*! Obtain the protoype */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  //@}

  /*! Obtain the bindable stack */
  virtual Bindable_stack* get_stack();

  /*! Set the attributes of this container */
  virtual void set_attributes(Element* elem);

  /*! Add the container to a given scene
   * \param scene_graph the given scene
   */
  virtual void add_to_scene(Scene_graph* scene_graph);

  /*! Draw the background */
  virtual void draw(Draw_action* draw_action);

  /*! Set the scene graph */
  void set_scene_graph(Scene_graph* sg) { m_scene_graph = sg; }

  /*! Obtain the flag that indicates whether to clear the color buffer */
  Boolean get_clear_color() const { return m_clear_color; }

  /*! Set the flag that indicates whether to clear the color buffer */
  void set_clear_color(Boolean clear) { m_clear_color = clear; }

  /*! Obtain the flag that Indicates whether to clear the depth buffer */
  Boolean get_clear_depth() const { return m_clear_depth; }

  /*! Set the flag that Indicates whether to clear the depth buffer */
  void set_clear_depth(Boolean clear) { m_clear_depth = clear; }

  /*! Obtain the flag that Indicates whether to clear the stencil buffer */
  Boolean get_clear_stencil() const { return m_clear_stencil; }

  /*! Set the flag that Indicates whether to clear the stencil buffer */
  void set_clear_stencil(Boolean clear) { m_clear_stencil = clear; }

protected:
  /*! Draw a 2d polygon with texture coordinates */
  void draw_polygon();

  /*! The Scene_graph */
  Scene_graph* m_scene_graph;

private:
  /*! The tag that identifies this container type */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Indicates whether the color buffer should be cleared */
  Boolean m_clear_color;

  /*! Indicates whether the depth buffer should be cleared */
  Boolean m_clear_depth;

  /*! Indicates whether the stencil buffer should be cleared */
  Boolean m_clear_stencil;
};

SGAL_END_NAMESPACE

#endif
