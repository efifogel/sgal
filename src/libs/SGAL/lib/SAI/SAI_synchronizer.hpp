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
// $Revision: 1309 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/*!
 * SAI Synchronization with the scene graph
 *
 * This class is used to synchronize SAI activity perfomed by the thread
 * using the SAI and the scene graph activity 
 * (performed by the workcycle thread) - event cascade, rendering ...
 *
 * The synchronizing thread should use SynchronizeActivity() to let
 * the synchronized thread perfome the synchronized activity.
 * The synchronized thread should should perform the synchronized activity
 * between calls to StartSynchronizedSection() and EndSynchronizedSection().
 * SynchronizeActivity() will be over after the call to
 * EndSynchronizedSection().
 */

#ifndef EFI_SAI_SYNCHRONIZER_H
#define EFI_SAI_SYNCHRONIZER_H

#include "CritSec.h"

class ESAISynchronizer {
private:
  // TCriticalSection objects for managing the synchronization
  TCriticalSection m_lockerA;
  TCriticalSection m_lockerB;

public:
  // Constructor locks B - to make synchronized activity wait by default
  ESAISynchronizer()
  {
    // Locks locker B
    m_lockerB.Enter();
  }

  // Should be used by the synchronized thread before starting the synchronized 
  // section
  void StartSynchronizedSection()
  {
    // Lock A to signal synchronization is required
    m_lockerA.Enter();

    // Wait for B to be unlocked
    m_lockerB.Enter();
  }

  // Should be used by the synchronized thread after the synchronized 
  // section
  void EndSynchronizedSection()
  {
    // Unlock B
    m_lockerB.Leave();

    // Unlock A - to let the synchronizing thread continue execution-
    // signaling the synchronized activity has end
    m_lockerA.Leave();
  }

  // Should be used by the synchronizing thread to synchronize the other thread's
  // (synchronized) activity
  void SynchronizeActivity()
  {
    // Unlock B to let synchronized activity be executed
    m_lockerB.Leave();

    // Wait on locker A until synchronized activity is end
    m_lockerA.Enter();

    // Lock B for to make next synchronized activity wait
    m_lockerB.Enter();

    // Unlock A for next time
    m_lockerA.Leave();
  }
};

// Automatic calls to start and end synchronized section in the constructor and
// destructor of ESAIAutoSynchronized - can be used as a local variable in 
// synchronized functions
class ESAIAutoSynchronized {
private:
  ESAISynchronizer * m_synch;

public:
  ESAIAutoSynchronized(ESAISynchronizer *synch) : m_synch(synch)
  {
    if (m_synch) 
      m_synch->StartSynchronizedSection();
  }

  ~ESAIAutoSynchronized()
  {
    if (m_synch) 
      m_synch->EndSynchronizedSection();
  }
};

#endif
