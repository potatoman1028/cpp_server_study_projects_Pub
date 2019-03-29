// DynamicAllocation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"

CProfile g_oProfile;

#define TESTOBJ_CNT 3000
#define TESTLOOP_CNT 10

class C1024Bytes : public CMemoryPool<C1024Bytes>
{
	BYTE m_szBuf[1024];
};

class C10240Bytes : public CMemoryPool<C10240Bytes>
{
	BYTE m_szBuf[10240];
};

class C102400Bytes : public CMemoryPool<C102400Bytes>
{
	BYTE m_szBuf[102400];
};

std::list<C1024Bytes*> g_v1024Bytes;
std::list<C10240Bytes*> g_v10240Bytes;
std::list<C102400Bytes*> g_v102400Bytes;

VOID Benchmark (VOID)
{
	g_oProfile.Begin(L"DynamicAllocation Benchmark Begin");

	g_oProfile.Begin(L"DynamicAllocation Benchmark 1024B Allocation Loop");
	for (DWORD i=0;i<TESTOBJ_CNT;i++)
	{
		g_oProfile.Begin(L"DynamicAllocation Benchmark 1024B Allocation");
		C1024Bytes *p1024Bytes = new C1024Bytes();
		g_oProfile.End(L"DynamicAllocation Benchmark 1024B Allocation");
		g_v1024Bytes.push_back(p1024Bytes);
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 1024B Allocation Loop");

	g_oProfile.Begin(L"DynamicAllocation Benchmark 10240B Allocation Loop");
	for (DWORD i=0;i<TESTOBJ_CNT;i++)
	{
		g_oProfile.Begin(L"DynamicAllocation Benchmark 10240B Allocation");
		C10240Bytes *p10240Bytes = new C10240Bytes();
		g_oProfile.End(L"DynamicAllocation Benchmark 10240B Allocation");
		g_v10240Bytes.push_back(p10240Bytes);
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 10240B Allocation Loop");

	g_oProfile.Begin(L"DynamicAllocation Benchmark 102400B Allocation Loop");
	for (DWORD i=0;i<TESTOBJ_CNT;i++)
	{
		g_oProfile.Begin(L"DynamicAllocation Benchmark 102400B Allocation");
		C102400Bytes *p102400Bytes = new C102400Bytes();
		g_oProfile.End(L"DynamicAllocation Benchmark 102400B Allocation");
		g_v102400Bytes.push_back(p102400Bytes);
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 102400B Allocation Loop");


	g_oProfile.Begin(L"DynamicAllocation Benchmark 1024B Deallocation Loop");
	for (std::list<C1024Bytes*>::iterator it=g_v1024Bytes.begin();it!=g_v1024Bytes.end();it++)
	{
		C1024Bytes *p1024Bytes = (C1024Bytes*)(*it);
		g_oProfile.Begin(L"DynamicAllocation Benchmark 1024B Deallocation");
		delete p1024Bytes;
		g_oProfile.End(L"DynamicAllocation Benchmark 1024B Deallocation");
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 1024B Deallocation Loop");
	g_v1024Bytes.clear();

	g_oProfile.Begin(L"DynamicAllocation Benchmark 10240B Deallocation Loop");
	for (std::list<C10240Bytes*>::iterator it=g_v10240Bytes.begin();it!=g_v10240Bytes.end();it++)
	{
		C10240Bytes *p10240Bytes = (C10240Bytes*)(*it);
		g_oProfile.Begin(L"DynamicAllocation Benchmark 10240B Deallocation");
		delete p10240Bytes;
		g_oProfile.End(L"DynamicAllocation Benchmark 10240B Deallocation");
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 10240B Deallocation Loop");
	g_v10240Bytes.clear();

	g_oProfile.Begin(L"DynamicAllocation Benchmark 102400B Deallocation Loop");
	for (std::list<C102400Bytes*>::iterator it=g_v102400Bytes.begin();it!=g_v102400Bytes.end();it++)
	{
		C102400Bytes *p102400Bytes = (C102400Bytes*)(*it);
		g_oProfile.Begin(L"DynamicAllocation Benchmark 102400B Deallocation");
		delete p102400Bytes;
		g_oProfile.End(L"DynamicAllocation Benchmark 102400B Deallocation");
	}
	g_oProfile.End(L"DynamicAllocation Benchmark 102400B Deallocation Loop");
	g_v102400Bytes.clear();

	g_oProfile.End(L"DynamicAllocation Benchmark Begin");
}

int _tmain(int argc, _TCHAR* argv[])
{
	LARGE_INTEGER liFrequency, liStart, liEnd;

	printf("Press any key.. for start");
	getchar();

	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liStart);

	g_oProfile.Begin(L"DynamicAllocation Begin");
	g_oProfile.Begin(L"DynamicAllocation First Benchmark");
	Benchmark();
	g_oProfile.End(L"DynamicAllocation First Benchmark");

	g_oProfile.Begin(L"DynamicAllocation Benchmark Loop");
	for (DWORD i=0;i<TESTLOOP_CNT;i++)
		Benchmark();
	g_oProfile.End(L"DynamicAllocation Benchmark Loop");
	
	g_oProfile.End(L"DynamicAllocation Begin");
	
	QueryPerformanceCounter(&liEnd);
	double Result = ((double)liEnd.QuadPart) / liFrequency.QuadPart - ((double)liStart.QuadPart) / liFrequency.QuadPart;
	CLog::WriteLog(L"Total Time : %4.2f\n", Result);
	g_oProfile.DumpProfileOutput();
	getchar();

	return 0;
}

