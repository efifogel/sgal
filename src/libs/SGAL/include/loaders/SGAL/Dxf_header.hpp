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
  bool m_attdia;        // Controls whether the INSERT command uses a dialog
                        // box for attribute value entry
  int16_t m_attmode;    // Attribute visibility: 0 = None, 1 = Normal, 2 = All
  bool m_attreq;        // Controls whether INSERT uses default attribute
                        // settings during insertion of blocks
  int16_t m_aunits;     // Units format for angles
  int16_t m_auprec;     // Units precision for angles
  bool m_axismode;      // Axis on
  double m_axisunit[2]; // Axis X and Y tick spacing
  bool m_blipmode;      // Display blips for click locations
  bool m_cameradisplay; // Turns the display of camera objects on or off
  double m_cameraheight; // Specifies the default height for new camera objects
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
  bool m_delobj;        // Controls object deletion
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
  int8_t m_dimassoc;    // Controls the associativity of dimension objects
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
  int16_t m_dimfrac;    // Sets the fraction format when DIMLUNIT is set to
                        // Architectural or Fractional.
  double m_dimfxl;      // Sets the total length of the extension lines starting
                        // from the dimension line toward the dimension origin
  bool m_dimfxlon;      // Controls whether extension lines are set to a fixed
                        // length
  double m_dimgap;      // Dimension line gap
  double m_dimjogang;   // Determines the angle of the transverse segment of the
                        // dimension line in a jogged radius dimension
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
  String m_dimltex1;    // Sets the line type of the first extension line
  String m_dimltex2;    // Sets the line type of the second extension line
  String m_dimltype;    // Sets the line type of the dimension line
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
  bool m_fastzoom;      // Fast zoom enabled
  double m_filletrad;   // Fillet radius
  int16_t m_fillmode;   // Fill mode on if nonzero
  String m_fingerprintguid;// Set at creation time, uniquely identifies a
                        // particular drawing
  bool m_gridmode;      // Grid mode on
  double m_gridunit[2]; // Grid X and Y spacing
  int8_t m_halogap;     // Specifies a gap to be displayed where an object is
                        // hidden by another object; the value is specified as
                        // a percent of one unit and is independent of the zoom
                        // level. A haloed line is shortened at the point where
                        // it is hidden when HIDE or the Hidden option of
                        // SHADEMODE is used
  bool m_handling;      // Handles available
  String m_handseed;    // Next available handle
  int8_t m_hidetext;    // Ignore text objects
  String m_hyperlinkbase;// Path for all relative hyperlinks in the
                        // drawing. If null, the drawing path is used.
  double m_insbase[3];  // Insertion base set by BASE command (in WCS)
  int8_t m_indexctl;    // Controls whether layer and spatial indexes are
                        // created and saved in drawing files
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
  int16_t m_intersectioncolor; // Specifies the entity color of intersection
                        // polylines
  int8_t m_intersectiondisplay; // Specifies the display of intersection
                        // polylines
  int8_t m_joinstyle;   // Lineweight joint setting for new objects:
                        // 0=none; 1= round; 2 = angle; 3 = flat
  String m_lastsavedby; // Name of the last user to modify the file
  double m_latitude;    // The latitude of the geographic location assigned to
                        // the drawing
  double m_lenslength;  // Stores the length of the lens in millimeters used in
                        // perspective viewing
  bool m_lightglyphdisplay; // Turns on and off the display of light glyphs
  int16_t m_limcheck;   // Nonzero if limits checking is on
  double m_limmax[2];   // XY drawing limits upper-right corner (in WCS)
  double m_limmin[2];   // XY drawing limits lower-left corner (in WCS)
  double m_loftang1;    // Sets the draft angle through the first cross section
                        // in a loft operation
  double m_loftang2;    // Sets the draft angle through the second cross section
                        // in a loft operation
  double m_loftmag1;    // Sets the magnitude of the draft angle through the
                        // first cross section in a loft operation
  double m_loftmag2;    // Sets the magnitude of the draft angle through the
                        // second cross section in a loft operation
  double m_longitude;   // The longitude of the geographic location assigned to
                        // the drawing
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
  double m_northdirection; // Specifies the angle between the Y axis of WCS and
                        // the grid north
  int16_t m_obscolor;   // pecifies the color of obscured lines. An obscured
                        // line is a hidden line made visible by changing its
                        // color and line type and is visible only when the HIDE
                        // or SHADEMODE command is used. The OBSCUREDCOLOR
                        // setting is visible only if the OBSCUREDLTYPE is
                        // turned ON by setting it to a value other than 0
  int8_t m_obsltype;    // Specifies the line type of obscured lines. Obscured
                        // line types are independent of zoom level, unlike
                        // regular AutoCAD line types. Value 0 turns off display
                        // of obscured lines and is the default
  bool m_olestartup;    // Controls whether the source application of an
                        // embedded OLE object loads when plotting
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
  String m_projectname; // Assigns a project name to the current drawing. Used
                        // when an external reference or image is not found on
                        // its original path. The project name points to a
                        // section in the registry that can contain one or more
                        // search paths for each project name defined. Project
                        // names and their search directories are created from
                        // the Files tab of the Options dialog box.
  int16_t m_proxygraphics; // Controls the saving of proxy object images
  double m_psolheight;  // Controls the default height for a swept solid object
                        // created with the POLYSOLID command
  double m_psolwidth;   // Controls the default width for a swept solid object
                        // created with the POLYSOLID command
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
  bool m_realworldscale; // Drawing is scaled to the real world
  int16_t m_regenmode;  // REGENAUTO mode on if nonzero
  int16_t m_shadedge;   // 0 = Faces shaded, edges not highlighted
                        // 1 = Faces shaded, edges highlighted in black
                        // 2 = Faces not filled, edges in entity color
                        // 3 = Faces in entity color, edges in black
  int16_t m_shadedif;   // Percent ambient/diffuse light, range 1-100,
                        // default 70
  double m_sketchinc;   // Sketch record increment
  int16_t m_skpoly;     // 0 = Sketch lines, 1 = Sketch polylines
  bool m_snapmode;      // Snap mode on
  double m_snapunit[2]; // Snap grid X and Y spacing
  bool m_solidhist;     // Controls whether new composite solids retain a
  int8_t m_sortents;    // Controls the object sorting methods; accessible from
                        // the Options dialog box User Preferences tab
  int16_t m_splframe;   // Spline control polygon display: 1 = On, 0 = Off
  int16_t m_splinesegs; // Number of line segments per spline patch
  int16_t m_splinetype; // Spline curve type for PEDIT Spline
  double m_stepsize;    // Specifies the size of each step when in walk or fly
                        // mode, in drawing units
  double m_stepspersec; // Specifies the number of steps taken per second when
  String m_stylesheet;  // Path to the stylesheet for the drawing
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
  bool m_tilemodelightsynch; // Unknown
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
  double m_viewctr[2];  // XY center of current view on screen
  double m_viewdir[3];  // Viewing direction (direction from target in WCS)
  double m_viewsize;    // Height of view
  int16_t m_visretain;  // 0 = Don't retain xref-dependent visibility settings
                        // 1 = Retain xref-dependent visibility settings
  int16_t m_worldview;  // 1 = Set UCS to WCS during DVIEW/VPOINT,
                        // 0 = Don't change UCS
  bool m_xclipframe;    // Controls the visibility of xref clipping boundaries
  bool m_xedit;         // Controls whether the current drawing can be
                        // edited in-place when being referenced by another
                        // drawing.
                        // 0 = Can't use in-place reference editing
                        // 1 = Can use in-place reference editing
};

SGAL_END_NAMESPACE

#endif

#if 0
  m_3ddwfprec;          // Controls the precision of 3D DWF or 3D DWFx
                        // publishing
  u32 m_cmaterial;      //  Sets the material of new objects
  m_coords;             // Controls the display of coordinates
  m_cshadow;            // Shadow mode for a 3D object
  m_dgnframe;           // Determines whether DGN underlay frames are visible
                        // or plotted in the current drawing
  m_dimarcsym;          // Controls the display of the arc symbol in an arc
                        // length dimension.
  m_dimfit;             // Placement of text and arrowheads
  m_dimtfill;           // Controls the background of dimension tex
  m_dimtfillclr;        // Sets the color for the text background in dimensions
  m_dimtxtdirection;    // Specifies the reading direction of the dimension text
  m_dimunit;            // Units format for all dimension style family members
                        // except angular
  m_dragmode;           // Controls the way dragged objects are displayed
  m_dwfframe;           // Controls the way dragged objects are displayed
  m_interferecolor;     // Represents the ACI color index of the "interference
                        // objects" created during the interfere command
  u32 m_interfereobjvs; // Hard-pointer ID to the visual stype for interference
                        // objects
  u32 m_interferevpvs;  // Hard-pointer ID to the visual styoe for the viewport
                        // during interference checking
  m_loftnormals;        // Controls the normals of a lofted object where it
                        // passes through cross sections.
  i32 m_loftparam;      // Controls the shape of lofted solids and surfaces
  i32 m_osmode;         // Running object snap modes
  m_pickstyle;          // Controls the group selection and associative hatch
                        // selection
  m_requiredversions;   //  Unknown
  m_shadowplanelocation; // Location of the ground shadow plane.
                        // This is a Z axis ordinate
  m_showhist;           // Controls the Show History property for solids in a
                        // drawing
  m_snapbase;           // Snap/grid/base point (in UCS)
  m_snapisopair;        // Isometric plane
  m_snapstyle;          // Snap style
                        // history of their original components
                        // you are in walk or fly mode
  m_timezone;           // Sets the time zone for the sun in the drawing
#endif
