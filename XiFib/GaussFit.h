/************************************************************************* 
    �汾��     2014-1-06 
    ����˵���� ��ƽ���ϵ�һЩ�е������С���˵ĸ�˹��ϣ���������ֵ�ֽⷨ 
               �����С���˽���Ϊ��˹������ 
    ������ʽ�� gaussfit( arrayx, arrayy,int n,float box,miny );��     
    ����˵���� arrayx: arrayx[n],ÿ��ֵΪx��һ���� 
               arrayx: arrayy[n],ÿ��ֵΪy��һ���� 
               n     : ��ĸ��� 
               box   : box[3],��˹������3���������ֱ�Ϊa,b,c; 
               miny  :y�����ϵ�ƽ�ƣ�ʵ����ϵĺ���Ϊy=a*exp(-((x-b)/c)^2)+miny 
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