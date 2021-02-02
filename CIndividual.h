#pragma once
#include "pch.h"
#include "CRandomSearch.h"
#include "CRandom.h"
#include "CProblem.h"
#include <iostream>

using namespace std;

class CIndividual
{
public:
	CIndividual();
	CIndividual(CProblem* pProblem, CRandom c_rand);
	CIndividual(CProblem* pProblem, int iGenSize);
	~CIndividual();

	void vChangeGen( int iGenNumber, double dNewGen);
	double dGetGen(int iGenNumber) { if (iGenNumber < 0 || iGenNumber >= i_gen_size)cout << "Nieprawidlowy argument\n"; return pd_genotype[iGenNumber]; }

	bool bCompareIndividuals(CIndividual cOther);

	int iGenSize() { return i_gen_size; }

	double* dGenotype() { return pd_genotype; }
	void vFixIndividual();
private:
	double* pd_genotype;
	int i_gen_size;
	CRandomSearch c_rand_search;
	CProblem* pc_problem;

};//class CIndividual
