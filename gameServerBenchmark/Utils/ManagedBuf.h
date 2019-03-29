#pragma once

class CManagedBuf : public CMemoryPool<CManagedBuf>
{
public:
	CManagedBuf(VOID);
	~CManagedBuf(VOID);

	BYTE m_aucBuf[MAX_BUF_LEN];
};

class CManagedBufSP
{
public:
	CManagedBufSP(VOID){m_poManagedBuf = new CManagedBuf();}
	~CManagedBufSP(VOID){delete m_poManagedBuf;}

	CManagedBuf* operator->(VOID){return m_poManagedBuf;}
	operator CManagedBuf*(VOID){return m_poManagedBuf;}

private:
	CManagedBuf	*m_poManagedBuf;
};