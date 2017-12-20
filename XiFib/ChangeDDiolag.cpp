// ChangeDDiolag.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "ChangeDDiolag.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CChangeDDiolag 对话框

IMPLEMENT_DYNAMIC(CChangeDDiolag, CDialogEx)

CChangeDDiolag::CChangeDDiolag(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeDDiolag::IDD, pParent)
	//, m_CurrentDis(0.5)
	, m_ChangeDis(0.5)
{

}

CChangeDDiolag::~CChangeDDiolag()
{
}

void CChangeDDiolag::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CD_EDIT, m_CurrentDis);
	DDV_MinMaxDouble(pDX, m_CurrentDis, 0, 50);
	DDX_Text(pDX, IDC_CH_EDIT, m_ChangeDis);
	DDV_MinMaxDouble(pDX, m_ChangeDis, 0.5, 50);
}


BEGIN_MESSAGE_MAP(CChangeDDiolag, CDialogEx)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CChangeDDiolag 消息处理程序


BOOL CChangeDDiolag::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChangeDDiolag::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_CurrentDis = ((CMainFrame *)AfxGetMainWnd())->m_Distance;  //获取当前值
	//m_CurrentDis = m_Distance;  //获取当前值
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(false);
	// TODO: 在此处添加消息处理程序代码
}


void CChangeDDiolag::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	((CMainFrame *)AfxGetMainWnd())->m_Distance = m_ChangeDis;   //更改值
	//m_Distance = m_ChangeDis;   //更改值
	CDialogEx::OnOK();
}
