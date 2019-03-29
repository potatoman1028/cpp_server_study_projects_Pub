#pragma once

#define MAX_QUEUE_LENGTH	1000

template <class T>
class CCircularQueue : public CSyncParent<CCircularQueue<T>>
{
public:
	CCircularQueue(VOID)
	{
		ZeroMemory(m_atDatas, sizeof(m_atDatas));
		m_dwHead = m_dwTail = 0;
	}
	~CCircularQueue(VOID){}

private:
	T		m_atDatas[MAX_QUEUE_LENGTH];
	DWORD	m_dwHead;
	DWORD	m_dwTail;

public:
	BOOL Push(T tInputData)
	{
		CSyncObj Sync;

		DWORD dwTempTail = (m_dwTail + 1) % MAX_QUEUE_LENGTH;
		
		if (dwTempTail == m_dwHead)
			return FALSE;

		CopyMemory(&m_atDatas[dwTempTail], &tInputData, sizeof(T));
		m_dwTail = dwTempTail;
		
		return TRUE;
	}

	BOOL Pop(T& rtOutputData)
	{
		CSyncObj Sync;

		if (m_dwHead == m_dwTail)
			return FALSE;

		DWORD dwTempHead = (m_dwHead + 1) % MAX_QUEUE_LENGTH;
		CopyMemory(&rtOutputData, &m_atDatas[dwTempHead], sizeof(T));
		m_dwHead = dwTempHead;

		return TRUE;
	}

	BOOL GetIsEmpty(VOID)
	{
		CSyncObj Sync;

		if (m_dwHead == m_dwTail)
			return TRUE;

		return FALSE;
	}
};
