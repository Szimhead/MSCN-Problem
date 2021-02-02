#include "pch.h"
#include "CRandom.h"

CRandom::CRandom()
{
	srand(time(NULL));
}//CRandom::CRandom()

CRandom::CRandom(int iSeed)
{
	srand(iSeed);
}//CRandom::CRandom(int iSeed)

int CRandom::iRandomIntMin(int iMin)
{
	return rand() + iMin;
}//int CRandom::iRandomIntMin(int iMin)

int CRandom::iRandomIntMax(int iMax)
{
	return iMax - rand();
}//int CRandom::iRandomIntMax(int iMax)

int CRandom::iRandomInt(int iMin, int iMax)
{
	return rand() % (iMax - iMin) + iMin;
}//int CRandom::iRandomInt(int iMin, int iMax)

double CRandom::dRandomDoubleMin(double dMin)
{
	return (double) RAND_MAX / rand() + dMin;
}//double CRandom::dRandomDoubleMin(double dMin)

double CRandom::dRandomDoubleMax(double dMax)
{
	return (double)rand() / RAND_MAX * dMax;
}//double CRandom::dRandomDoubleMax(double dMax)

double CRandom::dRandomDouble(double dMin, double dMax)
{
	return (double)rand() / RAND_MAX * (dMax - dMin) + dMin;
}//double CRandom::dRandomDouble(double dMin, double dMax)
