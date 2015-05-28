// Copyright (c) 2004  Israel.
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

#ifndef SGAL_IMAGE_HPP
#define SGAL_IMAGE_HPP

/*! \file
 * A class that holds images of various formats.
 */

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Math_defs.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SGAL_SGAL_DECL Image : public Container {
public:
  enum {
    FIRST = Container::LAST - 1,
    WIDTH,
    HEIGHT,
    FORMAT,
    PIXELS,
    LAST
  };

   enum Format {
    kIllegal = 0,

    // Generic formats:
    kFormat1,
    kFormat2,
    kFormat4,
    kFormat8,
    kFormat12,
    kFormat16,
    kFormat32,

    // Palette format:
    kPalette1,
    kPalette2,
    kPalette4,          // 10
    kPalette8,
    kPalette12,
    kPalette16,

    // Alpha formats:
    kAlpha4,
    kAlpha8,
    kAlpha12,
    kAlpha16,

    // Luminance formats:
    kLuminance4,
    kLuminance8,
    kLuminance12,       // 20
    kLuminance16,

    // Intensity formats:
    kIntensity4,
    kIntensity8,
    kIntensity12,
    kIntensity16,

    // Alpha Palette:
    kAlphaPalette8_8,

    // Luminance alpha:
    kLuminanceAlpha4_4,
    kLuminanceAlpha6_2,
    kLuminanceAlpha8_8,
    kLuminanceAlpha12_4,        // 30
    kLuminanceAlpha12_12,
    kLuminanceAlpha16_16,

    // Alpha Luminance:
    kAlphaLuminance4_4,
    kAlphaLuminance8_8,

    // RGB:
    kRGB3_3_2,
    kRGB4_4_4,
    kRGB5_5_5,
    kRGB5_6_5,
    kRGB8_8_8,
    kRGB10_10_10,               // 40
    kRGB12_12_12,
    kRGB16_16_16,

    // BGR:
    kBGR2_3_3,
    kBGR4_4_4,
    kBGR5_5_5,
    kBGR5_6_5,
    kBGR8_8_8,
    kBGR10_10_10,
    kBGR12_12_12,
    kBGR16_16_16,               // 50

    // YIQ:
    kYIQ4_2_2,

    // RGBA
    kRGBA2_2_2_2,
    kRGBA4_4_4_4,
    kRGBA3_3_2_8,
    kRGBA5_5_5_1,
    kRGBA8_8_8_8,
    kRGBA10_10_10_2,
    kRGBA12_12_12_12,
    kRGBA16_16_16_16,

    // ARGB:
    kARGB2_2_2_2,               // 60
    kARGB4_4_4_4,
    kARGB8_3_3_2,
    kARGB1_5_5_5,
    kARGB8_8_8_8,
    kARGB10_10_10_2,
    kkARGB12_12_12_12,
    kARGB16_16_16_16,

    // ABGR:
    kABGR2_2_2_2,
    kABGR4_4_4_4,
    kABGR8_2_3_3,               // 70
    kABGR1_5_5_5,
    kABGR8_8_8_8,
    kABGR2_10_10_10,
    kABGR12_12_12_12,
    kABGR16_16_16_16,

    // AYIQ:
    kAYIQ8_4_2_2,

    // Depth & Stencil:
    kDepth16,
    kDepth32,
    kStencil,
    kDepthStencil24_8,              // 80

    kNumFormats
  };

  /*! Construct default. */
  Image(Boolean proto = false);

  /*! Destructor */
  virtual ~Image();

  /*! Construct the prototype. */
  static Image* prototype();

  /*! Clone. */
  virtual Container* clone();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

  /// \name field handlers
  //@{
  Uint* width_handle(const Field_info*) { return &m_width; }
  Uint* height_handle(const Field_info*) { return &m_height; }
  Format* format_handle(const Field_info*) { return &m_format; }
  //@}

  /*! Set the attributes of this node. */
  virtual void set_attributes(Element* elem);

  // virtual Attribute_list get_attributes()
  // { assert(0); Attribute_list a; return a; }

  /*! Set the image width. */
  void set_width(Uint width);

  /*! Obtain the image width. */
  virtual Uint get_width();

  /*! Set the image height. */
  void set_height(Uint height);

  /*! Obtain the image height. */
  virtual Uint get_height();

  /*! Set the image format. */
  void set_format(Format format);

  /*! Obtain the image format. */
  virtual Format get_format();

  /*! Set the number of pixels in a row. */
  void set_pack_row_length(Uint length);

  /*! Obtain the number of pixels in a row. */
  Uint get_pack_row_length() const;

  /*! Set the image pixel space.
   * \param pixels the image pixel space.
   */
  void set_pixels(void* pixels);

  /*! Obtain the image pixel data. */
  virtual void* get_pixels();

  /*! Return true if image hasn't been updated yet and false otherwise. */
  virtual Boolean is_dirty();

  /*! Clean the image in case it is dirty. */
  virtual void clean();

  /*! Determines whether the image is empty. */
  Boolean empty();

  /*! Obtain the texture number of components. */
  Uint get_component_count();

  /*! Obtain the memory that is used by the image (in bytes). */
  Uint get_size() const;

  /*! Obtain the memory that is used by an image (in bytes) with the given
   * attributes.
   */
  static Uint get_size(Uint width, Uint height, Format format);

  /*! Allocate space for the image.
   */
  void allocate_space();

 /*! Obtain the number of bits. */
  static Uint get_format_size(Format format);

  /*! Obtain the openGl format. */
  static GLenum get_format_format(Format format);

  /*! Obtain the storage type of component. */
  static GLenum get_format_type(Format format);

  /*! Obtain the number of components. */
  static Uint get_format_components(Format format);

  /*! Obtain the openGl internal format. */
  static GLenum get_format_internal_format(Format format);

  /*! Obtain the format name (string). */
  static const char* get_format_name(Format format);

  /*! Draw the image. */
  void draw();

protected:
  /*! Obtain the tag (type) of the container. */
  virtual const std::string& get_tag() const;

  /*! A map from format to number of bits. */
  static Uint s_format_sizes[];

  /*! A map from format to openGl format. */
  static GLenum s_format_formats[];

  /*! A map from format to storage type of component. */
  static GLenum s_format_types[];

  /*! A map from format to number of components. */
  static Uint s_format_components[];

  /*! A map from format to OpenGl internal format. */
  static GLenum s_format_internal_formats[];

  /*! A map from format to format names. */
  static const char* s_format_names[];

  /*! The image width. */
  Uint m_width;

  /*! The image height. */
  Uint m_height;

  /*! The image format. */
  Format m_format;

  /*! The image pixel-data. */
  void* m_pixels;

  /*! Defines the number of pixels in a row. */
  Uint m_pack_row_length;

  /*! Indicates whether the image is dirty and should be cleaned. */
  Boolean m_dirty;

  /*! Indicates whether the image pixel space is owned. If it is owned it
   * should be destructed when the image is destructed.
   */
  Boolean m_owned_pixels;

private:
  /*! The node prototype. */
  static Container_proto* s_prototype;

  /*! The tag that identifies this container type. */
  static const std::string s_tag;

  /*! The size of memory allocated for the pixels. */
  Uint m_size;

  /// \name Default values
  //@{
  static const Uint s_def_width;
  static const Uint s_def_height;
  static const Format s_def_format;
  //@}
};

//! \brief constructs the prototype.
inline Image* Image::prototype() { return new Image(true); }

//! \brief clones.
inline Container* Image::clone() { return new Image(); }

//! \brief sets the number of pixels in a row.
inline void Image::set_pack_row_length(Uint length)
{ m_pack_row_length = length; }

//! \brief obtains the number of pixels in a row.
inline Uint Image::get_pack_row_length() const
{ return m_pack_row_length; }

//! \brief returns true if image hasn't been updated yet and false otherwise.
inline Boolean Image::is_dirty() { return m_dirty; }

/*! \brief cleans the image in case it is dirty. */
inline void Image::clean() {}

/*! \brief obtains the memory that is used by an image (in bytes) with the given
 * attributes.
 */
inline Uint Image::get_size(Uint width, Uint height, Format format)
{ return width * height * bits2bytes(s_format_sizes[format]); }

//! \brief obtains the number of bits.
inline Uint Image::get_format_size(Format format)
{ return s_format_sizes[format]; }

//! \brief obtains the openGl format.
inline GLenum Image::get_format_format(Format format)
{ return s_format_formats[format]; }

//! \brief obtains the storage type of component.
inline GLenum Image::get_format_type(Format format)
{ return s_format_types[format]; }

//! \brief obtains the number of components.
inline Uint Image::get_format_components(Format format)
{ return s_format_components[format]; }

//! \brief obtains the format name (string).
inline const char* Image::get_format_name(Format format)
{ return s_format_names[format]; }

//! \brief obtains the tag (type) of the container.
inline const std::string& Image::get_tag() const { return s_tag; }

SGAL_END_NAMESPACE

#endif
