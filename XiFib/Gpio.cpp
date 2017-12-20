// SetupMini.cpp : implementation file
//

#include "stdafx.h"
//#include "Viewer.h"
#include "XiFib.h"
#include "ViewerDlg.h"
#include "Gpio.h"
#include "acquisition.h"
#include <math.h>

extern LPSETUP lps;


// CGpio dialog

IMPLEMENT_DYNAMIC(CGpio, CDialog)

CGpio::CGpio(CWnd* pParent /*=NULL*/)
	: CDialog(CGpio::IDD, pParent)
{

}

CGpio::~CGpio()
{
}

void CGpio::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GPI1, m_gpi1);
	DDX_Control(pDX, IDC_GPI2, m_gpi2);
	DDX_Control(pDX, IDC_GPI3, m_gpi3);
	DDX_Control(pDX, IDC_GPI4, m_gpi4);
	DDX_Control(pDX, IDC_GPO1, m_gpo1);
	DDX_Control(pDX, IDC_GPO2, m_gpo2);
	DDX_Control(pDX, IDC_GPO3, m_gpo3);
	DDX_Control(pDX, IDC_GPO4, m_gpo4);
}


BEGIN_MESSAGE_MAP(CGpio, CDialog)

	ON_BN_CLICKED(IDOK, &CGpio::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGpio::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGpio message handlers

BOOL CGpio::OnInitDialog()
{
	CDialog::OnInitDialog();

	int gpiMax = 0, gpoMax = 0;
	xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR XI_PRM_INFO_MAX, &gpiMax);
	xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR XI_PRM_INFO_MAX, &gpoMax);

	int sel = 0;
	switch(gpiMax)
	{
	case 4:
		GetDlgItem(IDC_GPI4)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 4);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, &sel);
		m_gpi4.SetCurSel(sel);
	case 3:
		GetDlgItem(IDC_GPI3)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 3);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, &sel);
		m_gpi3.SetCurSel(sel);
	case 2:
		GetDlgItem(IDC_GPI2)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 2);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, &sel);
		m_gpi2.SetCurSel(sel);
	case 1:
		GetDlgItem(IDC_GPI1)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 1);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, &sel);
		m_gpi1.SetCurSel(sel);
	}

	switch(gpoMax)
	{
	case 4:
		GetDlgItem(IDC_GPO4)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 4);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, &sel);
		m_gpo4.SetCurSel(sel);
	case 3:
		GetDlgItem(IDC_GPO3)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 3);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, &sel);
		m_gpo3.SetCurSel(sel);
	case 2:
		GetDlgItem(IDC_GPO2)->EnableWindow(TRUE);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 2);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, &sel);
		m_gpo2.SetCurSel(sel);
	case 1:
		GetDlgItem(IDC_GPO1)->EnableWindow(TRUE);	
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 1);
		xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, &sel);
		m_gpo1.SetCurSel(sel);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGpio::OnBnClickedOk()
{
	char device_type[MAX_PATH];
	BOOL isUSB2 = FALSE;
	int gpiMax = 0, gpoMax = 0;
	xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR XI_PRM_INFO_MAX, &gpiMax);
	xiGetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR XI_PRM_INFO_MAX, &gpoMax);


	xiGetParamString( theApp.wDlg->hMV, XI_PRM_DEVICE_TYPE, device_type, MAX_PATH);
	if(!strcmp(device_type, "USB2.0")) isUSB2 = TRUE;

	int sel = 0;
	switch(gpiMax)
	{
	case 4:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 4);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, m_gpi4.GetCurSel());
	case 3:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 3);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, m_gpi3.GetCurSel());
	case 2:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 2);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, m_gpi2.GetCurSel());
	case 1:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_SELECTOR, 1);
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPI_MODE, m_gpi1.GetCurSel());
	}

	switch(gpoMax)
	{
	case 4:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 4);
		if(isUSB2){
			if(m_gpi4.GetCurSel() != XI_GPI_TRIGGER && m_gpi4.GetCurSel() != XI_GPI_EXT_EVENT) xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo4.GetCurSel());
		}else xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo4.GetCurSel());
	case 3:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 3);
		if(isUSB2){
			if(m_gpi3.GetCurSel() != XI_GPI_TRIGGER && m_gpi3.GetCurSel() != XI_GPI_EXT_EVENT) xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo3.GetCurSel());
		}else xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo3.GetCurSel());
	case 2:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 2);
		if(isUSB2){
			if(m_gpi2.GetCurSel() != XI_GPI_TRIGGER && m_gpi2.GetCurSel() != XI_GPI_EXT_EVENT) xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo2.GetCurSel());
		}else xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo2.GetCurSel());
	case 1:
		xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_SELECTOR, 1);
		if(isUSB2){
			if(m_gpi1.GetCurSel() != XI_GPI_TRIGGER && m_gpi1.GetCurSel() != XI_GPI_EXT_EVENT) xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo1.GetCurSel());
		}else xiSetParamInt( theApp.wDlg->hMV, XI_PRM_GPO_MODE, m_gpo1.GetCurSel());
	}

	OnOK();
}

void CGpio::OnBnClickedCancel()
{
	OnCancel();
}
