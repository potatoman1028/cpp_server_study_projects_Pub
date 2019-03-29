#include "stdafx.h"
#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "Character.h"
#include "Room.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"

CConnectedUserManager::CConnectedUserManager(VOID)
{
	mMaxUserCount		= 0;
	mCurrentUserCount	= 0;
}

CConnectedUserManager::~CConnectedUserManager(VOID)
{
}

BOOL CConnectedUserManager::Begin(DWORD maxUserCount, SOCKET listenSocket)
{
	CThreadSync Sync;

	if (maxUserCount <= 0 || !listenSocket)
		return FALSE;

	mMaxUserCount		= maxUserCount;
	mCurrentUserCount	= 0;

	mListenSocket		= listenSocket;

	for (DWORD i=0;i<mMaxUserCount;i++)
	{
		CConnectedUser *ConnectedUser = new CConnectedUser();

		if (ConnectedUser->Begin())
			mConnectedUserVector.push_back(ConnectedUser);
		else
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::End(VOID)
{
	CThreadSync Sync;

	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		ConnectedUser->End();

		delete ConnectedUser;
	}

	mConnectedUserVector.clear();

	return TRUE;
}

BOOL CConnectedUserManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		if (!ConnectedUser->Accept(mListenSocket))
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::WriteAll(DWORD protocol, BYTE *data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!data)
		return FALSE;

	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		if (ConnectedUser->GetIsConnected())
		{
			if (!ConnectedUser->WritePacket(protocol, data, dataLength))
				ConnectedUser->End();
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::IsAlreadyLogined(LPTSTR userID)
{
	CThreadSync Sync;

	if (!userID)
		return TRUE;

	TCHAR UserID[32] = {0,};

	for (DWORD i=0;i<mConnectedUserVector.size();i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		if (!_tcscmp(ConnectedUser->GetUserID(), userID))
			return TRUE;
	}

	return FALSE;
}