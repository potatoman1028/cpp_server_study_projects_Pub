#pragma once

class CLog
{
public:
	static BOOL	WriteLog(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32]					= {0,};
		TCHAR		CurrentFileName[MAX_PATH]		= {0,};
		FILE*		FilePtr							= NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH]		= {0,};

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH]	= {0,};

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay, 
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"), 
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay,
			SystemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %s\n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}

	static BOOL	WriteLogNoDate(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32]					= {0,};
		TCHAR		CurrentFileName[MAX_PATH]		= {0,};
		FILE*		FilePtr							= NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH]		= {0,};

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH]	= {0,};

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay, 
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"), 
			SystemTime.wYear, 
			SystemTime.wMonth, 
			SystemTime.wDay,
			SystemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("%s"), Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("%s"), Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};
