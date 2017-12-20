// CollimatDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiFib.h"
#include "CollimatDialog.h"
#include "afxdialogex.h"
#include "acquisition.h"
#include "MainFrm.h"

using namespace std;
cv::Point2d Center_0(1024,1024);
bool close_to_ceter0(cv::Point2d P_now,deque<cv::Point2d> & Center_last);




// CCollimatDialog �Ի���

IMPLEMENT_DYNAMIC(CCollimatDialog, CDialogEx)

CCollimatDialog::CCollimatDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCollimatDialog::IDD, pParent)
	/*, m_CircleFiber(0)
	, m_WedgeFiber(0)*/
	/*, m_ShortAngle()
	, m_LongAngle()
	, m_OffAngle()*/
{

}

CCollimatDialog::~CCollimatDialog()
{
}

void CCollimatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_CIRCLE_FIBER, m_CircleFiber);
	DDX_Control(pDX, IDC_WEDGE_FIBER, m_WedgeFiber);*/
	DDX_Control(pDX, IDC_SHORT_DIV_ANGLE, m_ShortAngle);
	DDX_Control(pDX, IDC_LONG_DIV_ANGLE, m_LongAngle);
	DDX_Control(pDX, IDC_OFF_ANGLE, m_OffAngle);
}


BEGIN_MESSAGE_MAP(CCollimatDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CIRCLE_FIBER, &CCollimatDialog::OnBnClickedCircleFiber)
	ON_BN_CLICKED(IDC_WEDGE_FIBER, &CCollimatDialog::OnBnClickedWedgeFiber)
	ON_BN_CLICKED(IDOK, &CCollimatDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCollimatDialog ��Ϣ�������


void CCollimatDialog::OnBnClickedCircleFiber()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_ShortAngle.EnableWindow(TRUE);
	//m_LongAngle.EnableWindow(TRUE);
	m_OffAngle.EnableWindow(FALSE);
	m_ShortAngle.SetCheck(1);
	m_LongAngle.SetCheck(1);
	m_OffAngle.SetCheck(0);
	FiberModelFlag=TRUE;
}


void CCollimatDialog::OnBnClickedWedgeFiber()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������  
	//m_ShortAngle.EnableWindow(TRUE);
	//m_LongAngle.EnableWindow(TRUE);
	m_OffAngle.EnableWindow(TRUE);
	m_ShortAngle.SetCheck(1);
	m_LongAngle.SetCheck(1);
	m_OffAngle.SetCheck(1);
	FiberModelFlag=FALSE;
}


BOOL CCollimatDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCollimatDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*double Distance =((CMainFrame *)AfxGetMainWnd())->m_Distance;

	double x=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.xc;

	double y=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.yc;

	if(Init_collimation(Distance,LhalfW,x,y))
		AfxMessageBox(_T("׼ֱ�ɹ�"));
	else
		AfxMessageBox(_T("׼ֱʧ��"));*/
	/*AfxMessageBox(_T("׼ֱ�ɹ�"));
	AfxMessageBox(_T("׼ֱʧ��"));*/

	//׼ֱ����1
	CDialogEx::OnOK();
	
}

//׼ֱ
//X��Y��Z��rX,rY��Ӧ��(1,2,3,4,5)
//flag��=1��Ӧ������=0��Ӧ�ڸ���
bool Init_collimation(double Distance,double LhalfW,double x,double y)
{
	double Z0_L = 2;//ÿ�ν�Z������ƶ��ľ���
	bool flag = false;//Z���ƶ�������
	int flag_x_y = 0;//�Ƕ�&X,&Y�ƶ�����

	std::deque<cv::Point2d> Center_last;
	double R_long = 0;//Բ�������ĵľ���
	/*double Z_distance = 0;
	cout<<"������Z���ʼ���룺";
	cin>>Z_distance; */
	double Z_distance = Distance; 
	double L_short = 150;//���᳤��
	double L_long  = 0;//���᳤��
	while(L_short>120)
	{
		/*cout<<"��������᳤�ȣ�";
		cin>>L_short;*/
		L_short = LhalfW;
		int data[3] = {0,0,0};
		int data_Z = abs(Z0_L*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(3,0,Z0_L,data);//������ǰ�ƶ�
	}
	//
	cv::Point2d Center_f = In_Center_Point(x,y);
	/*Center_f.x = m_Ellipse.xc;
	Center_f.y = m_Ellipse.yc;*/
	if(abs(Center_f.x-Center_0.x)>300 || abs(Center_f.y-Center_0.y)>300)
	{
		int dataX[3] = {0,0,0};
		int data_X = abs((Center_f.x-Center_0.x)*5.5);//um
		int dataY[3] = {0,0,0};
		int data_Y = abs((Center_f.y-Center_0.y)*5.5);//um

		data_X = data_X/10;
		dataX[2] = data_X%10;
		data_X = data_X/10;
		dataX[1] = data_X%10;
		data_X = data_X/10;
		dataX[0] = data_X;

		data_Y = data_Y/10;
		dataY[2] = data_Y%10;
		data_Y = data_Y/10;
		dataY[1] = data_Y%10;
		data_Y = data_Y/10;
		dataY[0] = data_Y;

		if(trans(1,1,(Center_f.x-Center_0.x),dataX)&&trans(2,1,(Center_f.y-Center_0.y),dataY))
		{


			//X_distace0 = Center_last[1].x-Center_0.x;
			// Y_distace0 = Center_last[1].y-Center_0.y;
			//cout<<"�����Ŀ��������ı�Ϊ:��"<<Center_0.x<<","<<Center_0.y<<")"<<endl;
			AfxMessageBox(_T("�����Ŀ��������ı�Ϊ:"));
		}
		else
		{
			//cout<<"�����Ŀ�������";
			AfxMessageBox("�����Ŀ�������");
		}
	}
	else
	{
		AfxMessageBox("���������Ŀ�������ʱԲ��Ϊ:��)");
		//cout<<"���������Ŀ�������ʱԲ��Ϊ:��"<<Center_f.x<<","<<Center_f.y<<")"<<endl;
		Center_last.push_back(Center_f);
	}
	//
	while(Center_last.size()<2)
	{
		if(Center_last.empty())
		{
			cv::Point2d Center_now;
			//cout<<"�����һ������ֵ��";
			AfxMessageBox("�����һ������ֵ��");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
		}
		else 
		{
			//flag = falseʱ����ƶ�
			flag = true;
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_L*1000);//(um)
			data_Z = data_Z/10;
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			if(!trans(3,1,Z0_L,data))
			{
				//cout<<"Z���ƶ�����";
				AfxMessageBox("Z���ƶ�����");
				return false;
			}
			Z_distance+=Z0_L;

			cv::Point2d Center_now;
			//cout<<"����ڶ�������ֵ��";
			AfxMessageBox("����ڶ�������ֵ��");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
		}
	}

	
	if(Center_last.size()!=2)
	{
		//cout<<"���ε���ʱ�������"<<endl;
		AfxMessageBox("���ε���ʱ�������");
	}
	//cout<<"����ѭ������"<<endl;
	AfxMessageBox("����ѭ������");
	int right_num = 0;
	while(right_num<3)
	{	
		if(Center_last.size()!=2)
		{
			//cout<<"�������"<<endl;
			AfxMessageBox("�������");
			return false;
		}
		//Z���ƶ�һ�Σ���¼����
		//cout<<"Z���ƶ�һ�Σ���¼����"<<endl;
		AfxMessageBox("Z���ƶ�һ�Σ���¼����");
		if(flag == false)//flag = falseʱ����ƶ�
		{
			flag = true;
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_L*1000);//(um)
			data_Z = data_Z/10;
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(3,1,Z0_L,data))
			{
				//cout<<"Z���ƶ�����";
				AfxMessageBox("Z���ƶ�����");
				return false;
			}
			Z_distance+=Z0_L;

			cv::Point2d Center_now;
			//cout<<"���뵱ǰ����ֵ:";
			AfxMessageBox("���뵱ǰ����ֵ:");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
			Center_last.pop_front();
		}
		else
		{
			flag = false;

			int data[3] = {0,0,0};
			int data_Z = Z0_L*1000;//(um)
			data_Z = abs(data_Z/10);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(3,0,Z0_L,data))
			{
				//cout<<"Z���ƶ�����";
				AfxMessageBox("Z���ƶ�����");
				return false;
			}
			Z_distance-=Z0_L;
			cv::Point2d Center_now;
			//cout<<"���뵱ǰ����ֵ��";
			AfxMessageBox("���뵱ǰ����ֵ��");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
			Center_last.pop_front();
		}
		if(abs(Center_last[1].x-Center_last[0].x)<1 && abs(Center_last[1].y-Center_last[0].y)<1 )
		{
			//cout<<"Z���ƶ�֮�����Ĳ���"<<endl;
			AfxMessageBox("Z���ƶ�֮�����Ĳ���");
			++right_num;
		}
		else
		{//////
			cv::Point2d p1 = Center_last[0];
			cv::Point2d p2 = Center_last[1];
			double X_long  = p2.x-p1.x;//X�仯
			double Y_long  = p2.y-p1.y;//Y�仯
			double X_add_now = X_long*Z_distance/Z0_L;
			double Y_add_now = Y_long*Z_distance/Z0_L;
			double angel = 3.1415926;
			double X_R = atan(X_long*5.5/1000/Z0_L);
			double Y_R = atan(Y_long*5.5/1000/Z0_L);

			int dataX[3] = {0,0,0};
			int data_X = abs(X_R/angel*180*3600);
			int dataY[3] = {0,0,0};
			int data_Y = abs(Y_R/angel*180*3600);

			dataX[2] = data_X%60;
			data_X = data_X/60;
			dataX[1] = data_X%60;
			data_X = data_X/60;
			dataX[0] = data_X;

			dataY[2] = data_Y%60;
			data_Y = data_Y/60;
			dataY[1] = data_Y%60;
			data_Y = data_Y/60;
			dataY[0] = data_Y;

			if(flag)//�ո�����ƶ���
			{	
				//cout<<"����ƶ���"<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
				AfxMessageBox("����ƶ���");
				if(trans(4,0,X_add_now,dataX)&&trans(5,0,Y_add_now,dataY))
				{
					//cout<<"�Ƕ�У׼�ɹ���������һ��У׼"<<endl;
					AfxMessageBox("�Ƕ�У׼�ɹ���������һ��У׼");
					cv::Point2d Center_now;
					//cout<<"���뵱ǰ����ֵ��";
					AfxMessageBox("���뵱ǰ����ֵ��");
					Center_now = In_Center_Point(x,y);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						//cout<<"������һ�ε���"<<endl;
						AfxMessageBox("������һ�ε���");
					}
					else
					{
						//cout<<"��������ʧ��"<<endl;
						AfxMessageBox("��������ʧ��");
					}
				}
				else
					//cout<<"�Ƕ�У׼ʧ��"<<endl;
					AfxMessageBox("�Ƕ�У׼ʧ��");

			}
			else
			{
				//cout<<"��ǰ�ƶ���"<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
				AfxMessageBox("��ǰ�ƶ���");
				if(trans(4,1,X_add_now,dataX)&&trans(5,1,Y_add_now,dataY))
				{
					//cout<<"�Ƕ�У׼�ɹ���������һ��У׼"<<endl;
					AfxMessageBox("�Ƕ�У׼�ɹ���������һ��У׼");
					cv::Point2d Center_now;
					//cout<<"���뵱ǰ����ֵ��";
					AfxMessageBox("���뵱ǰ����ֵ��");
					Center_now = In_Center_Point(x,y);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						//cout<<"������һ�ε���"<<endl;
						AfxMessageBox("������һ�ε���");
					}
					else
					{
						//cout<<"��������ʧ��"<<endl;
						AfxMessageBox("��������ʧ��");
					}
				}
				else
					//cout<<"�Ƕ�У׼ʧ��"<<endl;
					AfxMessageBox("�Ƕ�У׼ʧ��");
			}

		}
	}

	return true;
}

bool trans(int X_or_Y_or_Z,int up_down_flag,double distance,int* BYTE_data)
{
	switch(X_or_Y_or_Z)
	{
	case 1:
		{
			if(up_down_flag==1)
			{
				//cout<<"��X�������ƶ���"<<distance<<endl;//X���ʾ03�����X������01
				AfxMessageBox(_T("��X�������ƶ���"));
				if(distance>0)
				{
					int SData[6];
					//cout<<"01 02 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					//myWByte[1024]=010201+BYTE_data[0]+BYTE_data[1]+BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010201_1"));
				}
				else 
				{
					//cout<<"01 02 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010200_2"));
				}		
			}
			else if(up_down_flag==0)
			{
				//cout<<"��X�����ƶ���"<<distance<<endl;
				AfxMessageBox(_T("��X�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 02 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010201_2"));
				}
					
				else 
				{
					//cout<<"01 02 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010200_2"));
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
				AfxMessageBox(_T("X��������ȷ�ķ���"));
				return false;
			}
			break;

		}
	case 2:
		{
			if(up_down_flag==1)
			{
				//cout<<"��Y�������ƶ���"<<distance<<endl;//Y���ʾ03�����Y������00
				AfxMessageBox(_T("��Y�������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 03 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010300_1"));
				}
				else 
				{
					//cout<<"01 03 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010301_1"));
				}	
			}
			else if(up_down_flag==0)
			{
				//cout<<"��Y�����ƶ���"<<distance<<endl;
				AfxMessageBox(_T("��Y�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 03 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010300_2"));
				}
				else 
				{
					//cout<<"01 03 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010301_2"));
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
				AfxMessageBox(_T("Y��������ȷ�ķ���"));
				return false;
			}
			break;
		}
	case 3:
		{
			if(up_down_flag==1)
			{
				//cout<<"��Z�������ƶ���"<<distance<<endl;//X���ʾ01����߰뾶R���00
				AfxMessageBox(_T("��Z�������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 01 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010100_1"));
				}
				else 
				{
					//cout<<"01 01 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010101_1"));
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��Z�����ƶ���"<<distance<<endl;//�������
				AfxMessageBox(_T("��Z�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 01 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010100_2"));
				}
				else 
				{
					//cout<<"01 01 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010101_2"));
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
				AfxMessageBox(_T("Z��������ȷ�ķ���"));
				return false;
			}
			break;
		}
	case 4:
		{
			if(up_down_flag==1)
			{
				//cout<<"��&X�Ƕ��������ƶ���"<<distance<<endl;//&X���ʾ04�����X������01
				AfxMessageBox(_T("��&X�Ƕ��������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 04 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010401_1"));
				}
				else 
				{
					//cout<<"01 04 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010400_1"));
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��&X�Ƕȸ����ƶ���"<<distance<<endl;
				AfxMessageBox(_T("��&X�Ƕȸ����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 04 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010401_2"));
				}
				else 
				{
					//cout<<"01 04 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010400_2"));
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
				AfxMessageBox(_T("&X��������ȷ�ķ���"));
				return false;
			}
			break;
		}
	case 5:
		{
			if(up_down_flag==1)
			{
				//cout<<"��&Y�Ƕ��������ƶ���"<<distance<<endl;//&Y���ʾ05�����Y������00
				AfxMessageBox(_T("��&Y�Ƕ��������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 05 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010500_1"));
				}
				else 
				{
					//cout<<"01 05 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010501_1"));
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��&Y�Ƕȸ����ƶ���"<<distance<<endl;
				AfxMessageBox(_T("��&Y�Ƕȸ����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 05 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010500_2"));
				}
				else 
				{
					//cout<<"01 05 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					AfxMessageBox(_T("010501_2"));
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
				AfxMessageBox(_T("&Y��������ȷ�ķ���"));
				return false;
			}
			break;
		}
	default:AfxMessageBox(_T("��������ȷ�ķ���"));//cout<<"��ѡ����ȷ��ƽ����"<<endl;
		return false;break;
	}
	return true;
}
bool close_to_ceter0(cv::Point2d P_now,deque<cv::Point2d> & Center_last)
{
	double long_limit = 200;
	if(Center_last.size()!=2)
	{
		//cout<<"��¼�����������";
		AfxMessageBox(_T("��¼�����������"));
		return false;
	}
	double X_distace0 = P_now.x-Center_0.x;//��ǰ����ֵ-�������
	double Y_distace0 = P_now.y-Center_0.y;
	if(abs(X_distace0)>long_limit || abs(Y_distace0)>long_limit)
	{
		int dataX[3] = {0,0,0};
		int data_X = abs(X_distace0*5.5);//um
		int dataY[3] = {0,0,0};
		int data_Y = abs(Y_distace0*5.5);//um

		data_X = data_X/10;
		dataX[2] = data_X%10;
		data_X = data_X/10;
		dataX[1] = data_X%10;
		data_X = data_X/10;
		dataX[0] = data_X;

		data_Y = data_Y/10;
		dataY[2] = data_Y%10;
		data_Y = data_Y/10;
		dataY[1] = data_Y%10;
		data_Y = data_Y/10;
		dataY[0] = data_Y;

		if(trans(1,1,X_distace0,dataX)&&trans(2,1,Y_distace0,dataY))
		{
			cv::Point2d P_ceter_now;
			P_ceter_now.x = P_now.x-X_distace0;
			P_ceter_now.y = P_now.y-Y_distace0;
			//X_distace0 = Center_last[1].x-Center_0.x;
			// Y_distace0 = Center_last[1].y-Center_0.y;
			//cout<<"�����Ŀ��������ı�Ϊ:��"<<P_ceter_now.x<<","<<P_ceter_now.y<<")"<<endl;
			AfxMessageBox(_T("�����Ŀ��������ı�Ϊ:"));
			Center_last.push_back(P_ceter_now);
			Center_last.pop_front();
			return true;
		}
		else
		{
			//cout<<"�����Ŀ�������";
			AfxMessageBox(_T("�����Ŀ�������"));
			return false;
		}
	}
	else
	{
		//cout<<"���������Ŀ�������ʱԲ��Ϊ:��"<<P_now.x<<","<<P_now.y<<")"<<endl;
		AfxMessageBox(_T("���������Ŀ�������ʱԲ��Ϊ:"));
		Center_last.push_back(P_now);
		Center_last.pop_front();
		return true;
	}

}
cv::Point2d In_Center_Point(double xc,double yc)
{
	cv::Point2d P;
	//cout<<"X=";
	//cin>>P.x;
	P.x = xc;
	//cout<<"Y=";
	//cin>>P.y;
	P.y = yc;
	return P;
}