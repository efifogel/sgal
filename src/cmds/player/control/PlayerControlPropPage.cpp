// PlayerControlPropPage.cpp : Implementation of the CPlayerControlPropPage property page class.

#include "stdafx.h"
#include "PlayerControl.h"
#include "PlayerControlPropPage.h"
#include ".\playercontrolproppage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPlayerControlPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CPlayerControlPropPage, COlePropertyPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPlayerControlPropPage, "PLAYERCONTROL.PlayerControlPropPage.1",
	0xdedc36c7, 0xabe4, 0x4f78, 0x99, 0x5e, 0xc8, 0xfb, 0x1b, 0xc8, 0xfa, 0x16)



// CPlayerControlPropPage::CPlayerControlPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CPlayerControlPropPage

BOOL CPlayerControlPropPage::CPlayerControlPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PLAYERCONTROL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPlayerControlPropPage::CPlayerControlPropPage - Constructor

CPlayerControlPropPage::CPlayerControlPropPage() :
	COlePropertyPage(IDD, IDS_PLAYERCONTROL_PPG_CAPTION)
{
	SetHelpInfo(_T("Names to appear in the control"), _T("PlayerControl.HLP"), 0);
}



// CPlayerControlPropPage::DoDataExchange - Moves data between page and properties

void CPlayerControlPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_Text(pDX, IDC_EDIT_FILE_NAME, _fileName, _T("FileName"));
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, _fileName);
	DDP_PostProcessing(pDX);
}



// CPlayerControlPropPage message handlers

void CPlayerControlPropPage::OnBnClickedBrowse()
{
	char strFilter[] = { "VRML Files (*.wrl)|*.wrl|All Files (*.*)|*.*||" };
	CFileDialog FileDlg(FALSE, ".wrl", NULL, 0, strFilter);
	if (FileDlg.DoModal() == IDOK)
	{
		CEdit *edit = reinterpret_cast<CEdit *> (GetDlgItem(IDC_EDIT_FILE_NAME));
		edit->SetWindowText(FileDlg.GetPathName());
		UpdateData(TRUE);
	}
}
