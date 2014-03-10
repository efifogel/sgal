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

#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "SGAL/Scene_graph_int.h"
#include "SGAL/Text_texture.h"
#include "SGAL/Element.h"
#include "SGAL/Image.h"
#include "SGAL/Texture.hpp"
#include "SGAL/Container_proto.h"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"

SGAL_BEGIN_NAMESPACE

std::string Transform::s_tag = "TextTexture";
Container_proto* Text_texture::s_prototype(NULL);

REGISTER_TO_FACTORY(Text_texture, "Text_texture");

/*! Constructor */
Text_texture::Text_texture(Boolean proto) :
  Container(proto),
  m_font_size(8),
  m_font("fixedsys"),
  m_bold(false),
  m_italic(false),
  m_antialias(false),
  m_update_texture(true),
  m_original_image(0),
  m_parent(0)
{ m_color.set(1, 1, 1); }

/*! Destructor */
Text_texture::~Text_texture() { delete m_original_image; }

/*! \brief */
void Text_texture::draw(Context* ctx)
{
  if (m_update_texture) {
    void* bmpData = 0;

    Image_data& img = m_parent->get_image_data();
    int width = atoi(img.get_value(Image_data::WIDTH));
    int height = atoi(img.get_value(Image_data::HEIGHT));
    if (width == 0 || height == 0 || img.get_pixels() == 0) return;
    //if (m_parent->get_componentCount() != 3) return;

    const Image* current_image = m_parent->get_image();

    // if the new created image has a different size, we recreate the
    // text texture to adjust the size
    if (m_original_image &&
      ((m_original_image->get_width() != width) ||
      (m_original_image->GetHeight() != height)))
    {
      m_original_image->delete_pixels();
      //! \todo DELETE_OBJECT(m_original_image);
    }
    if (!m_original_image) {
      m_original_image = new Image;
      m_original_image->set_width(current_image->get_width());
      m_original_image->set_height(current_image->get_height());
      m_original_image->set_format(current_image->get_format());
      m_original_image->allocate_memory();
      ::memcpy(m_original_image->get_pixels(),
               current_image->get_pixels(), current_image->get_size());
    }

    HWND hwnd = get_desktop_window();
    HDC dc = get_dC(hwnd);
    HDC hdc = CreateCompatibleDC(get_dC(hwnd));
    ReleaseDC(hwnd,dc);

    /////////////////////////////////////////////////////////////////////////////
    // Create bitmap,
    /////////////////////////////////////////////////////////////////////////////
    // Initialize bitmap info structure
    BITMAPINFO bi;
    bi.bmiHeader.biSize   = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth  =  width;
    bi.bmiHeader.biHeight = height; // bitmap is a top-down DIB (origin is upper-left corner)
    bi.bmiHeader.biPlanes =  1;
    bi.bmiHeader.biBitCount    = 24;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage   = 0;
    bi.bmiHeader.biXPelsPerMeter = width;
    bi.bmiHeader.biYPelsPerMeter = height;
    bi.bmiHeader.biClrUsed       = 0;
    bi.bmiHeader.biClrImportant  = 0;

    // Create GDI bitmap
    HBITMAP hBmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &bmpData, 0, 0);
    HBITMAP oldBmp = (HBITMAP)SelectObject(hdc, hBmp);

    // Initialize all bitmap pixels to black
    BitBlt(hdc,0,0,width,height,hdc,0,0,BLACKNESS);


    int bmpSize = m_original_image->get_size();

    if (m_parent->get_component_count() == 3) {

      memcpy(bmpData,m_original_image->get_pixels(),bmpSize);
      DrawText(hdc,m_text,width,height);
      memcpy(current_image->get_pixels(),bmpData,bmpSize);

    } else if (m_parent->get_componentCount() == 4) {

      // handle RGB
      char* originalPixels = (char *)m_original_image->get_pixels();
      char* currentPixels = (char *)current_image->get_pixels();
      char* bmpDataC = (char *)bmpData;
      int i = 0;

      for (i = 0; i < bmpSize/4; ++i) {
        memcpy(bmpDataC+i*3, originalPixels+i*4, 3);
      }
      DrawText(hdc,m_text,width,height);
      for (i = 0; i < bmpSize/4; ++i) {
        memcpy(currentPixels+i*4, bmpDataC+i*3, 3);
      }

      // handle Alpha - create a mask according to the text
      BitBlt(hdc,0,0,width,height,hdc,0,0,BLACKNESS);
      Vector3f oldColor = m_color;
      m_color[0] = 1.0; m_color[1] = m_color[2] = 0.0;
      DrawText(hdc,m_text,width,height);
      m_color = oldColor;
      for (i = 0 ; i < bmpSize/4 ; ++i) {
        if (bmpDataC[i*3] != 0) currentPixels[i*4+3] = (char)255;
        else currentPixels[i*4+3] = originalPixels[i*4+3];
      }
    } else {
      ASSERT(0);
    }

    // Release Bitmap.
    // Destroy current bitmap.
    SelectObject(hdc, oldBmp);
    DeleteObject(hBmp);
    DeleteDC(hdc);

    m_update_texture = false;
  }
}

Boolean Text_texture::is_equal(const Text_texture* t) const
{
  if (m_text != t->m_text) return false;
  if (m_color != t->m_color) return false;
  if (m_font != t->m_font) return false;
  if (m_font_size != t->m_font_size) return false;
  if (m_bold != t->m_bold) return false;
  if (m_italic != t->m_italic) return false;
  return true;
}

/*!
 */
/*! \todo
void Text_texture::Draw_text(HDC hdc,const String &text,int width,int height)
{
  // Get number of pixels per inch
  int pixPerInch = get_deviceCaps(hdc, LOGPIXELSY);
  // height is in pixels, size in points (1 point = 1/72 inch)
  int   fontHeight = -MulDiv(m_font_size, pixPerInch, 72);

  int   weight  = m_bold ? FW_BOLD : FW_NORMAL;
  DWORD italic  = m_italic ? TRUE : FALSE;
  DWORD quality = m_antialias ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;

  HFONT hfont = Create_font(fontHeight, 0, 0, 0, weight, italic, FALSE,FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS, quality /* quality */,
                            DEFAULT_PITCH | FF_DONTCARE, m_font);

  if (hfont) {
    HFONT oldFont = (HFONT)SelectObject(hdc,hfont);

   // Set text appearance properties
    set_bkMode(hdc,TRANSPARENT);
    set_bkColor(hdc, RGB(0,0,0));
    SetTextColor(hdc,RGB(255*m_color[2],255*m_color[1],255*m_color[0]));

    RECT r;
    r.top = 0;
    r.left = 0;
    r.right = width;
    r.bottom = height;
    ::DrawText(hdc, text, text.length(), &r,
               DT_CENTER | DT_WORDBREAK | DT_CALCRECT);
    int rch = r.bottom - r.top;

    r.left = 0;
    r.right = width;
    r.top = height/2 - rch/2;
    r.bottom = height/2 + rch/2;
    ::DrawText(hdc, text, text.length(), &r, DT_CENTER|DT_WORDBREAK);

    SelectObject(hdc, oldFont);
    DeleteObject(hfont);
  }
}
*/

/*! \brief initializes the node prototype. */
void Text_texture::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto();

  // Add the field-info records to the prototype:
  Execution_function exec_func =
    static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  String_handle_function text_func =
    static_cast<String_handle_function>(&Color_background::text_handle);
  s_prototype->add_field_info(new SF_string(TEXT, "text",
                                            RULE_EXPOSED_FIELD,
                                            text_func,
                                            exec_func));

  exec_func =
    static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  Vector3f_handle_function color_func =
    static_cast<Vector3f_handle_function>(&Color_background::color_handle);
  s_prototype->add_field_info(new SF_vector3f(COLOR, "color",
                                              RULE_EXPOSED_FIELD,
                                              color_func,
                                              exec_func));

  exec_func = static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  String_handle_function font_func =
    static_cast<String_handle_function>(&Color_background::font_handle);
  s_prototype->add_field_info(new SF_string(FONT, "font",
                                            RULE_EXPOSED_FIELD,
                                            font_func,
                                            exec_func));

  exec_func = static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  Uint_handle_function font_size_func =
    static_cast<Uint_handle_function>(&Color_background::font_size_handle);
  s_prototype->add_field_info(new SF_uint(FONTSIZE, "fontSize",
                                          RULE_EXPOSED_FIELD,
                                          font_size_func,
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  Boolean_handle_function bold_func =
    static_cast<Boolean_handle_function>(&Color_background::bold_handle);
  s_prototype->add_field_info(new SF_bool(BOLD, "bold",
                                          RULE_EXPOSED_FIELD,
                                          bold_func, exec_func));

  exec_func = static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  Boolean_handle_function italic_func =
    static_cast<Boolean_handle_function>(&Color_background::italic_handle);
  s_prototype->add_field_info(new SF_bool(ITALIC, "italic", italic_func,
                                          exec_func));

  exec_func = static_cast<Execution_function>(&Text_texture::OnFieldChanged);
  Boolean_handle_function antialias_func =
    static_cast<Boolean_handle_function>(&Color_background::antialias_handle);
  s_prototype->add_field_info(new SF_bool(ANTIALIAS, "antialias",
                                          RULE_EXPOSED_FIELD,
                                          antialias_func, exec_func));
}

/*! \brief */
void Text_texture::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief */
Container_proto* Text_texture::get_prototype()
{
  if (!s_prototype) init_prototype();
  return s_prototype;
}
#endif

/*! \brief */
void Text_texture::OnFieldChanged(Field_info* field_info)
{
  m_parent->set_dirty_texture();
  m_update_texture = true;
  m_execution_coordinator->set_rendering_required();
}

/*! \brief sets the attributes of this. */
void Text_texture::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter          Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const Attribute* attr = *ai;
    const std::string* name = elem->get_name(ai);
    const std::string* value = elem->get_value(ai);
    bool do_delete = false;
    if (name == "text") {
      m_text = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "color") {
      Vector3f col(value);
      m_color = col;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "font") {
      m_font = value;
      elem->mark_delete(ai);
      continue;
    }
    if (name == "fontSize") {
      m_font_size = atoi(value.c_str());
      elem->mark_delete(ai);
      continue;
    }
    if (name == "bold") {
      m_bold = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "italic") {
      m_italic = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "antialias") {
      m_antialias = compare_to_true(value);
      elem->mark_delete(ai);
      continue;
    }
  }
  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! \brief */
Attribute_list Text_texture::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;

    attribs = Container::get_attributes();

  if (!m_text.empty()) {
    attrib.first = "text";
    attrib.second = m_text;
    attribs.push_back(attrib);
  }
  if (m_color[0] != 1 || m_color[1] != 1 || m_color[2] != 1) {
    attrib.first = "color";
    attrib.second = m_color.get_text();
    attribs.push_back(attrib);
  }
  if (m_font != "fixedsys") {
    attrib.first = "font";
    attrib.second = m_font;
    attribs.push_back(attrib);
  }
  if (m_font_size != 8) {
    attrib.first = "font_size";
    attrib.second = String::fromInteger(m_font_size);
    attribs.push_back(attrib);
  }
  if (m_bold) {
    attrib.first = "bold";
    attrib.second = "TRUE";
    attribs.push_back(attrib);
  }
  if (m_italic) {
    attrib.first = "italic";
    attrib.second = "TRUE";
    attribs.push_back(attrib);
  }
  if (m_antialias) {
    attrib.first = "antialias";
    attrib.second = "TRUE";
    attribs.push_back(attrib);
  }

  return attribs;
}

/*! */
void Text_texture::add_to_scene(Scene_graph* sg)
{
  Container::add_to_scene(sg, parent);
  Texture* t = dynamic_cast<Texture *>(parent);
  if (t) {
    t->SetText(this);
    m_parent = t;
  }
  sg->AddContainer(this);
}
#endif

SGAL_END_NAMESPACE
