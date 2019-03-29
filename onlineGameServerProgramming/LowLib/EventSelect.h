#pragma once

class CEventSelect
{
public:
	CEventSelect(VOID);
	virtual ~CEventSelect(VOID);

private:
	HANDLE			mSelectEventHandle;
	HANDLE			mStartupEventHandle;
	HANDLE			mDestroyEventHandle;

	HANDLE			mSelectThreadHandle;

	SOCKET			mSocket;

protected:
	virtual VOID	OnIoRead(VOID)			= 0;
	//virtual VOID	OnIoWrote(VOID)			= 0;
	virtual VOID	OnIoConnected(VOID)		= 0;
	virtual VOID	OnIoDisconnected(VOID)	= 0;

public:
	BOOL			Begin(SOCKET socket);
	BOOL			End(VOID);

	VOID			SelectThreadCallback(VOID);
};
