// SCIDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "SCIDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CSCIDialog 对话框

IMPLEMENT_DYNAMIC(CSCIDialog, CDialogEx)

CSCIDialog::CSCIDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSCIDialog::IDD, pParent)
	, m_Com(1)			//默认设置为串口2
	, m_BaudRate(1)		//默认设置为波特率9600
	, m_Parity(0)
{

}

CSCIDialog::~CSCIDialog()
{
}

void CSCIDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_COM, m_Com);
	DDX_CBIndex(pDX, IDC_COMBO_B, m_BaudRate);
	DDX_CBIndex(pDX, IDC_COMBO_P, m_Parity);
}


BEGIN_MESSAGE_MAP(CSCIDialog, CDialogEx)
	
END_MESSAGE_MAP()


// CSCIDialog 消息处理程序



//串口通信参数设置
//void CSCIDialog::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	CString strStatus;
//	CSCIDialog mysetup;
//	/*if(mysetup.DoModal() == IDOK)
//	{*/
//		switch(mysetup.m_Com)
//		{
//		case 0:
//			{
//				strStatus = "COM1";
//				myCom = "COM1";
//				break;
//			}
//		case 1:
//			{
//				strStatus = "COM2";
//				myCom = "COM2";
//				break;
//			}
//		case 2:
//			{
//				strStatus = "COM3";
//				myCom = "COM3";
//				break;
//			}
//		case 3:
//			{
//				strStatus = "COM4";
//				myCom = "COM4";
//				break;
//			}
//		}
//		strStatus+=",";
//		switch(mysetup.m_BaudRate)
//		{
//		case 0:
//			{
//				strStatus +="19200";
//				myBaudRate = CBR_19200;
//				break;
//			}
//		case 1:
//			{
//				strStatus +="9600";
//				myBaudRate = CBR_9600;
//				break;
//			}
//		case 2:
//			{
//				strStatus +="4800";
//				myBaudRate = CBR_4800;
//				break;
//			}
//		case 3:
//			{
//				strStatus +="2400";
//				myBaudRate = CBR_2400;
//				break;
//			}
//		}
//		strStatus+=",";
//		switch(mysetup.m_Parity)
//		{
//		case 0:
//			{
//				strStatus +="N";
//				myfParity = false;
//				myParity = NOPARITY;
//				break;
//			}
//		case 1:
//			{
//				strStatus +="O";
//				myfParity = true;
//				myParity = ODDPARITY;
//				break;
//			}
//		case 2:
//			{
//				strStatus +="E";
//				myfParity = true;
//				myParity = EVENPARITY;
//				break;
//			}
//
//		}
//		strStatus+=",8,1";
//		AfxMessageBox(strStatus);
//	//}
//
//	CDialogEx::OnOK();
//}
