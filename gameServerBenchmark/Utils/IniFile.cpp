#include "stdafx.h"
#include "Log.h"
#include "IniFile.h"

BOOL CIniFile::SetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, LPCWSTR lpcwValue)
{
	if (!lpcwFileName || lpcwAppName || lpcwKeyName || lpcwValue)
		return FALSE;

	if (!WritePrivateProfileString(lpcwAppName, lpcwKeyName, lpcwValue, lpcwFileName))
		return FALSE;

	return TRUE;
}

BOOL CIniFile::SetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, DWORD dwValue)
{
	if (!lpcwFileName || !lpcwAppName || lpcwKeyName)
		return FALSE;

	WCHAR wszValue[16] = {0,};
	_itow(dwValue, wszValue, 10);

	if (!WritePrivateProfileString(lpcwAppName, lpcwKeyName, wszValue, lpcwFileName))
		return FALSE;

	return TRUE;
}

BOOL CIniFile::SetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, FLOAT fValue)
{
	if (!lpcwFileName || !lpcwAppName || !lpcwKeyName)
		return FALSE;

	WCHAR wszValue[16] = {0,};
	_snwprintf(wszValue, 16, L"%f", fValue);

	if (!WritePrivateProfileString(lpcwAppName, lpcwKeyName, wszValue, lpcwFileName))
		return FALSE;

	return TRUE;
}

BOOL CIniFile::GetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, LPDWORD lpdwValue)
{
	if (!lpcwFileName || !lpcwAppName || !lpcwKeyName || lpdwValue)
		return FALSE;

	*lpdwValue = GetPrivateProfileInt(lpcwAppName, lpcwKeyName, 0, lpcwFileName);
	
	return TRUE;
}

BOOL CIniFile::GetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, LPWSTR lpwValue, LPDWORD lpdwBufLen)
{
	if (!lpcwFileName || !lpcwAppName || !lpcwKeyName || !lpwValue || !lpdwBufLen)
		return FALSE;

	*lpdwBufLen = GetPrivateProfileString(lpcwAppName, lpcwKeyName, L"", lpwValue, *lpdwBufLen, lpcwFileName);

	return TRUE;
}

BOOL CIniFile::GetValue(LPCWSTR lpcwFileName, LPCWSTR lpcwAppName, LPCWSTR lpcwKeyName, FLOAT *pfValue)
{
	if (!lpcwFileName || !lpcwAppName || !lpcwKeyName || !pfValue)
		return FALSE;

	WCHAR wszValue[16] = {0,};
	GetPrivateProfileString(lpcwAppName, lpcwKeyName, L"0", wszValue, 16, lpcwFileName);
	*pfValue = (FLOAT) _wtof(wszValue);

	return TRUE;
}