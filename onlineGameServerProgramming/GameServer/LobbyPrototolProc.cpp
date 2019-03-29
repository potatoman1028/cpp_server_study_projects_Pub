#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

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

VOID CGameIocp::onPT_CHANNEL_ENTER(CConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_NONE, PT_CHANNEL_ENTER_FAIL_U);

	connectedUser->SetStatus(US_CHANNEL_ENTERING);

	READ_PACKET(PT_CHANNEL_ENTER);
	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_ENTER %s/%s/%d\n"), Data.USER_ID,
		Data.VIRTUAL_ADDRESS, Data.VIRTUAL_PORT);

//#ifdef _INCLUDE_DATABASE
//	// DB에 아이디가 있는지 확인한다. 없으면 실패하고 닉네임 셋팅으로
//	if (!db_IsExistUserID(Data.USER_ID))
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_ENTER_FAIL_U, EC_NO_NICKNAME);
//
//		connectedUser->SetStatus(US_NONE);
//
//		return;
//	}
//#endif

	Data.USER_ID[sizeof(Data.USER_ID)-1]					= NULL;
	Data.VIRTUAL_ADDRESS[sizeof(Data.VIRTUAL_ADDRESS)-1]	= NULL;

	if (!mConnectedUserManager.IsAlreadyLogined(Data.USER_ID))
	{
		connectedUser->SetUserID(Data.USER_ID);
		connectedUser->SetVirtualInformation(Data.VIRTUAL_ADDRESS, Data.VIRTUAL_PORT);

		DWORD_PTR	SessionID	= (DWORD_PTR)(connectedUser);

		connectedUser->WritePacket(PT_CHANNEL_ENTER_SUCC_U,
			WriteBuffer,
			WRITE_PT_CHANNEL_ENTER_SUCC_U(WriteBuffer, 
			SessionID,
			connectedUser->GetVirtualAddress(),
			connectedUser->GetVirtualPort(),
			connectedUser->GetRealAddress(),
			connectedUser->GetRealPort()
			));

		connectedUser->SetStatus(US_CHANNEL_ENTERED);

		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_ENTER_SUCC_U 0x%x/%s/%d/%s/%d\n"), SessionID,
			connectedUser->GetVirtualAddress(),
			connectedUser->GetVirtualPort(),
			connectedUser->GetRealAddress(),
			connectedUser->GetRealPort());
	}
	else
	{
		WRITE_FAIL_U_PACKET(PT_CHANNEL_ENTER_FAIL_U, EC_USER_ALREADY_LOGINED);

		connectedUser->SetStatus(US_NONE);
	}
}

VOID CGameIocp::onPT_ROOM_QUICK_JOIN(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_ROOM_QUICK_JOIN_FAIL_U);

	connectedUser->SetStatus(US_ROOM_ENTERING);

	READ_PACKET(PT_ROOM_QUICK_JOIN);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_QUICK_JOIN\n"));

	USHORT SlotIndex = 0;
	CRoom *Room = mRoomManager.QuickJoin(connectedUser, SlotIndex);
	if (Room)
	{
		// 입장 성공 패킷을 보낸후에
		connectedUser->WritePacket(PT_ROOM_QUICK_JOIN_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_ROOM_QUICK_JOIN_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_ROOM_QUICK_JOIN_SUCC_U\n"));

		// 방안에 있는 사람들에게 들어온 사람의 정보를 보내준다.
		Room->WriteAll(PT_ROOM_ENTER_M, 
			WriteBuffer, 
			WRITE_PT_ROOM_ENTER_M(WriteBuffer, 
			SlotIndex, 
			connectedUser->GetUserID(), 
			(DWORD_PTR) connectedUser, 
			connectedUser->GetVirtualAddress(), 
			connectedUser->GetVirtualPort(), 
			connectedUser->GetRealAddress(), 
			connectedUser->GetRealPort()));

		CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_ENTER_M %d/%s/0x%x/%s/%d/%s/%d\n"),
			SlotIndex,
			connectedUser->GetUserID(),
			(DWORD_PTR) connectedUser,
			connectedUser->GetVirtualAddress(),
			connectedUser->GetVirtualPort(),
			connectedUser->GetRealAddress(),
			connectedUser->GetRealPort());

		connectedUser->SetStatus(US_ROOM_ENTERED);
	}
	else
	{
		WRITE_FAIL_U_PACKET(PT_ROOM_QUICK_JOIN_FAIL_U, EC_CANNOT_CREATE_MORE_ROOM);
		
		connectedUser->SetStatus(US_CHANNEL_ENTERED);
	}
}

VOID CGameIocp::onPT_ROOM_REQ_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_ROOM_ENTERED, PT_ROOM_REQ_INFO_FAIL_U)

		READ_PACKET(PT_ROOM_REQ_INFO);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_REQ_INFO\n"));

	CRoom *Room = connectedUser->GetEnteredRoom();
	if (Room)
	{
		CConnectedUser **RoomUser = Room->GetRoomUser();

		SLOT_USER_DATA SlotUserData[8] = {0,};

		for (USHORT i=0;i<8;i++)
		{
			if (RoomUser[i])
			{
				SlotUserData[i].SessionID	= (DWORD_PTR) RoomUser[i];
				SlotUserData[i].VirtualPort	= RoomUser[i]->GetVirtualPort();
				SlotUserData[i].RealPort	= RoomUser[i]->GetRealPort();
				SlotUserData[i].IsReady		= RoomUser[i]->GetIsReady();

				_tcsncpy(SlotUserData[i].UserID, RoomUser[i]->GetUserID(), 32);
				_tcsncpy(SlotUserData[i].VirtualAddress, RoomUser[i]->GetVirtualAddress(), 32);
				_tcsncpy(SlotUserData[i].RealAddress, RoomUser[i]->GetRealAddress(), 32);
			}
		}

		connectedUser->WritePacket(PT_ROOM_REQ_INFO_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_ROOM_REQ_INFO_SUCC_U(WriteBuffer, 
			Room->GetIndex(), 
			Room->GetTitle(), 
			Room->GetMapIndex(), 
			Room->GetCurrentUserCount(),
			(DWORD_PTR) Room->GetRootUser(),
			SlotUserData));

		CLog::WriteLog(_T("# Write packet : PT_ROOM_REQ_INFO_SUCC_U %d/%s/%d\n"), Room->GetIndex(), Room->GetTitle(), Room->GetMapIndex());

		for (USHORT i=0;i<8;i++)
		{
			CLog::WriteLogNoDate(_T("%s/0x%x/%s/%d/%s/%d/%d\n"), 
				SlotUserData[i].UserID, 
				SlotUserData[i].SessionID, 
				SlotUserData[i].VirtualAddress,
				SlotUserData[i].VirtualPort,
				SlotUserData[i].RealAddress,
				SlotUserData[i].RealPort,
				SlotUserData[i].IsReady);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_REQ_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_ROOM_LEAVE(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_ROOM_ENTERED, PT_ROOM_LEAVE_FAIL_U);

	connectedUser->SetStatus(US_ROOM_LEAVING);

	READ_PACKET(PT_ROOM_LEAVE);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_LEAVE\n"));

	CRoom *Room = connectedUser->GetEnteredRoom();
	if (Room)
	{
		Room->LeaveUser(FALSE, this, connectedUser);

		// 남은 사람에게 나가는것을 알려준다.
		Room->WriteAll(PT_ROOM_LEAVE_M,
			WriteBuffer,
			WRITE_PT_ROOM_LEAVE_M(WriteBuffer, (DWORD_PTR) connectedUser, (DWORD_PTR) Room->GetRootUser()));

		CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_LEAVE_M 0x%x\n"), (DWORD_PTR) connectedUser);

		connectedUser->SetStatus(US_CHANNEL_ENTERED);
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_LEAVE_FAIL_U);

		connectedUser->SetStatus(US_ROOM_ENTERED);
	}
}

VOID CGameIocp::onPT_ROOM_MAP_CHANGE(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_ROOM_ENTERED, PT_ROOM_MAP_CHANGE_FAIL_U);

	READ_PACKET(PT_ROOM_MAP_CHANGE);
	CLog::WriteLog(_T("# Read Packet : PT_ROOM_MAP_CHANGE %d\n"), Data.MAP_INDEX);

	CRoom *Room = connectedUser->GetEnteredRoom();
	if (Room)
	{
		if (Room->GetRootUser() == connectedUser)
		{
			Room->SetMapIndex(Data.MAP_INDEX);
			// 성공을 보내주고
			connectedUser->WritePacket(PT_ROOM_MAP_CHANGE_SUCC_U, 
				WriteBuffer,
				WRITE_PT_ROOM_MAP_CHANGE_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_ROOM_MAP_CHANGE_SUCC_U\n"));

			// 맵 변경 내용을 뿌려준다.
			Room->WriteAll(PT_ROOM_MAP_CHANGE_M,
				WriteBuffer,
				WRITE_PT_ROOM_MAP_CHANGE_M(WriteBuffer, 
				Data.MAP_INDEX));

			CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_MAP_CHANGE_M %d\n"), Data.MAP_INDEX);
		}
		else
		{
			WRITE_FAIL_U_PACKET(PT_ROOM_MAP_CHANGE_FAIL_U, EC_USER_PERMISSION_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_MAP_CHANGE_FAIL_U);
	}
}

VOID CGameIocp::onPT_ROOM_CHATTING(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(< US_ROOM_ENTERED, PT_ROOM_CHATTING_FAIL_U);

	READ_PACKET(PT_ROOM_CHATTING);
	CLog::WriteLog(_T("# Read Packet : PT_ROOM_CHATTING %s\n"), Data.CHAT);

	CRoom *Room = connectedUser->GetEnteredRoom();
	if (Room)
	{
		// 성공을 보내주고
		connectedUser->WritePacket(PT_ROOM_CHATTING_SUCC_U, 
			WriteBuffer,
			WRITE_PT_ROOM_CHATTING_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_ROOM_CHATTING_SUCC_U\n"));

		// 채팅 내용을 뿌려준다.
		Room->WriteAll(PT_ROOM_CHATTING_M,
			WriteBuffer,
			WRITE_PT_ROOM_CHATTING_M(WriteBuffer, 
			(DWORD_PTR) connectedUser, 
			Data.CHAT));

		CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_CHATTING_M %s\n"), Data.CHAT);
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_CHATTING_FAIL_U);
	}
}

VOID CGameIocp::onPT_ROOM_READY(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_ROOM_ENTERED, PT_ROOM_READY_FAIL_U);

	READ_PACKET(PT_ROOM_READY);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_READY %d\n"), Data.READY);

	CRoom *Room = connectedUser->GetEnteredRoom();
	if (Room)
	{
		connectedUser->SetIsReady(Data.READY);

		connectedUser->WritePacket(PT_ROOM_READY_SUCC_U,
			WriteBuffer,
			WRITE_PT_ROOM_READY_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_ROOM_READY_SUCC_U\n"));

		Room->WriteAll(PT_ROOM_READY_M, 
			WriteBuffer,
			WRITE_PT_ROOM_READY_M(WriteBuffer, 
			(DWORD_PTR) connectedUser,
			connectedUser->GetIsReady()));

		CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_READY_M 0x%x/%d\n"), 
			(DWORD_PTR) connectedUser, 
			connectedUser->GetIsReady());
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_READY_FAIL_U);
	}
}

VOID CGameIocp::onPT_ROOM_START(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_ROOM_ENTERED, PT_ROOM_START_FAIL_U);

	READ_PACKET(PT_ROOM_START);
	CLog::WriteLog(_T("# Read packet : PT_ROOM_START\n"));

	CRoom *Room = connectedUser->GetEnteredRoom();

	// 룸오브젝트가 존재하는가
	if (Room)
	{
		// 보낸 사용자가 루트유저인가
		if (Room->GetRootUser() == connectedUser)
		{
			// 방 시작
			DWORD Result = Room->RoomStart();

			if (Result == 0)
			{
				// 성공
				// 시작 성공을 보내고
				connectedUser->WritePacket(PT_ROOM_START_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_ROOM_START_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_ROOM_START_SUCC_U\n"));

				// 모두에게 알려준다.
				Room->WriteAll(PT_ROOM_START_M, 
					WriteBuffer, 
					WRITE_PT_ROOM_START_M(WriteBuffer));

				CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_START_M\n"));
			}
			else
			{
				WRITE_FAIL_U_PACKET(PT_ROOM_START_FAIL_U, Result);
			}
		}
		else
		{
			WRITE_FAIL_U_PACKET(PT_ROOM_START_FAIL_U, EC_USER_PERMISSION_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_ROOM_START_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_LOAD_COMPLETE(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTING, PT_GAME_LOAD_COMPLETE_FAIL_U);

	READ_PACKET(PT_GAME_LOAD_COMPLETE);
	CLog::WriteLog(_T("# Read packet : PT_GAME_LOAD_COMPLETE\n"));

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸상태가 맞는지 확인하고
	if (Room)
	{
		// 우선 LoadComplete 성공
		connectedUser->SetIsLoadComplete(TRUE);

		connectedUser->WritePacket(PT_GAME_LOAD_COMPLETE_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_LOAD_COMPLETE_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_LOAD_COMPLETE_SUCC_U\n"));

		// 모든 사용자에게 누가 완료되었는지 알려준다.
		Room->WriteAll(PT_GAME_LOAD_COMPLETE_M, 
			WriteBuffer, 
			WRITE_PT_GAME_LOAD_COMPLETE_M(WriteBuffer, 
			(DWORD_PTR) connectedUser));

		CLog::WriteLog(_T("# WriteAll packet : PT_GAME_LOAD_COMPLETE_M 0x%x\n"), (DWORD_PTR) connectedUser);

		if (Room->IsAllLoadComplete())
		{
			Room->WriteAll(PT_GAME_ALL_LOAD_COMPLETE_M,
				WriteBuffer,
				WRITE_PT_GAME_ALL_LOAD_COMPLETE_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_ALL_LOAD_COMPLETE_M\n"));
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_LOAD_COMPLETE_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_INTRO_COMPLETE(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTING, PT_GAME_INTRO_COMPLETE_FAIL_U);

	READ_PACKET(PT_GAME_INTRO_COMPLETE);
	CLog::WriteLog(_T("# Read packet : PT_GAME_INTRO_COMPLETE\n"));

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸상태가 맞는지 확인하고
	if (Room)
	{
		// 우선 IntroComplete 성공
		connectedUser->SetIsIntroComplete(TRUE);

		connectedUser->WritePacket(PT_GAME_INTRO_COMPLETE_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_INTRO_COMPLETE_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_INTRO_COMPLETE_SUCC_U\n"));

		// 모든 사용자에게 누가 완료되었는지 알려준다.
		Room->WriteAll(PT_GAME_INTRO_COMPLETE_M, 
			WriteBuffer, 
			WRITE_PT_GAME_INTRO_COMPLETE_M(WriteBuffer, 
			(DWORD_PTR) connectedUser));

		CLog::WriteLog(_T("# WriteAll packet : PT_GAME_INTRO_COMPLETE_M 0x%x\n"), (DWORD_PTR) connectedUser);

		if (Room->IsAllIntroComplete())
		{
			Room->WriteAll(PT_GAME_ALL_INTRO_COMPLETE_M,
				WriteBuffer,
				WRITE_PT_GAME_ALL_INTRO_COMPLETE_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_ALL_INTRO_COMPLETE_M\n"));

			// GameStart를 호출해서 모두 완료인지를 확인한다.
			if (Room->GameStart())
			{
				// 모두 완료이면 GameStartM을 전송 게임 시작이다.
				Room->WriteAll(PT_GAME_START_M, 
					WriteBuffer, 
					WRITE_PT_GAME_START_M(WriteBuffer));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_START_M\n"));

				// 여기에서 모든 사용자들의 HP, AP를 전송해 준다. (모두에게)
				CLog::WriteLog(_T("# WriteAll packet : InitHPAP\n"));
				Room->WriteAllInitHPAP();

				// NEW_UPDATE
				//Room->GameEnd(this);
			}
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_INTRO_COMPLETE_FAIL_U);
	}
}

//#ifdef _INCLUDE_DATABASE
//VOID CGameIocp::onPT_CHANNEL_NICKNAME(CConnectedUser *connectedUser, BYTE *packet)
//{
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
//
//	CHECK_USER_STATUS(!= US_NONE, PT_CHANNEL_NICKNAME_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_NICKNAME);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_NICKNAME %s/%s\n"), 
//		Data.USER_ID, 
//		Data.NICK_NAME);
//
//	if (db_IsExistNickName(Data.NICK_NAME))
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_NICKNAME_FAIL_U, EC_ALREADY_EXIST_NICKNAME);
//
//		return;
//	}
//
//	if (db_SetUserID(Data.USER_ID, Data.NICK_NAME))
//	{
//		// 성공
//		connectedUser->WritePacket(PT_CHANNEL_NICKNAME_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_NICKNAME_SUCC_U(WriteBuffer, 
//			Data.USER_ID, 
//			Data.NICK_NAME));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_NICKNAME_SUCC_U %s/%s\n"), 
//			Data.USER_ID, 
//			Data.NICK_NAME);
//	}
//	else
//	{
//		// 실패
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_NICKNAME_FAIL_U, EC_DATABASE_ERROR);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_REQ_USERINFO(CConnectedUser *connectedUser, BYTE *packet)
//{
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_REQ_USERINFO_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_REQ_USERINFO);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_REQ_USERINFO %s\n"), Data.USER_ID);
//
//	TCHAR	NickName[32]		= {0,};
//	INT64	GameMoney			= 0;
//	INT64	Exp					= 0;
//	INT64	CharacterIndex1		= 0;
//	INT64	CharacterIndex2		= 0;
//	INT64	CharacterIndex3		= 0;
//
//	if (db_GetUserInfo(Data.USER_ID, NickName, GameMoney, Exp, CharacterIndex1, CharacterIndex2, CharacterIndex3))
//	{
//		// 성공
//		connectedUser->SetNickName(NickName);
//		connectedUser->SetGameMoney(GameMoney);
//		connectedUser->SetExp(Exp);
//
//		CCharacter *Character = NULL;
//
//		Character = connectedUser->GetCharacterObject(0);
//        Character->SetIndex(CharacterIndex1);
//
//		Character = connectedUser->GetCharacterObject(1);
//		Character->SetIndex(CharacterIndex2);
//		
//		Character = connectedUser->GetCharacterObject(2);
//		Character->SetIndex(CharacterIndex3);
//
//		// DB값 셋팅 완료하면 클라이언트로 보내준다
//		connectedUser->WritePacket(PT_CHANNEL_REQ_USERINFO_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_REQ_USERINFO_SUCC_U(WriteBuffer, 
//			NickName, 
//			GameMoney, 
//			Exp, 
//			CharacterIndex1, 
//			CharacterIndex2, 
//			CharacterIndex3));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_REQ_USERINFO_SUCC_U %s/%I64d/%I64d/%I64d/%I64d/%I64d\n"),
//			NickName,
//			GameMoney,
//			Exp,
//			CharacterIndex1,
//			CharacterIndex2,
//			CharacterIndex3);
//	}
//	else
//	{
//		// 실패
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_REQ_USERINFO_FAIL_U, EC_DATABASE_ERROR);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_REQ_CHARINFO(CConnectedUser *connectedUser, BYTE *packet)
//{
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_REQ_CHARINFO_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_REQ_CHARINFO);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_REQ_CHARINFO %d\n"), Data.CHAR_SLOT);
//
//	CCharacter *Character = connectedUser->GetCharacterObject(Data.CHAR_SLOT);
//
//	if (Character->GetIndex())
//	{
//		if (db_GetCharacterInfo(Character->GetIndex(), Character))
//		{
//			// 성공
//			connectedUser->WritePacket(PT_CHANNEL_REQ_CHARINFO_SUCC_U, 
//				WriteBuffer, 
//				WRITE_PT_CHANNEL_REQ_CHARINFO_SUCC_U(WriteBuffer, 
//				Character->GetIndex(), 
//				Character->GetName(),
//				Character->GetJob(), 
//				Character->GetType(), 
//				Character->GetExp(), 
//				Character->GetEquip(E_HEAD),
//				Character->GetEquip(E_CHEST),
//				Character->GetEquip(E_PANTS),
//				Character->GetEquip(E_GLOVES),
//				Character->GetEquip(E_SOCKS),
//				Character->GetEquip(E_SHOES),
//				Character->GetEquip(E_ACCESSORY),
//				Character->GetEquip(E_SKILL1),
//				Character->GetEquip(E_SKILL2),
//				Character->GetEquip(E_SKILL3),
//				Character->GetEquip(E_SSHOOT1),
//				Character->GetEquip(E_SSHOOT2),
//				Character->GetEquip(E_SSHOOT3)));
//
//			CLog::WriteLog(_T("# Write packet : PT_CHANNEL_REQ_CHARINFO_SUCC_U %I64d/%s/%d/%d/%I64d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%d\n"),
//				Character->GetIndex(), 
//				Character->GetName(),
//				Character->GetJob(), 
//				Character->GetType(), 
//				Character->GetExp(), 
//				Character->GetEquip(E_HEAD),
//				Character->GetEquip(E_CHEST),
//				Character->GetEquip(E_PANTS),
//				Character->GetEquip(E_GLOVES),
//				Character->GetEquip(E_SOCKS),
//				Character->GetEquip(E_SHOES),
//				Character->GetEquip(E_ACCESSORY),
//				Character->GetEquip(E_SKILL1),
//				Character->GetEquip(E_SKILL2),
//				Character->GetEquip(E_SKILL3),
//				Character->GetEquip(E_SSHOOT1),
//				Character->GetEquip(E_SSHOOT2),
//				Character->GetEquip(E_SSHOOT3));
//		}
//		else
//		{
//			// 실패
//			WRITE_FAIL_U_PACKET(PT_CHANNEL_REQ_CHARINFO_FAIL_U, EC_DATABASE_ERROR);
//		}
//	}
//	else
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_REQ_CHARINFO_FAIL_U, EC_NO_CHARACTER_IN_SLOT);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_CHECK_CHARACTER_NAME(CConnectedUser *connectedUser, BYTE* packet)
//{
//	// 이미 존재하는 캐릭터 이름인지 확인
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_CHECK_CHARACTER_NAME);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_CHECK_CHARACTER_NAME %s\n"), Data.NAME);
//
//	if (!db_IsExistCharacterName(Data.NAME))
//	{
//		// 없으면 성공
//		connectedUser->WritePacket(PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U(WriteBuffer));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U\n"));
//	}
//	else
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U, EC_ALREADY_EXIST_CHARNAME);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_CREATE_CHARACTER(CConnectedUser *connectedUser, BYTE* packet)
//{
//	// 캐릭터 생성
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_CREATE_CHARACTER_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_CREATE_CHARACTER);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_CREATE_CHARACTER %s/%d/%d/%d\n"), Data.NAME, Data.JOB, Data.TYPE, Data.SLOT);
//
//	// 혹시나 같은 이름이 있으면 실패
//	if (db_IsExistCharacterName(Data.NAME))
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_CREATE_CHARACTER_FAIL_U, EC_ALREADY_EXIST_CHARNAME);
//
//		return;
//	}
//
//	if (db_NewCharacter(connectedUser->GetUserID(), MAKE_SERIAL(Data.TYPE, E_END, 0, 0), Data.NAME, Data.JOB, Data.TYPE, Data.SLOT))
//	{
//		// 성공
//		connectedUser->WritePacket(PT_CHANNEL_CREATE_CHARACTER_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_CREATE_CHARACTER_SUCC_U(WriteBuffer));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_CREATE_CHARACTER_SUCC_U\n"));
//	}
//	else
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_CREATE_CHARACTER_FAIL_U, EC_DATABASE_ERROR);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_EQUIP(CConnectedUser *connectedUser, BYTE* packet)
//{
//	// 장비 장착
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_EQUIP_FAIL_U);
//
//	READ_PACKET(PT_CHANNEL_EQUIP);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_EQUIP %I64d/%d/%d\n"), Data.CHAR_INDEX, Data.EQUIP_LOCATION, Data.ITEM_TYPE);
//
//	// 자기 아이템인지 확인
//	if (!db_IsOwnItem(connectedUser->GetUserID(), Data.EQUIP_LOCATION, Data.ITEM_TYPE))
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_EQUIP_FAIL_U, EC_ITEM_TYPE_INCORRECT);
//
//		return;
//	}
//
//	// 장착
//	if (db_Equipment(Data.CHAR_INDEX, Data.EQUIP_LOCATION, Data.ITEM_TYPE))
//	{
//		// 성공
//		connectedUser->WritePacket(PT_CHANNEL_EQUIP_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_EQUIP_SUCC_U(WriteBuffer));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_EQUIP_SUCC_U\n"));
//	}
//	else
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_EQUIP_FAIL_U, EC_DATABASE_ERROR);
//	}
//}
//
//VOID CGameIocp::onPT_CHANNEL_CHARACTER_SELECT(CConnectedUser *connectedUser, BYTE *packet)
//{
//	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = {0,};
//
//	CHECK_USER_STATUS(!= US_CHANNEL_ENTERED, PT_CHANNEL_CHARACTER_SELECT_FAIL_U);
//
//	// 해당 슬롯에 캐릭터가 있는지
//	READ_PACKET(PT_CHANNEL_CHARACTER_SELECT);
//	CLog::WriteLog(_T("# Read packet : PT_CHANNEL_CHARACTER_SELECT %d\n"), Data.SLOT);
//
//	CCharacter *Character = connectedUser->GetCharacterObject(Data.SLOT);
//	if (Character->GetIndex())
//	{
//		connectedUser->SetSelectedCharacterSlot(Data.SLOT);
//
//		connectedUser->WritePacket(PT_CHANNEL_CHARACTER_SELECT_SUCC_U, 
//			WriteBuffer, 
//			WRITE_PT_CHANNEL_CHARACTER_SELECT_SUCC_U(WriteBuffer));
//
//		CLog::WriteLog(_T("# Write packet : PT_CHANNEL_CHARACTER_SELECT_SUCC_U\n"));
//	}
//	else
//	{
//		WRITE_FAIL_U_PACKET(PT_CHANNEL_CHARACTER_SELECT_FAIL_U, EC_NO_CHARACTER_IN_SLOT);
//	}
//}
//#endif