// ChangePDialog.cpp : 实现文件
//工具--偏移--偏移距离对话框实现函数

#include "stdafx.h"
#include "XiFib.h"
#include "ChangePDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CChangePDialog 对话框

IMPLEMENT_DYNAMIC(CChangePDialog, CDialogEx)

CChangePDialog::CChangePDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangePDialog::IDD, pParent)
	, m_CDP(0)
	, m_DP(0)
{

}

CChangePDialog::~CChangePDialog()
{
}

void CChangePDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CP_EDIT, m_CDP);
	DDX_Text(pDX, IDC_CHP_EDIT, m_DP);
	DDV_MinMaxDouble(pDX, m_DP, -30, 30);
}


BEGIN_MESSAGE_MAP(CChangePDialog, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CChangePDialog 消息处理程序


BOOL CChangePDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChangePDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_CDP = ((CMainFrame *)AfxGetMainWnd())->m_P;
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(false);           //变量传给控件
	// TODO: 在此处添加消息处理程序代码
}


void CChangePDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	((CMainFrame *)AfxGetMainWnd())->m_P = m_DP;
	CDialogEx::OnOK();	
}
