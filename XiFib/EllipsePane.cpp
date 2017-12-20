// EllipsePane.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "EllipsePane.h"
#include <string>
#include <sstream>
// EllipsePane

IMPLEMENT_DYNAMIC(EllipsePane, CDockablePane)

EllipsePane::EllipsePane()
       :Epse(),
	    P0(0,0),
		lineL(0)
{
	SetMinSize(CSize(300,300));   //设置面板的最小尺寸
}

EllipsePane::~EllipsePane()
{
}


BEGIN_MESSAGE_MAP(EllipsePane, CDockablePane)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
//ON_WM_SIZING()
END_MESSAGE_MAP()

// EllipsePane 消息处理程序

BOOL EllipsePane::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, RGB(255,255,255)); // 背景色	

	CPen pen(PS_SOLID, 2, RGB(70, 70, 255));  
	pDC->SelectObject(&pen); 
	int width = P0.x-lineL;
	int height = rcClient.Height();
/***********************************************************/
	//画坐标系
	pDC->MoveTo(P0.x-lineL, P0.y);
	pDC->LineTo(P0.x+lineL, P0.y);
	pDC->TextOutA(P0.x+lineL-15, P0.y-20, _T("x"), 1);
    pDC->MoveTo(P0.x+lineL-5, P0.y+2);
	pDC->LineTo(P0.x+lineL+2, P0.y);
	pDC->MoveTo(P0.x+lineL-5, P0.y-2);
	pDC->LineTo(P0.x+lineL+2, P0.y);

	pDC->MoveTo(P0.x, P0.y-lineL);
	pDC->LineTo(P0.x, P0.y+lineL);
	pDC->TextOutA(P0.x+8, P0.y-lineL+5, _T("y"), 1);
	pDC->MoveTo(P0.x+2, P0.y-lineL+5);
	pDC->LineTo(P0.x, P0.y-lineL-2);
	pDC->MoveTo(P0.x-2, P0.y-lineL+5);
	pDC->LineTo(P0.x, P0.y-lineL-2);

	pDC->MoveTo(P0.x+lineL*0.6, P0.y);
	pDC->LineTo(P0.x+lineL*0.6, P0.y-7);
	pDC->MoveTo(P0.x-lineL*0.6, P0.y);
	pDC->LineTo(P0.x-lineL*0.6, P0.y-7);

    pDC->TextOutA(P0.x+5, P0.y+5, _T("X0,Y0"), 5); //将string转化为c风格字符串
	pDC->TextOutA(P0.x+lineL*0.6, P0.y+5, _T("300"), 3);
	pDC->TextOutA(P0.x-lineL*0.6, P0.y+5, _T("300"), 3);
/*******************************************************/

	return CDockablePane::OnEraseBkgnd(pDC);
}


void EllipsePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  
	dc.SelectObject(&pen); 
	
	//Epse.xl=600;
	//Epse.yl=600;
	//Epse.xc = 1003.3344;
	//Epse.yc = 2999.3333;
	//Epse.ang=0;   //cos(),sin()函数计算的都是弧度

	if (Epse.xl>0&&Epse.yl>0)
	{
		Epse.xl=Epse.xl*lineL/500;
		Epse.yl=Epse.yl*lineL/500;
		drawEllipse(Epse,P0,dc);
	}
	ReleaseDC(&dc);
}


void EllipsePane::OnSize(UINT nType, int cx, int cy)
{
	if (0.75< cx/cy && cx/cy<1.25)
	{
		P0.x = cx/2;
		P0.y = cy/2;
	}
	else
	{
		int len = cx<cy ? cx:cy;
		P0.x = P0.y = len/2;
	}
	lineL = (cx<cy ? cx:cy) * 0.45;  //坐标长度取窗口长度的90%
	
	InvalidateRect(NULL, TRUE);   //更新窗口,第二个参数设置为true可以在窗口缩小的时候刷新窗口
	CDockablePane::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
}

int EllipsePane::round(double d)     //四舍五入取整
{
	return (int)(d+0.5);
}

void EllipsePane::drawEllipse(const CEllipse &e, const CPoint &p0, CPaintDC &dc)   //画椭圆
{
	CPoint ptS;
	ptS.x = round(p0.x+e.xl*cos(e.ang));
	ptS.y = round(p0.y+e.xl*sin(e.ang));

	int total = 180;
	double fa = 2*3.1415926 / total;
	float x1, y1;

	dc.MoveTo(ptS.x, ptS.y);
	for (int i=0; i<=total; i++)
	{
		x1 = round(p0.x + e.xl*cos(fa*i)*cos(e.ang) - e.yl*sin(fa*i)*sin(e.ang));
		y1 = round(p0.y + e.xl*cos(fa*i)*sin(e.ang) + e.yl*sin(fa*i)*cos(e.ang));
		dc.LineTo(x1, y1);	
	}
}