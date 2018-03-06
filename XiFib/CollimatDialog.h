#pragma once
#include<cv.h>
#include<math.h>
#include<stack>
#include<vector>
#include "afxcmn.h"
#include<algorithm>

using namespace std;

// CCollimatDialog 对话框

class CCollimatDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCollimatDialog)

public:
	CCollimatDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCollimatDialog();
	

// 对话框数据
	enum { IDD = IDD_COLLIMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

	DECLARE_MESSAGE_MAP()
public:
	CButton m_ShortAngle;
	CButton m_LongAngle;
	CButton m_OffAngle;
	afx_msg void OnBnClickedCircleFiber();
	afx_msg void OnBnClickedWedgeFiber();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	
	
	CString fiberNum;
	afx_msg void OnEnChangeZ();
	CString SetZ;
};

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

typedef enum TRANSTYPE {
	ANGLEShort,
	ANGLELong,
	COORDSx,
	COORDSy
}TRANSTYPE;

bool Init_collimation();
bool transLCD(TRANSTYPE type, double longangle);
bool trans(AXISn X_or_Y,DIRECTION up_down_flag,double distance,int* BYTE_data);
cv::Point2d In_Center_Point(double xc,double yc);
bool close_to_ceter0(cv::Point2d P_now,deque<cv::Point2d> & Center_last);

//数据采集
bool DataGetLength(vector<double> &Xarr,vector<double> &Yarr);
bool DataGetPoint(vector<cv::Point2d> &arr);
//偏轴度计算
double Partial_degree(vector<cv::Point2d> arr);
//发散角计算
double GetDivAng(vector<double> arr,double Thre);
double GetDivAngNew1(vector<double> arrX,vector<double> arrY,double ThreX,double ThreY);

double GetDis(vector<double> &arr);
void SaveTempData(int count, int datax, int datay, cv::Point2d point_last, cv::Point2d point);