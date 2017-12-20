#pragma once

class CEllipse
{
public:
	//中心横坐标
	double xc;
	//中心纵坐标
	double yc;
	//长轴
	double xl;
	//短轴
	double yl;
	//偏转角度
	double ang;

	bool flag;

public:
	CEllipse()
		: xc(0)
		,yc(0)
		,xl(0)
		,yl(0)
		,ang(0)
		,flag(false)
	{

	}

	~CEllipse()
	{

	}

	CEllipse& operator +=(const CEllipse & elp)
	{
		xc += elp.xc;
		yc += elp.yc;
		xl += elp.xl;
		yl += elp.yl;
		ang+= elp.ang;
		return *this;
	}

	CEllipse& operator /=(double d)
	{
		if (d>0)
		{
			xc /= d;
			yc /= d;
			xl /= d;
			yl /= d;
			ang /= d;
		}
		return *this;
	}

	void clear()
	{
		xc = 0;
		yc = 0;
		xl = 0;
		yl = 0;
		ang = 0;
	}
};
