#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"
#include "../LowLib/IniFile.h"

#include "../Packet/ErrorCode.h"
#include "../Packet/TcpProtocol.h"
#include "../Packet/Tcp_Structure.h"
#include "../Packet/Tcp_ReadPacket.h"
#include "../Packet/Tcp_WritePacket.h"

#include "Character.h"
#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameIocp.h"

DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	CGameIocp *Owner = (CGameIocp*) parameter;
	Owner->KeepThreadCallback();

	return 0;
}

DWORD WINAPI GameThreadCallback(LPVOID parameter)
{
	CGameIocp *Owner = (CGameIocp*) parameter;
	Owner->GameThreadCallback();

	return 0;
}

CGameIocp::CGameIocp(VOID)
{
}

CGameIocp::~CGameIocp(VOID)
{
}

VOID CGameIocp::KeepThreadCallback(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0)
			return;

		mConnectedUserManager.WriteAll(0x3000000, (BYTE*) &KeepAlive, sizeof(DWORD));
	}
}

VOID CGameIocp::GameThreadCallback(VOID)
{
	// NEW_UPDATE
	//return;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mGameThreadDestroyEvent, 1000);

		if (Result == WAIT_OBJECT_0)
			return;

		mRoomManager.CheckGameTime(this);
	}
}

BOOL CGameIocp::Begin(VOID)
{
	// 데이터 파일을 읽어서 저장해 준다.
	CIniFile	IniFile;

	if (IniFile.Open(CONFIG_FILENAME))
	{
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_GAMETIME"), &DEFAULT_GAMETIME);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_MAX_HP"), &DEFAULT_MAX_HP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_MAX_AP"), &DEFAULT_MAX_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_SPECIAL_DAMAGE"), &DEFAULT_SPECIAL_DAMAGE);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_SPECIAL_AP"), &DEFAULT_SPECIAL_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_DAMAGE"), &DEFAULT_DAMAGE);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_AP"), &DEFAULT_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_CATCH_AP"), &DEFAULT_CATCH_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_SPECIAL_CATCH_AP"), &DEFAULT_SPECIAL_CATCH_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_SPEND_AP"), &DEFAULT_SPEND_AP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_EXP"), &DEFAULT_EXP);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_GAMEMONEY"), &DEFAULT_GAMEMONEY);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_EXPLOSION_TIME_FOR_PERSION"), &DEFAULT_EXPLOSION_TIME_FOR_PERSION);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_EXPLOSION_TIME_FOR_TEAM"), &DEFAULT_EXPLOSION_TIME_FOR_TEAM);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_EXPLISION_ALRAM_TIME"), &DEFAULT_EXPLISION_ALRAM_TIME);
		IniFile.GetValue(_T("SERVER"), _T("DEFAULT_TURNOVER_TIME"), &DEFAULT_TURNOVER_TIME);
	
		IniFile.GetValue(_T("SERVER"), _T("DOGS000_ADVANTAGE"), &DOGS000_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS001_ADVANTAGE"), &DOGS001_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS002_ADVANTAGE"), &DOGS002_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS003_ADVANTAGE"), &DOGS003_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS004_ADVANTAGE"), &DOGS004_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS005_ADVANTAGE"), &DOGS005_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS006_ADVANTAGE"), &DOGS006_ADVANTAGE);
		IniFile.GetValue(_T("SERVER"), _T("DOGS007_ADVANTAGE"), &DOGS007_ADVANTAGE);

		IniFile.Close();
	}
	else
	{
		CLog::WriteLogNoDate(_T("IniFile.Open"));

		End();

		return FALSE;
	}

	if (!CIocp::Begin())
	{	
		CLog::WriteLogNoDate(_T("CIocp::Begin"));

		End();

		return FALSE;
	}

	if (!mListenSession.Begin())
	{
		CLog::WriteLogNoDate(_T("ListenSession.Begin"));

		End();

		return FALSE;
	}

	if (!mListenSession.TcpBind())
	{
		CLog::WriteLogNoDate(_T("ListenSession.TcpBind"));

		End();

		return FALSE;
	}

	if (!mListenSession.Listen(DEFAULT_PORT, MAX_USER))
	{
		CLog::WriteLogNoDate(_T("ListenSession.Listen"));

		End();

		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), (ULONG_PTR) &mListenSession))
	{
		CLog::WriteLogNoDate(_T("CIocp::RegisterSocketToIocp"));

		End();

		return FALSE;
	}

	// 데이터베이스 오브젝트 시작
	//if (!mDataBase.Begin(MAX_DATABASE_CONNECTION_COUNT, DATABASE_ADDRESS, DATABASE_USERID, DATABASE_PASSWORD))
	//{
	//	CLog::WriteLogNoDate(_T("mDatabase.Begin"));

	//	End();

	//	return FALSE;
	//}

	if (!mConnectedUserManager.Begin(MAX_USER, mListenSession.GetSocket()))
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.Begin"));

		End();

		return FALSE;
	}

	if (!mRoomManager.Begin(MAX_USER))
	{
		CLog::WriteLogNoDate(_T("RoomManager.Begin"));

		End();

		return FALSE;
	}

	if (!mConnectedUserManager.AcceptAll())
	{
		CLog::WriteLogNoDate(_T("ConnectedUserManager.AllAccept"));

		End();

		return FALSE;
	}

	mKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mKeepThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mKeepThreadHandle		= CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}

	mGameThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mGameThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mGameThreadHandle		= CreateThread(NULL, 0, ::GameThreadCallback, this, 0, NULL);
	if (!mGameThreadHandle)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CGameIocp::End(VOID)
{
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent);

		WaitForSingleObject(mKeepThreadHandle, INFINITE);

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	if (mGameThreadDestroyEvent && mGameThreadHandle)
	{
		SetEvent(mGameThreadDestroyEvent);

		WaitForSingleObject(mGameThreadHandle, INFINITE);

		CloseHandle(mGameThreadDestroyEvent);
		CloseHandle(mGameThreadHandle);
	}

	CIocp::End();

	mRoomManager.End();

	mConnectedUserManager.End();

	mListenSession.End();

	// 데이터베이스는 가장 마지막에 종료
	//mDataBase.End();

	return TRUE;
}

VOID CGameIocp::OnIoConnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	TCHAR	RemoteAddress[32]	= {0,};
	USHORT	RemotePort			= 0;

	ConnectedUser->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);
	ConnectedUser->SetRealInformation(RemoteAddress, RemotePort);

	if (!CIocp::RegisterSocketToIocp(ConnectedUser->GetSocket(), (ULONG_PTR) ConnectedUser))
	{
		CLog::WriteLog(_T("! OnIoConnected : CIocp::RegisterSocketToIocp"));

		End();

		return;
	}

	ConnectedUser->SetIsConnected(TRUE);

	if (!ConnectedUser->InitializeReadForIocp())
	{
		if (!ConnectedUser->Reload(mListenSession.GetSocket()))
		{
			CLog::WriteLog(_T("! OnIoConnected : ConnectedUser->Reload"));

			End();

			return;
		}
	}

	mConnectedUserManager.IncreaseConnectedUserCount();

	CLog::WriteLog(_T("# New client connected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID CGameIocp::OnIoDisconnected(VOID *object)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	// 나가는 사용자가 방 안에 있을 경우
	if (ConnectedUser->GetStatus() >= US_ROOM_ENTERING)
	{
		// 다른사람들에게 알려주고 빼준다.
		CRoom *Room = ConnectedUser->GetEnteredRoom();

		if (Room)
			Room->LeaveUser(TRUE, this, ConnectedUser);
		//
	}

	if (!ConnectedUser->Reload(mListenSession.GetSocket()))
	{
		CLog::WriteLog(_T("! OnIoDisconnected : ConnectedUser->Reload"));

		End();

		return;
	}

	mConnectedUserManager.DecreaseConnectedUserCount();

	CLog::WriteLog(_T("# Client disconnected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID CGameIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	DWORD	Protocol						= 0;
	BYTE	Packet[MAX_BUFFER_LENGTH]		= {0,};
	DWORD	PacketLength					= 0;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (ConnectedUser->ReadPacketForIocp(dataLength))
	{
		while (ConnectedUser->GetPacket(Protocol, Packet, PacketLength))
		{
			//////////////////////////////////////////////////////////////////////////
			// 로비관련 패킷
			if (Protocol == PT_CHANNEL_ENTER)						// PROTOCOL : PT_CHANNEL_ENTER
				onPT_CHANNEL_ENTER(ConnectedUser, Packet);
			else if (Protocol == PT_ROOM_QUICK_JOIN)				// PROTOCOL : PT_ROOM_QUICK_JOIN
				onPT_ROOM_QUICK_JOIN(ConnectedUser, Packet);
			else if (Protocol == PT_ROOM_REQ_INFO)					// PROTOCOL : PT_ROOM_REQ_INFO
				onPT_ROOM_REQ_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_ROOM_LEAVE)						// PROTOCOL : PT_ROOM_LEAVE
				onPT_ROOM_LEAVE(ConnectedUser, Packet);
			else if (Protocol == PT_ROOM_MAP_CHANGE)				// PROTOCOL : PT_ROOM_MAP_CHANGE
				onPT_ROOM_MAP_CHANGE(ConnectedUser, Packet);
			else if (Protocol ==  PT_ROOM_CHATTING)					// PROTOCOL : PT_ROOM_CHATTING
				onPT_ROOM_CHATTING(ConnectedUser, Packet);		
			else if (Protocol == PT_ROOM_READY)						// PROTOCOL : PT_ROOM_READY
				onPT_ROOM_READY(ConnectedUser, Packet);
			else if (Protocol == PT_ROOM_START)						// PROTOCOL : PT_ROOM_START
				onPT_ROOM_START(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_LOAD_COMPLETE)				// PROTOCOL : PT_GAME_LOAD_COMPLETE
				onPT_GAME_LOAD_COMPLETE(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_INTRO_COMPLETE)			// PROTOCOL : PT_GAME_INTRO_COMPLETE
				onPT_GAME_INTRO_COMPLETE(ConnectedUser, Packet);
//#ifdef _INCLUDE_DATABASE
//			else if (Protocol == PT_CHANNEL_NICKNAME)				// PROTOCOL : PT_CHANNEL_NICKNAME
//				onPT_CHANNEL_NICKNAME(ConnectedUser, Packet);
//			else if (Protocol == PT_CHANNEL_REQ_USERINFO)			// PROTOCOL : PT_CHANNEL_REQ_USERINFO
//				onPT_CHANNEL_REQ_USERINFO(ConnectedUser, Packet);
//			else if (Protocol == PT_CHANNEL_REQ_CHARINFO)			// PROTOCOL : PT_CHANNEL_REQ_CHARINFO
//				onPT_CHANNEL_REQ_CHARINFO(ConnectedUser, Packet);	
//			else if (Protocol == PT_CHANNEL_CHECK_CHARACTER_NAME)	// PROTOCOL : PT_CHANNEL_CHECK_CHARACTER_NAME
//				onPT_CHANNEL_CHECK_CHARACTER_NAME(ConnectedUser, Packet);
//			else if (Protocol == PT_CHANNEL_CREATE_CHARACTER)		// PROTOCOL : PT_CHANNEL_CREATE_CHARACTER
//				onPT_CHANNEL_CREATE_CHARACTER(ConnectedUser, Packet);
//			else if (Protocol == PT_CHANNEL_EQUIP)					// PROTOCOL : PT_CHANNEL_EQUIP
//				onPT_CHANNEL_EQUIP(ConnectedUser, Packet);
//			else if (Protocol == PT_CHANNEL_CHARACTER_SELECT)		// PROTOCOL : PT_CHANNEL_CHARACTER_SELECT
//				onPT_CHANNEL_CHARACTER_SELECT(ConnectedUser, Packet);
//#endif
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			// 게임관련 패킷
			else if (Protocol == PT_GAME_PASS_INFO)					// PROTOCOL : PT_GAME_PASS_INFO
				onPT_GAME_PASS_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SHOOT_INFO)				// PROTOCOL : PT_GAME_SHOOT_INFO
				onPT_GAME_SHOOT_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_HIT_INFO)					// PROTOCOL : PT_GAME_HIT_INFO
				onPT_GAME_HIT_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SHOOT_RECEIVE_INFO)		// PROTOCOL : PT_GAME_SHOOT_RECEIVE_INFO
				onPT_GAME_SHOOT_RECEIVE_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_PASS_RECEIVE_INFO)			// PROTOCOL : PT_GAME_PASS_RECEIVE_INFO
				onPT_GAME_PASS_RECEIVE_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_FIELDBALL)					// PROTOCOL : PT_GAME_DROP_BALL
				onPT_GAME_FIELDBALL(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_GET_FIELDBALL)				// PROTOCOL : PT_GAME_GET_FIELDBALL
				onPT_GAME_GET_FIELDBALL(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_CHANGE_BALLOWNER)			// PROTOCOL : PT_GAME_CHANGE_BALLOWNER
				onPT_GAME_CHANGE_BALLOWNER(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SPECIAL_SHOOT_INFO)		// PROTOCOL : PT_GAME_SPECIAL_SHOOT_INFO
				onPT_GAME_SPECIAL_SHOOT_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SPECIAL_HIT_INFO)			// PROTOCOL : PT_GAME_SPECIAL_HIT_INFO
				onPT_GAME_SPECIAL_HIT_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SPECIAL_RECEIVE_INFO)		// PROTOCOL : PT_GAME_SPECIAL_RECEIVE_INFO
				onPT_GAME_SPECIAL_RECEIVE_INFO(ConnectedUser, Packet);
			else if (Protocol == PT_GAME_SKILL_INFO)				// PROTOCOL : PT_GAME_SKILL_INFO
				onPT_GAME_SKILL_INFO(ConnectedUser, Packet);
			//////////////////////////////////////////////////////////////////////////
		}
	}

	if (!ConnectedUser->InitializeReadForIocp())
		ConnectedUser->End();
	// TODO : 강제 종료부
	// End는 꼭 이후에
	// ConnectedUser->End();
}

VOID CGameIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = (CConnectedUser*) object;

	ConnectedUser->WriteComplete();

#ifdef _FULL_DEBUG
	CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}