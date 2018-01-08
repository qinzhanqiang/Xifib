// DataSum.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiFib.h"
#include "DataSum.h"
#include "Resource.h"


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



// DataSum ��Ϣ�������




int DataSum::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rect;
	GetClientRect(&rect);


	dataList.Create(WS_VISIBLE | WS_BORDER | LVS_REPORT, rect, this, 1);
	dataList.SetExtendedStyle(dataList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	dataList.InsertColumn(0, "���", LVCFMT_CENTER, 200);
	dataList.InsertColumn(1, "���", LVCFMT_CENTER, 200);
	dataList.InsertColumn(2, "���ᷢɢ��", LVCFMT_CENTER, 200);
	dataList.InsertColumn(3, "���ᷢɢ��", LVCFMT_CENTER, 200);
	dataList.DeleteColumn(0);
	//dataList.InsertColumn(2, "ƽ��ֵ", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(3, "���ֵ", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(4, "��Сֵ", LVCFMT_CENTER, 80);

	dataList.InsertItem(1, "1");
	dataList.InsertItem(2, "2");
	dataList.InsertItem(3, "3");
	dataList.InsertItem(4, "4");
	dataList.InsertItem(5, "5");
	dataList.InsertItem(6, "6");
	dataList.InsertItem(7, "7");
	dataList.InsertItem(8, "8");
	dataList.InsertItem(9, "9");
	dataList.InsertItem(10, "10");
	dataList.InsertItem(11, "��ֵ");

	dataList.ShowWindow(SW_RESTORE);

	

	return 0;
}


void DataSum::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDockablePane::OnPaint()
	ReleaseDC(&dc);
}


void DataSum::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	//InvalidateRect(NULL, true);   //���ԭ�б�   
	dataList.MoveWindow(rect, true);
	//dataList.ShowWindow(SW_SHOW);
	//InvalidateRect(NULL, true);
}



