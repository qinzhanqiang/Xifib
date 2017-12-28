// ManualAdjustDialog.cpp : ʵ���ļ�
//�ֶ����ڵ��ƽ��̨ʵ���ļ�

#include "stdafx.h"
#include "XiFib.h"
#include "ManualAdjustDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
//#include "CollimatDialog.h"
volatile bool buttonFlag =0;
volatile int ThreadUIFlag=0;
// ManualAdjustDialog �Ի���

IMPLEMENT_DYNAMIC(ManualAdjustDialog, CDialog)

ManualAdjustDialog::ManualAdjustDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ManualAdjustDialog::IDD, pParent)
	, Set_Distance(1000)
{

	
	
}

ManualAdjustDialog::~ManualAdjustDialog()
{
	//CloseHandle(mad_Mutex);
}

void ManualAdjustDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, Set_Distance);
}


BEGIN_MESSAGE_MAP(ManualAdjustDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDX, &ManualAdjustDialog::OnBnClickedButtonAddx)
	ON_BN_CLICKED(IDC_BUTTON_DECX, &ManualAdjustDialog::OnBnClickedButtonDecx)
	ON_BN_CLICKED(IDC_BUTTON_ADDY, &ManualAdjustDialog::OnBnClickedButtonAddy)
	ON_BN_CLICKED(IDC_BUTTON_DECY, &ManualAdjustDialog::OnBnClickedButtonDecy)
	ON_BN_CLICKED(IDC_BUTTON_ADDZ, &ManualAdjustDialog::OnBnClickedButtonAddz)
	ON_BN_CLICKED(IDC_BUTTON_DECZ, &ManualAdjustDialog::OnBnClickedButtonDecz)
	ON_EN_UPDATE(IDC_EDIT_DISTANCE, &ManualAdjustDialog::OnEnUpdateEditDistance)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &ManualAdjustDialog::OnBnClickedButtonExit)
	
END_MESSAGE_MAP()


// ManualAdjustDialog ��Ϣ�������
//ÿ����ĵ����߳�
DWORD WINAPI Threadaddx(LPVOID lpParam)  
{  
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISx,ManualAdjustDialog::FORWORD,(int)lpParam);
	

	return 0;
}
DWORD WINAPI Threaddecx(LPVOID lpParam)  
{  
	
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISx,ManualAdjustDialog::NEGITIVE,(int)lpParam);
	
	return 0;
}
DWORD WINAPI Threadaddy(LPVOID lpParam)  
{  
	
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISy,ManualAdjustDialog::FORWORD,(int)lpParam);
	
	return 0;
}
DWORD WINAPI Threaddecy(LPVOID lpParam)  
{  
	
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISy,ManualAdjustDialog::NEGITIVE,(int)lpParam);
	
	return 0;
}
DWORD WINAPI Threadaddz(LPVOID lpParam)  
{  
	
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISz,ManualAdjustDialog::FORWORD,(int)lpParam);
	
	return 0;
}
DWORD WINAPI Threaddecz(LPVOID lpParam)  
{  
	
	ManualAdjustDialog mad;
	mad.trans(ManualAdjustDialog::AXISz,ManualAdjustDialog::NEGITIVE,(int)lpParam);
	
	return 0;
}

//�����¼�������
void ManualAdjustDialog::OnBnClickedButtonAddx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HANDLE addxthread = CreateThread(NULL,0,Threadaddx,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addxthread);
	
}

void ManualAdjustDialog::OnBnClickedButtonDecx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HANDLE decxthread = CreateThread(NULL,0,Threaddecx,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(decxthread);
}


void ManualAdjustDialog::OnBnClickedButtonAddy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HANDLE addythread = CreateThread(NULL,0,Threadaddy,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addythread);
}


void ManualAdjustDialog::OnBnClickedButtonDecy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HANDLE decythread = CreateThread(NULL,0,Threaddecy,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(decythread);
}


void ManualAdjustDialog::OnBnClickedButtonAddz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HANDLE addzthread = CreateThread(NULL,0,Threadaddz,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addzthread);
}


void ManualAdjustDialog::OnBnClickedButtonDecz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	HANDLE deczthread = CreateThread(NULL,0,Threaddecz,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(deczthread);
}


void ManualAdjustDialog::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}




//���ݷ��ͺ���
bool ManualAdjustDialog::trans(AXISn X_or_Y_or_Z,DIRECTION up_down_flag,int distance)
{
	//����ϴε���δ��ɣ��������Ѳ��˳����ͺ���
	if (buttonFlag == true)
	{
		AfxMessageBox(_T("��ȴ��ϴε������"));
		return false;
	}
	buttonFlag = true;
	CMainFrame cmainfram;
	int SData[6];

	assert(distance >=0);

	int data_Z = distance;//(um)
	data_Z = abs(data_Z/10);

	//�����ݴ洢����������ĺ���λ
	SData[5] = data_Z%10;
	data_Z = data_Z/10;
	SData[4] = data_Z%10;
	data_Z = data_Z/10;
	SData[3] = data_Z%10;

	//����ѡ���Ҫ���ڵ���ͷ��������ǰ��λ����ѡ��
	switch(X_or_Y_or_Z){
	case AXISx:
		{
			if (up_down_flag==FORWORD)
			{

				SData[0] = 01;
				SData[1] = 02;
				SData[2] = 00;
			}
			else
			{
				SData[0] = 01;
				SData[1] = 02;
				SData[2] = 01;
			}
			goto sendmessage;
			break;
		};
	case AXISy:
		{
			if (up_down_flag==FORWORD)
			{
				SData[0] = 01;
				SData[1] = 03;
				SData[2] = 00;
			}
			else
			{
				SData[0] = 01;
				SData[1] = 03;
				SData[2] = 01;
			}
			goto sendmessage;
			break;
		};
	case AXISz:
		{
			if (up_down_flag==FORWORD)
			{
				SData[0] = 01;
				SData[1] = 01;
				SData[2] = 01;
			}
			else
			{
				SData[0] = 01;
				SData[1] = 01;
				SData[2] = 00;
			}
			goto sendmessage;
			break;
		};
	case AXISxangle:
		{
			if (up_down_flag==FORWORD)
			{
				SData[0] = 01;
				SData[1] = 04;
				SData[2] = 00;
			}
			else
			{
				SData[0] = 01;
				SData[1] = 04;
				SData[2] = 01;
			}
			goto sendmessage;
			break;
		};
	case AXISyangle:
		{
			if (up_down_flag==FORWORD)
			{
				SData[0] = 01;
				SData[1] = 05;
				SData[2] = 00;
			}
			else
			{
				SData[0] = 01;
				SData[1] = 05;
				SData[2] = 01;
			}
			goto sendmessage;
			break;
		};
	default:
		{
			AfxMessageBox(_T("��������ȷ�ķ���"));
			return false;
			break;
		};

	}
//��������
sendmessage:
	
	((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
	//��ȡ��ǰʱ��
	long begintime,endtime;
	clock_t t = clock();
	endtime = begintime = t/CLOCKS_PER_SEC;
	while(!next_ok)
	{
		t = clock();
		endtime = t/CLOCKS_PER_SEC;
		//���10S֮����λ������Ӧ���������ѣ��˳����ͺ���
		if(endtime - begintime > 10){
			Sleep(100);
			AfxMessageBox(_T("��λ������Ӧ�����˳����ͳ��򣬷���ʧ�ܣ���"));
			buttonFlag = false;
			return false;
		}

	}
	if(next_ok)
	{
		while(1)
		{
			cv::Point2d CenterAB1 = (g_Px,g_Py);
			for(int i=0;i<1000;++i)
				;
			cv::Point2d CenterAB2 = (g_Px,g_Py);
			if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
				break;
		}
		next_ok=false;
		AfxMessageBox(_T("����λ�������źųɹ����յ������źţ�"));
		buttonFlag = false;
		return true;
	}
	buttonFlag = false;
	return true;
}




//�������ÿ���ƶ��ľ���ı���޸�Set_Distance��ֵ
void ManualAdjustDialog::OnEnUpdateEditDistance()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_EDIT_DISTANCE)->GetWindowText(str);  
	Set_Distance = atoi(str);
	
}

//�˳�������Ӧ����
void ManualAdjustDialog::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	ThreadUIFlag = 1;
	CDialog::EndDialog(IDD_ADJUSTMENT);
}


