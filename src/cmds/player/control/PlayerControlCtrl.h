#pragma once
// PlayerControlCtrl.h : Declaration of the CPlayerControlCtrl ActiveX Control class.

#include "SGAL/Types.hpp"
#include "SGAL/Event_handler.hpp"

#include "Player_scene.hpp"
#include "Player_option_parser.hpp"
#include "SGAL/Windows_window_item.hpp"

// CPlayerControlCtrl : See PlayerControlCtrl.cpp for implementation.
class CPlayerControlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPlayerControlCtrl)

// Constructor
public:
	CPlayerControlCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();

// Override COleControl
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// window manager implementation
	virtual SGAL::Windows_window_item* create_window_item() { return &_window_item;}
	virtual void create_window(SGAL::Windows_window_item * window_item);
	virtual void destroy_window(SGAL::Windows_window_item * window_item) { _window_item.destroy(); }
	virtual void destroy_window_item(SGAL::Windows_window_item * window_item) { _window_item.destroy(); }

// Implementation
protected:
	~CPlayerControlCtrl();

	DECLARE_OLECREATE_EX(CPlayerControlCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPlayerControlCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPlayerControlCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CPlayerControlCtrl)		// Type name and misc status


	afx_msg void AboutBox();
	afx_msg BSTR GetFileName();
	afx_msg void SetFileName(LPCTSTR bstrFileName);
	afx_msg void Play();
	afx_msg void OnTimer(UINT TimerVal);

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum { DISPID_FILENAME = 2
	};

protected:
	/*! VRML file name */
	CString _fileName;
    
	/*! Options parser for options */
	Player_option_parser _option_parser;

	/*! The displayed scene */
	Player_scene _scene;

	/*! Window item for the control */
	SGAL::Windows_window_item _window_item;
	
	/*! Mouse button states */
	SGAL::Uint m_button_state;

	/*! Has the shift key pressed */
	static SGAL::Boolean s_capital;

	/*! The event handler */
	SGAL::Event_handler s_event_handler;

	/*! Play function was called */
	bool _bPlay;

	/*!  The system timer number assigned */
    UINT_PTR _tim_number;

	/*! The time when the current tick starts: */
	clock_t m_start_tick_time;

	/*! The accumulated simulation time */
    clock_t m_sim_time;

	/*! The estimated tick duration */
	clock_t m_est_tick_duration;

	/*! The required tick duration */
	clock_t m_required_tick_duration;

};

