#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <shlobj.h>
#include <dbghelp.h>
#include <assert.h>

#define MAX_BUF_LEN	1024
#define MAX_BUF_LEN_EXTRA MAX_BUF_LEN * 4

#define LOGFOLDER_NAME g_oUtilMisc._GetLogFolderName()
#define COMPUTER_NAME g_oUtilMisc._GetComputerName()

#define M2W(lpSrc, lpwDest, dwBufLen) g_oUtilMisc._M2W(lpSrc, lpwDest, dwBufLen)
#define W2M(lpwSrc, lpDest, dwBufLen) g_oUtilMisc._W2M(lpwSrc, lpDest, dwBufLen)

class CUtilMisc
{
public:
	CUtilMisc()
	{
		ZeroMemory(m_wszLogFolderName, sizeof(m_wszLogFolderName));
		ZeroMemory(m_wszComputerName, sizeof(m_wszComputerName));

		WCHAR wszExeFullPathName[MAX_PATH] = {0,};
		if (!GetModuleFileName(NULL, wszExeFullPathName, MAX_PATH))
			return;

		WCHAR wszExeFolderName[MAX_PATH] = {0,};
		LPWSTR lpwExeFileName = NULL;
		WCHAR wszExeFileName[MAX_PATH] = {0,};

		if (!GetFullPathName(wszExeFullPathName, MAX_PATH, wszExeFolderName, &lpwExeFileName))
			return;

		wcsncpy(wszExeFileName, lpwExeFileName, wcslen(lpwExeFileName) - 4);

		DWORD dwComputerNameLen = MAX_PATH;
		if (!GetComputerName(m_wszComputerName, &dwComputerNameLen))
			return;

		_snwprintf(m_wszLogFolderName, MAX_PATH, L"C:\\Log\\FTO\\%s", wszExeFileName);
		if (SHCreateDirectoryEx(NULL, m_wszLogFolderName, NULL) != ERROR_SUCCESS)
			return;
	}

	LPWSTR _GetLogFolderName(VOID) { return m_wszLogFolderName; }
	LPWSTR _GetComputerName(VOID) { return m_wszComputerName; }

	LPWSTR _M2W(LPCSTR lpSrc, LPWSTR lpwDest, DWORD dwBufLen)
	{
		MultiByteToWideChar(CP_ACP, 0, lpSrc, -1, lpwDest, dwBufLen);
		return lpwDest;
	}

	LPSTR _W2M(LPWSTR lpwSrc, LPSTR lpDest, DWORD dwBufLen)
	{
		WideCharToMultiByte(CP_ACP, 0, lpwSrc, -1, lpDest, dwBufLen, NULL, NULL);
		return lpDest;
	}

private:
	WCHAR m_wszLogFolderName[MAX_PATH];
	WCHAR m_wszComputerName[MAX_PATH];
};

static CUtilMisc g_oUtilMisc;