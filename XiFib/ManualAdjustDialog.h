#pragma once


// ManualAdjustDialog 对话框

class ManualAdjustDialog : public CDialog
{
	DECLARE_DYNAMIC(ManualAdjustDialog)

public:
	ManualAdjustDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ManualAdjustDialog();

// 对话框数据
	enum { IDD = IDD_ADJUSTMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddx();
	afx_msg void OnBnClickedButtonDecx();
	afx_msg void OnBnClickedButtonAddy();
	afx_msg void OnBnClickedButtonDecy();
	afx_msg void OnBnClickedButtonAddz();
	afx_msg void OnBnClickedButtonDecz();
	afx_msg void OnEnChangeEdit1();
	int Set_Distance;
};
