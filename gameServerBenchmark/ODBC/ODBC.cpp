// ODBC.cpp : Defines the entry point for the console application.
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

int _tmain(int argc, _TCHAR* argv[])
{
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	SQLINTEGER cbID = 0, cbName = SQL_NTS, cbPassword = SQL_NTS, cbDes = SQL_NTS;

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	SQLSetConnectAttr(hdbc, 5, (void *)SQL_LOGIN_TIMEOUT, 0);
	retcode = SQLConnect(hdbc, L"Video", SQL_NTS, L"sa", SQL_NTS, L"", SQL_NTS);
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	WCHAR **wszInsertQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszInsertQuery[i] = new WCHAR[100];

	WCHAR **wszUpdateQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszUpdateQuery[i] = new WCHAR[100];

	WCHAR **wszDeleteQuery = new WCHAR*[TESTQUERY_CNT];
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		wszDeleteQuery[i] = new WCHAR[100];

	
	//WCHAR wszInsertQuery[TESTQUERY_CNT][100] = {0,};
	//WCHAR wszUpdateQuery[TESTQUERY_CNT][100] = {0,};
	//WCHAR wszDeleteQuery[TESTQUERY_CNT][100] = {0,};

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		wsprintf(wszInsertQuery[i], L"INSERT INTO CUSTOMER (strName, strPhone, strAddress, strRegidentID) VALUES (%d, %d, %d, %d)", i, i, i, i);
		wsprintf(wszUpdateQuery[i], L"UPDATE CUSTOMER SET strAddress='%d' WHERE strName='%d'", i+1, i);
		wsprintf(wszDeleteQuery[i], L"DELETE FROM CUSTOMER WHERE strName='%d'", i);
	}

	printf("Press any key.. for start");
	getchar();

	LARGE_INTEGER liFrequency, liStart, liEnd;
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liStart);
	g_oProfile.Begin(L"ODBC Query");

	g_oProfile.Begin(L"ODBC Query Insert Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"ODBC Query Insert");
		retcode = SQLExecDirect(hstmt, wszInsertQuery[i], SQL_NTS);	
		g_oProfile.End(L"ODBC Query Insert");
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			printf("ERROR\n");
	}
	g_oProfile.End(L"ODBC Query Insert Loop");

	g_oProfile.Begin(L"ODBC Query Update Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"ODBC Query Update");
		retcode = SQLExecDirect(hstmt, wszUpdateQuery[i], SQL_NTS);	
		g_oProfile.End(L"ODBC Query Update");
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			printf("ERROR\n");
	}
	g_oProfile.End(L"ODBC Query Update Loop");

	g_oProfile.Begin(L"ODBC Query Delete Loop");
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		g_oProfile.Begin(L"ODBC Query Delete");
		retcode = SQLExecDirect(hstmt, wszDeleteQuery[i], SQL_NTS);	
		g_oProfile.End(L"ODBC Query Delete");
		if(retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
			printf("ERROR\n");
	}
	g_oProfile.End(L"ODBC Query Delete Loop");

	g_oProfile.End(L"ODBC Query");

	QueryPerformanceCounter(&liEnd);
	double Result = ((double)liEnd.QuadPart) / liFrequency.QuadPart - ((double)liStart.QuadPart) / liFrequency.QuadPart;
	CLog::WriteLog(L"Total Time : %4.2f\n", Result);
	g_oProfile.DumpProfileOutput();
	getchar();

	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszInsertQuery[i];
	delete[] wszInsertQuery;

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszUpdateQuery[i];
	delete[] wszUpdateQuery;

	for (DWORD i=0;i<TESTQUERY_CNT;i++)
		delete[] wszDeleteQuery[i];
	delete[] wszDeleteQuery;

	return NULL;
}

