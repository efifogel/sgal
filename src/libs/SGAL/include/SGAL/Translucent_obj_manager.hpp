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
 * Deals with depth sorting the shape nodes in the scene for translucency
 * effects and collision detection in the future.
 */

#ifndef SGAL_TRANSLUCENT_OBJ_MANAGER_HPP
#define SGAL_TRANSLUCENT_OBJ_MANAGER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Types.hpp"
#include "SGAL/Debug.hpp"
#include "SGAL/Translucent_obj.hpp"
#include "SGAL/Shape.hpp"
#include "SGAL/Matrix4f.hpp"
#include "SGAL/Light.hpp"

SGAL_BEGIN_NAMESPACE

class SGAL_SGAL_DECL Translucent_obj_manager {
public:
  Translucent_obj_manager();
  virtual ~Translucent_obj_manager();

  void SetFixedTransformation();
  void UpdateCurrentTransformation();
  void CalculateDepthVal(SphereBound sphereBound);
  void Add(Shape * shape );
  void Remove(Shape * shape );
  void Sort();
  void Clean(Draw_action * drawA);
  void Reset(Draw_action * drawA);
  void UpdateIterator();
  void Draw(Draw_action * drawA);
  void AddLightSource(Light * lightSource);
  void RemoveLightSource();
  void UpdateTranslucentLights ();
  float* get_currentMatrix() { return m_currentModelviewMat; }

private:

  ETranslucentList m_translucentList;
  ETranslucentVec m_translucentRef; // Holds a pointers indicate to m_translucentList objects.
  float       m_currentModelviewMat[16];// Holds the current modelview matrix
                         // for the translucent objects.
  ETranslucentVecIter m_tvIter; // Translucent vector iterator.
  ETranslucentObj * m_translPtr; // Translucent index pointer.
  ETranslucentQue m_tmpQue;
  LightList m_LightPtrList;
  Bit_mask m_currentLightsMask;
  Int m_lightsCounter;
};

SGAL_END_NAMESPACE

#endif
