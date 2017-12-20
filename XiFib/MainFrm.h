
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "afxwin.h"
#include "DataPane.h"
#include "EllipsePane.h"
#include "GaussPane.h"
#include "XiPlayPane.h"
#include "AdjustDialog.h"
#include "ChangeDDiolag.h"
#include "ChangePDialog.h"
#include "SCIDialog.h"
#include "CollimatDialog.h"

class CMainFrame : public CMDIFrameWndEx
{
	#define WM_MYMSG WM_USER+100
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 特性
public:
	//距离
	double m_Distance;
	double m_P;
	//最大值
	int max;
	//中心
	CvPoint Epoint;

// 控制
public:
	//是否计算
	//bool bCal;
	bool bcolor;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	

public:
	//悬浮停靠面板是放在主框架里面，注意不是放在子框架里面
	CMFCStatusBar     m_wndStatusBar;
	DataPane          m_wndData;
	EllipsePane       m_wndEllipse;
	//短轴面板
	GaussPane         m_wndGauss;
	//长轴面板
	GaussPane         m_wndGaussL;

	XiPlayPane        m_wndPlay;
	CAdjustDialog    *TZDlg;
	CChangeDDiolag   *CCDlg;
	CChangePDialog   *CPDlg;
	CListCtrl        *pDataList;
	//CSCIDialog       *SDlg;
	CCollimatDialog  *ColDlg;
// 生成的消息映射函数
//public:
	//计算数据保存

	//找到的椭圆点
	//std::vector<int> m_XVec;
	//std::vector<int> m_YVec;
	//短轴上的点
	//std::vector<double> m_GXVec;
	//std::vector<double> m_GYVec;
	//长轴上的点
	//std::vector<double> m_LGXVec;
	//std::vector<double> m_LGYVec;

	//CEllipse &m_Ellipse;
	//CGaussLine &m_GaussLine;
	//CGaussLine &m_LGaussLine;

	//std::vector<double> vecX;
	//std::vector<double> vecYS;//短轴长
	//std::vector<double> vecYL;// 长轴长
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnAdjust();
	afx_msg void OnDistance();
	afx_msg void OnPdis();
	afx_msg void OnStart();
	afx_msg void OnUpdateStart(CCmdUI *pCmdUI);
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	
private:
	int findHalf(cv::Mat& in, const int& max, std::vector<int> &Xp, std::vector<int> &Yp);
	int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value);
	double round(double r);
public:
	// 调整曝光时间
	int cExpose();
	int getMat(cv::Mat &src);
	afx_msg void OnSafeBmp();
	afx_msg void OnColorv();
	afx_msg void OnUpdateColorv(CCmdUI *pCmdUI);
	afx_msg void OnFomat();
	afx_msg void OnExpose();
	afx_msg void OnGain();
	afx_msg void OnAuto();
	afx_msg void OnPara();
	afx_msg void OnU();
	afx_msg void OnT();
	afx_msg void OnP();

	
	afx_msg void OnOpenCom();
	afx_msg void OnUpdateOpenCom(CCmdUI *pCmdUI);
	afx_msg void OnCloseCom();
	afx_msg void OnUpdateCloseCom(CCmdUI *pCmdUI);
	afx_msg void OnSci();
	afx_msg void OnCollimat();

	
	afx_msg void OnUpdateSci(CCmdUI *pCmdUI);

	void onSendMessage(int SData[6]);
	long OnReceiveEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopCollimat();
	afx_msg void OnUpdateStopCollimat(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCollimat(CCmdUI *pCmdUI);
};


