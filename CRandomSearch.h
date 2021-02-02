#pragma once
#include "pch.h"
#include "COptimizer.h"
#include "CProblem.h"
#include <iostream>

using namespace std; 

#define LOWEST_QUAL -1000
#define ITERATIONS 100

class CRandomSearch : public COptimizer
 {
 public:
	 CRandomSearch(CProblem* pProblem);
	 CRandomSearch() { i_number_of_elements = 0;}

	 void vSetProblem(double* dParams)override { (*pc_problem).vSetParameters(dParams); }

	double* dtFindBestSolution(double iSeconds) override;

	int iGetNumberOfElements();
	
 private:
	 int i_number_of_elements;
};//class CRandomSearch


