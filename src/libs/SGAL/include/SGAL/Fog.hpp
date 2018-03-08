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
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FOG_HPP
#define SGAL_FOG_HPP

/*! \file
 * Purpose: An implementation of Fog.
 *
 * Description:  Not implemented yet
 *
 *  Inherits from Node.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;

class SGAL_SGAL_DECL Fog : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    ON,
    FOG_MODE,
    FOG_DENSITY,
    FOG_START,
    FOG_END,
    FOG_INDEX,
    FOG_COLOR,
    LAST
  };

  /*! Constructor */
  Fog(Boolean proto = false);

  /*! Destructor */
  virtual ~Fog();

  /*! Construct the prototype */
  static Fog* prototype() { return new Fog(true); }

  /*! Create a new container of this type (virtual copy constructor).
   * \return a new container of this type.
   */
  virtual Container* create();

  enum Fog_mode {
    LINEAR_FOG,
    EXP_FOG,
    EXP2_FOG
  };

  void set_on(Boolean on);
  Boolean GetOn();

  void set_mode(FogMode mode);
  FogMode get_mode();

  void set_density(Float density);
  Float get_density();

  void set_start(Float start);
  Float get_start();

  void set_end(Float end);
  Float get_end();

  void set_index(Float index);
  Float get_index();

  void set_color(const Vector4f& color);
  void get_color(Vector4f& color);
  void set_color(Float v0, Float v1, Float v2, Float v3);
  void get_color(Float* v0, Float* v1, Float* v2, Float* v3);

  void push();
  void pop(Fog* outgoing_fog);
  virtual Trav_directive draw(Draw_action* draw_action);

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

#if 0
  virtual Attribute_list get_attributes() { }
#endif

private:
  void init(void);

  Boolean m_on;
  Int m_mode;
  Float m_density;
  Float m_start;
  Float m_end;
  Float m_index;
  Vector4f m_color;
};

inline void Fog::set_color(const Vector4f& color)
{ SetColor(color.Get(0), color.Get(1), color.Get(2), color.Get(3)); }

inline void Fog::get_color(Vector4f& color)
{ get_color(&color[0], &color[1], &color[2], &color[3]); }

//! \brief creates a new container of this type (virtual copy constructor).
inline Container* Fog::create()
{ return new Fog(); }

SGAL_END_NAMESPACE

#endif
