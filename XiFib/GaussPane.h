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

//��Ա����
public:
	CGaussLine gaussLine;
	CPoint P0;
	double lineL; //�����᳤��
	double unitL;  //����ȡֵ��Χ
	double scaleY;  //���ֵ
	double scaleX; //���ű�����ʵ�ʳ��ȣ����곤��
protected:
	DECLARE_MESSAGE_MAP()
public:
	int round(double d);
	void drawGaussLine(const CGaussLine &gaussLine, CPaintDC &dc);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};


