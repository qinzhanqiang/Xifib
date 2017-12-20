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


// CCollimatDialog ��Ϣ��������


void CCollimatDialog::OnBnClickedCircleFiber()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
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
	// TODO: �ڴ����ӿؼ�֪ͨ�����������  
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

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCollimatDialog::OnBnClickedOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CDialogEx::OnOK();
	/*double Distance =((CMainFrame *)AfxGetMainWnd())->m_Distance;

	double x=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.xc;

	double y=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.yc;*/

	/******************׼ֱ�߳�****************/
	m_cRun = TRUE;
	//��ʼ���߳�����
	/*THREAD_DATA threadData;
	threadData.Dist = Distance;
	threadData.LhW = LhalfW;
	threadData.x = g_Px;
	threadData.y = g_Py;*/

	

	
	HANDLE thread = CreateThread(NULL, 
		0, 
		ThreadProcCollimat, 
		//&threadData,
		NULL,
		0, 
		NULL);//����һ��׼ֱ�߳�
	//WaitForSingleObject(g_hMutex, INFINITE);//������һ����������
	CloseHandle(thread);//�ر��߳�9
	
	
	
	
}


DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter)
{
	//WaitForSingleObject(g_hMutex, INFINITE);//������һ����������
	//ΪԲ�ι�����ֱ�ӽ���׼ֱ��Ш�ι������ȼ���ƫ����ٽ���׼ֱ
	if(!FiberModelFlag)
	{
		//����ƫ���
		vector<cv::Point2d> arr;
		DataGetPoint(arr);
		//cout<<"ƫ���Ϊ��"<<Partial_degree(arr)<<endl;//�����doubleֵ����ƫ��ȵı���;
		PartialAng = Partial_degree(arr);
		while(m_cRun)
		{

			if(Init_collimation())
			{
				AfxMessageBox(_T("׼ֱ�ɹ�"));
				m_cRun = FALSE;
			}
			else
			{
				AfxMessageBox(_T("׼ֱʧ��"));
				m_cRun = FALSE;
			}

		}

		vector<double> Xarr;
		vector<double> Yarr;
		DataGetLength(Xarr,Yarr);
		//cout<<"������㳤�ȣ�"<<GetDivAng(Xarr)<<endl;
		//cout<<"������㳤�ȣ�"<<GetDivAng(Yarr)<<endl;//�����doubleֵ�����Ǹ����ȵı���;

		Dist = GetDivAng(Xarr,0.01);
	//	Dist = GetDivAng(Yarr,0.02);
	}
	else
	{
		while(m_cRun)
		{

			if(Init_collimation())
			{
				AfxMessageBox(_T("׼ֱ�ɹ�"));
				m_cRun = FALSE;
			}
			else
			{
				AfxMessageBox(_T("׼ֱʧ��"));
				m_cRun = FALSE;
			}

		}

		vector<double> Xarr;
		vector<double> Yarr;
		DataGetLength(Xarr,Yarr);
		//cout<<"������㳤�ȣ�"<<GetDivAng(Xarr)<<endl;
		//cout<<"������㳤�ȣ�"<<GetDivAng(Yarr)<<endl;//�����doubleֵ�����Ǹ����ȵı���;

		Dist = GetDivAng(Xarr,0.01);
    	//Dist = GetDivAng(Yarr,0.01);
		Dist = GetDivAngNew1(Xarr,Yarr,0.05,0.1);
	}
	//THREAD_DATA* pThreadData = (THREAD_DATA*)lpParameter;
	
	//m_cRun = TRUE;

	//cout<<"�����׼ֱ��������"<<endl;
	//while(m_cRun)
	//{
	//	
	//	if(Init_collimation())
	//	{
	//		AfxMessageBox(_T("׼ֱ�ɹ�"));
	//		m_cRun = FALSE;
	//	}
	//	else
	//	{
	//		AfxMessageBox(_T("׼ֱʧ��"));
	//		m_cRun = FALSE;
	//	}
	//	
	//}
	//
	//vector<double> Xarr;
	//vector<double> Yarr;
	//DataGetLength(Xarr,Yarr);
	////cout<<"������㳤�ȣ�"<<GetDivAng(Xarr)<<endl;
	////cout<<"������㳤�ȣ�"<<GetDivAng(Yarr)<<endl;//�����doubleֵ�����Ǹ����ȵı���;
	//
	//Dist = GetDivAng(Xarr,0.1);
	//Dist = GetDivAng(Yarr,0.1);
	//m_cRun = FALSE;
	//ReleaseMutex(g_hMutex);//�ͷŻ�������
	AfxMessageBox(_T("�����ɹ�"));
	return 0;
}


//׼ֱ
//X��Y��Z��rX,rY��Ӧ��(1,2,3,4,5)
//flag��=1��Ӧ������=0��Ӧ�ڸ���
bool Init_collimation()
{
	double Z0_L = 5;//ÿ�ν�Z������ƶ��ľ���
	double Z0_L_R=2;
	double Z0_C=1;
	bool flag = false;//Z���ƶ�������
	int flag_x_y = 0;//�Ƕ�&X,&Y�ƶ�����

	std::deque<cv::Point2d> Center_last;
	double R_long = 0;//Բ�������ĵľ���
	/*double Z_distance = 0;
	cout<<"������Z���ʼ���룺";
	cin>>Z_distance; */
	double Z_distance = Distance; 
	double L_short = LhalfW;//���᳤��
	//double L_long  = 0;//���᳤��
	
	//
	cv::Point2d Center_f = In_Center_Point(g_Px,g_Py);
	//cv::Point2d Center_f=(g_Px,g_Py);
	/*Center_f.x = m_Ellipse.xc;
	Center_f.y = m_Ellipse.yc;*/
	//�������ƶ���ָ��λ�ø���
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

			//cv::Point2d P_center_now0=In_Center_Point(g_Px,g_Py);
			cv::Point2d P_center_now0=In_Center_Point(g_Px,g_Py);;
			//X_distace0 = Center_last[1].x-Center_0.x;
			// Y_distace0 = Center_last[1].y-Center_0.y;
			//cout<<"�����Ŀ��������ı�Ϊ:��"<<Center_0.x<<","<<Center_0.y<<")"<<endl;
			
			//		AfxMessageBox(_T("�����Ŀ��������ı�Ϊ:"));
		}
		else
		{
			//cout<<"�����Ŀ�������";
			AfxMessageBox("�����Ŀ�������");
		}
	}
	else
	{
		//	AfxMessageBox("���������Ŀ�������ʱԲ��Ϊ:��)");
		//cout<<"���������Ŀ�������ʱԲ��Ϊ:��"<<Center_f.x<<","<<Center_f.y<<")"<<endl;
		;//Center_last.push_back(Center_f);
	}


	//while(L_short>140)
	//{		
	//	int data[3] = {0,0,0};
	//	int data_Z = abs(Z0_C*100);
	//	data[2] = data_Z%10;
	//	data_Z = data_Z/10;
	//	data[1] = data_Z%10;
	//	data_Z = data_Z/10;
	//	data[0] = data_Z;
	//	trans(3,0,Z0_L,data);//������ǰ�ƶ�		
	//	Sleep(1000);
	//	L_short = LhalfW;

	//}
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,0,Z0_C,data);//������ǰ�ƶ�		
			Sleep(1000);//��ʱ
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,1,Z0_C,data);//��������ƶ�		
			Sleep(1000);
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
	}

	//
	while(Center_last.size()<2)
	{
		if(Center_last.empty())
		{
			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			//cout<<"�����һ������ֵ��";
			//AfxMessageBox("�����һ������ֵ��");
			//Center_now = In_Center_Point(g_Px,g_Py);
		//	Center_now = (g_Px,g_Py);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);

		}
		else 
		{
			//flag = falseʱ����ƶ�
			flag = true;
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_L_R*1000);//(um)
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

			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			//cout<<"����ڶ�������ֵ��";
		//	AfxMessageBox("����ڶ�������ֵ��");
			//Center_now = In_Center_Point(g_Px,g_Py);
		//	Center_now = (g_Px,g_Py);
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
//	AfxMessageBox("����ѭ������");
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
	//	AfxMessageBox("Z���ƶ�һ�Σ���¼����");
		if(flag == false)//flag = falseʱ����ƶ�
		{
			flag = true;
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_L_R*1000);//(um)
			data_Z = data_Z/10;
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(3,1,Z0_L_R,data))
			{
				//cout<<"Z���ƶ�����";
				AfxMessageBox("Z���ƶ�����");
				return false;
			}
			Z_distance+=Z0_L_R;
			//Center_now = In_Center_Point(g_Px,g_Py);
			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			//cout<<"���뵱ǰ����ֵ:";
		//	AfxMessageBox("���뵱ǰ����ֵ:");
			
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/

			Center_last.push_back(Center_now);
			Center_last.pop_front();

		}
		else
		{
			flag = false;

			int data[3] = {0,0,0};
			int data_Z = Z0_L_R*1000;//(um)
			data_Z = abs(data_Z/10);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(3,0,Z0_L_R,data))
			{
				//cout<<"Z���ƶ�����";
				AfxMessageBox("Z���ƶ�����");
				return false;
			}
			Z_distance-=Z0_L_R;
			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			//cout<<"���뵱ǰ����ֵ��";
		//		AfxMessageBox("���뵱ǰ����ֵ��");
			//Center_now = In_Center_Point(g_Px,g_Py);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
			Center_last.pop_front();

		}
		if(abs(Center_last[1].x-Center_last[0].x)<1 && abs(Center_last[1].y-Center_last[0].y)<1 )
		{
			//cout<<"Z���ƶ�֮�����Ĳ���"<<endl;
			//AfxMessageBox("Z���ƶ�֮�����Ĳ���");
			++right_num;
		}
		else
		{//////
			cv::Point2d p1 = Center_last[0];

			cv::Point2d p2 = Center_last[1];

			CString xs1[4];
			int datatatat[4]={0,0,0,0};
			datatatat[0]=p1.x;
			datatatat[1]=p1.y;
			datatatat[2]=p2.x;
			datatatat[3]=p2.y;
			xs1[0].Format(_T("%d"),datatatat[0]);
			xs1[1].Format(_T("%d"),datatatat[1]);
			xs1[2].Format(_T("%d"),datatatat[2]);
			xs1[3].Format(_T("%d"),datatatat[3]);
		
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
			//	AfxMessageBox("����ƶ���");
			//		AfxMessageBox("����ƶ���"+xs1[0]+","+xs1[1]+"-----"+xs1[2]+","+xs1[3]);
				if(trans(4,0,X_add_now,dataX)&&trans(5,0,Y_add_now,dataY))
				{
					//cout<<"�Ƕ�У׼�ɹ���������һ��У׼"<<endl;
			//		AfxMessageBox("�Ƕ�У׼�ɹ���������һ��У׼");
					cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
					//cout<<"���뵱ǰ����ֵ��";
			//		AfxMessageBox("���뵱ǰ����ֵ��");
				//	Center_now = In_Center_Point(g_Px,g_Py);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						Center_now = In_Center_Point(g_Px,g_Py);
						Center_last.push_back(Center_now);
						Center_last.pop_front();
						//cout<<"������һ�ε���"<<endl;
			//			AfxMessageBox("������һ�ε���");
					}
					else
					{
						//cout<<"��������ʧ��"<<endl;
						AfxMessageBox("��������ʧ��");
					}
					Sleep(1000);
					Center_now = In_Center_Point(g_Px,g_Py);
					Center_last.push_back(Center_now);
					Center_last.pop_front();
				}
				else
					//cout<<"�Ƕ�У׼ʧ��"<<endl;
					AfxMessageBox("�Ƕ�У׼ʧ��");

			}
			else
			{
				//cout<<"��ǰ�ƶ���"<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
		//		AfxMessageBox("��ǰ�ƶ���");
			//	AfxMessageBox("��ǰ�ƶ���"+xs1[0]+","+xs1[1]+"-----"+xs1[2]+","+xs1[3]);
				if(trans(4,1,X_add_now,dataX)&&trans(5,1,Y_add_now,dataY))
				{
					//cout<<"�Ƕ�У׼�ɹ���������һ��У׼"<<endl;
		//			AfxMessageBox("�Ƕ�У׼�ɹ���������һ��У׼");
					cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
					//cout<<"���뵱ǰ����ֵ��";
		//			AfxMessageBox("���뵱ǰ����ֵ��");
					//Center_now = In_Center_Point(g_Px,g_Py);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/

					if(close_to_ceter0(Center_now,Center_last))
					{
						Center_now = In_Center_Point(g_Px,g_Py);
						Center_last.push_back(Center_now);
						Center_last.pop_front();
						//cout<<"������һ�ε���"<<endl;
		//				AfxMessageBox("������һ�ε���");
					}
					else
					{
						//cout<<"��������ʧ��"<<endl;
						AfxMessageBox("��������ʧ��");
					}
					Sleep(1000);
					Center_now = In_Center_Point(g_Px,g_Py);
					Center_last.push_back(Center_now);
					Center_last.pop_front();
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
	if(BYTE_data[0]==0&&BYTE_data[1]==0&&BYTE_data[2]<10)
		return true;

	switch(X_or_Y_or_Z)
	{
	case 1:
		{
			if(up_down_flag==1)
			{
				//cout<<"��X�������ƶ���"<<distance<<endl;//X���ʾ02�����X������01
				//AfxMessageBox(_T("��X�������ƶ���"));
				if(distance>0)
				{
					int SData[6];
					//cout<<"01 02 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
				/*	CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					AfxMessageBox("01 02 01"+xs[0]+xs[1]+xs[2]);*/
					
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];					
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010201_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 02 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					/*CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					AfxMessageBox("01 02 00"+xs[0]+xs[1]+xs[2]);*/
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010200_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}		
			}
			else if(up_down_flag==0)
			{
				//cout<<"��X�����ƶ���"<<distance<<endl;
				//AfxMessageBox(_T("��X�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 02 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					/*CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					AfxMessageBox("01 02 01"+xs[0]+xs[1]+xs[2]);
					*/
					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010201_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
					
				else 
				{
					//cout<<"01 02 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 02 00"+xs[0]+xs[1]+xs[2]);

					int SData[6];
					SData[0] = 01;
					SData[1] = 02;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010200_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
					{
						cv::Point2d CenterAB1 = (g_Px,g_Py);
						for(int i=0;i<1000;++i)
							;
						cv::Point2d CenterAB2 = (g_Px,g_Py);
						if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
							break;
					}
						next_ok=false;
						return true;
					}
					
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
				//AfxMessageBox(_T("��Y�������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 03 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					AfxMessageBox("01 03 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010300_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 03 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 03 01"+xs[0]+xs[1]+xs[2]);

					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010301_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}	
			}
			else if(up_down_flag==0)
			{
				//cout<<"��Y�����ƶ���"<<distance<<endl;
				//AfxMessageBox(_T("��Y�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 03 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 03 00"+xs[0]+xs[1]+xs[2]);

					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010300_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 03 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 03 01"+xs[0]+xs[1]+xs[2]);

					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010301_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
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
				//AfxMessageBox(_T("��Z�������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 01 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 01 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010100_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 01 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 01 01"+xs[0]+xs[1]+xs[2]);

					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010101_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��Z�����ƶ���"<<distance<<endl;//�������
				//AfxMessageBox(_T("��Z�����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 01 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 01 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010100_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 01 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					//AfxMessageBox("01 01 01"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 01;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010101_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
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
				//AfxMessageBox(_T("��&X�Ƕ��������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 04 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 04 01"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010401_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 04 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					//AfxMessageBox("01 04 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010400_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��&X�Ƕȸ����ƶ���"<<distance<<endl;
			//	AfxMessageBox(_T("��&X�Ƕȸ����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 04 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 04 01"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010401_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 04 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
				//	AfxMessageBox("01 04 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 04;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010400_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
			}
			else
			{
				//cout<<"��������ȷ�ķ���"<<endl;
		//		AfxMessageBox(_T("&X��������ȷ�ķ���"));
				return false;
			}
			break;
		}
	case 5:
		{
			if(up_down_flag==1)
			{
				//cout<<"��&Y�Ƕ��������ƶ���"<<distance<<endl;//&Y���ʾ05�����Y������00
	//			AfxMessageBox(_T("��&Y�Ƕ��������ƶ���"));
				if(distance>0)
				{
					//cout<<"01 05 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
		//			AfxMessageBox("01 05 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010500_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 05 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
			//		AfxMessageBox("01 05 01"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010501_1"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
			}
			else if(up_down_flag==0)
			{
				//cout<<"��&Y�Ƕȸ����ƶ���"<<distance<<endl;
			//	AfxMessageBox(_T("��&Y�Ƕȸ����ƶ���"));
				if(distance<0)
				{
					//cout<<"01 05 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
			//		AfxMessageBox("01 05 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 00;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010500_2"));
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
						}
						next_ok=false;
						return true;
					}
				}
				else 
				{
					//cout<<"01 05 01 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
			//		AfxMessageBox("01 05 01"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 05;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010501_2"));
					/*while(myRByte[0]!=10)
					{
						;
					}
					if(myRByte[0]==10)
						return true;*/
					while(!next_ok)
					{
						;
					}
					if(next_ok)
					{
						int num=0;
						while(1)
						{
							cv::Point2d CenterAB1 = (g_Px,g_Py);
							for(int i=0;i<1000;++i)
								;
							cv::Point2d CenterAB2 = (g_Px,g_Py);
							if(CenterAB1.x-CenterAB2.x<1&&CenterAB1.y-CenterAB2.y<1)
								break;
							++num;
							if(num>1000)
								break;
						}
						next_ok=false;
						return true;
					}
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
	//		AfxMessageBox(_T("�����Ŀ��������ı�Ϊ:"));
			
					cv::Point2d P_ceter_now1=In_Center_Point(g_Px,g_Py);
					Center_last.push_back(P_ceter_now1);
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
	//	AfxMessageBox(_T("���������Ŀ�������ʱԲ��Ϊ:"));
		
					
					Center_last.push_back(P_now);
					Center_last.pop_front();
					return true;
			
		
	}

}
cv::Point2d In_Center_Point(double xc,double yc)
{
	Sleep(100);
	while(1)
	{
		if(!next_ok)
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
	}
	cv::Point2d P;
	//cout<<"X=";
	//cin>>P.x;
	P.x = xc;
	//cout<<"Y=";
	//cin>>P.y;
	P.y = yc;
	return P;
}

//ƫ��ȼ���
double Partial_degree(vector<cv::Point2d> arr)
{
	vector<double> ResultValue;
	vector<int> ResultNum;
	double Thre=0.02;
	double pi=3.1415926;
	double AddL=0.5;
	for(int i=0;i<arr.size();++i)
	{
		for(int j=i+1;j<arr.size();++j)
		{
			double SquareDiff=sqrt((arr[j].x-arr[i].x)*5.5*5.5*(arr[j].x-arr[i].x)+(arr[j].y-arr[i].y)*5.5*5.5*(arr[j].y-arr[i].y));
			double ang=(atan(SquareDiff/1000/(j-i)/AddL))/pi*180;
			int k;
			for( k=0;k<ResultValue.size();++k)
			{
				if(abs(ResultValue[k]-ang)<Thre)
				{
					ResultValue[k]=(ResultValue[k]+ang)/2;
					++ResultNum[k];
					break;
				}
			}
			if(k==ResultValue.size())
			{
				ResultValue.push_back(ang);
				ResultNum.push_back(1);
			}
		}
	}

	double maxnum=0;
	double result=ResultValue[0];
	for(int i=0;i<ResultNum.size();++i)
	{
		if(ResultNum[i]>maxnum)
		{
			maxnum=ResultNum[i];
			result=ResultValue[i];
		}
	}

	return result;
}

//Z�᳤�ȣ���ɢ�ǣ�����
double GetDivAng(vector<double> arr,double Thre)
{
	vector<double> ResultValue;
	vector<int> ResultNum;
	//double Thre=0.1;
	double AddL=0.5;
	double pi=3.1415926;
	double Length=10.0;
	int MaxNum=0;
	
	double RightLen=Length;
	while(Length<18)
	{
		vector<double> ResultValue1;
		vector<int> ResultNum1;
		for(int i=0;i<arr.size();++i)
		{
			double ang=2*(atan(arr[i]*5.5/1000/(Length+AddL*i)))/pi*180;
			int j;
			for(j=0;j<ResultValue1.size();++j)
			{
				if(abs(ang-ResultValue1[j])<Thre)
				{
					ResultValue1[j]=(ResultValue1[j]+ang)/2;
					++ResultNum1[j];
				}
			}
			if(j==ResultValue1.size())
			{
				ResultValue1.push_back(ang);
				ResultNum1.push_back(1);
			}
		}
		for(int i=0;i<ResultNum1.size();++i)
		{
			if(ResultNum1[i]>MaxNum)
			{
				MaxNum=ResultNum1[i];
				RightLen=Length;
			}
		}
		Length+=0.01;
	}
	MaxNum=0;
	Length=18;
	int LeftLen=Length;
	while(Length>9)
	{
		vector<double> ResultValue1;
		vector<int> ResultNum1;
		for(int i=0;i<arr.size();++i)
		{
			double ang=2*(atan(arr[i]*5.5/1000/(Length+AddL*i)))/pi*180;
			int j;
			for(j=0;j<ResultValue1.size();++j)
			{
				if(abs(ang-ResultValue1[j])<Thre)
				{
					ResultValue1[j]=(ResultValue1[j]+ang)/2;
					++ResultNum1[j];
				}
			}
			if(j==ResultValue1.size())
			{
				ResultValue1.push_back(ang);
				ResultNum1.push_back(1);
			}
		}
		for(int i=0;i<ResultNum1.size();++i)
		{
			if(ResultNum1[i]>MaxNum)
			{
				MaxNum=ResultNum1[i];
				LeftLen=Length;
			}
		}
		Length-=0.01;
	}
	//return (RightLen+LeftLen)/2;
	return RightLen;
}

double GetDivAngNew1(vector<double> arrX,vector<double> arrY,double ThreX,double ThreY)
{
	double AddL=0.5;
	double pi=3.1415926;
	double Length=9.0;
	int MaxNum=0;
	double RightLen=Length;
	while(Length<18)
	{
		vector<double> ResultValue1;
		vector<int> ResultNum1;
		for(int i=0;i<arrX.size();++i)
		{
			double ang=2*(atan(arrX[i]*5.5/1000/(Length+AddL*i)))/pi*180;
			int j;
			ResultValue1.push_back(ang);
			ResultNum1.push_back(1);
			for(j=0;j<ResultValue1.size();++j)
			{
				if(abs(ang-ResultValue1[j])<ThreX)
				{
					++ResultNum1[ResultNum1.size()-1];
					++ResultNum1[j];
				}
			}
		}
		int maxnow=0;
		for(int i=0;i<ResultNum1.size();++i)
		{
			if(ResultNum1[i]>maxnow)
			{
				maxnow=ResultNum1[i];
			}
		}

		if(maxnow>MaxNum)
		{
			MaxNum=maxnow;
			RightLen=Length;
		}
		else if(MaxNum-maxnow<2)
		{
			vector<double> ResultValue2;
			vector<double> ResultValue3;
			int max2=0;
			int max3=0;
			for(int i=0;i<arrY.size();++i)
			{
				double ang2=2*(atan(arrY[i]*5.5/1000/(Length+AddL*i)))/pi*180;
				double ang3=2*(atan(arrY[i]*5.5/1000/(RightLen+AddL*i)))/pi*180;
				ResultValue2.push_back(ang2);
				ResultValue3.push_back(ang3);				
				if((abs(ang2-ResultValue2[0]))<ThreY)
				{
					++max2;
				}
				if((abs(ang3-ResultValue3[0]))<ThreY)
				{
					++max3;
				}
			}
			if(max2>max3)
			{
				//MaxNum=maxnow;
				RightLen=Length;
			}
		}


		Length+=0.01;
	}
	return RightLen;
}
//���ݲɼ�
bool DataGetLength(vector<double> &Xarr,vector<double> &Yarr)
{
	//double LhalfW;
	//cout<<"��������᳤�ȣ�";
	//cin>>LhalfW;
	double L_short = LhalfW;//���᳤��
	double Z0_L = 5;//ÿ�ν�Z������ƶ��ľ��루��λ��mm��
	double Z0_C=1;
	double AddL=0.5;

	//������߾���
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,0,Z0_C,data);//������ǰ�ƶ�		
			Sleep(1000);//��ʱ
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,1,Z0_C,data);//��������ƶ�		
			Sleep(1000);
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
	}

	//cout<<"�������ߴ�С��";
	//��ʽ�ɼ�����
	int Num=10;//�ɼ�����
	for(int i=0;i<Num;++i)
	{

		double XL;
		//cout<<"���᣺";
		//cin>>XL;
		XL = LhalfW;
		//	cout<<"���᣺";
		double YL;
		//cin>>YL;
		YL = halfW;

		Xarr.push_back(XL);
		Yarr.push_back(YL);
		if(i==Num-1)
			break;
		//����ƶ��ɼ�
		int data[3] = {0,0,0};
		int data_Z = abs(AddL*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(3,1,AddL,data);//��������ƶ�	
		Sleep(1000);
	}
	//�ص�ԭ��
	int data[3] = {0,0,0};
	int data_Z = abs(AddL*(Num-1)*100);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	trans(3,0,AddL*(Num-1),data);//��������ƶ�	
	return true;
}
bool DataGetPoint(vector<cv::Point2d> &arr)
{
	//double LhalfW;
	//cout<<"��������᳤�ȣ�";
	//cin>>LhalfW;
	double L_short = LhalfW;//���᳤��
	double Z0_L = 5;//ÿ�ν�Z������ƶ��ľ��루��λ��mm��
	double Z0_C=1;
	double AddL=0.5;

	//������߾���
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = abs(AddL*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,0,AddL,data);//������ǰ�ƶ�		
			Sleep(1000);//��ʱ
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(3,1,Z0_C,data);//��������ƶ�		
			Sleep(1000);
			//cout<<"��������᳤�ȣ�";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
	}

	//��ʽ�ɼ�����
	int Num=10;//�ɼ�����
	for(int i=0;i<Num;++i)
	{
		//cout<<"�������������꣺";
		cv::Point2d CPoint;
		//cout<<"X=";
		//cin>>CPoint.x;
		CPoint.x = g_Px;
		//cout<<"Y=";
		//cin>>CPoint.y;
		CPoint.x = g_Py;
		arr.push_back(CPoint);
		if(i==Num-1)
			break;
		//����ƶ��ɼ�
		int data[3] = {0,0,0};
		int data_Z = abs(AddL*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(3,1,AddL,data);//��������ƶ�	
		Sleep(1000);
	}
	//�ص�ԭ��
	int data[3] = {0,0,0};
	int data_Z = abs(AddL*(Num-1)*100);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	trans(3,0,AddL*(Num-1),data);//��������ƶ�	
	return true;
}