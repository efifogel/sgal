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
// $Revision: 1310 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include "SGAL/basic.hpp"
#include "TranslucentObj.h"

using namespace SGAL;

  /*! Constructor */
Translucent::Translucent(Boolean proto) :
  m_translucentShape(0),
  m_depthValue(0),
  m_id(-1)
{
  for (Uint i = 0; i < 16; ++i)
    m_modelviewMat[i] = 0;
}

/*! Destructor */
Translucent::~Translucent() {}

void Translucent::SetLightList ( LightList* Llist , Bit_mask* mask)
{
  LightListIter LItr = Llist->begin();
  while ( LItr != Llist->end() )
  {
    m_LightsPtr.push_back( (*LItr) );
    LItr++;
  }
    m_lightsMask.set_mask(mask->get_mask());
}

void Translucent::ReleaseLightsPtr()
{
  while ( m_LightsPtr.begin() != m_LightsPtr.end() )
  {
    LightListIter LItr = m_LightsPtr.begin();
    m_LightsPtr.pop_front();
  }
}

void Translucent::DisableAllLights( Draw_action* da )
{
  Context* context = da->get_context();

  LightListIter LItr = m_LightsPtr.begin();
  while ( LItr != m_LightsPtr.end() )
  {
    context->DisableLight((*LItr));
    LItr++;
  }
}

void Translucent::DrawLights( Draw_action* da , Translucent* prvTransObj)
{
  Bit_mask drawMask, disMask;
  Context* context = da->get_context();
  Bit_mask* prvLightMask=NULL;

  if ( prvTransObj != NULL )
  {
    prvLightMask = prvTransObj->GetLightsMask();
    drawMask.set_mask(m_lightsMask.get_mask());
    drawMask.AndNotEqual(*prvLightMask);
  }
  else
    drawMask.set_mask(m_lightsMask.get_mask());

  int ind = 0; int size = 32;
  LightListIter LItr = m_LightsPtr.begin();
  while ( LItr != m_LightsPtr.end() && ind < size )
  {
    if ( drawMask.get_bit( ind ) )
    {
      float* tmp = (*LItr)->get_fixedTransform();
      glLoadMatrixf( (*LItr)->get_fixedTransform() );
      (*LItr)->Draw( da );
      LItr++;
    }
    ind++;
  }
}

void Translucent::DisableLights( Draw_action* da , Translucent* nextTransObj)
{
  Bit_mask disMask;
  Context* context = da->get_context();
  Bit_mask* nextLightMask=NULL;

  if ( nextTransObj == NULL )  return;

  nextLightMask = nextTransObj->GetLightsMask();
  disMask.set_mask(m_lightsMask.get_mask());
  disMask.AndNotEqual(*nextLightMask);

  int ind = 0; int size = 32;
  LightListIter LItr = m_LightsPtr.begin();
  while ( LItr != m_LightsPtr.end() && ind < size )
  {
    if ( disMask.get_bit(ind))
    {
      context->DisableLight((*LItr)); 
      LItr++;
    }
    ind++;
  }
}

void Translucent::SetFixedTransform( float* m)
{
  for ( int i=0; i < 16; i++ )
  {
    m_modelviewMat[i] = m[i];
  }
}

bool Translucent::operator < (const Translucent* Obj) const
{
  if( m_depthValue < Obj->m_depthValue )
    return false;

  return true;
}
