#pragma once
#include "pch.h"
#include "CRandomSearch.h"
#include "CRandom.h"

CRandomSearch::CRandomSearch(CProblem* pProblem)
{
	pc_problem = pProblem;
	i_number_of_elements = pc_problem->iGetNumberOfElements();
	
}//CRandomSearch::CRandomSearch(CMscnProblem* pProblem)


double* CRandomSearch::dtFindBestSolution(double iSeconds)
{
	CRandom c_random;
	int d_best_quality = LOWEST_QUAL;
	double* pd_best_solution = new double[i_number_of_elements];
	double* pd_current_solution;
	double d_cur_q;
	int ii = 0;
	c_time.vStartTime();
	while(c_time.dGetTime()<iSeconds)
	{
		pd_current_solution = pc_problem->dGenerateSolution(c_random);
	
		pc_problem->iSetSolution(pd_current_solution);
		if ((d_cur_q = pc_problem->dGetQuality()) > d_best_quality)
		{
			d_best_quality = d_cur_q;
			cout << ii << ". quality = " << d_cur_q << ", satisfied = " << pc_problem->bConstraintsSatisfied() << endl;
			for (int i = 0; i < 10; i++)
					cout << pd_current_solution[i] << " ";
				cout << endl;
			pd_best_solution = pd_current_solution;
		}//if ((d_cur_q = pc_problem->dGetQuality()) > d_best_quality)
		else
			delete pd_current_solution;
		ii++;
		//if (pc_problem->bConstraintsSatisfied() == 1)
			//cout << ii ++<< ". quality = " << d_cur_q << ", satisfied = " << pc_problem->bConstraintsSatisfied() << endl;
		//pc_problem->vShowMatrixes();
		c_time.vStopTime();
	}
	return pd_best_solution;
}//double* CRandomSearch::dtBestSolutionIterations(int iSeconds)

int CRandomSearch::iGetNumberOfElements()
{
	return i_number_of_elements;
}//int CRandomSearch::iGetNumberOfElements()