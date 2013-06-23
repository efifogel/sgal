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

#ifndef SGAL_GRADIENT_BACKGROUND_HPP
#define SGAL_GRADIENT_BACKGROUND_HPP

/*! \file
 * Purpose: A gradient color background.
 *
 *  Inherits from Background
 */

#include "SGAL/basic.hpp"
#include "SGAL/Background.hpp"
#include "SGAL/Vector4f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SGAL_SGAL_DECL Gradient_background : public Background {
public:
  enum {
    FIRST = Node::LAST - 1,
    ULCOLOR,
    URCOLOR,
    LLCOLOR,
    LRCOLOR,
    LAST
  };

  /*! Constructor */
  Gradient_background(Boolean proto = false);

  /*! Destructor */
  virtual ~Gradient_background();

  /*! Construct the prototype */
  static Gradient_background* prototype()
  { return new Gradient_background(true); }

  /*! Clone */
  virtual Container* clone() { return new Gradient_background(); }

  void SetULColor(Vector4f color);
  Vector4f GetULColor() const;
  void SetURColor(Vector4f color);
  Vector4f GetURColor() const;
  void SetLLColor(Vector4f color);
  Vector4f GetLLColor() const;
  void SetLRColor(Vector4f color);
  Vector4f GetLRColor() const;

  virtual Trav_directive Draw(Draw_action* draw_action);

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  /*! Initialize the node prototype */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto* get_prototype();

private:

  static Container_proto* s_prototype;

  Vector4f m_ulColor;
  Vector4f m_urColor;
  Vector4f m_llColor;
  Vector4f m_lrColor;
  static float m_size;

  Appearance* m_appearance;

  void DrawPolygon(Context* context);
  void CreateAppearance();
  Gradient_background(const Background&) {}

  /** default color values.  */

  static const Vector4f m_def_ul_color;
  static const Vector4f m_def_ur_color;
  static const Vector4f m_def_ll_color;
  static const Vector4f m_def_lr_color;

};

SGAL_END_NAMESPACE

#endif
