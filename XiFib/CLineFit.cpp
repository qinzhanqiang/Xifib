#include "stdafx.h"
#include "CLineFit.h"

CLineFit::CLineFit()
{

}

CLineFit::~CLineFit()
{

}

void CLineFit::lineFit(const std::vector<double> &XVec, const std::vector<double> &YVec, CMyLine &lin)
{
	if (XVec.size()!=YVec.size())
	{
		return;
	}
	
	double sum[5] = {0, 0, 0, 0, 0};
	for(unsigned int i=0; i<XVec.size(); ++i)
	{
		sum[0] += XVec[i]*YVec[i];
		sum[1] += XVec[i]*XVec[i];
		sum[2] += XVec[i];
		sum[3] += YVec[i];
	}

	sum[4] =XVec.size();

	//double xy = 0, x2 = 0, xx = 0, yy = 0, num = XVec.size();
	//for(int i=0; i<XVec.size(); ++i)
	//{
	//	xy += XVec[i]*YVec[i];
	//	x2 += XVec[i]*XVec[i];
	//	xx += XVec[i];
	//	yy += YVec[i];
	//}
	getLine(sum, 5, lin);
}

void CLineFit::getLine(double * sum, int n, CMyLine &lin)
{
	if(n!=5 || NULL == sum)
		return;

	lin.B = (sum[0]*sum[2]-sum[1]*sum[3])/(sum[2]*sum[2]-sum[1]*sum[4]);
	lin.K = (sum[3]-lin.B*sum[4])/sum[2];

}