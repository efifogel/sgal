// Copyright (c) 2018 Israel.
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

#ifndef DXF_HEADER_HPP
#define DXF_HEADER_HPP

#include <cstdint>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"

DXF_BEGIN_NAMESPACE

struct SGAL_SGAL_DECL Dxf_header {
  int16_t m_acadmaintver; // Maintenance version number (should be ignored)
  SGAL::String m_acadver; // The AutoCAD drawing database version number:
                        // AC1006 = R10
                        // AC1009 = R11 and R12
                        // AC1012 = R13
                        // AC1014 = R14
                        // AC1015 = AutoCAD 2000
                        // AC1018 = AutoCAD 2004
                        // AC1021 = AutoCAD 2007
                        // AC1024 = AutoCAD 2010
                        // AC1027 = AutoCAD 2013
  double m_angbase;     // Angle 0 direction
  int16_t m_angdir;     // 1 = Clockwise angles; 0 = Counterclockwise angles
  int16_t m_attmode;    // Attribute visibility:
                        // 0 = None
                        // 1 = Normal
                        // 2 = All
  int16_t m_aunits;     // Units format for angles
  int16_t m_auprec;     // Units precision for angles
  int16_t m_cecolor;    // Current entity color number:
                        // 0 = BYBLOCK; 256 = BYLAYER
  double m_celtscale;   // Current entity linetype scale
  SGAL::String m_celtype; // Entity linetype name, or BYBLOCK or BYLAYER
  int8_t m_celweight;   // Lineweight of new objects
  SGAL::String m_cepsnid; // Plotstyle handle of new objects; if CEPSNTYPE is 3,
                        // then this value indicates the handle
  int8_t m_cepsntype;   // Plot style type of new objects:
                        // 0 = Plot style by layer
                        // 1 = Plot style by block
                        // 2 = Plot style by dictionary default
                        // 3 = Plot style by object ID/handle
  double m_chamfera;    // First chamfer distance
  double m_chamferb;    // Second chamfer distance
  double m_chamferc;    // Chamfer length
  double m_chamferd;    // Chamfer angle
  SGAL::String m_clayer; // Current layer name
  int16_t m_cmljust;    // Current multiline justification:
                        // 0 = Top; 1 = Middle; 2 = Bottom
  double m_cmlscale;    // Current multiline scale
  SGAL::String m_cmlstyle; // Current multiline style name
  int8_t m_cshadow;     // Shadow mode for a 3D object:
                        // 0 = Casts and receives shadows
                        // 1 = Casts shadows
                        // 2 = Receives shadows
                        // 3 = Ignores shadows
                        // Note: Starting with AutoCAD 2016-based products,
                        // this variable is obsolete but still supported for
                        // backwards compatibility.
  SGAL::String m_customproperty; //
  SGAL::String m_custompropertytag; //
  int16_t m_dimadec;    // Number of precision places displayed in angular
                        // dimensions
  int16_t m_dimalt;     // Alternate unit dimensioning performed if nonzero
  int16_t m_dimaltd;    // Alternate unit decimal places
  double m_dimaltf;     // Alternate unit scale factor
  double m_dimaltrnd;   // Determines rounding of alternate units
  int16_t m_dimalttd;   // Number of decimal places for tolerance values of an
                        // alternate units dimension
  int16_t m_dimalttz;   // Controls suppression of zeros for alternate
                        // tolerance values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
                        // To suppress leading or trailing zeros, add the
                        // following values to one of the preceding values:
                        // 4 = Suppresses leading zeros
                        // 8 = Suppresses trailing zeros
  int16_t m_dimaltu;    // Units format for alternate units of all dimension
                        // style family members except angular:
                        // 1 = Scientific
                        // 2 = Decimal
                        // 3 = Engineering
                        // 4 = Architectural (stacked)
                        // 5 = Fractional (stacked)
                        // 6 = Architectural
                        // 7 = Fractional
                        // 8 = Operating system defines the decimal separator
                        //     and number grouping symbols
  int16_t m_dimaltz;    // Controls suppression of zeros for alternate unit
                        // dimension values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
                        // 4 = Suppresses leading zeros in decimal dimensions
                        // 8 = Suppresses trailing zeros in decimal dimensions
                        // 12 = Suppresses both leading and trailing zeros
  SGAL::String m_dimapost; // Alternate dimensioning suffix
  int16_t m_dimaso;     // 1 = Create associative dimensioning
                        // 0 = Draw individual entities
                        // Note: Obsolete; see
  int8_t m_dimassoc;    // Controls the associativity of dimension objects
                        // 0 = Creates exploded dimensions; there is no
                        // association between elements of the dimension, and
                        // the lines, arcs, arrowheads, and text of a dimension
                        // are drawn as separate objects
                        // 1 = Creates non-associative dimension objects; the
                        //     elements of the dimension are formed into a
                        //     single object, and if the definition point on
                        //     the object moves, then the dimension value is
                        //     updated
                        // 2 = Creates associative dimension objects; the
                        //     elements of the dimension are formed into a
                        //     single object and one or more definition points
                        //     of the dimension are coupled with association
                        //     points on geometric objects
  double m_dimasz;      // Dimensioning arrow size
  int16_t m_dimatfit;   // Controls dimension text and arrow placement when
                        // space is not sufficient to place both within the
                        // extension lines:
                        // 0 = Places both text and arrows outside extension
                        //     lines
                        // 1 = Moves arrows first, then text
                        // 2 = Moves text first, then arrows
                        // 3 = Moves either text or arrows, whichever fits best
                        // AutoCAD adds a leader to moved dimension text when
                        // DIMTMOVE is set to 1
  int16_t m_dimaunit;   // Angle format for angular dimensions:
                        // 0 = Decimal degrees
                        // 1 = Degrees/minutes/seconds;
                        // 2 = Gradians
                        // 3 = Radians
                        // 4 = Surveyor's units
  int16_t m_dimazin;    // Controls suppression of zeros for angular dimensions:
                        // 0 = Displays all leading and trailing zeros
                        // 1 = Suppresses leading zeros in decimal dimensions
                        // 2 = Suppresses trailing zeros in decimal dimensions
                        // 3 = Suppresses leading and trailing zeros
  SGAL::String m_dimblk; // Arrow block name
  SGAL::String m_dimblk1; // First arrow block name
  SGAL::String m_dimblk2; // Second arrow block name
  double m_dimcen;      // Size of center mark/lines
  int16_t m_dimclrd;    // Dimension line color:
                        // range is 0 = BYBLOCK; 256 = BYLAYER
  int16_t m_dimclre;    // Dimension extension line color:
                        // range is 0 = BYBLOCK; 256 = BYLAYER
  int16_t m_dimclrt;    // Dimension text color:
                        // range is 0 = BYBLOCK; 256 = BYLAYER
  int16_t m_dimdec;     // Number of decimal places for the tolerance values of
                        // a primary units dimension
  double m_dimdle;      // Dimension line extension
  double m_dimdli;      // Dimension line increment
  int16_t m_dimdsep;    // Single-character decimal separator used when creating
                        // dimensions whose unit format is decimal
  double m_dimexe;      // Extension line extension
  double m_dimexo;      // Extension line offset
  double m_dimfac;      // Scale factor used to calculate the height of text for
                        // dimension fractions and tolerances. AutoCAD
                        // multiplies DIMTXT by DIMTFAC to set the fractional or
                        // tolerance text height.
  int16_t m_dimfrac;    // Sets the fraction format when DIMLUNIT is set to
                        // Architectural or Fractional.
  double m_dimgap;      // Dimension line gap
  int16_t m_dimjust;    // Horizontal dimension text position:
                        // 0 = Above dimension line and center-justified between
                        //     extension lines
                        // 1 = Above dimension line and next to first extension
                        //     line
                        // 2 = Above dimension line and next to second
                        //     extension line
                        // 3 = Above and center-justified to first extension line
                        // 4 = Above and center-justified to second extension
                        //     line
  SGAL::String m_dimldrblk; // Arrow block name for leaders
  double m_dimlfac;     // Linear measurements scale factor
  int16_t m_dimlim;     // Dimension limits generated if nonzero
  int16_t m_dimlunit;   // Sets units for all dimension types except Angular:
                        // 1 = Scientific
                        // 2 = Decimal
                        // 3 = Engineering
                        // 4 = Architectural
                        // 5 = Fractional
                        // 6 = Operating system
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
  SGAL::String m_dimpost; // General dimensioning suffix
  double m_dimrnd;      // Rounding value for dimension distances
  int16_t m_dimsah;     // Use separate arrow blocks if nonzero
  double m_dimscale;    // Overall dimensioning scale factor
  int16_t m_dimsd1;     // Suppression of first extension line:
                        // 0 = Not suppressed
                        // 1 = Suppressed
  int16_t m_dimsd2;     // Suppression of second extension line:
                        // 0 = Not suppressed
                        // 1 = Suppressed
  int16_t m_dimse1;     // First extension line suppressed if nonzero
  int16_t m_dimse2;     // Second extension line suppressed if nonzero
  int16_t m_dimsho;     // 1 = Recompute dimensions while dragging
                        // 0 = Drag original image
  int16_t m_dimsoxd;    // Suppress outside-extensions dimension lines if
                        // nonzero
  SGAL::String m_dimstyle; // Dimension style name
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
  int16_t m_dimtofl;    // If text is outside the extension lines, dimension
                        // lines are forced between the extension lines if
                        // nonzero
  int16_t m_dimtoh;     // Text outside horizontal if nonzero
  int16_t m_dimtol;     // Dimension tolerances generated if nonzero
  int16_t m_dimtolj;    // Vertical justification for tolerance values:
                        // 0 = Top
                        // 1 = Middle
                        // 2 = Bottom
  double m_dimtp;       // Plus tolerance
  double m_dimtsz;      // Dimensioning tick size:
                        // 0 = Draws arrowheads
                        // >0 = Draws oblique strokes instead of arrowheads
  double m_dimtvp;      // Text vertical position
  SGAL::String m_dimtxsty; // Dimension text style
  double m_dimtxt;      // Dimensioning text height
  int16_t m_dimtzin;    // Controls suppression of zeros for tolerance values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
                        // 4 = Suppresses leading zeros in decimal dimensions
                        // 8 = Suppresses trailing zeros in decimal dimensions
                        // 12 = Suppresses both leading and trailing zeros
  int16_t m_dimupt;     // Cursor functionality for user-positioned text:
                        // 0 = Controls only the dimension line location
                        // 1 = Controls the text position as well as the
                        //     dimension line location
  int16_t m_dimzin;     // Controls suppression of zeros for primary unit values:
                        // 0 = Suppresses zero feet and precisely zero inches
                        // 1 = Includes zero feet and precisely zero inches
                        // 2 = Includes zero feet and suppresses zero inches
                        // 3 = Includes zero inches and suppresses zero feet
                        // 4 = Suppresses leading zeros in decimal dimensions
                        // 8 = Suppresses trailing zeros in decimal dimensions
                        // 12 = Suppresses both leading and trailing zeros
  int16_t m_dispsilh;   // Controls the display of silhouette curves of body
                        // objects in Wireframe mode:
                        // 0 = Off
                        // 1 = On
  SGAL::String m_dragvs; // Hard-pointer ID to visual style while creating 3D
                        // solid primitives. The default value is NULL
  SGAL::String m_dwgcodepage; // Drawing code page; set to the system code page
                        //  when a new drawing is created, but not otherwise
                        // maintained by AutoCAD
  double m_elevation;   // Current elevation set by ELEV command
  int8_t m_endcaps;     // Lineweight endcaps setting for new objects:
                        // 0 = None
                        // 1 = Round
                        // 2 = Angle
                        // 3 = Square
  double m_extmax[3];   // X, Y, and Z drawing extents upper-right corner (in
                        // WCS)
  double m_extmin[3];   // X, Y, and Z drawing extents lower-left corner (in WCS)
  bool m_extnames;      // Controls symbol table naming:
                        // 0 = Release 14 compatibility. Limits names to 31
                        //     characters in length. Names can include the
                        //     letters A to Z, the numerals 0 to 9, and the
                        //     special characters dollar sign ($), underscore
                        //     (_), and hyphen (-).
                        // 1 = AutoCAD 2000. Names can be up to 255 characters
                        //     in length, and can include the letters A to Z,
                        //     the numerals 0 to 9, spaces, and any special
                        //     characters not used for other purposes by
                        //     Microsoft Windows and AutoCAD
  double m_filletrad;   // Fillet radius
  int16_t m_fillmode;   // Fill mode on if nonzero
  SGAL::String m_fingerprintguid; // Set at creation time, uniquely identifies a
                        // particular drawing
  int8_t m_halogap;     // Specifies a gap to be displayed where an object is
                        // hidden by another object; the value is specified as
                        // a percent of one unit and is independent of the zoom
                        // level. A haloed line is shortened at the point where
                        // it is hidden when HIDE or the Hidden option of
                        // SHADEMODE is used
  SGAL::String m_handseed;    // Next available handle
  int8_t m_hidetext;    // Specifies HIDETEXT system variable:
                        // 0 = HIDE ignores text objects when producing the
                        //     hidden view
                        // 1 = HIDE does not ignore text objects
  SGAL::String m_hyperlinkbase; // Path for all relative hyperlinks in the
                        // drawing. If null, the drawing path is used
 int8_t m_indexctl;     // Controls whether layer and spatial indexes are
                        // created and saved in drawing files:
                        // 0 = No indexes are created
                        // 1 = Layer index is created
                        // 2 = Spatial index is created
                        // 3 = Layer and spatial indexes are created
  double m_insbase[3];  // Insertion base set by BASE command (in WCS)
  int16_t m_insunits;   // Default drawing units for AutoCAD DesignCenter blocks:
                        // 0 = Unitless
                        // 1 = Inches
                        // 2 = Feet
                        // 3 = Miles
                        // 4 = Millimeters
                        // 5 = Centimeters
                        // 6 = Meters
                        // 7 = Kilometers
                        // 8 = Microinches
                        // 9 = Mils
                        // 10 = Yards
                        // 11 = Angstroms
                        // 12 = Nanometers
                        // 13 = Microns
                        // 14 = Decimeters
                        // 15 = Decameters
                        // 16 = Hectometers
                        // 17 = Gigameters
                        // 18 = Astronomical units
                        // 19 = Light years
                        // 20 = Parsecs
                        // 21 = US Survey Feet
                        // 22 = US Survey Inch
                        // 23 = US Survey Yard
                        // 24 = US Survey Mile
  int16_t m_interferecolor; // Represents the ACI color index of the
                        // "interference objects" created during the INTERFERE
                        // command. Default value is 1
  SGAL::String m_interfereobjvs; // Hard-pointer ID to the visual style for
                        // interference objects. Default visual style is
                        // Conceptual.
  SGAL::String m_interferevpvs; // Hard-pointer ID to the visual style for the
                        // viewport during interference checking. Default visual
                        // style is 3d Wireframe.
  int16_t m_intersectioncolor; // Specifies the entity color of intersection
                        // polylines:
                        // Values 1-255 designate an AutoCAD color index (ACI)
                        // 0 = Color BYBLOCK
                        // 256 = Color BYLAYER
                        // 257 = Color BYENTITY
  int8_t m_intersectiondisplay; // Specifies the display of intersection
                        // polylines:
                        // 0 = Turns off the display of intersection polylines
                        // 1 = Turns on the display of intersection polylines
  int8_t m_joinstyle;   // Lineweight joint setting for new objects:
                        // 0=None
                        // 1= Round
                        // 2 = Angle
                        // 3 = Flat
  SGAL::String m_last_saved_by;
  int16_t m_limcheck;   // Nonzero if limits checking is on
  double m_limmax[2];   // XY drawing limits upper-right corner (in WCS)
  double m_limmin[2];   // XY drawing limits lower-left corner (in WCS)
  double m_ltscale;     // Global linetype scale
  int16_t m_lunits;     // Units format for coordinates and distances
  int16_t m_luprec;     // Units precision for coordinates and distances
  bool m_lwdisplay;     // Controls the display of lineweights on the Model or
                        // Layout tab:
                        // 0 = Lineweight is not displayed
                        // 1 = Lineweight is displayed
  int16_t m_maxactvp;   // Sets maximum number of viewports to be regenerated
  int16_t m_measurement; // Sets drawing units:
                        // 0 = English
                        // 1 = Metric
  SGAL::String m_menu;  // Name of menu file
  int16_t m_mirrtext;   // Mirror text if nonzero
  int16_t m_obscolor;   // Specifies the color of obscured lines. An obscured
                        // line is a hidden line made visible by changing its
                        // color and linetype and is visible only when the HIDE
                        // or SHADEMODE command is used. The OBSCUREDCOLOR
                        // setting is visible only if the OBSCUREDLTYPE is
                        // turned ON by setting it to a value other than 0.
                        // 0 and 256 = Entity color
                        // 1-255 = An AutoCAD color index (ACI)
  int8_t m_obsltype;    // Specifies the linetype of obscured lines. Obscured
                        // linetypes are independent of zoom level, unlike
                        // regular AutoCAD linetypes. Value 0 turns off display
                        // of obscured lines and is the default. Linetype values
                        // are defined as follows:
                        // 0 = Off
                        // 1 = Solid
                        // 2 = Dashed
                        // 3 = Dotted
                        // 4 = Short Dash
                        // 5 = Medium Dash
                        // 6 = Long Dash
                        // 7 = Double Short Dash
                        // 8 = Double Medium Dash
                        // 9 = Double Long Dash
                        // 10 = Medium Long Dash
                        // 11 = Sparse Dot
  bool m_olestartup;    // Controls whether the source application of an
                        // embedded OLE object loads when plotting
  int16_t m_orthomode;  // Ortho mode on if nonzero
  int16_t m_pdmode;     // Point display mode
  double m_pdsize;      // Point display size
  double m_pelevation;  // Current paper space elevation
  double m_pextmax[3];  // Maximum X, Y, and Z extents for paper space
  double m_pextmin[3];  // Minimum X, Y, and Z extents for paper space
  double m_pinsbase[3]; // Paper space insertion base point
  int16_t m_plimcheck;  // Limits checking in paper space when nonzero
  double m_plimmax[2];  // Maximum X and Y limits in paper space
  double m_plimmin[2];  // Minimum X and Y limits in paper space
  int16_t m_plinegen;   // Governs the generation of linetype patterns around
                        // the vertices of a 2D polyline:
                        // 1 = Linetype is generated in a continuous pattern
                        // around vertices of the polyline
                        // 0 = Each segment of the polyline starts and ends
                        // with a dash
  double m_plinewid;    // Default polyline width
  SGAL::String m_projectname; // Assigns a project name to the current drawing.
                        // Used when an external reference or image is not found
                        // on its original path. The project name points to a
                        // section in the registry that can contain one or more
                        // search paths for each project name defined. Project
                        // names and their search directories are created from
                        // the Files tab of the Options dialog box
  int16_t m_proxygraphics; // Controls the saving of proxy object images
  int16_t m_psltscale;  // Controls paper space linetype scaling:
                        // 1 = No special linetype scaling
                        // 0 = Viewport scaling governs linetype scaling
  bool m_pstylemode;    // Indicates whether the current drawing is in a
                        // Color-Dependent or Named Plot Style mode:
                        // 0 = Uses named plot style tables in the current
                        //     drawing
                        // 1 = Uses color-dependent plot style tables in the
                        //     current drawing
  double m_psvpscale;   // View scale factor for new viewports:
                        // 0 = Scaled to fit
                        // >0 = Scale factor (a positive real value)
  SGAL::String m_pucsbase; // Name of the UCS that defines the origin and
                        // orientation of orthographic UCS settings (paper
                        // space only)
  SGAL::String m_pucsname; // Current paper space UCS name
  double m_pucsorg[3];  // Current paper space UCS origin
  double m_pucsorgback[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to BACK when PUCSBASE is
                        // set to WORLD
  double m_pucsorgbottom[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to BOTTOM when PUCSBASE is
                        // set to WORLD
  double m_pucsorgfront[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to FRONT when PUCSBASE is
                        // set to WORLD
  double m_pucsorgleft[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to LEFT when PUCSBASE is
                        // set to WORLD
  double m_pucsorgright[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to RIGHT when PUCSBASE is
                        // set to WORLD
  double m_pucsorgtop[3]; // Point which becomes the new UCS origin after
                        // changing paper space UCS to TOP when PUCSBASE is set
                        // to WORLD
  SGAL::String m_pucsorthoref; // If paper space UCS is orthographic
                        // (PUCSORTHOVIEW not equal to 0), this is the name of
                        // tthe UCS that he orthographic UCS is relative to. If
                        // blank, UCS is relative to WORLD
  int16_t m_pucsorthoview; // Orthographic view type of paper space UCS:
                        // 0 = UCS is not orthographic
                        // 1 = Top
                        // 2 = Bottom
                        // 3 = Front
                        // 4 = Back
                        // 5 = Left
                        // 6 = Right
  double m_pucsxdir[3]; // Current paper space UCS X axis
  double m_pucsydir[3]; // Current paper space UCS Y axis
  int16_t m_qtextmode;  // Quick Text mode on if nonzero
  int16_t m_regenmode;  // REGENAUTO mode on if nonzero
  int16_t m_shadedge;   // Controls the shading of edges:
                        // 0 = Faces shaded, edges not highlighted
                        // 1 = Faces shaded, edges highlighted in black
                        // 2 = Faces not filled, edges in entity color
                        // 3 = Faces in entity color, edges in black
  int16_t m_shadedif;   // Percent ambient/diffuse light; range 1-100; default 70
  double m_shadowplanelocation; // Location of the ground shadow plane. This is
                        // a Z axis ordinate.
  double m_sketchinc;   // Sketch record increment
  int16_t m_skpoly;     // Determines the object type created by the SKETCH
                        // command:
                        // 0 = Generates lines
                        // 1 = Generates polylines
                        // 2 = Generates splines
  int8_t m_sortents;    // Controls the object sorting methods; accessible from
                        // the Options dialog box User Preferences tab.
                        // SORTENTS uses the following bitcodes:
                        // 0 = Disables SORTENTS
                        // 1 = Sorts for object selection
                        // 2 = Sorts for object snap
                        // 4 = Sorts for redraws; obsolete
                        // 8 = Sorts for MSLIDE command slide creation; obsolete
                        // 16 = Sorts for REGEN commands
                        // 32 = Sorts for plotting
                        // 64 = Sorts for PostScript output; obsolete
  int16_t m_splframe;   // Spline control polygon display: 1 = On, 0 = Off
  int16_t m_splinesegs; // Number of line segments per spline patch
  int16_t m_splinetype; // Spline curve type for PEDIT Spline
  SGAL::String m_stylesheet; // Path to the stylesheet for the drawing
  int16_t m_surftab1;   // Number of mesh tabulations in first direction
  int16_t m_surftab2;   // Number of mesh tabulations in second direction
  int16_t m_surftype;   // Surface type for PEDIT Smooth
  int16_t m_surfu;      // Surface density (for PEDIT Smooth) in M direction
  int16_t m_surfv;      // Surface density (for PEDIT Smooth) in N direction
  double m_tdcreate;    // Local date/time of drawing creation (see Special
                        // Handling of Date/Time Variables)
  double m_tdindwg;     // Cumulative editing time for this drawing (see Special
                        // Handling of Date/Time Variables)
  double m_tducreate;   // Universal date/time the drawing was created (see
                        // Special Handling of Date/Time Variables)
  double m_tdupdate;    // Local date/time of last drawing update (see Special
                        // Handling of Date/Time Variables)
  double m_tdusrtimer;  // User-elapsed timer
  double m_tduupdate;   // Universal date/time of the last update/save (see
                        // Special Handling of Date/Time Variables)
  double m_textsize;    // Default text height
  SGAL::String m_textstyle; // Current text style name
  double m_thickness;   // Current thickness set by ELEV command
  int16_t m_tilemode;   // 1 for previous release compatibility mode;
                        // 0 otherwise
  double m_tracewid;    // Default trace width
  int16_t m_treedepth;  // Specifies the maximum depth of the spatial index
  SGAL::String m_ucsbase; // Name of the UCS that defines the origin and
                        // orientation of orthographic UCS settings
  SGAL::String m_ucsname; // Name of current UCS
  double m_ucsorg[3];   // Origin of current UCS (in WCS)
  double m_ucsorgback[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to BACK when UCSBASE is set
                        // to WORLD
  double m_ucsorgbottom[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to BOTTOM when UCSBASE is
                        // set to WORLD
  double m_ucsorgfront[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to FRONT when UCSBASE is
                        // set to WORLD
  double m_ucsorgleft[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to LEFT when UCSBASE is set
                        // to WORLD
  double m_ucsorgright[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to RIGHT when UCSBASE is set
                        // to WORLD
  double m_ucsorgtop[3]; // Point which becomes the new UCS origin after
                        // changing model space UCS to TOP when UCSBASE is set
                        // to WORLD
  SGAL::String m_ucsorthoref; // If model space UCS is orthographic
                        // (UCSORTHOVIEW not equal to 0), this is the name of
                        // the UCS that the orthographic UCS is relative to. If
                        // blank, UCS is relative to WORLD
  int16_t m_ucsorthoview; // Orthographic view type of model space UCS:
                        // 0 = UCS is not orthographic
                        // 1 = Top
                        // 2 = Bottom
                        // 3 = Front
                        // 4 = Back
                        // 5 = Left
                        // 6 = Right
  double m_ucsxdir[3];  // Direction of the current UCS X axis (in WCS)
  double m_ucsydir[3];  // Direction of the current UCS Y axis (in WCS)
  int16_t m_unitmode;   // Low bit set = Display fractions, feet-and-inches,
                        // and surveyor's angles in input format
  int16_t m_useri1;     // Used by third-party developers
  int16_t m_useri2;     // Used by third-party developers
  int16_t m_useri3;     // Used by third-party developers
  int16_t m_useri4;     // Used by third-party developers
  int16_t m_useri5;     // Used by third-party developers
  double m_userr1;      // Used by third-party developers
  double m_userr2;      // Used by third-party developers
  double m_userr3;      // Used by third-party developers
  double m_userr4;      // Used by third-party developers
  double m_userr5;      // Used by third-party developers
  int16_t m_usrtimer;   // Controls the user timer for the drawing:
                        // 0 = Timer off
                        // 1 = Timer on
  SGAL::String m_versionguid; // Uniquely identifies a particular version of a
                        // drawing. Updated when the drawing is modified
  int16_t m_visretain;  // Controls the properties of xref-dependent layers:
                        // 0 = Don't retain xref-dependent visibility settings
                        // 1 = Retain xref-dependent visibility settings
  int16_t m_worldview;  // Determines whether input for the DVIEW and VPOINT
                        // command evaluated as relative to the WCS or current
                        // UCS:
                        // 0 = Don't change UCS
                        // 1 = Set UCS to WCS during DVIEW/VPOINT
  bool m_xclipframe;    // Controls the visibility of xref clipping boundaries:
                        // 0 = Clipping boundary is not visible
                        // 1 = Clipping boundary is visible
  bool m_xedit;         // Controls whether the current drawing can be edited
                        // in-place when being referenced by another drawing:
                        // 0 = Can't use in-place reference editing
                        // 1 = Can use in-place reference editing
};

DXF_END_NAMESPACE

#endif
