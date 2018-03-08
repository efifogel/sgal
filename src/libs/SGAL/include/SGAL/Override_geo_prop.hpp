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
// $Source$
// $Revision: 12384 $
//
// Author(s) : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_OVERRIDE_GEO_PROP_HPP
#define SGAL_OVERRIDE_GEO_PROP_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Bit_mask.hpp"
#include "SGAL/Gfx.hpp"

SGAL_BEGIN_NAMESPACE

class Override_geo_prop {
private:
  // static void InitClass();
  // ENB_DLLEXPORT static EType* s_classType;

public: // Statics

  // static EType* get_classType() {
  // if (_ClassType == NULL)
  // InitClass();
  // return s_classType;
  // }

public: // Constructors
  Override_geo_prop();
  // Override_geo_prop(EType* type);
  // - used only in subclassing
  virtual ~Override_geo_prop();

public: // Enums
#if 0
  // Field Enums
  enum {
    FIRST = Container::LAST - 1,
    SGAL_STATE_ELEMENTS,
    LAST
  };
#endif

public: // Set/Get
  void set_cull_face(Gfx::Cull_face cull_face);
  Gfx::Cull_face get_cull_face() { return Gfx::NO_CULL; }

  void set_light_model_sides(Gfx::Light_model_sides light_model_sides);
  Gfx::Light_model_sides get_light_model_sides() { return Gfx::ONE_SIDE; }

  void set_light_model_color_control(Gfx::Light_model_color_control model);
  Gfx::Light_model_color_control get_light_model_color_control()
  { return Gfx::SINGLE_COLOR; }

  void set_line_width(Float line_width);
  Float get_line_width() { return 0; }

  void set_point_size(Float point_size);
  Float get_point_size() { return 0; }

  void set_override(const Bit_mask& override);
  void get_override(Bit_mask& /* override */) {} ;

public: // Variables

private:
  Bit_mask m_override;
  Float m_line_width;
  Float m_point_size;
  Gfx::Cull_face m_cull_face;
  Gfx::Light_model_sides m_light_model_sides;
  Gfx::Light_model_color_control m_light_model_color_control;

private: // Private
  void Init();

  // friend class Context;
};

SGAL_END_NAMESPACE

#endif
