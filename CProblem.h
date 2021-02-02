#pragma once
#include "pch.h"
#include "CRandom.h"
#include <iostream>
using namespace std;

class CProblem
{
public:
	virtual double dGetQuality() =0;
	virtual bool bConstraintsSatisfied() =0;

	virtual void vSetParameters(double* dParameters) =0;
	virtual int iSetSolution(double* dSolution) = 0;

	virtual void vFixSolution(double* pdSol) = 0;

	virtual double* dGenerateSolution(CRandom c_rand) = 0;
	virtual int iGetNumberOfElements() = 0;
};