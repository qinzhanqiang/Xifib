#include "stdafx.h"
#include "LMGaussFit.h"

/*
void expfunc(double *p, double *x, int m, int n, void *data)

{

	register int i;

	struct xtradata *dat;

	dat=(struct xtradata *)data;

	//float * PDataY = dat->PdataY;
	double * PDataX = dat->PDataX;

	for(i=0; i<n; ++i)
	{

		x[i]=p[0]*exp(-p[1]*PDataX[i]*PDataX[i]) + p[2];

	}

}

void jacexpfunc(double *p, double *jac, int m, int n, void *data)
{   

	register int i, j;

	struct xtradata *dat;

	dat=(struct xtradata *)data;
	double *PDataX = dat->PDataX;

	for(i=j=0; i<n; ++i){

		jac[j++]=exp(-p[1]*PDataX[i]*PDataX[i]);

		jac[j++]=-p[0]*PDataX[i]*PDataX[i]*exp(-p[1]*PDataX[i]*PDataX[i]);

		jac[j++]=1.0;

	}

}

void  gaussfit( std::vector<double> &vecX, std::vector<double> &vecY, const double &epsA, const double &epsL, CGaussLine &gaussLine)  
{
	if (vecX.size()!= vecY.size())
	{
		return;
	}

	int n=vecX.size();
	const int m=3; 
	double p[m], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	//register int i;

	int ret;

	struct xtradata data; 
	data.PDataX = &vecX[0];
	double * y = &vecY[0];

	p[0]=epsA; p[1]=log(2.0)/(epsL*epsL); p[2]=4;  //初始向量

	opts[0]=LM_INIT_MU; opts[1]=1; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; 

	//strcpy(data.msg, "Hello there!");

	ret=dlevmar_der(expfunc, jacexpfunc, p, y, m, n, 1000, opts, info, NULL, NULL, (void *)&data); // with analytic Jacobian

	//ret=dlevmar_dif(expfunc, p, y, m, n, 1000, opts, info, NULL, NULL, (void *)&data); // without Jacobian

	printf("Levenberg-Marquardt returned in %g iter, reason %g, sumsq %g [%g]\n", info[5], info[6], info[1], info[0]);

	printf("Best fit parameters: %.7g %.7g %.7g\n", p[0], p[1], p[2]);
	gaussLine.A = p[0];
	if (p[1]<0)
	{
		gaussLine.Q =0;
	}
	gaussLine.Q = 1/sqrt(p[1]);
	gaussLine.H = p[2];
}
*/


void expfunc(double *p, double *x, int m, int n, void *data)

{

	register int i;

	struct xtradata *dat;

	dat=(struct xtradata *)data;

	//float * PDataY = dat->PdataY;
	double * PDataX = dat->PDataX;

	for(i=0; i<n; ++i)
	{

		x[i]=p[0]*exp(-(PDataX[i]*PDataX[i])/((p[1]*p[1]))) + p[2];

	}

}

void jacexpfunc(double *p, double *jac, int m, int n, void *data)
{   

	register int i, j;

	struct xtradata *dat;

	dat=(struct xtradata *)data;
	double *PDataX = dat->PDataX;

	double tem = 0;

	for(i=j=0; i<n; ++i){

		tem = exp(-(PDataX[i]*PDataX[i])/((p[1]*p[1])));

		jac[j++]=tem;

		jac[j++]=2*p[0]*PDataX[i]*PDataX[i]*tem/p[1]/p[1]/p[1];

		jac[j++]=1.0;

	}

}

void  gaussfit( std::vector<double> &vecX, std::vector<double> &vecY, const double &epsA, const double &epsL, CGaussLine &gaussLine)  
{
	if (vecX.size()!= vecY.size())
	{
		return;
	}

	int n=vecX.size();
	const int m=3; 
	double p[m], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
	//register int i;

	int ret;

	struct xtradata data; 
	data.PDataX = &vecX[0];
	double * y = &vecY[0];

	p[0]=epsA; p[1]=epsL/sqrt(log(2.0))/2; p[2]=0;  //初始向量

	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]=LM_DIFF_DELTA; 

	//strcpy(data.msg, "Hello there!");

	ret=dlevmar_der(expfunc, jacexpfunc, p, y, m, n, 1000, opts, info, NULL, NULL, (void *)&data); // with analytic Jacobian

	//ret=dlevmar_dif(expfunc, p, y, m, n, 1000, opts, info, NULL, NULL, (void *)&data); // without Jacobian

	//printf("Levenberg-Marquardt returned in %g iter, reason %g, sumsq %g [%g]\n", info[5], info[6], info[1], info[0]);

	//printf("Best fit parameters: %.7g %.7g %.7g\n", p[0], p[1], p[2]);
	gaussLine.A = p[0];
	if (p[1]<0)
	{
		gaussLine.Q =0;
	}
	gaussLine.Q = p[1];
	gaussLine.H = p[2];
}