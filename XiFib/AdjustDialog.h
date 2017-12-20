#pragma once
#include "afxcmn.h"
#include "XiFib.h"
#include <vector>
#include "CLineFit.h"
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "CEllipseFit.h"
#include "GaussFit.h"
#include "LMGaussFit.h"

// CAdjustDialog 对话框

class CAdjustDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAdjustDialog)

public:
	//CAdjustDialog(CWnd* pParent = NULL);   // 标准构造函数
	CAdjustDialog(CAdjustDialog * &p, CWnd* pParent = NULL);
	virtual ~CAdjustDialog();

// 对话框数据
	enum { IDD = IDD_TZ_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ANSList;
	int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<int> &vx, std::vector<int> &vy, std::vector<int> &value);
private:
	int     scaleY;
	//CBrush  m_brush;
	CAdjustDialog * & Pthis;
public:
	std::vector<int> m_XVec;//找到的椭圆点
	std::vector<int> m_YVec;

	std::vector<double> m_GXVec;//短轴上的点
	std::vector<double> m_GYVec;
	std::vector<double> m_LGXVec;//长轴上的点
	std::vector<double> m_LGYVec;
	//std::vector<cv::Point> m_vPoint;
	CEllipse m_Ellipse;
	CGaussLine m_GaussLine;
	CGaussLine m_LGaussLine;

	std::vector<double> vecX;
	std::vector<double> vecYS;//短轴长
	std::vector<double> vecYL;// 长轴长

	CMyLine lineS;
	CMyLine lineL;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void OnOK();
public:
	double m_Dis;  //距离
	double m_CP;   //当前偏离
	double m_CHP;  //调整偏离
public:
	int findHalf(cv::Mat& in, std::vector<int> &Xp, std::vector<int> &Yp);  
	int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value);
	double round(double r);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedStartButton();
private:
	// 画点和直线
	void Draw(CPaintDC & dc);
};
