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
	
	
	
public:
	typedef enum AXISn
	{
		AXISx,
		AXISy,
		AXISz,
		AXISxangle,
		AXISyangle

	}AXISn;

	typedef enum DIRECTION{
		FORWORD,
		NEGITIVE
	}DIRECTION;
	bool trans(AXISn X_or_Y_or_Z,DIRECTION up_down_flag,int distance);
	afx_msg void OnEnUpdateEditDistance();

	
	afx_msg void OnBnClickedButtonExit();
};
typedef struct Button_xyz{
	CButton* bt1;
	CButton* bt2;
	CButton* bt3;
	CButton* bt4;
	CButton* bt5;
	CButton* bt6;
}Button_xyz;
//extern volatile bool buttonFlag = false;

DWORD WINAPI FlashManualAdjustDialogUI(LPVOID lpParam) ;