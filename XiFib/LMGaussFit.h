////////////////////////////////////////////////////////////////////////////////////

//  Copyright (C) 2008-11  Manolis Lourakis (lourakis at ics forth gr)

//  Institute of Computer Science, Foundation for Research & Technology - Hellas

//  Heraklion, Crete, Greece.

//

//  This program is free software; you can redistribute it and/or modify

//  it under the terms of the GNU General Public License as published by

//  the Free Software Foundation; either version 2 of the License, or

//  (at your option) any later version.

//

//  This program is distributed in the hope that it will be useful,

//  but WITHOUT ANY WARRANTY; without even the implied warranty of

//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

//  GNU General Public License for more details.

//

////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "levmar.h"
#include <vector>
#include "CGaussLine.h"
#include "CEllipse.h"

#ifdef _cplusplus

extern "C"{ 

#include "f2c.h"
	i
#include "clapack.h"

}

#endif

#pragma comment(lib,"libf2cd.lib")

#pragma comment(lib,"BLASd.lib")

#pragma comment(lib,"clapackd.lib")

#pragma comment(lib,"tmglibd.lib")

#pragma comment(lib,"levmar.lib")



#ifndef LM_DBL_PREC

#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!

#endif

#define M_PI 3.1415926

#ifdef _MSC_VER // MSVC

#include <process.h>

#define GETPID  _getpid

#elif defined(__GNUC__) // GCC

#include <sys/types.h>

#include <unistd.h>

#define GETPID  getpid

#else

#warning Do not know the name of the function returning the process id for your OS/compiler combination

#define GETPID  0

#endif /* _MSC_VER */

struct xtradata{

	char msg[128];
	double *PDataX;
	int n;

	xtradata()
		:PDataX(NULL)
		,n(0)
	{

	}

	~xtradata()
	{

	}
};

void expfunc(double *p, double *x, int m, int n, void *data);
void jacexpfunc(double *p, double *jac, int m, int n, void *data);
void  gaussfit( std::vector<double> &vecX, std::vector<double> &vecY, 
	const double &epsA, const double &epsL, CGaussLine &gaussLine);