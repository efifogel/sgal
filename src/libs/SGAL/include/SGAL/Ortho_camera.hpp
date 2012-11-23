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
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_ORTHO_CAMERA_HPP
#define SGAL_ORTHO_CAMERA_HPP

#include "SGAL/basic.hpp"
#include "Types.h"
#include "SGAL/Camera.hpp"
#include "SGAL/Vector2f.hpp"

SGAL_BEGIN_NAMESPACE

class Ortho_camera : public Camera {
private:
  // static void InitClass();
  // SGAL_DLLEXPORT static EType * s_classType;

public: // Statics
  // static EType * get_classType() {
  // if (s_classType == NULL) InitClass();
  // return s_classType;
  // }

public: // Constructors
  Ortho_camera();
  // EOrthoCamera(EType * type);
  // - used only in subclassing
  // virtual Container * Instantiate();
  // - inherited from Container
  virtual ~Ortho_camera();

public: // Enums
  enum {
    FIRST = Camera::LAST - 1,
    WIDTH,
    HEIGHT,
    CENTER,
    NEAR_CLIP,
    FAR_CLIP,
    ASPECT_MODE,
    ASPECT_RATIO,
    LAST
  };

public:
  // Set/Get

  void set_width(Float width);
  Float get_width() const;

  void set_height(Float height);
  Float get_height() const;

  void set_center(const Vector2f & center);
  void get_center(Vector2f & center) const;
  void set_center(Float v0, Float v1);
  void get_center(Float * v0, Float * v1) const;

  void set_near_clip(Float near_clip);
  Float get_near_clip() const;

  void set_far_clip(Float far_clip);
  Float get_far_clip() const;

  void set_aspect_mode(Frustum::Aspect_mode mode);
  Frustum::Aspect_mode get_aspect_mode() const;

  void set_aspect_ratio(Float aspect);
  Float get_aspect_ratio() const;

public:
  // virtual void FieldChanged(Short fieldId);
  // - only for subclassing

private:
  void init();

protected:
  Float m_width;
  Float m_height;
  Vector2f m_center;
  Float m_near_clip;
  Float m_far_clip;
  Int m_aspect_mode;
  Float m_aspect_ratio;
};

inline void Ortho_camera::set_Center(const Vector2f & center)
{
  set_Center(center[0], center[1]);
}

inline void Ortho_camera::get_center(Vector2f & center) const
{
  get_center(&center[0], &center[1]);
}

SGAL_END_NAMESPACE

#endif
