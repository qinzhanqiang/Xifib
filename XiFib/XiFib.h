
// XiFib.h : XiFib Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "XiPlayPane.h"
#include "CommandLineParser.h"

// CXiFibApp:
// �йش����ʵ�֣������ XiFib.cpp
//

class CXiFibApp : public CWinAppEx
{
public:
	CXiFibApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	
	XiPlayPane *wDlg;
	CommandLineParser cmdInfo;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CXiFibApp theApp;
