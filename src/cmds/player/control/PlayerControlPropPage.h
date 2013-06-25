#pragma once

// PlayerControlPropPage.h : Declaration of the CPlayerControlPropPage property page class.


// CPlayerControlPropPage : See PlayerControlPropPage.cpp for implementation.

class CPlayerControlPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPlayerControlPropPage)
	DECLARE_OLECREATE_EX(CPlayerControlPropPage)

// Constructor
public:
	CPlayerControlPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PLAYERCONTROL };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()

	CString _fileName;
public:
	afx_msg void OnBnClickedBrowse();
};

