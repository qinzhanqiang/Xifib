#pragma once

#include <vector>
#include "opencv2/imgproc/imgproc.hpp"

inline double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

class CLinePoint
{
  public:
	CLinePoint()
	{

	};
	
	static int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<int> &vx, std::vector<int> &vy, std::vector<int> &value);
	static int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<int> &vx, std::vector<int> &value);
	
	~CLinePoint()
	{

	};
};