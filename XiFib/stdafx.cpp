
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// XiFib.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "CEllipse.h"

CEdit m_Send;
CString receiveData;
CString sendData;

BYTE myWByte[1024];	//���д����
long dataWLen;	//�������ݵĳ���
BYTE myRByte[1024]; //��Ŷ�����


LPCTSTR myCom;	//��������
BYTE myParity;	//��żУ������
DWORD myfParity;	//�Ƿ�ʹ����żУ��
DWORD myBaudRate;	//������
bool bIsOpened;		//���ڴ򿪱�־

bool blnOpened = false;

HANDLE hCom;	//���ھ��	
HANDLE hThreadEvent;//
HWND hWnd;//ֻ�߳�

bool fEventRun;//�߳�
DWORD dwThreadID;//
bool fStopMsg;//�߳� ����
DWORD WINAPI ThreadProcEvent(LPVOID pParam);//�߳�


//���᳤��
volatile double LhalfW=0;
//���᳤��
volatile double halfW=0;
//���ĵ�����
volatile double g_Px;
volatile double g_Py;
//����
volatile double Distance;
volatile double Dist=0.5;
volatile bool next_ok=false;


DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter);//׼ֱ�߳�

HANDLE g_hMutex = NULL;//������

volatile BOOL m_cRun;//׼ֱ�߳�

//�����ͺţ�trueΪԲ���ˣ�falseΪШ�ι���
bool FiberModelFlag;
//ƫ���
volatile double PartialAng;
