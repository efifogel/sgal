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
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

// Remote_controller.cpp: implementation of the Remote_controller class.
//
//////////////////////////////////////////////////////////////////////

#include "SGAL/basic.hpp"
#include "Scene_graphInt.h"
#include <COMDEF.H>

#include "Remote_controller.h"


#include "EnbTypes.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Rotation.h"
#include "FieldTypesEnum.h"
#include "SAIFieldSpecificServicesTemplate.h"
#include "SAINodeServices.h"

using namespace SGAL;

typedef SAI_fieldSpecificServicesTemplate<Boolean,SFBOOL> ESABoolean;
typedef SAI_fieldSpecificServicesTemplate<Float,SFFLOAT> ESAFloat;
typedef SAI_fieldSpecificServicesTemplate<Scene_time,SFTIME> ESATime;
typedef SAI_fieldSpecificServicesTemplate<Int,SFINT32> ESAInt;
typedef SAI_fieldSpecificServicesTemplate<Vector2f,SFVEC2F> ESAVector2f;
typedef SAI_fieldSpecificServicesTemplate<Vector3f,SFVEC3F> ESAVector3f;
typedef SAI_fieldSpecificServicesTemplate<Rotation,SFROTATION> ESARotation;
typedef SAI_fieldSpecificServicesTemplate<Vector4f,SFVEC4F> ESAVector4f;
typedef SAI_fieldSpecificServicesTemplate<String,SFSTRING> ESAString;
typedef SAI_fieldSpecificServicesTemplate<SAI_node_services*,SFNODE> ESASAINodeServices;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Remote_controller::Remote_controller(Scene_graphInt *pScene)
{
  m_pScene = pScene;
  assert(m_pScene);
  m_SAIServices = NULL;

  m_dX = 0;
  m_dY = 0;
  m_dZ = 0;

  m_rotaionX = 0.0;
  m_rotaionY = 0.0;

  m_pSAIServices = NULL;
  m_pSAIFieldTranslate = NULL;
  m_pSAIFieldRotate = NULL;

  m_timerOn = false;

  m_pRemoteKeys = NULL;
}

/*! Destructor */
Remote_controller::~Remote_controller() {}

unsigned Remote_controller::ThreadHandlerProc(void)
{
  if (!Init("10.0.0.137",5000))
    MessageBox(NULL,"Error in initializing socket","Remote_controller",MB_OK);
//  else
//    MessageBox(NULL,"socket init OK","Remote_controller",MB_OK);

  m_pSAIServices = get_sAIServices();
  SAI_node_services* pSAINodeServices = NULL;

  pSAINodeServices = m_pSAIServices->get_node("NAVIGATION ROOT");
  if ( !pSAINodeServices ) {
    std::string msg = "NAVIGATION ROOT Remote_controller: Invalid node name";
    MessageBox(NULL,msg,"",MB_OK);
    return THREAD_KILLED_EXIT_CODE;
  }

  m_pSAIFieldTranslate = pSAINodeServices->get_field("translation");
  if ( !m_pSAIFieldTranslate ) {
    std::string msg = "translation Remote_controller: Invalid field name";
    MessageBox(NULL,msg,"",MB_OK);
    return THREAD_KILLED_EXIT_CODE;
  }
  m_pSAIFieldRotate = pSAINodeServices->get_field("rotation");
  if ( !m_pSAIFieldRotate ) {
    std::string msg = "rotation Remote_controller: Invalid field name";
    MessageBox(NULL,msg,"",MB_OK);
    return THREAD_KILLED_EXIT_CODE;
  }

  StartMonitoring();

  return THREAD_NORMAL_EXIT_CODE;
}

void Remote_controller::Start()
{
  m_pThread = new TThread(this);
  ASSERT(m_pThread);
}

void Remote_controller::CheckKill()
{
}

ESAIServices* Remote_controller::get_sAIServices()
{
  ESAIServices* pSAIServices = NULL;
  RELEASE_ONLY(try)
  {
    if (m_SAIServices == NULL)
    {
      if (m_pScene)
      {
        pSAIServices = m_pScene->CreateSAI();
        if (pSAIServices == NULL)
        {
          assert(0);
          return NULL;
        }
        m_SAIServices = pSAIServices;
      }
      else
      {
        assert(0);
      }
    }
    else
    {
      pSAIServices = m_SAIServices;
    }
  }
  RELEASE_ONLY(
    catch (...)
    {
      pSAIServices = NULL;
    }
  )

  return pSAIServices;
}

void Remote_controller::SetSpecificValue(const char* pValue, SAI_fieldServices* pSAIFieldServices)
{
  RELEASE_ONLY(try)
  {
    bool error = false;
    String Del = " ";
    Int Type = pSAIFieldServices->get_type();
    //_bstr_t pBstrValue(pBValue);
    //char* pValue = pBstrValue;

    switch(Type)
    {
      case SFBOOL:
      {
        ESABoolean* pTSAIField = dynamic_cast<ESABoolean*>(pSAIFieldServices);
        //Boolean RValue = (String(_strupr(pValue))=="TRUE"); // case insensitive
        Boolean RValue = (String(pValue)=="TRUE"); // case insensitive
        pTSAIField->set_value(RValue);
        break;
      }
      case SFFLOAT:
      {
        ESAFloat* pTSAIField = dynamic_cast<ESAFloat*>(pSAIFieldServices);
        char *mask = "0123456789.eE-+";
        if ( strspn(pValue, mask) != strlen(pValue) ) error = true;
        Float RValue = atof(pValue);
        pTSAIField->set_value(RValue);
        break;
      }
      case SFTIME:
      {
        ESATime* pTSAIField = dynamic_cast<ESATime*>(pSAIFieldServices);
        Scene_time RValue = atof(pValue);
        pTSAIField->set_value(RValue);
        break;
      }
      case SFINT32:
      {
        ESAInt* pTSAIField = dynamic_cast<ESAInt*>(pSAIFieldServices);
        char *mask = "0123456789.eE-+";
        if ( strspn(pValue, mask) != strlen(pValue) ) error = true;
        Int RValue = atoi(pValue);
        pTSAIField->set_value(RValue);
        break;
      }
      case SFVEC2F:
      {
        ESAVector2f* pTSAIField = dynamic_cast<ESAVector2f*>(pSAIFieldServices);
        FloatVector Vals;
        String(pValue).getFloatArray( Del,Vals );
        if ( Vals.size() != 2 ) error = true;
        assert(Vals.size() == 2);
        Vector2f RValue( Vals[0], Vals[1] );
        pTSAIField->set_value(RValue);
        break;
      }
      case SFVEC3F:
      {
        ESAVector3f* pTSAIField = dynamic_cast<ESAVector3f*>(pSAIFieldServices);
        FloatVector Vals;
        String(pValue).getFloatArray( Del,Vals );
        if ( Vals.size() != 3 ) error = true;
        assert(Vals.size() == 3);
        Vector3f RValue( Vals[0], Vals[1], Vals[2] );
        pTSAIField->set_value(RValue);
        break;
      }
      case SFROTATION:
      {
        ESARotation* pTSAIField = dynamic_cast<ESARotation*>(pSAIFieldServices);
        FloatVector Vals;
        String(pValue).getFloatArray( Del,Vals );
        if ( Vals.size() != 4 ) error = true;
        assert(Vals.size() == 4); // (x, y, z, radians)
        Rotation RValue( Vals[0], Vals[1],Vals[2],Vals[3] );
        pTSAIField->set_value(RValue);
        break;
      }
      case SFVEC4F:
      {
        ESAVector4f* pTSAIField = dynamic_cast<ESAVector4f*>(pSAIFieldServices);
        FloatVector Vals;
        String(pValue).getFloatArray( Del,Vals );
        if ( Vals.size() != 4 ) error = true;
        assert(Vals.size() == 4);
        Vector4f RValue( Vals[0],Vals[1], Vals[2],Vals[3] );
        pTSAIField->set_value(RValue);
        break;
      }
      case SFSTRING:
      {
        ESAString* pTSAIField = dynamic_cast<ESAString*>(pSAIFieldServices);
        String RValue(pValue);
        pTSAIField->set_value(RValue);
        break;
      }
      case SFNODE:
      {
        ESASAINodeServices* pTSAIField = dynamic_cast<ESASAINodeServices*>(pSAIFieldServices);
        SAI_node_services* pSAINodeServices = get_sAIServices()->get_node(pValue);
        if (pSAINodeServices != NULL)
        {
          pTSAIField->set_value(pSAINodeServices);
        } else {
          error = true;
        }
        break;
      }
      default:
        assert(0); // Type still not yet implemented
    }
    if ( error ) {
      //std::string msg;
      //msg.format("%s|%s", pValue,"SetFieldValue: Invalid value");
      //SendError(etInvalidName, msg);
      assert(0); // Type still not yet implemented
    }

  }
  RELEASE_ONLY(
    catch (...)
    {
    }
  )
}

void Remote_controller::OnData(char *inBuff,int len)
{
  if (inBuff)
  {
    String inMessage = inBuff;

    int change =5;
    int rotChange =1;

    if (inMessage == "1")
    {
      ExecuteKey(inMessage);
/*      m_dY += change;
      m_dX -= change;
      Translate(m_dX,m_dY,m_dZ);*/
    }

    if (inMessage == "2")
    {
      ExecuteKey(inMessage);
/*      m_rotaionX -= rotChange;
      Rotate(1,0,0,m_rotaionX);*/
    }

    if (inMessage == "3")
    {
      ExecuteKey(inMessage);
/*      m_dX += change;
      m_dY += change;
      Translate(m_dX,m_dY,m_dZ);*/
    }

    if (inMessage == "4")
    {
      ExecuteKey(inMessage);
/*      m_rotaionY -= rotChange;
      Rotate(0,1,0,m_rotaionY);*/
    }

    if (inMessage == "5")
    {
      ExecuteKey(inMessage);
/*      m_dX = 0;
      m_dY = 0;
      m_dZ = 0;

      m_rotaionX = 0;
      m_rotaionY = 0;
      Rotate(0,0,0,0);
      Translate(0,0,0);*/
    }

    if (inMessage == "6")
    {
      ExecuteKey(inMessage);
/*      m_rotaionY += rotChange;
      Rotate(0,1,0,m_rotaionY);*/
    }
    if (inMessage == "7")
    {
      ExecuteKey(inMessage);
/*      m_dX -= change;
      m_dY -= change;
      Translate(m_dX,m_dY,m_dZ);*/
    }
    if (inMessage == "8")
    {
      ExecuteKey(inMessage);
/*      m_rotaionX += rotChange;
      Rotate(1,0,0,m_rotaionX);*/
    }
    if (inMessage == "9")
    {
      ExecuteKey(inMessage);
/*      m_dX += change;
      m_dY -= change;
      Translate(m_dX,m_dY,m_dZ);*/
    }
    if (inMessage == "*")
    {
      ExecuteKey(inMessage);
/*      m_dZ -= change;
      Translate(m_dX,m_dY,m_dZ);*/
    }
    if (inMessage == "0")
    {
      ExecuteKey(inMessage);
/*      SAI_node_services* pSAINodeServices = NULL;
      SAI_fieldServices* m_pSAIFieldStart = NULL;

      pSAINodeServices = m_pSAIServices->get_node("LidTouchSensor-SENSOR");
      if ( !pSAINodeServices ) {
        std::string msg = "LidTimeSensor-TIMER Remote_controller: Invalid node name";
        MessageBox(NULL,msg,"",MB_OK);
        return ;
      }

      m_pSAIFieldStart = pSAINodeServices->get_field("exActivate");
      if ( !m_pSAIFieldStart ) {
        std::string msg = "startTime Remote_controller: Invalid field name";
        MessageBox(NULL,msg,"",MB_OK);
        return ;
      }
      std::string pValue;
      pValue.format("true");
      m_pSAIServices->BeginUpdate();
      SetSpecificValue(pValue.c_str(), m_pSAIFieldStart);
      m_pSAIServices->EndUpdate();*/
/*      if (m_timerOn)
      {
        KillTimer(REMOTE_TIMER_ID);
        m_timerOn = false;
      }
      else
      {
        SetTimer(REMOTE_TIMER_ID,REMOTE_TIMER_INTERVAL);
        m_timerOn = true;
      }*/


    }
    if (inMessage == "#")
    {
      ExecuteKey(inMessage);
/*      m_dZ += change;
      Translate(m_dX,m_dY,m_dZ);*/
    }
    if (inMessage == "M")
    {
      ExecuteKey(inMessage);
/*      m_dZ += change;
      Translate(m_dX,m_dY,m_dZ);*/
    }

  }
}

void Remote_controller::Translate(int dx,int dy,int dz)
{
  std::string pValue;
  pValue.format("%d %d %d",dx,dy,dz);
  m_pSAIServices->BeginUpdate(); // Begin and End Update automatic in this stage
  SetSpecificValue(pValue.c_str(), m_pSAIFieldTranslate);
  m_pSAIServices->EndUpdate();
}

void Remote_controller::Rotate(int dx,int dy,int dz,int deg)
{
  double pi = 3.1415926535897932384626433832795;
  double rad = (double)deg*pi/180.0;
  std::string pValue;
  pValue.format("%d %d %d %f",dx,dy,dz,rad);
  m_pSAIServices->BeginUpdate(); // Begin and End Update automatic in this stage
  SetSpecificValue(pValue.c_str(), m_pSAIFieldRotate);
  m_pSAIServices->EndUpdate();
}

void Remote_controller::SetField(String nodeName, String fieldName,String value)
{
  SAI_node_services* pSAINodeServices = NULL;
  SAI_fieldServices* m_pSAIFieldStart = NULL;

  pSAINodeServices = m_pSAIServices->get_node(nodeName);
  if ( !pSAINodeServices ) {
    std::string msg = "LidTimeSensor-TIMER Remote_controller: Invalid node name";
    MessageBox(NULL,msg,"",MB_OK);
    return ;
  }

  m_pSAIFieldStart = pSAINodeServices->get_field(fieldName);
  if ( !m_pSAIFieldStart ) {
    std::string msg = "startTime Remote_controller: Invalid field name";
    MessageBox(NULL,msg,"",MB_OK);
    return ;
  }
  m_pSAIServices->BeginUpdate();
  SetSpecificValue(value.c_str(), m_pSAIFieldStart);
  m_pSAIServices->EndUpdate();
}

void Remote_controller::OnTimer(UINT nIDEvent)
{
  if (nIDEvent == REMOTE_TIMER_ID)
  {
    m_rotaionY += 10;
    Rotate(0,1,0,m_rotaionY);
  }
}

void Remote_controller::SetRemoteKeys(Container *pContainer)
{
  ERemoteKeys *m_pTemp = NULL;
  if (m_pTemp = dynamic_cast<ERemoteKeys *>(pContainer))
  {
    m_pRemoteKeys = m_pTemp;
  }
}

void Remote_controller::ExecuteKey(String keyStr)
{
  if (m_pRemoteKeys)
  {
    EKeyAction *pTemp = m_pRemoteKeys->get_keyAction(keyStr);
    if (pTemp)
    {
      EKARotate *pkaRotate = NULL;
      EKATranslate *pkaTranslate = NULL;
      EKAField *pkaField = NULL;
      EKASwitchFields *pkaSwitchFields = NULL;
      EKAReset *pkaReset = NULL;
      if ((pkaRotate = dynamic_cast<EKARotate*>(pTemp))!=NULL)
      {
        if (pkaRotate->get_vX() == 1)
        {
          m_rotaionX += pkaRotate->get_angle();
          Rotate(1,0,0,m_rotaionX);
        }
        if (pkaRotate->get_vY() == 1)
        {
          m_rotaionY += pkaRotate->get_angle();
          Rotate(0,1,0,m_rotaionY);
        }
      } else if ((pkaTranslate = dynamic_cast<EKATranslate*>(pTemp))!=NULL)
      {
        m_dX += pkaTranslate->get_dX();
        m_dY += pkaTranslate->get_dY();
        m_dZ += pkaTranslate->get_dZ();
        Translate(m_dX,m_dY,m_dZ);
      } else if ((pkaField = dynamic_cast<EKAField*>(pTemp))!=NULL)
      {
        SetField(pkaField->get_nodeName(),pkaField->get_fieldName(),pkaField->get_value());
      } else if ((pkaSwitchFields = dynamic_cast<EKASwitchFields*>(pTemp))!=NULL)
      {
        SetField(pkaSwitchFields->get_nodeName(),pkaSwitchFields->get_fieldName(),pkaSwitchFields->get_value());
        pkaSwitchFields->SwitchFields();
      } else if ((pkaReset = dynamic_cast<EKAReset*>(pTemp))!=NULL)
      {
        m_dX = 0;
        m_dY = 0;
        m_dZ = 0;

        m_rotaionX = 0;
        m_rotaionY = 0;
        Rotate(0,0,0,0);
        Translate(0,0,0);
      }
    }
  }
}
