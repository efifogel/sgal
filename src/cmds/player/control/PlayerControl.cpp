// PlayerControl.cpp : Implementation of CPlayerControlApp and DLL registration.

#include "stdafx.h"
#include "PlayerControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPlayerControlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid = {
  0x5418AF6D, 0x20CE, 0x4FAE,
  { 0xBA, 0x36, 0x67, 0xB7, 0x3, 0xCF, 0x6C, 0x54 }
};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 1;

// CPlayerControlApp::InitInstance - DLL initialization

BOOL CPlayerControlApp::InitInstance()
{
  BOOL bInit = COleControlModule::InitInstance();

  if (bInit)
  {
    // TODO: Add your own module initialization code here.
  }

  return bInit;
}

// CPlayerControlApp::ExitInstance - DLL termination

int CPlayerControlApp::ExitInstance()
{
  // TODO: Add your own module termination code here.
  return COleControlModule::ExitInstance();
}

// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  AFX_MANAGE_STATE(_afxModuleAddrThis);

  if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
    return ResultFromScode(SELFREG_E_TYPELIB);

  if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
    return ResultFromScode(SELFREG_E_CLASS);

  return NOERROR;
}

// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
  AFX_MANAGE_STATE(_afxModuleAddrThis);

  if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
    return ResultFromScode(SELFREG_E_TYPELIB);

  if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
    return ResultFromScode(SELFREG_E_CLASS);

  return NOERROR;
}
