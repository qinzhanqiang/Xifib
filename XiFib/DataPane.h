#pragma once


// DataPane

class DataPane : public CDockablePane
{
	DECLARE_DYNAMIC(DataPane)
public:
	CListCtrl dataList;
public:
	DataPane();
	virtual ~DataPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


