
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


extern BYTE myWByte[1024];	//���д����
extern	long dataWLen;	//�������ݵĳ���
extern	BYTE myRByte[1024]; //��Ŷ�����
extern	LPCTSTR myCom;	//��������
extern	BYTE myParity;	//��żУ������
extern	DWORD myfParity;	//�Ƿ�ʹ����żУ��
extern	DWORD myBaudRate;	//������
extern	bool bIsOpened;		//���ڴ򿪱�־

extern bool blnOpened;

extern	CEdit m_Send;
extern	CString receiveData;
extern	CString sendData;

extern HANDLE hCom;	//���ھ��
extern HANDLE hThreadEvent;
extern DWORD WINAPI ThreadProcEvent(LPVOID pParam);
extern bool fEventRun;
extern HWND hWnd;	
extern DWORD dwThreadID;
extern bool fStopMsg;

extern volatile double LhalfW;//���᳤��
extern volatile double halfW;//���᳤��
extern volatile double g_Px;
extern volatile double g_Py;
extern volatile double Distance;
extern volatile double Dist;
extern volatile bool next_ok;


extern DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter);

extern HANDLE g_hMutex;
extern volatile BOOL m_cRun;

extern bool FiberModelFlag;//�����ͺţ�trueΪԲ���ˣ�falseΪШ�ι���

extern volatile double PartialAng;