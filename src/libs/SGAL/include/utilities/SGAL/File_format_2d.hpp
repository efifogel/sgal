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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_FILE_FORMAT_2D_HPP
#define SGAL_FILE_FORMAT_2D_HPP

#include <string>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL File_format_2d {
public:
  /*! Format ids */
  enum Id {
    NONE = 0,
    // Raster
    ID_ANI,
    ID_ANIM,
    ID_APNG,
    ID_ART,
    ID_BMP,
    ID_DIB,
    ID_BPG,
    ID_BSAVE,
    ID_CAL,
    ID_CIN,
    ID_CPC,
    ID_CPT,
    ID_DPX,
    ID_ECW,
    ID_EXR,
    ID_FITS,
    ID_FIT,
    ID_FTS,
    ID_FLI,
    ID_FLC,
    ID_FPX,
    ID_GIF,
    ID_HDRI,
    ID_HEVC,
    ID_ICER,
    ID_ICNS,
    ID_ICO,
    ID_CUR,
    ID_ICS,
    ID_IDS,
    ID_LBM,
    ID_IFF,
    ID_JBIG,
    ID_JBG,
    ID_JNG,
    ID_JPEG,
    ID_JPG,
    ID_JXR,
    ID_HDP,
    ID_WDP,
    ID_MNG,
    ID_MIFF,
    ID_NRRD,
    ID_PAM,
    ID_PBM,
    ID_PGM,
    ID_PPM,
    ID_PNM,
    ID_PCX,
    ID_PGF,
    ID_PICTOR,
    ID_PNG,
    ID_PSD,
    ID_PSB,
    ID_PSP,
    ID_QTVR,
    ID_RAS,
    ID_SUN,
    ID_RBE,
    ID_SGI,
    ID_RGB,
    ID_TGA,
    ID_TIFF,
    ID_TIF,
    ID_WBMP,
    ID_WEBP,
    ID_XBM,
    ID_XCF,
    ID_XPM,
    ID_XWD,
    // Raw
    ID_CIFF,
    ID_DNG,
    // Vector
    ID_AI,
    ID_CDR,
    ID_CGM,
    ID_DXF,
    ID_EVA,
    ID_GERBER,
    ID_HVIF,
    ID_IGES,
    ID_PGML,
    ID_SVG,
    ID_SVGZ,
    ID_HTM,
    ID_HTML,
    ID_WMF,
    ID_EMF,
    ID_WMZ,
    ID_EMZ,
    ID_XAR,
    // Compound
    ID_CDF,
    ID_DJVU,
    ID_DJV,
    ID_EPS,
    ID_PDF,
    ID_PICT,
    ID_PCT,
    ID_PIC,
    ID_PS,
    ID_SWF,
    ID_XAML,
    NUM_IDS
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
