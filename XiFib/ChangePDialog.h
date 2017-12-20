#pragma once


// CChangePDialog 对话框

class CChangePDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChangePDialog)

public:
	CChangePDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangePDialog();

// 对话框数据
	enum { IDD = IDD_CP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_CDP;
	double m_DP;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
};
