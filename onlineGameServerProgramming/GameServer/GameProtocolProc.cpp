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

VOID CGameIocp::onPT_GAME_PASS_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_PASS_INFO_FAIL_U);

	// 패킷 받기
	READ_PACKET(PT_GAME_PASS_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_PASS_INFO 0x%x/0x%x\n"), Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID);

	CRoom *Room = connectedUser->GetEnteredRoom();

	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.SHOOTER_SESSION_ID)
			Room->PassInfo(connectedUser, (CConnectedUser*) Data.TARGET_SESSION_ID);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_PASS_INFO_FAIL_U, EC_GAME_SHOOTERUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_PASS_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_SHOOT_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SHOOT_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SHOOT_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SHOOT_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_POWER);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.SHOOTER_SESSION_ID)
			Room->ShootInfo(connectedUser, (CConnectedUser*) Data.TARGET_SESSION_ID, Data.SHOOT_POWER);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SHOOT_INFO_FAIL_U, EC_GAME_SHOOTERUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SHOOT_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_HIT_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_HIT_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_HIT_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_HIT_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_POWER);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.TARGET_SESSION_ID)
			Room->ShootHitInfo((CConnectedUser*) Data.SHOOTER_SESSION_ID, connectedUser, Data.SHOOT_POWER, this);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_HIT_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_HIT_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_SHOOT_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SHOOT_RECEIVE_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SHOOT_RECEIVE_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_POWER);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.TARGET_SESSION_ID)
			Room->ShootReceiveInfo((CConnectedUser*) Data.SHOOTER_SESSION_ID, connectedUser, Data.SHOOT_POWER);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_PASS_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_PASS_RECEIVE_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_PASS_RECEIVE_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_PASS_RECEIVE_INFO 0x%x/0x%x\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.TARGET_SESSION_ID)
			Room->PassReceiveInfo((CConnectedUser*) Data.SHOOTER_SESSION_ID, connectedUser);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_PASS_RECEIVE_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_PASS_RECEIVE_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_FIELDBALL(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_FIELDBALL_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_FIELDBALL);
	CLog::WriteLog(_T("# Read packet : PT_GAME_FIELDBALL 0x%x\n"), Data.DROP_SESSION_ID);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.DROP_SESSION_ID)
			Room->FieldBall(connectedUser);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_FIELDBALL_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_FIELDBALL_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_GET_FIELDBALL(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_GET_FIELDBALL_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_GET_FIELDBALL);
	CLog::WriteLog(_T("# Read packet : PT_GAME_GET_FIELDBALL 0x%x/%d\n"), Data.GET_SESSION_ID, Data.PARAMETER);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.GET_SESSION_ID)
			Room->GetFieldBall(connectedUser, Data.PARAMETER);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_GET_FIELDBALL_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_GET_FIELDBALL_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_CHANGE_BALLOWNER(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	READ_PACKET(PT_GAME_CHANGE_BALLOWNER);
	CLog::WriteLog(_T("# Read packet : PT_GAME_CHANGE_BALLOWNER 0x%x\n"), Data.NEW_OWNER_SESSION_ID);

	CRoom *Room = connectedUser->GetEnteredRoom();
	CConnectedUser *NewOwner = (CConnectedUser*) Data.NEW_OWNER_SESSION_ID;

	Room->SetBallUser(NewOwner);

	Room->WriteAll(PT_GAME_CHANGE_BALLOWNER_M, WriteBuffer, WRITE_PT_GAME_CHANGE_BALLOWNER_M(WriteBuffer, Data.NEW_OWNER_SESSION_ID));
	CLog::WriteLog(_T("# WriteAll packet : WRITE_PT_GAME_CHANGE_BALLOWNER_M 0x%x\n"), Data.NEW_OWNER_SESSION_ID);
}

VOID CGameIocp::onPT_GAME_SPECIAL_SHOOT_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SPECIAL_SHOOT_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SPECIAL_SHOOT_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_TYPE);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.SHOOTER_SESSION_ID)
			Room->SpecialShootInfo(connectedUser, (CConnectedUser*) Data.TARGET_SESSION_ID, Data.SHOOT_TYPE);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U, EC_GAME_SHOOTERUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_SPECIAL_HIT_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SPECIAL_HIT_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SPECIAL_HIT_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SPECIAL_HIT_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_TYPE);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.TARGET_SESSION_ID)
			Room->SpecialShootHitInfo((CConnectedUser*) Data.SHOOTER_SESSION_ID, connectedUser, Data.SHOOT_TYPE, this);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SPECIAL_HIT_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SPECIAL_HIT_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_SPECIAL_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SPECIAL_RECEIVE_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SPECIAL_RECEIVE_INFO 0x%x/0x%x/%d\n"), 
		Data.SHOOTER_SESSION_ID, 
		Data.TARGET_SESSION_ID, 
		Data.SHOOT_TYPE);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.TARGET_SESSION_ID)
			Room->SpecialShootReceiveInfo((CConnectedUser*) Data.SHOOTER_SESSION_ID, connectedUser, Data.SHOOT_TYPE);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U);
	}
}

VOID CGameIocp::onPT_GAME_SKILL_INFO(CConnectedUser *connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	CHECK_USER_STATUS(!= US_GAME_STARTED, PT_GAME_SKILL_INFO_FAIL_U);
	// 패킷 받기
	READ_PACKET(PT_GAME_SKILL_INFO);
	CLog::WriteLog(_T("# Read packet : PT_GAME_SKILL_INFO 0x%x/%d/0x%x\n"), 
		Data.SKILL_SESSION_ID, 
		Data.SKILL_TYPE, 
		Data.TARGET_SESSION_ID);

	CRoom *Room = connectedUser->GetEnteredRoom();
	// 룸 오브젝트 확인
	if (Room)
	{
		if (connectedUser == (CConnectedUser*) Data.SKILL_SESSION_ID)
			Room->SkillInfo(connectedUser, (CConnectedUser*) Data.TARGET_SESSION_ID, Data.SKILL_TYPE);
		else
		{
			WRITE_FAIL_U_PACKET(PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		ROOM_OBJCT_FAIL(PT_GAME_SKILL_INFO_FAIL_U);
	}
}