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

/*!
 * A class that implement texture that can be placed on a texture
 */

#ifndef SGAL_TEXT_TEXTURE_HPP
#define SGAL_TEXT_TEXTURE_HPP

#if (defined _MSC_VER)
#include <windows.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Vector3f.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Image;
class Texture;
class Container_proto;

class SGAL_SGAL_DECL Text_texture : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    TEXT,
    COLOR,
    FONT,
    FONTSIZE,
    BOLD,
    ITALIC,
    ANTIALIAS,
    LAST
  };

  /*! Constructor */
  Text_texture(Boolean proto = false);

  /*! Destructor */
  virtual ~Text_texture();

  /* Construct the prototype. */
  static Text_texture* prototype() { return new Text_texture(true); }

  /*! Clone. */
  virtual Container* clone() { return new Text_texture(); }

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  //! \todo virtual Attribute_list get_attributes();

  virtual void draw(Context* ctx);

  void clean() { m_update_texture = true; }

  Boolean is_equal(const Text_texture* t) const;

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  virtual void delete_prototype();

  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  std::string* _handle(Field_info*) { return &m_text; }
  Vector3f* _handle(Field_info*) { return &m_color; }
  std::string* _handle(Field_info*) { return &m_font; }
  Int* _handle(Field_info*) { return &m_font_size; }
  Boolean* _handle(Field_info*) { return &m_bold; }
  Boolean* _handle(Field_info*) { return &m_italic; }
  Boolean* _handle(Field_info*) { return &m_antialias; }
  //@}

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const { return s_tag; }

private:
  /*! The tag that identifies this container type. */
  static std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  std::string m_text;
  Vector3f m_color;
  std::string m_font;
  Uint m_font_size;
  Boolean m_bold;
  Boolean m_italic;
  Boolean m_antialias;

  bool m_update_texture;
  Image* m_original_image;
  Texture* m_parent;

  void on_field_changed(Field_info* field_info = NULL);
  //! \todo void draw_text(HDC hdc, const std::string& text, int width, int height);
};

SGAL_END_NAMESPACE

#endif
