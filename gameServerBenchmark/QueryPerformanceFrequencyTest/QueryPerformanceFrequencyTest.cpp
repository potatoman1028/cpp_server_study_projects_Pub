// QueryPerformanceFrequencyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LARGE_INTEGER liFrequency, liStart, liEnd;

	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liStart);

	DWORD dwData = 0;
	for (INT i=0;i<210000000;i++)
		dwData += i % 3;

	QueryPerformanceCounter(&liEnd);
	double Result = ((double)liEnd.QuadPart) / liFrequency.QuadPart - ((double)liStart.QuadPart) / liFrequency.QuadPart;
	printf("%4.2f\n", Result);
	getchar();

	return 0;
}

