#include "stdafx.h"
#include "CLinePoint.h"

int CLinePoint::linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<int> &vx, std::vector<int> &vy, std::vector<int> &value)
{
	int i=0;
	cv::Point linepoint[2];
	double xielv = std::tan(ang);

	double x1 = p.x-p.y/xielv;
	if( x1>0 && x1<src.cols )
	{
		linepoint[i].x = x1; 
		linepoint[i].y = 0;
		++i;
	}

	double x2 = p.x-(p.y-src.rows)/xielv;
	if( x2>0 && x2<src.cols )
	{
		linepoint[i].x = x2;
		linepoint[i].y = src.rows;
		++i;
	}

	double y1 = p.y-p.x*xielv;
	if( i<2 && y1>0 && y1<src.rows )
	{
		linepoint[i].x = 0;
		linepoint[i].y = y1;
		++i;
	}

	double y2 = p.y-(p.x-src.cols)*xielv;
	if( i<2 && y2>0 && y2<src.rows )
	{
		linepoint[i].x = src.cols;
		linepoint[i].y = y2;
		//++i;
	}

	cv::LineIterator it(src, linepoint[0], linepoint[1], 8, true);

	for(int j = 0; j < it.count; j++, ++it)
	{
		int x = it.pos().x;
		vx.push_back(x);

		int y = it.pos().y;
		vy.push_back(y);

		unsigned short val = src.at<unsigned short>(it.pos());
		value.push_back(val);
	}
	auto k = it.count;
	return k;
}

//把长短轴上的点 和像素大小找出来
int CLinePoint::linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<int> &vx, std::vector<int> &value)
{
	int i=0;
	cv::Point linepoint[2];
	double xielv = std::tan(ang);

	double x1 = p.x-p.y/xielv;
	if( x1>0 && x1<src.cols )
	{
		linepoint[i].x = x1;
		linepoint[i].y = 0;
		++i;
	}

	double x2 = p.x-(p.y-src.rows)/xielv;
	if( x2>0 && x2<src.cols )
	{
		linepoint[i].x = x2;
		linepoint[i].y = src.rows;
		++i;
	}

	double y1 = p.y-p.x*xielv;
	if( i<2 && y1>0 && y1<src.rows )
	{
		linepoint[i].x = 0;
		linepoint[i].y = y1;
		++i;
	}

	double y2 = p.y-(p.x-src.cols)*xielv;
	if( i<2 && y2>0 && y2<src.rows )
	{
		linepoint[i].x = src.cols;
		linepoint[i].y = y2;
		//++i;
	}

	cv::LineIterator it(src, linepoint[0], linepoint[1], 8, true);

	for(int j = 0; j < it.count; j++, ++it)
	{
		int x = it.pos().x;
		int y = it.pos().y;
		double lx=0;

		if (x < p.x)
			lx = p.x - round(std::sqrt((double)(x*x + y*y)));
		else
			lx = p.x + round(std::sqrt((double)(x*x + y*y)));

		vx.push_back(lx);


		unsigned short val = src.at<unsigned short>(it.pos());
		value.push_back(val);
	}
	//cv::line(src, linepoint[0], linepoint[1], cv::Scalar(0), 1, 8, 0 );画出长短轴 
	auto k = it.count;
	return k;
}