// DataPane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XiFib.h"
#include "DataPane.h"


// DataPane

IMPLEMENT_DYNAMIC(DataPane, CDockablePane)

DataPane::DataPane()
{

}

DataPane::~DataPane()
{
}


BEGIN_MESSAGE_MAP(DataPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// DataPane ��Ϣ��������




void DataPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
	CRect rect;
	GetClientRect(rect);  
	//InvalidateRect(NULL, true);   //���ԭ�б�   
	dataList.MoveWindow(rect,true);  
	//dataList.ShowWindow(SW_SHOW);
	//InvalidateRect(NULL, true);
}


int DataPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	CRect rect;
	GetClientRect(&rect);
	dataList.Create(WS_VISIBLE|WS_BORDER|LVS_REPORT,rect,this,1);
	dataList.SetExtendedStyle(dataList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
	dataList.InsertColumn(0, "����", LVCFMT_CENTER, 80);
	dataList.InsertColumn(1, "��ǰֵ", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(2, "ƽ��ֵ", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(3, "���ֵ", LVCFMT_CENTER, 80);
	//dataList.InsertColumn(4, "��Сֵ", LVCFMT_CENTER, 80);
	
	dataList.InsertItem(1,"��ֵ");
	dataList.InsertItem(2,"X0");
	dataList.InsertItem(3,"Y0");
	dataList.InsertItem(4,"���᳤��");
	dataList.InsertItem(5,"���᳤��");
	dataList.InsertItem(6,"���ᷢɢ��");
	dataList.InsertItem(7,"���ᷢɢ��");
	dataList.InsertItem(8,"������֮��");
	dataList.InsertItem(9,"����Ƕ�");
	dataList.InsertItem(10,"ƫ���");
	dataList.InsertItem(11,"״̬");
	
	dataList.ShowWindow(SW_SHOWNORMAL);
	return 0;
}


void DataPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴�������Ϣ�����������
	// ��Ϊ��ͼ��Ϣ���� CDockablePane::OnPaint()
	ReleaseDC(&dc);
}