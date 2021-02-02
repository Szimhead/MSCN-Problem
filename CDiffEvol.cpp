#include "pch.h"
#include "CDiffEvol.h"
#include "CRandomSearch.h"
#include "CRandom.h"
#include <iostream>

using namespace std;

CDiffEvol::CDiffEvol(CProblem* pProblem)
{
	pc_problem = pProblem;
}//CDiffEvol::CDiffEvol(CMscnProblem* pProblem)

CDiffEvol::~CDiffEvol()
{
	//delete t_population;
}//CDiffEvol::~CDiffEvol()

double* CDiffEvol::dtFindBestSolution(double dSeconds)
{
	CRandom c_rand;
	CIndividual* cBaseInd;
	CIndividual* cAddInd0;
	CIndividual* cAddInd1;
	
	t_population = new CIndividual*[POPULATION];
	for (int ii = 0; ii < POPULATION; ii++)
	{
		//cout << "buduje populacje: " << ii << endl;
		t_population[ii] = new CIndividual(pc_problem,c_rand);
	}
	double* pd_best_solution = t_population[0]->dGenotype();
	int i_checks = 0;
	c_time.vStartTime();
	while (c_time.dGetTime() < dSeconds)
	{
		for (int ii = 0; ii < POPULATION; ii++)
		{
			int i_ind1 = c_rand.iRandomInt(0, POPULATION), i_ind2 = c_rand.iRandomInt(0, POPULATION),i_ind3= c_rand.iRandomInt(0, POPULATION);

			if (ii!=i_ind1 && ii!=i_ind2 && ii!=i_ind3 && i_ind1!=i_ind2 && i_ind1!=i_ind3 && i_ind2!=i_ind3)
			{
				cBaseInd = t_population[i_ind1];
				cAddInd0 = t_population[i_ind2];
				cAddInd1 = t_population[i_ind3];
				CIndividual* cIndNew=new CIndividual(pc_problem, (*t_population[ii]).iGenSize());
				(*cIndNew).vChangeGen(0, cBaseInd->dGetGen(0));
				(*cIndNew).vChangeGen(1, cBaseInd->dGetGen(1));
				(*cIndNew).vChangeGen(2, cBaseInd->dGetGen(2));
				(*cIndNew).vChangeGen(3, cBaseInd->dGetGen(3));
				for (int i_gene_offset = 4; i_gene_offset < (*t_population[ii]).iGenSize(); i_gene_offset++)
				{
					
					if (c_rand.dRandomDouble(0, 1) < CROSS_PROB)
					{
						(*cIndNew).vChangeGen(i_gene_offset, (*cBaseInd).dGetGen(i_gene_offset) + DIFFWEIGHT * ((*cAddInd0).dGetGen(i_gene_offset) - (*cAddInd1).dGetGen(i_gene_offset)));
					}//if (c_rand.dRandomDouble(0, 1) < CROSS_PROB)
					else
					{
						(*cIndNew).vChangeGen(i_gene_offset, (*t_population[ii]).dGetGen(i_gene_offset));
					}//else
					
				}//for (int i_gene_offset = 4; i_gene_offset < (*t_population[ii]).iGenSize(); i_gene_offset++)
				cIndNew->vFixIndividual();
				pc_problem->iSetSolution((*cIndNew).dGenotype());
		//		cout << "nowy cIndNew: " << pc_problem->bConstraintsSatisfied() << endl;
				double d_new_qual = pc_problem->dGetQuality();
				pc_problem->iSetSolution((*t_population[ii]).dGenotype());
		//		cout << "stary osobnik: " << pc_problem->bConstraintsSatisfied() << endl;

				double d_previous_qual = pc_problem->dGetQuality();

				if (d_new_qual > d_previous_qual)
				{
					delete t_population[ii];
					t_population[ii] = cIndNew;
					
				}//if (d_new_qual > d_previous_qual)
			}//if (ii!=i_ind1 && ii!=i_ind2 && ii!=i_ind3 && i_ind1!=i_ind2 && i_ind1!=i_ind3 && i_ind2!=i_ind3)
		}//for (int ii = 0; ii < POPULATION; ii++)
		c_time.vStopTime();
	}//while (i_checks < NUMBER_OF_CHECKS)

	pc_problem->iSetSolution((*t_population[0]).dGenotype());
	double d_best_qual = pc_problem->dGetQuality();
	double d_cur_qual;
	for (int ii = 1; ii < POPULATION; ii++)
	{
		pc_problem->iSetSolution((*t_population[ii]).dGenotype());
		d_cur_qual = pc_problem->dGetQuality();
		if (d_cur_qual > d_best_qual)
		{
			d_best_qual = d_cur_qual;
			pd_best_solution = (*t_population[ii]).dGenotype();
		}
	}
	return pd_best_solution;
}//void CDiffEvol::vFindSolution()

void CDiffEvol::vShowPopulation()
{
	for (int ii = 0; ii < POPULATION; ii++)
	{
		pc_problem->iSetSolution((*t_population[ii]).dGenotype());
			cout << ii << ". quality = " << pc_problem->dGetQuality() << endl << "safisfaction = " << pc_problem->bConstraintsSatisfied() << endl;
			for (int ij = 0; ij < t_population[ii]->iGenSize(); ij++)
			{
				cout << t_population[ii]->dGetGen(ij) << " ";
			}//for (int ij = 0; ij < t_population[ii]->iGenSize(); ij++)
			cout << endl;
	}//for (int ii = 0; ii < POPULATION; ii++)
}//void CDiffEvol::vShowPopulation()