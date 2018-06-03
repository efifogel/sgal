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
#include "SGAL/Dxf_record_wrapper.hpp"
#include "SGAL/Dxf_plotsettings_object.hpp"

SGAL_BEGIN_NAMESPACE

typedef Dxf_record_wrapper<Dxf_plotsettings_object>
  Dxf_plotsettings_object_wrapper;

//! Record members
template <>
const std::map<int, Dxf_plotsettings_object_wrapper::Record_member>
Dxf_plotsettings_object_wrapper::s_record_members = {
  {1, {&Dxf_plotsettings_object::m_page_setup_name, 1, 0}},
  {2, {&Dxf_plotsettings_object::m_printer_name, 1, 0}},
  {4, {&Dxf_plotsettings_object::m_paper_size, 1, 0}},
  {6, {&Dxf_plotsettings_object::m_plot_view_name, 1, 0}},
  {40, {&Dxf_plotsettings_object::m_unprintable_left_margin_size, 1, 0}},
  {41, {&Dxf_plotsettings_object::m_unprintable_bottom_margin_size, 1, 0}},
  {42, {&Dxf_plotsettings_object::m_unprintable_right_margin_size, 1, 0}},
  {43, {&Dxf_plotsettings_object::m_unprintable_top_margin_size, 1, 0}},
  {44, {&Dxf_plotsettings_object::m_paper_width, 1, 0}},
  {45, {&Dxf_plotsettings_object::m_paper_height, 1, 0}},
  {46, {&Dxf_plotsettings_object::m_plot_origin_x_value, 1, 0}},
  {47, {&Dxf_plotsettings_object::m_plot_origin_y_value, 1, 0}},
  {48, {&Dxf_plotsettings_object::m_plot_window_lower_left_x_value, 1, 0}},
  {49, {&Dxf_plotsettings_object::m_plot_window_lower_left_y_value, 1, 0}},
  {140, {&Dxf_plotsettings_object::m_plot_window_upper_right_x_value, 1, 0}},
  {141, {&Dxf_plotsettings_object::m_plot_window_upper_right_y_value, 1, 0}},
  {142, {&Dxf_plotsettings_object::m_custom_print_scale_numerator, 1, 0}},
  {143, {&Dxf_plotsettings_object::m_custom_print_scale_denominator, 1, 0}},
  {70, {&Dxf_plotsettings_object::m_plot_layout_flags, 1, 0}},
  {72, {&Dxf_plotsettings_object::m_plot_paper_units, 1, 0}},
  {73, {&Dxf_plotsettings_object::m_plot_rotation, 1, 0}},
  {74, {&Dxf_plotsettings_object::m_plot_type, 1, 0}},
  {7, {&Dxf_plotsettings_object:: m_current_style_sheet, 1, 0}},
  {75, {&Dxf_plotsettings_object::m_standard_scale, 1, 0}},
  {76, {&Dxf_plotsettings_object::m_shade_plot_mode, 1, 0}},
  {77, {&Dxf_plotsettings_object::m_shade_plot_resolution_level, 1, 0}},
  {78, {&Dxf_plotsettings_object::m_shade_plot_custom_dpi, 1, 0}},
  // {Valid range: 100 to 32767
  // Only applied when the ShadePlot resolution level is set to 5 (Custom)
  {147, {&Dxf_plotsettings_object::m_standard_scale_value, 1, 0}},
   {148, {&Dxf_plotsettings_object::m_paper_image_origin_x, 1, 0}},
  {149, {&Dxf_plotsettings_object::m_paper_image_origin_y, 1, 0}},
  {333, {&Dxf_plotsettings_object::m_shade_plot_object , 1, 0}}
};

//! Record handlers
template <>
const std::map<int, Dxf_plotsettings_object_wrapper::Record_handler_type>
Dxf_plotsettings_object_wrapper::s_record_handlers = {};

//! Handles a marker.
bool Dxf_plotsettings_object::handle_marker(const String& marker)
{ return ("AcDbPlotSettings" == marker); }

SGAL_END_NAMESPACE
