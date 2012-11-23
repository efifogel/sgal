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
#include "TranslucentObjManager.h"
#include "TranslucentObj.h"
#include "SphereBound.h"

using namespace SGAL;

/*********************************************************************************************************
  ETranslucentObjManager holds two ETranslucentObj pointers list. One for depth sorting - m_translucentList, 
  and one in the traverse order - m_translucentRef.

  m_translucentList holds pointers to the objects in m_translucentRef.

  m_translucentList:    ____   ____    ____     ____
            -->|____|-->|____|-->|____|-->|____|
               /\        /\       /\       /\
              _||_   _||_    _||_     _||_
  m_translucentRef:  -->|____|-->|____|-->|____|-->|____|
            
**********************************************************************************************************/

bool Greater ( Translucent* Obj1, Translucent* Obj2);

/*! Constructor */
Translucent_manager::Translucent_manager(Boolean proto) :
  m_translucentList(0),
  m_tmpQue(0),
  m_translucentRef(0),
  m_translPtr(0),
  m_lightsCounter(0)
{
  for (Uint i = 0; i < 16; i++)
    m_currentModelviewMat[i] = 0;
}

/*! Destructor */
Translucent_manager::~Translucent_manager()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Translucent_manager ...");
  m_translucentRef.erase(m_translucentRef.begin(), m_translucentRef.end());

  for (Translucent_iter tIter = m_translucentList.begin();
       tIter != m_translucentList.end(); ++tIter)
  {
    delete (*tIter);
  }
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/** Call this function before the first pass. 
*/
void Translucent_manager::Clean(Draw_action* drawA)
{
  m_lightsCounter=0;

  if ( m_translucentList.empty() ) return;

  Translucent_iter tIter = m_translucentList.begin();
  while( tIter != m_translucentList.end() )
  {
    (*tIter)->DisableAllLights(drawA);
    (*tIter)->ReleaseLightsPtr();
    tIter++;
  }

  for ( int i=0; i < 16; i++ )
  {
    m_currentModelviewMat[i] = 0.0f;
  }

}

/** Call this function before the second pass. 
*/
void Translucent_manager::Reset(Draw_action* drawA)
{
  if ( m_translucentList.empty() ) return;

  Translucent_iter tIter = m_translucentList.begin();
  while( tIter != m_translucentList.end() )
  {
    (*tIter)->DisableAllLights(drawA);
    tIter++;
  }
}

/** Update the pointer to the current TranslucentObj during 
  the traverse in first pass. 
*/
void Translucent_manager::UpdateIterator()
{
  if( m_tvIter == m_translucentRef.end() )
  {
    m_tvIter = m_translucentRef.begin();
  }
  m_translPtr = (*m_tvIter);
  m_tvIter++;
}

/** Update the lights list for the current translucent object 
  pointed by m_translPtr. 
*/
void Translucent_manager::UpdateTranslucentLights ()
{
  m_tvIter--;
  (*m_tvIter)->SetLightList( &m_LightPtrList, &m_currentLightsMask );
  m_tvIter++;
}

/** Add new translucent object to the lists "m_translucentList" and
  "m_translucentRef".  
*/
void Translucent_manager::Add( Shape* shape )
{
  Translucent *translucent = new Translucent;
  if ( translucent != NULL )
  {
    translucent->SetTranslucentShape( shape );
    m_translucentList.push_back(translucent);
    m_translucentRef.push_back(translucent);
    m_translPtr = m_translucentList.back();
    m_tvIter = m_translucentRef.end();
  }
}

// FIX - should remove also from m_translucentRef.
/** Remove translucent object from the back of the lists
  "m_translucentList" and "m_translucentRef". 
*/
void Translucent_manager::Remove( Shape* shape )
{
  Translucent_iter tIter = m_translucentList.begin();
  while( tIter != m_translucentList.end() )
  {
    if ( (*tIter)->get_iD() == shape->get_iD() )
    {
      m_translucentList.erase(tIter);
      break;
    }
    tIter++;
  }
}

/** Add new light object to the current lights list m_LightPtrList.
  also set the current bit mask in the right bit to on.
*/
void Translucent_manager::AddLightSource( Light* lightSource )
{
  m_LightPtrList.push_back(lightSource);
  m_currentLightsMask.OnBit( m_lightsCounter );
  m_lightsCounter++;
}

/** Remove light source from the back of the list m_LightPtrList.
  also set the current bit mask in the right bit to off.
*/
void Translucent_manager::RemoveLightSource()
{
  m_LightPtrList.pop_back();
  int lightsMaskInd = m_lightsCounter-1;
  while ( !m_currentLightsMask.get_bit(lightsMaskInd) &&  lightsMaskInd >= 0)
    lightsMaskInd--;
  m_currentLightsMask.OffBit( lightsMaskInd );
}

/** Set the fixed transformation matrix for the current 
  translucent object pointed by  m_translPtr.
  */
void Translucent_manager::SetFixedTransformation()
{
  if ( !m_translucentRef.empty() && m_translPtr != NULL)
  {
    m_translPtr->SetFixedTransform( m_currentModelviewMat );
  }
}

/** Update the current modelview matrix of OpenGL. 
*/
void Translucent_manager::UpdateCurrentTransformation()
{
  glget_floatv (GL_MODELVIEW_MATRIX, m_currentModelviewMat);
}

/** Calculate the depth value of the current translucent object
  pointed by m_translPtr. 
  Calculate the distance between the origin of the active camera
  and the center of the bounding sphere of the object.
*/
void Translucent_manager::CalculateDepthVal( SphereBound sphereBound)
{
  if ( !m_translucentList.empty() &&  m_translPtr != NULL)
  {
    Matrix4f m;
    Vector4f center = sphereBound.get_center();
    center[3] = 1.0f;
    Vector4f origin( 0.0f, 0.0f, 0.0f, 1.0f );

    m.Set(m_currentModelviewMat);
    center.Xform( center, m );
    float distance = center.SqrDistance( origin );
    m_translPtr->SetDepthValue( distance );
  }
}

/** used for sorting the translucent object.
*/
bool Greater (Translucent* Obj1, Translucent* Obj2)
{
  if( Obj1->get_depthVal() >= Obj2->get_depthVal())
    return true;

  return false;
}

void Translucent_manager::Sort()
{
  ETranslucentQueIter qIter = m_tmpQue.begin();
  Translucent_iter tIter = m_translucentList.begin();

  if ( !m_translucentList.empty() )
  {
    while ( tIter != m_translucentList.end() )
    {
      m_tmpQue.push_back( (*tIter) );
      tIter++;
      qIter++;
    }

    std::sort( m_tmpQue.begin(), m_tmpQue.end(), Greater);

    qIter = m_tmpQue.begin();
    tIter = m_translucentList.begin();
    while ( qIter != m_tmpQue.end() )
    {
      (*tIter) = (*qIter);
      tIter++;
      qIter++;
    }

    m_tmpQue.erase(m_tmpQue.begin(), m_tmpQue.end());
  }
}

/** Draw the (depth) sorted translucent objects with it's lights.
*/  
void Translucent_manager::Draw(Draw_action* drawA)
{
  Shape * shape = NULL;
  Translucent_iter prvtIter, nexttIter, tIter;

  int ind = 0;
  int size = m_translucentList.size();

  tIter = m_translucentList.begin();
  while( tIter != m_translucentList.end() )
  {
    shape = (*tIter)->get_translucentShape();
    if ( shape ) {

      prvtIter = nexttIter = tIter;
      prvtIter--; nexttIter++;
      if(  tIter == m_translucentList.begin() )
      {
        (*tIter)->DrawLights(drawA, NULL);
      }
      else (*tIter)->DrawLights(drawA, (*prvtIter));

      glLoadMatrixf( (*tIter)->get_fixedTransform() );
      shape->Draw(drawA);

      if( nexttIter == m_translucentList.end() )
      {
        (*tIter)->DisableLights(drawA, NULL);
      }
      else (*tIter)->DisableLights(drawA, (*nexttIter));
    }
    tIter++;
  }
}
