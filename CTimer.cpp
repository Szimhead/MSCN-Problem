#include "pch.h"
#include "CTimer.h"

CTimer::CTimer() { 
	d_seconds = 0; 
	QueryPerformanceFrequency(&li_freq); 
}

void CTimer::vStartTime() 
{
	QueryPerformanceCounter(&li_start);
}

void CTimer::vStopTime() 
{
	QueryPerformanceCounter(&li_end);
}

double CTimer::dGetTime()
{
	return (((li_end.QuadPart - li_start.QuadPart)*1.0) / li_freq.QuadPart);
}