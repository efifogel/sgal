// Copyright (c) 2004,2018 Israel.
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
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_DXF_HEADER_HPP
#define SGAL_DXF_HEADER_HPP

#include <cstdint>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

SGAL_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header {
  int16_t m_acadmaintver;// Maintenance version number (should be ignored)
  String m_acadver;     // The AutoCAD drawing database version number:
                        // AC1006 = R10, AC1009 = R11 and R12,
                        // AC1012 = R13, AC1014 = R14, AC1500 = AutoCAD 2000
  double m_angbase;     // Angle 0 direction
  int16_t m_angdir;     // 1 = Clockwise angles, 0 = Counterclockwise
  int16_t m_attmode;    // Attribute visibility: 0 = None, 1 = Normal, 2 = All
  int16_t m_aunits;     // Units format for angles
  int16_t m_auprec;     // Units precision for angles
  int16_t m_cecolor;    // Current entity color number:
                        // 0 = BYBLOCK, 256 = BYLAYER
  double m_celtscale;   // Current entity linetype scale
  String m_celtype;     // Entity linetype name, or BYBLOCK or BYLAYER
  int8_t m_celweight;   // Lineweight of new objects
  unsigned int m_cpsnid;// Plotstyle handle of new objects. If CEPSNTYPE is 3,
                        // then this value indicates the handle.
  int8_t m_cepsntype;   // Plotstyle type of new objects:
                        // 0 = PlotStyle by layer
                        // 1 = PlotStyle by block
                        // 2 = PlotStyle by dictionary default
                        // 3 = PlotStyle by object ID/handle
  double m_chamfera;    // First chamfer distance
  double m_chamferb;    // Second chamfer distance
  double m_chamferc;    // Chamfer length
  double m_chamferd;    // Chamfer angle
  String m_clayer;      // Current layer name
  int16_t m_cmljust;    // Current multiline justification:
                        // 0 = Top, 1 = Middle, 2 = Bottom
  double m_cmlscale;    // Current multiline scale
  String m_cmlstyle;    // Current multiline style name
  int16_t m_dimadec;    // Number of precision places displayed in angular
                        // dimensions
  int16_t m_dimalt;     // Alternate unit dimensioning performed if nonzero
  int16_t m_dimaltd;    // Alternate unit decimal places
  double m_dimaltf;     // Alternate unit scale factor
  double m_dimaltrnd;   // Determines rounding of alternate units
  int16_t m_dimalttd;   // Number of decimal places for tolerance values of
                        // an alternate units dimension
  int16_t m_dimalttz;   // Controls suppression of zeros for alternate
                        // tolerance values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
  int16_t m_dimaltu;    // Units format for alternate units of all dimension
                        // style family members except angular:
                        // 1 = Scientific; 2 = Decimal; 3 = Engineering;
                        // 4 = Architectural (stacked);
                        // 5 = Fractional (stacked); 6 = Architectural;
                        // 7 = Fractional
  int16_t m_dimaltz;    // Controls suppression of zeros for alternate unit
                        // dimension values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
  String m_dimapost;    // Alternate dimensioning suffix
  int16_t m_dimaso;     // 1 = Create associative dimensioning,
                        // 0 = Draw individual entities
  double m_dimasz;      // Dimensioning arrow size
  int16_t m_dimatfit;   // Controls dimension text and arrow placement when
                        // space is not sufficient to place both within the
                        // extension lines:
                        // 0 = Places both text and arrows outside extension
                        //     lines
                        // 1 = Moves arrows first, then text
                        // 2 = Moves text first, then arrows
                        // 3 = Moves either text or arrows, whichever fits
                        //     best AutoCAD adds a leader to moved dimension
                        //     text when DIMTMOVE is set to 1.
  int16_t m_dimaunit;   // Angle format for angular dimensions:
                        // 0 = Decimal degrees,
                        // 1 = Degrees/minutes/seconds, 2 = Gradians,
                        // 3 = Radians, 4 = Surveyor's units
  int16_t m_dimazin;    // Controls suppression of zeros for angular
                        // dimensions:
                        // 0 = Displays all leading and trailing zeros
                        // 1 = Suppresses leading zeros in decimal dimensions
                        // 2 = Suppresses trailing zeros in decimal dimensions
                        // 3 = Suppresses leading and trailing zeros
  String m_dimblk;      // Arrow block name
  String m_dimblk1;     // First arrow block name
  String m_dimblk2;     // Second arrow block name
  double m_dimcen;      // Size of center mark/lines
  int16_t m_dimclrd;    // Dimension line color:
                        // range is 0 = BYBLOCK, 256 = BYLAYER
  int16_t m_dimclre;    // Dimension extension line color:
                        // range is 0 = BYBLOCK, 256 = BYLAYER
  int16_t m_dimclrt;    // Dimension text color:
                        // range is 0 = BYBLOCK, 256 = BYLAYER
  int16_t m_dimdec;     // Number of decimal places for the tolerance values
                        // of a primary units dimension
  double m_dimdle;      // Dimension line extension
  double m_dimdli;      // Dimension line increment
  int16_t m_dimdsep;    // Single-character decimal separator used when
                        // creating dimensions whose unit format is decimal.
  double m_dimexe;      // Extension line extension
  double m_dimexo;      // Extension line offset
  double m_dimfac;      // Scale factor used to calculate the height of text
                        // for dimension fractions and tolerances. AutoCAD
                        // multiplies DIMTXT by DIMTFAC to set the fractional
                        // or tolerance text height.
  double m_dimgap;      // Dimension line gap
  int16_t m_dimjust;    // Horizontal dimension text position:
                        // 0 = Above dimension line and center-justified
                        //     between extension lines,
                        // 1 = Above dimension line and next to first
                        //     extension line,
                        // 2 = Above dimension line and next to second
                        //     extension line,
                        // 3 = Above and center-justified to first extension
                        //     line,
                        // 4 = Above and center-justified to second extension
                        //     line
  String m_dimldrblk;   // Arrow block name for leaders
  double m_dimlfac;     // Linear measurements scale factor
  int16_t m_dimlim;     // Dimension limits generated if nonzero
  int16_t m_dimlunit;   // Sets units for all dimension types except Angular:
                        // 1 = Scientific; 2 = Decimal; 3 = Engineering
                        // 4 = Architectural; 5 = Fractional;
                        // 6 = Windows desktop
  int16_t m_dimlwd;     // Dimension line lineweight:
                        // -3 = Standard
                        // -2 = ByLayer
                        // -1 = ByBlock
                        // 0-211 = an integer representing 100th of mm
  int16_t m_dimlwe;     // Extension line lineweight:
                        // -3 = Standard
                        // -2 = ByLayer
                        // -1 = ByBlock
                        // 0-211 = an integer representing 100th of mm
  String m_dimpost;     // General dimensioning suffix
  double m_dimrnd;      // Rounding value for dimension distances
  int16_t m_dimsah;     // Use separate arrow blocks if nonzero
  double m_dimscale;    // Overall dimensioning scale factor
  int16_t m_dimsd1;     // Suppression of first extension line:
                        // 0 = Not suppressed, 1 = Suppressed
  int16_t m_dimsd2;     // Suppression of second extension line:
                        // 0 = Not suppressed, 1 = Suppressed
  int16_t m_dimse1;     // First extension line suppressed if nonzero
  int16_t m_dimse2;     // Second extension line suppressed if nonzero
  int16_t m_dimsho;     // 1 = Recompute dimensions while dragging
                        // 0 = Drag original image
  int16_t m_dimsoxd;    // Suppress outside-extensions dimension lines if
                        // nonzero
  String m_dimstyle;    // Dimension style name
  int16_t m_dimtad;     // Text above dimension line if nonzero
  int16_t m_dimtdec;    // Number of decimal places to display the tolerance
                        // values
  double m_dimtfac;     // Dimension tolerance display scale factor
  int16_t m_dimtih;     // Text inside horizontal if nonzero
  int16_t m_dimtix;     // Force text inside extensions if nonzero
  double m_dimtm;       // Minus tolerance
  int16_t m_dimtmove;   // Dimension text movement rules:
                        // 0 = Moves the dimension line with dimension text
                        // 1 = Adds a leader when dimension text is moved
                        // 2 = Allows text to be moved freely without a leader
  int16_t m_dimtofl;    // If text is outside extensions, force line
                        // extensions between extensions if nonzero
  int16_t m_dimtoh;     // Text outside horizontal if nonzero
  int16_t m_dimtol;     // Dimension tolerances generated if nonzero
  int16_t m_dimtolj;    // Vertical justification for tolerance values:
                        // 0 = Top, 1 = Middle, 2 = Bottom
  double m_dimtp;       // Plus tolerance
  double m_dimtsz;      // Dimensioning tick size:
                        // 0 = No ticks
  double m_dimtvp;      // Text vertical position
  String m_dimtxsty;    // Dimension text style
  double m_dimtxt;      // Dimensioning text height
  int16_t m_dimtzin;    // Controls suppression of zeros for tolerance values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
  int16_t m_dimupt;     // Cursor functionality for user positioned text:
                        // 0 = Controls only the dimension line location
                        // 1 = Controls the text position as well as the
                        // dimension line location
  int16_t m_dimzin;     // Controls suppression of zeros for primary unit values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
  int16_t m_dispsilh;   // Controls the display of silhouette curves of body
                        // objects in Wire-frame mode:
                        // 0 = Off, 1 = On
  String m_dwgcodepage; // Drawing code page; Set to the system code page
                        // when a new drawing is created, but not otherwise
                        // maintained by AutoCAD
  double m_elevation;   // Current elevation set by ELEV command
  int8_t m_endcaps;     // Lineweight endcaps setting for new objects:
                        // 0 = none; 1 = round; 2=angle; 3=square
  double m_extmax[3];   // X, Y, and Z drawing extents upper-right
                        // corner (in WCS)
  double m_extmin[3];   // X, Y, and Z drawing extents lower-left corner
                        // (in WCS)
  bool m_extnames;      // Controls symbol table naming:
                        // 0 = Release 14 compatibility. Limits names to 31
                        //   characters in length. Names can include the
                        //   letters A to Z, the numerals 0 to 9, and the
                        //   special characters, dollar sign ($),
                        //   underscore (_), and hyphen (-).
                        // 1 = AutoCAD 2000. Names can be up to 255
                        //   characters in length, and can include the
                        //   letters A to Z, the numerals 0 to 9, spaces,
                        //   and any special characters not used by Microsoft
                        //   Windows and AutoCAD for other purposes.
  int16_t m_filletrad;  // Fillet radius
  int16_t m_fillmode;   // Fill mode on if nonzero
  String m_fingerprintguid;// Set at creation time, uniquely identifies a
                        // particular drawing
  String m_handseed;    // Next available handle
  String m_hyperlinkbase;// Path for all relative hyperlinks in the
                        // drawing. If null, the drawing path is used.
  double m_insbase[3];  // Insertion base set by BASE command (in WCS)
  int16_t m_insunits;   // Default drawing units for AutoCAD DesignCenter
                        // blocks:
                        // 0 = Unitless; 1 = Inches; 2 = Feet; 3 = Miles;
                        // 4 = Millimeters; 5 = Centimeters; 6 = Meters;
                        // 7 = Kilometers; 8 = Microinches; 9 = Mils;
                        // 10 = Yards; 11 = Angstroms; 12 = Nanometers;
                        // 13 = Microns; 14 = Decimeters; 15 = Decameters;
                        // 16 = Hectometers; 17 = Gigameters;
                        // 18 = Astronomical units; 19 = Light years;
                        // 20 = Parsecs
  int8_t m_joinstyle;   // Lineweight joint setting for new objects:
                        // 0=none; 1= round; 2 = angle; 3 = flat
  int16_t m_limcheck;   // Nonzero if limits checking is on
  double m_limmax[2];   // XY drawing limits upper-right corner (in WCS)
  double m_limmin[2];   // XY drawing limits lower-left corner (in WCS)
  double m_ltscale;     // Global linetype scale
  int16_t m_lunits;     // Units format for coordinates and distances
  int16_t m_luprec;     // Units precision for coordinates and distances
  bool m_lwdisplay;     // Controls the display of lineweights on the Model
                        // or Layout tab:
                        // 0 = Lineweight is not displayed
                        // 1 = Lineweight is displayed
  int16_t m_maxactvp;   // Sets maximum number of viewports to be regenerated
  int16_t m_measurement;// Sets drawing units: 0 = English; 1 = Metric
  String m_menu;        // Name of menu file
  int16_t m_mirrtext;   // Mirror text if nonzero
  int16_t m_orthomode;  // Ortho mode on if nonzero
  int16_t m_pdmode;     // Point Display mode
  double m_pdsize;      // Point display size
  double m_pelevation;  // Current paper space elevation
  double m_pextmax[3];  // Maximum X, Y, and Z extents for paper space
  double m_pextmin[3];  // Minimum X, Y, and Z extents for paper space
  double m_pinsbase[3]; // Paper space insertion base point
  int16_t m_plimcheck;  // Limits checking in paper space when nonzero
  double m_plimmax[2];  // Maximum X and Y limits in paper space
  double m_plimmin[2];  // Minimum X and Y limits in paper space
  int16_t m_plinegen;   // Governs the generation of linetype patterns
                        // around the vertices of a 2D polyline:
                        // 1 = Linetype is generated in a continuous pattern
                        //     around vertices of the polyline
                        // 0 = Each segment of the polyline starts and ends
                        //     with a dash
  double m_plinewid;    // Default polyline width
  int16_t m_proxygraphics; // Controls the saving of proxy object images
  int16_t m_psltscale;  // Controls paper space linetype scaling:
                        // 1 = No special linetype scaling
                        // 0 = Viewport scaling governs linetype scaling
  bool m_pstylemode;    // Indicates whether the current drawing is in a
                        // Color-Dependent or Named Plot Style mode:
                        // 0 = Uses color-dependent plot style tables in the
                        //     current drawing
                        // 1 = Uses named plot style tables in the current
                        //     drawing
  double m_psvpscale;   // View scale factor for new viewports:
                        // 0 = Scaled to fit
                        // >0 = Scale factor (a positive real value)
  String m_pucsbase;    // Name of the UCS that defines the origin and
                        // orientation of orthographic UCS settings (paper
                        // space only)
  String m_pucsname;    // Current paper space UCS name
  double m_pucsorg[3];  // Current paper space UCS origin
  double m_pucsorgback[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'BACK' when
                        // PUCSBASE is set to WORLD.
  double m_pucsorgbottom[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'BOTTOM' when
                        // PUCSBASE is set to WORLD.
  double m_pucsorgfront[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'FRONT' when
                        // PUCSBASE is set to WORLD.
  double m_pucsorgleft[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'LEFT' when
                        // PUCSBASE is set to WORLD.
  double m_pucsorgright[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'RIGHT' when
                        // PUCSBASE is set to WORLD.
  double m_pucsorgtop[3]; // Point which becomes the new UCS origin
                        // after changing paper space UCS to 'TOP' when
                        // PUCSBASE is set to WORLD.
  String m_pucsorthoref;// If paper space UCS is orthographic
                        // (PUCSORTHOVIEW not equal to 0), this is the name
                        // of the UCS that the orthographic UCS is relative
                        // to. If blank, UCS is relative to WORLD.
  int16_t m_pucsorthoview;  // Orthographic view type of paper space UCS:
                        // 0 = UCS is not orthographic;
                        // 1 = Top; 2 = Bottom;
                        // 3 = Front; 4 = Back;
                        // 5 = Left; 6 = Right
  double m_pucsxdir[3]; // Current paper space UCS X axis
  double m_pucsydir[3]; // Current paper space UCS Y axis
  int16_t m_qtextmode;  // Quick Text mode on if nonzero
  int16_t m_regenmode;  // REGENAUTO mode on if nonzero
  int16_t m_shadedge;   // 0 = Faces shaded, edges not highlighted
                        // 1 = Faces shaded, edges highlighted in black
                        // 2 = Faces not filled, edges in entity color
                        // 3 = Faces in entity color, edges in black
  int16_t m_shadedif;   // Percent ambient/diffuse light, range 1-100,
                        // default 70
  double m_sketchinc;   // Sketch record increment
  int16_t m_skpoly;     // 0 = Sketch lines, 1 = Sketch polylines
  int16_t m_splframe;   // Spline control polygon display: 1 = On, 0 = Off
  int16_t m_splinesegs; // Number of line segments per spline patch
  int16_t m_splinetype; // Spline curve type for PEDIT Spline
  int16_t m_surftab1;   // Number of mesh tabulations in first direction
  int16_t m_surftab2;   // Number of mesh tabulations in second direction
  int16_t m_surftype;   // Surface type for PEDIT Smooth
  int16_t m_surfu;      // Surface density (for PEDIT Smooth) in M direction
  int16_t m_surfv;      // Surface density (for PEDIT Smooth) in N direction
  double m_tdcreate;    // Local date/time of drawing creation (see "Special
                        // Handling of Date/Time Variables")
  double m_tdindwg;     // Cumulative editing time for this drawing (see
                        // "Special Handling of Date/Time Variables")
  double m_tducreate;   // Universal date/time the drawing was created (see
                        // "Special Handling of Date/Time Variables")
  double m_tdupdate;    // Local date/time of last drawing update (see
                        // "Special Handling of Date/Time Variables")
  double m_tdusrtimer;  // User-elapsed timer
  double m_tduupdate;   // Universal date/time of the last update/save (see
                        // "Special Handling of Date/Time Variables")
  double m_textsize;    // Default text height
  String m_textstyle;   // Current text style name
  double m_thickness;   // Current thickness set by ELEV command
  int16_t m_tilemode;   // 1 for previous release compatibility mode, 0
                        // otherwise
  double m_tracewid;    // Default trace width
  int16_t m_treedepth;  // Specifies the maximum depth of the spatial index
  String m_ucsbase;     // Name of the UCS that defines the origin and
                        // orientation of orthographic UCS settings
  String m_ucsname;     // Name of current UCS
  double m_ucsorg[3];   // Origin of current UCS (in WCS)
  double m_ucsorgback[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'BACK' when
                        // UCSBASE is set to WORLD.
  double m_ucsorgbottom[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'BOTTOM' when
                        // UCSBASE is set to WORLD.
  double m_ucsorgfront[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'FRONT' when
                        // UCSBASE is set to WORLD.
  double m_ucsorgleft[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'LEFT' when
                        // UCSBASE is set to WORLD.
  double m_ucsorgright[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'RIGHT' when
                        // UCSBASE is set to WORLD.
  double m_ucsorgtop[3]; // Point which becomes the new UCS origin
                        // after changing model space UCS to 'TOP' when
                        // UCSBASE is set to WORLD.
  String m_ucsorthoref; // If model space UCS is orthographic (UCSORTHOVIEW
                        // not equal to 0), this is the name of the UCS that
                        // the orthographic UCS is relative to. If blank, UCS
                        // is relative to WORLD.
  int16_t m_ucsorthoview;  // Orthographic view type of model space UCS:
                        // 0 = UCS is not orthographic;
                        // 1 = Top; 2 = Bottom;
                        // 3 = Front; 4 = Back;
                        // 5 = Left; 6 = Right
  double m_ucsxdir[3];  // Direction of the current UCS X axis (in WCS)
  double m_ucsydir[3];  // Direction of the current UCS Y axis (in WCS)
  int16_t m_unitmode;   // Low bit set = Display fractions, feet-and-inches,
                        // and surveyor's angles in input format
  int16_t m_useri1;     // Reserved for use by third-party developers
  int16_t m_useri2;     // Reserved for use by third-party developers
  int16_t m_useri3;     // Reserved for use by third-party developers
  int16_t m_useri4;     // Reserved for use by third-party developers
  int16_t m_useri5;     // Reserved for use by third-party developers
  double m_userr1;      // Reserved for use by third-party developers
  double m_userr2;      // Reserved for use by third-party developers
  double m_userr3;      // Reserved for use by third-party developers
  double m_userr4;      // Reserved for use by third-party developers
  double m_userr5;      // Reserved for use by third-party developers
  int16_t m_usrtimer;   // 0 = Timer off, 1 = Timer on
  String m_versionguid; // Uniquely identifies a particular version of a
                        // drawing. Updated when the drawing is modified.
  int16_t m_visretain;  // 0 = Don't retain xref-dependent visibility settings
                        // 1 = Retain xref-dependent visibility settings
  int16_t m_worldview;  // 1 = Set UCS to WCS during DVIEW/VPOINT,
                        // 0 = Don't change UCS
  bool m_xedit;         // Controls whether the current drawing can be
                        // edited in-place when being referenced by another
                        // drawing.
                        // 0 = Can't use in-place reference editing
                        // 1 = Can use in-place reference editing
};

SGAL_END_NAMESPACE

#endif
