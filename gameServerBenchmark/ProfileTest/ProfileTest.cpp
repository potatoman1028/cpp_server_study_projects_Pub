// ProfileTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Profile.h"

CProfile g_oProfile;

VOID Test(VOID)
{
	int i, data;
	g_oProfile.Begin(L"Test1");	
	for (i=0;i<1000000;i++)
	{
		data = i * 2;
		data &= 0xfffffff;
	}
	g_oProfile.End(L"Test1");
	g_oProfile.Begin(L"Test2");	
	for (i=0;i<2000000;i++)
	{
		data = i * 2;
		data &= 0xfffffff;
	}
	g_oProfile.End(L"Test2");
	g_oProfile.Begin(L"Test3");	
	for (i=0;i<1000000;i++)
	{
		data = i * 2;
		data &= 0xfffffff;
	}
	g_oProfile.End(L"Test3");
}

int _tmain(int argc, _TCHAR* argv[])
{
	g_oProfile.Begin(L"Test0");	
	Test();
	g_oProfile.End(L"Test0");

	g_oProfile.DumpProfileOutput();
	getchar();

	return 0;
}

