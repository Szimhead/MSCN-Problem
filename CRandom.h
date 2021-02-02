#pragma once
#include "pch.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

class CRandom
{
public:
	CRandom();
	CRandom(int iSeed);

	int iRandomIntMin(int iMin);
	int iRandomIntMax(int iMax);
	int iRandomInt(int iMin, int iMax);

	double dRandomDoubleMin(double dMin);
	double dRandomDoubleMax(double dMax);
	double dRandomDouble(double dMin, double dMax);
};//class CRandom