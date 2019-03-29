// GetTickCountTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwStart = GetTickCount();

	DWORD dwData = 0;
	for (INT i=0;i<210000000;i++)
		dwData += i % 3;

	printf("%4.2f\n", (double)(GetTickCount() - dwStart) / 1000);
	getchar();

	return 0;
}

