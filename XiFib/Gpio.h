#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSetup dialog

class CGpio : public CDialog
{
	DECLARE_DYNAMIC(CGpio)

public:
	CGpio(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGpio();

// Dialog Data
	enum { IDD = IDD_GPIO };

	DWORD	dwExposureLUT[256];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	///////////////////////////////
	CComboBox m_gpi1;
	CComboBox m_gpi2;
	CComboBox m_gpi3;
	CComboBox m_gpi4;
	CComboBox m_gpo1;
	CComboBox m_gpo2;
	CComboBox m_gpo3;
	CComboBox m_gpo4;
};
