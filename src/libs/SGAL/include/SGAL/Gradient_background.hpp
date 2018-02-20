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
  static Gradient_background* prototype();

  /*! Clone */
  virtual Container* clone();

  /*! Initialize the node prototype */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Vector3f* ul_color_handle(const Field_info*) { return &m_ul_color; }
  Vector3f* ur_color_handle(const Field_info*) { return &m_ur_color; }
  Vector3f* ll_color_handle(const Field_info*) { return &m_ll_color; }
  Vector3f* lr_color_handle(const Field_info*) { return &m_lr_color; }
  //@}

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  virtual Trav_directive draw(Draw_action* draw_action);

  /*! Set the color of the upper left corner.
   * @param color (in) the color.
   */
  void set_ul_color(Vector4f color);

  /*! Obtain the color of the upper left corner.
   * @return the color.
   */
  Vector4f get_ul_color() const;

  /*! Set the color of the upper right corner.
   * @param color (in) the color.
   */
  void set_ur_color(Vector4f color);

  /*! Obtain the color of the upper right corner.
   * @return the color.
   */
  Vector4f get_ur_color() const;

  /*! Set the color of the lower left corner.
   * @param color (in)  the color.
   */
  void set_ll_color(Vector4f color);

  /*! Obtain the color of the lower left corner.
   * @return the color.
   */
  Vector4f get_ll_color() const;

  /*! Set the color of the lower right corner.
   * @param color (in)  the color.
   */
  void set_lr_color(Vector4f color);

  /*! Obtain the color of the lower right corner.
   * @return the color.
   */
  Vector4f get_lr_color() const;

private:
  static Container_proto* s_prototype;

  Vector4f m_ul_color;
  Vector4f m_ur_color;
  Vector4f m_ll_color;
  Vector4f m_lr_color;
  static float m_size;

  Appearance* m_appearance;

  void DrawPolygon(Context* context);
  void CreateAppearance();
  Gradient_background(const Background&) {}

  /** default color values. */
  static const Vector4f s_def_ul_color;
  static const Vector4f s_def_ur_color;
  static const Vector4f s_def_ll_color;
  static const Vector4f s_def_lr_color;
};

/*! \brief constructs the prototype. */
inline Gradient_background* Gradient_background::prototype()
{ return new Gradient_background(true); }

/*! \brief clones. */
inline Container* Gradient_background::clone()
{ return new Gradient_background(); }

/*! \brief sets the color of the upper left corner. */
inline void Gradient_background::set_ul_color(Vector4f color)
{ m_ul_color = color; }

/*! \brief obtains the color of the upper left corner. */
inline Vector4f Gradient_background::get_ul_color() const
{ return m_ul_color; }

/*! \brief sets the color of the upper right corner. */
inline void Gradient_background::set_ur_color(Vector4f color)
{ m_ur_color = color; }

/*! \brief obtains the color of the upper right corner. */
inline Vector4f Gradient_background::get_ur_color() const
{ return m_ur_color; }

/*! \brief sets the color of the lower left corner. */
inline void Gradient_background::set_ll_color(Vector4f color)
{ m_ll_color = color; }

/*! \brief obtains the color of the lower left corner. */
inline Vector4f Gradient_background::get_ll_color() const
{ return m_ll_color; }

/*! \brief sets the color of the lower right corner. */
inline void Gradient_background::set_lr_color(Vector4f color)
{ m_lr_color = color; }

/*! \brief Obtain the color of the lower right corner. */
inline Vector4f Gradient_background::get_lr_color() const
{ return m_lr_color; }

SGAL_END_NAMESPACE

#endif
