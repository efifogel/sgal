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

#include "SGAL/basic.hpp"
#include "SGAL/Image_format.hpp"

SGAL_BEGIN_NAMESPACE

//! The singleton.
Image_format* Image_format::s_instance(nullptr);

//! \brief obtains the singleton.
Image_format* Image_format::get_instance()
{
  if (!s_instance) s_instance = new Image_format();
  return s_instance;
}

//! \brief constructs.
Image_format::Image_format()
{
  // Trace options.
  Map options = {
    {"", INVALID},
#if 0
    {"ani", ANI},   // Animated mouse cursors (Microsoft)
    {"anim", ANIM}, // Store digital movies and computer generated animations
    {"apng", APNG}, // Animated Portable Network Graphics (extension to PNG)
    {"art", ART},   // Proprietary file format used mostly by the AOL service
    {"bmp", BMP},   // bitmap image file or DIB (device independent bitmap)
    {"dib", DIB},   // same as above
    {"bpg", BPG},   // Better Portable Graphics
    {"bsave", BSAVE}, // created usually by saving raw video memory to disk
    {"cal", CAL},   // interchange of graphics data
    {"cin", CIN},   // designed to represent scanned film images (Kodak)
    {"cpc", CPC},   // Cartesian Perceptual Compression
    {"cpt", CPT},   // used by some versions of Corel Photo Paint
    {"dpx", DPX},   // Digital Picture Exchange
    {"ecw", ECW},   // Enhanced Compression Wavelet, wavelet compression
    {"exr", EXR},   // high dynamic range imaging image file format (ILM)
    {"fits", FITS}, // Flexible Image Transport System, commonly used in astronomy
    {"fit", FIT},   // same as above
    {"fts", FTS},   // same as above
    {"fli", FLI},   // used to store animation files (Autodesk)
    {"flc", FLC},   // same as above
    {"fpx", FPX},   // bitmapped computer graphics
    {"gif", GIF},   // Graphics Interchange Format (CompuServe)
    {"", HDRI},     // HDR scans in a TIFF container (LaserSoft Imaging)
    {"hevc", HEVC}, // High Efficiency Video Coding
    {"icer", ICER}, // a wavelet-based image compression (NASA Mars Rovers)
    {"icns", ICNS}, // Apple Icon Image
    {"ico", ICO},   // used for computer icons (Microsoft)
    {"cur", CUR},   // used non-animated cursors (Microsoft)
    {"ics", ICS},   // Image Cytometry Standar, used in life sciences microscopy
    {"ids", IDS},   // same as above
    {"iff", IFF},   // InterLeaved BitMap (Electronic Arts)
    {"lbm", LBM},   // same as above
    {"jbig", JBIG}, // Joint Bi-level Image (ISO, IEC, ITU-T)
    {"jbg", JBG},   // same as above
    {"jng", JNG},   // JPEG Network Graphics
#endif
    {"jpeg", JPEG}, // lossy compression for digital images
    {"jpg", JPG},   // same as above
#if 0
    {"jxr", JXR},   // JPEG extended range (Microsoft, ITU-T, ISO/IEC)
    {"hdp", HDP},   // same as above
    {"wdp", WDP},   // same as above
    {"mng", MNG},   // Multiple-image Network Graphics, used for animated images
    {"miff", MIFF}, // Magick Image File Format (ImageMagick)
    {"nrrd", NRRD}, // nearly raster raw data, used for n-dimensional raster data
    {"pam", PAM},   // Portable Arbitrary Map
#endif
    {"pbm", PBM},   // portable bitmap format  (Netpbm)
    {"pgm", PGM},   // portable graymap format (Netpbm)
    {"ppm", PPM},   // portable pixmap format  (Netpbm)
#if 0
    {"pnm", PNM},   // portable anymap format  (Netpbm)
    {"pcx", PCX},   // PiCture eXchange (ZSoft Corporation)
    {"pgf", PGF},   // Progressive Graphics File, wavelet-based (xeraina GmbH)
    {"pic", PIC},   // PC Paint
#endif
    {"png", PNG},   // Portable Network Graphics
#if 0
    {"psd", PSD},   // Photoshop Document (Photoshop)
    {"psb", PSB},   // Photoshop Big (Photoshop)
    {"psp", PSP},   // PaintShop Pro (Jasc Software)
    {"qtvr", QTVR}, // QuickTime Virtual Reality (Apple)
    {"ras", RAS},   // Sun Raster (Sun Microsystems)
    {"sun", SUN},   // same as above
    {"rbe", RBE},   // RGBE
    {"sgi", SGI},   // Silicon Graphics Image (SGI)
    {"rgb", RGB},   // same as above
    {"tga", TGA},   // native format of TARGA and VISTA boards (Truevision)
    {"tiff", TIFF}, // Tag Image File Format/Electronic Photography
    {"tif", TIF},   // same as above
    {"wbmp", WBMP}, // Wireless Application Protocol Bitmap Format (WAP Forum)
    {"webp", WEBP}, // lossy and lossless compression (Google)
    {"xbm", XBM},   // X BitMap
    {"xcf", XCF},   // eXperimental Computing Facility (GIMP)
    {"xpm", XPM},   // X PixMap (BULL Research)
    {"xwd", XWD},   // X Window dump
    // Raw
    {"crw", CIFF},  // Camera Image File Format (Canon, CIFF Forum)
    {"dng", DNG},   // Digital Negative (Adobe Systems)
    // Vector
    {"ai", AI},     // Adobe Illustrator Artwork(Adobe Systems)
    {"cdr", CDR},   // CorelDraw file format (Corel Corporation)
    {"cgm", CGM},   // Computer Graphics Metafile (ANSI, ISO/IEC, W3C)
    {"dxf", DXF},   // Drawing Interchange Format, or Drawing Exchange Format (Autodesk)
    {"eva", EVA},   // Extended Vector Animation (Sharp Corporation)
    {"gbr", GERBER}, // used by PCB industry (Gerber Systems Corp., now Ucamco)
    {"", HVIF},     // Haiku Vector Icon Format (Haiku Project)
    {"iges", IGES}, // Initial Graphics Exchange Specification
    {"pgml", PGML}, // Precision Graphics Markup Language
    {"svg", SVG},   // Scalable Vector Graphics (W3C)
    {"svgz", SVGZ}, // SVG compressed with gzip
    {"htm", HTM},   // Vector Markup Language
    {"html", HTML}, // same as above
    {"wmf", WMF},   // Windows Metafile (Microsoft)
    {"emf", EMF},   // same as above
    {"wmz", WMZ},   // same as above
    {"emz", EMZ},   // same as above
    {"xar", XAR},   // (Xara)
    // Compound
    {"cdf", CDF},   // Computable Document Format (Wolfram Research)
    {"djvu", DJVU}, // used to store scanned documents (AT&T)
    {"djv", DJV},   // same as above
    {"eps", EPS},   // Encapsulated PostScript
    {"pdf", PDF},   // Portable Document Format (Adobe Systems)
    {"pict", PICT}, // (Apple Computer)
    {"pct", PCT},   // same as above
    {"pic", PIC},   // same as above
    {"ps", PS},     // PostScript (Adobe Systems)
    {"swf", SWF},   // Small Web Format (Adobe Systems)
    {"xaml", XAML}  // Extensible Application Markup Language (Microsoft)
#endif
  };
  set_options(std::move(options));
};

SGAL_END_NAMESPACE
