// GaussPane.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "GaussPane.h"


// GaussPane

IMPLEMENT_DYNAMIC(GaussPane, CDockablePane)

//构造函数
GaussPane::GaussPane()
          :gaussLine(),
           P0(0,0),
		   lineL(0),
		   unitL(2000),
		   scaleX(10),
		   scaleY(1)
{
	SetMinSize(CSize(400,200));   //设置面板的最小尺寸
}

GaussPane::~GaussPane()
{
}


BEGIN_MESSAGE_MAP(GaussPane, CDockablePane)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// GaussPane 消息处理程序




BOOL GaussPane::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, RGB(255,255,255)); // 背景色

	CPen pen(PS_JOIN_ROUND, 2, RGB(70,70,255));  
	pDC->SelectObject(&pen); 
	int width = P0.x-lineL;
	int height = rcClient.Height();
	/***********************************************************/
	//画坐标系
	pDC->MoveTo(P0.x-lineL, P0.y);   //x轴
	pDC->LineTo(P0.x+lineL, P0.y);
	pDC->TextOutA(P0.x+lineL-15, P0.y-20, _T("x"), 1);
	pDC->MoveTo(P0.x+lineL-5, P0.y+2);
	pDC->LineTo(P0.x+lineL+2, P0.y);
	pDC->MoveTo(P0.x+lineL-5, P0.y-2);
	pDC->LineTo(P0.x+lineL+2, P0.y);

	pDC->MoveTo(P0.x, P0.y-0.75*lineL);    //y轴
	pDC->LineTo(P0.x, P0.y+0.15*lineL);
	pDC->TextOutA(P0.x+15, P0.y-0.75*lineL-10, _T("y"), 1);
	pDC->MoveTo(P0.x+2, P0.y-0.75*lineL+5);
	pDC->LineTo(P0.x, P0.y-0.75*lineL-2);

	pDC->MoveTo(P0.x-2, P0.y-0.75*lineL+5);
	pDC->LineTo(P0.x, P0.y-0.75*lineL-2);

	pDC->MoveTo(P0.x, P0.y-0.35*lineL);
	pDC->LineTo(P0.x+5, P0.y-0.35*lineL);
	pDC->MoveTo(P0.x, P0.y-0.7*lineL);
	pDC->LineTo(P0.x+5, P0.y-0.7*lineL);
	pDC->TextOutA(P0.x-20, P0.y-0.75*lineL, _T("1"), 1);

	pDC->MoveTo(P0.x+lineL*0.5, P0.y);
	pDC->LineTo(P0.x+lineL*0.5, P0.y-5);
	pDC->MoveTo(P0.x-lineL*0.5, P0.y);
	pDC->LineTo(P0.x-lineL*0.5, P0.y-5);

	pDC->TextOutA(P0.x+5, P0.y+5, _T("0"), 1); //将string转化为c风格字符串
	pDC->TextOutA(P0.x+lineL*0.5, P0.y+5, _T("1000"), 4);
	pDC->TextOutA(P0.x-lineL*0.5, P0.y+5, _T("1000"), 4);
	/*******************************************************/
	return CDockablePane::OnEraseBkgnd(pDC);
}


void GaussPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	
	lineL = (cx/2<cy ? cx/2:cy)*0.95;  //坐标长度取窗口长度的90%
	P0.x = cx/2;
	P0.y = cy/2+0.3*lineL;
	InvalidateRect(NULL, TRUE);   //更新窗口,第二个参数设置为true可以在窗口缩小的时候刷新窗口
}


void GaussPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDockablePane::OnPaint()
	CPen pen(PS_JOIN_ROUND, 2, RGB(255,0,0));
	dc.SelectObject(&pen);
	//gaussLine.A = 230;
	//gaussLine.Q = 2000;
	//gaussLine.U = 0;
	//gaussLine.H = 5;
	if(gaussLine.A>0)
	{
		drawGaussLine(gaussLine,dc);
	}
	ReleaseDC(&dc);
}

int GaussPane::round(double d)     //四舍五入取整
{
	return (int)(d+0.5);
}

void GaussPane::drawGaussLine(const CGaussLine &gaussLine, CPaintDC &dc)
{
	double XValue = gaussLine.Q*std::sqrt(-log(0.1f))+gaussLine.U;
	
	/*while (XValue > unitL)
	{
		unitL *= 2;
	}*/
	if (unitL==4000)
	{ 
		dc.TextOutA(P0.x+lineL*0.5, P0.y+5, _T("2000"), 4);
		dc.TextOutA(P0.x-lineL*0.5, P0.y+5, _T("2000"), 4);
	}
	scaleY = gaussLine.A*((0.7*lineL)/255);
	scaleX = unitL/lineL;

/*************************************************************/	
//画曲线	
	double startL = P0.x-lineL*0.9;          
	double startX = -lineL*0.9*scaleX;

	dc.MoveTo(startL, P0.y-(scaleY * std::exp( -(((startX - gaussLine.U)/gaussLine.Q)
		*((startX - gaussLine.U)/gaussLine.Q))) + gaussLine.H));
	++startL;
	//++startX;
	int endL = P0.x + lineL*0.9;
	int yValue;
	for ( ; startL<endL; ++startL )
	{
		startX += scaleX;
		yValue = round(scaleY * std::exp( -(((startX - gaussLine.U)/gaussLine.Q)
			*((startX - gaussLine.U)/gaussLine.Q))) + gaussLine.H);
		dc.LineTo(startL,P0.y-yValue); 
	}
/***********************************************************************/

	dc.TextOutA(5, 5, _T("缩放比例："), 10);
	CString str;
	str.Append(_T("X: "));
	str.AppendFormat("%d",1);
	str.Append(_T(":"));
	str.AppendFormat("%f",scaleX);
	dc.TextOutA(5, 25, str, str.GetLength());
	str.Empty();
	str.Append(_T("Y: "));
	str.AppendFormat("%f",0.7*lineL);
	str.Append(_T(":"));
	str.AppendFormat("%d",1);
	str.Append(_T(":"));
	str.AppendFormat("%d",255);
	dc.TextOutA(5, 45, str, str.GetLength());
}