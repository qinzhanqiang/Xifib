// ManualAdjustDialog.cpp : 实现文件
//手动调节电控平移台实现文件

#include "stdafx.h"
#include "XiFib.h"
#include "ManualAdjustDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
//#include "CollimatDialog.h"
volatile bool buttonFlag =0;
volatile int ThreadUIFlag=0;
// ManualAdjustDialog 对话框

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


// ManualAdjustDialog 消息处理程序
//每个轴的调节线程
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

//按键事件处理函数
void ManualAdjustDialog::OnBnClickedButtonAddx()
{
	// TODO: 在此添加控件通知处理程序代码

	HANDLE addxthread = CreateThread(NULL,0,Threadaddx,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addxthread);
	
}

void ManualAdjustDialog::OnBnClickedButtonDecx()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HANDLE decxthread = CreateThread(NULL,0,Threaddecx,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(decxthread);
}


void ManualAdjustDialog::OnBnClickedButtonAddy()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HANDLE addythread = CreateThread(NULL,0,Threadaddy,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addythread);
}


void ManualAdjustDialog::OnBnClickedButtonDecy()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HANDLE decythread = CreateThread(NULL,0,Threaddecy,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(decythread);
}


void ManualAdjustDialog::OnBnClickedButtonAddz()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HANDLE addzthread = CreateThread(NULL,0,Threadaddz,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(addzthread);
}


void ManualAdjustDialog::OnBnClickedButtonDecz()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HANDLE deczthread = CreateThread(NULL,0,Threaddecz,(LPVOID)Set_Distance,0,NULL);
	CloseHandle(deczthread);
}


void ManualAdjustDialog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}




//数据发送函数
bool ManualAdjustDialog::trans(AXISn X_or_Y_or_Z,DIRECTION up_down_flag,int distance)
{
	//如果上次调节未完成，弹窗提醒并退出发送函数
	if (buttonFlag == true)
	{
		AfxMessageBox(_T("请等待上次调整完成"));
		return false;
	}
	buttonFlag = true;
	CMainFrame cmainfram;
	int SData[6];

	assert(distance >=0);

	int data_Z = distance;//(um)
	data_Z = abs(data_Z/10);

	//将数据存储到发送数组的后三位
	SData[5] = data_Z%10;
	data_Z = data_Z/10;
	SData[4] = data_Z%10;
	data_Z = data_Z/10;
	SData[3] = data_Z%10;

	//根据选择的要调节的轴和方向对数组前三位进行选择
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
			AfxMessageBox(_T("请输入正确的方向"));
			return false;
			break;
		};

	}
//发送数据
sendmessage:
	
	((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
	//获取当前时间
	long begintime,endtime;
	clock_t t = clock();
	endtime = begintime = t/CLOCKS_PER_SEC;
	while(!next_ok)
	{
		t = clock();
		endtime = t/CLOCKS_PER_SEC;
		//如果10S之内下位机无响应，弹窗提醒，退出发送函数
		if(endtime - begintime > 10){
			Sleep(100);
			AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
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
		AfxMessageBox(_T("向下位机发送信号成功，收到反馈信号！"));
		buttonFlag = false;
		return true;
	}
	buttonFlag = false;
	return true;
}




//当输入的每次移动的距离改变后修改Set_Distance的值
void ManualAdjustDialog::OnEnUpdateEditDistance()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_DISTANCE)->GetWindowText(str);  
	Set_Distance = atoi(str);
	
}

//退出按键响应函数
void ManualAdjustDialog::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	
	ThreadUIFlag = 1;
	CDialog::EndDialog(IDD_ADJUSTMENT);
}


