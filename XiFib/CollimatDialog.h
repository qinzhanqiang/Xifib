#pragma once
#include<cv.h>
#include<math.h>
#include<stack>
#include<vector>
using namespace std;

// CCollimatDialog �Ի���

class CCollimatDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCollimatDialog)

public:
	CCollimatDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCollimatDialog();

// �Ի�������
	enum { IDD = IDD_COLLIMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButton m_ShortAngle;
	CButton m_LongAngle;
	CButton m_OffAngle;
	afx_msg void OnBnClickedCircleFiber();
	afx_msg void OnBnClickedWedgeFiber();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	
};

	//Z���ʼ����
	/*extern double Z_distance = 0;
	extern double Distance;
	extern double LhalfW;
	extern double halfW;*/

bool Init_collimation();
bool trans(int X_or_Y,int up_down_flag,double distance,int* BYTE_data);
cv::Point2d In_Center_Point(double xc,double yc);
bool close_to_ceter0(cv::Point2d P_now,deque<cv::Point2d> & Center_last);

//���ݲɼ�
bool DataGetLength(vector<double> &Xarr,vector<double> &Yarr);
bool DataGetPoint(vector<cv::Point2d> &arr);
//ƫ��ȼ���
double Partial_degree(vector<cv::Point2d> arr);
//��ɢ�Ǽ���
double GetDivAng(vector<double> arr,double Thre);
double GetDivAngNew1(vector<double> arrX,vector<double> arrY,double ThreX,double ThreY);

//typedef struct _THREAD_DATA 
//{ 
//	double &Dist;
//}THREAD_DATA;