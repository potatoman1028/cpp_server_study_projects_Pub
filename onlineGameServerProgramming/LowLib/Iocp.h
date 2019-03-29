#pragma once

class CIocp
{
public:
	CIocp(VOID);
	virtual ~CIocp(VOID);

private:
	HANDLE				mIocpHandle;
	DWORD				mWorkerThreadCount;

	std::vector<HANDLE>	mWorkerThreadVector;

	HANDLE				mStartupEventHandle;

protected:
	virtual VOID		OnIoRead(VOID *object, DWORD dataLength)	= 0;
	virtual VOID		OnIoWrote(VOID *object, DWORD dataLength)	= 0;
	virtual VOID		OnIoConnected(VOID *object)					= 0;
	virtual VOID		OnIoDisconnected(VOID *object)				= 0;

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);
	
	BOOL				RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	VOID				WorkerThreadCallback(VOID);
};

