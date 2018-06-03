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

#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Dxf_parser.hpp"

SGAL_BEGIN_NAMESPACE

/*! This file contains the description of DXF header variables. In
 * particular, it consists of the initialization of a mapping from variable
 * names to structs that contains information about the the variables. The
 * domain struct consists of the handle to the member in the header struct and
 * a list of codes that precede the values of the variable, respectively.
 * The DXF header members listed below are extracted from
 *   http://help.autodesk.com/view/ACD/2017/ENU/?guid=GUID-A85E8E67-27CD-4C59-BE61-4DC9FADBE74A
 * The page is entitled "Autodesk Autocad 2017", which may lead the belief that
 * the list is complete. However, some variables had to be added.
 * Additional variables where taken from
 *  https://docs.rs/dxf/0.2.0/dxf/struct.Header.html
 *
 * Also, some of the codes listed in the Autodesk pahe where erroneous (i.e.,
 * HIDETEXT, INTERSECTIONDISPLAY).
 */

const std::map<String, Dxf_parser::Header_member>
Dxf_parser::s_header_members = {
  {"ACADMAINTVER",    {&Dxf_header::m_acadmaintver, {70}}},
  {"ACADVER",         {&Dxf_header::m_acadver, {1}}},
  {"ANGBASE",         {&Dxf_header::m_angbase, {50}}},
  {"ANGDIR",          {&Dxf_header::m_angdir, {70}}},
  {"ATTMODE",         {&Dxf_header::m_attmode, {70}}},
  {"AUNITS",          {&Dxf_header::m_aunits, {70}}},
  {"AUPREC",          {&Dxf_header::m_auprec, {70}}},
  {"CECOLOR",         {&Dxf_header::m_cecolor, {62}}},
  {"CELTSCALE",       {&Dxf_header::m_celtscale, {40}}},
  {"CELTYPE",         {&Dxf_header::m_celtype, {6}}},
  {"CELWEIGHT",       {&Dxf_header::m_celweight, {370}}},
  {"CEPSNID",         {&Dxf_header::m_cepsnid, {390}}},
  {"CEPSNTYPE",       {&Dxf_header::m_cepsntype, {380}}},
  {"CHAMFERA",        {&Dxf_header::m_chamfera, {40}}},
  {"CHAMFERB",        {&Dxf_header::m_chamferb, {40}}},
  {"CHAMFERC",        {&Dxf_header::m_chamferc, {40}}},
  {"CHAMFERD",        {&Dxf_header::m_chamferd, {40}}},
  {"CLAYER",          {&Dxf_header::m_clayer, {8}}},
  {"CMLJUST",         {&Dxf_header::m_cmljust, {70}}},
  {"CMLSCALE",        {&Dxf_header::m_cmlscale, {40}}},
  {"CMLSTYLE",        {&Dxf_header::m_cmlstyle, {2}}},
  {"CSHADOW",         {&Dxf_header::m_cshadow, {280}}},
  {"DIMADEC",         {&Dxf_header::m_dimadec, {70}}},
  {"CUSTOMPROPERTY",  {&Dxf_header::m_customproperty, {1}}},
  {"CUSTOMPROPERTYTAG",{&Dxf_header::m_custompropertytag, {1}}},
  {"DIMALT",          {&Dxf_header::m_dimalt, {70}}},
  {"DIMALTD",         {&Dxf_header::m_dimaltd, {70}}},
  {"DIMALTF",         {&Dxf_header::m_dimaltf, {40}}},
  {"DIMALTRND",       {&Dxf_header::m_dimaltrnd, {40}}},
  {"DIMALTTD",        {&Dxf_header::m_dimalttd, {70}}},
  {"DIMALTTZ",        {&Dxf_header::m_dimalttz, {70}}},
  {"DIMALTU",         {&Dxf_header::m_dimaltu, {70}}},
  {"DIMALTZ",         {&Dxf_header::m_dimaltz, {70}}},
  {"DIMAPOST",        {&Dxf_header::m_dimapost, {1}}},
  {"DIMASO",          {&Dxf_header::m_dimaso, {70}}}, // Obsolete; see DIMASSOC.
  {"DIMASSOC",        {&Dxf_header::m_dimassoc, {280}}},
  {"DIMASZ",          {&Dxf_header::m_dimasz, {40}}},
  {"DIMATFIT",        {&Dxf_header::m_dimatfit, {70}}},
  {"DIMAUNIT",        {&Dxf_header::m_dimaunit, {70}}},
  {"DIMAZIN",         {&Dxf_header::m_dimazin, {70}}},
  {"DIMBLK",          {&Dxf_header::m_dimblk, {1}}},
  {"DIMBLK1",         {&Dxf_header::m_dimblk1, {1}}},
  {"DIMBLK2",         {&Dxf_header::m_dimblk2, {1}}},
  {"DIMCEN",          {&Dxf_header::m_dimcen, {40}}},
  {"DIMCLRD",         {&Dxf_header::m_dimclrd, {70}}},
  {"DIMCLRE",         {&Dxf_header::m_dimclre, {70}}},
  {"DIMCLRT",         {&Dxf_header::m_dimclrt, {70}}},
  {"DIMDEC",          {&Dxf_header::m_dimdec, {70}}},
  {"DIMDLE",          {&Dxf_header::m_dimdle, {40}}},
  {"DIMDLI",          {&Dxf_header::m_dimdli, {40}}},
  {"DIMDSEP",         {&Dxf_header::m_dimdsep, {70}}},
  {"DIMEXE",          {&Dxf_header::m_dimexe, {40}}},
  {"DIMEXO",          {&Dxf_header::m_dimexo, {40}}},
  {"DIMFAC",          {&Dxf_header::m_dimfac, {40}}},
  {"DIMFRAC",         {&Dxf_header::m_dimfrac, {70}}},
  {"DIMGAP",          {&Dxf_header::m_dimgap, {40}}},
  {"DIMJUST",         {&Dxf_header::m_dimjust, {70}}},
  {"DIMLDRBLK",       {&Dxf_header::m_dimldrblk, {1}}},
  {"DIMLFAC",         {&Dxf_header::m_dimlfac, {40}}},
  {"DIMLIM",          {&Dxf_header::m_dimlim, {70}}},
  {"DIMLUNIT",        {&Dxf_header::m_dimlunit, {70}}},
  {"DIMLWD",          {&Dxf_header::m_dimlwd, {70}}},
  {"DIMLWE",          {&Dxf_header::m_dimlwe, {70}}},
  {"DIMPOST",         {&Dxf_header::m_dimpost, {1}}},
  {"DIMRND",          {&Dxf_header::m_dimrnd, {40}}},
  {"DIMSAH",          {&Dxf_header::m_dimsah, {70}}},
  {"DIMSCALE",        {&Dxf_header::m_dimscale, {40}}},
  {"DIMSD1",          {&Dxf_header::m_dimsd1, {70}}},
  {"DIMSD2",          {&Dxf_header::m_dimsd2, {70}}},
  {"DIMSE1",          {&Dxf_header::m_dimse1, {70}}},
  {"DIMSE2",          {&Dxf_header::m_dimse2, {70}}},
  {"DIMSHO",          {&Dxf_header::m_dimsho, {70}}},
  {"DIMSOXD",         {&Dxf_header::m_dimsoxd, {70}}},
  {"DIMSTYLE",        {&Dxf_header::m_dimstyle, {2}}},
  {"DIMTAD",          {&Dxf_header::m_dimtad, {70}}},
  {"DIMTDEC",         {&Dxf_header::m_dimtdec, {70}}},
  {"DIMTFAC",         {&Dxf_header::m_dimtfac, {40}}},
  {"DIMTIH",          {&Dxf_header::m_dimtih, {70}}},
  {"DIMTIX",          {&Dxf_header::m_dimtix, {70}}},
  {"DIMTM",           {&Dxf_header::m_dimtm, {40}}},
  {"DIMTMOVE",        {&Dxf_header::m_dimtmove, {70}}},
  {"DIMTOFL",         {&Dxf_header::m_dimtofl, {70}}},
  {"DIMTOH",          {&Dxf_header::m_dimtoh, {70}}},
  {"DIMTOL",          {&Dxf_header::m_dimtol, {70}}},
  {"DIMTOLJ",         {&Dxf_header::m_dimtolj, {70}}},
  {"DIMTP",           {&Dxf_header::m_dimtp, {40}}},
  {"DIMTSZ",          {&Dxf_header::m_dimtsz, {40}}},
  {"DIMTVP",          {&Dxf_header::m_dimtvp, {40}}},
  {"DIMTXSTY",        {&Dxf_header::m_dimtxsty, {7}}},
  {"DIMTXT",          {&Dxf_header::m_dimtxt, {40}}},
  {"DIMTZIN",         {&Dxf_header::m_dimtzin, {70}}},
  {"DIMUPT",          {&Dxf_header::m_dimupt, {70}}},
  {"DIMZIN",          {&Dxf_header::m_dimzin, {70}}},
  {"DISPSILH",        {&Dxf_header::m_dispsilh, {70}}},
  {"DRAGVS",          {&Dxf_header::m_dragvs, {349}}},
  {"DWGCODEPAGE",     {&Dxf_header::m_dwgcodepage, {3}}},
  {"ELEVATION",       {&Dxf_header::m_elevation, {40}}},
  {"ENDCAPS",         {&Dxf_header::m_endcaps, {280}}},
  {"EXTMAX",          {&Dxf_header::m_extmax, {10, 20, 30}}},
  {"EXTMIN",          {&Dxf_header::m_extmin, {10, 20, 30}}},
  {"EXTNAMES",        {&Dxf_header::m_extnames, {290}}},
  {"FILLETRAD",       {&Dxf_header::m_filletrad, {40}}},
  {"FILLMODE",        {&Dxf_header::m_fillmode, {70}}},
  {"FINGERPRINTGUID", {&Dxf_header::m_fingerprintguid, {2}}},
  {"HALOGAP",         {&Dxf_header::m_halogap, {280}}},
  {"HANDSEED",        {&Dxf_header::m_handseed, {5}}},
  {"HIDETEXT",        {&Dxf_header::m_hidetext, {280}}},
  {"HYPERLINKBASE",   {&Dxf_header::m_hyperlinkbase, {1}}},
  {"INDEXCTL",        {&Dxf_header::m_indexctl, {280}}},
  {"INSBASE",         {&Dxf_header::m_insbase, {10, 20, 30}}},
  {"INSUNITS",        {&Dxf_header::m_insunits, {70}}},
  {"INTERFERECOLOR",  {&Dxf_header::m_interferecolor, {62}}},
  {"INTERFEREOBJVS",  {&Dxf_header::m_interfereobjvs, {345}}},
  {"INTERFEREVPVS",   {&Dxf_header::m_interferevpvs, {346}}},
  {"INTERSECTIONCOLOR", {&Dxf_header::m_intersectioncolor, {70}}},
  {"INTERSECTIONDISPLAY", {&Dxf_header::m_intersectiondisplay, {280}}},
  {"JOINSTYLE",       {&Dxf_header::m_joinstyle, {280}}},
  {"LASTSAVEDBY",     {&Dxf_header::m_last_saved_by, {1}}},
  {"LIMCHECK",        {&Dxf_header::m_limcheck, {70}}},
  {"LIMMAX",          {&Dxf_header::m_limmax, {10, 20}}},
  {"LIMMIN",          {&Dxf_header::m_limmin, {10, 20}}},
  {"LTSCALE",         {&Dxf_header::m_ltscale, {40}}},
  {"LUNITS",          {&Dxf_header::m_lunits, {70}}},
  {"LUPREC",          {&Dxf_header::m_luprec, {70}}},
  {"LWDISPLAY",       {&Dxf_header::m_lwdisplay, {290}}},
  {"MAXACTVP",        {&Dxf_header::m_maxactvp, {70}}},
  {"MEASUREMENT",     {&Dxf_header::m_measurement, {70}}},
  {"MENU",            {&Dxf_header::m_menu, {1}}},
  {"MIRRTEXT",        {&Dxf_header::m_mirrtext, {70}}},
  {"OBSCOLOR",        {&Dxf_header::m_obscolor, {70}}},
  {"OBSLTYPE",        {&Dxf_header::m_obsltype, {280}}},
  {"OLESTARTUP",      {&Dxf_header::m_olestartup, {290}}},
  {"ORTHOMODE",       {&Dxf_header::m_orthomode, {70}}},
  {"PDMODE",          {&Dxf_header::m_pdmode, {70}}},
  {"PDSIZE",          {&Dxf_header::m_pdsize, {40}}},
  {"PELEVATION",      {&Dxf_header::m_pelevation, {40}}},
  {"PEXTMAX",         {&Dxf_header::m_pextmax, {10, 20, 30}}},
  {"PEXTMIN",         {&Dxf_header::m_pextmin, {10, 20, 30}}},
  {"PINSBASE",        {&Dxf_header::m_pinsbase, {10, 20, 30}}},
  {"PLIMCHECK",       {&Dxf_header::m_plimcheck, {70}}},
  {"PLIMMAX",         {&Dxf_header::m_plimmax, {10, 20}}},
  {"PLIMMIN",         {&Dxf_header::m_plimmin, {10, 20}}},
  {"PLINEGEN",        {&Dxf_header::m_plinegen, {70}}},
  {"PLINEWID",        {&Dxf_header::m_plinewid, {40}}},
  {"PROJECTNAME",     {&Dxf_header::m_projectname, {1}}},
  {"PROXYGRAPHICS",   {&Dxf_header::m_proxygraphics, {70}}},
  {"PSLTSCALE",       {&Dxf_header::m_psltscale, {70}}},
  {"PSTYLEMODE",      {&Dxf_header::m_pstylemode, {290}}},
  {"PSVPSCALE",       {&Dxf_header::m_psvpscale, {40}}},
  {"PUCSBASE",        {&Dxf_header::m_pucsbase, {2}}},
  {"PUCSNAME",        {&Dxf_header::m_pucsname, {2}}},
  {"PUCSORG",         {&Dxf_header::m_pucsorg, {10, 20, 30}}},
  {"PUCSORGBACK",     {&Dxf_header::m_pucsorgback, {10, 20, 30}}},
  {"PUCSORGBOTTOM",   {&Dxf_header::m_pucsorgbottom, {10, 20, 30}}},
  {"PUCSORGFRONT",    {&Dxf_header::m_pucsorgfront, {10, 20, 30}}},
  {"PUCSORGLEFT",     {&Dxf_header::m_pucsorgleft, {10, 20, 30}}},
  {"PUCSORGRIGHT",    {&Dxf_header::m_pucsorgright, {10, 20, 30}}},
  {"PUCSORGTOP",      {&Dxf_header::m_pucsorgtop, {10, 20, 30}}},
  {"PUCSORTHOREF",    {&Dxf_header::m_pucsorthoref, {2}}},
  {"PUCSORTHOVIEW",   {&Dxf_header::m_pucsorthoview, {70}}},
  {"PUCSXDIR",        {&Dxf_header::m_pucsxdir, {10, 20, 30}}},
  {"PUCSYDIR",        {&Dxf_header::m_pucsydir, {10, 20, 30}}},
  {"QTEXTMODE",       {&Dxf_header::m_qtextmode, {70}}},
  {"REGENMODE",       {&Dxf_header::m_regenmode, {70}}},
  {"SHADEDGE",        {&Dxf_header::m_shadedge, {70}}},
  {"SHADEDIF",        {&Dxf_header::m_shadedif, {70}}},
  {"SHADOWPLANELOCATION", {&Dxf_header::m_shadowplanelocation, {40}}},
  {"SKETCHINC",       {&Dxf_header::m_sketchinc, {40}}},
  {"SKPOLY",          {&Dxf_header::m_skpoly, {70}}},
  {"SORTENTS",        {&Dxf_header::m_sortents, {280}}},
  {"SPLFRAME",        {&Dxf_header::m_splframe, {70}}},
  {"SPLINESEGS",      {&Dxf_header::m_splinesegs, {70}}},
  {"SPLINETYPE",      {&Dxf_header::m_splinetype, {70}}},
  {"STYLESHEET",      {&Dxf_header::m_stylesheet, {1}}},
  {"SURFTAB1",        {&Dxf_header::m_surftab1, {70}}},
  {"SURFTAB2",        {&Dxf_header::m_surftab2, {70}}},
  {"SURFTYPE",        {&Dxf_header::m_surftype, {70}}},
  {"SURFU",           {&Dxf_header::m_surfu, {70}}},
  {"SURFV",           {&Dxf_header::m_surfv, {70}}},
  {"TDCREATE",        {&Dxf_header::m_tdcreate, {40}}},
  {"TDINDWG",         {&Dxf_header::m_tdindwg, {40}}},
  {"TDUCREATE",       {&Dxf_header::m_tducreate, {40}}},
  {"TDUPDATE",        {&Dxf_header::m_tdupdate, {40}}},
  {"TDUSRTIMER",      {&Dxf_header::m_tdusrtimer, {40}}},
  {"TDUUPDATE",       {&Dxf_header::m_tduupdate, {40}}},
  {"TEXTSIZE",        {&Dxf_header::m_textsize, {40}}},
  {"TEXTSTYLE",       {&Dxf_header::m_textstyle, {7}}},
  {"THICKNESS",       {&Dxf_header::m_thickness, {40}}},
  {"TILEMODE",        {&Dxf_header::m_tilemode, {70}}},
  {"TRACEWID",        {&Dxf_header::m_tracewid, {40}}},
  {"TREEDEPTH",       {&Dxf_header::m_treedepth, {70}}},
  {"UCSBASE",         {&Dxf_header::m_ucsbase, {2}}},
  {"UCSNAME",         {&Dxf_header::m_ucsname, {2}}},
  {"UCSORG",          {&Dxf_header::m_ucsorg, {10, 20, 30}}},
  {"UCSORGBACK",      {&Dxf_header::m_ucsorgback, {10, 20, 30}}},
  {"UCSORGBOTTOM",    {&Dxf_header::m_ucsorgbottom, {10, 20, 30}}},
  {"UCSORGFRONT",     {&Dxf_header::m_ucsorgfront, {10, 20, 30}}},
  {"UCSORGLEFT",      {&Dxf_header::m_ucsorgleft, {10, 20, 30}}},
  {"UCSORGRIGHT",     {&Dxf_header::m_ucsorgright, {10, 20, 30}}},
  {"UCSORGTOP",       {&Dxf_header::m_ucsorgtop, {10, 20, 30}}},
  {"UCSORTHOREF",     {&Dxf_header::m_ucsorthoref, {2}}},
  {"UCSORTHOVIEW",    {&Dxf_header::m_ucsorthoview, {70}}},
  {"UCSXDIR",         {&Dxf_header::m_ucsxdir, {10, 20, 30}}},
  {"UCSYDIR",         {&Dxf_header::m_ucsydir, {10, 20, 30}}},
  {"UNITMODE",        {&Dxf_header::m_unitmode, {70}}},
  {"USERI1",          {&Dxf_header::m_useri1, {70}}},
  {"USERI2",          {&Dxf_header::m_useri2, {70}}},
  {"USERI3",          {&Dxf_header::m_useri3, {70}}},
  {"USERI4",          {&Dxf_header::m_useri4, {70}}},
  {"USERI5",          {&Dxf_header::m_useri5, {70}}},
  {"USERR1",          {&Dxf_header::m_userr1, {40}}},
  {"USERR2",          {&Dxf_header::m_userr2, {40}}},
  {"USERR3",          {&Dxf_header::m_userr3, {40}}},
  {"USERR4",          {&Dxf_header::m_userr4, {40}}},
  {"USERR5",          {&Dxf_header::m_userr5, {40}}},
  {"USRTIMER",        {&Dxf_header::m_usrtimer, {70}}},
  {"VERSIONGUID",     {&Dxf_header::m_versionguid, {2}}},
  {"VISRETAIN",       {&Dxf_header::m_visretain, {70}}},
  {"WORLDVIEW",       {&Dxf_header::m_worldview, {70}}},
  {"XCLIPFRAME",      {&Dxf_header::m_xclipframe, {290}}},
  {"XEDIT",           {&Dxf_header::m_xedit, {290}}}
};

SGAL_END_NAMESPACE
