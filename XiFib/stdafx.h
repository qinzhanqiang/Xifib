
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


extern BYTE myWByte[1024];	//存放写数据
extern	long dataWLen;	//发送数据的长度
extern	BYTE myRByte[1024]; //存放读数据
extern	LPCTSTR myCom;	//串口名称
extern	BYTE myParity;	//奇偶校验类型
extern	DWORD myfParity;	//是否使用奇偶校验
extern	DWORD myBaudRate;	//波特率
extern	bool bIsOpened;		//串口打开标志

extern bool blnOpened;

extern	CEdit m_Send;
extern	CString receiveData;
extern	CString sendData;

extern HANDLE hCom;	//串口句柄
extern HANDLE hThreadEvent;
extern DWORD WINAPI ThreadProcEvent(LPVOID pParam);
extern bool fEventRun;
extern HWND hWnd;	
extern DWORD dwThreadID;
extern bool fStopMsg;

extern volatile double LhalfW;//短轴长度
extern volatile double halfW;//长轴长度
extern volatile double g_Px;
extern volatile double g_Py;
extern volatile double Distance;
extern volatile double Dist;
extern volatile bool next_ok;


extern DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter);

extern HANDLE g_hMutex;
extern volatile BOOL m_cRun;

extern bool FiberModelFlag;//光纤型号，true为圆光纤，false为楔形光纤

extern volatile double PartialAng;