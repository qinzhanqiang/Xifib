#include "stdafx.h"
#include <windowsx.h>
#include <conio.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <mmsystem.h>
#include "LMGaussFit.h"
#include "XiFib.h"
#include "XiPlayPane.h"
#include "acquisition.h"
#include "MainFrm.h"
#include "CEllipseFit.h"
#include "LMGaussFit.h"


//OVERLAPPED Eol = {0};
//OVERLAPPED Wol = {0};	//写操作结构变量
//OVERLAPPED Rol = {0};//读操作结构变量


extern int WM_UPDATE_DIALOG;
extern int WM_UPDATE_IMG_SAVE_DIALOG;

static	volatile BOOL	bHalted;

static SETUP	setup;  //全局设置
LPSETUP lps = &setup;
//临界区用于线程同步
CRITICAL_SECTION C_lock;   

#define MAX_DOWNSAMPLING_ITEMS 12
//保存文件名
static char save_image_filename[MAX_PATH]="";  
static dsList DSL[MAX_DOWNSAMPLING_ITEMS];
pdsList pDSL = &DSL[0];

DWORD WINAPI AcquisitionProc( LPVOID lpv );

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



void	setT0_acq(void){
	lps->pcn0_acq = timeGetTime();
}
double	getT_acq(void){
	lps->pcn_acq = timeGetTime();  
	//获取系统时间，函数返回以毫秒计的系统时间
	//该时间为从系统开启算起所经过的时间
	return (double)(lps->pcn_acq-lps->pcn0_acq) / 1000;
}

void	setT0_disp(void){
	lps->pcn0_disp = timeGetTime();
}
double	getT_disp(void){
	lps->pcn_disp = timeGetTime();
	return (double)(lps->pcn_disp-lps->pcn0_disp) / 1000;
}

#define MAX_FILENAME 300

static bool flag_request_save_image=false;

bool flag_start=false;
int saveImageBMP(char* filename, XI_IMG* image);

static int image_cycle_sleep_ms=1;

/*--------------------------------------------------------------*/

void Acquisition_Init()
{
	printf("Acquisition_Init\n");
	memset(&setup,0,sizeof(setup));
	for (int i=0;i<sizeof(DSL)/sizeof(dsList);i++)
	{
		DSL[i].dsVal=0;
	}
}

/*--------------------------------------------------------------*/
BOOL shStartLive( HANDLE hMV, XiPlayPane *childv )
{

BOOL	ret = FALSE;
HCURSOR hoc;
DWORD	ThreadId;

	lps->bHaveData = FALSE;

	if (hMV == INVALID_HANDLE_VALUE)
		return FALSE;

	hoc = SetCursor( LoadCursor( NULL, IDC_WAIT ) ); //该函数确定光标的形状。
	
	if (lps->bRun) goto ex;
	
	bHalted = FALSE;

	//将对话框的指针赋给相机设置结构体中的对话框指针
	lps->childv = childv;  
	//初始化临界区
	InitializeCriticalSection(&C_lock);

	lps->hShadingThread = CreateThread( NULL, 0, AcquisitionProc, lps, 0, &ThreadId);
	//线程安全属性、堆栈大小、线程函数、线程参数、线程创建属性、线程ID
	if (lps->hShadingThread==NULL) goto ex;

	setT0_acq();
	setT0_disp();

	while(!lps->bRun && !bHalted) 
		Sleep(5);
	ret = lps->bRun;

ex:
	SetCursor(hoc);
	return ret;
}

/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
BOOL shIsLive()
{
	return lps->bRun && (lps->hShadingThread!=NULL);
}

void shSlowDownAcquisition(int image_sleep_ms)  //acquisition 获得，收获
{
	image_cycle_sleep_ms=image_sleep_ms;
}

/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
VOID shStopLive( HANDLE hMV )
{
	HCURSOR hoc;

	hoc = SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	if (!lps->bRun || lps->hShadingThread==NULL) return;
	lps->bRun = FALSE;
	lps->bHaveData = FALSE;
	
	//暂停、停止线程
	while (WaitForSingleObject( lps->hShadingThread, 100 ) == WAIT_TIMEOUT) {
	MSG	msg;
		while (PeekMessage( &msg,0,0,0,PM_NOREMOVE)) {
			if (msg.message == WM_QUIT) break;
			PeekMessage( &msg,0,0,0,PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CloseHandle(lps->hShadingThread);
	lps->hShadingThread = NULL;

	SetCursor(hoc);
	return;
}

#define LUTEN 0

#define CheckxiError(ret,msg,value) if (XI_OK != mvret) {	printf("ERROR: %s returns error: %d value %d. Retrying to open.\n", msg, mvret, value); xiCloseDevice(hMVlocal); hMVlocal = INVALID_HANDLE_VALUE; Sleep(200); }

#define SETUP_DLG lps->childv->sdlg 

DWORD WINAPI AcquisitionProc( LPVOID lpv )
{
	LPSETUP lps = (LPSETUP)lpv;
	HANDLE	hMVlocal = INVALID_HANDLE_VALUE;
	XI_RETURN	mvret;
	int iPrevFrame = -1;
	lps->bRun = TRUE; 
	lps->img.size = sizeof(XI_IMG);

	//最大值
	int max=0;
	int Vpoint=0;
	//中心
	CvPoint Epoint;
	//计算数据保存

	//找到的椭圆点
	std::vector<int> m_XVec;
	std::vector<int> m_YVec;
	//短轴上的点
	std::vector<double> m_GXVec;
	std::vector<double> m_GYVec;
	//长轴上的点
	std::vector<double> m_LGXVec;
	std::vector<double> m_LGYVec;
	CEllipseFit LSellipse;

	cv::Mat l = cv::imread("F:\\mycpp\\XiFib\\Debug\\1.bmp",0);
	CEllipse &m_Ellipse = lps->ShowWin->m_wndEllipse.Epse;
	CGaussLine &m_LGaussLine = lps->ShowWin->m_wndGauss.gaussLine;
	CGaussLine &m_GaussLine = lps->ShowWin->m_wndGaussL.gaussLine;
	CListCtrl &dataList = lps->ShowWin->m_wndData.dataList;

	//IplImage* ipl=NULL;
	IplImage* image = cvCreateImage(cvSize( 2048, 2048), IPL_DEPTH_8U, 1); //将XI_IMG转化为IplImage
	cv::Mat tempMat;
	cv::Mat src;
	
	lps->uiExposure = 600;
	lps->bAEAG = FALSE;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG, 0);
	lps->fGain = 0;

	SETUP_DLG->m_ExposureSlider.EnableWindow(TRUE);
	SETUP_DLG->m_GainSlider.EnableWindow(TRUE);
	
	//float fl = 0;
	xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN XI_PRM_INFO_MIN, &lps->fGain);
    xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN, lps->fGain);
	
	if (lps->childv!=NULL)
	{
		if(SETUP_DLG) ::PostMessage(SETUP_DLG->m_hWnd, WM_UPDATE_DIALOG, 0, 0);
	}

	while (lps->bRun) 
	{
		if (hMVlocal == INVALID_HANDLE_VALUE) //非法的、无效的
		{
			printf("AcquisitionProc open device DSid:%d\n",lps->uiDS);
			if (lps->uiDS>=MAX_DOWNSAMPLING_ITEMS)
			{
				printf("AcquisitionProc: error Parameters are not ready\n");
				Sleep(100);
				continue;
			}

			if (xiOpenDevice( lps->devIndex, &hMVlocal ) != XI_OK ) 
			{
				lps->bRun = FALSE;
				break;
			}
			if(hMVlocal == INVALID_HANDLE_VALUE){
				lps->bRun = FALSE;
				break;
			}
			
			mvret = xiSetParamInt(hMVlocal, XI_PRM_DOWNSAMPLING, DSL[lps->uiDS].dsVal);//DSL结构体数组
			CheckxiError(mvret,"XI_PRM_DOWNSAMPLING",DSL[lps->uiDS].dsVal);   //设置采样速率
			
			mvret = xiSetParamInt(hMVlocal, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
			CheckxiError(mvret,"XI_PRM_IMAGE_DATA_FORMAT",lps->frm);         //设置图像格式
	
			mvret = xiSetParamInt(hMVlocal, XI_PRM_EXPOSURE, lps->uiExposure);
			CheckxiError(mvret,"XI_PRM_EXPOSURE",lps->uiExposure);      //设置曝光时间，单位为us

			mvret = xiSetParamInt(hMVlocal, XI_PRM_WIDTH, lps->dwROIcx);
			CheckxiError(mvret,"XI_PRM_WIDTH",lps->dwROIcx);

            //就是在要处理的图像中提取出的要处理的区域。
			mvret = xiSetParamInt(hMVlocal, XI_PRM_HEIGHT, lps->dwROIcy);
			CheckxiError(mvret,"XI_PRM_HEIGHT",lps->dwROIcy);

			mvret = xiSetParamInt(hMVlocal, XI_PRM_OFFSET_X, lps->dwROIx0);
			CheckxiError(mvret,"XI_PRM_OFFSET_X",lps->dwROIx0);

			mvret = xiSetParamInt(hMVlocal, XI_PRM_OFFSET_Y, lps->dwROIy0);
			CheckxiError(mvret,"XI_PRM_OFFSET_Y",lps->dwROIy0);
			//开始接收
			mvret = xiStartAcquisition(hMVlocal);
			CheckxiError(mvret,"xiStartAcquisition",0);
		}
		if ( hMVlocal != INVALID_HANDLE_VALUE ) 
		{
			while ( lps->bRun && hMVlocal != INVALID_HANDLE_VALUE) 
			{
				//最大延时时间为1s
				#define IMAGE_TIMEOUT_SAFE_MARGIN_MS 1000    // cover all readouts 读出器，读出的东西
				// 计算延时时间
				int image_timeout_ms=IMAGE_TIMEOUT_SAFE_MARGIN_MS;   //margin 边缘、极限
				image_timeout_ms += lps->uiExposure / 1000; // add exposure time
				if (lps->fUserFrameRateFPS>0)
				{
					// add selected framerate (especially important for low framerates)
					image_timeout_ms += (int)(1000/lps->fUserFrameRateFPS);
				}
//自动曝光
//****************************************************************		
				if (flag_start)
				{

					if (max==0)
					{
						lps->uiExposure+=50;
					}
					else if (max>205||max<195)
					{
						UINT expos = 200*lps->uiExposure/max;
						int ltime = (lps->uiExposure) - expos;

						if (ltime>10||ltime<(-10))
						{
							lps->uiExposure = expos;
						}
						else if (ltime>0)
						{
							lps->uiExposure -=1;
						}
						else
							lps->uiExposure +=1;

						//lps->uiExposure = expos;

						if (lps->uiExposure > 10000)
						{
							lps->uiExposure=10000;
						}
						else if (lps->uiExposure<5)
						{
							lps->uiExposure=5;
						}
						//xiSetParamInt(hMVlocal);
						if (lps->bAEAG == TRUE)
						{
							xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG, 0);
						}
						xiSetParamInt(hMVlocal, XI_PRM_EXPOSURE, lps->uiExposure);
					}
				}
//**************************************************************************

				//进入临界区
				EnterCriticalSection(&C_lock);
				mvret = xiGetImage( hMVlocal, image_timeout_ms, &lps->img );
				LeaveCriticalSection(&C_lock);
				//离开临界区

				if(!strcmp(lps->devType,"CURRERA"))
					Sleep(image_cycle_sleep_ms);   
				//执行挂起一段时间
				//Sleep()单位为毫秒，sleep()单位为秒（如果需要更精确可以用usleep单位为微秒）
				
				if(mvret == XI_TIMEOUT)
				{
					continue;
				}
				else if(mvret != XI_OK)
				{
					lps->bRun = FALSE;
					break;
				}

				CheckxiError(mvret,"xiGetImage",0);
				if ( mvret == XI_OK ) break;	
			}
			lps->dwNframes++;
			iPrevFrame = lps->img.nframe;
		}

		lps->bDataLocked = FALSE;
		//更新对话框
		if (lps->childv!=NULL)
		{
			if(SETUP_DLG) ::PostMessage(SETUP_DLG->m_hWnd, WM_UPDATE_DIALOG, 0, 0);
		}
 		
		if (!lps->bRun) break;

		switch(lps->img.frm)
		{
		case XI_MONO8 :
		case XI_RAW8  :
			lps->bpp = 1;
			break;
		case XI_MONO16 :
		case XI_RAW16  :
			lps->bpp = 2;
			break;
		case XI_RGB24 :
		case XI_RGB_PLANAR  :
			lps->bpp = 3;
			break;
		case XI_RGB32 :
			lps->bpp = 4;
			break;
		default:
			lps->bpp = 1;
			break;
		}

		lps->bHaveData = TRUE;
	
		if (flag_request_save_image)
		{
			int size=0;
			XI_IMG image = lps->img;
			switch(image.frm)
			{
			case XI_MONO8 :
			case XI_RAW8  :
				size = (image.width + image.padding_x) * image.height;
				break;
			case XI_MONO16 :
			case XI_RAW16  :
				size = (image.width + image.padding_x / 2) * image.height * 2;
				break;
			case XI_RGB24 :
			case XI_RGB_PLANAR  :
				size = (image.width + image.padding_x / 3) * image.height * 3;
				break;
			case XI_RGB32 :
				size = (image.width + image.padding_x / 4) * image.height * 4;
				break;
			default:
				size = (image.width + image.padding_x) * image.height;
				break;
			}
			image.bp = malloc(size);
			memcpy(image.bp, lps->img.bp, size);
			saveImageBMP(save_image_filename, &image);
			flag_request_save_image=false;
			free(image.bp);
		}
/**********************************************/
		if (flag_start)
		{
			//创建mat对象
			//rows 是 行数 相当于 height 对应 y
			//cols 是 列数 相当于 width 对应 x
			
			if (m_XVec.size()>0)
			{
				m_XVec.clear();
			}

			if (m_YVec.size()>0)
			{
				m_YVec.clear();
			}
			if (m_GXVec.size()>0)
			{
				//短轴上的点
				m_GXVec.clear();
			}
			if (m_GYVec.size()>0)
			{
              m_GYVec.clear();
			}
			if (m_LGXVec.size()>0)
			{
                //长轴上的点
				m_LGXVec.clear();
			}	
			if (m_LGYVec.size()>0)
			{
				m_LGYVec.clear();
			}
				
//**********************************************			
			double maxval=0;
			double minval=0;
			cv::Point maxpoint(0,0);
			//double LhalfW=0;
			double halfW=0;
			double dev=0;
			double Ldev=0;
			double Distance = lps->ShowWin->m_Distance;
/*******************************************************/
			//CEllipseFit LSellipse;
			//CEllipse tem_Ellipse;
			//CGaussLine tem_GaussLine;
			//CGaussLine tem_LGaussLine;      
			//设置图像大小及深度
			/*if (lps->img.width != src.cols || lps->img.height!= src.rows || lps->bpp != src.elemSize())
			{
				src.release();
				switch(lps->bpp)
				{
				case 1:
					src.create(lps->imgs.height, lps->imgs.width, CV_8UC1);
					break;
				case 2:
					src.create(lps->imgs.height, lps->imgs.width, CV_16UC1);
					break;
				default:
					src.create(lps->imgs.height, lps->imgs.width, CV_8UC1);
				}	
			}*/
			//内存拷贝
			
			memcpy( image->imageData, lps->img.bp, (lps->img.width)*(lps->img.height)*lps->bpp);
			//cvSaveImage("D:\\saveImage3.jpg",image);
			
			tempMat = cv::cvarrToMat(image);//旧版数据CvMat,IplImage,CvMatND转换到新版数据Mat
		//	tempMat = tempMat - l;
			medianBlur(tempMat,src,3); //第一个为原图，第二个为目标图像
			tempMat.release();

			//std::vector<int> compression_params;
			//compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
			//compression_params.push_back(100);
			//imwrite("D:\\saveImage4.jpg",src,compression_params);
			
			//m_Ellipse.clear();
			//m_GaussLine.clear();
			//m_LGaussLine.clear();

			cv::minMaxLoc(src, &minval, &maxval, NULL, &maxpoint);    //获取最大最小值及其坐标
			max = maxval;

			if (max<50&&lps->uiExposure==10000)
			{
				dataList.SetItemText(10,1, _T("光强太弱，请调整光源"));
			}
			else if (max==255&&lps->uiExposure<2)
			{
				dataList.SetItemText(10,1, _T("光强太强，请调整光源"));
			}
			else
				dataList.SetItemText(10,1, _T("正常"));
			
			findHalf(src, max, m_XVec, m_YVec);//从图片求了 半高点的X、Y的坐标
			
			if( m_XVec.size()>5 && m_YVec.size()>5 &&m_XVec.size()<10000&& max>50)
			{
				LSellipse.getEllipse(m_XVec, m_YVec, m_Ellipse);//求椭圆的中心xc、yc，长xl、短轴yl，偏转角ang，都在CEllipse结构的m_Ellipse中
				if (src.elemSize()==1)
				{
					Vpoint = src.at<unsigned char>(m_Ellipse.yc, m_Ellipse.xc); //中心值
				}
				else{
					Vpoint = src.at<unsigned short>(m_Ellipse.yc, m_Ellipse.xc); //中心值
				}
			}
			dataList.SetItemText(9,1, "--");//偏轴度

			
		
			if (m_Ellipse.xl!=0&&max>50)
			{
				//把长短轴、高斯拟合曲线上的点 和像素大小找出来
				//长轴
				linePointL(src, cv::Point((int)(round(m_Ellipse.xc)), (int)(round(m_Ellipse.yc))), m_Ellipse.ang+1.5707963, m_LGXVec, m_LGYVec);
				//短轴
				linePoint(src, cv::Point((int)(round(m_Ellipse.xc)), (int)(round(m_Ellipse.yc))), m_Ellipse.ang, m_GXVec, m_GYVec);
				//拟合高斯曲线
				//长轴

				//linePoint(src, maxpoint, m_Ellipse.ang, m_GXVec, m_GYVec);
				//短轴
				//linePoint(src, maxpoint, m_Ellipse.ang+1.5707963, m_LGXVec, m_LGYVec);
				//拟合高斯曲线

				gaussfit(m_GXVec, m_GYVec, maxval , m_Ellipse.xl, m_GaussLine);
				gaussfit(m_LGXVec, m_LGYVec, maxval, m_Ellipse.yl, m_LGaussLine);

				if (lps->ShowWin->m_wndEllipse.m_hWnd && lps->bRun)
				{
					UpdateWindow( lps->ShowWin->m_wndEllipse.m_hWnd );
					InvalidateRect( lps->ShowWin->m_wndEllipse.m_hWnd, NULL, TRUE );
				}
				if (lps->ShowWin->m_wndGauss.m_hWnd && lps->bRun)
				{
					UpdateWindow( lps->ShowWin->m_wndGauss.m_hWnd );
					InvalidateRect( lps->ShowWin->m_wndGauss.m_hWnd, NULL, TRUE );
				}
				if (lps->ShowWin->m_wndGaussL.m_hWnd && lps->bRun)
				{
					UpdateWindow( lps->ShowWin->m_wndGaussL.m_hWnd );
					InvalidateRect( lps->ShowWin->m_wndGaussL.m_hWnd, NULL, TRUE );
				}

				//插入光斑的10个数据，在数据列表中
				//pDataList->SetRedraw(FALSE);
				CString lstr;
				lstr.AppendFormat("%d",max); 
				dataList.SetItemText(0,1,lstr); //峰值
				//maxval++;
				//Sleep(15);
				lstr.Format("%.4lf",m_Ellipse.xc);
				dataList.SetItemText(1,1,lstr); //X0

				lstr.Format("%.4lf",m_Ellipse.yc);
				dataList.SetItemText(2,1,lstr); //Y0

				LhalfW = sqrt( log( (double)2) )*m_LGaussLine.Q;   //求高斯曲线的半宽度

				lstr.Format("%.4lf",LhalfW);
				dataList.SetItemText(3,1,lstr);//短轴长度

				halfW = sqrt( log( (double) 2) )*m_GaussLine.Q; 
				//halfW = sqrt( log( m_GaussLine.A*2/(Vpoint-m_GaussLine.H)) )*m_GaussLine.Q; 
				lstr.Format("%.4lf",halfW);	
				dataList.SetItemText(4,1,lstr); //长轴长度

				Ldev = 2*atan(LhalfW*5.5/(Distance*1000))*180/3.1415926;
				lstr.Format("%.4lf",Ldev);	
				dataList.SetItemText(5,1,lstr); //短轴发散角

				dev = 2*atan(halfW*5.5/(Distance*1000))*180/3.1415926;
				lstr.Format("%.4lf",dev);	
				dataList.SetItemText(6,1,lstr); //长轴发散角

				lstr.Format("%.4lf", halfW/LhalfW);
				dataList.SetItemText(7,1,lstr); //长短轴之比

				lstr.Format("%.4lf",m_Ellipse.ang*180/3.1415926);
				dataList.SetItemText(8,1,lstr); //长轴角度
				//pDataList->SetRedraw(TRUE);*/
			}
			src.release();
			//如果int k==1----准直1，k=0；
			//delay(1000000);
			/********************准直**********************/
	//		double Z_distance = 0;
	//		double Z0_L = 2;//每次将Z轴进行移动的距离
	//		bool flag = false;//Z轴移动方向标记
	//		int flag_x_y = 0;//角度&X,&Y移动方向

	//		std::deque<cv::Point2d> Center_last;
	//		double R_long = 0;//圆心与中心的距离
	//		/*double Z_distance = 0;
	//		cout<<"请输入Z轴初始距离：";
	//		cin>>Z_distance; */
	//		Z_distance = Distance; //输入Z轴初始距离
	//		double L_short = LhalfW;//短轴长度
	//		double L_long  = halfW;//长轴长度
	//		while(L_short>120)
	//		{
	//			/*cout<<"请输入短轴长度：";
	//			cin>>L_short;*/
	//			L_short = LhalfW;
	//			int data[3] = {0,0,0};
	//			int data_Z = abs(Z0_L*100);
	//			data[2] = data_Z%10;
	//			data_Z = data_Z/10;
	//			data[1] = data_Z%10;
	//			data_Z = data_Z/10;
	//			data[0] = data_Z;
	//			trans(3,0,Z0_L,data);//反复向前移动
	//		}
	////
	//		cv::Point2d Center_f ;//= In_Center_Point();
	//		Center_f.x = m_Ellipse.xc;
	//		Center_f.y = m_Ellipse.yc;
	//		if(abs(Center_f.x-Center_0.x)>300 || abs(Center_f.y-Center_0.y)>300)
	//		{
	//			int dataX[3] = {0,0,0};
	//			int data_X = abs((Center_f.x-Center_0.x)*5.5);//um
	//			int dataY[3] = {0,0,0};
	//			int data_Y = abs((Center_f.y-Center_0.y)*5.5);//um

	//			data_X = data_X/10;
	//			dataX[2] = data_X%10;
	//			data_X = data_X/10;
	//			dataX[1] = data_X%10;
	//			data_X = data_X/10;
	//			dataX[0] = data_X;

	//			data_Y = data_Y/10;
	//			dataY[2] = data_Y%10;
	//			data_Y = data_Y/10;
	//			dataY[1] = data_Y%10;
	//			data_Y = data_Y/10;
	//			dataY[0] = data_Y;

	//			if(trans(1,1,(Center_f.x-Center_0.x),dataX)&&trans(2,1,(Center_f.y-Center_0.y),dataY))
	//			{


	//				//X_distace0 = Center_last[1].x-Center_0.x;
	//				// Y_distace0 = Center_last[1].y-Center_0.y;
	//				//cout<<"向中心靠近后中心变为:（"<<Center_0.x<<","<<Center_0.y<<")"<<endl;
	//				AfxMessageBox(_T("向中心靠近后中心变为:"));
	//			}
	//			else
	//			{
	//				//cout<<"向中心靠近出错";
	//				AfxMessageBox("向中心靠近出错");
	//			}
	//		}
	//		else
	//		{
	//			AfxMessageBox("不用向中心靠近，此时圆心为:（)");
	//			//cout<<"不用向中心靠近，此时圆心为:（"<<Center_f.x<<","<<Center_f.y<<")"<<endl;
	//			Center_last.push_back(Center_f);
	//		}
	//		//
	//		while(Center_last.size()<2)
	//		{
	//			if(Center_last.empty())
	//			{
	//				cv::Point2d Center_now;
	//				//cout<<"读入第一个坐标值：";
	//				AfxMessageBox("读入第一个坐标值：");
	//				//Center_now = In_Center_Point();
	//				Center_now.x = m_Ellipse.xc;
	//				Center_now.y = m_Ellipse.yc;
	//				Center_last.push_back(Center_now);
	//			}
	//			else 
	//			{
	//				//flag = false时向后移动
	//				flag = true;
	//				int data[3] = {0,0,0};
	//				int data_Z = abs(Z0_L*1000);//(um)
	//				data_Z = data_Z/10;
	//				data[2] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[1] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[0] = data_Z;
	//				if(!trans(3,1,Z0_L,data))
	//				{
	//					//cout<<"Z轴移动出错";
	//					AfxMessageBox("Z轴移动出错");
	//					return false;
	//				}
	//				Z_distance+=Z0_L;

	//				cv::Point2d Center_now;
	//				//cout<<"读入第二个坐标值：";
	//				AfxMessageBox("读入第二个坐标值：");
	//				//Center_now = In_Center_Point();
	//				Center_now.x = m_Ellipse.xc;
	//				Center_now.y = m_Ellipse.yc;
	//				Center_last.push_back(Center_now);
	//			}
	//		}


	//		if(Center_last.size()!=2)
	//		{
	//			//cout<<"初次调节时数组出错"<<endl;
	//			AfxMessageBox("初次调节时数组出错");
	//		}
	//		//cout<<"进入循环调节"<<endl;
	//		AfxMessageBox("进入循环调节");
	//		int right_num = 0;
	//		while(right_num<3)
	//		{	
	//			if(Center_last.size()!=2)
	//			{
	//				//cout<<"数组出错"<<endl;
	//				AfxMessageBox("数组出错");
	//				return false;
	//			}
	//			//Z轴移动一次，记录数据
	//			//cout<<"Z轴移动一次，记录数据"<<endl;
	//			AfxMessageBox("Z轴移动一次，记录数据");
	//			if(flag == false)//flag = false时向后移动
	//			{
	//				flag = true;
	//				int data[3] = {0,0,0};
	//				int data_Z = abs(Z0_L*1000);//(um)
	//				data_Z = data_Z/10;
	//				data[2] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[1] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[0] = data_Z;

	//				if(!trans(3,1,Z0_L,data))
	//				{
	//					//cout<<"Z轴移动出错";
	//					AfxMessageBox("Z轴移动出错");
	//					return false;
	//				}
	//				Z_distance+=Z0_L;

	//				cv::Point2d Center_now;
	//				//cout<<"读入当前坐标值:";
	//				AfxMessageBox("读入当前坐标值:");
	//				//Center_now = In_Center_Point();
	//				Center_now.x = m_Ellipse.xc;
	//				Center_now.y = m_Ellipse.yc;
	//				Center_last.push_back(Center_now);
	//				Center_last.pop_front();
	//			}
	//			else
	//			{
	//				flag = false;

	//				int data[3] = {0,0,0};
	//				int data_Z = Z0_L*1000;//(um)
	//				data_Z = abs(data_Z/10);
	//				data[2] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[1] = data_Z%10;
	//				data_Z = data_Z/10;
	//				data[0] = data_Z;

	//				if(!trans(3,0,Z0_L,data))
	//				{
	//					//cout<<"Z轴移动出错";
	//					AfxMessageBox("Z轴移动出错");
	//					return false;
	//				}
	//				Z_distance-=Z0_L;
	//				cv::Point2d Center_now;
	//				//cout<<"读入当前坐标值：";
	//				AfxMessageBox("读入当前坐标值：");
	//				//Center_now = In_Center_Point();
	//				Center_now.x = m_Ellipse.xc;
	//				Center_now.y = m_Ellipse.yc;
	//				Center_last.push_back(Center_now);
	//				Center_last.pop_front();
	//			}
	//			if(abs(Center_last[1].x-Center_last[0].x)<1 && abs(Center_last[1].y-Center_last[0].y)<1 )
	//			{
	//				//cout<<"Z轴移动之后中心不变"<<endl;
	//				AfxMessageBox("Z轴移动之后中心不变");
	//				++right_num;
	//			}
	//			else
	//			{//////
	//				cv::Point2d p1 = Center_last[0];
	//				cv::Point2d p2 = Center_last[1];
	//				double X_long  = p2.x-p1.x;//X变化
	//				double Y_long  = p2.y-p1.y;//Y变化
	//				double X_add_now = X_long*Z_distance/Z0_L;
	//				double Y_add_now = Y_long*Z_distance/Z0_L;
	//				double angel = 3.1415926;
	//				double X_R = atan(X_long*5.5/1000/Z0_L);
	//				double Y_R = atan(Y_long*5.5/1000/Z0_L);

	//				int dataX[3] = {0,0,0};
	//				int data_X = abs(X_R/angel*180*3600);
	//				int dataY[3] = {0,0,0};
	//				int data_Y = abs(Y_R/angel*180*3600);

	//				dataX[2] = data_X%60;
	//				data_X = data_X/60;
	//				dataX[1] = data_X%60;
	//				data_X = data_X/60;
	//				dataX[0] = data_X;

	//				dataY[2] = data_Y%60;
	//				data_Y = data_Y/60;
	//				dataY[1] = data_Y%60;
	//				data_Y = data_Y/60;
	//				dataY[0] = data_Y;

	//				if(flag)//刚刚向后移动过
	//				{	
	//					//cout<<"向后移动："<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
	//					AfxMessageBox("向后移动：");
	//					if(trans(4,0,X_add_now,dataX)&&trans(5,0,Y_add_now,dataY))
	//					{
	//						//cout<<"角度校准成功，进入下一次校准"<<endl;
	//						AfxMessageBox("角度校准成功，进入下一次校准");
	//						cv::Point2d Center_now;
	//						//cout<<"读入当前坐标值：";
	//						AfxMessageBox("读入当前坐标值：");
	//						//Center_now = In_Center_Point();
	//						Center_now.x = m_Ellipse.xc;
	//						Center_now.y = m_Ellipse.yc;
	//						if(close_to_ceter0(Center_now,Center_last))
	//						{
	//							//cout<<"进行下一次调节"<<endl;
	//							AfxMessageBox("进行下一次调节");
	//						}
	//						else
	//						{
	//							//cout<<"靠近中心失败"<<endl;
	//							AfxMessageBox("靠近中心失败");
	//						}
	//					}
	//					else
	//						//cout<<"角度校准失败"<<endl;
	//						AfxMessageBox("角度校准失败");

	//				}
	//				else
	//				{
	//					//cout<<"向前移动："<<"("<<p1.x<<","<<p1.y<<")---("<<p2.x<<","<<p2.y<<")"<<endl;
	//					AfxMessageBox("向前移动：");
	//					if(trans(4,1,X_add_now,dataX)&&trans(5,1,Y_add_now,dataY))
	//					{
	//						//cout<<"角度校准成功，进入下一次校准"<<endl;
	//						AfxMessageBox("角度校准成功，进入下一次校准");
	//						cv::Point2d Center_now;
	//						//cout<<"读入当前坐标值：";
	//						AfxMessageBox("读入当前坐标值：");
	//						//Center_now = In_Center_Point();
	//						Center_now.x = m_Ellipse.xc;
	//						Center_now.y = m_Ellipse.yc;
	//						if(close_to_ceter0(Center_now,Center_last))
	//						{
	//							//cout<<"进行下一次调节"<<endl;
	//							AfxMessageBox("进行下一次调节");
	//						}
	//						else
	//						{
	//							//cout<<"靠近中心失败"<<endl;
	//							AfxMessageBox("靠近中心失败");
	//						}
	//					}
	//					else
	//						//cout<<"角度校准失败"<<endl;
	//						AfxMessageBox("角度校准失败");
	//				}

	//			}
//			}

	//return true;
		}
		
		if (lps->childv->m_hWnd && lps->bRun) 
		{
			UpdateWindow( lps->childv->m_hWnd );
			//如果窗口更新的区域不为空，UpdateWindow函数通过发送一个WM_PAINT消息来更新指定窗口的客户区。
			//函数绕过应用程序的消息队列，直接发送WM_PAINT消息给指定窗口的窗口过程，如果更新区域为空，则不发送消息。
			
			InvalidateRect( lps->childv->m_hWnd, NULL, FALSE );
			//InvalidateRect是将窗口中的一块矩形区域标注为“无效”，系统会不断向窗口发送WM_PAINT消息令其重绘。
			//该函数会合并所有“无效”区域，对DC进行裁剪，将整个窗口标注为“有效”，清除WM_PAINT消息。
		}
	}

	if (image)
	{
		cvReleaseImage(&image);
	}
	if (hMVlocal != INVALID_HANDLE_VALUE) {
		xiStopAcquisition( hMVlocal );
		xiCloseDevice( hMVlocal );
	}
	Sleep(10);
	//暂停、停止
	bHalted = TRUE;
	lps->hShadingThread=NULL;
	return 0;
}

/****************************************************************************/
//保存数据
void SaveRaw(LPVOID	lpData, DWORD cx, DWORD cy)
{
	char	filename[64];
	FILE * fp;
	CTime t = CTime::GetCurrentTime();
	int sekond = t.GetSecond();
	int minute = t.GetMinute();
	int hour = t.GetHour();
	int day = t.GetDay();
	int month = t.GetMonth();
	int year = t.GetYear();
	struct _timeb timebuffer;
	_ftime( &timebuffer );
	int mili = timebuffer.millitm;
	sprintf( filename, "%dx%d_%d.%.2d.%.2d %.2d.%.2d.%.2d.%03d.raw", cx, cy, year, month, day, hour, minute, sekond, mili);
	fp = fopen(filename, "wb");
	int written = 0;
	int	towrite;
	LPBYTE buffer = (LPBYTE)lpData;
	int block_towrite = 100000;
	DWORD dwDataSize = cx * cy;
	if(fp != NULL)
	{
		towrite = dwDataSize;
		while(towrite > 0){
			buffer += written;
			written = (int)fwrite(buffer, 1, (towrite > block_towrite)?block_towrite:towrite, fp);
			towrite -= written;
		}
		fclose(fp);
		fp = NULL;
	}
}

//保存图片
bool RequestSaveImage(char* filename)
{
	strncpy(save_image_filename,filename,sizeof(save_image_filename));
	flag_request_save_image=true;
	return true;
}

unsigned int ConvertCoordinates(LPSETUP lps, unsigned int coordinates, BOOL y)    //协调
{
	if(y){ //Vertical coordinates
		return (coordinates - lps->y0) * lps->imgs.height / lps->cy;
	}else{
		return (coordinates - lps->x0) * lps->imgs.width / lps->cx;
	}
}

/****************************************************************************/
void pntInitPalette()   // 调色板
{
int		i, j;
int nUsableColors;
struct {
	WORD Version;
	WORD NumberOfEntries;
	PALETTEENTRY aEntries[COLORS];
	} Palette = { 0x300, COLORS };

	//*** Fill in the entries from the given color table
	nUsableColors = COLORS;
	for (j=0,i=0; i<nUsableColors; i++,j++) {
		Palette.aEntries[i].peRed   = (BYTE)((COLORS*j) / nUsableColors);
		Palette.aEntries[i].peGreen = (BYTE)((COLORS*j) / nUsableColors);
		Palette.aEntries[i].peBlue  = (BYTE)((COLORS*j) / nUsableColors);
		Palette.aEntries[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
	}

	lps->hPalGray = CreatePalette ((LOGPALETTE *)&Palette);
}

//#define USE_BITMAP

void DrawImage( CDC * dc, XiPlayPane *childv)
{
	BOOL	stretch = TRUE;  //可伸缩的
	HPEN hpRed;

#ifdef USE_BITMAP
	CDC      m_MemDC;
	CBitmap  m_Bitmap;
#endif

	if(childv->hMV == INVALID_HANDLE_VALUE) return;
	if(!lps->bHaveData) return;

	double	tau = 0.5;
	double Disp = 0;
	DWORD level = 0;
	lps->fTPF_disp = getT_disp();
	
	if(lps->fTPF_disp < 1. / lps->display_refresh) 
		return;

	setT0_disp();

	lps->dwNframes_disp++;

	if(lps->fTPF_disp) Disp = 1./lps->fTPF_disp;
	if (lps->dwNframes_disp) lps->fDisp_sum = lps->fDisp_sum*tau + Disp*(1.-tau);
	else		      lps->fDisp_sum = Disp;

#ifdef USE_BITMAP
	m_MemDC.CreateCompatibleDC(dc);

	HDC	hDC = m_MemDC.m_hDC;
#endif

	memcpy(&lps->imgs, &lps->img, sizeof(XI_IMG));
	//lps->imgs.width = lps->img.width + lps->img.padding_x / lps->bpp;
	CRect rect;
	//childv->GetClientRect(&lps->wndsize);    //获取客户区的大小
	childv->GetClientRect(&rect);
	lps->wndsize.left = rect.left;
	lps->wndsize.top = rect.top;
	lps->wndsize.right = rect.right/2;
	lps->wndsize.bottom = rect.bottom/2;

#ifdef USE_BITMAP
	m_Bitmap.CreateCompatibleBitmap(dc,lps->wndsize.Width(),lps->wndsize.Height());
	m_MemDC.SelectObject(m_Bitmap);
#else
	HDC	hDC = dc->m_hDC;
#endif

	SelectPalette(hDC, lps->hPalGray, FALSE);  //Palette调色板、颜料
	RealizePalette( hDC );
	//该函数从当前逻辑调色板中映射调色板入口点到系统调色板中。
	SetStretchBltMode( hDC, COLORONCOLOR );

	if(lps->zoom == ZOOM_STR){
		stretch = TRUE;

		if (lps->imgs.width*lps->wndsize.Height() > lps->imgs.height*lps->wndsize.Width()) {
			lps->y0 = (lps->wndsize.Height() - lps->imgs.height) / 2;
			lps->y0 = 0;
		}else{
			lps->x0 = (lps->wndsize.Width() - lps->imgs.width) / 2;
			lps->y0 = 0;
		}

		if (lps->imgs.width*lps->wndsize.Height() > lps->imgs.height*lps->wndsize.Width()) {
			double scale = (double)lps->wndsize.Width()/(double)lps->imgs.width;
			lps->cx = lps->wndsize.Width();
			lps->cy = (int)(lps->imgs.height * scale);
			lps->y0 = (lps->wndsize.Height() - lps->cy) / 2;
			lps->x0 = 0;
		} else {
			double scale = (double)(lps->wndsize.Height())/(double)lps->imgs.height;
			lps->cx = (int)(lps->imgs.width * scale);
			lps->cy = lps->wndsize.Height();
			lps->x0 = (lps->wndsize.Width() - lps->cx) / 2;
			lps->y0 = 0;
		}
	}else{
		switch(lps->zoom)
		{
		case ZOOM_25:
			lps->cx = lps->imgs.width / 4;

			lps->cy = lps->imgs.height / 4;
			break;
		case ZOOM_50:
			lps->cx = lps->imgs.width / 2;
			lps->cy = lps->imgs.height / 2;
			break;
		case ZOOM_100:
			lps->cx = lps->imgs.width;
			lps->cy = lps->imgs.height;
			stretch = FALSE;
			break;
		case ZOOM_200:
			lps->cx = lps->imgs.width * 2;
			lps->cy = lps->imgs.height * 2;
			break;
		}
		lps->x0 = 0;
		lps->y0 = 0;
	}

#ifndef USE_BITMAP
	ExcludeClipRect(hDC, lps->x0, lps->y0, lps->cx + lps->x0, lps->cy + lps->y0);
	PatBlt( hDC, 0, 0, 4096, 4096, BLACKNESS );
	SelectClipRgn( hDC, NULL );
#endif

	if(lps->imgs.frm == XI_RGB32){
		myBITMAPINFO	bi   = { 40, 0, 0, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };
	
		bi.biBitCount = 32;
		bi.biClrUsed  = 0;
		bi.biClrImportant = 0;
		bi.biSizeImage = 0;
		bi.biWidth  = lps->imgs.width + lps->imgs.padding_x / lps->bpp;
		bi.biHeight = -1*(lps->imgs.height);
		
		if(stretch) 
			StretchDIBits(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, lps->imgs.width, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS, SRCCOPY);
		else 
			SetDIBitsToDevice(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, 0, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS);
	}
	else if(lps->imgs.frm == XI_RGB24)
	{
		myBITMAPINFO	bi   = { 40, 0, 0, 1, 24, BI_RGB, 0, 0, 0, 0, 0 };
	
		bi.biBitCount = 24;
		bi.biClrUsed  = 0;
		bi.biClrImportant = 0;
		bi.biSizeImage = 0;
		bi.biWidth  = lps->imgs.width + lps->imgs.padding_x / lps->bpp; 
		bi.biHeight = -1*(lps->imgs.height);
		if(stretch) StretchDIBits(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, lps->imgs.width, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS, SRCCOPY);
		else SetDIBitsToDevice(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, 0, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS);
	}
	else if(lps->imgs.frm == XI_RAW16)
	{
		myBITMAPINFO	bi   = { 40, 0, 0, 1, 16, BI_RGB, 0, 0, 0, 0, 0 };
		
		bi.biBitCount = 16;
		bi.biClrUsed  = COLORS;
		bi.biClrImportant = COLORS;
		bi.biSizeImage = 0; 
		bi.biWidth  = lps->imgs.width + lps->imgs.padding_x / lps->bpp; 
		bi.biHeight = -1*(lps->imgs.height);
		for(int i=0; i<COLORS; i++) 
			bi.index[i] = (WORD)i;
		if(stretch) 
			StretchDIBits(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, lps->imgs.width, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS, SRCCOPY);
		else 
			SetDIBitsToDevice(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, 0, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS);
	}
	else{
		myBITMAPINFO	bi   = { 40, 0, 0, 1, 8, BI_RGB, 0, 0, 0, 0, 0 };
		
		bi.biBitCount = 8;
		bi.biClrUsed  = COLORS;
		bi.biClrImportant = COLORS;
		bi.biSizeImage = 0;
		bi.biWidth  = lps->imgs.width + lps->imgs.padding_x / lps->bpp; 
		bi.biHeight = -1*(lps->imgs.height);
		for(int i=0; i<COLORS; i++) 
			bi.index[i] = (WORD)i;
		if(stretch) 
			StretchDIBits(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, lps->imgs.width, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS, SRCCOPY);
		else SetDIBitsToDevice(hDC, lps->x0, lps->y0, lps->cx, lps->cy, 0, 0, 0, lps->imgs.height, lps->imgs.bp, (LPBITMAPINFO)&bi, DIB_PAL_COLORS);
	}

	if(lps->cLBDpoint.x >= lps->x0 && lps->cLBDpoint.x < lps->cx + lps->x0 && lps->cLBDpoint.y >= lps->y0 && lps->cLBDpoint.y < lps->cy + lps->y0 &&
		lps->cDragPoint.x >= lps->x0 && lps->cDragPoint.x < lps->cx + lps->x0 && lps->cDragPoint.y >= lps->y0 && lps->cDragPoint.y < lps->cy + lps->y0 &&
			lps->cLBDpoint.x != lps->cDragPoint.x && lps->cLBDpoint.y != lps->cDragPoint.y)
	{ 
			hpRed  = CreatePen(PS_SOLID, 1, RGB( 255,   0,   0));
			SelectPen( hDC, hpRed );
			SelectBrush( hDC, GetStockObject(NULL_BRUSH));
			Rectangle( hDC, lps->cLBDpoint.x, lps->cLBDpoint.y, lps->cDragPoint.x, lps->cDragPoint.y );
			if (hpRed) DeletePen( hpRed );
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_X, 0);
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_Y, 0);
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_WIDTH, ConvertCoordinates(lps, max(lps->cLBDpoint.x, lps->cDragPoint.x), FALSE) - ConvertCoordinates(lps, min(lps->cLBDpoint.x, lps->cDragPoint.x), FALSE));
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_HEIGHT, ConvertCoordinates(lps, max(lps->cLBDpoint.y, lps->cDragPoint.y), TRUE) - ConvertCoordinates(lps, min(lps->cLBDpoint.y, lps->cDragPoint.y), TRUE));
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_X, ConvertCoordinates(lps, min(lps->cLBDpoint.x, lps->cDragPoint.x), FALSE));
			xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_Y, ConvertCoordinates(lps, min(lps->cLBDpoint.y, lps->cDragPoint.y), TRUE));
	}else{
		xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_X, 0);
		xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_OFFSET_Y, 0);
		xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_WIDTH, lps->img.width);
		xiSetParamInt(childv->hMV, XI_PRM_AEAG_ROI_HEIGHT, lps->img.height);
	}

	if(childv->note && childv->sdlg->m_note && childv->note[0]){
		dc->SelectObject(&childv->font);
		SetTextColor(hDC, RGB(255, 123, 0)); 
		SetBkMode(hDC, TRANSPARENT);
		for(int i = 0; i < childv->number_of_rows; i++)
		{
			TextOut( hDC, 10, 10 + i * childv->font_size, childv->note[i], (int)strlen(childv->note[i]) );
		}
	}

#ifdef USE_BITMAP
	::BitBlt(dc->m_hDC, 0, 0, lps->wndsize.Width(),lps->wndsize.Height(), hDC, 0, 0, SRCCOPY);
	m_MemDC.DeleteDC();
	m_Bitmap.DeleteObject();
#endif
}


int findHalf(cv::Mat& in, const int & maxval, std::vector<int> &Xp, std::vector<int> &Yp)  
{   
	if(in.channels()>1)
		return -1;

	int k=0;
	//cv::Point halfpoint;
	double halfmax=0.5*maxval;

	int rows = in.rows;  
	int cols = in.cols;

	Xp.clear();
	Yp.clear();

	switch (in.elemSize())          //判断图片是8位还是16位
	{
	case 1:

		if(maxval<50)
			return -1;

		for( int i = 0;i < rows;i++)     //i为纵坐标，j为横坐标
		{  
			unsigned char* datain = in.ptr<unsigned char>(i);   //获取某一行的指针
			for(int j = 0;j < cols;j++)  
			{  
				if(datain[j]<(halfmax+1)&&datain[j]>(halfmax-1))
				{
					//((CSeries)m_Chart.Series(0)).AddXY(rows-j, i, NULL, RGB(255,0,255)); 
					k++;
					Xp.push_back(j);
					Yp.push_back(i);
				}
			}  
		}
		//((CCanvas)(m_Chart.get_Canvas())).EllipseWithZ(100, 100, 200, 200, 100);
		break;

	case 2:

		if(maxval<25000)
			return -1;

		for( int i = 0;i < rows;i++)  
		{  
			unsigned short* datain = in.ptr<unsigned short>(i);   
			for(int j = 0;j < cols;j++)  
			{  
				if(datain[j]<(halfmax+10)&&datain[j]>(halfmax-10))
				{
					//((CSeries)m_Chart.Series(0)).AddXY(rows-i, j, NULL, RGB(255,0,255)); 
					k++;
					Xp.push_back(j);
					Yp.push_back(i);
				}
			}  
		}
		break;

	default:
		return -1;
	}
	return k;
}

int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value)
{
	int i=0;
	cv::Point linepoint[2];
	double xielv = std::tan(ang);

	double x1 = p.x-p.y/xielv;
	if( x1>0 && x1<src.cols )
	{
		linepoint[i].x = x1;
		linepoint[i].y = 0;
		++i;
	}
	double x2 = p.x-(p.y-src.rows)/xielv;
	if( x2>0 && x2<src.cols )
	{
		linepoint[i].x = x2;
		linepoint[i].y = src.rows;
		++i;
	}

	double y1 = p.y-p.x*xielv;
	if( i<2 && y1>0 && y1<src.rows )
	{
		linepoint[i].x = 0;
		linepoint[i].y = y1;
		++i;
	}

	double y2 = p.y-(p.x-src.cols)*xielv;
	if( i<2 && y2>0 && y2<src.rows )
	{
		linepoint[i].x = src.cols;
		linepoint[i].y = y2;
		//++i;
	}

	vx.clear();
	value.clear();

	cv::LineIterator it(src, linepoint[0], linepoint[1], 8, true);
	double lx =0;
	double val =0;
	int x = 0;
	int y = 0;
	switch (src.elemSize())
	{
	case 1:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned char>(it.pos());
			if(val>1)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
					vx.push_back(lx);
					value.push_back(val);

				/*	if (x < 0&&val>4)
				{
				lx = -std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
				}*/
			}
		}
		break;

	case 2:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned short>(it.pos());
			if (val>1)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
			}
		}
		break;

	default:
		return -1;
		//cv::line(src, linepoint[0], linepoint[1], cv::Scalar(0), 1, 8, 0 );画出长短轴 
		auto k = it.count;
		return k;
	}
}

int linePointL(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value)
{
	int i=0;
	cv::Point linepoint[2];
	double xielv = std::tan(ang);

	double x1 = p.x-p.y/xielv;
	if( x1>0 && x1<src.cols )
	{
		linepoint[i].x = x1;
		linepoint[i].y = 0;
		++i;
	}
	double x2 = p.x-(p.y-src.rows)/xielv;
	if( x2>0 && x2<src.cols )
	{
		linepoint[i].x = x2;
		linepoint[i].y = src.rows;
		++i;
	}

	double y1 = p.y-p.x*xielv;
	if( i<2 && y1>0 && y1<src.rows )
	{
		linepoint[i].x = 0;
		linepoint[i].y = y1;
		++i;
	}

	double y2 = p.y-(p.x-src.cols)*xielv;
	if( i<2 && y2>0 && y2<src.rows )
	{
		linepoint[i].x = src.cols;
		linepoint[i].y = y2;
		//++i;
	}

	vx.clear();
	value.clear();

	cv::LineIterator it(src, linepoint[0], linepoint[1], 8, true);
	double lx =0;
	double val =0;
	int x = 0;
	int y = 0;
	switch (src.elemSize())
	{
	case 1:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned char>(it.pos());
			if(val>1)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
					vx.push_back(lx);
					value.push_back(val);

				/*	if (x < 0&&val>4)
				{
				lx = -std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
				}*/
			}
		}
		break;

	case 2:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned short>(it.pos());
			if (val>2)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
			}
		}
		break;

	default:
		return -1;
		//cv::line(src, linepoint[0], linepoint[1], cv::Scalar(0), 1, 8, 0 );画出长短轴 
		auto k = it.count;
		return k;
	}
}



//////////////串口发送消息
//void onSendMessage(int SData[6])
//{
//	DWORD dwRes;
//	DWORD dwWrite;
//	BOOL fRes;
//	sendData = "010500010000";
//	//UpdateData(TRUE);
//	AfxMessageBox(sendData);
//	//int arr[10] = {1,2,3,4,5,60,6,7,8,9};
//	//myWByte[1024] = SData[1024];
//	for(int i=0;i<6;++i)
//	{
//		myWByte[i] = SData[i];
//	}
//	//sendData =str;
//	//UpdateData(TRUE);
//	//AfxMessageBox(sendData);
//	//myWByte = (BYTE*)sendData.GetBuffer(sendData.GetLength());
//	//memcpy(myWByte,sendData.GetBuffer(sendData.GetLength()),sendData.GetLength()*2);  //将cstring放入byte数组
//	//memcpy(myWByte+32,sendData.GetBuffer(sendData.GetLength())+16,32);  //将cstring放入byte数组
//	int i = sendData.GetLength();
//	int j = sizeof(myWByte);
//	//dataWLen = sendData.GetLength();
//	dataWLen = 6;
//
//	Wol.hEvent = CreateEvent(NULL,
//		TRUE,
//		FALSE,
//		NULL);		
//	if(WriteFile(hCom,&myWByte,dataWLen,NULL,&Wol))
//	{
//		//发送成功
//	}
//	else
//	{
//		dwRes = WaitForSingleObject(Wol.hEvent,500);
//		switch(dwRes)
//		{
//		case WAIT_OBJECT_0:
//			if(!GetOverlappedResult(hCom,
//				&Wol,
//				&dwWrite,
//				TRUE))
//			{
//				//success
//			}
//			else
//			{
//				//fail
//			}
//			break;
//		case WAIT_TIMEOUT:
//			break;
//		default:
//			break;
//		}
//	}
//	CloseHandle(Wol.hEvent);
//
//	PurgeComm(hCom, PURGE_TXABORT|PURGE_TXCLEAR); 
//}


//////////////////////线程/////////////////////////////////
//DWORD WINAPI ThreadProcEvent(LPVOID pParam){
//	DWORD dwEvtMask,dwRes;
//
//	Eol.hEvent = CreateEvent(NULL,
//		TRUE,
//		FALSE,
//		NULL);
//	while(fEventRun){
//		WaitCommEvent(hCom,
//			&dwEvtMask,
//			&Eol);
//		dwRes = WaitForSingleObject(Eol.hEvent,100);
//		switch(dwRes)
//		{
//		case WAIT_OBJECT_0:
//			switch(dwEvtMask)
//			{
//			case EV_RXCHAR:
//				if(!fStopMsg)
//				{
//					AfxMessageBox(_T("监测到接收信息。。"));
//					fStopMsg = true;
//					::PostMessageA(hWnd,
//						WM_MYMSG,
//						0,
//
//						(LPARAM)EV_RXCHAR);
//				}
//				break;
//			case EV_TXEMPTY:
//				AfxMessageBox(_T("监测到发送信息。。"));
//				break;
//			}
//			break;
//		}
//	}
//	return 0;
//}

//////////////////////////接收消息
//long OnReceiveEvent(WPARAM wParam, LPARAM lParam)
//{
//	fStopMsg = true;
//	//	MessageBox(_T("bbbbbbb"));
//	receiveData="";
//
//
//	//AfxMessageBox(_T("到达接收对应的消息响应函数。。"));
//	//PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR );
//	CString temp;
//	//AfxMessageBox(L"到此。。");
//	DWORD dwRes;
//	DWORD dwRead;
//	DWORD dwErrors;
//	COMSTAT Rcs;
//	ClearCommError(hCom,
//		&dwErrors,
//		&Rcs);
//	if(ReadFile(hCom,
//		&myRByte,
//		Rcs.cbInQue,
//		NULL,
//		&Rol))
//	{
//		//串口数据在此成功读出
//	}
//	else
//	{
//		Rol.hEvent = CreateEvent(NULL,
//			TRUE,
//			FALSE,
//			NULL);
//
//		dwRes = WaitForSingleObject(Rol.hEvent,3000);
//		switch(dwRes)
//		{
//		case WAIT_OBJECT_0:
//			if(!GetOverlappedResult(hCom,
//				&Rol,
//				&dwRead,
//				TRUE))
//			{
//				AfxMessageBox(_T("使用GetOverlappedResult函数出错。。"));
//			}
//			else
//			{
//				//AfxMessageBox(L"操作完成，数据读出。。");
//				//延迟串口数据在此成功读出
//			}
//			break;
//		case WAIT_TIMEOUT:
//			AfxMessageBox(_T("读取超时。。"));
//			break;
//		default:
//			AfxMessageBox(_T("不清楚原因。。"));
//			break;
//		}
//	}
//
//	int i;
//	for(i=0;i<Rcs.cbInQue;i++)
//	{
//		temp.Format(_T("%c"),myRByte[i]);
//		receiveData += temp;
//	}
//	CString S=receiveData;
//	AfxMessageBox(receiveData);
//	if(S=="jiang")
//	{
//		AfxMessageBox(_T("动一动。"));
//	}
//	//UpdateData(FALSE);
//	PurgeComm(hCom,PURGE_RXABORT|PURGE_RXCLEAR);
//	fStopMsg = false;
//	/*if (conPicSign == true)
//	{
//		onContinue();
//	}*/
//	return 0;
//}




