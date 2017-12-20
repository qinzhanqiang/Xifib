// TZDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "AdjustDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "acquisition.h"
#include "InputDialog.h"
#include "CLineFit.h"

extern LPSETUP lps;   //保存设置的结构体
extern pdsList pDSL;

// CAdjustDialog 对话框

IMPLEMENT_DYNAMIC(CAdjustDialog, CDialogEx)

//CAdjustDialog::CAdjustDialog(CWnd* pParent /*=NULL*/)
//	: CDialogEx(CAdjustDialog::IDD, pParent),
//	  scaleY(5),
//	  vecXL(5),
//	  vecYL(5),
//	  vecXS(5),
//	  vecYS(5),
//	  lineL(),
//	  lineS(),
//	  Pthis(NULL)
//{

//}

CAdjustDialog::	CAdjustDialog(CAdjustDialog * &p, CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdjustDialog::IDD, pParent),
	m_CP(0),
	m_CHP(0),
	scaleY(5),
	vecX(5),
	vecYL(5),
	vecYS(5),
	lineL(),
	lineS(),
	Pthis(p), //关闭对话框时释放当前对象
	m_Dis(5.0)
{
	//m_brush.CreateSolidBrush(RGB(150,150,150));
}

CAdjustDialog::~CAdjustDialog()
{
	//m_brush.DeleteObject();
}

void CAdjustDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANS_LIST, m_ANSList);
	DDX_Text(pDX, IDC_CP_EDIT, m_CP);
	DDV_MinMaxDouble(pDX, m_CP, -30, 30);
	DDX_Text(pDX, IDC_CHP_EDIT, m_CHP);
	DDV_MinMaxDouble(pDX, m_CHP, -30, 30);
}


BEGIN_MESSAGE_MAP(CAdjustDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
//	ON_WM_NCDESTROY()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_START_BUTTON, &CAdjustDialog::OnBnClickedStartButton)
END_MESSAGE_MAP()


// CAdjustDialog 消息处理程序


BOOL CAdjustDialog::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFont font;
	font.CreateFont(20,0,0,0,FW_THIN,TRUE,FALSE,0,1,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("微软雅黑"));
	m_ANSList.SetFont(&font);       //改变字体大小
	
	CRect rect;   
	// 获取编程语言列表视图控件的位置和大小   
	m_ANSList.GetClientRect(&rect);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_ANSList.SetExtendedStyle(m_ANSList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// 为列表视图控件添加三列   
	m_ANSList.InsertColumn(0, _T("距离"), LVCFMT_CENTER, rect.Width()/6, 0);   
	m_ANSList.InsertColumn(1, _T("长轴长度"), LVCFMT_CENTER, rect.Width()/6, 1);   
	m_ANSList.InsertColumn(2, _T("短轴长度"), LVCFMT_CENTER, rect.Width()/6, 2);
	m_ANSList.InsertColumn(3, _T("X0"), LVCFMT_CENTER, rect.Width()/6, 3);   
	m_ANSList.InsertColumn(4, _T("Y0"), LVCFMT_CENTER, rect.Width()/6, 4);
	m_ANSList.InsertColumn(5, _T("旋转角度"), LVCFMT_CENTER, rect.Width()/6, 5);   

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAdjustDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	dc.SetBkMode(TRANSPARENT);   //设置文字背景色为透明
	CPen pen(PS_SOLID, 2, RGB(70, 70, 255));  
	dc.SelectObject(&pen); 

	//x轴为10：1
	//y轴为1：scaleY
	/***********************************************************/
	//画坐标系
	dc.MoveTo(30, 235);   //x轴
	dc.LineTo(385,235);
	dc.TextOutA(370, 240, _T("x"), 1);
	dc.MoveTo(380,233);
	dc.LineTo(387,235);
	//dc.MoveTo(380,233);
	dc.LineTo(380,237);
	
	dc.MoveTo(85,35);     //y轴
	dc.LineTo(85, 290);
	dc.TextOutA(90, 35, _T("y"), 1);
	dc.MoveTo(83,40);
	dc.LineTo(85,33);
	//dc.MoveTo(380,233);
	dc.LineTo(87,40);

	dc.TextOutA(90, 240,_T("0"),1);

	dc.MoveTo(35,235);
	dc.LineTo(35,230);
	dc.TextOutA(35,240,_T("-5"),2);
	for (int i =1; i<6; i++)
	{
		int x = i*50+85;
		CString str;
		str.AppendFormat("%d",i*5);
		dc.MoveTo(x,235);
		dc.LineTo(x,230);
		dc.TextOutA(x,240,str,str.GetLength());
	}

	dc.MoveTo(85,285);
	dc.LineTo(90,285);
	CString str;
	str.Append("-");
	str.AppendFormat("%d",scaleY*50);
	dc.TextOutA(50,285,str,str.GetLength());
	for (int i =1;i<4;++i)
	{
		int y = 235-i*50;
		CString str;
		str.AppendFormat("%d",50*scaleY*i);
		dc.TextOutA(50,y,str,str.GetLength());
		dc.MoveTo(85,y);
		dc.LineTo(90,y);
	}

	/*******************************************************/
	dc.MoveTo(30, 545);   //x轴
	dc.LineTo(385,545);
	dc.TextOutA(370, 550, _T("x"), 1);
	dc.MoveTo(380,543);
	dc.LineTo(387,545);
	//dc.MoveTo(380,233);
	dc.LineTo(380,547);

	dc.MoveTo(85,345);     //y轴
	dc.LineTo(85, 600);
	dc.TextOutA(90, 345, _T("y"), 1);
	dc.MoveTo(83,350);
	dc.LineTo(85,343);
	//dc.MoveTo(380,233);
	dc.LineTo(87,350);

	dc.TextOutA(90, 550,_T("0"),1);

	dc.MoveTo(35,545);   //x轴刻度
	dc.LineTo(35,540);
	dc.TextOutA(35,550,_T("-5"),2);
	for (int i =1; i<6; i++)
	{
		int x = i*50+85;
		CString str;
		str.AppendFormat("%d",i*5);
		dc.MoveTo(x,545);
		dc.LineTo(x,540);
		dc.TextOutA(x,550,str,str.GetLength());
	}

	dc.MoveTo(85,595);   //y轴刻度
	dc.LineTo(90,595);
	dc.TextOutA(50,595,str,str.GetLength());
	for (int i =1;i<4;++i)
	{
		int y = 545-i*50;
		CString str;
		str.AppendFormat("%d",50*scaleY*i);
		dc.TextOutA(50,y,str,str.GetLength());
		dc.MoveTo(85,y);
		dc.LineTo(90,y);
	}
	//
	//vecX[0] = 5;
	//vecYL[0] = 250;
	//vecYS[0] = 500;
	//lineL.K=50;
	//lineS.K=25;
	//lineS.B = 5;
	//lineL.B = 10;
	Draw(dc);
}


BOOL CAdjustDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL ret = CDialogEx::OnEraseBkgnd(pDC);
	// TODO: Add your control notification handler code here   
	COLORREF color = RGB(70, 70, 70);    // 颜色对话框的初始颜色     
	
	CRect rect(10,10,410,310);       // 图片控件的矩形区域坐标   
	CRect rect2(10,320,410,620);

	CBrush newBrush;         // 创建的新画刷   
	CBrush *pOldBrush;       // 旧画刷的指针   
	newBrush.CreateSolidBrush(color);       
	pOldBrush = pDC->SelectObject(&newBrush);     
	pDC->Rectangle(rect); 
	pDC->Rectangle(rect2); 

	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);   
	// 删除新画刷   
	newBrush.DeleteObject(); 
	return ret;
}


HBRUSH CAdjustDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)    //设置背景色
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
	// TODO:  在此更改 DC 的任何特性
	//return m_brush;
}

void CAdjustDialog::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	//delete this;
	//CDialogEx::PostNcDestroy();
	Pthis =NULL;	
	delete this;
		
}


void CAdjustDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
	SendMessage(WM_DESTROY);
}


void CAdjustDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	PostNcDestroy();
	// TODO: 在此处添加消息处理程序代码
}

void CAdjustDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类 
	UpdateData(false);     //显示计算结果
	((CMainFrame *)AfxGetMainWnd())->m_P = m_CHP;
	CDialogEx::OnOK();
}

void CAdjustDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	m_CP = ((CMainFrame *)AfxGetMainWnd())->m_P;
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(false);      
	// TODO: 在此处添加消息处理程序代码
}


void CAdjustDialog::OnBnClickedStartButton()
{
	 //TODO: 在此添加控件通知处理程序代码

	if(!lps->bRun) 
	{
		CString str("相机未连接");
		MessageBox(str, _T("警告"), MB_OK | MB_ICONWARNING); 
		return;
	}
	if(!lps->bHaveData)
	{
		CString str("没有数据");
		MessageBox(str, _T("警告"), MB_OK | MB_ICONWARNING); 
		return;
	}

	long IDImg = 0;  //图片编号
	GaussFit m_gaussFit;
	CEllipseFit LSellipse;

	CEllipse tem_Ellipse;
	CGaussLine tem_GaussLine;
	CGaussLine tem_LGaussLine;

	vecX.clear();  //清空已保存的数据
	vecYL.clear();
	vecYS.clear();
	int num = vecX.size();

	cv::Mat src(lps->imgs.height, lps->imgs.width, CV_8UC1);
	CInputDialog dlg;

	while (num<17)    //列表控件只有17行，最多存17次
	{
		int ret = dlg.DoModal();
		if( ret == 11)   //如果点击完成
		{
			if (num < 2)
			{
				CString str("至少需要2次");
			    MessageBox(str, _T("警告"), MB_OK | MB_ICONWARNING); 
			    continue;
			}
			else
				break;     //如果点的个数大于2个就跳出循环
		}

		if (ret == IDOK)
		{
			m_Dis = dlg.m_Dis;
			m_Ellipse.clear();
			m_GaussLine.clear();
			m_LGaussLine.clear();

			int i=0;
			for ( ;i<3;++i)
			{
				while (!(lps->dwNframes>IDImg))
				{
					Sleep(1);
				}

				IplImage* image = cvCreateImage(cvSize( 2048, 2048), IPL_DEPTH_8U, 1); //将XI_IMG转化为IplImage
				memcpy( image->imageData, lps->img.bp, (lps->img.width)*(lps->img.height)*lps->bpp);
				cvSaveImage("D:\\saveImage3.jpg",image);
				src = cv::cvarrToMat(image);
				//memcpy(src.data, lps->imgs.bp, lps->imgs.width*lps->imgs.height);  //内存拷贝
				findHalf(src, m_XVec, m_YVec);

				if( m_XVec.size()>5 && m_YVec.size()>5 )
				{
					LSellipse.getEllipse(m_XVec, m_YVec, tem_Ellipse);
				}
				int valofc;
				if (src.elemSize()==1)
				{
					valofc = src.at<unsigned char>(m_Ellipse.yc, m_Ellipse.xc); //中心值
				}
				else{
					valofc = src.at<unsigned short>(m_Ellipse.yc, m_Ellipse.xc); //中心值
				}
				if (m_Ellipse.xl!=0)
				{
					linePoint(src, cv::Point((int)(round(tem_Ellipse.xc)), (int)(round(tem_Ellipse.yc))), tem_Ellipse.ang, m_GXVec, m_GYVec);
					linePoint(src, cv::Point((int)(round(tem_Ellipse.xc)), (int)(round(tem_Ellipse.yc))), tem_Ellipse.ang+1.5707963, m_LGXVec, m_LGYVec);
					gaussfit(m_GXVec, m_GYVec, valofc , m_Ellipse.xl, tem_GaussLine);
					gaussfit(m_LGXVec, m_LGYVec, valofc, m_Ellipse.yl, tem_LGaussLine);
				}

				m_Ellipse    += tem_Ellipse;
				m_GaussLine  += tem_GaussLine;
				m_LGaussLine += tem_LGaussLine;
			}

			m_Ellipse    /= i;   //求平均值
			m_GaussLine  /= i;
			m_LGaussLine /= i;
			
			CString lstr;
			lstr.AppendFormat("%.2lf""%s",m_Dis,_T("mm"));  //同时格式化多个字符串
			m_ANSList.InsertItem(num,lstr);

			double LhalfW = sqrt( log( (double) 2) )*m_LGaussLine.Q;   //求高斯曲线的半宽度
			//double anghalfweith = 2*(atan((LhalfW * 2.2/1000)/m_Dis))*360/(2*3.14);  //2.2为像素大小
            
			lstr.Format("%.4lf",LhalfW);
			m_ANSList.SetItemText(num,1,lstr);
		
			double halfW = sqrt( log( (double) 2) )*m_GaussLine.Q; 
			lstr.Format("%.4lf",halfW);	
			m_ANSList.SetItemText(num,2,lstr);

			vecX.push_back(m_Dis);    //保存距离和长度
			vecYL.push_back(LhalfW);
			vecYS.push_back(halfW);
			++num;
			lstr.Format("%.4lf",m_Ellipse.xc);
			m_ANSList.SetItemText(num,3,lstr);
			lstr.Format("%.4lf",m_Ellipse.yc);
			m_ANSList.SetItemText(num,4,lstr);
			lstr.Format("%.4lf",m_Ellipse.ang);
			m_ANSList.SetItemText(num,5,lstr);
			SendMessage(WM_PAINT,0,0);
		}
	}
	CLineFit LFit;
	LFit.lineFit(vecX,vecYL,lineL);
	LFit.lineFit(vecX,vecYS,lineS);
	m_CHP =-(lineS.B/lineS.K+lineL.B/lineL.K)/2;
	UpdateData(false);
	PostMessage(WM_PAINT);
}

 double CAdjustDialog::round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}


 int CAdjustDialog::findHalf(cv::Mat& in, std::vector<int> &Xp, std::vector<int> &Yp)  
 {   
	 if(in.channels()>1)
		 return -1;

	 double maxval, minval;
	 cv::Point maxpoint;
	 cv::minMaxLoc(in, &minval, &maxval, NULL, &maxpoint);    //获取最大最小值及其坐标

	 int k=0;
	 //cv::Point halfpoint;
	 double halfmax=0.5*maxval;

	 int rows = in.rows;  
	 int cols = in.cols;

	 //int numofpoint = ((CSeries)(m_Chart.Series(0))).CountLegendItems();
	 //if (numofpoint>0)     //获取曲线Series0上点的个数
	 //  {
	 //	((CSeries)(m_Chart.Series(0))).Clear();                 //清空曲线
	 //  }

	 Xp.clear();
	 Yp.clear();

	 switch (in.elemSize())          //判断图片是8位还是16位
	 {
	 case 1:

		 if(minval>200||maxval<50||(maxval-minval)<50)
			 return -1;

		 for( int i = 0;i < rows;i++)     //i为纵坐标，j为横坐标
		 {  
			 unsigned char* datain = in.ptr<unsigned char>(i);   //获取某一行的指针
			 for(int j = 0;j < cols;j++)  
			 {  
				 if(datain[j]<(halfmax+2)&&datain[j]>(halfmax-2))
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

		 if(minval>10000||maxval<5000||(maxval-minval)<5000)
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

 int CAdjustDialog::linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value)
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
			 if(val>2)
			 {
				 x = it.pos().x-p.x;
				 y = it.pos().y-p.y;
				 if (x < 0)
					 lx = - std::sqrt((double)(x*x + y*y));
				 else
					 lx = std::sqrt((double)(x*x + y*y));
				 vx.push_back(lx);
				 value.push_back(val);
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

 // 画点和直线
 void CAdjustDialog::Draw(CPaintDC & dc)
 {
	
	 if (vecX.size()>0 && vecX[0]>0)
	 {
		 CPen newPen;
		 newPen.CreatePen(PS_SOLID, 2, RGB(255,0,0));
		 CPen* oldPen;
		 oldPen = dc.SelectObject(&newPen);
		 dc.SelectStockObject(NULL_BRUSH);  //原点背景色透明

		 for (int i=0;i<vecX.size()&&vecX[i]>0;++i)
		 {
			 int px = 85+round(vecX[i]*10);  //第一幅图原点坐标为（85，235）
			 int py = 235-round(vecYL[i]/scaleY);
			 int big = 3;  //点的大小
			 dc.Ellipse(px-big,py-big,px+big,py+big);

			 int pyL = 545 -round(vecYS[i]/scaleY);//第2幅图原点坐标为（85，545）
			 dc.Ellipse(px-big,pyL-big,px+big,pyL+big);
		 }
		 dc.SelectObject(oldPen);
		 newPen.DeleteObject();
	 }
	 if (lineL.K!=0&&lineS.K!=0)
	 {
		 CPen newPen;
		 newPen.CreatePen(PS_SOLID, 2, RGB(255,0,255));
		 CPen* oldPen;
		 oldPen = dc.SelectObject(&newPen);

		 int lpx1 = 85-4*10;  //第一幅图原点坐标为（85，235）
		 int lpy1 = 235-round((-4*lineL.K+lineL.B)/scaleY);
		 int lpx2 = 85+(1000-lineL.B)/lineL.K*10;  //连接两个点（-4，y）和（x,1000）
		 int lpy2 =  235-round(1000/scaleY);
		 if ((1000-lineL.B)/lineL.K>30)
		 {
			 lpx2 = 85+30*10;
			 lpy2 = 235-round((30*lineL.K+lineL.B)/scaleY);
		 }
		 
		 dc.MoveTo(lpx1,lpy1);
	     dc.LineTo(lpx2,lpy2);

		 int py1 = 545 -round((-4*lineS.K+lineS.B)/scaleY);
		 int px2 = 85+(1000-lineS.B)/lineS.K*10;
		 int py2 = 545 -round(1000/scaleY);
		 if ((1000-lineS.B)/lineS.K>30)
		 {
			 px2 = 85+30*10;
			 py2 = 545-round((30*lineS.K+lineS.B)/scaleY);
		 }
		 dc.MoveTo(lpx1,py1);
		 dc.LineTo(px2,py2);

		 dc.SelectObject(oldPen);
		 newPen.DeleteObject();
	 }
 }
