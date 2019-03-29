#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"

CCircularQueue::CCircularQueue(VOID)
{
	memset(mQueue, 0, sizeof(mQueue));

	mQueueHead	= 0;
	mQueueTail	= 0;
}

CCircularQueue::~CCircularQueue(VOID)
{
}

BOOL CCircularQueue::Begin(VOID)
{
	memset(mQueue, 0, sizeof(mQueue));

	mQueueHead	= 0;
	mQueueTail	= 0;

	return TRUE;
}

BOOL CCircularQueue::End(VOID)
{
	return TRUE;
}

BYTE* CCircularQueue::Push(VOID *object, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object		= object;
	mQueue[TempTail].DataLength	= dataLength;

	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE* CCircularQueue::Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object		= object;
	mQueue[TempTail].DataLength	= dataLength;
	mQueue[TempTail].Protocol	= protocol;

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE *CCircularQueue::Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object		= object;
	mQueue[TempTail].DataLength	= dataLength;
	mQueue[TempTail].RemotePort	= remotePort;
	
	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));
	
	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE *CCircularQueue::Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object		= object;
	mQueue[TempTail].DataLength	= dataLength;
	mQueue[TempTail].RemotePort	= remotePort;
	mQueue[TempTail].Protocol	= protocol;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));
	
	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));	
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BOOL CCircularQueue::Pop(VOID)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID **object, BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength	= mQueue[TempHead].DataLength;
	*object		= mQueue[TempHead].Object;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength	= mQueue[TempHead].DataLength;
	*object		= mQueue[TempHead].Object;
	protocol	= mQueue[TempHead].Protocol;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength	= mQueue[TempHead].DataLength;
	*object		= mQueue[TempHead].Object;
	remotePort	= mQueue[TempHead].RemotePort;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength	= mQueue[TempHead].DataLength;
	*object		= mQueue[TempHead].Object;
	remotePort	= mQueue[TempHead].RemotePort;
	protocol	= mQueue[TempHead].Protocol;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::IsEmpty(VOID)
{
	CThreadSync Sync;

	if (mQueueHead == mQueueTail)
		return TRUE;

	return FALSE;
}
