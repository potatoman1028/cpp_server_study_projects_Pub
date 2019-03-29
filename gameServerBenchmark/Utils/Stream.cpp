#include "stdafx.h"
#include "CriticalSection.h"
#include "StaticSyncParent.h"
#include "MemoryPool.h"
#include "Stream.h"

CStream::CStream(VOID)
{
	m_pucBuf	= NULL;
	m_dwLen			= 0;
}

CStream::~CStream(VOID)
{
}

BOOL CStream::SetBuffer(BYTE *pucBuf)
{
	if (!pucBuf)
		return FALSE;

	m_pucBuf = pucBuf;
	m_dwLen	= 0;

	return TRUE;
}

BOOL CStream::SetBuffer()
{
	if (!m_pucBuf)
		return FALSE;

	m_dwLen			= 0;

	return TRUE;
}

BOOL CStream::ReadInt32(INT *piData)
{
	memcpy(piData, m_pucBuf + m_dwLen, sizeof(INT));

	m_dwLen += sizeof(INT);

	return TRUE;
}

BOOL CStream::ReadDWORD(DWORD *pdwData)
{
	memcpy(pdwData, m_pucBuf + m_dwLen, sizeof(DWORD));

	m_dwLen += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::ReadDWORD_PTR(DWORD_PTR *pdwData)
{
	memcpy(pdwData, m_pucBuf + m_dwLen, sizeof(DWORD_PTR));

	m_dwLen += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::ReadByte(BYTE *pucData)
{
	memcpy(pucData, m_pucBuf + m_dwLen, sizeof(BYTE));

	m_dwLen += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::ReadBytes(BYTE *pucData, DWORD length)
{
	memcpy(pucData, m_pucBuf + m_dwLen, length);

	m_dwLen += length;

	return TRUE;
}

BOOL CStream::ReadFloat(FLOAT *pfData)
{
	memcpy(pfData, m_pucBuf + m_dwLen, sizeof(FLOAT));

	m_dwLen += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::ReadInt64(INT64 *pi64Data)
{
	memcpy(pi64Data, m_pucBuf + m_dwLen, sizeof(INT64));

	m_dwLen += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadSHORT(SHORT *psData)
{
	memcpy(psData, m_pucBuf + m_dwLen, sizeof(SHORT));

	m_dwLen += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::ReadUSHORT(USHORT *pusData)
{
	memcpy(pusData, m_pucBuf + m_dwLen, sizeof(USHORT));

	m_dwLen += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::ReadBOOL(BOOL *pbData)
{
	memcpy(pbData, m_pucBuf + m_dwLen, sizeof(BOOL));

	m_dwLen += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::ReadWCHAR(WCHAR *pwcData)
{
	memcpy(pwcData, m_pucBuf + m_dwLen, sizeof(WCHAR));

	m_dwLen += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::ReadWCHARs(LPWSTR lpwData, DWORD dwLen)
{
	memcpy(lpwData, m_pucBuf + m_dwLen, dwLen * sizeof(WCHAR));

	m_dwLen += dwLen * sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteInt32(INT iData)
{
	memcpy(m_pucBuf + m_dwLen, &iData, sizeof(INT));

	m_dwLen += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteDWORD(DWORD dwData)
{
	memcpy(m_pucBuf + m_dwLen, &dwData, sizeof(DWORD));

	m_dwLen += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::WriteDWORD_PTR(DWORD_PTR dwData)
{
	memcpy(m_pucBuf + m_dwLen, &dwData, sizeof(DWORD_PTR));

	m_dwLen += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::WriteByte(BYTE ucData)
{
	memcpy(m_pucBuf + m_dwLen, &ucData, sizeof(BYTE));

	m_dwLen += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::WriteBytes(BYTE *pucData, DWORD dwLen)
{
	memcpy(m_pucBuf + m_dwLen, pucData, dwLen);

	m_dwLen += dwLen;

	return TRUE;
}

BOOL CStream::WriteFloat(FLOAT fData)
{
	memcpy(m_pucBuf + m_dwLen, &fData, sizeof(FLOAT));

	m_dwLen += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::WriteInt64(INT64 i64Data)
{
	memcpy(m_pucBuf + m_dwLen, &i64Data, sizeof(INT64));

	m_dwLen += sizeof(INT64);

	return TRUE;
}

BOOL CStream::WriteSHORT(SHORT sData)
{
	memcpy(m_pucBuf + m_dwLen, &sData, sizeof(SHORT));

	m_dwLen += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::WriteUSHORT(USHORT usData)
{
	memcpy(m_pucBuf + m_dwLen, &usData, sizeof(USHORT));

	m_dwLen += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::WriteBOOL(BOOL bData)
{
	memcpy(m_pucBuf + m_dwLen, &bData, sizeof(BOOL));

	m_dwLen += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::WriteWCHAR(WCHAR wcData)
{
	memcpy(m_pucBuf + m_dwLen, &wcData, sizeof(WCHAR));

	m_dwLen += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteWCHARs(LPCWSTR lpcwData, DWORD dwLen)
{
	memcpy(m_pucBuf + m_dwLen, lpcwData, dwLen * sizeof(WCHAR));

	m_dwLen += dwLen * sizeof(WCHAR);

	return TRUE;
}

DWORD CStream::GetLength(VOID)
{
	return m_dwLen;
}
