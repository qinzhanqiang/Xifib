#pragma once


// CInputDialog �Ի���

class CInputDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputDialog();

// �Ի�������
	enum { IDD = IDD_IP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_Dis;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedFinish();
};
