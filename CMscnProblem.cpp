#include "pch.h"
#include "CMscnProblem.h"
#include <iostream>
#include <string>

using namespace std;

//constructor
CMscnProblem::CMscnProblem()
{
	i_deliverers = 0;
	i_factories = 0;
	i_magazines = 0;
	i_shops = 0;
}//CMscnProblem::CMscnProblem()
//constructor

//destructor
CMscnProblem::~CMscnProblem()
{
	delete pd_profit;

	delete[] pd_costs_del_fac_d_tab;
	delete[] pd_costs_fac_mag_d_tab;
	delete[] pd_costs_mag_shop_d_tab;

	delete[] pd_x_del_fac_d_tab;
	delete[] pd_x_fac_mag_d_tab;
	delete[] pd_x_mag_shop_d_tab;

	delete pd_income_tab;

	delete pd_unavoidably_paid_delivery_tab;
	delete pd_unavoidably_paid_fabrication_tab;
	delete pd_unavoidably_paid_magazine_tab;
	
	delete pd_summary_delivery_tab;
	delete pd_summary_fabrication_tab;
	delete pd_summary_magazine_tab;
	delete pd_summary_shop_tab;
}//CMscnProblem::~CMscnProblem()
//destructor


double CMscnProblem::dGetQuality()
{
	if (!bConstraintsSatisfied())
	{
		vFixSolution(pd_solution);
		iSetSolution(pd_solution);
	}
	double d_profits=0, d_transport=0, d_deals=0;
	//profits
	for (int ii = 0; ii < i_magazines; ii++)
	{
		for (int ij = 0; ij < i_shops; ij++)
		{
			d_profits += pd_profit[ij] * pd_x_mag_shop_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_shops; ij++)
	}//for (int ii = 0; ii < i_magazines; ii++)

	//transport
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		for (int ij = 0; ij < i_factories; ij++)
		{
			d_transport += pd_costs_del_fac_d_tab[ii][ij]*pd_x_del_fac_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_factories; ij++)
	}//for (int ii = 0; ii < i_deliverers; ii++)
	for (int ii = 0; ii < i_factories; ii++)
	{
		for (int ij = 0; ij < i_magazines; ij++)
		{
			d_transport += pd_costs_fac_mag_d_tab[ii][ij]*pd_x_fac_mag_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_magazines; ij++)
	}//for (int ii = 0; ii < i_factories; ii++)
	for (int ii = 0; ii < i_magazines; ii++)
	{
		for (int ij = 0; ij < i_shops; ij++)
		{
			d_transport += pd_costs_mag_shop_d_tab[ii][ij] * pd_x_mag_shop_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_shops; ij++)
	}//for (int ii = 0; ii < i_magazines; ii++)

	//deals
	bool b_paid = false;
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		b_paid = false;
		for (int ij = 0; ij < i_factories; ij++)
		{
			if (pd_x_del_fac_d_tab[ii][ij] > 0 && !b_paid)
			{
				d_deals += pd_unavoidably_paid_delivery_tab[ii];
				b_paid = true;
			}//if (pd_x_del_fac_d_tab[ii][ij] > 0 && !b_paid)
		}//for (int ij = 0; ij < i_factories; ij++)
	}//for (int ii = 0; ii < i_deliverers; ii++)

	for (int ii = 0; ii < i_factories; ii++)
	{
		b_paid = false;
		for (int ij = 0; ij < i_magazines; ij++)
		{
			if (pd_x_fac_mag_d_tab[ii][ij] > 0 && !b_paid)
			{
				d_deals += pd_unavoidably_paid_fabrication_tab[ii];
				b_paid = true;
			}//if (pd_x_fac_mag_d_tab[ii][ij] > 0 && !b_paid)
		}//for (int ij = 0; ij < i_magazines; ij++)
	}//for (int ii = 0; ii < i_factories; ii++)

	for (int ii = 0; ii < i_magazines; ii++)
	{
		b_paid = false;
		for (int ij = 0; ij < i_shops; ij++)
		{
			if (pd_x_mag_shop_d_tab[ii][ij] > 0 && !b_paid)
			{
				d_deals += pd_unavoidably_paid_magazine_tab[ii];
				b_paid = true;
			}//if (pd_x_mag_shop_d_tab[ii][ij] > 0 && !b_paid)
		}//for (int ij = 0; ij < i_shops; ij++)
	}//	for (int ii = 0; ii < i_magazines; ii++)
	return d_profits - d_transport - d_deals;
}//double CMscnProblem::dGetQuality()

bool CMscnProblem::bConstraintsSatisfied()
{ 
	int res = 0;
	if ((res=i_solution_result()) < 0)
	{
	//	cout << "blad: " << res<<endl;
		return false;
	}//if ((res=i_solution_result()) < 0)

	double i_sum_order = 0;
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		for (int ij = 0; ij < i_factories; ij++)
		{
			if (pd_x_del_fac_d_tab[ii][ij] < this->dDelFacMin() || pd_x_del_fac_d_tab[ii][ij]>this->dDelFacMax())
			{
	//			cout << "delfacmin/delfacmax " << endl;
				return false;
			}//if (pd_x_del_fac_d_tab[ii][ij] < this->dDelFacMin() || pd_x_del_fac_d_tab[ii][ij]>this->dDelFacMax())
		}//for (int ij = 0; ij < i_factories; ij++)
	}//for (int ii = 0; ii < i_deliverers; ii++)
	

	for (int ii = 0; ii < i_factories; ii++)
	{
		for (int ij = 0; ij < i_magazines; ij++)
		{
			if (pd_x_fac_mag_d_tab[ii][ij] < this->dFacMagMin() || pd_x_fac_mag_d_tab[ii][ij]>this->dFacMagMax())
			{
			//	cout << "facmagmin/facmagcmax " << endl;
				return false;
			}//if (pd_x_fac_mag_d_tab[ii][ij] < this->dFacMagMin() || pd_x_fac_mag_d_tab[ii][ij]>this->dFacMagMax())
		}//for (int ij = 0; ij < i_magazines; ij++)
	}//for (int ii = 0; ii < i_factories; ii++)

	
	for (int ii = 0; ii < i_magazines; ii++)
	{
		for (int ij = 0; ij < i_shops; ij++)
		{
			if (pd_x_mag_shop_d_tab[ii][ij] < this->dMagShopMin() || pd_x_mag_shop_d_tab[ii][ij]>this->dMagShopMax())
			{
			//	cout << "magshopmin/magshopmax " << endl;
				return false;
			}//if (pd_x_mag_shop_d_tab[ii][ij] < this->dMagShopMin() || pd_x_mag_shop_d_tab[ii][ij]>this->dMagShopMax())
		}//for (int ij = 0; ij < i_shops; ij++)
	}//for (int ii = i_par_index; ii < pdSolution[2] + i_end_of_data; ii++)

	//sprawdzenie czy element przyjmuje tyle samo towaru, co wydaje
	for (int ii = 0; ii < i_factories; ii++)
	{
		int i_sum = 0;
		for (int ij = 0; ij < i_deliverers; ij++)
		{
			i_sum += pd_x_del_fac_d_tab[ij][ii];
		}//for (int ij = 0; ij < i_deliverers; ij++)
		for (int ij = 0; ij < i_magazines; ij++)
		{
			i_sum -= pd_x_fac_mag_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_magazines; ij++)
		if (i_sum != 0)
		{
		//	cout << "z fabryk nie tyle samo " << endl;
			return false;
		}
	}//for (int ii = 0; ii < i_factories; ii++)

	for (int ii = 0; ii < i_magazines; ii++)
	{
		int i_sum = 0;
		for (int ij = 0; ij < i_factories; ij++)
		{
			i_sum += pd_x_fac_mag_d_tab[ij][ii];
		}//for (int ij = 0; ij < i_factories; ij++)
		for (int ij = 0; ij < i_shops; ij++)
		{
			i_sum -= pd_x_mag_shop_d_tab[ii][ij];
		}//for (int ij = 0; ij < i_shops; ij++)
		if (i_sum != 0)
		{
		//	cout << "z magazynow nie tyle samo " << endl;
			return false;
		}
	}//for (int ii = 0; ii < i_magazines; ii++)
	return true;
}//bool CMscnProblem::bConstraintsSatisfied(double *pdSolution)

//setters
int CMscnProblem::iSetDeliverers(int iD)
{
	if (iD <= 0)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iD <= 0)
	v_deliverers_change(iD);
	return SUCCEED;
}//int CMscnProblem::iSetDeliverers(int iD)

int CMscnProblem::iSetFactories(int iF)
{
	if (iF <= 0)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iF <= 0)
	v_factories_change(iF);
	return SUCCEED;
}//int CMscnProblem::iSetFactories(int iF)

int CMscnProblem::iSetMagazines(int iM)
{
	if (iM <= 0)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iM <= 0)
	v_magazines_change(iM);
	return SUCCEED;
}//int CMscnProblem::iSetMagazines(int iM)

int CMscnProblem::iSetShops(int iS)
{
	if (iS <= 0)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iS <= 0)
	v_shops_change(iS);
	return SUCCEED;
}//int CMscnProblem::iSetShops(int iS)
//setters

//cell modifiers
int CMscnProblem::iModDeliveryCost(double iNewVal, int iDel, int iFac)
{
	return i_matrix_cell_modification(&pd_costs_del_fac_d_tab, iNewVal, i_deliverers, iDel, i_factories, iFac);
}//int CMscnProblem::iModDeliveryCost(double iNewVal, int iDel, int iFac)

int CMscnProblem::iModFabricationCost(double iNewVal, int iFac, int iMag)
{
	return i_matrix_cell_modification(&pd_costs_fac_mag_d_tab, iNewVal, i_factories, iFac, i_magazines, iMag);
}//int CMscnProblem::iModFabricationCost(double iNewVal, int iFac, int iMag)

int CMscnProblem::iModMagazineCost(double iNewVal, int iMag, int iShop)
{
	return i_matrix_cell_modification(&pd_costs_mag_shop_d_tab, iNewVal, i_magazines, iMag, i_shops, iShop);
}//int CMscnProblem::iModMagazineCost(double iNewVal, int iMag, int iShop)

int CMscnProblem::iModDelivererCapacity(double iNewVal, int iDel)
{
	return i_table_cell_modification(&pd_summary_delivery_tab, iNewVal, iDel, i_deliverers);
}//int CMscnProblem::iModDelivererCapacity(double iNewVal, int iDel)

int CMscnProblem::iModFactoryCapacity(double iNewVal, int iFac)
{
	return i_table_cell_modification(&pd_summary_fabrication_tab, iNewVal, iFac, i_factories);
}//int CMscnProblem::iModFactoryCapacity(double iNewVal, int iFac)

int CMscnProblem::iModMagazineCapacity(double iNewVal, int iMag)
{
	return i_table_cell_modification(&pd_summary_magazine_tab, iNewVal, iMag, i_magazines);
}//int CMscnProblem::iModMagazineCapacity(double iNewVal, int iMag)

int CMscnProblem::iModShopCapacity(double iNewVal, int iShop)
{
	return i_table_cell_modification(&pd_summary_shop_tab, iNewVal, iShop, i_shops);
}//int CMscnProblem::iModShopCapacity(double iNewVal, int iShop)
//cell modifiers

//orders setters
int CMscnProblem::iSetDelOrder(double dNewVal, int iDel, int iFac)
{
	return i_order_setter(&pd_x_del_fac_d_tab, pd_summary_delivery_tab, pd_summary_fabrication_tab, dNewVal, iDel, i_deliverers, iFac, i_factories);
}//int CMscnProblem::iSetDelOrder(double dNewVal, int iDel, int iFac)

int CMscnProblem::iSetFacOrder(double dNewVal, int iFac, int iMag)
{
	return i_order_setter(&pd_x_fac_mag_d_tab, pd_summary_fabrication_tab, pd_summary_magazine_tab, dNewVal, iFac, i_factories, iMag, i_magazines);
}//int CMscnProblem::iSetFacOrder(double dNewVal, int iFac, int iMag)

int CMscnProblem::iSetMagOrder(double dNewVal, int iMag, int iShop)
{
	return i_order_setter(&pd_x_mag_shop_d_tab, pd_summary_magazine_tab, pd_summary_shop_tab, dNewVal, iMag, i_magazines, iShop, i_shops);
}//int CMscnProblem::iSetMagOrder(double dNewVal, int iMag, int iShop)
//orders setters

//one-time costs setters
int CMscnProblem::iSetOneTimeDelCost(int iDel, double dCost)
{
	return i_table_cell_modification(&pd_unavoidably_paid_delivery_tab, dCost, iDel, i_deliverers);
}//int CMscnProblem::iSetOneTimeDelCost(int iDel, double dCost)

int CMscnProblem::iSetOneTimeFacCost(int iFac, double dCost)
{
	return i_table_cell_modification(&pd_unavoidably_paid_fabrication_tab, dCost, iFac, i_factories);
}//int CMscnProblem::iSetOneTimeFacCost(int iFac, double dCost)

int CMscnProblem::iSetOneTimeMagCost(int iMag, double dCost)
{
	return i_table_cell_modification(&pd_unavoidably_paid_magazine_tab, dCost, iMag, i_magazines);
}//int CMscnProblem::iSetOneTimeMagCost(int iMag, double dCost)
//one-time costs setters

int CMscnProblem::iSetProfit(int iShop, double dProfit)
{
	return i_table_cell_modification(&pd_profit, dProfit, iShop, i_shops);
}//int CMscnProblem::iSetProfit(int iShop, double dProfit)

void CMscnProblem::vShowMinMax()
{
	cout << "Minimum od dostawcy do fabryki = " << this->dDelFacMin() << endl << "Maximum od dostawcy do fabryki = " << this->dDelFacMax() << endl << "Minimum od fabryki do magazynu = " << this->dFacMagMin() << endl << "Maximum od fabryki do magazynu = " << this->dFacMagMax() << endl << "Minimum od magazynu do sklepu = " << this->dMagShopMin() << endl << "Maximum od magazynu do sklepu = " << this->dMagShopMax() << endl;
}//void CMscnProblem::vShowMinMax()

//matrix modifiers
void CMscnProblem::v_deliverers_change(int iD)
{
	v_matrix_modification(&pd_costs_del_fac_d_tab, iD, i_deliverers, i_factories);
	v_matrix_modification(&pd_x_del_fac_d_tab, iD, i_deliverers, i_factories);
	v_table_modification(&pd_summary_delivery_tab, iD,i_deliverers);
	v_table_modification(&pd_unavoidably_paid_delivery_tab, iD, i_deliverers);
	i_deliverers = iD;
}//void CMscnProblem::v_deliverers_change(int iD)

void CMscnProblem::v_factories_change(int iF)
{
	v_matrix_modification(&pd_costs_fac_mag_d_tab, iF, i_factories, i_magazines);
	v_matrix_modification(&pd_x_fac_mag_d_tab, iF, i_factories, i_magazines);
	v_table_modification(&pd_summary_fabrication_tab, iF,i_factories);
	v_table_modification(&pd_unavoidably_paid_fabrication_tab, iF, i_factories);
	i_factories = iF;
	v_matrix_modification(&pd_costs_del_fac_d_tab, i_deliverers, 0, i_factories);
	v_matrix_modification(&pd_x_del_fac_d_tab, i_deliverers, 0, i_factories);
}//void CMscnProblem::v_factories_change(int iF)

void CMscnProblem::v_magazines_change(int iM)
{
	v_matrix_modification(&pd_costs_mag_shop_d_tab, iM, i_magazines, i_shops);
	v_matrix_modification(&pd_x_mag_shop_d_tab, iM, i_magazines, i_shops);
	v_table_modification(&pd_summary_magazine_tab, iM, i_magazines);
	v_table_modification(&pd_unavoidably_paid_magazine_tab, iM, i_magazines);
	i_magazines = iM;
	v_matrix_modification(&pd_costs_fac_mag_d_tab, i_factories, 0, i_magazines);
	v_matrix_modification(&pd_x_fac_mag_d_tab, i_factories, 0, i_magazines);
}//void CMscnProblem::v_magazines_change(int iM)

void CMscnProblem::v_shops_change(int iS)
{
	v_table_modification(&pd_income_tab, iS, i_shops);
	v_table_modification(&pd_summary_shop_tab, iS, i_shops);
	v_table_modification(&pd_profit, iS, i_shops);
	i_shops = iS;
	v_matrix_modification(&pd_costs_mag_shop_d_tab, i_magazines,  0,i_shops);
	v_matrix_modification(&pd_x_mag_shop_d_tab, i_magazines, 0, i_shops);
}//void CMscnProblem::v_shops_change(int iS)
//matrix modifiers

int CMscnProblem::i_matrix_cell_modification(double*** piMatrixToChange, double dNewVal, int iThisLevel, int iThisLevelToChange, int iLowerLevel, int iLowerLevelToChange)
{
	if (*piMatrixToChange == NULL)
	{
		return STRUCTURE_DOESNT_EXIST;
	}//if (*piMatrixToChange == NULL)
	if (dNewVal <= 0)
	{
		return NEGATIVE_VALUE;
	}//if (dNewVal <= 0)
	if (iThisLevelToChange < 0 || iThisLevelToChange >= iThisLevel || iLowerLevelToChange < 0 || iLowerLevelToChange >= iLowerLevel)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iThisLevelToChange < 0 || iThisLevelToChange >= iThisLevel || iLowerLevelToChange < 0 || iLowerLevelToChange >= iLowerLevel)
	(*piMatrixToChange)[iThisLevelToChange][iLowerLevelToChange] = dNewVal;
	return SUCCEED;
}//int CMscnProblem::i_matrix_cell_modification(double*** piMatrixToChange, double dNewVal, int iThisLevel, int iThisLevelToChange, int iLowerLevel, int iLowerLevelToChange)

int CMscnProblem::i_table_cell_modification(double** piTableToChange, double dNewVal, int iUnitToChange, int iUnits)
{
	if (*piTableToChange == NULL)
	{
		return STRUCTURE_DOESNT_EXIST;
	}//	if (*piTableToChange == NULL)

	if (dNewVal <= 0)
	{
		return NEGATIVE_VALUE;
	}//	if (dNewVal <= 0)

	if (iUnitToChange < 0 || iUnitToChange >= iUnits)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//	if (iUnitToChange < 0 || iUnitToChange >= iUnits)

	(*piTableToChange)[iUnitToChange] = dNewVal;
	return SUCCEED;
}//int CMscnProblem::i_table_cell_modification(double** piTableToChange, double dNewVal, int iUnitToChange, int iUnits)

int CMscnProblem::i_order_setter(double*** pdOrderFromTab, double* pdTLCapacity, double*pdLLCapacity, double dOrder, int iThisLevelToSet, int iThisLevel, int iLowerLevelToSet, int iLowerLevel)
{
	int d_sum = 0;
	if (*pdOrderFromTab == NULL)
	{
		return STRUCTURE_DOESNT_EXIST;
	}//if (*pdOrderFromTab == NULL)

	if (iThisLevelToSet < 0 || iThisLevelToSet >= iThisLevel || iLowerLevelToSet < 0 || iLowerLevelToSet >= iLowerLevel)
	{
		return NEGATIVE_UNIT_NUMBER;
	}//if (iThisLevelToSet < 0 || iThisLevelToSet >= iThisLevel || iLowerLevelToSet < 0 || iLowerLevelToSet >= iLowerLevel)
	
	(*pdOrderFromTab)[iThisLevelToSet][iLowerLevelToSet] = dOrder;//2 pierwsze warunki musz¹ zajœæ, ¿eby mo¿na by³o wykonaæ operacjê. Dalsze kryteria s¹ jedynie wymogami wynikaj¹cymi z pliku problemu

	if (dOrder < 0)
	{
		return NEGATIVE_VALUE;
	}//if (dOrder < 0)

	for (int ii = 0; ii < iLowerLevelToSet; ii++)
	{
		d_sum += (*pdOrderFromTab)[iThisLevelToSet][ii];
	}//for (int ii = 0; ii < iLowerLevel; ii++)
	if (d_sum > pdTLCapacity[iThisLevelToSet])
	{
		return ABOVE_CAPACITY;
	}//if (dOrder + d_sum > pdTLCapacity[iThisLevelToSet])
	d_sum = 0;
	for (int ii = 0; ii < iThisLevelToSet; ii++)
	{
		d_sum += (*pdOrderFromTab)[ii][iLowerLevelToSet];
	}//for (int ii = 0; ii < iThisLevel; ii++)
	if (d_sum > pdLLCapacity[iLowerLevelToSet])
	{
		return ABOVE_CAPACITY;
	}//f (dOrder + d_sum > pdLLCapacity[iLowerLevelToSet])

	
	return SUCCEED;
}//int CMscnProblem::i_order_setter(double*** pdOrderFromTab, double* pdTLCapacity, double*pdLLCapacity, double dOrder, int iThisLevelToSet, int iThisLevel, int iLowerLevelToSet, int iLowerLevel)

void CMscnProblem::v_matrix_modification(double*** pdMatrixToChange, int iNumber, int iThisLevel, int iLowerLevel)
{
	double** pd_temp = new double*[iNumber];
	if (*pdMatrixToChange == NULL || iThisLevel==0)
	{
		for (int ii = 0; ii < iNumber; ii++)
		{
			pd_temp[ii] = new double[iLowerLevel];
		}//for (int ii = 0; ii < iNumber; ii++)
		
		
		for (int ii = 0; ii <iNumber; ii++)
		{
			for (int ij = 0; ij < iLowerLevel; ij++)
			{
				pd_temp[ii][ij] = -1;
			}//for (int ij = 0; ij < iLowerLevel; ij++)
		}//for (int ii = 0; ii <iNumber; ii++)
	}//if (*pdMatrixToChange == NULL || iThisLevel==0)
	else
	{
		if (iNumber > iThisLevel)
		{
			for (int ii = 0; ii < iThisLevel; ii++)
			{
				pd_temp[ii] = (*pdMatrixToChange)[ii];
			}//for (int ii = 0; ii < iThisLevel; ii++)
			for (int ii = iThisLevel; ii < iNumber; ii++)
			{
				for (int ij = 0; ij < iLowerLevel; ij++)
				{
					pd_temp[ii][ij] = -1;
				}//for (int ij = 0; ij < iLowerLevel; ij++)
			}//for (int ii = iThisLevel; ii < iNumber; ii++)
			delete[] *pdMatrixToChange;
		}
		else
		{
			for (int ii = 0; ii < iNumber; ii++)
			{
				pd_temp[ii] = (*pdMatrixToChange)[ii];
			}//for (int ii = 0; ii < iNumber; ii++)
			for (int ii = iNumber; ii < iThisLevel; ii++)
			{
				delete[] (*pdMatrixToChange)[ii];
			}//for (int ii = iNumber; ii < iThisLevel; ii++)
		}//else
	}//else
	*pdMatrixToChange = pd_temp;
}//void CMscnProblem::v_matrix_modification(double*** pdMatrixToChange, int iNumber, int iThisLevel, int iLowerLevel)

void CMscnProblem::v_table_modification(double** pdTabToChange, int iNumber, int iUnits)
{
	if (*pdTabToChange == NULL)
	{
		*pdTabToChange = new double[iNumber];
	}//if (*pdTabToChange == NULL)
	else
	{
		double* pd_tab = new double[iNumber];
		if (iNumber > iUnits)
		{
			for (int ii = 0; ii < iUnits; ii++)
			{
				pd_tab[ii] = (*pdTabToChange)[ii];
			}//for (int ii = 0; ii < iUnits; ii++)
			for (int ii = iUnits; ii < iNumber; ii++)
			{
				pd_tab[ii] = -1;
			}//for (int ii = iUnits; ii < iNumber; ii++)
		}//if (iNumber > iUnits)
		else
		{
			for (int ii = 0; ii < iNumber; ii++)
			{
				pd_tab[ii] = (*pdTabToChange)[ii];
			}//for (int ii = 0; ii < iNumber; ii++)
		}//else
		delete *pdTabToChange;
		*pdTabToChange = pd_tab;
	}//else
}//void CMscnProblem::v_table_modification(double** pdTabToChange, int iNumber, int iUnits)


int CMscnProblem::iProblemFile(string sFileName, double** dParam)
{
	FILE *pf_file;

	pf_file = fopen(sFileName.c_str(), "r");
	if (pf_file == NULL)
	{
		return FILE_ERROR;
	}//if (pf_file == NULL)

	char c_data;
	string s_word;
	double d_res;
	int i_counter = 0;

	do
	{
		c_data = fgetc(pf_file);
		if (c_data == ' ' || c_data == '\n' || c_data==EOF)
		{	
			d_res = d_string_to_double(s_word);
			if (d_res != -1)
			{
				i_counter++;
			}//if (d_res != -1)
			s_word = "";
		}//if (c_data == ' ' || c_data == '\n' || c_data==EOF)
		else
		{
			s_word+=c_data;
		}//else
		
	} while ( c_data!= EOF);

	fclose(pf_file);

	double *dParameters = new double[i_counter];
	int i_index = 0;

	pf_file = fopen(sFileName.c_str(), "r");
	if (pf_file == NULL)
	{
		return FILE_ERROR;
	}//if (pf_file == NULL)

	do
	{
		c_data = fgetc(pf_file);
		if (c_data == ' ' || c_data == '\n' || c_data == EOF)
		{
			d_res = d_string_to_double(s_word);
			if (d_res != NOT_A_NUMBER)
			{
				(dParameters)[i_index] = d_res;
				i_index++;
			}//if (d_res != -1)
			s_word = "";
		}//if (c_data == ' ' || c_data == '\n' || c_data == EOF)
		else
		{
			s_word += c_data;
		}//else

	} while (c_data != EOF);

	fclose(pf_file);

	*dParam = dParameters;
	return SUCCEED;
}// int CMscnProblem::iProblemFile(string sFileName, double** dParam)

double CMscnProblem::d_string_to_double(string sText)
{
	double d_temp_val = 0;
	double d_frac_val = 0;
	int i_frac_pos = 1;
	bool b_fraction = false;
	for (int ii = 0; ii < sText.size(); ii++)
	{
		int i_str_val = sText.at(ii);
		if (i_str_val >= '0' && i_str_val <= '9')
		{
			if (b_fraction)
			{
				d_frac_val = (i_str_val - '0') / pow(10, i_frac_pos++);
				d_temp_val += d_frac_val;
			}//if (b_fraction)
			else
			{
				if (ii < sText.size())
				{
					d_temp_val *= 10;
				}//if (ii < sText.size())
				d_temp_val = d_temp_val + i_str_val - '0';
			}//else
		}//if (i_str_val >= '0' && i_str_val <= '9')
		else if (i_str_val == '.')
		{
			b_fraction = true;
		}//else if (i_str_val == '.')
		else
		{
			return NOT_A_NUMBER;
		}//else
	}//for (int ii = 0; ii < sText.size(); ii++)
	return d_temp_val;
}//int Zakres::iStringToInt(string sText)

void CMscnProblem::vSetParameters(double* dParameters)
{
	cout << "ustawienie liczby dostawcow: "<<this->iSetDeliverers(dParameters[0]) << endl;
	cout << "ustawienie liczby fabryk: " << this->iSetFactories(dParameters[1]) << endl;
	cout << "ustawienie liczby magazynow: " << this->iSetMagazines(dParameters[2]) << endl;
	cout << "ustawienie liczby sklepow: " << this->iSetShops(dParameters[3]) << endl;


	//capacity
	int i_index = 0;
	int i_par_index = 4;
	int i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[0]+i_end_of_data; ii++)
	{
		cout<<"ustawianie pojemnosci dostaw: "<<this->iModDelivererCapacity(dParameters[ii],i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii < dParameters[0]+i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[1]+ i_end_of_data; ii++)
	{
		cout <<"ustawianie pojemnosci fabryk: "<< this->iModFactoryCapacity(dParameters[ii], i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii < dParameters[1]+ i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii =i_par_index; ii < dParameters[2]+ i_end_of_data; ii++)
	{
		cout <<"ustawianie pojemnosci magazynow: "<< this->iModMagazineCapacity(dParameters[ii], i_index++) << endl;
		i_par_index++;
	}//for (int ii =i_par_index; ii < dParameters[2]+ i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii <dParameters[3]+ i_end_of_data; ii++)
	{
		cout <<"ustawianie pojemnosci sklepow: "<< this->iModShopCapacity(dParameters[ii], i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii <dParameters[3]+ i_end_of_data; ii++)
	//capacity

	//costs
	int i_col_index = 0, i_row_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[1]; ij++)
		{
			cout <<"ustawianie kosztow dostaw: "<< this->iModDeliveryCost(dParameters[i_par_index], i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[1]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[2]; ij++)
		{
			cout <<"ustawianie kosztow fabryk: "<< this->iModFabricationCost(dParameters[i_par_index], i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[2]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[3]; ij++)
		{
			cout << "ustawianie kosztow magazynow: " << this->iModMagazineCost(dParameters[i_par_index], i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[3]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	//costs

	//one-time costs
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych dostaw: " << this->iSetOneTimeDelCost(i_index++,dParameters[i_end_of_data]) << endl;
	}//for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)
	i_par_index++;
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych fabryk: " << this->iSetOneTimeFacCost(i_index++,dParameters[i_end_of_data]) << endl;
	}//for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)
	i_par_index++;
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych magazynow: " << this->iSetOneTimeMagCost(i_index++,dParameters[i_end_of_data]) << endl;
	}//for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	i_par_index++;
	//one-time costs

	//profit
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[3] + i_end_of_data; ii++)
	{
		cout << "ustawianie przychodow: " << this->iSetProfit(i_index++,dParameters[ii]) << endl;
	}//for (int ii = i_par_index; ii < dParameters[3] + i_end_of_data; ii++)
	//profit

	i_par_index++;

	//min, max
	this->vSetDelFacMin(dParameters[i_par_index++]);
	this->vSetDelFacMax(dParameters[i_par_index++]);
	this->vSetFacMagMin (dParameters[i_par_index++]);
	this->vSetFacMagMax(dParameters[i_par_index++]);
	this->vSetMagShopMin(dParameters[i_par_index++]);
	this->vSetMagShopMax(dParameters[i_par_index++]);
	//min, max
}//void CMscnProblem::vSetParameters(double* dParameters)

int CMscnProblem::iSetSolution(double* dParameters)
{
	pd_solution = dParameters;
	//orders
	int i_col_index = 0;
	int i_row_index = 0;
	int i_par_index = 4;
	int i_end_of_data = i_par_index;
	int i_operation_res=1;
	int i_setting_res = 1;
	/*cout << "dParameters";
	for (int ii = 0; ii < 7; ii++)
		cout << dParameters[ii] << " ";
	cout << endl;*/
	for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[1]; ij++)
		{
			if ((i_operation_res=this->iSetDelOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			{
				if(i_setting_res==1) i_setting_res=i_operation_res;
			}//if ((i_operation_res=this->iSetDelOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[1]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[2]; ij++)
		{
			if ((i_operation_res = this->iSetFacOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			{
				if (i_setting_res == 1) i_setting_res = i_operation_res;
			}//if ((i_operation_res = this->iSetFacOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[2]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < dParameters[3]; ij++)
		{
			if ((i_operation_res = this->iSetMagOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			{
				if (i_setting_res == 1) i_setting_res = i_operation_res;
			}//if ((i_operation_res = this->iSetMagOrder(dParameters[i_par_index], i_col_index, i_row_index++)) < 0)
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[3]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	v_set_solution_result(i_setting_res);
	return i_setting_res;
	//orders
}//int CMscnProblem::iSetSolution(double* dParameters)

void CMscnProblem::vShowMatrixes()
{
	cout << "i_deliverers " << i_deliverers<<endl;
	cout << "i_factories " << i_factories << endl;
	cout << "i_magazines " << i_magazines << endl;
	cout << "i_shops " << i_shops << endl;

	cout << "pd_profit " << endl;
	for (int ii = 0; ii < i_shops; ii++)
	{
		cout << pd_profit[ii] << " ";
	}//for (int ii = 0; ii < i_shops; ii++)
	cout << endl;

	cout << "\n\nmacierz kosztow od dostawcow do fabryk\n";
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		for (int ij = 0; ij < i_factories; ij++)
		{
			cout << pd_costs_del_fac_d_tab[ii][ij]<<" ";
		}//for (int ij = 0; ij < i_factories; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_deliverers; ii++)

	cout << "\n\nmacierz kosztow od fabryk do magazynow\n";
	for (int ii = 0; ii < i_factories; ii++)
	{
		for (int ij = 0; ij < i_magazines; ij++)
		{
			cout <<  pd_costs_fac_mag_d_tab[ii][ij] << " ";
		}//for (int ij = 0; ij < i_magazines; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_factories; ii++)

	cout << "\n\nmacierz kosztow od magazynow do sklepow\n";
	for (int ii = 0; ii < i_magazines; ii++)
	{
		for (int ij = 0; ij < i_shops; ij++)
		{
			cout << pd_costs_mag_shop_d_tab[ii][ij] << " ";
		}//for (int ij = 0; ij < i_shops; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_magazines; ii++)



	cout << "\n\n\nmacierz dostaw od dostawcow do fabryk\n";
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		for (int ij = 0; ij < i_factories; ij++)
		{
			cout << pd_x_del_fac_d_tab[ii][ij] << " ";
		}//for (int ij = 0; ij < i_factories; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_deliverers; ii++)

	cout << "\n\nmacierz dostaw od fabryk do magazynow\n";
	for (int ii = 0; ii < i_factories; ii++)
	{
		for (int ij = 0; ij < i_magazines; ij++)
		{
			cout << pd_x_fac_mag_d_tab[ii][ij] << " ";
		}//for (int ij = 0; ij < i_magazines; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_factories; ii++)

	cout << "\n\nmacierz dostaw od magazynow do sklepw\n";
	for (int ii = 0; ii < i_magazines; ii++)
	{
		for (int ij = 0; ij < i_shops; ij++)
		{
			cout << pd_x_mag_shop_d_tab[ii][ij] << " ";
		}//for (int ij = 0; ij < i_shops; ij++)
		cout << endl;
	}//for (int ii = 0; ii < i_magazines; ii++)



	cout << "\n\n\ntablica przychodu\n";
	for (int ii = 0; ii < i_shops; ii++)
	{
		cout << pd_income_tab[ii] << " ";
	}//for (int ii = 0; ii < i_shops; ii++)
	cout << endl;



	cout << "\n\ntablica kosztow jednorazowych dostaw\n";
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		cout << pd_unavoidably_paid_delivery_tab[ii] << " ";
	}// for (int ii = 0; ii < i_deliverers; ii++)
	cout << endl;

	cout << "\n\ntablica kosztow jednorazowych fabryk\n";
	for (int ii = 0; ii < i_factories; ii++)
	{
		cout << pd_unavoidably_paid_fabrication_tab[ii] << " ";
	}//	for (int ii = 0; ii < i_factories; ii++)
	cout << endl;

	cout << "\n\ntablica kosztow jednorazowych magazynow\n";
	for (int ii = 0; ii < i_magazines; ii++)
	{
		cout << pd_unavoidably_paid_magazine_tab[ii] << " ";
	}//	for (int ii = 0; ii < i_magazines; ii++)
	cout << endl;
	


	cout << "\n\n\ntablica pojemnosci dostaw\n";
	for (int ii = 0; ii < i_deliverers; ii++)
	{
		cout << pd_summary_delivery_tab[ii] << " ";
	}//for (int ii = 0; ii < i_deliverers; ii++)
	cout << endl;

	cout << "\n\ntablica pojemnosci fabryk\n";
	for (int ii = 0; ii < i_factories; ii++)
	{
		cout << pd_summary_fabrication_tab[ii] << " ";
	}//for (int ii = 0; ii < i_factories; ii++)
	cout << endl;

	cout << "\n\ntablica pojemnosci magazynow\n";
	for (int ii = 0; ii < i_magazines; ii++)
	{
		cout << pd_summary_magazine_tab[ii] << " ";
	}//for (int ii = 0; ii < i_magazines; ii++)
	cout << endl;

	cout << "\n\ntablica pojemnosci sklepow\n";
	for (int ii = 0; ii < i_shops; ii++)
	{
		cout << pd_summary_shop_tab[ii] << " ";
	}//for (int ii = 0; ii < i_shops; ii++)
	cout << endl;
}//void CMscnProblem::vShowMatrixes()

void CMscnProblem::vGenerateInstance(int iInstanceSeed)
{
	CRandom c_rand(iInstanceSeed);
	
	//capacity
	int i_index = 0;
	int i_par_index = 4;
	int i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_deliverers + i_end_of_data; ii++)
	{
		cout << "ustawianie pojemnosci dostaw: " << this->iModDelivererCapacity(c_rand.dRandomDouble(SUM_DEL_MIN,SUM_DEL_MAX), i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii < dParameters[0]+i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_factories + i_end_of_data; ii++)
	{
		cout << "ustawianie pojemnosci fabryk: " << this->iModFactoryCapacity(c_rand.dRandomDouble(SUM_FAB_MIN,SUM_FAB_MAX), i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii < dParameters[1]+ i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_magazines + i_end_of_data; ii++)
	{
		cout << "ustawianie pojemnosci magazynow: " << this->iModMagazineCapacity(c_rand.dRandomDouble(SUM_MAG_MIN,SUM_MAG_MAX), i_index++) << endl;
		i_par_index++;
	}//for (int ii =i_par_index; ii < dParameters[2]+ i_end_of_data; ii++)

	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii <i_shops + i_end_of_data; ii++)
	{
		cout << "ustawianie pojemnosci sklepow: " << this->iModShopCapacity(c_rand.dRandomDouble(SUM_SHOP_MIN,SUM_SHOP_MAX), i_index++) << endl;
		i_par_index++;
	}//for (int ii = i_par_index; ii <dParameters[3]+ i_end_of_data; ii++)
	//capacity

	//costs
	int i_col_index = 0, i_row_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_deliverers + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < i_factories; ij++)
		{
			cout << "ustawianie kosztow dostaw: " << this->iModDeliveryCost(c_rand.dRandomDouble(DEL_FAC_COST_MIN,DEL_FAC_COST_MAX), i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[1]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_factories + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < i_magazines; ij++)
		{
			cout << "ustawianie kosztow fabryk: " << this->iModFabricationCost(c_rand.dRandomDouble(FAC_MAG_COST_MIN,FAC_MAG_COST_MAX), i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[2]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)

	i_col_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_magazines + i_end_of_data; ii++)
	{
		i_row_index = 0;
		for (int ij = 0; ij < i_shops; ij++)
		{
			cout << "ustawianie kosztow magazynow: " << this->iModMagazineCost(c_rand.dRandomDouble(MAG_SHOP_COST_MIN,MAG_SHOP_COST_MAX), i_col_index, i_row_index++) << endl;
			i_par_index++;
		}//for (int ij = 0; ij < dParameters[3]; ij++)
		i_col_index++;
	}//for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	//costs

	//one-time costs
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_deliverers + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych dostaw: " << this->iSetOneTimeDelCost(i_index++, c_rand.dRandomDouble(ONE_TIME_DEL_COST_MIN,ONE_TIME_DEL_COST_MAX)) << endl;

	}//for (int ii = i_par_index; ii < dParameters[0] + i_end_of_data; ii++)
	i_par_index++;
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_factories + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych fabryk: " << this->iSetOneTimeFacCost(i_index++, c_rand.dRandomDouble(ONE_TIME_FAB_COST_MIN,ONE_TIME_FAB_COST_MAX)) << endl;

	}//for (int ii = i_par_index; ii < dParameters[1] + i_end_of_data; ii++)
	i_par_index++;
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_magazines + i_end_of_data; ii++)
	{
		cout << "ustawianie kosztow jednorazowych magazynow: " << this->iSetOneTimeMagCost(i_index++, c_rand.dRandomDouble(ONE_TIME_MAG_COST_MIN,ONE_TIME_MAG_COST_MAX)) << endl;

	}//for (int ii = i_par_index; ii < dParameters[2] + i_end_of_data; ii++)
	i_par_index++;
	//one-time costs

	//profit
	i_index = 0;
	i_end_of_data = i_par_index;
	for (int ii = i_par_index; ii < i_shops + i_end_of_data; ii++)
	{
		cout << "ustawianie przychodow: " << this->iSetProfit(i_index++, c_rand.dRandomDouble(INCOME_MIN,INCOME_MAX)) << endl;
	}//for (int ii = i_par_index; ii < dParameters[3] + i_end_of_data; ii++)
	//profit

	i_par_index++;

	//min, max
	this->vSetDelFacMin(c_rand.dRandomDoubleMin(DEL_FAC_MIN_MAX));
	this->vSetDelFacMax(c_rand.dRandomDoubleMax(DEL_FAC_MAX_MIN));
	this->vSetFacMagMin(c_rand.dRandomDoubleMin(FAC_MAG_MIN_MAX));
	this->vSetFacMagMax(c_rand.dRandomDoubleMax(FAC_MAG_MAX_MIN));
	this->vSetMagShopMin(c_rand.dRandomDoubleMin(MAG_SHOP_MIN_MAX));
	this->vSetMagShopMax(c_rand.dRandomDoubleMax(MAG_SHOP_MAX_MIN));
	//min, max
}//void CMscnProblem::vGenerateInstance(int iInstanceSeed)

void CMscnProblem::vSetNumbers(int iD, int iF, int iM, int iS)
{
	cout << iSetDeliverers(iD) << endl;
	cout << iSetFactories(iF) << endl;
	cout << iSetMagazines(iM) << endl;
	cout << iSetShops(iS) << endl;
}//void CMscnProblem::vSetNumbers(int iD, int iF, int iM, int iS)

void CMscnProblem::vFixSolution(double* pdSol)
{
	int* pd_del_fac_sums = new int[iGetFactories()];
	int* pd_fac_mag_sums = new int[iGetMagazines()];
	int* pd_mag_shop_sums = new int[iGetShops()];
	int d_from_one;
	for (int ik = 0; ik < iGetFactories(); ik++)
	{
		pd_del_fac_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetFactories(); ik++)

	for (int ik = 0; ik < iGetMagazines(); ik++)
	{
		pd_fac_mag_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetMagazines(); ik++)

	for (int ik = 0; ik < iGetShops(); ik++)
	{
		pd_mag_shop_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetShops(); ik++)
	int i_index = 4;
	//	cout << "\n\nPoprawianie osobnika\n";
	for (int ij = 0; ij < iGetDeliverers(); ij++)
	{
		d_from_one = 0;
		for (int ik = 0; ik < iGetFactories(); ik++)
		{
			if (pdSol[i_index] < dDelFacMin())
				pdSol[i_index] = dDelFacMin();

			if (pdSol[i_index] + pd_del_fac_sums[ik] > dGetFactoriesCapacity(ik))
				pdSol[i_index] = dGetFactoriesCapacity(ik) - pd_del_fac_sums[ik];

			if (pdSol[i_index] + d_from_one > dGetDeliverersCapacity(ij))
				pdSol[i_index] = dGetDeliverersCapacity(ij) - pd_del_fac_sums[ik];

			if ((pd_del_fac_sums[ik] + pdSol[i_index]) / iGetMagazines() > dFacMagMax())
				pdSol[i_index] = (dFacMagMax()*iGetMagazines()) - pd_del_fac_sums[ik];

			if (pdSol[i_index] > dDelFacMax())
				pdSol[i_index] = dDelFacMax();

			pd_del_fac_sums[ik] += pdSol[i_index];
			d_from_one += pdSol[i_index];
			i_index++;
			
		}//for (int ik = 0; ik < pc_problem->iGetFactories(); ik++)


	
	}//for (int ij = 0; ij < pc_problem->iGetDeliverers(); ij++)

	for (int ij = 0; ij < iGetFactories(); ij++)
	{
		d_from_one = 0;
		for (int ik = 0; ik < iGetMagazines() - 1; ik++)
		{
			//minimum
			if (pdSol[i_index] < dFacMagMin())
				pdSol[i_index] = dFacMagMin();

			//maximum
			if (pdSol[i_index] > dFacMagMax())
				pdSol[i_index] = dFacMagMax();

			//magazine capacity
			if (pdSol[i_index] + pd_fac_mag_sums[ik] > dGetMagazinesCapacity(ik))
				pdSol[i_index] = dGetMagazinesCapacity(ik) - pd_fac_mag_sums[ik];

			//factory capacity
			if (pdSol[i_index] + d_from_one > dGetFactoriesCapacity(ij))
				pdSol[i_index] = dGetFactoriesCapacity(ij) - d_from_one;

			//in and out
			if (pdSol[i_index] + d_from_one > pd_del_fac_sums[ij])
				pdSol[i_index] = pd_del_fac_sums[ij] - d_from_one;

			pd_fac_mag_sums[ik] += pdSol[i_index];
			d_from_one += pdSol[i_index];
			i_index++;
		
		}//for (int ik = 0; ik < pc_problem->iGetMagazines()-1; ik++)
		if (pdSol[i_index] + d_from_one != pd_del_fac_sums[ij])
			pdSol[i_index] = pd_del_fac_sums[ij] - d_from_one;
		pd_fac_mag_sums[iGetMagazines() - 1] += pdSol[i_index];
		d_from_one += pdSol[i_index];
		i_index++;

	}//for (int ij = 0; ij < pc_problem->iGetFactories(); ij++)

	for (int ij = 0; ij < iGetMagazines(); ij++)
	{
		d_from_one = 0;
		for (int ik = 0; ik < iGetShops() - 1; ik++)
		{
			if (pdSol[i_index] < dMagShopMin())
				pdSol[i_index] = dMagShopMin();

			if (pdSol[i_index] > dMagShopMax())
				pdSol[i_index] = dMagShopMax();

			if (pdSol[i_index] + pd_mag_shop_sums[ik] > dGetShopsCapacity(ik))
				pdSol[i_index] = dGetShopsCapacity(ik) - pd_mag_shop_sums[ik];

			if (pdSol[i_index] + d_from_one > dGetMagazinesCapacity(ij))
				pdSol[i_index] = dGetMagazinesCapacity(ij) - d_from_one;

			if (pdSol[i_index] + d_from_one > pd_fac_mag_sums[ij])
				pdSol[i_index] = pd_fac_mag_sums[ij] - d_from_one;

			pd_mag_shop_sums[ik] += pdSol[i_index];
			d_from_one += pdSol[i_index];
			i_index++;
			
		}//for (int ik = 0; ik < pc_problem->iGetShops() - 1; ik++)
		if (pdSol[i_index] + d_from_one != pd_fac_mag_sums[ij])
			pdSol[i_index] = pd_fac_mag_sums[ij] - d_from_one;
		pd_mag_shop_sums[iGetShops() - 1] += pdSol[i_index];
		d_from_one += pdSol[i_index];
		i_index++;
	
	}//for (int ij = 0; ij < pc_problem->iGetMagazines(); ij++)


	delete[] pd_del_fac_sums;
	delete[] pd_fac_mag_sums;
	delete[] pd_mag_shop_sums;
}


double* CMscnProblem::dGenerateSolution(CRandom c_random)
{
	int i_number_of_elements= iGetDeliverers()*iGetFactories() + iGetFactories()*iGetMagazines() + iGetMagazines()*iGetShops() + 4;
	int* pd_del_fac_sums = new int[iGetFactories()];
	int* pd_fac_mag_sums = new int[iGetMagazines()];
	int* pd_mag_shop_sums = new int[iGetShops()];
	int d_from_one;
	int d_rand;

	double* pd_current_solution = new double[i_number_of_elements];

	for (int ik = 0; ik < iGetFactories(); ik++)
	{
		pd_del_fac_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetFactories(); ik++)

	for (int ik = 0; ik < iGetMagazines(); ik++)
	{
		pd_fac_mag_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetMagazines(); ik++)

	for (int ik = 0; ik < iGetShops(); ik++)
	{
		pd_mag_shop_sums[ik] = 0;
	}//for (int ik = 0; ik < pc_problem->iGetShops(); ik++)
	int i_index = 4;

	pd_current_solution[0] = iGetDeliverers();
	pd_current_solution[1] = iGetFactories();
	pd_current_solution[2] = iGetMagazines();
	pd_current_solution[3] = iGetShops();

	for (int ij = 0; ij < iGetDeliverers(); ij++)
	{
		for (int ik = 0; ik < iGetFactories(); ik++)
		{
			d_rand = c_random.iRandomInt(dDelFacMin()+1, dDelFacMax());
			pd_current_solution[i_index++] = d_rand;
			pd_del_fac_sums[ik] += d_rand;
		}//for (int ik = 0; ik < pc_problem->iGetFactories(); ik++)
	}//for (int ij = 0; ij < pc_problem->iGetDeliverers(); ij++)

	for (int ij = 0; ij < iGetFactories(); ij++)
	{
		d_from_one = 0;
		for (int ik = 0; ik < iGetMagazines() - 1; ik++)
		{
			d_rand = c_random.iRandomInt(dFacMagMin()+1, pd_del_fac_sums[ij]);
			pd_current_solution[i_index++] = d_rand;
			pd_fac_mag_sums[ik] += d_rand;
			d_from_one += d_rand;
		}//for (int ik = 0; ik < pc_problem->iGetMagazines()-1; ik++)
		pd_current_solution[i_index++] = pd_del_fac_sums[ij] - d_from_one;
		pd_fac_mag_sums[iGetMagazines() - 1] += pd_del_fac_sums[ij] - d_from_one;
	}//for (int ij = 0; ij < pc_problem->iGetFactories(); ij++)

	for (int ij = 0; ij < iGetMagazines(); ij++)
	{
		d_from_one = 0;
		for (int ik = 0; ik < iGetShops() - 1; ik++)
		{
			d_rand = c_random.iRandomInt(dMagShopMin()+1, pd_fac_mag_sums[ij]);
			pd_current_solution[i_index++] = d_rand;
			pd_mag_shop_sums[ik] += d_rand;
			d_from_one += d_rand;
		}//for (int ik = 0; ik < pc_problem->iGetShops() - 1; ik++)
		pd_current_solution[i_index++] = pd_fac_mag_sums[ij] - d_from_one;
	}//for (int ij = 0; ij < pc_problem->iGetMagazines(); ij++)


	delete[] pd_del_fac_sums;
	delete[] pd_fac_mag_sums;
	delete[] pd_mag_shop_sums;


	
	return pd_current_solution;
}//double* dGenerateSolution(CRandom c_random)