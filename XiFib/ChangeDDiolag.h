#pragma once


// CChangeDDiolag �Ի���

class CChangeDDiolag : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeDDiolag)

public:
	CChangeDDiolag(CWnd* pParent = NULL);   // ��׼���캯��
	//CChangeDDiolag(double dis, CChangeDDiolag* &p, CWnd* pParent = NULL); 
	virtual ~CChangeDDiolag();

// �Ի�������
	enum { IDD = IDD_CD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_CurrentDis;
	double m_ChangeDis;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
};
