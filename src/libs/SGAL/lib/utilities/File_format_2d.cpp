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
#include "SGAL/File_format_2d.hpp"

SGAL_BEGIN_NAMESPACE

//! file format options.
const Char* File_format_2d::s_names[] = {
  "none",
  // Raster
  "ani",    // used for animated mouse cursors (Microsoft)
  "anim",   // used to store digital movies and computer generated animations
  "apng",   // Animated Portable Network Graphics (extension to PNG)
  "art",    // a proprietary image file format used mostly by the AOL service
  "bmp",    // bitmap image file or DIB (device independent bitmap)
  "dib",    // same as above
  "bpg",    // Better Portable Graphics
  "bsave",  // created usually by saving raw video memory to disk
  "cal",    // interchange of graphics data
  "cin",    // designed to represent scanned film images (Kodak)
  "cpc",    // Cartesian Perceptual Compression
  "cpt",    // used by some versions of Corel Photo Paint
  "dpx",    // Digital Picture Exchange
  "ecw",    // Enhanced Compression Wavelet, wavelet compression
  "exr",    //  high dynamic range imaging image file format (ILM)
  "fits",   // Flexible Image Transport System, commonly used in astronomy
  "fit",    // same as above
  "fts",    // same as above
  "fli",    // used to store animation files (Autodesk)
  "flc",    // same as above
  "fpx",    // bitmapped computer graphics
  "gif",    // Graphics Interchange Format (CompuServe)
  "",       // HDR scans in a TIFF container (LaserSoft Imaging)
  "hevc",   // High Efficiency Video Coding
  "icer",   // a wavelet-based image compression (NASA Mars Rovers)
  "icns",   // Apple Icon Image
  "ico",    // used for computer icons (Microsoft)
  "cur",    // used non-animated cursors (Microsoft)
  "ics",    // Image Cytometry Standar, used in life sciences microscopy
  "ids",    // same as above
  "iff",    // InterLeaved BitMap (Electronic Arts)
  "lbm",    // same as above
  "jbig",   // Joint Bi-level Image (ISO, IEC, ITU-T)
  "jbg",    // same as above
  "jng",    // JPEG Network Graphics
  "jpeg",   // lossy compression for digital images
  "jpg",    // same as above
  "jxr",    // JPEG extended range (Microsoft, ITU-T, ISO/IEC)
  "hdp",    // same as above
  "wdp",    // same as above
  "mng",    // Multiple-image Network Graphics, used for animated images
  "miff",   // Magick Image File Format (ImageMagick)
  "nrrd",   // nearly raster raw data, used for n-dimensional raster data
  "pam",    // Portable Arbitrary Map
  "pbm",    // portable bitmap format  (Netpbm)
  "pgm",    // portable graymap format (Netpbm)
  "ppm",    // portable pixmap format  (Netpbm)
  "pnm",    // portable anymap format  (Netpbm)
  "pcx",    // PiCture eXchange (ZSoft Corporation)
  "pgf",    // Progressive Graphics File, wavelet-based (xeraina GmbH)
  "pic",    // PC Paint
  "png",    // Portable Network Graphics
  "psd",    // Photoshop Document (Photoshop)
  "psb",    // Photoshop Big (Photoshop)
  "psp",    // PaintShop Pro (Jasc Software)
  "qtvr",   // QuickTime Virtual Reality (Apple)
  "ras",    // Sun Raster (Sun Microsystems)
  "sun",    // same as above
  "hdr",    // RGBE
  "sgi",    // Silicon Graphics Image (SGI)
  "rgb",    // same as above
  "tga",    // native format of TARGA and VISTA boards (Truevision)
  "tiff",   // Tag Image File Format/Electronic Photography
  "tif",    // same as above
  "wbmp",   // Wireless Application Protocol Bitmap Format (WAP Forum)
  "webp",   // lossy and lossless compression (Google)
  "xbm",    // X BitMap
  "xcf",    // eXperimental Computing Facility (GIMP)
  "xpm",    // X PixMap (BULL Research)
  "xwd",    // X Window dump
  // Raw
  "crw",    // Camera Image File Format (Canon, CIFF Forum)
  "dng",    // Digital Negative (Adobe Systems)
  // Vector
  "ai",     // Adobe Illustrator Artwork(Adobe Systems)
  "cdr",    // CorelDraw file format (Corel Corporation)
  "cgm",    // Computer Graphics Metafile (ANSI, ISO/IEC, W3C)
  "dxf",    // Drawing Interchange Format, or Drawing Exchange Format (Autodesk)
  "eva",    // Extended Vector Animation (Sharp Corporation)
  "gbr",    // used by PCB industry (Gerber Systems Corp., now Ucamco)
  "",       // Haiku Vector Icon Format (Haiku Project)
  "iges",   // Initial Graphics Exchange Specification
  "pgml",   // Precision Graphics Markup Language
  "svg",    // Scalable Vector Graphics (W3C)
  "svgz",   // SVG compressed with gzip
  "htm",    // Vector Markup Language
  "html",   // same as above
  "wmf",    // Windows Metafile (Microsoft)
  "emf",    // same as above
  "wmz",    // same as above
  "emz",    // same as above
  "xar",    // (Xara)
  // Compound
  "cdf",    // Computable Document Format (Wolfram Research)
  "djvu",   // used to store scanned documents (AT&T)
  "djv",    // same as above
  "eps",    // Encapsulated PostScript
  "pdf",    // Portable Document Format (Adobe Systems)
  "pict",   // (Apple Computer)
  "pct",    // same as above
  "pic",    // same as above
  "ps",     // PostScript (Adobe Systems)
  "swf",    // Small Web Format (Adobe Systems)
  "xaml"    // Extensible Application Markup Language (Microsoft)
};

//! \brief compares the name of the ith format to a given token.
Boolean File_format_2d::compare_name(Uint i, const std::string& token)
{ return (token.compare(s_names[i]) == 0); }

//! obtains the ith format name.
const Char* File_format_2d::get_name(Uint i) { return s_names[i]; }

SGAL_END_NAMESPACE
