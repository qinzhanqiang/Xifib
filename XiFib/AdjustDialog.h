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

// CAdjustDialog �Ի���

class CAdjustDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAdjustDialog)

public:
	//CAdjustDialog(CWnd* pParent = NULL);   // ��׼���캯��
	CAdjustDialog(CAdjustDialog * &p, CWnd* pParent = NULL);
	virtual ~CAdjustDialog();

// �Ի�������
	enum { IDD = IDD_TZ_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	std::vector<int> m_XVec;//�ҵ�����Բ��
	std::vector<int> m_YVec;

	std::vector<double> m_GXVec;//�����ϵĵ�
	std::vector<double> m_GYVec;
	std::vector<double> m_LGXVec;//�����ϵĵ�
	std::vector<double> m_LGYVec;
	//std::vector<cv::Point> m_vPoint;
	CEllipse m_Ellipse;
	CGaussLine m_GaussLine;
	CGaussLine m_LGaussLine;

	std::vector<double> vecX;
	std::vector<double> vecYS;//���᳤
	std::vector<double> vecYL;// ���᳤

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
	double m_Dis;  //����
	double m_CP;   //��ǰƫ��
	double m_CHP;  //����ƫ��
public:
	int findHalf(cv::Mat& in, std::vector<int> &Xp, std::vector<int> &Yp);  
	int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value);
	double round(double r);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedStartButton();
private:
	// �����ֱ��
	void Draw(CPaintDC & dc);
};
