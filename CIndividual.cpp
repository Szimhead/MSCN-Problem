#include "pch.h"
#include "CIndividual.h"

using namespace std;

CIndividual::CIndividual()
{
	i_gen_size = 0;
}//CIndividual::CIndividual()

CIndividual::~CIndividual()
{
	delete[] pd_genotype;
}//CIndividual::~CIndividual()

CIndividual::CIndividual(CProblem* pProblem, CRandom c_rand)
{
	c_rand_search = CRandomSearch(pProblem);
	double* pd_temp_sol=nullptr;
	do
	{
		if (pd_temp_sol != nullptr)
			delete pd_temp_sol;
		pd_temp_sol = (*pProblem).dGenerateSolution(c_rand);
		pProblem->iSetSolution((pd_temp_sol));
	}while (!pProblem->bConstraintsSatisfied());
	i_gen_size = c_rand_search.iGetNumberOfElements();
	pd_genotype = pd_temp_sol;
	pc_problem = pProblem;
}//CIndividual::CIndividual(CMscnProblem* pProblem, CRandom c_rand)

CIndividual::CIndividual(CProblem* pProblem, int iGenSize)
{
	if (iGenSize < 0)
		cout << "Nieprawidlowy rozmiar\n";
	else
	{
		i_gen_size = iGenSize;
		pd_genotype = new double[iGenSize];
		pc_problem = pProblem;
	}//else
}//CIndividual::CIndividual(CMscnProblem* pProblem, int iGenSize)

void CIndividual::vChangeGen(int iGenNumber, double dNewGen)
{
	if (iGenNumber<0 || iGenNumber>=i_gen_size)
		cout << "Nieprawidlowy index\n";
	else
	{
		pd_genotype[iGenNumber] = dNewGen;
	}//else
}//void CIndividual::vChangeGen(int iGenNumber, double dNewGen)

bool CIndividual::bCompareIndividuals(CIndividual cOther)
{
	if (i_gen_size != cOther.i_gen_size)
		return false;
	else
	{
		for (int ii = 0; ii < i_gen_size; ii++)
		{
			if (pd_genotype[ii] != cOther.pd_genotype[ii])
				return false;
		}//for (int ii = 0; ii < i_gen_size; ii++)
		return true;
	}//else
}//bool CIndividual::bCompareIndividuals(CIndividual cOther)

void CIndividual::vFixIndividual()
{
	(*pc_problem).vFixSolution(pd_genotype);
}//void CIndividual::vFixIndividual()