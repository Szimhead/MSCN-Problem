#pragma once
#include "pch.h"
#include "CRandom.h"
#include "CProblem.h"
#include <iostream>
#include <string>
#include <cstring>

#define SUCCEED 1
#define NEGATIVE_UNIT_NUMBER -1
#define STRUCTURE_DOESNT_EXIST -2
#define NEGATIVE_VALUE -3
#define ABOVE_CAPACITY -4
#define FILE_ERROR -5
#define NOT_A_NUMBER -6

//costs
#define DEL_FAC_COST_MIN 1
#define DEL_FAC_COST_MAX 100
#define FAC_MAG_COST_MIN 1
#define FAC_MAG_COST_MAX 100
#define MAG_SHOP_COST_MIN 1
#define MAG_SHOP_COST_MAX 100

//income
#define INCOME_MIN 1
#define INCOME_MAX 1000

//one-time costs
#define ONE_TIME_DEL_COST_MIN 1
#define ONE_TIME_DEL_COST_MAX 100
#define ONE_TIME_FAB_COST_MIN 1
#define ONE_TIME_FAB_COST_MAX 100
#define ONE_TIME_MAG_COST_MIN 1
#define ONE_TIME_MAG_COST_MAX 100

//units' capacity
#define SUM_DEL_MIN 100
#define SUM_DEL_MAX 1000
#define SUM_FAB_MIN 100
#define SUM_FAB_MAX 1000
#define SUM_MAG_MIN 100
#define SUM_MAG_MAX 1000
#define SUM_SHOP_MIN 100
#define SUM_SHOP_MAX 1000


#define DEL_FAC_MIN_MAX 1
#define DEL_FAC_MAX_MIN 1000
#define FAC_MAG_MIN_MAX 1
#define FAC_MAG_MAX_MIN 1000
#define MAG_SHOP_MIN_MAX 1
#define MAG_SHOP_MAX_MIN 1000

using namespace std;

class CMscnProblem: public CProblem
{
public:
	CMscnProblem();
	~CMscnProblem();
	double dGetQuality();
	bool bConstraintsSatisfied();

	//setters
	int iSetDeliverers(int iD);
	int iSetFactories(int iF);
	int iSetMagazines(int iM);
	int iSetShops(int iS);
	//setters

	//getters
	int iGetDeliverers() { return i_deliverers; }
	int iGetFactories() { return i_factories; }
	int iGetMagazines() { return i_magazines; }
	int iGetShops() { return i_shops; }

	//capacity getters
	double dGetDeliverersCapacity(int iNumber) { return  pd_summary_delivery_tab[iNumber]; }
	double dGetFactoriesCapacity(int iNumber) { return  pd_summary_fabrication_tab[iNumber]; }
	double dGetMagazinesCapacity(int iNumber) { return pd_summary_magazine_tab[iNumber]; }
	double dGetShopsCapacity(int iNumber) { return pd_summary_shop_tab[iNumber]; }
	//getters

	//cell modifiers
	int iModDeliveryCost(double iNewVal, int iDel, int iFac);
	int iModFabricationCost(double iNewVal, int iFac, int iMag);
	int iModMagazineCost(double iNewVal, int iMag, int iShop);

	int iModDelivererCapacity(double iNewVal, int iDel);
	int iModFactoryCapacity(double iNewVal, int Fac);
	int iModMagazineCapacity(double iNewVal, int iMag);
	int iModShopCapacity(double iNewVal, int iShop);
	//cell modifiers

	//orders setters
	int iSetDelOrder(double dNewVal, int iDel, int iFac);
	int iSetFacOrder(double dNewVal, int iFac, int iMag);
	int iSetMagOrder(double dNewVal, int iMag, int iShop);
	//orders setters

	//one-time costs setters
	int iSetOneTimeDelCost(int iDel,double dCost);
	int iSetOneTimeFacCost(int iFac, double dCost);
	int iSetOneTimeMagCost(int iMag, double dCost);
	//one-time costs setters

	//profit setter
	int iSetProfit(int iShop, double dProfit);
	
	//min i max
	void vSetDelFacMin(int iVal) { d_del_fac_min = iVal; }
	void vSetDelFacMax(int iVal) { d_del_fac_max = iVal; }
	void vSetFacMagMin(int iVal) { d_fac_mag_min = iVal; }
	void vSetFacMagMax(int iVal) { d_fac_mag_max = iVal; }
	void vSetMagShopMin(int iVal) { d_mag_shop_min = iVal; }
	void vSetMagShopMax(int iVal) { d_mag_shop_max = iVal; }

	double dDelFacMin() { return d_del_fac_min; }
	double dDelFacMax() { return d_del_fac_max; }
	double dFacMagMin() { return d_fac_mag_min; }
	double dFacMagMax() { return d_fac_mag_max; }
	double dMagShopMin() { return d_mag_shop_min; }
	double dMagShopMax() { return d_mag_shop_max; }

	void vShowMinMax();
	//min i max

	int iProblemFile(string sFileName, double** dParameters);

	void vSetParameters(double* dParameters);
	int iSetSolution(double* dSolution);

	//kontrola macierzy
	void vShowMatrixes();

	//Random
	void vSetNumbers(int iD, int iF, int iM, int iS);
	void vGenerateInstance(int iInstanceSeed);

	//fix
	void vFixSolution(double* pdSol);

	double* dGenerateSolution(CRandom c_rand) override;
	int iGetNumberOfElements() override {return iGetDeliverers()*iGetFactories() + iGetFactories()*iGetMagazines() + iGetMagazines()*iGetShops() + 4;	}

private:
	//methods
	//matrix modifiers
	void v_deliverers_change(int iD);
	void v_factories_change(int iF);
	void v_magazines_change(int iM);
	void v_shops_change(int iS);
	//matrix modifiers

	void v_matrix_modification(double*** pdMatrixToChange, int iNewNumber, int iThisLevelUnits, int iLowerLevelUnits);
	void v_table_modification(double** pdTabToChange, int iNewNumber,  int iUnits);
	int i_matrix_cell_modification(double*** pdMatrixToChange, double dNewVal, int iThisLevel, int iThisLevelToChange, int iLowerLevel, int iLowerLevelToChange);
	int i_table_cell_modification(double** pdTableToChange, double dNewVal, int iUnitToChange, int iUnits);
	int i_order_setter(double*** pdOrderFromTab, double* pdThisLevelCapacity, double* pdLowerLevelCapacity, double dOrder, int iThisLevelToSet, int iThisLevel, int iLowerLevelToSet, int iLowerLevel);
	
	double d_string_to_double(string sWord);
	void v_set_solution_result(int iRes) { i_solution_res = iRes; }
	int i_solution_result() { return i_solution_res; }
	//methods

	//fields
	//numbers of elements in the structure
	int i_deliverers;
	int i_factories;
	int i_magazines;
	int i_shops;

	double* pd_profit;

	//costs
	double** pd_costs_del_fac_d_tab;
	double** pd_costs_fac_mag_d_tab;
	double** pd_costs_mag_shop_d_tab;

	//amount of transported products
	double** pd_x_del_fac_d_tab;
	double** pd_x_fac_mag_d_tab;
	double** pd_x_mag_shop_d_tab;

	//income
	double* pd_income_tab;

	//one-time costs
	double* pd_unavoidably_paid_delivery_tab;
	double* pd_unavoidably_paid_fabrication_tab;
	double* pd_unavoidably_paid_magazine_tab;

	//units' capacity
	double* pd_summary_delivery_tab;
	double* pd_summary_fabrication_tab;
	double* pd_summary_magazine_tab;
	double* pd_summary_shop_tab;
	

	double d_del_fac_min;
	double d_del_fac_max;
	double d_fac_mag_min;
	double d_fac_mag_max;
	double d_mag_shop_min;
	double d_mag_shop_max;

	int i_solution_res;
	double* pd_solution;
	//fields
};//CMscnProblem
