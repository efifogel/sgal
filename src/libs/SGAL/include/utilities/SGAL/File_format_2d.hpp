// Copyright (c) 2014 Israel.
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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FILE_FORMAT_2D_HPP
#define SGAL_FILE_FORMAT_2D_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL File_format_2d {
public:
  //! Format codes
  enum Code {
    INVALID = 0,
    ANI,
    ANIM,
    APNG,
    ART,
    BMP,
    DIB,
    BPG,
    BSAVE,
    CAL,
    CIN,
    CPC,
    CPT,
    DPX,
    ECW,
    EXR,
    FITS,
    FIT,
    FTS,
    FLI,
    FLC,
    FPX,
    GIF,
    HDRI,
    HEVC,
    ICER,
    ICNS,
    ICO,
    CUR,
    ICS,
    IDS,
    LBM,
    IFF,
    JBIG,
    JBG,
    JNG,
    JPEG,
    JPG,
    JXR,
    HDP,
    WDP,
    MNG,
    MIFF,
    NRRD,
    PAM,
    PBM,
    PGM,
    PPM,
    PNM,
    PCX,
    PGF,
    PICTOR,
    PNG,
    PSD,
    PSB,
    PSP,
    QTVR,
    RAS,
    SUN,
    RBE,
    SGI,
    RGB,
    TGA,
    TIFF,
    TIF,
    WBMP,
    WEBP,
    XBM,
    XCF,
    XPM,
    XWD,
    // Raw
    CIFF,
    DNG,
    // Vector
    AI,
    CDR,
    CGM,
    DXF,
    EVA,
    GERBER,
    HVIF,
    IGES,
    PGML,
    SVG,
    SVGZ,
    HTM,
    HTML,
    WMF,
    EMF,
    WMZ,
    EMZ,
    XAR,
    // Compound
    CDF,
    DJVU,
    DJV,
    EPS,
    PDF,
    PICT,
    PCT,
    PIC,
    PS,
    SWF,
    XAML,
    NUM_CODES
  };

  /*! Compare the name of the ith format to a given token.
   * \param[in] i The index of the format to compare against.
   * \param[in] token The token string to compare.
   */
  static Boolean compare_name(Uint i, const std::string& token);

  /*! obtains the ith format name.
   * \param[in] i The index of the format.
   * \return the name of the ith format.
   */
  static const Char* get_name(Uint i);

private:
  //! file format names also used as file extensions.
  static const Char* s_names[];
};

SGAL_END_NAMESPACE

#endif
