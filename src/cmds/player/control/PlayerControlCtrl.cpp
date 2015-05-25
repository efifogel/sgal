// PlayerControlCtrl.cpp : Implementation of the CPlayerControlCtrl ActiveX Control class.

#include "stdafx.h"

#ifdef LEFT_BUTTON
#undef LEFT_BUTTON
#endif

#ifdef MIDDLE_BUTTON
#undef MIDDLE_BUTTON
#endif

#ifdef RIGHT_BUTTON
#undef RIGHT_BUTTON
#endif

#include "SGAL/basic.hpp"
#include "SGAL/Mouse_event.hpp"
#include "SGAL/Keyboard_event.hpp"
#include "SGAL/Passive_motion_event.hpp"
#include "SGAL/Motion_event.hpp"
#include "SGAL/Tick_event.hpp"

#include "PlayerControl.h"
#include "PlayerControlCtrl.h"
#include "PlayerControlPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#if (defined SGAL_USE_GLUT)
#include "SGLUT/Glut_window_manager.hpp"
#elif defined(_WIN32)
#include "SGAL/Windows_window_manager.hpp"
#else
#include "SGAL/X11_window_manager.hpp"
#endif

//#include "Player_scene.hpp"
#include "Player_option_parser.hpp"

#if (defined SGAL_USE_CGAL)
SGAL_BEGIN_NAMESPACE
extern void scgal_init();
SGAL_END_NAMESPACE
#endif

#define IDT_TIMER_0       WM_USER + 200

using namespace SGAL;

IMPLEMENT_DYNCREATE(CPlayerControlCtrl, COleControl)

// Message map

BEGIN_MESSAGE_MAP(CPlayerControlCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CPlayerControlCtrl, COleControl)
	DISP_FUNCTION_ID(CPlayerControlCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CPlayerControlCtrl, "Play", 3, Play, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CPlayerControlCtrl,"FileName", 2, GetFileName, SetFileName, VT_BSTR)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CPlayerControlCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CPlayerControlCtrl, 1)
	PROPPAGEID(CPlayerControlPropPage::guid)
END_PROPPAGEIDS(CPlayerControlCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPlayerControlCtrl, "PLAYERCONTROL.PlayerControlCtrl.1",
	0x47879b80, 0x6cbb, 0x432e, 0x92, 0xec, 0x26, 0x17, 0x44, 0x46, 0xac, 0xc5)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CPlayerControlCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DPlayerControl =
		{ 0xEF0DF460, 0xC17C, 0x4314, { 0xA2, 0x64, 0x79, 0xE6, 0xD8, 0x77, 0x95, 0x65 } };
const IID BASED_CODE IID_DPlayerControlEvents =
		{ 0x70CA8E21, 0xEBA9, 0x4DD6, { 0xA9, 0x4C, 0x62, 0x53, 0x31, 0x16, 0xD7, 0x76 } };



// Control type information

static const DWORD BASED_CODE _dwPlayerControlOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPlayerControlCtrl, IDS_PLAYERCONTROL, _dwPlayerControlOleMisc)


/*! Has the shift key pressed */
Boolean CPlayerControlCtrl::s_capital(false);

// CPlayerControlCtrl::CPlayerControlCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPlayerControlCtrl

BOOL CPlayerControlCtrl::CPlayerControlCtrlFactory::
UpdateRegistry(BOOL bRegister)
{
  // TODO: Verify that your control follows apartment-model threading rules.
  // Refer to MFC TechNote 64 for more information.
  // If your control does not conform to the apartment-model rules, then
  // you must modify the code below, changing the 6th parameter from
  // afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

  if (bRegister)
    return AfxOleRegisterControlClass(AfxGetInstanceHandle(),
                                      m_clsid,
                                      m_lpszProgID,
                                      IDS_PLAYERCONTROL,
                                      IDB_PLAYERCONTROL,
                                      afxRegInsertable | afxRegApartmentThreading,
                                      _dwPlayerControlOleMisc,
                                      _tlid,
                                      _wVerMajor,
                                      _wVerMinor);
  else
    return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CPlayerControlCtrl::CPlayerControlCtrl - Constructor

CPlayerControlCtrl::CPlayerControlCtrl() :
  _bPlay(false),
  _tim_number(0),
  m_start_tick_time(0),
  m_sim_time(0),
  m_est_tick_duration(0),
  m_required_tick_duration(0)
{
  InitializeIIDs(&IID_DPlayerControl, &IID_DPlayerControlEvents);
  // TODO: Initialize your control's instance data here.
}



// CPlayerControlCtrl::~CPlayerControlCtrl - Destructor

CPlayerControlCtrl::~CPlayerControlCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CPlayerControlCtrl::OnDraw - Drawing function

void CPlayerControlCtrl::OnDraw(CDC* pdc, const CRect& rcBounds,
                                const CRect& rcInvalid)
{
  if (!pdc) return;

  BOOL ui = AmbientUIDead();
  if (_bPlay == false || ui == TRUE) {
    pdc->FillRect(rcBounds,
                  CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
  }
  else {
    _window_item.set_redraw(SGAL_FALSE);
    _scene.draw_window(&_window_item, m_button_state != 0);
  }

  //if (!IsOptimizedDraw())
  //{
  //	// The container does not support optimized drawing.

  //	// TODO: if you selected any GDI objects into the device context *pdc,
  //	//		restore the previously-selected objects here.
  //}
}



// CPlayerControlCtrl::DoPropExchange - Persistence support

void CPlayerControlCtrl::DoPropExchange(CPropExchange* pPX)
{
  ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
  COleControl::DoPropExchange(pPX);

  // TODO: Call PX_ functions for each persistent custom property.
  if (pPX->GetVersion() == (DWORD)MAKELONG(_wVerMinor, _wVerMajor))
    PX_String(pPX, _T("FileName"), _fileName, _T(""));
  else if (pPX->IsLoading()) {
    // Skip over persistent data
    CString strDummy;
    PX_String(pPX, _T("FileName"), strDummy, _T(""));

    // Force property values to these defaults
    _fileName = _T("");
  }
}

// CPlayerControlCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
DWORD CPlayerControlCtrl::GetControlFlags()
{
  DWORD dwFlags = COleControl::GetControlFlags();

  // The control's output is not being clipped.
  // The control guarantees that it will not paint outside its
  // client rectangle.
  dwFlags &= ~clipPaintDC;

  // The control will not be redrawn when making the transition
  // between the active and inactivate state.
  dwFlags |= noFlickerActivate;

  // The control can optimize its OnDraw method, by not restoring
  // the original GDI objects in the device context.
  dwFlags |= canOptimizeDraw;
  return dwFlags;
}



// CPlayerControlCtrl::OnResetState - Reset control to default state

void CPlayerControlCtrl::OnResetState()
{
  COleControl::OnResetState();  // Resets defaults found in DoPropExchange

  // TODO: Reset any other control state here.
}



// CPlayerControlCtrl::AboutBox - Display an "About" box to the user

void CPlayerControlCtrl::AboutBox()
{
  CDialog dlgAbout(IDD_ABOUTBOX_PLAYERCONTROL);
  dlgAbout.DoModal();
}

// Custom properties
BSTR CPlayerControlCtrl::GetFileName()
{
  return _fileName.AllocSysString();
}

void CPlayerControlCtrl::SetFileName(LPCTSTR tstrFileName)
{
  if (_fileName != tstrFileName)
  {
    _fileName = tstrFileName;
    SetModifiedFlag();
    InvalidateControl();
  }
}

// CPlayerControlCtrl message handlers


// COleControl overrides
void CPlayerControlCtrl::Play()
{
  try {

#if	(defined SGAL_USE_CGAL)
    SGAL::scgal_init();
#endif
  }
  catch(std::exception & e)
  {
    MessageBox(e.what(), "Exception");
    return;
  }

  //// Create a window manager:
  //SGAL::Windows_window_manager * wm	= SGAL::Windows_window_manager::instance();

  // Parse program options:
  try
  {
    _option_parser.init();
    TCHAR *argv[2];
    argv[0] = _T("player");
    //		argv[1] = _T("D:\\home\\ophirset\\trees\\tcgl\\trunk\\data\\vrml\\tests\\Geometry\\Cylinder\\default.wrl");
    argv[1] = _fileName.GetBuffer();

    _option_parser(2, argv);
    _option_parser.apply();
  }
  catch(std::exception & e) {
    MessageBox(e.what(), "Exception");
    return;
  }

  try {
    // Initialize the scene:
    _scene.set_option_parser(&_option_parser);
    _scene.set_window_manager(this);
    if (_scene.is_scene_initiated())
    {
      _scene.clear_scene();
      _scene.destroy_scene();
    }
    _scene.create_scene();
  }
  catch (std::exception & e)
  {
    MessageBox(e.what(), "Exception");
    return;
  }

  try
  {
    //		this->init(static_cast<SGAL::Uint>(argc), argv);
    _scene.init_scene();
  }
  catch(std::exception & e)
  {
    MessageBox(e.what(), "Exception");
    return;
  }

  //	if (_tim_number != 0)
  //		KillTimer(_tim_number);
  _tim_number = SetTimer(IDT_TIMER_0, CLOCKS_PER_SEC / 30, NULL);
  _bPlay = true;
}

BOOL CPlayerControlCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
                                  LRESULT* pResult)
{
  BOOL ret = CWnd::OnWndMsg(message, wParam, lParam, pResult);

  Mouse_event::Mouse_button button;
  Mouse_event::Press state;
  Boolean pressed;
  Mouse_event * mouse_event;
  Keyboard_event * keyboard_event;
  Motion_event * motion_event;
  Passive_motion_event * passive_motion_event;

  switch (message) {
   case WM_SIZE:
    if (_bPlay) {
      // In X11 we check if the size really changed.
      _window_item.set_size(LOWORD(lParam), HIWORD(lParam));
      _window_item.make_current();
      _scene.reshape_window(&_window_item, LOWORD(lParam), HIWORD(lParam));
    }
    break;

   case WM_KEYDOWN:
    switch (wParam) {
     case VK_SHIFT: s_capital = !s_capital; break;
     case VK_CAPITAL: s_capital = true; break;
     default:
      pressed = true;

process_key:
      keyboard_event = new Keyboard_event;
      keyboard_event->set_window_item(&_window_item);
      keyboard_event->set_key((s_capital) ? wParam : wParam + 32);
      keyboard_event->set_x(LOWORD(lParam));
      keyboard_event->set_y(_window_item.m_height - (HIWORD(lParam)));
      keyboard_event->set_pressed(pressed);
      s_event_handler.issue(keyboard_event);
      _window_item.set_redraw(SGAL_TRUE);
      InvalidateControl();
      break;
    }
    break;

   case WM_KEYUP:
    switch (wParam) {
     case VK_SHIFT: s_capital = !s_capital; break;
     case VK_CAPITAL: s_capital = false; break;
     default:
      pressed = SGAL_FALSE;
      goto process_key;
    }
    break;

   case WM_LBUTTONDOWN:
    button = Mouse_event::LEFT_BUTTON;
    state = Mouse_event::DOWN;
    m_button_state |= (0x1 << 1);

process_mouse:
    mouse_event = new Mouse_event;
    mouse_event->set_window_item(&_window_item);
    mouse_event->set_x(LOWORD(lParam));
    mouse_event->set_y(_window_item.m_height - (HIWORD(lParam)));
    mouse_event->set_button(button);
    mouse_event->set_state(state);
    s_event_handler.issue(mouse_event);
    _window_item.set_redraw(SGAL_TRUE);
    InvalidateControl();
    break;

   case WM_LBUTTONUP:
    button = Mouse_event::LEFT_BUTTON;
    state = Mouse_event::UP;
    m_button_state &= ~(0x1 << 1);
    goto process_mouse;

   case WM_MBUTTONDOWN:
    button = Mouse_event::MIDDLE_BUTTON;
    state = Mouse_event::DOWN;
    m_button_state |= (0x1 << 2);
    goto process_mouse;

   case WM_MBUTTONUP:
    button = Mouse_event::MIDDLE_BUTTON;
    state = Mouse_event::UP;
    m_button_state &= ~(0x1 << 2);
    goto process_mouse;

   case WM_RBUTTONDOWN:
    button = Mouse_event::RIGHT_BUTTON;
    state = Mouse_event::DOWN;
    m_button_state |= (0x1 << 3);
    goto process_mouse;

   case WM_RBUTTONUP:
    button = Mouse_event::RIGHT_BUTTON;
    state = Mouse_event::UP;
    m_button_state &= ~(0x1 << 3);
    goto process_mouse;

   case WM_MOUSEMOVE:
    if (m_button_state == 0) {
      passive_motion_event = new Passive_motion_event;
      passive_motion_event->set_window_item(&_window_item);
      passive_motion_event->set_x(LOWORD(lParam));
      passive_motion_event->set_y(_window_item.m_height - (HIWORD(lParam)));
      s_event_handler.issue(passive_motion_event);
    }
    else {
      motion_event = new Motion_event;
      motion_event->set_window_item(&_window_item);
      motion_event->set_x(LOWORD(lParam));
      motion_event->set_y(_window_item.m_height - (HIWORD(lParam)));
      s_event_handler.issue(motion_event);
    }
    _window_item.set_redraw(SGAL_TRUE);
    InvalidateControl();
    break;

   case WM_CAPTURECHANGED:
    // std::cout << "recieved WM_CAPTURECHANGED" << std::endl;
    break;
  }

  while (s_event_handler.is_empty() == false)
    s_event_handler.process();

  return ret;
}

// Window manager functions
void CPlayerControlCtrl::create_window(SGAL::Windows_window_item * window_item)
{
  OLE_HANDLE h = GetHwnd();
  HWND hWnd = reinterpret_cast<HWND> (h);
  HINSTANCE inst = GetModuleHandle(NULL);
  _window_item.create(inst, "control", hWnd);
}

void CPlayerControlCtrl::OnTimer (UINT TimerVal)
{
  if (_bPlay == false) return;

  //Chech whether simulation is required:
  if (_scene.is_simulating() == false) return;

  // Measure the ellapsed time:
  clock_t end_tick_time = clock();
  clock_t tick_duration = end_tick_time - m_start_tick_time;
  long sleep_time = m_required_tick_duration - tick_duration;
  // Slow down if necessary:
  if (sleep_time > 0) {
    _window_item.set_redraw(SGAL_TRUE);
    InvalidateControl();
  }
  else {
    // If any window is being accumulated, do not advance in the time line.
    if (!_window_item.is_accumulating()) {
      // Continue along the time line - issue a tick event:
      m_start_tick_time = end_tick_time;
      m_sim_time += tick_duration;
      SGAL::Tick_event * event = new SGAL::Tick_event;
      event->set_est_tick_duration(m_est_tick_duration);
      event->set_sim_time(m_sim_time);
      s_event_handler.issue(event);
      m_required_tick_duration = 0;

      _window_item.set_redraw(SGAL_TRUE);
      InvalidateControl();
    }
  }
}
