// ChangePDialog.cpp : ʵ���ļ�
//����--ƫ��--ƫ�ƾ���Ի���ʵ�ֺ���

#include "stdafx.h"
#include "XiFib.h"
#include "ChangePDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CChangePDialog �Ի���

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


// CChangePDialog ��Ϣ�������


BOOL CChangePDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CChangePDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_CDP = ((CMainFrame *)AfxGetMainWnd())->m_P;
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(false);           //���������ؼ�
	// TODO: �ڴ˴������Ϣ����������
}


void CChangePDialog::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	((CMainFrame *)AfxGetMainWnd())->m_P = m_DP;
	CDialogEx::OnOK();	
}
