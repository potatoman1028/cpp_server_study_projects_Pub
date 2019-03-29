#pragma once

class CStream : public CMemoryPool<CStream>
{
public:
	CStream(VOID);
	~CStream(VOID);

private:
	BYTE	*m_pucBuf;
	DWORD	m_dwLen;

public:
	BOOL	SetBuffer(BYTE *pucBuf);
	BOOL	SetBuffer();

	BOOL	ReadInt32(INT *piData);
	BOOL	ReadDWORD(DWORD *pdwData);
	BOOL	ReadDWORD_PTR(DWORD_PTR *pdwData);
	BOOL	ReadByte(BYTE *pucData);
	BOOL	ReadBytes(BYTE *pucData, DWORD dwLen);
	BOOL	ReadFloat(FLOAT *pfData);
	BOOL	ReadInt64(INT64 *pi64Data);
	BOOL	ReadUSHORT(USHORT *pusData);
	BOOL	ReadSHORT(SHORT *psData);
	BOOL	ReadBOOL(BOOL *pbData);
	BOOL	ReadWCHAR(WCHAR *pwcData);
	BOOL	ReadWCHARs(LPWSTR lpwData, DWORD dwLen);

	BOOL	WriteInt32(INT iData);
	BOOL	WriteDWORD(DWORD dwData);
	BOOL	WriteDWORD_PTR(DWORD_PTR dwData);
	BOOL	WriteByte(BYTE ucData);
	BOOL	WriteBytes(BYTE *pucData, DWORD dwLen);
	BOOL	WriteFloat(FLOAT fData);
	BOOL	WriteInt64(INT64 i64Data);
	BOOL	WriteUSHORT(USHORT usData);
	BOOL	WriteSHORT(SHORT sData);
	BOOL	WriteBOOL(BOOL bData);
	BOOL	WriteWCHAR(WCHAR wcData);
	BOOL	WriteWCHARs(LPCWSTR lpcwData, DWORD dwLen);

	DWORD	GetLength(VOID);
};

class CStreamSP
{
public:
	CStreamSP(VOID){m_pStream = new CStream();}
	~CStreamSP(VOID){delete m_pStream;}

	CStream* operator->(VOID){return m_pStream;}
	operator CStream*(VOID){return m_pStream;}

private:
	CStream	*m_pStream;
};
