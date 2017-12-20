#pragma once

#include "CEllipse.h"
#include <cmath>
// EllipsePane

class EllipsePane : public CDockablePane
{
	DECLARE_DYNAMIC(EllipsePane)

public:
	EllipsePane();
	virtual ~EllipsePane();

public:
	CEllipse Epse;
	CPoint P0;
	int lineL;
protected:
	DECLARE_MESSAGE_MAP()
public:
	int  round(double d);
	void drawEllipse(const CEllipse &e, const CPoint &p0, CPaintDC &dc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


