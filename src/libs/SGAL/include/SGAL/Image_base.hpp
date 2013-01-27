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
// $Id: $
// $Revision: 7205 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_IMAGE_HPP
#define SGAL_IMAGE_HPP

/*! \file
 * A class that holds images of various formats.
 * Image_base is a class that can hold an image in various formats. 
 * We currently assume that the default image is RGB8_8_8 (24 bit).
 *    
 *  Inherits from Container
 */

#if (defined _MSC_VER)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Container.hpp"
#include "SGAL/Math_defs.hpp"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;

class SGAL_CLASSDEF Image_base : public Container {
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

  /*! Constructor */
  Image_base(Boolean proto = false);

  /*! Destructor */
  virtual ~Image_base();

  /*! Initialize the node prototype. */
  virtual void init_prototype();

  /*! Delete the node prototype. */
  virtual void delete_prototype();

  /*! Obtain the node prototype. */
  virtual Container_proto* get_prototype();

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
  
  /*! Set the image pixel data. */
  void set_pixels(void* pixels);

  /*! Obtain the image pixel data. */
  virtual void* get_pixels();

  /*! Return true if image hasn't been updated yet and false otherwise. */
  virtual Boolean is_dirty();

  /*! Clean the image in case it is dirty. */
  virtual void clean();

  /*! Obtain the memory that is used by the image (in bytes). */
  Uint get_size() const;
  
  /*! Obtain the memory that is used by an image (in bytes) with the given
   * attributes.
   */
  static Uint get_size(Uint width, Uint height, Format format);

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

 private:
  /*! The node prototype. */
  static Container_proto* s_prototype;
  
  /*! Default value. */
  static Format s_def_format;
};

/*! \brief sets the image width. */
inline void Image_base::set_width(Uint width) { m_width = width; }

/*! \brief obtains the image width. */
inline Uint Image_base::get_width() { return m_width; }

/*! \brief sets the image height. */
inline void Image_base::set_height(Uint height) { m_height = height; }

/*! \brief obtains the image height. */
inline Uint Image_base::get_height() { return m_height; }

/*! \brief sets the image format. */
inline void Image_base::set_format(Format format) { m_format = format; }

/*! \brief obtains the image format. */
inline Image_base::Format Image_base::get_format() { return m_format; }

/*! \brief sets the number of pixels in a row. */
inline void Image_base::set_pack_row_length(Uint length)
{ m_pack_row_length = length; }

/*! \brief obtains the number of pixels in a row. */
inline Uint Image_base::get_pack_row_length() const { return m_pack_row_length; }
  
/*! \brief sets the image pixel data. */
inline void Image_base::set_pixels(void* pixels) { m_pixels = pixels; }

/*! \brief obtains the image pixel data. */
inline void* Image_base::get_pixels() { return m_pixels; }

/*! \brief returns true if image hasn't been updated yet and false otherwise. */
inline Boolean Image_base::is_dirty() { return false; }

/*! \brief cleans the image in case it is dirty. */
inline void Image_base::clean() {}

/*! \brief obtains the memory that is used by an image (in bytes) with the given
 * attributes.
 */
inline Uint Image_base::get_size(Uint width, Uint height, Format format)
{ return width * height * bits2bytes(s_format_sizes[format]); }

/*! \brief obtains the number of bits. */
inline Uint Image_base::get_format_size(Format format)
{ return s_format_sizes[format]; }

/*! \brief obtains the openGl format. */
inline GLenum Image_base::get_format_format(Format format)
{ return s_format_formats[format]; }

/*! \brief obtains the storage type of component. */
inline GLenum Image_base::get_format_type(Format format)
{ return s_format_types[format]; }

/*! \brief obtains the number of components. */
inline Uint Image_base::get_format_components(Format format)
{ return s_format_components[format]; }

/*! \brief obtains the openGl internal format. */
inline GLenum Image_base::get_format_internal_format(Format format)
{ return s_format_internal_formats[format]; }

/*! \brief obtains the format name (string). */
inline const char* Image_base::get_format_name(Format format)
{ return s_format_names[format]; }
  
SGAL_END_NAMESPACE

#endif
