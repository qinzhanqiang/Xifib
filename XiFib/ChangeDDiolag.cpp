// ChangeDDiolag.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiFib.h"
#include "ChangeDDiolag.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CChangeDDiolag �Ի���

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


// CChangeDDiolag ��Ϣ�������


BOOL CChangeDDiolag::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CChangeDDiolag::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_CurrentDis = ((CMainFrame *)AfxGetMainWnd())->m_Distance;  //��ȡ��ǰֵ
	//m_CurrentDis = m_Distance;  //��ȡ��ǰֵ
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(false);
	// TODO: �ڴ˴������Ϣ����������
}


void CChangeDDiolag::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(true);
	((CMainFrame *)AfxGetMainWnd())->m_Distance = m_ChangeDis;   //����ֵ
	//m_Distance = m_ChangeDis;   //����ֵ
	CDialogEx::OnOK();
}
