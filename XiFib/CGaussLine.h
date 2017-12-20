#pragma once

class CGaussLine
{
public:
	//��ֵ
	double A;
	//����
	double U;
	//xitia
	double Q;
	//ƫ��
	double H;
	bool flag;

public:
	CGaussLine()
		:A(0)
		,U(0)
		,Q(0)
		,H(0)
		,flag(false)
	{

	}

	~CGaussLine()
	{

	}

	CGaussLine& operator += (const CGaussLine & gline)
	{
		A += gline.A;
		U += gline.U;
		Q += gline.Q;
		H += gline.H;

		return *this;
	}

	CGaussLine& operator /= (double d)
	{
		if (d>0)
		{
			A /= d;
			U /= d;
			Q /= d;
			H /= d;
		}
		return *this;
	}
	//�������
	void clear()
	{
		A = 0;
		U = 0;
		Q = 0;
		H = 0;
	}
};