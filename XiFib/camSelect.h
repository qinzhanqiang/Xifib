#pragma once

#include "resource.h"
#include "afxwin.h"

#include "..\include\xiApi.h"

// camSelect dialog

class camSelect : public CDialog
{
	DECLARE_DYNAMIC(camSelect)

public:
	camSelect(CWnd* pParent = NULL);   // standard constructor
	virtual ~camSelect();

// Dialog Data
	enum { IDD = IDD_CAMSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpen();
	void GetIndex( PDWORD index);

	PDWORD selected_index;
};
