#pragma once
#include "stdafx.h"

// CSCIDialog �Ի���

class CSCIDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSCIDialog)

public:
	CSCIDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSCIDialog();

// �Ի�������
	enum { IDD = IDD_SCI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_Com;
	int m_BaudRate;
	int m_Parity;
};
//extern LPCTSTR myCom;	//��������
//extern BYTE myParity;	//��żУ������
//extern DWORD myfParity;	//�Ƿ�ʹ����żУ��
//extern DWORD myBaudRate;	//������
