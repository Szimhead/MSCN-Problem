#pragma once
#include "pch.h"
#include "CMscnProblem.h"
#include "CIndividual.h"
#include "COptimizer.h"
#include <iostream>

using namespace std;	

#define POPULATION 50
#define NUMBER_OF_CHECKS 100
#define CROSS_PROB 0.6
#define DIFFWEIGHT 1.0

class CDiffEvol :public COptimizer
{
public:
	CDiffEvol(CProblem* pProblem);
	~CDiffEvol();

	void vSetProblem(double* dParams)override { (*pc_problem).vSetParameters(dParams);  }	

	double* dtFindBestSolution(double dSeconds) override;

	CIndividual* cGetPopulation() { return *t_population; }
	void vShowPopulation();
private:
	CIndividual** t_population;
	
};//class CDiffEvol
