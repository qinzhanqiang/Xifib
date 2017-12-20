/*************************************************************************
	版本：     2014-12-31
	功能说明： 对平面上的一些列点给出最小二乘的椭圆拟合，利用奇异值分解法
	           解得最小二乘解作为椭圆参数。
	调用形式： cvFitEllipse2f（arrayx,arrayy,box）；    
	参数说明： arrayx: arrayx[n],每个值为x轴一个点
	           arrayx: arrayy[n],每个值为y轴一个点
			   n     : 点的个数
			   box   : box[5],椭圆的五个参数，分别为center.x,center.y,2a,2b,xtheta
			   esp: 解精度，通常取1e-6,这个是解方程用的说
***************************************************************************/

#ifndef LSELLIPSE_H 
#define LSELLIPSE_H

#include<cstdlib>
#include<float.h>
#include<vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "CEllipse.h"

//using namespace std;

class CEllipseFit
{
public:
	CEllipseFit(void);
	~CEllipseFit(void);
	void  CEllipseFit::getEllipse(const std::vector<int> &vec_X, const std::vector<int> &vec_Y, CEllipse &elp);
	void  CEllipseFit::cvFitEllipse(  int *arrayx, int *arrayy,int n, CEllipse &ellipse );

private:
	int SVD(float *a,int m,int n,float b[],float x[],float esp);
	int gmiv(float a[],int m,int n,float b[],float x[],float aa[],float eps,float u[],float v[],int ka);
    int ginv(float a[],int m,int n,float aa[],float eps,float u[],float v[],int ka);
	int muav(float a[],int m,int n,float u[],float v[],float eps,int ka);
};

#endif