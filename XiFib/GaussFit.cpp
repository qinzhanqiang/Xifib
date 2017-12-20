#include "stdafx.h"
#include "GaussFit.h"  
#include <cmath>  
#include<queue>  
#include<vector> 

using namespace std;

GaussFit::GaussFit(void)  
{  
}  
  
  
GaussFit::~GaussFit(void)  
{  
}  
  
 
void  GaussFit::gaussfit(  int *arrayx, int *arrayy, int n, CGaussLine &gaussLine)  
{     
	float *A1=new float[n*3];  
	float *B1=new float[n];  
	float *Pointx=new float[n];  
	float *Pointy=new float[n];  
	int maxy=0,midx,minx;  
	gaussLine.H = INT_MAX;  
	const double min_eps = 1e-10;  
	int i;  
	priority_queue<GPOINT> gp;            //用来对point.x排序  
	priority_queue<int>py;                //用来计算第m小的point.y  
	int m=n/10;  
	m=m>0?m:1;  
	for(i=0;i<n;i++)  
	{  
		GPOINT tmp(arrayx[i],arrayy[i]);  
		gp.push(tmp);  
		if(py.size()<m)  
		{  
			py.push(arrayy[i]);  
		}  
		else if(py.top()>arrayy[i])  
		{  
			py.pop();  
			py.push(arrayy[i]);  
		}  
	}  
	gaussLine.H = py.top();         //用第m小的y代替最小的y，防止异常点  
	minx=gp.top().x;          
	for( i=0;i<n;i++)  
	{  
		GPOINT tmp=gp.top();  
		gp.pop();  
		Pointx[i]=(tmp.x-minx)*1.0;  
		Pointy[i]=(tmp.y-gaussLine.H)*1.0;  
		if(Pointy[i]>maxy)  
		{  
			maxy=Pointy[i];  
			midx=i;  
		}  
		else if (Pointy[i]<0)  
		{  
			Pointy[i]=0;  
		}  
	}  
	float meany=0;  
	for(int i=0;i<n;i++)  
	{  
		meany+=Pointy[i];  
	}  
	meany/=n;  

	//统计峰值  
	vector<GPOINT>VG,Vtmp;  
	for(int i=0;i<n;i++)  
	{  
		if(Pointy[i]>meany)  
		{  
			GPOINT tmp(Pointx[i],Pointy[i]);  
			Vtmp.push_back(tmp);  
		}  
		else  
		{  
			int s1=VG.size(),s2=Vtmp.size();  
			if(s1<s2)  
			{  
				VG.clear();  
				for(int j=0;j<Vtmp.size();j++)  
				{  
					GPOINT tmp1(Vtmp[j].x,Vtmp[j].y);  
					VG.push_back(tmp1);  
				}  
			}  
			Vtmp.clear();  
		}  
	}  
	int s1=VG.size(),s2=Vtmp.size();  
	if(s1<s2)  
	{  
		VG.clear();  
		for(int j=0;j<Vtmp.size();j++)  
		{  
			GPOINT tmp1(Vtmp[j].x,Vtmp[j].y);  
			VG.push_back(tmp1);  
		}  
	}  

	//对峰值进行高斯拟合  
	int size=VG.size();  
	if(size>0)  
	{  
		for( i = 0; i < n; i++ )  
		{  
			int step=(i)*3;  
			float px,py;  
			px = VG[i%size].x;  
			py = VG[i%size].y;  
			B1[i] = log(py);  
			A1[step] = 1.0;  
			A1[step + 1] = px;  
			A1[step + 2] = px * px;  
		}  
		float *x1=new float[3];  

		SVD(A1,n,3,B1,x1,min_eps);  
		if (x1[2]<0)  
		{  
			gaussLine.Q=sqrt(-1.0/x1[2]);  
			gaussLine.U=x1[1]*gaussLine.Q*gaussLine.Q*0.5;  
			gaussLine.A=exp(x1[0]+gaussLine.U*gaussLine.U/(gaussLine.Q*gaussLine.Q));  
			gaussLine.U+=minx;  
		}  
		else  
		{  
			gaussLine.A=gaussLine.U=gaussLine.Q=-1;  
		}  
		delete []x1;  
	}  
	else  
	{  
		gaussLine.A=gaussLine.U=gaussLine.Q=-1;  
	}  
	delete []A1;  
	delete []B1;  
	delete []Pointx;  
	delete []Pointy;  
}

//void  GaussFit::gaussfit(  int *arrayx, int *arrayy, int n, float *box, int &miny )  
//{     
//    float *A1=new float[n*3];  
//    float *B1=new float[n];  
//    float *Pointx=new float[n];  
//    float *Pointy=new float[n];  
//    int maxy=0,midx,minx;  
//    miny=INT_MAX;  
//    const double min_eps = 1e-10;  
//    int i;  
//    priority_queue<GPOINT> gp;            //用来对point.x排序  
//    priority_queue<int>py;                //用来计算第m小的point.y  
//    int m=n/10;  
//    m=m>0?m:1;  
//    for(i=0;i<n;i++)  
//    {  
//        GPOINT tmp(arrayx[i],arrayy[i]);  
//        gp.push(tmp);  
//        if(py.size()<m)  
//        {  
//            py.push(arrayy[i]);  
//        }  
//        else if(py.top()>arrayy[i])  
//        {  
//            py.pop();  
//            py.push(arrayy[i]);  
//        }  
//    }  
//    miny=py.top();         //用第m小的y代替最小的y，防止异常点  
//    minx=gp.top().x;          
//    for( i=0;i<n;i++)  
//    {  
//        GPOINT tmp=gp.top();  
//        gp.pop();  
//        Pointx[i]=(tmp.x-minx)*1.0;  
//        Pointy[i]=(tmp.y-miny)*1.0;  
//        if(Pointy[i]>maxy)  
//        {  
//            maxy=Pointy[i];  
//            midx=i;  
//        }  
//        else if (Pointy[i]<0)  
//        {  
//            Pointy[i]=0;  
//        }  
//    }  
//    float meany=0;  
//    for(int i=0;i<n;i++)  
//    {  
//        meany+=Pointy[i];  
//    }  
//    meany/=n;  
//  
//    //统计峰值  
//    vector<GPOINT>VG,Vtmp;  
//    for(int i=0;i<n;i++)  
//    {  
//        if(Pointy[i]>meany)  
//        {  
//            GPOINT tmp(Pointx[i],Pointy[i]);  
//            Vtmp.push_back(tmp);  
//        }  
//        else  
//        {  
//            int s1=VG.size(),s2=Vtmp.size();  
//            if(s1<s2)  
//            {  
//                VG.clear();  
//                for(int j=0;j<Vtmp.size();j++)  
//                {  
//                    GPOINT tmp1(Vtmp[j].x,Vtmp[j].y);  
//                    VG.push_back(tmp1);  
//                }  
//            }  
//            Vtmp.clear();  
//        }  
//    }  
//   int s1=VG.size(),s2=Vtmp.size();  
//    if(s1<s2)  
//    {  
//        VG.clear();  
//        for(int j=0;j<Vtmp.size();j++)  
//        {  
//            GPOINT tmp1(Vtmp[j].x,Vtmp[j].y);  
//            VG.push_back(tmp1);  
//        }  
//    }  
//  
//    //对峰值进行高斯拟合  
//    int size=VG.size();  
//    if(size>0)  
//    {  
//         for( i = 0; i < n; i++ )  
//        {  
//            int step=(i)*3;  
//            float px,py;  
//            px = VG[i%size].x;  
//            py = VG[i%size].y;  
//            B1[i] = log(py);  
//            A1[step] = 1.0;  
//            A1[step + 1] = px;  
//            A1[step + 2] = px * px;  
//        }  
//        float *x1=new float[3];  
//  
//        SVD(A1,n,3,B1,x1,min_eps);  
//        if (x1[2]<0)  
//        {  
//            box[2]=sqrt(-1.0/x1[2]);  
//            box[1]=x1[1]*box[2]*box[2]*0.5;  
//            box[0]=exp(x1[0]+box[1]*box[1]/(box[2]*box[2]));  
//            box[1]+=minx;  
//        }  
//        else  
//        {  
//            box[0]=box[1]=box[2]=-1;  
//        }  
//        delete []x1;  
//    }  
//    else  
//    {  
//        box[0]=box[1]=box[2]=-1;  
//    }  
//    delete []A1;  
//    delete []B1;  
//    delete []Pointx;  
//    delete []Pointy;  
//}  
  
int GaussFit::SVD(float *a,int m,int n,float b[],float x[],float esp)  
{    
    float *aa;  
    float *u;  
    float *v;  
    aa=new float[n*m];  
    u=new  float[m*m];  
    v=new  float[n*n];  
     
   int ka;  
   int  flag;  
   if(m>n)  
   {   
    ka=m+1;  
   }else  
   {  
       ka=n+1;  
   }  
     
   flag=gmiv(a,m,n,b,x,aa,esp,u,v,ka);  
     
      
      
    delete []aa;  
    delete []u;  
    delete []v;  
      
    return(flag);  
}  
  
  
  
  
  
int GaussFit::gmiv( float a[],int m,int n,float b[],float x[],float aa[],float eps,float u[],float v[],int ka)    
{   
    int i,j;  
    i=ginv(a,m,n,aa,eps,u,v,ka);  
  
    if (i<0) return(-1);  
    for (i=0; i<=n-1; i++)  
      { x[i]=0.0;  
        for (j=0; j<=m-1; j++)  
          x[i]=x[i]+aa[i*m+j]*b[j];  
      }  
    return(1);  
  }  
  
  
int GaussFit::ginv(float a[],int m,int n,float aa[],float eps,float u[],float v[],int ka)  
  {   
  
 //  int muav(float a[],int m,int n,float u[],float v[],float eps,int ka);  
      
    int i,j,k,l,t,p,q,f;  
    i=muav(a,m,n,u,v,eps,ka);  
    if (i<0) return(-1);  
    j=n;  
    if (m<n) j=m;  
    j=j-1;  
    k=0;  
    while ((k<=j)&&(a[k*n+k]!=0.0)) k=k+1;  
    k=k-1;  
    for (i=0; i<=n-1; i++)  
    for (j=0; j<=m-1; j++)  
      { t=i*m+j; aa[t]=0.0;  
        for (l=0; l<=k; l++)  
          { f=l*n+i; p=j*m+l; q=l*n+l;  
            aa[t]=aa[t]+v[f]*u[p]/a[q];  
          }  
      }  
    return(1);  
  }  
  
  
  
  
  
  
int GaussFit::muav(float a[],int m,int n,float u[],float v[],float eps,int ka)  
  { int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;  
    float d,dd,t,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];  
    float *s,*e,*w;  
    //void pppGaussFit();  
   // void sssGaussFit();  
     void pppGaussFit(float a[],float e[],float s[],float v[],int m,int n);  
     void sssGaussFit(float fg[],float cs[]);  
  
    s=(float *) malloc(ka*sizeof(float));  
    e=(float *) malloc(ka*sizeof(float));  
    w=(float *) malloc(ka*sizeof(float));  
    it=60; k=n;  
    if (m-1<n) k=m-1;  
    l=m;  
    if (n-2<m) l=n-2;  
    if (l<0) l=0;  
    ll=k;  
    if (l>k) ll=l;  
    if (ll>=1)  
      { for (kk=1; kk<=ll; kk++)  
          { if (kk<=k)  
              { d=0.0;  
                for (i=kk; i<=m; i++)  
                  { ix=(i-1)*n+kk-1; d=d+a[ix]*a[ix];}  
                s[kk-1]=(float)sqrt(d);  
                if (s[kk-1]!=0.0)  
                  { ix=(kk-1)*n+kk-1;  
                    if (a[ix]!=0.0)  
                      { s[kk-1]=(float)fabs(s[kk-1]);  
                        if (a[ix]<0.0) s[kk-1]=-s[kk-1];  
                      }  
                    for (i=kk; i<=m; i++)  
                      { iy=(i-1)*n+kk-1;  
                        a[iy]=a[iy]/s[kk-1];  
                      }  
                    a[ix]=1.0f+a[ix];  
                  }  
                s[kk-1]=-s[kk-1];  
              }  
            if (n>=kk+1)  
              { for (j=kk+1; j<=n; j++)  
                  { if ((kk<=k)&&(s[kk-1]!=0.0))  
                      { d=0.0;  
                        for (i=kk; i<=m; i++)  
                          { ix=(i-1)*n+kk-1;  
                            iy=(i-1)*n+j-1;  
                            d=d+a[ix]*a[iy];  
                          }  
                        d=-d/a[(kk-1)*n+kk-1];  
                        for (i=kk; i<=m; i++)  
                          { ix=(i-1)*n+j-1;  
                            iy=(i-1)*n+kk-1;  
                            a[ix]=a[ix]+d*a[iy];  
                          }  
                      }  
                    e[j-1]=a[(kk-1)*n+j-1];  
                  }  
              }  
            if (kk<=k)  
              { for (i=kk; i<=m; i++)  
                  { ix=(i-1)*m+kk-1; iy=(i-1)*n+kk-1;  
                    u[ix]=a[iy];  
                  }  
              }  
            if (kk<=l)  
              { d=0.0;  
                for (i=kk+1; i<=n; i++)  
                  d=d+e[i-1]*e[i-1];  
                e[kk-1]=(float)sqrt(d);  
                if (e[kk-1]!=0.0)  
                  { if (e[kk]!=0.0)  
                      { e[kk-1]=(float)fabs(e[kk-1]);  
                        if (e[kk]<0.0) e[kk-1]=-e[kk-1];  
                      }  
                    for (i=kk+1; i<=n; i++)  
                      e[i-1]=e[i-1]/e[kk-1];  
                    e[kk]=1.0f+e[kk];  
                  }  
                e[kk-1]=-e[kk-1];  
                if ((kk+1<=m)&&(e[kk-1]!=0.0))  
                  { for (i=kk+1; i<=m; i++) w[i-1]=0.0;  
                    for (j=kk+1; j<=n; j++)  
                      for (i=kk+1; i<=m; i++)  
                        w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];  
                    for (j=kk+1; j<=n; j++)  
                      for (i=kk+1; i<=m; i++)  
                        { ix=(i-1)*n+j-1;  
                          a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];  
                        }  
                  }  
                for (i=kk+1; i<=n; i++)  
                  v[(i-1)*n+kk-1]=e[i-1];  
              }  
          }  
      }  
    mm=n;  
    if (m+1<n) mm=m+1;  
    if (k<n) s[k]=a[k*n+k];  
    if (m<mm) s[mm-1]=0.0;  
    if (l+1<mm) e[l]=a[l*n+mm-1];  
    e[mm-1]=0.0;  
    nn=m;  
    if (m>n) nn=n;  
    if (nn>=k+1)  
      { for (j=k+1; j<=nn; j++)  
          { for (i=1; i<=m; i++)  
              u[(i-1)*m+j-1]=0.0;  
            u[(j-1)*m+j-1]=1.0;  
          }  
      }  
    if (k>=1)  
      { for (ll=1; ll<=k; ll++)  
          { kk=k-ll+1; iz=(kk-1)*m+kk-1;  
            if (s[kk-1]!=0.0)  
              { if (nn>=kk+1)  
                  for (j=kk+1; j<=nn; j++)  
                    { d=0.0;  
                      for (i=kk; i<=m; i++)  
                        { ix=(i-1)*m+kk-1;  
                          iy=(i-1)*m+j-1;  
                          d=d+u[ix]*u[iy]/u[iz];  
                        }  
                      d=-d;  
                      for (i=kk; i<=m; i++)  
                        { ix=(i-1)*m+j-1;  
                          iy=(i-1)*m+kk-1;  
                          u[ix]=u[ix]+d*u[iy];  
                        }  
                    }  
                  for (i=kk; i<=m; i++)  
                    { ix=(i-1)*m+kk-1; u[ix]=-u[ix];}  
                  u[iz]=1.0f+u[iz];  
                  if (kk-1>=1)  
                    for (i=1; i<=kk-1; i++)  
                      u[(i-1)*m+kk-1]=0.0;  
              }  
            else  
              { for (i=1; i<=m; i++)  
                  u[(i-1)*m+kk-1]=0.0;  
                u[(kk-1)*m+kk-1]=1.0;  
              }  
          }  
      }  
    for (ll=1; ll<=n; ll++)  
      { kk=n-ll+1; iz=kk*n+kk-1;  
        if ((kk<=l)&&(e[kk-1]!=0.0))  
          { for (j=kk+1; j<=n; j++)  
              { d=0.0;  
                for (i=kk+1; i<=n; i++)  
                  { ix=(i-1)*n+kk-1; iy=(i-1)*n+j-1;  
                    d=d+v[ix]*v[iy]/v[iz];  
                  }  
                d=-d;  
                for (i=kk+1; i<=n; i++)  
                  { ix=(i-1)*n+j-1; iy=(i-1)*n+kk-1;  
                    v[ix]=v[ix]+d*v[iy];  
                  }  
              }  
          }  
        for (i=1; i<=n; i++)  
          v[(i-1)*n+kk-1]=0.0;  
        v[iz-n]=1.0;  
      }  
    for (i=1; i<=m; i++)  
    for (j=1; j<=n; j++)  
      a[(i-1)*n+j-1]=0.0;  
    m1=mm; it=60;  
    while (1==1)  
      { if (mm==0)  
          { pppGaussFit(a,e,s,v,m,n);  
            free(s); free(e); free(w); return(1);  
          }  
        if (it==0)  
          { pppGaussFit(a,e,s,v,m,n);  
            free(s); free(e); free(w); return(-1);  
          }  
        kk=mm-1;  
    while ((kk!=0)&&(fabs(e[kk-1])!=0.0))  
          { d=(float)(fabs(s[kk-1])+fabs(s[kk]));  
            dd=(float)fabs(e[kk-1]);  
            if (dd>eps*d) kk=kk-1;  
            else e[kk-1]=0.0;  
          }  
        if (kk==mm-1)  
          { kk=kk+1;  
            if (s[kk-1]<0.0)  
              { s[kk-1]=-s[kk-1];  
                for (i=1; i<=n; i++)  
                  { ix=(i-1)*n+kk-1; v[ix]=-v[ix];}  
              }  
            while ((kk!=m1)&&(s[kk-1]<s[kk]))  
              { d=s[kk-1]; s[kk-1]=s[kk]; s[kk]=d;  
                if (kk<n)  
                  for (i=1; i<=n; i++)  
                    { ix=(i-1)*n+kk-1; iy=(i-1)*n+kk;  
                      d=v[ix]; v[ix]=v[iy]; v[iy]=d;  
                    }  
                if (kk<m)  
                  for (i=1; i<=m; i++)  
                    { ix=(i-1)*m+kk-1; iy=(i-1)*m+kk;  
                      d=u[ix]; u[ix]=u[iy]; u[iy]=d;  
                    }  
                kk=kk+1;  
              }  
            it=60;  
            mm=mm-1;  
          }  
        else  
          { ks=mm;  
            while ((ks>kk)&&(fabs(s[ks-1])!=0.0))  
              { d=0.0;  
                if (ks!=mm) d=d+(float)fabs(e[ks-1]);  
                if (ks!=kk+1) d=d+(float)fabs(e[ks-2]);  
                dd=(float)fabs(s[ks-1]);  
                if (dd>eps*d) ks=ks-1;  
                else s[ks-1]=0.0;  
              }  
            if (ks==kk)  
              { kk=kk+1;  
                d=(float)fabs(s[mm-1]);  
                t=(float)fabs(s[mm-2]);  
                if (t>d) d=t;  
                t=(float)fabs(e[mm-2]);  
                if (t>d) d=t;  
                t=(float)fabs(s[kk-1]);  
                if (t>d) d=t;  
                t=(float)fabs(e[kk-1]);  
                if (t>d) d=t;  
                sm=s[mm-1]/d; sm1=s[mm-2]/d;  
                em1=e[mm-2]/d;  
                sk=s[kk-1]/d; ek=e[kk-1]/d;  
                b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0f;  
                c=sm*em1; c=c*c; shh=0.0;  
                if ((b!=0.0)||(c!=0.0))  
                  { shh=(float)sqrt(b*b+c);  
                    if (b<0.0) shh=-shh;  
                    shh=c/(b+shh);  
                  }  
                fg[0]=(sk+sm)*(sk-sm)-shh;  
                fg[1]=sk*ek;  
                for (i=kk; i<=mm-1; i++)  
                  { sssGaussFit(fg,cs);  
                    if (i!=kk) e[i-2]=fg[0];  
                    fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];  
                    e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];  
                    fg[1]=cs[1]*s[i];  
                    s[i]=cs[0]*s[i];  
                    if ((cs[0]!=1.0)||(cs[1]!=0.0))  
                      for (j=1; j<=n; j++)  
                        { ix=(j-1)*n+i-1;  
                          iy=(j-1)*n+i;  
                          d=cs[0]*v[ix]+cs[1]*v[iy];  
                          v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];  
                          v[ix]=d;  
                        }  
                    sssGaussFit(fg,cs);  
                    s[i-1]=fg[0];  
                    fg[0]=cs[0]*e[i-1]+cs[1]*s[i];  
                    s[i]=-cs[1]*e[i-1]+cs[0]*s[i];  
                    fg[1]=cs[1]*e[i];  
                    e[i]=cs[0]*e[i];  
                    if (i<m)  
                      if ((cs[0]!=1.0)||(cs[1]!=0.0))  
                        for (j=1; j<=m; j++)  
                          { ix=(j-1)*m+i-1;  
                            iy=(j-1)*m+i;  
                            d=cs[0]*u[ix]+cs[1]*u[iy];  
                            u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];  
                            u[ix]=d;  
                          }  
                  }  
                e[mm-2]=fg[0];  
                it=it-1;  
              }  
            else  
              { if (ks==mm)  
                  { kk=kk+1;  
                    fg[1]=e[mm-2]; e[mm-2]=0.0;  
                    for (ll=kk; ll<=mm-1; ll++)  
                      { i=mm+kk-ll-1;  
                        fg[0]=s[i-1];  
                        sssGaussFit(fg,cs);  
                        s[i-1]=fg[0];  
                        if (i!=kk)  
                          { fg[1]=-cs[1]*e[i-2];  
                            e[i-2]=cs[0]*e[i-2];  
                          }  
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))  
                          for (j=1; j<=n; j++)  
                            { ix=(j-1)*n+i-1;  
                              iy=(j-1)*n+mm-1;  
                              d=cs[0]*v[ix]+cs[1]*v[iy];  
                              v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];  
                              v[ix]=d;  
                            }  
                      }  
                  }  
                else  
                  { kk=ks+1;  
                    fg[1]=e[kk-2];  
                    e[kk-2]=0.0;  
                    for (i=kk; i<=mm; i++)  
                      { fg[0]=s[i-1];  
                        sssGaussFit(fg,cs);  
                        s[i-1]=fg[0];  
                        fg[1]=-cs[1]*e[i-1];  
                        e[i-1]=cs[0]*e[i-1];  
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))  
                          for (j=1; j<=m; j++)  
                            { ix=(j-1)*m+i-1;  
                              iy=(j-1)*m+kk-2;  
                              d=cs[0]*u[ix]+cs[1]*u[iy];  
                              u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];  
                              u[ix]=d;  
                            }  
                      }  
                  }  
              }  
          }  
      }  
     
    free(s);free(e);free(w);   
      return(1);  
  
  
  }  
  
   
void pppGaussFit(float a[],float e[],float s[],float v[],int m,int n)   
{ int i,j,p,q;  
    float d;  
    if (m>=n) i=n;  
    else i=m;  
    for (j=1; j<=i-1; j++)  
      { a[(j-1)*n+j-1]=s[j-1];  
        a[(j-1)*n+j]=e[j-1];  
      }  
    a[(i-1)*n+i-1]=s[i-1];  
    if (m<n) a[(i-1)*n+i]=e[i-1];  
    for (i=1; i<=n-1; i++)  
    for (j=i+1; j<=n; j++)  
      { p=(i-1)*n+j-1; q=(j-1)*n+i-1;  
        d=v[p]; v[p]=v[q]; v[q]=d;  
      }  
    return;  
  }  
  
   
  void sssGaussFit(float fg[],float cs[])  
 { float r,d;  
    if ((fabs(fg[0])+fabs(fg[1]))==0.0)  
      { cs[0]=1.0; cs[1]=0.0; d=0.0;}  
    else   
      { d=(float)sqrt(fg[0]*fg[0]+fg[1]*fg[1]);  
        if (fabs(fg[0])>fabs(fg[1]))  
          { d=(float)fabs(d);  
            if (fg[0]<0.0) d=-d;  
          }  
        if (fabs(fg[1])>=fabs(fg[0]))  
          { d=(float)fabs(d);  
            if (fg[1]<0.0) d=-d;  
          }  
        cs[0]=fg[0]/d; cs[1]=fg[1]/d;  
      }  
    r=1.0;  
    if (fabs(fg[0])>fabs(fg[1])) r=cs[1];  
    else  
      if (cs[0]!=0.0) r=1.0f/cs[0];  
    fg[0]=d; fg[1]=r;  
    return;  
  }  