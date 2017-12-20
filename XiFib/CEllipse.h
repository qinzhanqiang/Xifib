#pragma once

class CEllipse
{
public:
	//���ĺ�����
	double xc;
	//����������
	double yc;
	//����
	double xl;
	//����
	double yl;
	//ƫת�Ƕ�
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
