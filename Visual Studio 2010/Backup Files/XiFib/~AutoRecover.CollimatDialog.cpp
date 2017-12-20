// CollimatDialog.cpp : 实现文件
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




// CCollimatDialog 对话框

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


// CCollimatDialog 消息处理程序


void CCollimatDialog::OnBnClickedCircleFiber()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码  
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

	// TODO:  在此添加额外的初始化
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCollimatDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	/*double Distance =((CMainFrame *)AfxGetMainWnd())->m_Distance;

	double x=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.xc;

	double y=((CMainFrame *)AfxGetMainWnd())->m_wndEllipse.Epse.yc;

	if(Init_collimation(Distance,LhalfW,x,y))
		AfxMessageBox(_T("准直成功"));
	else
		AfxMessageBox(_T("准直失败"));*/
	/*AfxMessageBox(_T("准直成功"));
	AfxMessageBox(_T("准直失败"));*/

	//准直函数1
	CDialogEx::OnOK();
	
}

//准直
//X，Y，Z，rX,rY对应于(1,2,3,4,5)
//flag：=1对应于正向，=0对应于负向；
bool Init_collimation(double Distance,double LhalfW,double x,double y)
{
	double Z0_L = 2;//每次将Z轴进行移动的距离
	bool flag = false;//Z轴移动方向标记
	int flag_x_y = 0;//角度&X,&Y移动方向

	std::deque<cv::Point2d> Center_last;
	double R_long = 0;//圆心与中心的距离
	/*double Z_distance = 0;
	cout<<"请输入Z轴初始距离：";
	cin>>Z_distance; */
	double Z_distance = Distance; 
	double L_short = 150;//短轴长度
	double L_long  = 0;//长轴长度
	while(L_short>120)
	{
		/*cout<<"请输入短轴长度：";
		cin>>L_short;*/
		L_short = LhalfW;
		int data[3] = {0,0,0};
		int data_Z = abs(Z0_L*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(3,0,Z0_L,data);//反复向前移动
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
			//cout<<"向中心靠近后中心变为:（"<<Center_0.x<<","<<Center_0.y<<")"<<endl;
			AfxMessageBox(_T("向中心靠近后中心变为:"));
		}
		else
		{
			//cout<<"向中心靠近出错";
			AfxMessageBox("向中心靠近出错");
		}
	}
	else
	{
		AfxMessageBox("不用向中心靠近，此时圆心为:（)");
		//cout<<"不用向中心靠近，此时圆心为:（"<<Center_f.x<<","<<Center_f.y<<")"<<endl;
		Center_last.push_back(Center_f);
	}
	//
	while(Center_last.size()<2)
	{
		if(Center_last.empty())
		{
			cv::Point2d Center_now;
			//cout<<"读入第一个坐标值：";
			AfxMessageBox("读入第一个坐标值：");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
		}
		else 
		{
			//flag = false时向后移动
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
				//cout<<"Z轴移动出错";
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance+=Z0_L;

			cv::Point2d Center_now;
			//cout<<"读入第二个坐标值：";
			AfxMessageBox("读入第二个坐标值：");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
		}
	}

	
	if(Center_last.size()!=2)
	{
		//cout<<"初次调节时数组出错"<<endl;
		AfxMessageBox("初次调节时数组出错");
	}
	//cout<<"进入循环调节"<<endl;
	AfxMessageBox("进入循环调节");
	int right_num = 0;
	while(right_num<3)
	{	
		if(Center_last.size()!=2)
		{
			//cout<<"数组出错"<<endl;
			AfxMessageBox("数组出错");
			return false;
		}
		//Z轴移动一次，记录数据
		//cout<<"Z轴移动一次，记录数据"<<endl;
		AfxMessageBox("Z轴移动一次，记录数据");
		if(flag == false)//flag = false时向后移动
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
				//cout<<"Z轴移动出错";
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance+=Z0_L;

			cv::Point2d Center_now;
			//cout<<"读入当前坐标值:";
			AfxMessageBox("读入当前坐标值:");
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
				//cout<<"Z轴移动出错";
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance-=Z0_L;
			cv::Point2d Center_now;
			//cout<<"读入当前坐标值：";
			AfxMessageBox("读入当前坐标值：");
			Center_now = In_Center_Point(x,y);
			/*Center_now.x = m_Ellipse.xc;
			Center_now.y = m_Ellipse.yc;*/
			Center_last.push_back(Center_now);
			Center_last.pop_front();
		}
		if(abs(Center_last[1].x-Center_last[0].x)<1 && abs(Center_last[1].y-Center_last[0].y)<1 )
		{
			//cout<<"Z轴移动之后中心不变"<<endl;
			AfxMessageBox("Z轴移动之后中心不变");
			++right_num;
		}
		else
		{//////
			cv::Point2d p1 = Center_last[0];
			cv::Point2d p2 = Center_last[1];
			double X_long  = p2.x-p1.x;//X变化
			double Y_long  = p2.y-p1.y;//Y变化
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

			if(flag)//刚刚向后移动过
			{	
				//cout<<"向后移动："<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
				AfxMessageBox("向后移动：");
				if(trans(4,0,X_add_now,dataX)&&trans(5,0,Y_add_now,dataY))
				{
					//cout<<"角度校准成功，进入下一次校准"<<endl;
					AfxMessageBox("角度校准成功，进入下一次校准");
					cv::Point2d Center_now;
					//cout<<"读入当前坐标值：";
					AfxMessageBox("读入当前坐标值：");
					Center_now = In_Center_Point(x,y);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						//cout<<"进行下一次调节"<<endl;
						AfxMessageBox("进行下一次调节");
					}
					else
					{
						//cout<<"靠近中心失败"<<endl;
						AfxMessageBox("靠近中心失败");
					}
				}
				else
					//cout<<"角度校准失败"<<endl;
					AfxMessageBox("角度校准失败");

			}
			else
			{
				//cout<<"向前移动："<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
				AfxMessageBox("向前移动：");
				if(trans(4,1,X_add_now,dataX)&&trans(5,1,Y_add_now,dataY))
				{
					//cout<<"角度校准成功，进入下一次校准"<<endl;
					AfxMessageBox("角度校准成功，进入下一次校准");
					cv::Point2d Center_now;
					//cout<<"读入当前坐标值：";
					AfxMessageBox("读入当前坐标值：");
					Center_now = In_Center_Point(x,y);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						//cout<<"进行下一次调节"<<endl;
						AfxMessageBox("进行下一次调节");
					}
					else
					{
						//cout<<"靠近中心失败"<<endl;
						AfxMessageBox("靠近中心失败");
					}
				}
				else
					//cout<<"角度校准失败"<<endl;
					AfxMessageBox("角度校准失败");
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
				//cout<<"将X向正向移动："<<distance<<endl;//X轴表示03，光斑X坐标变大：01
				AfxMessageBox(_T("将X向正向移动："));
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
				//cout<<"将X向负向移动："<<distance<<endl;
				AfxMessageBox(_T("将X向负向移动："));
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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("X请输入正确的方向"));
				return false;
			}
			break;

		}
	case 2:
		{
			if(up_down_flag==1)
			{
				//cout<<"将Y向正向移动："<<distance<<endl;//Y轴表示03，光斑Y坐标变大：00
				AfxMessageBox(_T("将Y向正向移动："));
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
				//cout<<"将Y向负向移动："<<distance<<endl;
				AfxMessageBox(_T("将Y向负向移动："));
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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("Y请输入正确的方向"));
				return false;
			}
			break;
		}
	case 3:
		{
			if(up_down_flag==1)
			{
				//cout<<"将Z向正向移动："<<distance<<endl;//X轴表示01，光斑半径R变大：00
				AfxMessageBox(_T("将Z向正向移动："));
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
				//cout<<"将Z向负向移动："<<distance<<endl;//靠近相机
				AfxMessageBox(_T("将Z向负向移动："));
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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("Z请输入正确的方向"));
				return false;
			}
			break;
		}
	case 4:
		{
			if(up_down_flag==1)
			{
				//cout<<"将&X角度向正向移动："<<distance<<endl;//&X轴表示04，光斑X坐标变大：01
				AfxMessageBox(_T("将&X角度向正向移动："));
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
				//cout<<"将&X角度负向移动："<<distance<<endl;
				AfxMessageBox(_T("将&X角度负向移动："));
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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("&X请输入正确的方向"));
				return false;
			}
			break;
		}
	case 5:
		{
			if(up_down_flag==1)
			{
				//cout<<"将&Y角度向正向移动："<<distance<<endl;//&Y轴表示05，光斑Y坐标变大：00
				AfxMessageBox(_T("将&Y角度向正向移动："));
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
				//cout<<"将&Y角度负向移动："<<distance<<endl;
				AfxMessageBox(_T("将&Y角度负向移动："));
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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("&Y请输入正确的方向"));
				return false;
			}
			break;
		}
	default:AfxMessageBox(_T("请输入正确的方向"));//cout<<"请选择正确的平移轴"<<endl;
		return false;break;
	}
	return true;
}
bool close_to_ceter0(cv::Point2d P_now,deque<cv::Point2d> & Center_last)
{
	double long_limit = 200;
	if(Center_last.size()!=2)
	{
		//cout<<"记录数据数组出错";
		AfxMessageBox(_T("记录数据数组出错"));
		return false;
	}
	double X_distace0 = P_now.x-Center_0.x;//当前坐标值-相机中心
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
			//cout<<"向中心靠近后中心变为:（"<<P_ceter_now.x<<","<<P_ceter_now.y<<")"<<endl;
			AfxMessageBox(_T("向中心靠近后中心变为:"));
			Center_last.push_back(P_ceter_now);
			Center_last.pop_front();
			return true;
		}
		else
		{
			//cout<<"向中心靠近出错";
			AfxMessageBox(_T("向中心靠近出错"));
			return false;
		}
	}
	else
	{
		//cout<<"不用向中心靠近，此时圆心为:（"<<P_now.x<<","<<P_now.y<<")"<<endl;
		AfxMessageBox(_T("不用向中心靠近，此时圆心为:"));
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