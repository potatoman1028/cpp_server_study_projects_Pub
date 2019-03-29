#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CStaticSyncParent<T>
{
public:
	static VOID* operator new(size_t iAllocLen)
	{
		CStaticSyncObj Sync2;

		assert(sizeof(T) == iAllocLen);
		assert(sizeof(T) >= sizeof(UCHAR*));

		if (!m_pucFree)
			allocBlock();

		UCHAR *pReturn = m_pucFree;
		m_pucFree = *reinterpret_cast<UCHAR**>(pReturn);
		++m_iLength;

		return pReturn;
	}

	static VOID	operator delete(VOID* pDelete)
	{
		CStaticSyncObj Sync2;

		if (m_iLength == 0) throw "delete invalid pointer";
		--m_iLength;

		*reinterpret_cast<UCHAR**>(pDelete) = m_pucFree;
		m_pucFree = static_cast<UCHAR*>(pDelete);
	}

	static INT GetLength(VOID) { CStaticSyncObj Sync2; return m_iLength; }
	static INT GetCapacity(VOID) { CStaticSyncObj Sync2; return m_iCapacity;}

private:
	static VOID	allocBlock()
	{
		m_pucFree = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		UCHAR **ppCur = reinterpret_cast<UCHAR **>(m_pucFree);
		UCHAR *pNext = m_pucFree;

		for (INT i=0;i<ALLOC_BLOCK_SIZE-1;++i)
		{
			pNext += sizeof(T);
			*ppCur = pNext;
			ppCur = reinterpret_cast<UCHAR**>(pNext);
		}

		*ppCur = 0;
		m_iCapacity += ALLOC_BLOCK_SIZE;
	}

private:
	static UCHAR *m_pucFree;
	static INT m_iLength;
	static INT m_iCapacity;

protected:
	~CMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_pucFree;
template <class T, int ALLOC_BLOCK_SIZE>
INT CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_iLength;
template <class T, int ALLOC_BLOCK_SIZE>
INT CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_iCapacity;
