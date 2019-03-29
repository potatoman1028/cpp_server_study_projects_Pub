// ODBCPooling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/CircularQueue.h"
#include "../Utils/Log.h"
#include "../Utils/Profile.h"

#define TESTQUERY_CNT 1000
#define POOLING_CNT 3

class CQuery : public CMemoryPool<CQuery>
{
public:
	CQuery()
	{
		m_pQuery = NULL;
	}

	WCHAR *m_pQuery;
};

BOOL g_bThreadStopFlag = TRUE;
std::vector<HANDLE> g_vThreadHandle;
CCircularQueue<CQuery*> g_qQuery;

DWORD WINAPI DBThreadCallback(LPVOID lpParam)
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

	while (g_bThreadStopFlag)
	{
		CQuery *pQuery = NULL;
		if (g_qQuery.Pop(pQuery))
		{
			retcode = SQLExecDirect(hstmt, pQuery->m_pQuery, SQL_NTS);	
			delete pQuery;
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
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

	for (DWORD i=0;i<POOLING_CNT;i++)
	{
		HANDLE hThread = CreateThread(NULL, 0, ::DBThreadCallback, NULL, 0, NULL);
		g_vThreadHandle.push_back(hThread);
	}

	//
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		CQuery *pInsertQuery = new CQuery();
		pInsertQuery->m_pQuery = wszInsertQuery[i];
		g_qQuery.Push(pInsertQuery);
	}
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		CQuery *pUpdateQuery = new CQuery();
		pUpdateQuery->m_pQuery = wszUpdateQuery[i];
		g_qQuery.Push(pUpdateQuery);
	}
	for (DWORD i=0;i<TESTQUERY_CNT;i++)
	{
		CQuery *pUpdateQuery = new CQuery();
		pUpdateQuery->m_pQuery = wszUpdateQuery[i];
		g_qQuery.Push(pUpdateQuery);
	}

	printf("Push End");
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

	for (DWORD i=0;i<POOLING_CNT;i++)
		CloseHandle(g_vThreadHandle[i]);

	g_vThreadHandle.clear();

	return 0;
}

