#pragma once


// CChangePDialog �Ի���

class CChangePDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChangePDialog)

public:
	CChangePDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangePDialog();

// �Ի�������
	enum { IDD = IDD_CP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_CDP;
	double m_DP;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
};
