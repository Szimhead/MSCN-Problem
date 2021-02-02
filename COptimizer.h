#pragma once
#include "pch.h"
#include "CTimer.h"
#include "CProblem.h"
#include <iostream>

using namespace std;

class COptimizer
{
public:
	virtual void vSetProblem(double* dParams) = 0;
	virtual double* dtFindBestSolution(double dSeconds) =0;

protected:
	CTimer c_time;
	CProblem* pc_problem;
};