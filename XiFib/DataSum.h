#pragma once


// DataSum

class DataSum : public CDockablePane
{
	DECLARE_DYNAMIC(DataSum)

public:
	DataSum();
	CListCtrl dataList;

	virtual ~DataSum();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSaveData();
};


