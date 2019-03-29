#include "stdafx.h"
#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "Character.h"
#include "Room.h"
#include "ConnectedUser.h"

CConnectedUser::CConnectedUser(VOID)
{
	ZeroMemory(mUserID, sizeof(mUserID));
	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));
	ZeroMemory(mNickName, sizeof(mNickName));
	ZeroMemory(mCharacters, sizeof(mCharacters));

	mVirtualPort			= 0;
	mRealPort				= 0;

	mStatus					= US_NONE;

	mEnteredRoom			= NULL;

	mIsConnected			= FALSE;
	mIsReady				= FALSE;

	mIsLoadComplete			= FALSE;
	mIsIntroComplete		= FALSE;

	mHP						= 0;
	mAP						= 0;
	mIsInCourt				= FALSE;

	mGameMoney				= 0;
	mExp					= 0;

	mSelectedCharacterSlot	= 0;

	mCharacters[0].Initialize();
	mCharacters[1].Initialize();
	mCharacters[2].Initialize();
}

CConnectedUser::~CConnectedUser(VOID)
{
}

BOOL CConnectedUser::Begin(VOID)
{
	CThreadSync Sync;

	ZeroMemory(mUserID, sizeof(mUserID));
	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));
	ZeroMemory(mNickName, sizeof(mNickName));
	ZeroMemory(mCharacters, sizeof(mCharacters));

	mVirtualPort			= 0;
	mRealPort				= 0;

	mStatus					= US_NONE;

	mEnteredRoom			= NULL;

	mIsConnected			= FALSE;
	mIsReady				= FALSE;

	mIsLoadComplete			= FALSE;
	mIsIntroComplete		= FALSE;

	mHP						= 0;
	mAP						= 0;
	mIsInCourt				= FALSE;

	mGameMoney				= 0;
	mExp					= 0;

	mSelectedCharacterSlot	= 0;

	mCharacters[0].Initialize();
	mCharacters[1].Initialize();
	mCharacters[2].Initialize();

	return CPacketSession::Begin();
}

BOOL CConnectedUser::End(VOID)
{
	CThreadSync Sync;

	ZeroMemory(mUserID, sizeof(mUserID));
	ZeroMemory(mVirtualAddress, sizeof(mVirtualAddress));
	ZeroMemory(mRealAddress, sizeof(mRealAddress));
	ZeroMemory(mNickName, sizeof(mNickName));
	ZeroMemory(mCharacters, sizeof(mCharacters));

	mVirtualPort			= 0;
	mRealPort				= 0;

	mStatus					= US_NONE;

	mEnteredRoom			= NULL;

	mIsConnected			= FALSE;
	mIsReady				= FALSE;

	mIsLoadComplete			= FALSE;
	mIsIntroComplete		= FALSE;

	mHP						= 0;
	mAP						= 0;
	mIsInCourt				= FALSE;

	mGameMoney				= 0;
	mExp					= 0;

	mSelectedCharacterSlot	= 0;

	mCharacters[0].Initialize();
	mCharacters[1].Initialize();
	mCharacters[2].Initialize();

	return CPacketSession::End();
}

BOOL CConnectedUser::Reload(SOCKET listenSocket)
{
	CThreadSync Sync;

	End();

	if (!Begin())
		return FALSE;

	if (!CNetworkSession::Accept(listenSocket))
		return FALSE;

	return TRUE;
}