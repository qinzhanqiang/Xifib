// DataSum.cpp : 实现文件
//

#include "stdafx.h"
#include "XiFib.h"
#include "DataSum.h"


// DataSum

IMPLEMENT_DYNAMIC(DataSum, CDockablePane)

DataSum::DataSum()
{

}

DataSum::~DataSum()
{
}


BEGIN_MESSAGE_MAP(DataSum, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// DataSum 消息处理程序




int DataSum::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect;
	GetClientRect(&rect);
	dataList.Create(WS_VISIBLE | WS_BORDER | LVS_REPORT, rect, this, 1);
	dataList.SetExtendedStyle(dataList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	dataList.InsertColumn(0, "属性", LVCFMT_CENTER, 80);
	dataList.InsertColumn(1, "当前值", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(2, "平均值", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(3, "最大值", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(4, "最小值", LVCFMT_CENTER, 80);

	dataList.InsertItem(1, "峰值");
	dataList.InsertItem(2, "X0");
	dataList.InsertItem(3, "Y0");
	dataList.InsertItem(4, "短轴长度");
	dataList.InsertItem(5, "长轴长度");
	dataList.InsertItem(6, "短轴发散角");
	dataList.InsertItem(7, "长轴发散角");
	dataList.InsertItem(8, "长短轴之比");
	dataList.InsertItem(9, "长轴角度");
	dataList.InsertItem(10, "偏轴度");
	dataList.InsertItem(11, "状态");

	dataList.ShowWindow(SW_SHOWNORMAL);
	return 0;
}


void DataSum::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDockablePane::OnPaint()
	ReleaseDC(&dc);
}


void DataSum::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);
	//InvalidateRect(NULL, true);   //清除原列表   
	dataList.MoveWindow(rect, true);
	//dataList.ShowWindow(SW_SHOW);
	//InvalidateRect(NULL, true);
}
