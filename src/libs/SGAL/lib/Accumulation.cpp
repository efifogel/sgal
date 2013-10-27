// Copyright (c) 2004 Israel.
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
// $Id: Accumulation.cpp 7628 2009-05-26 11:11:45Z efif $
// $Revision: 7628 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*! A data structure used to accumulate frame buffers produced in a sequence.
 * It is used to implement antialiasing of the highest quality at the price
 * of a delay.
 */

#include <boost/lexical_cast.hpp>

#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SGAL/basic.hpp"
#include "SGAL/Accumulation.hpp"
#include "SGAL/Gl_wrapper.hpp"
#include "SGAL/Utilities.hpp"
#include "SGAL/Element.hpp"
#include "SGAL/Container_factory.hpp"
#include "SGAL/Container_proto.hpp"
#include "SGAL/Execution_function.hpp"
#include "SGAL/Field_infos.hpp"
#include "SGAL/Window_item.hpp"

SGAL_BEGIN_NAMESPACE

const std::string Accumulation::s_tag = "Accumulation";
Container_proto* Accumulation::s_prototype(NULL);

/* jitter point arrays for 2,3,4,8,15,24 and 66 jitters.
 * The arrays are named j2, j3, etc.
 * Floating point in the range -.5 < x < .5, -.5 < y < .5 with gaussian
 * distribution around the origin.
 */

/* 2 jitter points */
Accumulation::Jitter Accumulation::s_j2[] =
{
  { 0.246490f,  0.249999f},
  {-0.246490f, -0.249999f}
};


/* 3 jitter points */
Accumulation::Jitter Accumulation::s_j3[] =
{
  {-0.373411f, -0.250550f},
  { 0.256263f,  0.368119f},
  { 0.117148f, -0.117570f}
};

/* 4 jitter points */
Accumulation::Jitter Accumulation::s_j4[] =
{
  {-0.208147f,  0.353730f},
  { 0.203849f, -0.353780f},
  {-0.292626f, -0.149945f},
  { 0.296924f,  0.149994f}
};

/* 8 jitter points */
Accumulation::Jitter Accumulation::s_j8[] =
{
  {-0.334818f,  0.435331f},
  { 0.286438f, -0.393495f},
  { 0.459462f,  0.141540f},
  {-0.414498f, -0.192829f},
  {-0.183790f,  0.082102f},
  {-0.079263f, -0.317383f},
  { 0.102254f,  0.299133f},
  { 0.164216f, -0.054399f}
};

/* 15 jitter points */
Accumulation::Jitter Accumulation::s_j15[] =
{
  { 0.285561f,  0.188437f},
  { 0.360176f, -0.065688f},
  {-0.111751f,  0.275019f},
  {-0.055918f, -0.215197f},
  {-0.080231f, -0.470965f},
  { 0.138721f,  0.409168f},
  { 0.384120f,  0.458500f},
  {-0.454968f,  0.134088f},
  { 0.179271f, -0.331196f},
  {-0.307049f, -0.364927f},
  { 0.105354f, -0.010099f},
  {-0.154180f,  0.021794f},
  {-0.370135f, -0.116425f},
  { 0.451636f, -0.300013f},
  {-0.370610f,  0.387504f}
};

/* 24 jitter points */
Accumulation::Jitter Accumulation::s_j24[] =
{
  { 0.030245f,  0.136384f},
  { 0.018865f, -0.348867f},
  {-0.350114f, -0.472309f},
  { 0.222181f,  0.149524f},
  {-0.393670f, -0.266873f},
  { 0.404568f,  0.230436f},
  { 0.098381f,  0.465337f},
  { 0.462671f,  0.442116f},
  { 0.400373f, -0.212720f},
  {-0.409988f,  0.263345f},
  {-0.115878f, -0.001981f},
  { 0.348425f, -0.009237f},
  {-0.464016f,  0.066467f},
  {-0.138674f, -0.468006f},
  { 0.144932f, -0.022780f},
  {-0.250195f,  0.150161f},
  {-0.181400f, -0.264219f},
  { 0.196097f, -0.234139f},
  {-0.311082f, -0.078815f},
  { 0.268379f,  0.366778f},
  {-0.040601f,  0.327109f},
  {-0.234392f,  0.354659f},
  {-0.003102f, -0.154402f},
  { 0.297997f, -0.417965f}
};

/* 66 jitter points */
Accumulation::Jitter Accumulation::s_j66[] =
{
  { 0.266377f, -0.218171f},
  {-0.170919f, -0.429368f},
  { 0.047356f, -0.387135f},
  {-0.430063f,  0.363413f},
  {-0.221638f, -0.313768f},
  { 0.124758f, -0.197109f},
  {-0.400021f,  0.482195f},
  { 0.247882f,  0.152010f},
  {-0.286709f, -0.470214f},
  {-0.426790f,  0.004977f},
  {-0.361249f, -0.104549f},
  {-0.040643f,  0.123453f},
  {-0.189296f,  0.438963f},
  {-0.453521f, -0.299889f},
  { 0.408216f, -0.457699f},
  { 0.328973f, -0.101914f},
  {-0.055540f, -0.477952f},
  { 0.194421f,  0.453510f},
  { 0.404051f,  0.224974f},
  { 0.310136f,  0.419700f},
  {-0.021743f,  0.403898f},
  {-0.466210f,  0.248839f},
  { 0.341369f,  0.081490f},
  { 0.124156f, -0.016859f},
  {-0.461321f, -0.176661f},
  { 0.013210f,  0.234401f},
  { 0.174258f, -0.311854f},
  { 0.294061f,  0.263364f},
  {-0.114836f,  0.328189f},
  { 0.041206f, -0.106205f},
  { 0.079227f,  0.345021f},
  {-0.109319f, -0.242380f},
  { 0.425005f, -0.332397f},
  { 0.009146f,  0.015098f},
  {-0.339084f, -0.355707f},
  {-0.224596f, -0.189548f},
  { 0.083475f,  0.117028f},
  { 0.295962f, -0.334699f},
  { 0.452998f,  0.025397f},
  { 0.206511f, -0.104668f},
  { 0.447544f, -0.096004f},
  {-0.108006f, -0.002471f},
  {-0.380810f,  0.130036f},
  {-0.242440f,  0.186934f},
  {-0.200363f,  0.070863f},
  {-0.344844f, -0.230814f},
  { 0.408660f,  0.345826f},
  {-0.233016f,  0.305203f},
  { 0.158475f, -0.430762f},
  { 0.486972f,  0.139163f},
  {-0.301610f,  0.009319f},
  { 0.282245f, -0.458671f},
  { 0.482046f,  0.443890f},
  {-0.121527f,  0.210223f},
  {-0.477606f, -0.424878f},
  {-0.083941f, -0.121440f},
  {-0.345773f,  0.253779f},
  { 0.234646f,  0.034549f},
  { 0.394102f, -0.210901f},
  {-0.312571f,  0.397656f},
  { 0.200906f,  0.333293f},
  { 0.018703f, -0.261792f},
  {-0.209349f, -0.065383f},
  { 0.076248f,  0.478538f},
  {-0.073036f, -0.355064f},
  { 0.145087f,  0.221726f}
};

/*! Size array */
Accumulation::Jitter_pair Accumulation::s_sizes[] = {
  Accumulation::Jitter_pair(2, Accumulation::s_j2),
  Accumulation::Jitter_pair(3, Accumulation::s_j3),
  Accumulation::Jitter_pair(4, Accumulation::s_j4),
  Accumulation::Jitter_pair(8, Accumulation::s_j8),
  Accumulation::Jitter_pair(15,Accumulation::s_j15),
  Accumulation::Jitter_pair(24,Accumulation::s_j24),
  Accumulation::Jitter_pair(66,Accumulation::s_j66)
};

// Default values:
const Boolean Accumulation::s_def_enabled(true);
const Uint Accumulation::s_def_delay(800);
const Accumulation::Quality Accumulation::s_def_quality(Accumulation::Q_HIGH);

const Char* Accumulation::s_quality_names[] = {"high", "low"};

const Uint Accumulation::s_def_red_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS);
const Uint Accumulation::s_def_green_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS);
const Uint Accumulation::s_def_blue_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS);
const Uint Accumulation::s_def_alpha_bits(SGAL_DEF_WINDOW_ACCUM_COLOR_BITS);

REGISTER_TO_FACTORY(Accumulation, "Accumulation");

/*! Constructor */
Accumulation::Accumulation(Boolean proto) :
  Container(proto),
  m_enabled(s_def_enabled),
  m_delay(s_def_delay),
  m_quality(s_def_quality),
  m_num_iters(),
  m_active(false),
  m_accumulate(0),
  m_iteration_no(0),
  m_show(false),
  m_red_bits(s_def_red_bits),
  m_green_bits(s_def_green_bits),
  m_blue_bits(s_def_blue_bits),
  m_alpha_bits(s_def_alpha_bits)
{ set_num_iters(8); }

/*! \brief initializes the node prototype. */
void Accumulation::init_prototype()
{
  if (s_prototype) return;
  s_prototype = new Container_proto(Container::get_prototype());

  // Add the object fields to the prototype
  Execution_function exec_func;

  // enabled
  exec_func =
    static_cast<Execution_function>(&Container::set_rendering_required);
  Boolean_handle_function enabled_func =
    static_cast<Boolean_handle_function>(&Accumulation::enabled_handle);
  s_prototype->add_field_info(new SF_bool(ENABLED, "enabled", enabled_func,
                                          exec_func));

  // quality
  Uint_handle_function quality_func =
    reinterpret_cast<Uint_handle_function>(&Accumulation::quality_handle);
  s_prototype->add_field_info(new SF_uint(QUALITY, "quality", quality_func,
                                          exec_func));

  // delay
  Uint_handle_function delay_func =
    static_cast<Uint_handle_function>(&Accumulation::delay_handle);
  s_prototype->add_field_info(new SF_uint(DELAY, "delay", delay_func));
}

/*! \brief deletes the node prototype. */
void Accumulation::delete_prototype()
{
  delete s_prototype;
  s_prototype = NULL;
}

/*! \brief obtains the node prototype. */
Container_proto* Accumulation::get_prototype()
{
  if (!s_prototype) Accumulation::init_prototype();
  return s_prototype;
}

/*! \brief sets defualt values. */
void Accumulation::reset(Boolean def_enabled, Uint def_delay,
                         Quality def_quality)
{
  m_enabled = def_enabled;
  m_delay = def_delay;
  m_quality = def_quality;
}

/*! \brief sets the attributes of the object. */
void Accumulation::set_attributes(Element* elem)
{
  Container::set_attributes(elem);

  typedef Element::Str_attr_iter                Str_attr_iter;
  Str_attr_iter ai;
  for (ai = elem->str_attrs_begin(); ai != elem->str_attrs_end(); ++ai) {
    const std::string& name = elem->get_name(ai);
    const std::string& value = elem->get_value(ai);
    if (name == "enabled") {
      set_enabled(compare_to_true(value));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "delay") {
      set_delay(atoi(value.c_str()));
      elem->mark_delete(ai);
      continue;
    }
    if (name == "quality") {
      Uint num = sizeof(s_quality_names) / sizeof(char *);
      const char** found = std::find(s_quality_names, &s_quality_names[num],
                                      strip_double_quotes(value));
      Uint index = found - s_quality_names;
      if (index < num) m_quality = static_cast<Quality>(index);
      elem->mark_delete(ai);
      continue;
    }
    if (name == "accumBits") {
      Uint bits = boost::lexical_cast<Uint>(value);
      set_number_of_bits(bits, bits, bits, bits);
      elem->mark_delete(ai);
      continue;
    }
  }

  // Remove all the marked attributes:
  elem->delete_marked();
}

#if 0
/*! \brief */
Attribute_list Accumulation::get_attributes()
{
  Attribute_list attrs;
  Attribue attrib;
  char buf[32];

  attrs = Container::get_attributes();

  if (m_enabled != s_def_enabled) {
    attrib.first = "enabled";
    attrib.second = FALSE_STR;
    attrs.push_back(attrib);
  }
  if (m_delay != s_def_delay) {
    attrib.first = "delay";
    sprintf(buf, "%d", m_delay);
    attrib.second = buf;
    attrs.push_back(attrib);
  }
  if (m_quality != s_def_quality) {
    attrib.first = "quality";
    attrib.second = s_quality_names[m_quality];
    attrs.push_back(attrib);
  }
}
#endif

/*! \brief sets the total number of iterations. */
void Accumulation::set_num_iters(Uint num_iters)
{
  Uint num_jitter_arrays = sizeof(s_sizes) / sizeof(Jitter_pair);
  Uint i;
  for (i = 0; i < num_jitter_arrays; ++i)
    if (num_iters <= s_sizes[i].first) break;
  if (i == num_jitter_arrays) {
    --i;
    num_iters = s_sizes[i].first;
  }

  m_num_jitters = s_sizes[i].first;
  m_jitters = s_sizes[i].second;

  m_num_iters = num_iters;
  m_contribution = 1 / (Float) m_num_iters;
}

/*! \brief applies Actions when accumulation starts. */
void Accumulation::pre_render(Draw_action* /* draw_action */)
{
  if (m_iteration_no == 0) {
    glClearAccum(0, 0, 0, 0);
    glClear(GL_ACCUM_BUFFER_BIT);
  }
}

/*! \brief applies actions when accumulation ends. */
void Accumulation::post_render(Draw_action* /* draw_action */)
{
  Boolean blendMode = (glIsEnabled(GL_BLEND) == GL_TRUE);
  if (blendMode) glDisable(GL_BLEND);

  ++m_iteration_no;
  m_accumulate += m_contribution;
  glAccum(GL_ACCUM, m_contribution);

  m_show = m_iteration_no == m_num_iters;
  // ((m_iteration_no == m_num_iters / 2) || (m_iteration_no == m_num_iters))

  if (is_done()) glAccum(GL_RETURN, 1.0f / m_accumulate);

  if (blendMode) glEnable(GL_BLEND);
}

/*! \brief resets the delay start time to the current time. */
void Accumulation::reset_delay_time() {}

/*! \brief enactivates accumulation. */
void Accumulation::enactivate()
{
  m_active = true;
  m_iteration_no = 0;
  m_accumulate = 0;
}

/*! \brief returns ture if accumulation is done. */
Boolean Accumulation::is_done() { return (m_iteration_no == m_num_iters); }

/*! \brief obtains the jitter value of the current iteration. */
void Accumulation::get_jitter(float& x, float& y)
{
  x = m_jitters[m_iteration_no].x;
  y = m_jitters[m_iteration_no].y;
}

/*! \brief sets the number of RGBA bits stored in the accumulation buffer. */
void Accumulation::set_number_of_bits(Uint red_bits, Uint green_bits,
                                      Uint blue_bits, Uint alpha_bits)
{
  m_red_bits = red_bits;
  m_green_bits = green_bits;
  m_blue_bits = blue_bits;
  m_alpha_bits = alpha_bits;
}

/*! \brief obtains the number of RGBA bits stored in the accumulation buffer. */
void Accumulation::get_number_of_bits(Uint& red_bits, Uint& green_bits,
                                      Uint& blue_bits, Uint& alpha_bits) const
{
  red_bits = m_red_bits;
  green_bits = m_green_bits;
  blue_bits = m_blue_bits;
  alpha_bits = m_alpha_bits;
}

SGAL_END_NAMESPACE
