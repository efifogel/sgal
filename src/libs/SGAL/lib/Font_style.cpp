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

#include <stdexcept>

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/scope_exit.hpp>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MODULE_H
#include FT_STROKER_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
  //#include FT_SYNTHESIS_H
  //#include FT_LCD_FILTER_H
  //#include FT_CFF_DRIVER_H
  //#include FT_TRUETYPE_DRIVER_H
  //#include FT_INTERNAL_OBJECTS_H
  //#include FT_INTERNAL_DRIVER_H
}

#if defined(_WIN32)
#include <windows.h>
#include <strsafe.h>
#include <shlobj.h>
#undef interface
#else
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Draw_action.hpp"
#include "SGAL/Isect_action.hpp"
#include "SGAL/Cull_context.hpp"
#include "SGAL/Text.hpp"
#include "SGAL/Font_style.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Trace.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Texture_font.hpp"
#include "SGAL/Imagemagick_font.hpp"
#include "SGAL/Font_outliner.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Font_style::s_tag = "FontStyle";
Container_proto* Font_style::s_prototype(nullptr);

REGISTER_TO_FACTORY(Font_style, "Font_style");

// Default values
const String_array Font_style::s_def_families =
  boost::assign::list_of(static_cast<std::string>("SERIF"));
const Boolean Font_style::s_def_horizontal(true);
const Uint_array Font_style::s_def_justify =
  boost::assign::list_of(static_cast<Uint>(JUSTIFY_BEGIN))
  (static_cast<Uint>(JUSTIFY_FIRST));
const String_array Font_style::s_def_justify_str =
  boost::assign::list_of("BEGIN")("FIRST");
const std::string Font_style::s_def_language("");
const Boolean Font_style::s_def_left_to_right(true);
const Float Font_style::s_def_size(1.0f);
const Float Font_style::s_def_spacing(1.0f);
const std::string Font_style::s_def_style("PLAIN");
const Boolean Font_style::s_def_top_to_bottom(true);

const Char* Font_style::s_family_names[] =
  { "SERIF", "SANS", "TYPEWRITER" };

const Char* Font_style::s_justify_names[] =
  { "BEGIN", "FIRST", "MIDDLE", "END" };

const Char* Font_style::s_style_names[] =
  { "PLAIN", "BOLD", "ITALIC", "BOLDITALIC" };

#if !defined(_WIN32)
/*! Class thrown when fontconfig initialization fails. */
class SGAL_SGAL_DECL Fontconfig_initialization_error : public std::runtime_error
{
public:
  Fontconfig_initialization_error() :
    std::runtime_error("Failed to initialize fontconfig library!") {}

  ~Fontconfig_initialization_error() SGAL_NOTHROW {}
};

/*! Class thrown when fontconfig fails. */
class SGAL_SGAL_DECL Fontconfig_error : public std::runtime_error {
public:
  Fontconfig_error(FcResult result) :
    std::runtime_error(s_fc_result_message[result]) {}

  ~Fontconfig_error() SGAL_NOTHROW {}

private:
  static const Char* s_fc_result_message[];
};

const Char* Fontconfig_error::s_fc_result_message[] =
  {"match", "no match", "type mismatch", "no id" };
#endif

/*! Class thrown when FreeType initialization fails. */
class SGAL_SGAL_DECL FreeType_initialization_error : public std::runtime_error {
public:
  FreeType_initialization_error() :
    std::runtime_error("Failed to initialize FreeType library!") {}

  ~FreeType_initialization_error() SGAL_NOTHROW {}
};

/*! Class thrown when FreeType termination fails. */
class SGAL_SGAL_DECL FreeType_termination_error : public std::runtime_error {
public:
  FreeType_termination_error() :
    std::runtime_error("Failed to terminate FreeType library!") {}

  ~FreeType_termination_error() SGAL_NOTHROW {}
};

/*! Class thrown when FreeType fails. */
class SGAL_SGAL_DECL FreeType_error : public std::runtime_error {
public:
  FreeType_error(FT_Error error) :
    std::runtime_error("FreeType failed to  library!!") {}

  ~FreeType_error() SGAL_NOTHROW {}
};

//! \brief constructor
Font_style::Font_style(Boolean proto) :
  Node(proto),
  m_families(s_def_families),
  m_horizontal(s_def_horizontal),
  m_justify(s_def_justify),
  m_justify_str(s_def_justify_str),
  m_language(s_def_language),
  m_left_to_right(s_def_left_to_right),
  m_size(s_def_size),
  m_spacing(s_def_spacing),
  m_style(s_def_style),
  m_top_to_bottom(s_def_top_to_bottom),
  m_dirty(true),
  m_font(nullptr),
  m_dirty_face(true),
  m_face(nullptr)
{
  if (proto) return;

#if defined(_WIN32)
  auto success = FcInit();
  if (!success) throw Fontconfig_initialization_error();
#endif

  auto failure = FT_Init_FreeType(&m_ft_library);
  if (failure) throw FreeType_initialization_error();

  m_ft_driver = (FT_Driver) FT_Get_Module(m_ft_library, "truetype");
  if (!m_ft_driver) {
    std::cerr << "Failed to find the TrueType driver in FreeType 2!"
              << std::endl;
    throw;
  }
}

//! \brief Destructor
Font_style::~Font_style()
{
  if (m_font) {
    delete m_font;
    m_font = nullptr;
  }
  FT_Done_Face(m_face);
  auto failure = FT_Done_FreeType(m_ft_library);
  if (failure) throw FreeType_termination_error();

  m_dirty_face = true;
}

//! \brief draws the node while traversing the scene graph.
inline Action::Trav_directive Font_style::draw(Draw_action* /* draw_action */)
{ return Action::TRAV_CONT; }

//! \brief sets the flag that indicates whether the font is accentuated.
void Font_style::set_bold(Boolean b)
{
  m_bold = b;
  m_dirty = true;
}

/*! \brief set the flag that indicates whether the font is italicized
 * (slanted).
 */
void Font_style::set_italic(Boolean i)
{
  m_italic = i;
  m_dirty = true;
}

//! \brief sets the flag that indicates whether the font is antialiased font.
void Font_style::set_antialias(Boolean a)
{
  m_antialias = a;
  m_dirty = true;
}

//! \breif
void Font_style::on_field_change(const Field_info* /* field_info */)
{
  m_dirty = true;
  set_rendering_required();

  m_dirty_face = true;
  m_glyph_geometries.clear();
}

//! \breif initializes the prototype.
void Font_style::init_prototype()
{
  if (s_prototype != nullptr) return;
  s_prototype = new Container_proto(Node::get_prototype());

  // Add the object fields to the prototype
  auto exec_func = static_cast<Execution_function>(&Font_style::on_field_change);

  // family
  auto families_func =
    static_cast<String_array_handle_function>(&Font_style::families_handle);
  s_prototype->add_field_info(new MF_string(FAMILIES, "family",
                                            RULE_EXPOSED_FIELD,
                                            families_func,
                                            exec_func));

  // horizontal
  auto horizontal_func =
    static_cast<Boolean_handle_function>(&Font_style::horizontal_handle);
  s_prototype->add_field_info(new SF_bool(HORIZONTAL, "horizontal",
                                          RULE_EXPOSED_FIELD,
                                          horizontal_func, exec_func));

  // justify
  auto justify_func =
    static_cast<Uint_array_handle_function>(&Font_style::justify_handle);
  s_prototype->add_field_info(new MF_uint(JUSTIFY, "justifyCode",
                                          RULE_EXPOSED_FIELD,
                                          justify_func,
                                          exec_func));

  // justify string
  auto justify_str_func =
    static_cast<String_array_handle_function>(&Font_style::justify_str_handle);
  s_prototype->add_field_info(new MF_string(JUSTIFY_STR, "justify",
                                            RULE_EXPOSED_FIELD,
                                            justify_str_func,
                                            exec_func));

  // language
  auto language_func =
    static_cast<String_handle_function>(&Font_style::language_handle);
  s_prototype->add_field_info(new SF_string(LANGUAGE, "language",
                                            RULE_EXPOSED_FIELD,
                                            language_func, exec_func));

  // leftToRight
  auto l2r_func = static_cast<Boolean_handle_function>(&Font_style::l2r_handle);
  s_prototype->add_field_info(new SF_bool(LEFT_TO_RIGHT, "leftToRight",
                                          RULE_EXPOSED_FIELD,
                                          l2r_func, exec_func));

  // size
  auto size_func = static_cast<Float_handle_function>(&Font_style::size_handle);
  s_prototype->add_field_info(new SF_float(SIZE, "size",
                                           RULE_EXPOSED_FIELD,
                                           size_func,
                                           exec_func));

  // spacing
  auto spacing_func =
    static_cast<Float_handle_function>(&Font_style::spacing_handle);
  s_prototype->add_field_info(new SF_float(SPACING, "spacing",
                                           RULE_EXPOSED_FIELD,
                                           spacing_func,
                                           exec_func));

  // style
  auto style_func =
    reinterpret_cast<Uint_handle_function>(&Font_style::style_handle);
  s_prototype->add_field_info(new SF_uint(STYLE, "style",
                                          RULE_EXPOSED_FIELD,
                                          style_func,
                                          exec_func));

  // topToBottom
  auto t2b_func = static_cast<Boolean_handle_function>(&Font_style::t2b_handle);
  s_prototype->add_field_info(new SF_bool(TOP_TO_BOTTOM, "topToBottom",
                                          RULE_EXPOSED_FIELD,
                                          t2b_func, exec_func));
}

//! \brief deletes the prototype.
void Font_style::delete_prototype()
{
  delete s_prototype;
  s_prototype = nullptr;
}

//! \brief obtains the prototype.
Container_proto* Font_style::get_prototype()
{
  if (!s_prototype)  Font_style::init_prototype();
  return s_prototype;
}

//! \brief sets the attributes of this object.
void Font_style::set_attributes(Element* elem)
{
  Node::set_attributes(elem);

  for (auto ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const auto& name = elem->get_name(ai);
    const auto& value = elem->get_value(ai);
    if (name == "family") {
      m_families.clear();
      auto start = value.find_first_of('\"');
      while (start != std::string::npos) {
        ++start;
        auto end = value.find_first_of('\"', start);
        if (end == std::string::npos) break;
        m_families.push_back(value.substr(start, end-start));
        ++end;
        if (end == value.size()) break;
        start = value.find_first_of('\"', end);
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "horizontal") {
      set_horizontal(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "justify") {
      m_justify_str.clear();
      auto start = value.find_first_of('\"');
      while (start != std::string::npos) {
        ++start;
        auto end = value.find_first_of('\"', start);
        if (end == std::string::npos) break;
        m_justify_str.push_back(value.substr(start, end-start));
        ++end;
        if (end == value.size()) break;
        start = value.find_first_of('\"', end);
      }
      // Reflect in the code
      m_justify.clear();
      for (auto it = m_justify_str.begin(); it != m_justify_str.end(); ++it) {
        if (*it == "BEGIN") m_justify.push_back(JUSTIFY_BEGIN);
        else if (*it == "FIRST") m_justify.push_back(JUSTIFY_FIRST);
        else if (*it == "MIDDLE") m_justify.push_back(JUSTIFY_MIDDLE);
        else if (*it == "END") m_justify.push_back(JUSTIFY_END);
        else SGAL_error(); //! \todo throw---user error
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "justifyCode") {
      Uint num_values = get_num_tokens(value);
      m_justify.resize(num_values);
      std::istringstream svalue(value, std::istringstream::in);
      for (Uint i = 0; i < num_values; ++i) svalue >> m_justify[i];
      // Reflect in the strings
      m_justify_str.clear();
      for (auto it = m_justify.begin(); it != m_justify.end(); ++it) {
        switch (m_justify[*it]) {
         case JUSTIFY_BEGIN: m_justify_str.push_back("BEGIN"); break;
         case JUSTIFY_FIRST: m_justify_str.push_back("FIRST"); break;
         case JUSTIFY_MIDDLE: m_justify_str.push_back("MIDDLE"); break;
         case JUSTIFY_END: m_justify_str.push_back("END"); break;
         default: SGAL_error(); break;
        }
      }
      elem->mark_delete(ai);
      continue;
    }
    if (name == "language") {
      set_language(value);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "leftToRight")  {
      set_left_to_right(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "size") {
      set_size(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "spacing") {
      set_spacing(boost::lexical_cast<Float>(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "style") {
      set_style(strip_double_quotes(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "topToBottom") {
      set_top_to_bottom(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
Attribute_list Font_style::get_attributes()
{
  Attribute_list attribs;
  Attribue attrib;
  Vector3f vec;
  Rotation rot;

  attribs = Node::get_attributes();

  if (!m_family.empty()) {
    attrib.first = "family";
    attrib.second = m_family;
    attribs.push_back(attrib);
  }
  if (!m_style.empty() && !m_style.equal("PLAIN")) {
    attrib.first = "style";
    attrib.second = m_style;
    attribs.push_back(attrib);
  }
  if (!m_horizontal) {
    attrib.first = "horizontal";
    attrib.second = (m_horizontal)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }
  if (!m_justify.empty() && !m_justify.equal("PLAIN")) {
    attrib.first = "justify";
    attrib.second = m_justify;
    attribs.push_back(attrib);
  }
  if (!m_language.empty()) {
    attrib.first = "language";
    attrib.second = m_language;
    attribs.push_back(attrib);
  }
  if (!m_left_to_right) {
    attrib.first = "leftToRight";
    attrib.second = (m_left_to_right)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }
  if (m_spacing != 1) {
    attrib.first = "spacing";
    attrib.second = String::fromFloat(m_spacing,2);
    attribs.push_back(attrib);
  }
  if (m_size != 1) {
    attrib.first = "size";
    attrib.second = String::fromFloat(m_size,2);
    attribs.push_back(attrib);
  }
  if (!m_top2Bottom) {
    attrib.first = "topToBottom";
    attrib.second = (m_top2Bottom)?"TRUE":"FALSE";
    attribs.push_back(attrib);
  }

  return attribs;
}
#endif

//! \brief cleans the face.
void Font_style::clean_face()
{
  std::string file_name;
  FT_Long face_index;
  get_font_file_name(file_name, face_index);

  if (file_name.empty()) return;
  SGAL_TRACE_MSG(Trace::FONT, "Font file name: " + file_name + "\n");

  auto err = FT_New_Face(m_ft_library, file_name.c_str(), face_index, &m_face);
  if (err) throw FreeType_error(err);

  SGAL_assertion(m_face->driver == m_ft_driver);     // check format

  m_dirty_face = false;
}

//! \brief obtains the font.
Font* Font_style::get_font()
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  return m_font;
}

//! obtains constructs and initializes the font.
void Font_style::create_font()
{
  m_font = new Imagemagick_font(m_font_name, m_antialias,
                                m_left_to_right, m_top_to_bottom);
  m_font->init();
  m_dirty = false;
}

//! Draw one string.
void Font_style::draw_string(Context* context, const std::string& str)
{
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  m_font->draw_string(context, str, m_size);
}

//! Obtain the width and height of the string.
void Font_style::get_string_size(const std::string& str,
                                 Float& width, Float& height)
{
  Uint w, h;
  if (!m_font || m_dirty) {
    if (m_font) delete m_font;
    create_font();
  }
  m_font->get_string_size(str, w, h);
  height = m_size;
  width = height * w / h;
}

//! \brief sets the font family.
void Font_style::set_families(const String_array& families)
{ m_families = families; }

/*! \brief sets the flag that indicates whether the text advances horizontally
 * in its major direction.
 */
void Font_style::set_horizontal(Boolean flag) { m_horizontal = flag; }

/*! \brief sets the alignment of the above text layout relative to the origin
 * of the object coordinate system.
 */
void Font_style::set_justify(const Uint_array& justify)
{ m_justify = justify; }

//! \brief sets the context of the language for the text string.
void Font_style::set_language(const std::string& labguage)
{ m_language = labguage; }

/*! \brief sets the flag that indicates whether the text advances from left to
 * right.
 */
void Font_style::set_left_to_right(Boolean flag) { m_left_to_right = flag; }

//! \brief sets the nominal height (in object space units) of glyphs rendered.
void Font_style::set_size(Float size) { m_size = size; }

//! \brief sets the line spacing between adjacent lines of text.
void Font_style::set_spacing(Float spacing) { m_spacing = spacing; }

//! \brief sets the font style, e.g., bold.
void Font_style::set_style(const std::string& style) {m_style = style; }

/*! \brief sets the flag that indicates whether the text advances from top to
 * bottom.
 */
void Font_style::set_top_to_bottom(Boolean flag) { m_top_to_bottom = flag; }

//! \brief processes 'move to' instructions.
int Font_style::move_to(FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b0(static_cast<Float>(to->x), static_cast<Float>(to->y));
  font_outliner->move_to(b0);
  return 0;
}

//! \brief processes 'line to' instructions.
int Font_style::line_to(FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(static_cast<Float>(to->x), static_cast<Float>(to->y));
  font_outliner->line_to(b1);
  return 0;
}

//! processes 'conic to' instructions.
int Font_style::conic_to(FT_Vector* control, FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(static_cast<Float>(control->x), static_cast<Float>(control->y));
  Vector2f b2(static_cast<Float>(to->x), static_cast<Float>(to->y));
  font_outliner->conic_to(b1, b2);
  return 0;
}

//! processes 'cubic to' instructions.
int Font_style::cubic_to(FT_Vector* control1, FT_Vector* control2,
                         FT_Vector* to, void* user)
{
  auto font_outliner = reinterpret_cast<Font_outliner*>(user);
  SGAL_assertion(font_outliner);
  Vector2f b1(static_cast<Float>(control1->x), static_cast<Float>(control1->y));
  Vector2f b2(static_cast<Float>(control2->x), static_cast<Float>(control2->y));
  Vector2f b3(static_cast<Float>(to->x), static_cast<Float>(to->y));
  font_outliner->cubic_to(b1, b2, b3);
  return 0;
}

//! \brief computes the outlines.
void Font_style::compute_outlines(Char32 c, Outlines& outlines)
{
  // FT_Get_Glyph(face->glyph, &glyph);
  FT_Outline outline = m_face->glyph->outline;

  if (m_face->glyph->format != ft_glyph_format_outline) {
    std::cerr << "Not an outline font!" << std::endl;
  }

  FT_Outline_Funcs funcs;
  funcs.shift = 0;
  funcs.delta = 0;
  funcs.move_to = (FT_Outline_MoveTo_Func)&move_to;
  funcs.line_to = (FT_Outline_LineTo_Func)&line_to;
  funcs.conic_to = (FT_Outline_ConicTo_Func)&conic_to;
  funcs.cubic_to = (FT_Outline_CubicTo_Func)&cubic_to;
  // trace outline of the glyph
  Font_outliner outliner(outlines);
  auto err = FT_Outline_Decompose(&outline, &funcs, &outliner);
  if (err) {
    std::cerr << "Failed to decompose!" << std::endl;
    throw;
  }

  // Remove last point from every outline, as it is a replicate of the first
  for (auto oit = outlines.begin(); oit != outlines.end(); ++oit)
    oit->pop_back();
}

//! \brief computes the glyph of a character.
const Glyph_geometry& Font_style::compute_glyph_geometry(Char32 c)
{
  if (m_dirty_face) clean_face();

  Uint glyph_index =
#if defined(_WIN32)
    FT_Get_Char_Index(m_face, c);
#else
    FcFreeTypeCharIndex(m_face, c);
#endif

  // Load glyph
  auto err = FT_Load_Char(m_face, c, FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE);
  if (err) throw(err);

  Float glyph_scale =
    (m_face->bbox.yMax > 0.0) ? get_size() / m_face->bbox.yMax : get_size();

  Outlines outlines;
  compute_outlines(c, outlines);

  // Compute the triangulations
  auto it = m_glyph_geometries.find(glyph_index);
  if (it != m_glyph_geometries.end()) return it->second;

  auto& glyph_geom = m_glyph_geometries[glyph_index];
  Uint k(0);
  for (auto oit = outlines.begin(); oit != outlines.end(); ++oit) {
    const auto& outline = *oit;
    auto& tri = glyph_geom.get_triangulation();
    k = glyph_geom.construct_triangulation(outline.begin(), outline.end(), k);
  }
  // Mark facets that are inside the domain
  glyph_geom.mark_domains();
  glyph_geom.set_scale(glyph_scale);
  Float x = m_face->glyph->metrics.horiAdvance;
  Float y = m_face->glyph->metrics.vertAdvance;
  glyph_geom.set_advance(x, y);
  return glyph_geom;
}

//! \brief obtains the font file name.
void Font_style::get_font_file_name(std::string& file_name, FT_Long& face_index)
{
#if defined(_WIN32)
  LOGFONT lf;
  lf.lfHeight = 0;
  lf.lfWidth = 0;
  lf.lfEscapement = 0;
  lf.lfOrientation = 0;
  lf.lfWeight = FW_MEDIUM;
  lf.lfItalic = FALSE;
  lf.lfUnderline = FALSE;
  lf.lfStrikeOut = FALSE;
  lf.lfCharSet = DEFAULT_CHARSET;
  lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
  lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lf.lfQuality = DEFAULT_QUALITY;
  lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;

  HDC hdc = CreateCompatibleDC(0);
  BOOST_SCOPE_EXIT((hdc)) {
    DeleteDC(hdc);
  } BOOST_SCOPE_EXIT_END
  HFONT hfont = CreateFontIndirect(&lf);
  SelectObject(hdc, hfont);
  TCHAR face_name[256] = {};
  GetTextFace(hdc, sizeof face_name / sizeof (TCHAR), face_name);
  const int face_name_len = lstrlen(face_name);

  // Get the fonts folder.
  TCHAR fonts_path[MAX_PATH];
  HRESULT status =
    SHGetFolderPath(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, fonts_path);
  if (FAILED(status)) { /* bail */ }

  // Enumerate the fonts in the registry and pick one that matches.
  HKEY fonts_key;
  LONG result =
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                 "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
                 0,
                 KEY_READ,
                 &fonts_key);
  if (result != ERROR_SUCCESS) { /* bail */ }
  BOOST_SCOPE_EXIT((fonts_key)) {
    RegCloseKey(fonts_key);
  } BOOST_SCOPE_EXIT_END

  DWORD max_value_name_len, max_value_len;
  result = RegQueryInfoKey(fonts_key,
                           NULL,  // lpClass
                           NULL,  // lpcClass
                           NULL,  // lpReserved
                           NULL,  // lpcSubKeys
                           NULL,  // lpcMaxSubKeyLen
                           NULL,  // lpcMaxClassLen
                           NULL,  // lpcValues
                           &max_value_name_len,
                           &max_value_len,
                           NULL,  // lpcbSecurityDescriptor
                           NULL); // lpftLastWriteTime

  DWORD index = 0;
  vector<TCHAR> value_name(max_value_name_len + 1);
  DWORD type;
  vector<BYTE> data(max_value_len);
  TCHAR font_path[MAX_PATH] = {};
  result = ERROR_SUCCESS;
  while (result != ERROR_NO_MORE_ITEMS) {
    DWORD data_length = DWORD(data.size());
    DWORD value_name_length = DWORD(value_name.size());
    result = RegEnumValue(fonts_key,
                          index,
                          &value_name.front(),
                          &value_name_length,
                          NULL,
                          &type,
                          &data.front(),
                          &data_length);
    if (result == ERROR_MORE_DATA) {
      data.resize(data_length);
      continue;
    }
    if (result == ERROR_SUCCESS) {
      if (DWORD(face_name_len + 1) <= value_name_length &&
          std::equal(face_name, face_name + face_name_len, value_name.begin()))
      {
        HRESULT strcat_result = StringCchCat(font_path, MAX_PATH, fonts_path);
        assert(SUCCEEDED(strcat_result));
        strcat_result = StringCchCat(font_path, MAX_PATH, "\\");
        assert(SUCCEEDED(strcat_result));
        strcat_result =
          StringCchCat(font_path, MAX_PATH,
                       reinterpret_cast<STRSAFE_LPCSTR>(&data.front()));
        assert(SUCCEEDED(strcat_result));
        break;
      }
      ++index;
    }
  }

  const size_t font_path_len = lstrlen(font_path);
  assert(font_path_len != 0);
  file_name.assign(font_path, font_path + font_path_len + 1);
  face_index = 0;

# else
  std::string font_name;

  // Set the attributes
  auto add_family = [&](std::vector<std::string>::const_iterator it) {
    const auto& family = *it;
    if (family == "SERIF") font_name += "serif";
    else if (family == "SANS") font_name += "sans";
    else if (family == "TYPEWRITER") font_name += "monospace";
    else font_name += family;
  };
  auto it = m_families.begin();
  add_family(it);
  for (++it; it != m_families.end(); ++it) {
    font_name += ",";
    add_family(it);
  }
  if (m_style.find("BOLD") != std::string::npos) font_name += ":bold";
  if (m_style.find("ITALIC") != std::string::npos) font_name += ":italic";
  SGAL_TRACE_MSG(Trace::FONT, "Font name: " + font_name + "\n");

  FcPattern* initial_pat = FcNameParse((FcChar8*)(font_name.c_str()));
  if (!initial_pat) { throw std::bad_alloc(); }
  BOOST_SCOPE_EXIT((initial_pat)) {
    FcPatternDestroy(initial_pat);
  } BOOST_SCOPE_EXIT_END

  FcPatternAddBool(initial_pat, FC_OUTLINE, FcTrue);
  FcPatternAddBool(initial_pat, FC_SCALABLE, FcTrue);
  FcPatternAddString(initial_pat, FC_FONTFORMAT, (FcChar8*)("TrueType"));

  // Set the language.
  if (!m_language.empty())
    FcPatternAddString(initial_pat, FC_LANG, (FcChar8*)(m_language.c_str()));
  FcPatternAddDouble(initial_pat, FC_PIXEL_SIZE, (double)(get_size()));

  FcConfigSubstitute(0, initial_pat, FcMatchPattern);
  FcDefaultSubstitute(initial_pat);

  FcResult result = FcResultMatch;
  FcPattern* const matched_pat = FcFontMatch(0, initial_pat, &result);
  if (result != FcResultMatch) { throw Fontconfig_error(result); }
  assert(matched_pat);
  BOOST_SCOPE_EXIT((matched_pat)) {
    FcPatternDestroy(matched_pat);
  } BOOST_SCOPE_EXIT_END

  FcChar8* file_name_str = 0;
  result = FcPatternGetString(matched_pat, FC_FILE, 0, &file_name_str);
  if (result != FcResultMatch) { throw Fontconfig_error(result); }
  file_name.assign((Char*)file_name_str);

  int face_index_int = 0;
  result = FcPatternGetInteger(matched_pat, FC_INDEX, 0, &face_index_int);
  if (result != FcResultMatch) { throw Fontconfig_error(result); }
  face_index = FT_Long(face_index_int);
# endif
}

SGAL_END_NAMESPACE
