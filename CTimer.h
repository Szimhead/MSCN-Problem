#pragma once
#include "pch.h"
#include <windows.h>
#include <iostream>

using namespace std;

class CTimer
{
public:
	CTimer() ;

	void vStartTime();
	void vStopTime() ;
	double dGetTime();

private:
	double d_seconds;
	LARGE_INTEGER li_start;
	LARGE_INTEGER li_end;
	LARGE_INTEGER li_freq;
};
