#include "StdAfx.h"
#include "MiniDump.h"

#define SERVER_DUMP_FOLDER L"C:\\Log\\FTO\\%s"

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER g_pPrevExceptionFilter = NULL;

LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	HMODULE hDllHandle = NULL;
	hDllHandle = LoadLibrary(L"DBGHELP.DLL");

	if (hDllHandle)
	{
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP) GetProcAddress(hDllHandle, "MiniDumpWriteDump");
		if (Dump)
		{
			WCHAR wszDumpPath[MAX_PATH] = {0,};
			SYSTEMTIME st;

			GetLocalTime(&st);

#ifdef _SERVER
			_snwprintf(wszDumpPath, MAX_PATH, L"%s\\%s_%d-%d-%d %d_%d_%d.dmp",
				LOGFOLDER_NAME,
				COMPUTER_NAME,
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond);
#else
			_snwprintf(wszDumpPath, MAX_PATH, L"%d-%d-%d %d_%d_%d.dmp",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond);
#endif
			HANDLE hDumpFile = CreateFile(
				wszDumpPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hDumpFile != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION mei;
				mei.ThreadId = GetCurrentThreadId();
				mei.ExceptionPointers = pExceptionInfo;
				mei.ClientPointers = NULL;

				BOOL bSucc = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					hDumpFile,
					MiniDumpNormal,
					&mei,
					NULL,
					NULL);
				if (bSucc)
				{
					CloseHandle(hDumpFile);
					return EXCEPTION_EXECUTE_HANDLER;
				}
			}
			
			CloseHandle(hDumpFile);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

VOID CMiniDump::Begin(VOID)
{
	SetErrorMode(SEM_FAILCRITICALERRORS);
	g_pPrevExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);
}

VOID CMiniDump::End(VOID)
{
	SetUnhandledExceptionFilter(g_pPrevExceptionFilter);
}