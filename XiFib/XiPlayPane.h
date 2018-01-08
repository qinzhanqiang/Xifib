#include "Setup.h"

#pragma once

#define NUMBER_ROWS	32

// XiPlayPane

class XiPlayPane : public CDockablePane
{
	DECLARE_DYNAMIC(XiPlayPane)

	//构造函数和析构函数
public:
	XiPlayPane();
	virtual ~XiPlayPane();

	//特性
public:
	//HICON m_hIcon;

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

	//操作和消息映射
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	
	void RefreshWindowText();
	void CloseCurrentDevice();		
	void LoadPresetsFromReg(void* handle);
	void SavePresetsToReg(void* handle);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};


