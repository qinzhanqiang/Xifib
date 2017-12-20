#pragma once
#include <vector>

class CMyLine
{
public:
	double K;
	double B;

	CMyLine(double k = 0, double b = 0)
		:K(k),
		 B(b)
	{

	}

	~CMyLine()
	{

	}
};

class CLineFit
{
public:
	CLineFit();
	~CLineFit();
	void lineFit(const std::vector<double> &XVec, const std::vector<double> &YVec, CMyLine &lin);
private:
	void getLine(double * sum, int n, CMyLine &lin);
};