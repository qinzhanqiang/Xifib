// ManualAdjustDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "ManualAdjustDialog.h"
#include "afxdialogex.h"
#include "CollimatDialog.h"

// ManualAdjustDialog 对话框

IMPLEMENT_DYNAMIC(ManualAdjustDialog, CDialog)

ManualAdjustDialog::ManualAdjustDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ManualAdjustDialog::IDD, pParent)
	, Set_Distance(100)
{

}

ManualAdjustDialog::~ManualAdjustDialog()
{
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
	ON_EN_CHANGE(IDC_EDIT1, &ManualAdjustDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// ManualAdjustDialog 消息处理程序


void ManualAdjustDialog::OnBnClickedButtonAddx()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;

	trans(AXISx,FORWORD,Set_Distance,data);
}


void ManualAdjustDialog::OnBnClickedButtonDecx()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	trans(AXISx,NEGITIVE,Set_Distance,data);
}


void ManualAdjustDialog::OnBnClickedButtonAddy()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	
	trans(AXISy,FORWORD,Set_Distance,data);
}


void ManualAdjustDialog::OnBnClickedButtonDecy()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	
	trans(AXISy,NEGITIVE,Set_Distance,data);
}


void ManualAdjustDialog::OnBnClickedButtonAddz()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	
	trans(AXISz,FORWORD,Set_Distance,data);
}


void ManualAdjustDialog::OnBnClickedButtonDecz()
{
	// TODO: 在此添加控件通知处理程序代码
	int data[3] = {0,0,0};
	int data_Z = (int)Set_Distance*1000;//(um)
	data_Z = abs(data_Z/10);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	
	trans(AXISz,NEGITIVE,Set_Distance,data);
}


void ManualAdjustDialog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
