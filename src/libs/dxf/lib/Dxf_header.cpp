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
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <map>

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"

#include "dxf/basic.hpp"
#include "dxf/Dxf_header.hpp"
#include "dxf/Dxf_header_wrapper.hpp"

DXF_BEGIN_NAMESPACE

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
 * Also, some of the codes listed in the Autodesk page where erroneous (i.e.,
 * HIDETEXT, INTERSECTIONDISPLAY).
 */

//! The DXF version names
const std::map<size_t, std::string> Dxf_header::s_version_names = {
  {10, "AC1006"},
  {11, "AC1009"},
  {12, "AC1009"},
  {13, "AC1012"},
  {14, "AC1014"},
  {15, "AC1015"},
  {16, "AC1018"},
  {17, "AC1021"},
  {18, "AC1024"},
  {19, "AC1027"},
  {20, "AC1029"},
  {21, "AC1031"},
  {22, "AC1032"}
};

const std::map<std::string, std::list<int> >
Dxf_header_wrapper::s_header_members = {
  {"$ACADMAINTVER",    {70}},
  {"$ACADVER",         {1}},
  {"$ANGBASE",         {50}},
  {"$ANGDIR",          {70}},
  {"$ATTMODE",         {70}},
  {"$AUNITS",          {70}},
  {"$AUPREC",          {70}},
  {"$CECOLOR",         {62}},
  {"$CELTSCALE",       {40}},
  {"$CELTYPE",         {6}},
  {"$CELWEIGHT",       {370}},
  {"$CEPSNID",         {390}},
  {"$CEPSNTYPE",       {380}},
  {"$CHAMFERA",        {40}},
  {"$CHAMFERB",        {40}},
  {"$CHAMFERC",        {40}},
  {"$CHAMFERD",        {40}},
  {"$CLAYER",          {8}},
  {"$CMLJUST",         {70}},
  {"$CMLSCALE",        {40}},
  {"$CMLSTYLE",        {2}},
  {"$CSHADOW",         {280}},
  {"$DIMADEC",         {70}},
  {"$CUSTOMPROPERTY",  {1}},
  {"$CUSTOMPROPERTYTAG",{1}},
  {"$DIMALT",          {70}},
  {"$DIMALTD",         {70}},
  {"$DIMALTF",         {40}},
  {"$DIMALTRND",       {40}},
  {"$DIMALTTD",        {70}},
  {"$DIMALTTZ",        {70}},
  {"$DIMALTU",         {70}},
  {"$DIMALTZ",         {70}},
  {"$DIMAPOST",        {1}},
  {"$DIMASO",          {70}}, // Obsolete; see DIMASSOC.
  {"$DIMASSOC",        {280}},
  {"$DIMASZ",          {40}},
  {"$DIMATFIT",        {70}},
  {"$DIMAUNIT",        {70}},
  {"$DIMAZIN",         {70}},
  {"$DIMBLK",          {1}},
  {"$DIMBLK1",         {1}},
  {"$DIMBLK2",         {1}},
  {"$DIMCEN",          {40}},
  {"$DIMCLRD",         {70}},
  {"$DIMCLRE",         {70}},
  {"$DIMCLRT",         {70}},
  {"$DIMDEC",          {70}},
  {"$DIMDLE",          {40}},
  {"$DIMDLI",          {40}},
  {"$DIMDSEP",         {70}},
  {"$DIMEXE",          {40}},
  {"$DIMEXO",          {40}},
  {"$DIMFAC",          {40}},
  {"$DIMFRAC",         {70}},
  {"$DIMGAP",          {40}},
  {"$DIMJUST",         {70}},
  {"$DIMLDRBLK",       {1}},
  {"$DIMLFAC",         {40}},
  {"$DIMLIM",          {70}},
  {"$DIMLUNIT",        {70}},
  {"$DIMLWD",          {70}},
  {"$DIMLWE",          {70}},
  {"$DIMPOST",         {1}},
  {"$DIMRND",          {40}},
  {"$DIMSAH",          {70}},
  {"$DIMSCALE",        {40}},
  {"$DIMSD1",          {70}},
  {"$DIMSD2",          {70}},
  {"$DIMSE1",          {70}},
  {"$DIMSE2",          {70}},
  {"$DIMSHO",          {70}},
  {"$DIMSOXD",         {70}},
  {"$DIMSTYLE",        {2}},
  {"$DIMTAD",          {70}},
  {"$DIMTDEC",         {70}},
  {"$DIMTFAC",         {40}},
  {"$DIMTIH",          {70}},
  {"$DIMTIX",          {70}},
  {"$DIMTM",           {40}},
  {"$DIMTMOVE",        {70}},
  {"$DIMTOFL",         {70}},
  {"$DIMTOH",          {70}},
  {"$DIMTOL",          {70}},
  {"$DIMTOLJ",         {70}},
  {"$DIMTP",           {40}},
  {"$DIMTSZ",          {40}},
  {"$DIMTVP",          {40}},
  {"$DIMTXSTY",        {7}},
  {"$DIMTXT",          {40}},
  {"$DIMTZIN",         {70}},
  {"$DIMUPT",          {70}},
  {"$DIMZIN",          {70}},
  {"$DISPSILH",        {70}},
  {"$DRAGVS",          {349}},
  {"$DWGCODEPAGE",     {3}},
  {"$ELEVATION",       {40}},
  {"$ENDCAPS",         {280}},
  {"$EXTMAX",          {10, 20, 30}},
  {"$EXTMIN",          {10, 20, 30}},
  {"$EXTNAMES",        {290}},
  {"$FILLETRAD",       {40}},
  {"$FILLMODE",        {70}},
  {"$FINGERPRINTGUID", {2}},
  {"$HALOGAP",         {280}},
  {"$HANDSEED",        {5}},
  {"$HIDETEXT",        {280}},
  {"$HYPERLINKBASE",   {1}},
  {"$INDEXCTL",        {280}},
  {"$INSBASE",         {10, 20, 30}},
  {"$INSUNITS",        {70}},
  {"$INTERFERECOLOR",  {62}},
  {"$INTERFEREOBJVS",  {345}},
  {"$INTERFEREVPVS",   {346}},
  {"$INTERSECTIONCOLOR", {70}},
  {"$INTERSECTIONDISPLAY", {280}},
  {"$JOINSTYLE",       {280}},
  {"$LASTSAVEDBY",     {1}},
  {"$LIMCHECK",        {70}},
  {"$LIMMAX",          {10, 20}},
  {"$LIMMIN",          {10, 20}},
  {"$LTSCALE",         {40}},
  {"$LUNITS",          {70}},
  {"$LUPREC",          {70}},
  {"$LWDISPLAY",       {290}},
  {"$MAXACTVP",        {70}},
  {"$MEASUREMENT",     {70}},
  {"$MENU",            {1}},
  {"$MIRRTEXT",        {70}},
  {"$OBSCOLOR",        {70}},
  {"$OBSLTYPE",        {280}},
  {"$OLESTARTUP",      {290}},
  {"$ORTHOMODE",       {70}},
  {"$PDMODE",          {70}},
  {"$PDSIZE",          {40}},
  {"$PELEVATION",      {40}},
  {"$PEXTMAX",         {10, 20, 30}},
  {"$PEXTMIN",         {10, 20, 30}},
  {"$PINSBASE",        {10, 20, 30}},
  {"$PLIMCHECK",       {70}},
  {"$PLIMMAX",         {10, 20}},
  {"$PLIMMIN",         {10, 20}},
  {"$PLINEGEN",        {70}},
  {"$PLINEWID",        {40}},
  {"$PROJECTNAME",     {1}},
  {"$PROXYGRAPHICS",   {70}},
  {"$PSLTSCALE",       {70}},
  {"$PSTYLEMODE",      {290}},
  {"$PSVPSCALE",       {40}},
  {"$PUCSBASE",        {2}},
  {"$PUCSNAME",        {2}},
  {"$PUCSORG",         {10, 20, 30}},
  {"$PUCSORGBACK",     {10, 20, 30}},
  {"$PUCSORGBOTTOM",   {10, 20, 30}},
  {"$PUCSORGFRONT",    {10, 20, 30}},
  {"$PUCSORGLEFT",     {10, 20, 30}},
  {"$PUCSORGRIGHT",    {10, 20, 30}},
  {"$PUCSORGTOP",      {10, 20, 30}},
  {"$PUCSORTHOREF",    {2}},
  {"$PUCSORTHOVIEW",   {70}},
  {"$PUCSXDIR",        {10, 20, 30}},
  {"$PUCSYDIR",        {10, 20, 30}},
  {"$QTEXTMODE",       {70}},
  {"$REGENMODE",       {70}},
  {"$SHADEDGE",        {70}},
  {"$SHADEDIF",        {70}},
  {"$SHADOWPLANELOCATION", {40}},
  {"$SKETCHINC",       {40}},
  {"$SKPOLY",          {70}},
  {"$SORTENTS",        {280}},
  {"$SPLFRAME",        {70}},
  {"$SPLINESEGS",      {70}},
  {"$SPLINETYPE",      {70}},
  {"$STYLESHEET",      {1}},
  {"$SURFTAB1",        {70}},
  {"$SURFTAB2",        {70}},
  {"$SURFTYPE",        {70}},
  {"$SURFU",           {70}},
  {"$SURFV",           {70}},
  {"$TDCREATE",        {40}},
  {"$TDINDWG",         {40}},
  {"$TDUCREATE",       {40}},
  {"$TDUPDATE",        {40}},
  {"$TDUSRTIMER",      {40}},
  {"$TDUUPDATE",       {40}},
  {"$TEXTSIZE",        {40}},
  {"$TEXTSTYLE",       {7}},
  {"$THICKNESS",       {40}},
  {"$TILEMODE",        {70}},
  {"$TRACEWID",        {40}},
  {"$TREEDEPTH",       {70}},
  {"$UCSBASE",         {2}},
  {"$UCSNAME",         {2}},
  {"$UCSORG",          {10, 20, 30}},
  {"$UCSORGBACK",      {10, 20, 30}},
  {"$UCSORGBOTTOM",    {10, 20, 30}},
  {"$UCSORGFRONT",     {10, 20, 30}},
  {"$UCSORGLEFT",      {10, 20, 30}},
  {"$UCSORGRIGHT",     {10, 20, 30}},
  {"$UCSORGTOP",       {10, 20, 30}},
  {"$UCSORTHOREF",     {2}},
  {"$UCSORTHOVIEW",    {70}},
  {"$UCSXDIR",         {10, 20, 30}},
  {"$UCSYDIR",         {10, 20, 30}},
  {"$UNITMODE",        {70}},
  {"$USERI1",          {70}},
  {"$USERI2",          {70}},
  {"$USERI3",          {70}},
  {"$USERI4",          {70}},
  {"$USERI5",          {70}},
  {"$USERR1",          {40}},
  {"$USERR2",          {40}},
  {"$USERR3",          {40}},
  {"$USERR4",          {40}},
  {"$USERR5",          {40}},
  {"$USRTIMER",        {70}},
  {"$VERSIONGUID",     {2}},
  {"$VISRETAIN",       {70}},
  {"$WORLDVIEW",       {70}},
  {"$XCLIPFRAME",      {290}},
  {"$XEDIT",           {290}}
};

DXF_END_NAMESPACE
