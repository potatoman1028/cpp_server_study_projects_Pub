#pragma once

class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	virtual ~CConnectedUserManager(VOID);

private:
	std::vector<CConnectedUser*>	mConnectedUserVector;
	DWORD							mMaxUserCount;

	DWORD							mCurrentUserCount;
	
	SOCKET							mListenSocket;

public:
	BOOL	Begin(DWORD maxUserCount, SOCKET listenSocket);
	BOOL	End(VOID);

	BOOL	AcceptAll(VOID);
	BOOL	WriteAll(DWORD protocol, BYTE *data, DWORD dataLength);

	BOOL	IsAlreadyLogined(LPTSTR userID);

	inline VOID	IncreaseConnectedUserCount(VOID){InterlockedIncrement((LONG*) &mCurrentUserCount);}
	inline VOID	DecreaseConnectedUserCount(VOID){InterlockedDecrement((LONG*) &mCurrentUserCount);}
};
