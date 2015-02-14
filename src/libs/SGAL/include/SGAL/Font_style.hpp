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

#ifndef SGAL_FONT_STYLE_HPP
#define SGAL_FONT_STYLE_HPP

/*! \file
 * A node in the scene graph that specifies font style for text node.
 */

#include <vector>

#include <boost/unordered_map.hpp>

extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
}

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include "SGAL/basic.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Node.hpp"
#include "SGAL/Action.hpp"
#include "SGAL/Font_outliner.hpp"
#include "SGAL/Inexact_kernel.hpp"
#include "SGAL/Face_nesting_level.hpp"

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
    FAMILIES,
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

  enum Justify {JUSTIFY_BEGIN, JUSTIFY_FIRST, JUSTIFY_MIDDLE, JUSTIFY_END};

  enum Style {STYLE_PLAIN, STYLE_BOLD, STYLE_ITALIC, STYLE_BOLDITALIC};

  typedef Font_outliner::Outline                        Outline;
  typedef Font_outliner::Outlines                       Outlines;

  // Triangulation.
  typedef Inexact_kernel                                                Kernel;
  typedef CGAL::Triangulation_vertex_base_with_info_2<Uint, Kernel>     VB;
  typedef CGAL::Triangulation_face_base_with_info_2<Face_nesting_level, Kernel>
                                                                        FBI;
  typedef CGAL::Constrained_triangulation_face_base_2<Kernel, FBI>      FB;
  typedef CGAL::Triangulation_data_structure_2<VB, FB>                  TDS;
  typedef CGAL::No_intersection_tag                                     Itag;
  // typedef CGAL::Exact_predicates_tag                                    Itag;
  typedef CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, Itag>
    Triangulation;

  /*! Constructor from prototype.
   * \param proto (in) the prototype.
   */
  Font_style(Boolean proto = false);

  /*! Destructor. */
  virtual ~Font_style();

  /*! Construct the prototype.
   */
  static Font_style* prototype() { return new Font_style(true); }

  /*! Clone.
   */
  virtual Container* clone() { return new Font_style(); }

  /*! Initialize the container prototype.
   */
  virtual void init_prototype();

  /*! Delete the container prototype.
   */
  virtual void delete_prototype();

  /*! Obtain the container prototype.
   */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  String_array* families_handle(const Field_info*) { return &m_families; }
  Boolean* horizontal_handle(const Field_info*) { return &m_horizontal; }
  Uint_array* justify_handle(const Field_info*) { return &m_justify; }
  std::string* language_handle(const Field_info*) { return &m_language; }
  Boolean* l2r_handle(const Field_info*) { return &m_left_to_right; }
  Float* size_handle(const Field_info*) { return &m_size; }
  Float* spacing_handle(const Field_info*) { return &m_spacing; }
  std::string* style_handle(const Field_info*) { return &m_style; }
  Boolean* t2b_handle(const Field_info*) { return &m_top_to_bottom; }
  //@}

  /*! Set the attributes of this node.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the face.
   */
  void clean_face();

  /*! UTF-32 (or UCS-4) stands for Unicode Transformation Format 32 bits. It is
   * a protocol to encode Unicode characters that uses exactly 32 bits per
   * Unicode code point.
   */
  typedef boost::mpl::if_c<sizeof(wchar_t) == 4, wchar_t, Int32>::type Char32;

  /*! Compute the outlines.
   */
  void compute_outlines(Char32 c, Outlines& outlines);

  /*! Compute the glyph of a character.
   * \param c (in) the character.
   */
  const Triangulation& compute_glyph_geometry(Char32 c);

  /*! Draw the node while traversing the scene graph.
   */
  virtual Action::Trav_directive draw(Draw_action* draw_action);

  //virtual void Isect(Isect_action* isect_action);

  /*! Set the font families. A font family is a font rendering technique,
   * such as "SERIF", "SANS", or "TYPEWRITER", or a font name, such as
   * "Times New Roman".
   * \param family (in) the new families.
   */
  void set_families(const String_array& families);

  /*! Obtain the font families.
   * \return the font families.
   */
  const String_array& get_families() const;

  /*! Set the flag that indicates whether the text advances horizontally in
   * its major direction.
   * \param flag (in) indicates whether the text advances horizontally.
   */
  void set_horizontal(Boolean flag);

  /*! Determine whether the text advances horizontally in its major direction.
   * \return true if the text advances horizontally and false otherwise.
   */
  Boolean is_horizontal() const;

  /*! Set the alignment of the above text layout relative to the origin
   * of the object coordinate system.
   */
  void set_justify(const Uint_array& justify);

  /*! Obtain the alignment of the above text layout relative to the origin
   * of the object coordinate system.
   */
  const Uint_array& get_justify() const;

  /*! Set the context of the language for the text string.
   * \param labguage (in) the context of the language for the text string.
   */
  void set_language(const std::string& labguage);

  /*! Obtain the context of the language for the text string.
   * \return the context of the language for the text string.
   */
  const std::string& get_language() const;

  /*! Set the flag that indicates whether the text advances from left to right
   * either the major (characters within a single string) or minor (successive
   * strings) axes of layout depending on the horizontal flag.
   * \param flag (in) indicates whether the text advances from left to right.
   */
  void set_left_to_right(Boolean flag);

  /*! Obtain the flag that indicates whether the text advances from left to
   * right.
   * \return true if the text advances from left to right and false otherwise.
   */
  Boolean is_left_to_right() const;

  /*! Set the nominal height (in object space units) of glyphs rendered.
   * \param size (in) the nominal height (in object space units) of glyphs
   *        rendered.
   */
  void set_size(Float size);

  /*! Obtain the nominal height (in object space units) of glyphs rendered.
   * \return the nominal height (in object space units) of glyphs rendered.
   */
  Float get_size() const;

  /*! Set the line spacing between adjacent lines of text.
   * \param spacing (in) the line spacing between adjacent lines of text.
   */
  void set_spacing(Float spacing);

  /*! Obtain the line spacing between adjacent lines of text.
   * \return the line spacing between adjacent lines of text.
   */
  Float get_spacing() const;

  /*! Set the font style, e.g., bold.
   */
  void set_style(const std::string& style);

  /*! Obtain the font style.
   * \return the font style.
   */
  const std::string& get_style() const;

  /*! Set the flag that indicates whether the text advances from top to bottom
   * either the major (characters within a single string) or minor (successive
   * strings) axes of layout depending on the horizontal flag.
   * \param flag (in) indicates whether the text advances from top to bottom.
   */
  void set_top_to_bottom(Boolean flag);

  /*! Obtain the flag that indicates whether the text advances from top to
   * bottom.
   * \return the flag that indicates whether the text advances from top to
   * bottom.
   */
  Boolean is_top_to_bottom() const;

  /*! Draw one string.
   */
  void draw_string(Context* context, const std::string& text);

  /*! Obtain the width and height of the string. */
  void get_string_size(const std::string& str, Float& width, Float& height);


  /*! Obtain the font. */
  Font* get_font();

  /*! */
  virtual void on_field_change(const Field_info* field_info = nullptr);

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

protected:
  /*! Obtain the tag (type) of the container.
   */
  virtual const std::string& get_tag() const { return s_tag; }

  /*! Obtain the font file name.
   * \param file_name (out) the file name of the font.
   * \param face_index (out) the face index of the font.
   */
  void get_font_file_name(std::string& file_name, FT_Long& face_index);

  /*! The font name */
  std::string m_font_name;

  /*! A list of font families. A font family is a font rendering technique,
   * such as "SERIF", "SANS", or "TYPEWRITER", or a font name, such as
   * "Times New Roman"."SERIF", "SANS", or "TYPEWRITER".
   */
  String_array m_families;

  /*! Indicates whether the text advances horizontally in its major direction.
   */
  Boolean m_horizontal;

  /*! Determines alignment of the above text layout relative to the origin of
   * the object coordinate system. It is an MFString which can contain 2
   * values. The first value specifies alignment along the major axis and the
   * second value specifies alignment along the minor axis, as determined by
   * the horizontal field. A justify value of "" is equivalent to the default
   * value. If the second string, minor alignment, is not specified then it
   * defaults to the value "FIRST". Thus, justify values of "", "BEGIN", and
   * ["BEGIN" "FIRST"] are equivalent.
   */
  Uint_array m_justify;

  /*! Specify the context of the language for the text string. Due to the
   * multilingual nature of the ISO 10646-1:1993, the language field is needed
   * to provide a proper language attribute of the text string. The format is
   * based on the POSIX locale specification as well as the RFC 1766:
   * language[_territory]. The values for the language tag is based on the ISO
   * 639, i.e. zh for Chinese, jp for Japanese, sc for Swedish. The territory
   * tag is based on the ISO 3166 country code, i.e. TW is for Taiwan and CN
   * for China for the "zh" Chinese language tag. If the language field is
   * set to empty "", then local language bindings are used.
   */
  std::string m_language;

  /*! Indicate the horizontal direction of the text. */
  Boolean m_left_to_right;

  /*! Specifies the nominal height (in object space units) of glyphs rendered,
   * in the local coordinate system. The size is used to determine the spacing
   * of adjacent lines of text. All subsequent strings advance in either X or Y
   * by -( size * spacing).
   */
  Float m_size;

  /*! Indicates the line spacing between adjacent lines of text. */
  Float m_spacing;

  /*! The font style. Must be one of "PLAIN" (the default) for default plain
   * type; "BOLD" for boldface type; "ITALIC" for italic type; or "BOLDITALIC"
   * for bold and italic type. A style value of empty quotes, "", is identical
   * to "PLAIN".
   */
  std::string m_style;

  /*! Indicates the vertical direction of the text. */
  Boolean m_top_to_bottom;

  // Default values
  static const String_array s_def_families;
  static const Boolean s_def_horizontal;
  static const Uint_array s_def_justify;
  static const std::string s_def_language;
  static const Boolean s_def_left_to_right;
  static const Float s_def_size;
  static const Float s_def_spacing;
  static const std::string s_def_style;
  static const Boolean s_def_top_to_bottom;

private:
  /*! Process 'move to' instructions.
   * \param to (in) the target point of the 'move to'.
   * \param user (out) points to this.
   */
  static int move_to(FT_Vector* to, void* user);

  /*! Process 'line to' instructions.
   * \param to (in) the end point of the line.
   * \param user (out) points to this.
   */
  static int line_to(FT_Vector* to, void* user);

  /*! Process 'conic to' instructions.
   * A 'conic to' indicate a second-order Bézier arc in the outline.
   * \param control (in) the intermediate control point between the last
   *                position and the new target in to.
   * \param to (in) the target end point of the conic arc.
   * \param user (out) points to this.
   */
  static int conic_to(FT_Vector* control, FT_Vector* to, void* user);

  /*! Process 'cubic to' instructions.
   * A 'cubic to' indicates a third-order Bézier arc  in the outline.
   * \param control1 (in) the first Bézier control point.
   * \param control2 (in) the second Bézier control point.
   * \param to (in) the target end point of the cubic arc.
   * \param user (out) points to this.
   */
  static int cubic_to(FT_Vector* control1, FT_Vector* control2, FT_Vector* to,
                      void* user);

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The node prototype */
  static Container_proto* s_prototype;

  /*! Font family names. */
  static const Char* s_family_names[];

  /*! Font justify names. */
  static const Char* s_justify_names[];

  /*! Font style names. */
  static const Char* s_style_names[];

  typedef boost::unordered_map<Uint, Triangulation>     Triangulations;

  /*! A search structure of constrained triangulations. */
  Triangulations m_triangulations;

  /*! Indicates whether the font is accentuated. */
  Boolean m_bold;

  /*! Indicates whether the font is italicized (slanted). */
  Boolean m_italic;

  /*! Indicates whether the font is antialiased. */
  Boolean m_antialias;

  /*! Indicates that the font is dirty and should be recreated. */
  Boolean m_dirty;

  /*! The font. */
  Font* m_font;

  /*! The freetype root library object. */
  FT_Library m_ft_library;

  /*! The truetype driver. */
  FT_Driver m_ft_driver;

  /*! A handle to a given typographic face object. A face object models a
   * given typeface, in a given style.
   */
  FT_Face m_ft_face;

  /*! Indicates that the face is dirty. */
  Boolean m_dirty_face;

  /*! Construct and initialize the font. */
  void create_font();
};

#if defined(_MSC_VER)
#pragma warning( pop )
#endif

//! \brief obtains the font family.
inline const String_array& Font_style::get_families() const
{ return m_families; }

/*! \brief determines whether the text advances horizontally in its major
 * direction.
 */
inline Boolean Font_style::is_horizontal() const { return m_horizontal; }

/*! \brief obtains the alignment of the above text layout relative to the
 * origin of the object coordinate system.
 */
inline const Uint_array& Font_style::get_justify() const
{ return m_justify; }

//! \brief obtaints the context of the language for the text string.
inline const std::string& Font_style::get_language() const
{ return m_language; }

/*! \brief obtains the flag that indicates whether the text advances from left
 * to right.
 */
inline Boolean Font_style::is_left_to_right() const { return m_left_to_right; }

/*! \brief obtains the nominal height (in object space units) of glyphs
 * rendered.
 */
inline Float Font_style::get_size() const { return m_size; }

//! \brief obtains the line spacing between adjacent lines of text.
inline Float Font_style::get_spacing() const { return m_spacing; }

//! \brief obtains the font style.
inline const std::string& Font_style::get_style() const { return m_style; }

/*! \brief obtains the flag that indicates whether the text advances from top
 * to bottom.
 */
inline Boolean Font_style::is_top_to_bottom() const { return m_top_to_bottom; }

SGAL_END_NAMESPACE

#endif
