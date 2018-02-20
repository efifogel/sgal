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

// ERemoteController.h: interface for the ERemoteController class.

#include "SGAL/basic.hpp"
#include "Thread.h"
#include "SocketUtil.h"
#include "RemoteKeys.h"
#include "KeyAction.h"

#define REMOTE_TIMER_INTERVAL  100 
#define REMOTE_TIMER_ID      2

SGAL_BEGIN_NAMESPACE

class ERemoteController : public TThreadHandler, public ESocketUtil {
public:
  ERemoteController(Scene_graphInt *pScene);
  virtual ~ERemoteController();

  unsigned ThreadHandlerProc(void);
  void Start();

  void OnData(char *inBuff = NULL,int len = 0);
  void OnTimer(UINT nIDEvent);

  void SetRemoteKeys(Container *pContainer);

protected:
  void CheckKill();
  ESAIServices* get_sAIServices();
  void SetSpecificValue(const char* pValue, SAI_fieldServices* pSAIFieldServices);

  void Translate(int dx = 0,int dy = 0,int dz = 0);
  void Rotate(int dx = 0,int dy = 0,int dz = 0,int deg = 0);
  void SetField(String nodeName, String fieldName,String value);


  void ExecuteKey(String keyStr);

private:
  TThread      *m_pThread;    
  Scene_graphInt  *m_pScene;
  ESAIServices  *m_SAIServices;

  int m_dX;
  int m_dY;
  int m_dZ;

  float m_rotaionX;
  float m_rotaionY;

  ESAIServices* m_pSAIServices;
  SAI_fieldServices* m_pSAIFieldTranslate;
  SAI_fieldServices* m_pSAIFieldRotate;

  bool m_timerOn;

  ERemoteKeys *m_pRemoteKeys;
};

SGAL_END_NAMESPACE
