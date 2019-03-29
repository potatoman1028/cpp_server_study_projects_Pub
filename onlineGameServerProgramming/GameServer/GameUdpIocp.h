#pragma once

class CGameUdpIocp : public CIocp
{
public:
	CGameUdpIocp(VOID);
	virtual ~CGameUdpIocp(VOID);

private:
	CPacketSession			mUdpSession;

	HANDLE					mResetThreadHandle;
	HANDLE					mResetThreadDestroyEvent;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	VOID	ResetThreadCallback(VOID);

protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);
};
