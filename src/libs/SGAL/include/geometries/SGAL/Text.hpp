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
// $Revision: 12369 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_TEXT_HPP
#define SGAL_TEXT_HPP

/*! \file
 * A node in the scene graph that render text.
 */

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Geometry.hpp"

SGAL_BEGIN_NAMESPACE

class Draw_action;
class Font_style;
class Container_proto;
class Element;
class Cull_context;

class SGAL_CLASSDEF Text : public Geometry {
public:
  enum {
    FIRST = Geometry::LAST - 1,
    STRING,
    LAST
  };

  /*! Constructor */
  Text(Boolean proto = false);

  /*! Destructor */
  virtual ~Text();
  
  /* Construct the prototype */
  static Text* prototype() { return new Text(true); }

  /*! Clone */
  virtual Container* clone() { return new Text(); }

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

  /*! Set the attributes of this node */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  virtual void cull(Cull_context& /* cull_context */) {}

  virtual void draw(Draw_action* action);

  virtual void isect(Isect_action* /* isect_action */) {}

  /*! Calculate the sphere bound */
  virtual Boolean clean_sphere_bound();

  /*! Set the font style */
  void set_font_style(Font_style* font_style);

  /*! Obtain the font style */
  Font_style* get_font_style() const { return m_font_style; }

  /*! Return true if the geometry has color (as opposed to material) */
  virtual Boolean has_color() const { return SGAL_FALSE; }
  
protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! The text to render */
  std::string m_string;

  /*! The font style */
  Font_style* m_font_style;

  /*! Indicates whether a default fontStyle was constructed */
  Font_style* m_default_font_style;
};

SGAL_END_NAMESPACE

#endif
