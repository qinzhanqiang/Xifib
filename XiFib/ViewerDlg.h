// ViewerDlg.h : header file
//
#include "Setup.h"

#pragma once

#define NUMBER_ROWS	32


// CViewerDlg dialog
class CViewerDlg : public CDialog
{
// Construction
public:
	CViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//enum { IDD = IDD_VIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	HANDLE hMV; //Handle to device
	CSetup     * sdlg; //Pointer to SETUP dialog class
	BOOL	     sdlgOpened;
	char *	note[NUMBER_ROWS];//pointer to note buffer	
	int number_of_rows;
	int font_size;
	CFont font;

	BOOL  isRunWithSN;
	char runSN[MAX_PATH];
	DWORD devIndex;
	BOOL  auto_bandwidth_measure_enabled;
	int   bandwidth_limit_MBPS;
	BOOL  bandwidth_limit_enabled;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual BOOL DestroyWindow();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	
	void RefreshWindowText();
	void CloseCurrentDevice();		
	void LoadPresetsFromReg(void* handle);
	void SavePresetsToReg(void* handle);
};

