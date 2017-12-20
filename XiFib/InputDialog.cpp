// InputDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "InputDialog.h"
#include "afxdialogex.h"


// CInputDialog 对话框

IMPLEMENT_DYNAMIC(CInputDialog, CDialogEx)

CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDialog::IDD, pParent)
	, m_Dis(0)
{

}

CInputDialog::~CInputDialog()
{
}

void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP_EDIT, m_Dis);
	DDV_MinMaxDouble(pDX, m_Dis, 0.1, 50);
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialogEx)
	ON_BN_CLICKED(IDFINISH, &CInputDialog::OnBnClickedFinish)
END_MESSAGE_MAP()


// CInputDialog 消息处理程序


BOOL CInputDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInputDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	INT_PTR nRes;
	UpdateData(TRUE);
	CString str("确定设置距离为：");
	str.AppendFormat("%.4f", m_Dis);
	str.AppendChar('?');
	nRes = MessageBox(str, NULL, MB_OKCANCEL | MB_ICONQUESTION);   
	// 判断消息对话框返回值。如果为IDCANCEL就return，否则继续向下执行   
	if (IDOK != nRes)
	{
		return;
	}
	CDialogEx::OnOK();
}


void CInputDialog::OnBnClickedFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	int FINISH = 11;

	EndDialog(FINISH);  //点击完成返回11；

	return;
}
