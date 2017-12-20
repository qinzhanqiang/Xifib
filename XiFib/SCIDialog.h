#pragma once
#include "stdafx.h"

// CSCIDialog 对话框

class CSCIDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSCIDialog)

public:
	CSCIDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSCIDialog();

// 对话框数据
	enum { IDD = IDD_SCI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Com;
	int m_BaudRate;
	int m_Parity;
};
//extern LPCTSTR myCom;	//串口名称
//extern BYTE myParity;	//奇偶校验类型
//extern DWORD myfParity;	//是否使用奇偶校验
//extern DWORD myBaudRate;	//波特率
