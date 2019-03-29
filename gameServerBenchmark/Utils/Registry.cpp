#include "StdAfx.h"
#include "Registry.h"

CRegistry::CRegistry(VOID)
{
	m_hRootKey = NULL;
}

CRegistry::~CRegistry(VOID)
{
}

BOOL CRegistry::Open(HKEY hRootKey, LPWSTR lpwSubKey)
{
	if (!hRootKey || !lpwSubKey)
		return FALSE;

	if (RegOpenKey(hRootKey, lpwSubKey, &m_hRootKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::Close(VOID)
{
	if (RegCloseKey(m_hRootKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::CreateKey(HKEY hRootKey, LPWSTR lpwSubKey)
{
	if (!hRootKey || !lpwSubKey)
		return FALSE;

	if (RegCreateKey(hRootKey, lpwSubKey, &m_hRootKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::DeleteKey(HKEY hRootKey, LPWSTR lpwSubKey)
{
	if (!hRootKey || !lpwSubKey)
		return FALSE;

	if (RegDeleteKey(hRootKey, lpwSubKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::SetValue(LPWSTR lpwValueName, LPWSTR lpwValue)
{
	if (!m_hRootKey || !lpwValueName || !lpwValue)
		return FALSE;

	CHAR szTemp[256] = {0,};
	if (RegSetValueEx(m_hRootKey, lpwValueName, 0, REG_SZ, (BYTE*) W2M(lpwValue, szTemp, sizeof(szTemp)), (DWORD) strlen(szTemp)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::SetValue(LPWSTR lpwValueName, DWORD dwValue)
{
	if (!m_hRootKey || !lpwValueName)
		return FALSE;

	WCHAR wszValue[16] = {0,};
	_itow(dwValue, wszValue, 10);

	if (RegSetValueEx(m_hRootKey, lpwValueName, 0, REG_DWORD, (BYTE*) &dwValue, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::GetValue(LPWSTR lpwValueName, LPWSTR lpwValue, LPDWORD lpdwBufLen)
{
	if (!m_hRootKey || !lpwValueName || !lpwValue || !lpdwBufLen)
		return FALSE;

	DWORD dwValueType = 0;
	CHAR szTemp[256] = {0,};
	if (RegQueryValueEx(m_hRootKey, lpwValueName, 0, &dwValueType, (BYTE*) szTemp, lpdwBufLen) != ERROR_SUCCESS)
		return FALSE;

	M2W(szTemp, lpwValue, *lpdwBufLen);

	return TRUE;
}

BOOL CRegistry::GetValue(LPWSTR lpwValueName, LPDWORD lpdwValue)
{
	if (!m_hRootKey || !lpwValueName || !lpdwValue)
		return FALSE;

	DWORD dwValueType = 0;
	DWORD dwBufLen = sizeof(DWORD);

	if (RegQueryValueEx(m_hRootKey, lpwValueName, 0, &dwValueType, (BYTE*) lpdwValue, &dwBufLen) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL CRegistry::DeleteValue(LPWSTR lpwValueName)
{
	if (!m_hRootKey || !lpwValueName)
		return FALSE;

	if (RegDeleteValue(m_hRootKey, lpwValueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}