// Copyright (c) 2015 Israel.
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

#ifndef SGAL_TEXT_3D_HPP
#define SGAL_TEXT_3D_HPP

#include <string>
#include <boost/shared_ptr.hpp>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Array_types.hpp"
#include "SGAL/Indexed_face_set.hpp"

SGAL_BEGIN_NAMESPACE

class Container_proto;
class Font_style;
class Element;

#if (defined _MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

/*! \class Text_3d Text_3d.hpp Text_3d is a geometry container that represents a
 * 3D text string object that is positioned with its middle vertical plane in
 * the Z=0 plane of the local coordinate system, based on values defined in the
 * fontStyle field. The Text3D nodes may contain multiple text strings using the
 * UTF-8 encoding as specified by ISO 10646-1:1993. The text strings are stored
 * in the order in which the text mode characters are to be produced as defined
 * by the parameters in the FontStyle node. The fields of the Text3D node, with
 * the exception of depth are analogous to the ones for the Text node.
 */

class SGAL_SGAL_DECL Text_3d : public Indexed_face_set {
public:
  enum {
    FIRST = Indexed_face_set::LAST - 1,
    STRINGS,
    FONT_STYLE,
    LENGTHS,
    MAX_EXTENT,
    DEPTH,
    LAST
  };

  typedef boost::shared_ptr<Font_style>         Shared_font_style;

  /*! Constructor.
   * \param proto (in) determines whether to construct a prototype.
   */
  Text_3d(Boolean proto = false);

  /*! Destructor. */
  virtual ~Text_3d();

  /* Construct the prototype.
   * \return the prototype.
   */
  static Text_3d* prototype();

  /*! Clone.
   * \return the clone.
   */
  virtual Container* clone();

  /// \name Protoype handling
  //@{
  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype.
   * \return the node prototype.
   */
  virtual Container_proto* get_prototype();
  //@}

  /// \name field handlers
  //@{
  String_array* strings_handle(const Field_info*) { return &m_strings; }
  Shared_font_style* font_style_handle(const Field_info*)
  { return &m_font_style; }
  Float_array* lengths_handle(const Field_info*) { return &m_lengths; }
  Float* max_extent_handle(const Field_info*) { return &m_max_extent; }
  Float* depth_handle(const Field_info*) { return &m_depth; }
  //@}

  /*! Set the attributes of the transform.
   * \param elem (in) contains lists of attribute name and value pairs.
   */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes();

  /*! Clean the coordinate array.
   */
  virtual void clean_coords();

  /*! Clean the coordinate indices.
   */
  virtual void clean_flat_coord_indices();

  /*! Calculate the default 2D texture-mapping oordinates.
   */
  virtual void clean_tex_coord_array_2d();

  /*! Clean the texture coordinate indices.
   */
  void clean_flat_tex_coord_indices();

  /*! Process change of structure.
   */
  void structure_changed(const Field_info* field_info);

  /*! Set the text strings.
   * \param strings (in) the text strings.
   */
  void set_strings(const String_array& strings);

  /*! Obtain the text strings.
   * \return the text strings.
   */
  String_array get_strings() const;

  /*! Set the font style.
   * \param font_style (in) the font style.
   */
  void set_font_style(Shared_font_style font_style);

  /*! Get the font style.
   * \return the font style.
   */
  Shared_font_style get_font_style() const;

  /*! Set the lengths of the text strings.
   * \param lengths (in) the lengths of the text strings.
   */
  void set_lengths(const Float_array& lengths);

  /*! Get the lengths of the text strings.
   * \return the lengths of the text strings.
   */
  Float_array get_lengths() const;

  /*! Set the maximum extent of the text strings.
   * \param (in) the maximum extent of the text strings.
   */
  void set_max_extent(Float max_extent);

  /*! Get the maximum extent of the text strings.
   * \return the maximum extent of the text strings.
   */
  Float get_max_extent() const;

  /*! Set the depth of the 3D text.
   * \param depth (in) the depth of the 3D text.
   */
  void set_depth(Float depth);

  /*! Get the depth of the 3D text.
   * \return the depth of the 3D text.
   */
  Float get_depth() const;

protected:
  /*! obtains the tag (type) of the container.
   */
  virtual const std::string& get_tag() const;

  String_array m_strings;

  /*! Defines the size, font family, and style of the text font, the direction
   * of the text strings, and any specific language rendering techniques that
   * must be applied for non-English text.
   */
  Shared_font_style m_font_style;

  /*! Limits and scales the text strings if the length of the maximum string is
   * longer than the maximum extent, as measured in the local coordinate
   * system. If the text string with the maximum length is shorter than the
   * maxExtent, then there is no scaling. The maximum extent is measured
   * horizontally for horizontal text (FontStyle node: horizontal=TRUE) and
   * vertically for vertical text (FontStyle node: horizontal=FALSE). The
   * maxExtent field must be >= 0.0.
   */
  Float_array m_lengths;

  /*! Limits and scales the text strings if the length of the maximum string is
   * longer than the maximum extent, as measured in the local coordinate
   * system. If the text string with the maximum length is shorter than the
   * maxExtent, then there is no scaling. The maximum extent is measured
   * horizontally for horizontal text (FontStyle node: horizontal=TRUE) and
   * vertically for vertical text (FontStyle node: horizontal=FALSE). The
   * maxExtent field must be >= 0.0.
   */
  Float m_max_extent;

  /*! Specifies the thickness of each text string in the local coordinate
   * system.
   */
  Float m_depth;

private:
  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The container prototype. */
  static Container_proto* s_prototype;

  /*! default values for Text_3d.  */
  static const Float s_def_max_extent;
  static const Float s_def_depth;
};

//! \brief constructs the prototype.
inline Text_3d* Text_3d::prototype() { return new Text_3d(true); }

//! \brief clones.
inline Container* Text_3d::clone() { return new Text_3d(); }

//! \brief obtains the text strings.
inline String_array Text_3d::get_strings() const { return m_strings; }

//! \brief obtains the font style.
inline Text_3d::Shared_font_style Text_3d::get_font_style() const
{ return m_font_style; }

//! \brief obtains the lengths of the text strings.
inline Float_array Text_3d::get_lengths() const { return m_lengths; }

//! \brief obtains the maximum extent of the text strings.
inline Float Text_3d::get_max_extent() const { return m_max_extent; }

//! \brief obtains the depth of the 3D text.
inline Float Text_3d::get_depth() const { return m_depth; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Text_3d::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
