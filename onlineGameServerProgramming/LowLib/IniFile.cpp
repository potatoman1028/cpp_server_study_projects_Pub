#include "stdafx.h"
#include "IniFile.h"

CIniFile::CIniFile(VOID)
{
	ZeroMemory(mFileName, sizeof(mFileName));
}

CIniFile::~CIniFile(VOID)
{
}

BOOL CIniFile::Open(LPCTSTR fileName)
{
	if (!fileName)
		return FALSE;

	_tcsncpy(mFileName, fileName, MAX_PATH);

	return TRUE;
}

BOOL CIniFile::Close(VOID)
{
	return TRUE;
}

BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, mFileName);

	return TRUE;
}

BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT *value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = {0,};

	GetPrivateProfileString(keyName, valueName, _T(""), Value, 16, mFileName);

	*value = (FLOAT) _tstof(Value);

	return TRUE;
}

BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	if (!keyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(keyName, valueName, _T(""), value, *bufferLength, mFileName);

	return TRUE;
}

BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16]	= {0,};
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}

BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, mFileName);

	return TRUE;
}

BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	TCHAR Value[16]	= {0,};
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, Value, mFileName);

	return TRUE;
}