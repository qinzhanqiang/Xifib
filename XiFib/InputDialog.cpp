// InputDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiFib.h"
#include "InputDialog.h"
#include "afxdialogex.h"


// CInputDialog �Ի���

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


// CInputDialog ��Ϣ�������


BOOL CInputDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CInputDialog::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	INT_PTR nRes;
	UpdateData(TRUE);
	CString str("ȷ�����þ���Ϊ��");
	str.AppendFormat("%.4f", m_Dis);
	str.AppendChar('?');
	nRes = MessageBox(str, NULL, MB_OKCANCEL | MB_ICONQUESTION);   
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDOK != nRes)
	{
		return;
	}
	CDialogEx::OnOK();
}


void CInputDialog::OnBnClickedFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int FINISH = 11;

	EndDialog(FINISH);  //�����ɷ���11��

	return;
}
