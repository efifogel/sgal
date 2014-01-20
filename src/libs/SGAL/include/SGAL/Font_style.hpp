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

#ifndef SGAL_FONT_STYLE_HPP
#define SGAL_FONT_STYLE_HPP

/*! \file
 * A node in the scene graph that specifies font style for text node.
 */

#include "SGAL/basic.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"

SGAL_BEGIN_NAMESPACE

class Texture_font;
class Element;
class Container_proto;
class Font;
class Context;

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

class SGAL_SGAL_DECL Font_style : public Node {
public:
  enum {
    FIRST = Node::LAST - 1,
    FAMILY,
    HORIZONTAL,
    JUSTIFY,
    LANGUAGE,
    LEFT_TO_RIGHT,
    SIZE,
    SPACING,
    STYLE,
    TOP_TO_BOTTOM,
    LAST
  };

  /*! Constructor */
  Font_style(Boolean proto = false);

  /*! Destructor */
  virtual ~Font_style();

  /*! Construct the prototype */
  static Font_style * prototype() { return new Font_style(true); }

  /*! Clone */
  virtual Container * clone() { return new Font_style(); }

  /*! Set the attributes of this node */
  virtual void set_attributes(Element * elem);

  // virtual Attribute_list get_attributes();

  /*! Initialize the container prototype */
  virtual void init_prototype();

  /*! Delete the container prototype */
  virtual void delete_prototype();

  /*! Obtain the container prototype */
  virtual Container_proto * get_prototype();

  /// \name field handlers
  //@{
  std::string* family_handle(const Field_info*) { return &m_family; }
  std::string* style_handle(const Field_info*) { return &m_style; }
  Boolean* horizontal_handle(const Field_info*) { return &m_horizontal; }
  std::string* justify_handle(const Field_info*) { return &m_justify; }
  std::string* language_handle(const Field_info*) { return &m_language; }
  Boolean* l2r_handle(const Field_info*) { return &m_left_to_right; }
  Float* size_handle(const Field_info*) { return &m_size; }
  Float* spacing_handle(const Field_info*) { return &m_spacing; }
  Boolean* t2b_handle(const Field_info*) { return &m_top_to_bottom; }
  //@}

  /*! Draw the node while traversing the scene graph */
  virtual Action::Trav_directive draw(Draw_action *);

  //virtual void Isect(Isect_action *isect_action);

  /*! Draw one string */
  void draw_string(Context * context, const std::string & text);

  /*! Obtain the width and height of the string */
  void get_string_size(const std::string & str, Float & width, Float & height);

  /*! Obtain the font */
  Font * get_font();

  /*! */
  virtual void on_field_change(Field_info * field_info = NULL);

  /*! Set the flag that indicates whether the font is accentuated
   * \parameter b indicates whether the font is accentuated
   */
  void set_bold(Boolean b);

  /*! Set the flag that indicates whether the font is italicized (slanted)
   * \parameter i indicates whether the font is italicized
   */
  void set_italic(Boolean i);

  /*! Set the flag that indicates whether the font is antialiased
   * \param b indicates whether the font is antialiased
   */
  void set_antialias(Boolean a);

  /*! Set the flag that indicates the horizontal text direction
   * \param left_to_right indicates whether the direction is from right to left
   */
  void set_left_to_right(Boolean left_to_right);

  /*! Set the flag that indicates the vertical text direction
   * \param top_to_bottom indicates whether the direction is from top to bottom
   */
  void set_top_to_bottom(Boolean top_to_bottom);

protected:
  /*! obtains the tag (type) of the container */
  virtual const std::string & get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto * s_prototype;

  /*! The font name */
  std::string m_font_name;

  /*! The font family: "SERIF", "SANS", or "TYPEWRITER" */
  std::string m_family;

  /*! Indicates whether the text advances horizontally in its major direction */
  Boolean m_horizontal;

  /*! Determines alignment of the above text layout relative to the origin of
   * the object coordinate system
   */
  std::string m_justify;

  /*! The context of the language for the text string */
  std::string m_language;

  /*! Indicate the horizontal direction of the text */
  Boolean m_left_to_right;

  /*! Specifies the nominal height, in the local coordinate system */
  Float m_size;

  /*! Determines the line spacing between adjacent lines of text */
  Float m_spacing;

  /*! The font style: "PLAIN", "BOLD", "ITALIC", "BOLDITALIC" */
  std::string m_style;

  /*! Indicate the vertical direction of the text */
  Boolean m_top_to_bottom;

  /*! Indicates whether the font is accentuated */
  Boolean m_bold;

  /*! Indicates whether the font is italicized (slanted) */
  Boolean m_italic;

  /*! Indicates whether the font is antialiased */
  Boolean m_antialias;

  /*! Indicates that the font is dirty and should be recreated */
  Boolean m_dirty;

  /*! The font */
  Font * m_font;

  /*! Construct and initialize the font */
  void create_font();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

/*! Draw the node while traversing the scene graph */
inline Action::Trav_directive Font_style::draw(Draw_action *)
{ return Action::TRAV_CONT; }

SGAL_END_NAMESPACE

#endif
