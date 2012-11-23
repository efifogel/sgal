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

/*!
 * Holds the data for rendering in the depth sorting order.
 */

#ifndef SGAL_TRANSLUCENT_OBJ_HPP
#define SGAL_TRANSLUCENT_OBJ_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Debug.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/SGAL_defs.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Matrix4f.hpp"

SGAL_BEGIN_NAMESPACE

class Translucent_obj {
  DECLARE_DEBUG_CLASS(DBG_TRANSLUCENTOBJ);

public:
  Translucent_obj();
  virtual ~Translucent_obj();
  virtual bool operator < (const Translucent_obj* Obj) const;

  void SetFixedTransform( float* m);
  void SetDepthValue(float depth) { m_depthValue = depth; }
  void SetTranslucentShape (Shape * shape) { m_translucentShape = shape; }
  void SetLightList (LightList * Llist , Bit_mask* mask);
  void ReleaseLightsPtr();
  void DisableAllLights(Draw_action * da);
  void DrawLights(Draw_action * da , Translucent_obj * prvTransObj);
  void DisableLights(Draw_action * da , Translucent_obj * nextTransObj);
  
  int get_iD() { return m_id; }
  float get_depthVal() { return m_depthValue; }
  float * get_fixedTransform() { return m_modelviewMat; }
  Shape * get_translucentShape() { return m_translucentShape; }
  Boolean HasLights() { return !m_LightsPtr.empty(); }
  Bit_mask * GetLightsMask() { return &m_lightsMask; }

private:

  Shape * m_translucentShape;
  LightList m_LightsPtr;
  Bit_mask m_lightsMask;
  float m_modelviewMat[16];//  Holds the current modelview matrix for the translucent objects.
  float m_depthValue;
  int m_id;
};

typedef std::list < Translucent_obj* > ETranslucentList;
typedef ETranslucentList::iterator ETranslucentListIter;

typedef std::greater < Translucent_obj* > ETranslucentGreater;

typedef std::vector < Translucent_obj* > Translucent_vec;
typedef Translucent_vec::iterator Translucent_vecIter;

typedef std::deque < Translucent_obj* > ETranslucentQue;
typedef ETranslucentQue::iterator ETranslucentQueIter;

SGAL_END_NAMESPACE

#endif
