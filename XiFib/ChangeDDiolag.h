#pragma once


// CChangeDDiolag 对话框

class CChangeDDiolag : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeDDiolag)

public:
	CChangeDDiolag(CWnd* pParent = NULL);   // 标准构造函数
	//CChangeDDiolag(double dis, CChangeDDiolag* &p, CWnd* pParent = NULL); 
	virtual ~CChangeDDiolag();

// 对话框数据
	enum { IDD = IDD_CD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_CurrentDis;
	double m_ChangeDis;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
};
