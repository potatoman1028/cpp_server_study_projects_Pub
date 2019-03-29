// OLEDB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"

#define TESTQUERY_CNT 3000

CProfile g_oProfile;

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);

	WCHAR **wszInsertQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszInsertQuery[i] = new WCHAR[100];

	WCHAR **wszUpdateQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszUpdateQuery[i] = new WCHAR[100];

	WCHAR **wszDeleteQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszDeleteQuery[i] = new WCHAR[100];

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		wsprintf(wszInsertQuery[i], L"INSERT INTO CUSTOMER (strName, strPhone, strAddress, strRegidentID) VALUES (%d, %d, %d, %d)", i, i, i, i);
		wsprintf(wszUpdateQuery[i], L"UPDATE CUSTOMER SET strAddress='%d' WHERE strName='%d'", i+1, i);
		wsprintf(wszDeleteQuery[i], L"DELETE FROM CUSTOMER WHERE strName='%d'", i);
	}

    _ConnectionPtr pConnection  = NULL;
	_RecordsetPtr pRecordset = NULL;

	TESTHR(pConnection.CreateInstance(__uuidof(Connection))); 
	_bstr_t strCnn("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\\Video.mdb;");
	TESTHR(pConnection->Open(strCnn, L"", L"", -1));

	printf("Press any key.. for start");
	getchar();

	LARGE_INTEGER liFrequency, liStart, liEnd;
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liStart);

	g_oProfile.Begin(L"OLEDB Query");
	
	g_oProfile.Begin(L"OLEDB Query Insert Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"OLEDB Query Insert");
		pRecordset = pConnection->Execute(wszInsertQuery[i], NULL, adOptionUnspecified); 
		g_oProfile.End(L"OLEDB Query Insert");
	}
	g_oProfile.End(L"OLEDB Query Insert Loop");

	g_oProfile.Begin(L"OLEDB Query Update Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"OLEDB Query Update");
		pRecordset = pConnection->Execute(wszUpdateQuery[i], NULL, adOptionUnspecified); 
		g_oProfile.End(L"OLEDB Query Update");
	}
	g_oProfile.End(L"OLEDB Query Update Loop");

	g_oProfile.Begin(L"OLEDB Query Delete Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"OLEDB Query Delete");
		pRecordset = pConnection->Execute(wszDeleteQuery[i], NULL, adOptionUnspecified); 
		g_oProfile.End(L"OLEDB Query Delete");
	}
	g_oProfile.End(L"OLEDB Query Delete Loop");

	//pRecordset->Close();
	//pConnection->Close();
	//pConnection.Release();

	g_oProfile.End(L"OLEDB Query");

	QueryPerformanceCounter(&liEnd);
	double Result = ((double)liEnd.QuadPart) / liFrequency.QuadPart - ((double)liStart.QuadPart) / liFrequency.QuadPart;
	CLog::WriteLog(L"Total Time : %4.2f\n", Result);
	g_oProfile.DumpProfileOutput();
	getchar();

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszInsertQuery[i];
	delete[] wszInsertQuery;

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszUpdateQuery[i];
	delete[] wszUpdateQuery;

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszDeleteQuery[i];
	delete[] wszDeleteQuery;

	::CoUninitialize();

	return 0;
}

