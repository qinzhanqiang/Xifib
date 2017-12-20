/************************************************************************* 
    版本：     2014-1-06 
    功能说明： 对平面上的一些列点给出最小二乘的高斯拟合，利用奇异值分解法 
               解得最小二乘解作为高斯参数。 
    调用形式： gaussfit( arrayx, arrayy,int n,float box,miny );；     
    参数说明： arrayx: arrayx[n],每个值为x轴一个点 
               arrayx: arrayy[n],每个值为y轴一个点 
               n     : 点的个数 
               box   : box[3],高斯函数的3个参数，分别为a,b,c; 
               miny  :y方向上的平移，实际拟合的函数为y=a*exp(-((x-b)/c)^2)+miny 
***************************************************************************/  
#ifndef GAUSSFIT_H
#define GAUSSFIT_H

#pragma once  

#include "CgaussLine.h"

struct GPOINT  
{  
    int x;  
    int y;  
    GPOINT(int x,int y):x(x),y(y){};  
    friend bool operator <(GPOINT p1,GPOINT p2)  
    {  
        return p1.x>p2.x;  
    }  
  
};  
class GaussFit  
{  
public:  
    GaussFit(void);  
    ~GaussFit(void);  
    //void gaussfit( int *arrayx, int *arrayy,int n,float *box,int &miny );  
	void gaussfit( int *arrayx, int *arrayy, int n, CGaussLine &gaussLine);
private:  
    int SVD(float *a,int m,int n,float b[],float x[],float esp);  
    int gmiv(float a[],int m,int n,float b[],float x[],float aa[],float eps,float u[],float v[],int ka);  
    int ginv(float a[],int m,int n,float aa[],float eps,float u[],float v[],int ka);  
    int muav(float a[],int m,int n,float u[],float v[],float eps,int ka);  
}; 
#endif