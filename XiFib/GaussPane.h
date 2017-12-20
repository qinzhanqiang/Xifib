#pragma once
#include "CGaussLine.h"
#include <cmath>
// GaussPane

class GaussPane : public CDockablePane
{
	DECLARE_DYNAMIC(GaussPane)

public:
	GaussPane();
	virtual ~GaussPane();

//成员特性
public:
	CGaussLine gaussLine;
	CPoint P0;
	double lineL; //坐标轴长度
	double unitL;  //坐标取值范围
	double scaleY;  //最大值
	double scaleX; //缩放比例，实际长度：坐标长度
protected:
	DECLARE_MESSAGE_MAP()
public:
	int round(double d);
	void drawGaussLine(const CGaussLine &gaussLine, CPaintDC &dc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};


