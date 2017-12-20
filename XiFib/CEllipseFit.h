/*************************************************************************
	�汾��     2014-12-31
	����˵���� ��ƽ���ϵ�һЩ�е������С���˵���Բ��ϣ���������ֵ�ֽⷨ
	           �����С���˽���Ϊ��Բ������
	������ʽ�� cvFitEllipse2f��arrayx,arrayy,box����    
	����˵���� arrayx: arrayx[n],ÿ��ֵΪx��һ����
	           arrayx: arrayy[n],ÿ��ֵΪy��һ����
			   n     : ��ĸ���
			   box   : box[5],��Բ������������ֱ�Ϊcenter.x,center.y,2a,2b,xtheta
			   esp: �⾫�ȣ�ͨ��ȡ1e-6,����ǽⷽ���õ�˵
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