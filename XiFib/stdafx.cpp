
// stdafx.cpp : 只包括标准包含文件的源文件
// XiFib.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "CEllipse.h"

CEdit m_Send;
CString receiveData;
CString sendData;

BYTE myWByte[1024];	//存放写数据
long dataWLen;	//发送数据的长度
BYTE myRByte[1024]; //存放读数据


LPCTSTR myCom;	//串口名称
BYTE myParity;	//奇偶校验类型
DWORD myfParity;	//是否使用奇偶校验
DWORD myBaudRate;	//波特率
bool bIsOpened;		//串口打开标志

bool blnOpened = false;

HANDLE hCom;	//串口句柄	
HANDLE hThreadEvent;//
HWND hWnd;//只线程

bool fEventRun;//线程
DWORD dwThreadID;//
bool fStopMsg;//线程 接收
DWORD WINAPI ThreadProcEvent(LPVOID pParam);//线程


//短轴长度
volatile double LhalfW=0;
//长轴长度
volatile double halfW=0;
//中心的坐标
volatile double g_Px;
volatile double g_Py;
//距离
volatile double Distance;
volatile double Dist=0.5;
volatile bool next_ok=false;


DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter);//准直线程

HANDLE g_hMutex = NULL;//互斥量

volatile BOOL m_cRun;//准直线程

//光纤型号，true为圆光纤，false为楔形光纤
bool FiberModelFlag;
//偏轴度
volatile double PartialAng;
