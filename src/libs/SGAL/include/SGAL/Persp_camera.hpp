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
// $Id: $
// $Revision: 1308 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#ifndef SGAL_PERSP_CAMERA_HPP
#define SGAL_PERSP_CAMERA_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Camera.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Persp_camera : public Camera {
private:
  // static void InitClass();
  // SGAL_DLLEXPORT static EType * s_classType;

public: // Statics
  // static EType * get_classType() {
  // if (s_classType == NULL) InitClass();
  // return s_classType;
  // }

public: // Constructors
  Persp_camera();
  // Persp_camera(EType * type);
  // - used only in subclassing
  // virtual Container * Instantiate();
  // - inherited from Container
  virtual ~Persp_camera();

public: // Enums
  // Fields
  enum {
    FIRST = Camera::LAST - 1,
    HORIZ_FOV,
    VERT_FOV,
    HORIZ_FOV_OFF,
    VERT_FOV_OFF,
    NEAR_CLIP,
    FAR_CLIP,
    ASPECT_MODE,
    ASPECT_RATIO,
    LAST
  };

public:
  // Set/Get
  void set_horiz_fov(Float horiz_fov);
  Float get_HorizFOV() const;

  void set_vert_fov(Float vert_fov);
  Float get_vertFOV() const;

  void set_horiz_fov_off(Float horiz_fov_off);
  Float get_HorizFOVOff() const;

  void set_vert_fov_off(Float vert_fov_off);
  Float get_vertFOVOff() const;

  void set_near_clip(Float near_clip);
  Float get_near_clip() const;

  void set_far_clip(Float far_clip);
  Float get_farClip() const;

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
  // Fields
  Float m_horiz_fov;
  Float m_vert_fov;
  Float m_horiz_fov_off;
  Float m_vert_fov_off;
  Float m_near_clip;
  Float m_far_clip;
  Frustum::Aspect_mode m_aspect_mode;
  Float m_aspect_ratio;
};

SGAL_END_NAMESPACE

#endif
