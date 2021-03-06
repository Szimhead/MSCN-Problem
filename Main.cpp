#include "pch.h"
#include "CMscnProblem.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"
#include "CTimer.h"
#include "COptimizer.h"
#include "CProblem.h"
#include <iostream>

using namespace std;

int main()
{
	CMscnProblem c_problem;
	/*cout << c_problem.iSetDeliverers(1) << endl;
	cout << c_problem.iSetFactories(1) << endl;
	cout << c_problem.iSetMagazines(1) << endl;
	cout << c_problem.iSetShops(1) << endl;

	cout << c_problem.iModDelivererCapacity(100,0) << endl;
	cout << c_problem.iModFactoryCapacity(50,0) << endl;
	cout << c_problem.iModMagazineCapacity(10.5,0) << endl;
	cout << c_problem.iModShopCapacity(100, 0) << endl;

	cout << c_problem.iModDeliveryCost(3, 0, 0) << endl;
	cout << c_problem.iModFabricationCost(4, 0, 0) << endl;
	cout << c_problem.iModMagazineCost(1, 0, 0) << endl;
	
	cout << c_problem.iSetOneTimeDelCost(0, 10) << endl;
	cout << c_problem.iSetOneTimeFacCost(0, 10) << endl;
	cout << c_problem.iSetOneTimeMagCost(0, 10) << endl;

	cout << c_problem.iSetProfit(0, 100) << endl;*/

	

	double* d_problem_tab;
	double* d_solution_tab;
	

	cout<<c_problem.iProblemFile("test5.txt",&d_problem_tab)<<endl;
	c_problem.vSetParameters(d_problem_tab);
	c_problem.vShowMinMax();

	cout << c_problem.iProblemFile("solution5.txt", &d_solution_tab)<<endl;
	cout << "zastosowanie rozwiazania: "<<c_problem.iSetSolution(d_solution_tab) << endl;
	c_problem.vShowMatrixes();
	cout << "spelnienie kryteriow: "<<c_problem.bConstraintsSatisfied();
	cout << "\nquality: "<<c_problem.dGetQuality() << endl;


	//LISTA 10
	//CMscnProblem c_problem1;
	//double* d_solution_tab1;
	//c_problem1.vSetNumbers(3,2,1,1);
	//c_problem1.vGenerateInstance(time(NULL));
	//cout << c_problem1.iProblemFile("solution5.txt", &d_solution_tab1) << endl;
	//cout << "zastosowanie rozwiazania: " << c_problem1.iSetSolution(d_solution_tab1) << endl;
	//c_problem1.vShowMatrixes();
	//cout << "spelnienie kryteriow: " << c_problem1.bConstraintsSatisfied();
	//cout << "\nquality: " << c_problem1.dGetQuality() << endl;


	/*CTimer c_time;
	c_time.vStartTime();
	Sleep(200);
	c_time.vStopTime();
	cout << "\n\nczas: " << c_time.dGetTime();

	Sleep(200);
	c_time.vStartTime();
	Sleep(1000);
	c_time.vStopTime();
	cout << "\n\nczas: " << c_time.dGetTime();*/



	//RandomSearch
	CProblem* pc_problem12 = new CMscnProblem();
	(*pc_problem12).vSetParameters(d_problem_tab);
	COptimizer* pc_rs=new CRandomSearch(pc_problem12);
	double* pd_rs_best=(*pc_rs).dtFindBestSolution(5);

	pc_problem12->iSetSolution(pd_rs_best);
	cout <<"Najlepsze rozwiazanie z Random Search:"<<endl<< pc_problem12->dGetQuality() << endl;

	//Differential Evolution
	COptimizer* pc_de=new CDiffEvol (pc_problem12);
	double* pd_de_best=(*pc_de).dtFindBestSolution(5);
	
	pc_problem12->iSetSolution(pd_de_best);
	cout << "Najlepsze rozwiazanie z Differential Evolution:" << endl << pc_problem12->dGetQuality() << endl;
	

	

}
