#pragma once


// ManualAdjustDialog �Ի���

class ManualAdjustDialog : public CDialog
{
	DECLARE_DYNAMIC(ManualAdjustDialog)

public:
	ManualAdjustDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ManualAdjustDialog();

// �Ի�������
	enum { IDD = IDD_ADJUSTMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
