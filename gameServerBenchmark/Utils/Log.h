#pragma once

#define MAX_LOG_LENGTH 1024
#define SERVER_LOG_FOLDER L"C:\\Log\\FTO\\%s"

class CLog
{
public:
	static VOID WriteLog(LPWSTR lpwLog, ...)
	{
		va_list ap;
		SYSTEMTIME st;
		FILE *pFile = NULL;
		WCHAR wszCurDate[32] = {0,};
		WCHAR wszLogFileName[MAX_PATH] = {0,};
		WCHAR wszDebugWndLog[MAX_LOG_LENGTH] = {0, };
		WCHAR wszLog[MAX_LOG_LENGTH] = {0,};

		va_start(ap, lpwLog);
		vswprintf(wszLog, lpwLog, ap);
		va_end(ap);

		GetLocalTime(&st);
		_snwprintf(wszCurDate, 32, L"%d-%d-%d %d:%d:%d",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour,
			st.wMinute,
			st.wSecond);
#ifdef _SERVER
		_snwprintf(wszLogFileName, MAX_PATH, L"%s\\%s_LOG_%d-%d-%d %d.log",
			LOGFOLDER_NAME,
			COMPUTER_NAME,
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour);

		pFile = _wfopen(wszLogFileName, L"a");
		if (!pFile)
			return;

		fwprintf(pFile, L"[%s] %s\n", wszCurDate, wszLog);
		_snwprintf(wszDebugWndLog, MAX_LOG_LENGTH, L"[%s] %s", wszCurDate, wszLog);
		
		fflush(pFile);
		fclose(pFile);

		wprintf(L"%s", wszDebugWndLog);
#else
		_snwprintf(wszLogFileName, MAX_PATH, L"LOG_%d-%d-%d %d.log",
			st.wYear,
			st.wMonth,
			st.wDay,
			st.wHour);

		pFile = _wfopen(wszLogFileName, L"a");
		if (!pFile)
			return;

		fwprintf(pFile, L"[%s] %s", wszCurDate, wszLog);
		_snwprintf(wszDebugWndLog, MAX_LOG_LENGTH, L"[%s] %s", wszCurDate, wszLog);

		fflush(pFile);
		fclose(pFile);

		wprintf(L"%s", wszDebugWndLog);
#endif
	}
};