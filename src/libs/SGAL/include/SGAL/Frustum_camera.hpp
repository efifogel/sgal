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

#ifndef SGAL_FRUSTUM_CAMERA_HPP
#define SGAL_FRUSTUM_CAMERA_HPP

/*! \file
 *
 */

#include "SGAL/basic.hpp"
#include "SGAL/Camera.hpp"

SGAL_BEGIN_NAMESPACE

class Frustum_camera : public Camera {
private:
  // static void InitClass();
  // ENB_DLLEXPORT static EType * s_classType;

public: // Statics
  // static EType * get_classType() {
  // if (s_classType == NULL) InitClass();
  // return s_classType;
  // }

public: // Constructors
  Frustum_camera();
  // Frustum_camera(EType * type);
  // - used only in subclassing
  virtual Container * Instantiate();
  // - inherited from Container
  virtual ~Frustum_camera();

public: // Set/Get
  void SetFrustum(Frustum * frustum);

private:
  void * m_asymFrustumData;
};

SGAL_END_NAMESPACE

#endif
