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
	CDialogEx::OnOK();
	/******************准直线程****************/
	m_cRun = TRUE;
	//初始化线程数据
	/*THREAD_DATA threadData;
	threadData.Dist = Distance;
	threadData.LhW = LhalfW;
	threadData.x = g_Px;
	threadData.y = g_Py;*/
	extern LPSETUP lps;
	HANDLE thread = CreateThread(NULL, 
		0, 
		ThreadProcCollimat, 
		//&threadData,
		lps,
		0, 
		NULL);//创建一个准直线程
	//WaitForSingleObject(g_hMutex, INFINITE);//请求获得一个互斥量锁
	CloseHandle(thread);//关闭线程句柄

}

//准直线程
DWORD WINAPI ThreadProcCollimat(LPVOID lpParameter)
{
	//WaitForSingleObject(g_hMutex, INFINITE);//请求获得一个互斥量锁
	extern bool flag_start;
	int mesure_success_n = 0;
	double save_aver_long = 0;
	double save_aver_short = 0;
	LPSETUP lps = (LPSETUP)lpParameter;
	CListCtrl &dataList = lps->ShowWin->m_wndData.dataList;
	CListCtrl &dataSumList = lps->ShowWin->m_wndDataSum.dataList;
	
	double data_long[10], data_short[10];
	CString Cdata_long, Cdata_short;
	assert(mesure_success_n == 0);
LOOP:
	//m_cRun = TRUE;
	//分别获取DataPane和DataSum中的CListCtrl
	if (mesure_success_n == 0)
	{
		dataSumList.DeleteColumn(1);
		dataSumList.InsertColumn(1, "长轴发散角", LVCFMT_CENTER, 150);
		dataSumList.DeleteColumn(2);
		dataSumList.InsertColumn(2, "短轴发散角", LVCFMT_CENTER, 150);
		for (size_t i = 0; i < 10; i++)
		{
			data_long[i] = 0;
			data_long[i] = 0;
		}
	}

	//为圆形光纤则直接进行准直，楔形光纤则先计算偏轴度再进行准直
	if(!FiberModelFlag)		//楔形光纤
	{
		//先求偏轴度
		vector<cv::Point2d> arr;
		DataGetPoint(arr);
		//cout<<"偏轴度为："<<Partial_degree(arr)<<endl;//这个的double值赋给偏轴度的变量;
		PartialAng = Partial_degree(arr);
		
		if(m_cRun)
		{

			if(Init_collimation())
			{
				AfxMessageBox(_T("准直成功"));
				//m_cRun = FALSE;
				
			}
			else
			{
				AfxMessageBox(_T("准直失败"));
				return false;
				//m_cRun = FALSE;
			}

		}
		else 
		{
			return false;
		}

		vector<double> Xarr;
		vector<double> Yarr;
		DataGetLength(Xarr,Yarr);
		//cout<<"短轴计算长度："<<GetDivAng(Xarr)<<endl;
		//cout<<"长轴计算长度："<<GetDivAng(Yarr)<<endl;//这个的double值赋给那个长度的变量;

		Dist = GetDivAng(Xarr,0.01);
	//	Dist = GetDivAng(Yarr,0.02);
	}
	else		//圆形光纤
	{
		if(m_cRun)
		{

			if(Init_collimation())
			{
				AfxMessageBox(_T("准直成功"));
				//m_cRun = FALSE;
			}
			else
			{
				AfxMessageBox(_T("准直失败"));
				return false;
				//m_cRun = FALSE;
			}

		}
		else
		{
			return false;
		}

		vector<double> Xarr;
		vector<double> Yarr;
		DataGetLength(Xarr,Yarr);
		//cout<<"短轴计算长度："<<GetDivAng(Xarr)<<endl;
		//cout<<"长轴计算长度："<<GetDivAng(Yarr)<<endl;//这个的double值赋给那个长度的变量;

		Dist = GetDivAng(Xarr,0.01);
    	//Dist = GetDivAng(Yarr,0.01);
		Dist = GetDivAngNew1(Xarr,Yarr,0.05,0.1);
	}
	//AfxMessageBox(_T("测量成功"));
	Sleep(1000);
	//准直成功之后暂停
	//flag_start = false;
	
	//从datapane中获取长轴发散角和短轴发散角
	Cdata_long = dataList.GetItemText(6, 1);
	Cdata_short = dataList.GetItemText(5, 1);
	//将第mesure_success_n次的数据存储到数组中
	data_long[mesure_success_n] = (atof(Cdata_long));			
	data_short[mesure_success_n] = (atof(Cdata_short));
	//在datasum中显示长轴发散角和短轴发散角
	dataSumList.SetItemText(mesure_success_n, 1, Cdata_long); //长轴发散角
	dataSumList.SetItemText(mesure_success_n, 2, Cdata_short); //短轴发散角

	if (mesure_success_n < 9) 
	{
		mesure_success_n ++;
		//flag_start = true;
		Sleep(500);
		goto LOOP;
	}
	//求平均值
	assert(mesure_success_n == 9);
	//save_long = atof(dataSumList.GetItemText(1, 1)) + atof(dataSumList.GetItemText(2, 1)) + atof(dataSumList.GetItemText(3, 1)) + atof(dataSumList.GetItemText(4, 1)) + atof(dataSumList.GetItemText(5, 1)) +
	//			atof(dataSumList.GetItemText(6, 1)) + atof(dataSumList.GetItemText(7, 1)) + atof(dataSumList.GetItemText(8, 1)) + atof(dataSumList.GetItemText(9, 1)) + atof(dataSumList.GetItemText(0, 1));
	//save_short = atof(dataSumList.GetItemText(1, 2)) + atof(dataSumList.GetItemText(2, 2)) + atof(dataSumList.GetItemText(3, 2)) + atof(dataSumList.GetItemText(4, 2)) + atof(dataSumList.GetItemText(5, 2)) +
	//			atof(dataSumList.GetItemText(6, 2)) + atof(dataSumList.GetItemText(7, 2)) + atof(dataSumList.GetItemText(8, 2)) + atof(dataSumList.GetItemText(9, 2)) + atof(dataSumList.GetItemText(0, 2));
	//将数据进行排序处理
	
	std::sort(&data_long[0], &data_long[9]);
	std::sort(&data_short[0], &data_short[9]);
	//去掉最大值和最小值，求取平均值
	save_aver_long = (data_long[1] + data_long[2] + data_long[3] + data_long[4] + data_long[5] + data_long[6] + data_long[7] + data_long[8])/8;
	save_aver_short = (data_short[1] + data_short[2] + data_short[3] + data_short[4] + data_short[5] + data_short[6] + data_short[7] + data_short[8]) / 8;
	//在datasum中显示求得的均值
	CString str;
	str.Format("%.4lf", save_aver_long);
	dataSumList.SetItemText(10, 1, str);
	str.Format("%.4lf", save_aver_short);
	dataSumList.SetItemText(10, 2, str);

	mesure_success_n = 0;
	AfxMessageBox(_T("测量成功"));
	return 0;
}


//准直
//X，Y，Z，rX,rY对应于(1,2,3,4,5)
//flag：=1对应于正向，=0对应于负向；
bool Init_collimation()
{
	double Z0_L = 5;//每次将Z轴进行移动的距离
	double Z0_L_R=2;
	double Z0_C=1;
	bool flag = false;						//Z轴移动方向标记
	int flag_x_y = 0;						//角度&X,&Y移动方向

	std::deque<cv::Point2d> Center_last;
	double R_long = 0;						//圆心与中心的距
	//ifstream m_fout;
	
	double Z_distance = Distance; 
	double L_short = LhalfW;				//短轴长度
	//double L_long  = 0;					//长轴长度
	
	//读取当前中心点坐标
	cv::Point2d Center_f = In_Center_Point(g_Px,g_Py);
	
	//将中心移动到指定位置附近，如果当前中心点坐标相比于设置的原点坐标x,y轴的距离大于300，则调整光斑位置
	if(abs(Center_f.x-Center_0.x)>300 || abs(Center_f.y-Center_0.y)>300)
	{
		int dataX[3] = {0,0,0};
		int data_X = (int)abs((Center_f.x-Center_0.x)*5.5);//um
		int dataY[3] = {0,0,0};
		int data_Y = (int)abs((Center_f.y-Center_0.y)*5.5);//um

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

		if(trans(AXISx,FORWORD,(Center_f.x-Center_0.x),dataX)&&trans(AXISy,FORWORD,(Center_f.y-Center_0.y),dataY))
		{
			cv::Point2d P_center_now0=In_Center_Point(g_Px,g_Py);;
			AfxMessageBox(_T("向设定中心靠近完成"));
		}
		else
		{
			AfxMessageBox("向设定中心靠近出错");
		}
	}
	else
	{
		//AfxMessageBox("不用向中心靠近，此时圆心为:（)");
		//cout<<"不用向中心靠近，此时圆心为:（"<<Center_f.x<<","<<Center_f.y<<")"<<endl;
		;//Center_last.push_back(Center_f);
	}

	//判断短轴长度是否在120--130之间，否则调整距离
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,NEGITIVE,Z0_C,data);//反复向后移动		
			Sleep(1000);//延时
			
			//获取当前时刻的短轴长度
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,FORWORD,Z0_C,data);//反复向后移动		
			Sleep(1000);

			//获取当前时刻的短轴长度
			L_short = LhalfW;
		}
	}

	//读取并存储两个中心坐标
	while(Center_last.size()<2)
	{
		if(Center_last.empty())
		{
			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			Center_last.push_back(Center_now);
		}
		else 
		{
			//flag = false时向后移动
			flag = true;				//正向移动
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_L_R*1000);//(um)
			data_Z = data_Z/10;
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			if(!trans(AXISz,FORWORD,Z0_L,data))
			{
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance+=Z0_L;

			Sleep(1000);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			Center_last.push_back(Center_now);
		}
	}

	
	if(Center_last.size()!=2)
	{
		AfxMessageBox("初次调节时数组出错");
	}

	//进入循环调节环节
	//AfxMessageBox("进入循环调节");
	int right_num = 0;
	while(right_num<5)
	{	
		if(Center_last.size()!=2)
		{
			AfxMessageBox("数组出错");
			return false;
		}
		
		//AfxMessageBox("Z轴移动一次，记录数据");
		if(flag == false)		//flag = false时向后移动
		{
			flag = true;
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_L_R*1000);//(um)
			data_Z = data_Z/10;
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(AXISz,FORWORD,Z0_L_R,data))
			{
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance+=Z0_L_R;
			Sleep(100);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			//储存当前中心坐标
			Center_last.push_back(Center_now);
			Center_last.pop_front();
		}
		else				//flag = true时向前移动
		{
			flag = false;

			int data[3] = {0,0,0};
			int data_Z = (int)Z0_L_R*1000;//(um)
			data_Z = abs(data_Z/10);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;

			if(!trans(AXISz,NEGITIVE,Z0_L_R,data))
			{
				//cout<<"Z轴移动出错";
				AfxMessageBox("Z轴移动出错");
				return false;
			}
			Z_distance-=Z0_L_R;
			Sleep(100);
			cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
			
			Center_last.push_back(Center_now);
			Center_last.pop_front();

		}
		//判断两次移动的过程中光斑中心的位置是否距离改变值小于1
		if(abs(Center_last[1].x-Center_last[0].x)<1 && abs(Center_last[1].y-Center_last[0].y)<1 )
		{
			//AfxMessageBox("Z轴移动之后中心不变");
			++right_num;
		}
		else
		{
			right_num = 0;
			//计算出偏转角度，移动角位置台
			cv::Point2d p1 = Center_last[0];
			cv::Point2d p2 = Center_last[1];

			CString xs1[4];
			int datatatat[4]={0,0,0,0};
			datatatat[0]=(int)p1.x;
			datatatat[1]=(int)p1.y;
			datatatat[2]=(int)p2.x;
			datatatat[3]=(int)p2.y;
			xs1[0].Format(_T("%d"),datatatat[0]);
			xs1[1].Format(_T("%d"),datatatat[1]);
			xs1[2].Format(_T("%d"),datatatat[2]);
			xs1[3].Format(_T("%d"),datatatat[3]);
		
			double X_long  = p2.x-p1.x;//X变化
			double Y_long  = p2.y-p1.y;//Y变化
			double X_add_now = X_long*Z_distance/Z0_L;
			double Y_add_now = Y_long*Z_distance/Z0_L;
			double angel = 3.1415926;
			double X_R = atan(X_long*5.5/1000/Z0_L);
			double Y_R = atan(Y_long*5.5/1000/Z0_L);

			int dataX[3] = {0,0,0};
			int data_X = (int)abs(X_R/angel*180*3600);
			int dataY[3] = {0,0,0};
			int data_Y = (int)abs(Y_R/angel*180*3600);

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
			//	AfxMessageBox("向后移动：");
			//		AfxMessageBox("向后移动："+xs1[0]+","+xs1[1]+"-----"+xs1[2]+","+xs1[3]);
				if(trans(AXISxangle,NEGITIVE,X_add_now,dataX)&&trans(AXISyangle,NEGITIVE,Y_add_now,dataY))
				{
					//cout<<"角度校准成功，进入下一次校准"<<endl;
			//		AfxMessageBox("角度校准成功，进入下一次校准");
					cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
					//cout<<"读入当前坐标值：";
			//		AfxMessageBox("读入当前坐标值：");
				//	Center_now = In_Center_Point(g_Px,g_Py);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/
					if(close_to_ceter0(Center_now,Center_last))
					{
						Center_now = In_Center_Point(g_Px,g_Py);
						Center_last.push_back(Center_now);
						Center_last.pop_front();
						//cout<<"进行下一次调节"<<endl;
			//			AfxMessageBox("进行下一次调节");
					}
					else
					{
						//cout<<"靠近中心失败"<<endl;
						AfxMessageBox("靠近中心失败");
					}
					Sleep(1000);
					Center_now = In_Center_Point(g_Px,g_Py);
					Center_last.push_back(Center_now);
					Center_last.pop_front();
				}
				else
					//cout<<"角度校准失败"<<endl;
					AfxMessageBox("角度校准失败");

			}
			else
			{
				//cout<<"向前移动："<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
		//		AfxMessageBox("向前移动：");
			//	AfxMessageBox("向前移动："+xs1[0]+","+xs1[1]+"-----"+xs1[2]+","+xs1[3]);
				if(trans(AXISxangle,FORWORD,X_add_now,dataX)&&trans(AXISyangle,FORWORD,Y_add_now,dataY))
				{
					//cout<<"角度校准成功，进入下一次校准"<<endl;
		//			AfxMessageBox("角度校准成功，进入下一次校准");
					cv::Point2d Center_now=In_Center_Point(g_Px,g_Py);
					//cout<<"读入当前坐标值：";
		//			AfxMessageBox("读入当前坐标值：");
					//Center_now = In_Center_Point(g_Px,g_Py);
					/*Center_now.x = m_Ellipse.xc;
					Center_now.y = m_Ellipse.yc;*/

					if(close_to_ceter0(Center_now,Center_last))
					{
						Center_now = In_Center_Point(g_Px,g_Py);
						Center_last.push_back(Center_now);
						Center_last.pop_front();
						//cout<<"进行下一次调节"<<endl;
		//				AfxMessageBox("进行下一次调节");
					}
					else
					{
						//cout<<"靠近中心失败"<<endl;
						AfxMessageBox("靠近中心失败");
					}
					Sleep(1000);
					Center_now = In_Center_Point(g_Px,g_Py);
					Center_last.push_back(Center_now);
					Center_last.pop_front();
				}
				else
					//cout<<"角度校准失败"<<endl;
					AfxMessageBox("角度校准失败");
			}

		}
	}

	return true;
}
/*
函数名	：trans
功能		：与下位机通信
参数		：
		X_or_Y_or_Z	: 1代表平移台的x轴；2代表平移台的y轴；3代表平移台的z轴；4代表角位台的x角度；5代表角位台的y角度
		up_down_flag: 1代表正向移动；0代表负向移动
		distance	: 移动的距离
		BYTE_data	: 发送到下位机的数据

*/
bool trans(AXISn X_or_Y_or_Z,DIRECTION up_down_flag,double distance,int* BYTE_data)
{
	if(BYTE_data[0]==0&&BYTE_data[1]==0&&BYTE_data[2]<10)
		return true;

	switch(X_or_Y_or_Z)
	{
	case AXISx:
		{
			if(up_down_flag==FORWORD)
			{
				//cout<<"将X向正向移动："<<distance<<endl;//X轴表示02，光斑X坐标变大：01
				//AfxMessageBox(_T("将X向正向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 10){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}
						
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 10){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
			else if(up_down_flag==NEGITIVE)
			{
				//cout<<"将X向负向移动："<<distance<<endl;
				//AfxMessageBox(_T("将X向负向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 10){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 10){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("X请输入正确的方向"));
				return false;
			}
			break;

		}
	case AXISy:
		{
			if(up_down_flag==FORWORD)
			{
				//cout<<"将Y向正向移动："<<distance<<endl;//Y轴表示03，光斑Y坐标变大：00
				//AfxMessageBox(_T("将Y向正向移动："));
				if(distance>0)
				{
					//cout<<"01 03 00 "<<hex<<BYTE_data[0]<<" "<<hex<<BYTE_data[1]<<" "<<hex<<BYTE_data[2]<<endl;
					CString xs[3];
					for(int i=0;i<3;++i)
						xs[i].Format(_T("%d"),BYTE_data[i]);				
					//AfxMessageBox("01 03 00"+xs[0]+xs[1]+xs[2]);
					
					int SData[6];
					SData[0] = 01;
					SData[1] = 03;
					SData[2] = 01;
					SData[3] = BYTE_data[0];
					SData[4] = BYTE_data[1];
					SData[5] = BYTE_data[2];
					((CMainFrame *)AfxGetMainWnd())->onSendMessage(SData);
					//AfxMessageBox(_T("010300_1"));
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
			else if(up_down_flag==NEGITIVE)
			{
				//cout<<"将Y向负向移动："<<distance<<endl;
				//AfxMessageBox(_T("将Y向负向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("Y请输入正确的方向"));
				return false;
			}
			break;
		}
	case AXISz:
		{
			if(up_down_flag==FORWORD)
			{
				//cout<<"将Z向正向移动："<<distance<<endl;//X轴表示01，光斑半径R变大：00
				//AfxMessageBox(_T("将Z向正向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
			else if(up_down_flag==NEGITIVE)
			{
				//cout<<"将Z向负向移动："<<distance<<endl;//靠近相机
				//AfxMessageBox(_T("将Z向负向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
				//cout<<"请输入正确的方向"<<endl;
				AfxMessageBox(_T("Z请输入正确的方向"));
				return false;
			}
			break;
		}
	case AXISxangle:
		{
			if(up_down_flag==FORWORD)
			{
				//cout<<"将&X角度向正向移动："<<distance<<endl;//&X轴表示04，光斑X坐标变大：01
				//AfxMessageBox(_T("将&X角度向正向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
			else if(up_down_flag==NEGITIVE)
			{
				//cout<<"将&X角度负向移动："<<distance<<endl;
			//	AfxMessageBox(_T("将&X角度负向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
				//cout<<"请输入正确的方向"<<endl;
		//		AfxMessageBox(_T("&X请输入正确的方向"));
				return false;
			}
			break;
		}
	case AXISyangle:
		{
			if(up_down_flag==FORWORD)
			{
				//cout<<"将&Y角度向正向移动："<<distance<<endl;//&Y轴表示05，光斑Y坐标变大：00
	//			AfxMessageBox(_T("将&Y角度向正向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
			else if(up_down_flag==NEGITIVE)
			{
				//cout<<"将&Y角度负向移动："<<distance<<endl;
			//	AfxMessageBox(_T("将&Y角度负向移动："));
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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
					//获取当前时间
					long begintime,endtime;
					clock_t t = clock();
					endtime = begintime = t/CLOCKS_PER_SEC;
					while(!next_ok)
					{
						t = clock();
						endtime = t/CLOCKS_PER_SEC;
						if(endtime - begintime > 5){
							AfxMessageBox(_T("下位机无响应，将退出发送程序，发送失败！！"));
							return false;
						}

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
		int data_X = (int)abs(X_distace0*5.5);//um
		int dataY[3] = {0,0,0};
		int data_Y = (int)abs(Y_distace0*5.5);//um

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

		if(trans(AXISx,FORWORD,X_distace0,dataX)&&trans(AXISy,FORWORD,Y_distace0,dataY))
		{
			cv::Point2d P_ceter_now;
			P_ceter_now.x = P_now.x-X_distace0;
			P_ceter_now.y = P_now.y-Y_distace0;
			//X_distace0 = Center_last[1].x-Center_0.x;
			// Y_distace0 = Center_last[1].y-Center_0.y;
			//cout<<"向中心靠近后中心变为:（"<<P_ceter_now.x<<","<<P_ceter_now.y<<")"<<endl;
	//		AfxMessageBox(_T("向中心靠近后中心变为:"));
			
					cv::Point2d P_ceter_now1=In_Center_Point(g_Px,g_Py);
					Center_last.push_back(P_ceter_now1);
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
	//	AfxMessageBox(_T("不用向中心靠近，此时圆心为:"));
		
					
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
			P.x = xc;
			P.y = yc;
			return P;
		}
	}
	cv::Point2d P;
	P.x = xc;
	P.y = yc;
	return P;
}

//偏轴度计算
double Partial_degree(vector<cv::Point2d> arr)
{
	vector<double> ResultValue;
	vector<int> ResultNum;
	double Thre=0.02;
	double pi=3.1415926;
	double AddL=0.5;

	for(int i=0;i<(int) arr.size();++i)
	{
		for(int j=i+1;j<(int)arr.size();++j)
		{
			double SquareDiff=sqrt((arr[j].x-arr[i].x)*5.5*5.5*(arr[j].x-arr[i].x)+(arr[j].y-arr[i].y)*5.5*5.5*(arr[j].y-arr[i].y));
			double ang=(atan(SquareDiff/1000/(j-i)/AddL))/pi*180;
			int k;
			for( k=0;k<(int)ResultValue.size();++k)
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
	for(unsigned int i=0;i<ResultNum.size();++i)
	{
		if(ResultNum[i]>maxnum)
		{
			maxnum=ResultNum[i];
			result=ResultValue[i];
		}
	}

	return result;
}

//Z轴长度（发散角）计算
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
		for(int i=0;i<(int)arr.size();++i)
		{
			double ang=2*(atan(arr[i]*5.5/1000/(Length+AddL*i)))/pi*180;
			int j;
			for(j=0;j<(int)ResultValue1.size();++j)
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
	double LeftLen=Length;
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
//数据采集
bool DataGetLength(vector<double> &Xarr,vector<double> &Yarr)
{
	//double LhalfW;
	//cout<<"请输入短轴长度：";
	//cin>>LhalfW;
	double L_short = LhalfW;//短轴长度
	double Z0_L = 5;//每次将Z轴进行移动的距离（单位：mm）
	double Z0_C=1;
	double AddL=0.5;

	//调整光斑距离
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,NEGITIVE,Z0_C,data);//反复向前移动		
			Sleep(1000);//延时
			//cout<<"请输入短轴长度：";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,FORWORD,Z0_C,data);//反复向后移动		
			Sleep(1000);
			//cout<<"请输入短轴长度：";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
	}

	//cout<<"请输入光斑大小：";
	//正式采集数据
	int Num=10;//采集数量
	for(int i=0;i<Num;++i)
	{

		double XL;
		//cout<<"短轴：";
		//cin>>XL;
		XL = LhalfW;
		//	cout<<"长轴：";
		double YL;
		//cin>>YL;
		YL = halfW;

		Xarr.push_back(XL);
		Yarr.push_back(YL);
		if(i==Num-1)
			break;
		//向后移动采集
		int data[3] = {0,0,0};
		int data_Z = (int)abs(AddL*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(AXISz,FORWORD,AddL,data);//反复向后移动	
		Sleep(1000);
	}
	//回到原处
	int data[3] = {0,0,0};
	int data_Z = (int)abs(AddL*(Num-1)*100);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	trans(AXISz,NEGITIVE,AddL*(Num-1),data);//反复向后移动	
	return true;
}
bool DataGetPoint(vector<cv::Point2d> &arr)
{
	//double LhalfW;
	//cout<<"请输入短轴长度：";
	//cin>>LhalfW;
	double L_short = LhalfW;//短轴长度
	double Z0_L = 5;//每次将Z轴进行移动的距离（单位：mm）
	double Z0_C=1;
	double AddL=0.5;

	//调整光斑距离
	while(L_short>130 || L_short<120)
	{
		if(L_short>130)
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(AddL*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,NEGITIVE,AddL,data);//反复向前移动		
			Sleep(1000);//延时
			//cout<<"请输入短轴长度：";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
		else
		{
			int data[3] = {0,0,0};
			int data_Z = (int)abs(Z0_C*100);
			data[2] = data_Z%10;
			data_Z = data_Z/10;
			data[1] = data_Z%10;
			data_Z = data_Z/10;
			data[0] = data_Z;
			trans(AXISz,FORWORD,Z0_C,data);//反复向后移动		
			Sleep(1000);
			//cout<<"请输入短轴长度：";
			//cin>>LhalfW;
			L_short = LhalfW;
		}
	}

	//正式采集数据
	int Num=10;//采集数量
	for(int i=0;i<Num;++i)
	{
		//cout<<"请输入中心坐标：";
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
		//向后移动采集
		int data[3] = {0,0,0};
		int data_Z = (int)abs(AddL*100);
		data[2] = data_Z%10;
		data_Z = data_Z/10;
		data[1] = data_Z%10;
		data_Z = data_Z/10;
		data[0] = data_Z;
		trans(AXISz,FORWORD,AddL,data);//反复向后移动	
		Sleep(1000);
	}
	//回到原处
	int data[3] = {0,0,0};
	int data_Z = (int)abs(AddL*(Num-1)*100);
	data[2] = data_Z%10;
	data_Z = data_Z/10;
	data[1] = data_Z%10;
	data_Z = data_Z/10;
	data[0] = data_Z;
	trans(AXISz,NEGITIVE,AddL*(Num-1),data);//反复向后移动	
	return true;
}