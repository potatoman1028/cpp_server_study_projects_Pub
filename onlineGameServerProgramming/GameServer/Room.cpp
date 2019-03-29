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

#include "../Packet/ErrorCode.h"

CRoom::CRoom(VOID)
{
	mIndex						= 0;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));
}

CRoom::~CRoom(VOID)
{
}

BOOL CRoom::Begin(DWORD index)
{
	CThreadSync Sync;

	mIndex						= index;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

BOOL CRoom::End(VOID)
{
	CThreadSync Sync;

	mIndex						= 0;
	mIsRoomStarted				= FALSE;
	mMapIndex					= 0;
	mCurrentUserCount			= 0;
	mRootUser					= NULL;
	mIsGameStarted				= FALSE;

	mRemainGameTime				= DEFAULT_GAMETIME;

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;	
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	ZeroMemory(mTitle, sizeof(mTitle));
	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

BOOL CRoom::JoinUser(CConnectedUser *connectedUser, USHORT &slotIndex)
{
	CThreadSync Sync;

	if (!connectedUser)
		return FALSE;

	// 편이 맞는지 확인한다.
	USHORT	RedTeamCount	= 0;
	USHORT	BlueTeamCount	= 0;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i] && i < 4)
			RedTeamCount++;
		else if (mRoomUser[i] && i >= 4)
			BlueTeamCount++;
	}

	// FULL
	if (RedTeamCount + BlueTeamCount == 8)
		return FALSE;

	USHORT StartPos = RedTeamCount <= BlueTeamCount ? 0 : 4;

	for (USHORT i=StartPos;i<8;i++)
	{
		// 편에 따른 위치 선정이 필요하다.
		// 빈자리를 찾아준다.

		if (mRoomUser[i] == NULL)
		{
			mRoomUser[i] = connectedUser;
			connectedUser->SetEnteredRoom(this);

			mCurrentUserCount = min(SHORT(mCurrentUserCount++), 8);

			slotIndex = i;

			// 방 처음 생성
			if (mCurrentUserCount == 1)
			{
				mRootUser = connectedUser;
				mMapIndex = 0;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRoom::LeaveUser(BOOL isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (!connectedUser)
		return FALSE;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i] == connectedUser)
		{
			mRoomUser[i] = NULL;
			connectedUser->SetEnteredRoom(NULL);

			mCurrentUserCount	= max(SHORT(mCurrentUserCount--), 0);

			if (connectedUser == mRootUser)
			{
				mRootUser			= NULL;

				for (USHORT j=0;j<8;j++)
				{
					if (mRoomUser[j])
					{
						mRootUser = mRoomUser[j];

						break;
					}
				}
			}

			if (!isDisconnected)
			{
				// 방 나가기 성공을 보내고
				connectedUser->WritePacket(PT_ROOM_LEAVE_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_ROOM_LEAVE_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_ROOM_LEAVE_SUCC_U\n"));
			}

			WriteAll(PT_ROOM_LEAVE_M, 
				WriteBuffer, 
				WRITE_PT_ROOM_LEAVE_M(WriteBuffer, 
				(DWORD_PTR) connectedUser, 
				(DWORD_PTR) mRootUser));

			CLog::WriteLog(_T("# WriteAll packet : PT_ROOM_LEAVE_M 0x%x/0x%x\n"), 
				(DWORD_PTR) connectedUser, 
				(DWORD_PTR) mRootUser);

			if (mIsRoomStarted)
			{
				// Game End를 시켜준다.
				GameEnd(iocp);

				// 방안 모든 사용자에게 Game End를 보낸다. (이후 사용자들은 방으로 돌아가 레디가 안된 상태로 대기)
				WriteAll(PT_GAME_END_M, 
					WriteBuffer, 
					WRITE_PT_GAME_END_M(WriteBuffer));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (protocol <= 0 || !packet)
		return FALSE;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i])
			mRoomUser[i]->WritePacket(protocol, packet, packetLength);
	}

	return TRUE;
}

BOOL CRoom::WriteAllInitHPAP(VOID)
{
	CThreadSync Sync;

	BYTE		WriteBuffer[MAX_BUFFER_LENGTH] = {0,};

	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
				WriteBuffer, 
				WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
				(DWORD_PTR) mRoomUser[i], 
				mRoomUser[i]->GetHP(), 
				mRoomUser[i]->GetAP()));

			CLog::WriteLog(_T("0x%x/%d/%d\n"), 
				(DWORD_PTR) mRoomUser[i], 
				mRoomUser[i]->GetHP(), 
				mRoomUser[i]->GetAP());
		}
	}

	return TRUE;
}

DWORD CRoom::RoomStart(VOID)
{
	CThreadSync Sync;
#ifdef _CHECK_MODULE
	// 사람들이 전부 Ready인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsReady())
				return EC_ROOM_START_FAIL_ALL_READY;
		}
	}

	// 사용자가 짝수인지 확인한다.
	if (mCurrentUserCount % 2 != 0)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;

	// 편이 맞는지 확인한다.
	USHORT	RedTeamCount	= 0;
	USHORT	BlueTeamCount	= 0;

	for (USHORT i=0;i<8;i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i] && i < 4)
			RedTeamCount++;
		else if (mRoomUser[i] && i >= 4)
			BlueTeamCount++;
	}

	if (RedTeamCount != BlueTeamCount)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;
#endif
	mIsRoomStarted = TRUE;

	// 모든 사람의 상태를 변화해 준다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			mRoomUser[i]->SetStatus(US_GAME_STARTING);
			mRoomUser[i]->SetIsReady(FALSE);
		}
	}

	return 0;
}

BOOL CRoom::GameStart(VOID)
{
	CThreadSync Sync;

	// 모두 IntroComplete인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsIntroComplete())
				return FALSE;
		}
	}

	mIsGameStarted		= TRUE;
	mRemainGameTime		= DEFAULT_GAMETIME;

	// 모두일 경우 TRUE를 리턴 해 주고 LoadComplete 초기화
	// 모든 사용자 게임상태로 변경
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			mRoomUser[i]->SetStatus(US_GAME_STARTED);
			mRoomUser[i]->SetIsLoadComplete(FALSE);
			mRoomUser[i]->SetIsIntroComplete(FALSE);
			mRoomUser[i]->InitializeForGameStart();
		}
	}

	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	return TRUE;
}

BOOL CRoom::GameEnd(CGameIocp *iocp)
{
	CThreadSync Sync;

	// 승패 관련 처리

	// 모든 룸에 사용자의 HP를 더해서 많은쪽이 이긴거다
	// 혹시나 같으면 무승부
	DWORD	Team0TotalHP	= 0;
	DWORD	Team1TotalHP	= 0;
	USHORT	WinTeam			= 0;

	for (USHORT i=0;i<4;i++)
	{
		if (mRoomUser[i])
			Team0TotalHP += mRoomUser[i]->GetHP();
	}

	for (USHORT i=4;i<8;i++)
	{
		if (mRoomUser[i])
			Team1TotalHP += mRoomUser[i]->GetHP();
	}

	if (Team0TotalHP < Team1TotalHP)
		WinTeam = 1;

	// 무승부 일때
	if (Team0TotalHP == Team1TotalHP)
		WinTeam = 2;

//#ifdef _INCLUDE_DATABASE
//	// 게임 머니, 경험치 업데이트
//	for (USHORT i=0;i<8;i++)
//	{
//		if (mRoomUser[i])
//		{
//			mRoomUser[i]->SetGameMoney(mRoomUser[i]->GetGameMoney() + DEFAULT_GAMEMONEY);
//			mRoomUser[i]->SetExp(mRoomUser[i]->GetExp() + DEFAULT_EXP);
//
//			iocp->db_SetGameMoneyAndExp(mRoomUser[i]->GetUserID(), mRoomUser[i]->GetCharacterObject(mRoomUser[i]->GetSelectedCharacterSlot())->GetIndex(), mRoomUser[i]->GetGameMoney(), mRoomUser[i]->GetExp());
//		}
//	}
//#endif

	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };

	WriteAll(PT_GAME_RESULT_M, 
		WriteBuffer, 
		WRITE_PT_GAME_RESULT_M(WriteBuffer, 
		WinTeam, 
		Team0TotalHP, 
		Team1TotalHP, 0));

	CLog::WriteLog(_T("# WriteAll packet : PT_GAME_RESULT_M %d/%d/%d/%d\n"), 
		WinTeam, 
		Team0TotalHP, 
		Team1TotalHP, 
		0/*Game Money*/);

	// READY, LOADCOMPLETE, IsStarted, USER_STATS 다 초기화
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			mRoomUser[i]->SetStatus(US_ROOM_ENTERED);
			mRoomUser[i]->SetIsLoadComplete(FALSE);
			mRoomUser[i]->SetIsIntroComplete(FALSE);
			mRoomUser[i]->SetIsReady(FALSE);
			mRoomUser[i]->InitializeForGameStart();
		}
	}

	mIsRoomStarted				= FALSE;
	mIsGameStarted				= FALSE;
	mBallUser					= NULL;
	//mLastShootUser				= NULL;
	//mLastPassUser				= NULL;

	mExplosionTimeForPersion	= DEFAULT_EXPLOSION_TIME_FOR_PERSION;
	mExplosionTimeForTeam		= DEFAULT_EXPLOSION_TIME_FOR_TEAM;
	mExplosionReadyFlag			= FALSE;
	mTurnOverTime				= DEFAULT_TURNOVER_TIME;
	mTurnOverReadyFlag			= FALSE;

	return TRUE;
}

BOOL CRoom::IsSameTeam(CConnectedUser *player1, CConnectedUser *player2)
{
	CThreadSync Sync;

	if (!player1 || !player2)
		return FALSE;

	USHORT	Player1Team = 0;
	USHORT	Player2Team	= 0;

	// 우선 각자의 슬롯을 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (player1 == mRoomUser[i])
				Player1Team = i / 4;
			else if (player2 == mRoomUser[i])
				Player2Team = i / 4;
		}
	}

	if (Player1Team == Player2Team)
		return TRUE;

	return FALSE;
}

DWORD CRoom::GetTeam(CConnectedUser *player)
{
	CThreadSync Sync;

	if (!player)
		return FALSE;

	USHORT	PlayerTeam = 0;

	// 우선 자기팀을 찾고
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (player == mRoomUser[i])
			{
				PlayerTeam = i / 4;
				break;
			}
		}
	}

	return PlayerTeam;
}

BOOL CRoom::IsAllOut(CConnectedUser *player)
{
	CThreadSync Sync;

	if (!player)
		return FALSE;

	USHORT	PlayerTeam = 0;

	// 우선 자기팀을 찾고
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (player == mRoomUser[i])
			{
				PlayerTeam = i / 4;
				break;
			}
		}
	}

	// 같은 팀 사람들 HP 확인
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (i / 4 == PlayerTeam)
			{
				// 같은 팀의 경우
				if (mRoomUser[i]->GetHP() > 0)
					return FALSE;
			}
		}
	}

	return TRUE;
}

VOID CRoom::PassInfo(CConnectedUser *shooter, CConnectedUser *target)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 현재 공 소유권과 확인한다.
	if (shooter == GetBallUser())
	{
		// 패스가 상대방이 아닌지 확인
#ifdef _CHECK_MODULE
		if (IsSameTeam(shooter, target))
		{
#endif
			// 공 소유권을 비워준다.
			//SetBallUser(NULL);
			// 마지막 패서로 만들어준다.
			//SetLastPassUser(shooter);

			// 성공을 날려주고
			shooter->WritePacket(PT_GAME_PASS_INFO_SUCC_U, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_INFO_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_GAME_PASS_INFO_SUCC_U\n"));

			// 방 사람들에게 알려준다.
			WriteAll(PT_GAME_PASS_INFO_M, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_INFO_M(WriteBuffer, 
				(DWORD_PTR) shooter, 
				(DWORD_PTR) target));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PASS_INFO_M 0x%x/0x%x\n"), (DWORD_PTR) shooter, 
				(DWORD_PTR) target);
#ifdef _CHECK_MODULE			
		}
		else
		{
			WRITE_FAIL_U_PACKET2(shooter, PT_GAME_PASS_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
#endif
	}
	else
	{
		WRITE_FAIL_U_PACKET2(shooter, PT_GAME_PASS_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

VOID CRoom::PassReceiveInfo(CConnectedUser *shooter, CConnectedUser *target)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 마지막 패서 확인
	//if (GetLastPassUser() == shooter)
	//{
	if (GetBallUser())
	{
		// 슛 상대가 같은 편인지 확인
#ifdef _CHECK_MODULE
		if (IsSameTeam(shooter, target))
		{
#endif
			// 마지막 패서를 풀어준다.
			//SetLastPassUser(NULL);
			mExplosionReadyFlag = FALSE;

			// 공 소요권 이전
			SetBallUser(target);

			// 패스를 받았으니 개인 폭발 리셋
			ResetExplosionTimeForPersion();

			// 성공 패킷 날리고
			target->WritePacket(PT_GAME_PASS_RECEIVE_INFO_SUCC_U, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_RECEIVE_INFO_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_GAME_PASS_RECEIVE_INFO_SUCC_U\n"));
			// 모두에게 알려준다.

			WriteAll(PT_GAME_PASS_RECEIVE_INFO_M, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_RECEIVE_INFO_M(WriteBuffer, 
				(DWORD_PTR) shooter, 
				(DWORD_PTR) target));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PASS_RECEIVE_INFO_M 0x%x/0x%x\n"), 
				(DWORD_PTR) shooter, 
				(DWORD_PTR) target);
#ifdef _CHECK_MODULE
		}
		else
		{
			WRITE_FAIL_U_PACKET2(target, PT_GAME_PASS_RECEIVE_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
		}
#endif
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_PASS_RECEIVE_INFO_FAIL_U, EC_GAME_LASTPASSUSER_INCORRECT);
	//}
	}
	else
	{
		WRITE_FAIL_U_PACKET2(target, PT_GAME_PASS_RECEIVE_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

VOID CRoom::ShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 던진 사람이 공 소유자 인지 확인
	if (shooter == GetBallUser())
	{
		// 슛 상대가 같은 편이 아닌지 확인
		if (!IsSameTeam(shooter, target))
		{
			// 슛 상대가 코트밖인지 안인지 확인
			//if (target->GetIsInCourt())
			//{
				// 공 소유권을 잃는다.
				//SetBallUser(NULL);
				// 마지막 슈터로 선정된다.
				//SetLastShootUser(shooter);
		
				mExplosionReadyFlag = FALSE;

				ResetExplosionTime();

				// 성공 전송하고 모든사람들에게 알려준다.
				shooter->WritePacket(PT_GAME_SHOOT_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SHOOT_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SHOOT_INFO_SUCC_U\n"));

				WriteAll(PT_GAME_SHOOT_INFO_M, 
					WriteBuffer, 
					WRITE_PT_GAME_SHOOT_INFO_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SHOOT_INFO_M 0x%x/0x%x/%d\n"), 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power);
			//}
			//else
			//{
			//	WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SHOOT_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
			//}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SHOOT_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SHOOT_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

VOID CRoom::ShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 마지막 슈터 확인
	//if (GetLastShootUser() == shooter)
	//{
	if (GetBallUser())
	{
		// 슛 상대가 같은 편이 아닌지 확인
		if (!IsSameTeam(shooter, target) && !IsSameTeam(GetBallUser(), target))
		{
			// 타겟이 안인지 확인
			//if (target->GetIsInCourt())
			//{
				// 마지막 슈터를 풀어준다.
				//SetLastShootUser(NULL);
				mExplosionReadyFlag = FALSE;

				// 공 소요권 이전
				SetBallUser(target);

				// 공수가 변경되니 폭발 시간 리셋
				ResetExplosionTime();

				target->SetAP(min(INT(target->GetAP() + DEFAULT_CATCH_AP), DEFAULT_MAX_AP));

				WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
					WriteBuffer, 
					WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					target->GetHP(), 
					target->GetAP()));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
					(DWORD_PTR) shooter,
					target->GetHP(),
					target->GetAP());

				// 성공 패킷 날리고
				target->WritePacket(PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U\n"));
				// 모두에게 알려준다.

				WriteAll(PT_GAME_SHOOT_RECEIVE_INFO_M, 
					WriteBuffer, 
					WRITE_PT_GAME_SHOOT_RECEIVE_INFO_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SHOOT_RECEIVE_INFO_M 0x%x/0x%x/%d\n"), 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power);
			//}
			//else
			//{
			//	WRITE_FAIL_U_PACKET2(target, PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U, EC_GAME_TARGETUSER_NOTINCOURT);
			//}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(target, PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
		}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U, EC_GAME_LASTSHOOTUSER_INCORRECT);
	//}
	}
	else
	{
		WRITE_FAIL_U_PACKET2(target, PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);	
	}
}

VOID CRoom::ShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power, CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슈터를 확인해 주고, 맞는 사람도 확인해 준다.(혹시나) 코트안에 있는지 확인
	//if (shooter == GetLastShootUser())
	//{
	//if (GetBallUser())
	//{
		// 다른 편인지 확인해 준다.
		if (!IsSameTeam(shooter, target))// && !IsSameTeam(GetBallUser(), target))
		{
			//if (target->GetIsInCourt())
			//{
				// 슈터를 풀어준다.
				//SetLastShootUser(NULL);
				mTurnOverReadyFlag	= FALSE;

				// 공 소유권도 풀어준다.
				SetBallUser(NULL);

				// 맞았으니 소유권 변경으로 인해 폭발 시간 리셋
				ResetTurnOverTime();

				// 때린 사람, 맞은 사람 HP, AP를 계산해 준다.
				//target->SetAP(min(INT(target->GetAP() + DEFAULT_AP * power), DEFAULT_MAX_AP));
				//target->SetHP(max(INT(target->GetHP() - (DWORD)(DEFAULT_DAMAGE * power * GetAdVantage(shooter))), 0));
				target->SetAP(min(INT(target->GetAP() + DEFAULT_AP), DEFAULT_MAX_AP));
				target->SetHP(max(INT(target->GetHP() - (DWORD)(DEFAULT_DAMAGE * GetAdVantage(shooter))), 0));

				// 성공 패킷 전송
				target->WritePacket(PT_GAME_HIT_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_HIT_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_HIT_INFO_SUCC_U\n"));

				// 모두에게 알려준다.
				WriteAll(PT_GAME_HIT_INFO_M, 
					WriteBuffer, 
					WRITE_PT_GAME_HIT_INFO_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_HIT_INFO_M 0x%x/0x%x/%d\n"), 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					power);

				// HP, AP 변화도 모두에게 알려준다.
				WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
					WriteBuffer, 
					WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
					(DWORD_PTR) target, 
					target->GetHP(), 
					target->GetAP()));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
					(DWORD_PTR) target,
					target->GetHP(),
					target->GetAP());

				// HP가 0이면 Out 시켜준다.
				if (target->GetHP() == 0 && target->GetIsInCourt())
				{
					target->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) target));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) target);
				}

				// All Out인지 확인한다
				if (IsAllOut(target))
				{
					GameEnd(gameIocp);

					WriteAll(PT_GAME_END_M, WriteBuffer, WRITE_PT_GAME_END_M(WriteBuffer));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
				}
			//}
			//else
			//{
			//	WRITE_FAIL_U_PACKET2(target, PT_GAME_HIT_INFO_FAIL_U, EC_GAME_TARGETUSER_NOTINCOURT);
			//}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(target, PT_GAME_HIT_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
		}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_HIT_INFO_FAIL_U, EC_GAME_LASTSHOOTUSER_INCORRECT);						
	//}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_HIT_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	//}
}

VOID CRoom::SpecialShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 던진 사람이 공 소유자 인지 확인
	if (shooter == GetBallUser())
	{
		// 슛 상대가 같은 편이 아닌지 확인
		if (!IsSameTeam(shooter, target))
		{
			// 슛 상대가 코트밖인지 안인지 확인
			if (target->GetIsInCourt())
			{
#ifdef _CHECK_MODULE
				// AP가 되는지를 확인한다
				if (shooter->GetAP() == DEFAULT_MAX_AP)
				{
#endif
					// 공 소유권을 잃는다.
					//SetBallUser(NULL);
					// 마지막 패서로 선정된다.
					//SetLastShootUser(shooter);
					mExplosionReadyFlag = FALSE;

					ResetExplosionTime();

					// AP 모두 소모
					shooter->SetAP(0);

					// HP, AP 변화도 모두에게 알려준다.
					WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
						WriteBuffer, 
						WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
						(DWORD_PTR) shooter, 
						shooter->GetHP(), 
						shooter->GetAP()));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
						(DWORD_PTR) shooter,
						shooter->GetHP(),
						shooter->GetAP());

					// 성공 전송하고 모든사람들에게 알려준다.
					shooter->WritePacket(PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U, 
						WriteBuffer, 
						WRITE_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U(WriteBuffer));

					CLog::WriteLog(_T("# Write packet : PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U\n"));

					WriteAll(PT_GAME_SPECIAL_SHOOT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_SPECIAL_SHOOT_INFO_M(WriteBuffer, 
						(DWORD_PTR) shooter, 
						(DWORD_PTR) target, 
						shootType));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SPECIAL_SHOOT_INFO_M 0x%x/0x%x/%d\n"), 
						(DWORD_PTR) shooter, 
						(DWORD_PTR) target, 
						shootType);
#ifdef _CHECK_MODULE
				}
				else
				{
					WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U, EC_GAME_SPECIALSHOOT_AP_INCORRECT);
				}
#endif
			}
			else
			{
				WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
			}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U, EC_GAME_TARGETUSER_INCORRECT);
		}
	}
	else
	{
		WRITE_FAIL_U_PACKET2(shooter, PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

VOID CRoom::SpecialShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 마지막 슈터 확인
	//if (GetLastShootUser() == shooter)
	//{
	if (GetBallUser())
	{
		// 슛 상대가 같은 편이 아닌지 확인
		if (!IsSameTeam(shooter, target) && !IsSameTeam(GetBallUser(), target))
		{
			// 타겟이 안인지 확인
			if (target->GetIsInCourt())
			{
				// 마지막 슈터를 풀어준다.
				//SetLastShootUser(NULL);
				mExplosionReadyFlag = FALSE;

				// 공 소요권 이전
				SetBallUser(target);

				// 공수가 변경되니 폭발시간 리셋
				ResetExplosionTime();

				target->SetAP(min(INT(target->GetAP() + DEFAULT_SPECIAL_CATCH_AP), DEFAULT_MAX_AP));

				WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
					WriteBuffer, 
					WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					target->GetHP(), 
					target->GetAP()));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
					(DWORD_PTR) shooter,
					target->GetHP(),
					target->GetAP());

				// 성공 패킷 날리고
				target->WritePacket(PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U\n"));
				// 모두에게 알려준다.

				WriteAll(PT_GAME_SPECIAL_RECEIVE_INFO_M, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					shootType));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SPECIAL_RECEIVE_INFO_M 0x%x/0x%x/%d\n"), 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					shootType);
			}
			else
			{
				WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U, EC_GAME_TARGETUSER_NOTINCOURT);
			}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
		}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U, EC_GAME_LASTSHOOTUSER_INCORRECT);
	//}
	}
	else
	{
		WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

VOID CRoom::SpecialShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType, CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슈터를 확인해 주고, 맞는 사람도 확인해 준다.(혹시나) 코트안에 있는지 확인
	//if (shooter == GetLastShootUser())
	//{
	//if (GetBallUser())
	//{
		// 다른 편인지 확인해 준다.
		if (!IsSameTeam(shooter, target))// && !IsSameTeam(GetBallUser(), target))
		{
			if (target->GetIsInCourt())
			{
				// 슈터를 풀어준다.
				//SetLastShootUser(NULL);
				mTurnOverReadyFlag	= FALSE;

				// 공 소유권을 풀어준다.
				SetBallUser(NULL);

				// 맞았으니 소유권 변경으로 인해 폭발 시간 리셋
				ResetTurnOverTime();

				// 때린 사람, 맞은 사람 HP, AP를 계산해 준다.
				target->SetAP(min(INT(target->GetAP() + DEFAULT_SPECIAL_AP), DEFAULT_MAX_AP));
				target->SetHP(max(INT(target->GetHP() - DEFAULT_SPECIAL_DAMAGE), 0));

				// 성공 패킷 전송
				target->WritePacket(PT_GAME_SPECIAL_HIT_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_HIT_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SPECIAL_HIT_INFO_SUCC_U\n"));

				// 모두에게 알려준다.
				WriteAll(PT_GAME_SPECIAL_HIT_INFO_M, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_HIT_INFO_M(WriteBuffer, 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					shootType));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SPECIAL_HIT_INFO_M 0x%x/0x%x/%d\n"), 
					(DWORD_PTR) shooter, 
					(DWORD_PTR) target, 
					shootType);

				// HP, AP 변화도 모두에게 알려준다.
				WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
					WriteBuffer, 
					WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
					(DWORD_PTR) target, 
					target->GetHP(), 
					target->GetAP()));

				CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
					(DWORD_PTR) target,
					target->GetHP(),
					target->GetAP());

				// HP가 0이면 Out 시켜준다.
				if (target->GetHP() == 0)
				{
					target->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) target));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) target);
				}

				// All Out인지 확인한다
				if (IsAllOut(target))
				{
					GameEnd(gameIocp);

					WriteAll(PT_GAME_END_M, WriteBuffer, WRITE_PT_GAME_END_M(WriteBuffer));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
				}
			}
			else
			{
				WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_HIT_INFO_FAIL_U, EC_GAME_TARGETUSER_NOTINCOURT);
			}
		}
		else
		{
			WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_HIT_INFO_FAIL_U, EC_GAME_USERTEAM_INCORRECT);
		}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_HIT_INFO_FAIL_U, EC_GAME_LASTSHOOTUSER_INCORRECT);						
	//}
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(target, PT_GAME_SPECIAL_HIT_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	//}
}

VOID CRoom::FieldBall(CConnectedUser *user)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 현재 소유권이 자기 자신인지 확인
	//if (GetBallUser() == user)
	//{
		mTurnOverReadyFlag	= FALSE;

		// 공 소유권 이전
		SetBallUser(NULL);

		ResetTurnOverTime();

		// 성공 보내고
		user->WritePacket(PT_GAME_FIELDBALL_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_FIELDBALL_SUCC_U\n"));

		// 모두에게 알려준다
		WriteAll(PT_GAME_FIELDBALL_M, 
			WriteBuffer, 
			WRITE_PT_GAME_FIELDBALL_M(WriteBuffer, 
			(DWORD_PTR) user));

		CLog::WriteLog(_T("# WriteAll packet : PT_GAME_FIELDBALL_M 0x%x\n"), (DWORD_PTR) user);
	//}
	//else
	//{
	//	WRITE_FAIL_U_PACKET2(user, PT_GAME_FIELDBALL_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	//}
}

VOID CRoom::GetFieldBall(CConnectedUser *user, DWORD parameter)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 현재 소유권이 없는지 확인
	if (GetBallUser() == NULL)
	{
		mExplosionReadyFlag = FALSE;

		// 공 소유권 이전
		SetBallUser(user);

		ResetExplosionTime();

		// 성공 보내고
		user->WritePacket(PT_GAME_GET_FIELDBALL_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_GET_FIELDBALL_SUCC_U\n"));

		// 모두에게 알려준다
		WriteAll(PT_GAME_GET_FIELDBALL_M, 
			WriteBuffer, 
			WRITE_PT_GAME_GET_FIELDBALL_M(WriteBuffer, 
			(DWORD_PTR) user,
			parameter));

		CLog::WriteLog(_T("# WriteAll packet : PT_GAME_GET_FIELDBALL_M 0x%x\n"), (DWORD_PTR) user);
	}
	else
	{
		WRITE_FAIL_U_PACKET2(user, PT_GAME_GET_FIELDBALL_FAIL_U, EC_GAME_BALLUSER_INCORRECT);
	}
}

BOOL CRoom::IsAllLoadComplete(VOID)
{
	CThreadSync Sync;

	// 모두 LoadComplete인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsLoadComplete())
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CRoom::IsAllIntroComplete(VOID)
{
	CThreadSync Sync;

	// 모두 IntroComplete인지 확인한다.
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsIntroComplete())
				return FALSE;
		}
	}

	return TRUE;
}

FLOAT CRoom::GetAdVantage(CConnectedUser *user)
{
	CThreadSync Sync;

	if (!user)
		return FALSE;

	//_tcsupr
	if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs000")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs001")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs002")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs003")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs004")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs005")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs006")))
		return DOGS000_ADVANTAGE;
	else if (!_tcscmp(_tcslwr(user->GetUserID()), _T("dogs007")))
		return DOGS000_ADVANTAGE;

	return 1.0F;
}

VOID CRoom::SkillInfo(CConnectedUser *actor, CConnectedUser *target, USHORT skillType)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	// 슛 상대가 코트밖인지 안인지 확인
	if (actor->GetIsInCourt())
	{
#ifdef _CHECK_MODULE
		// AP가 되는지를 확인한다
		if (actor->GetAP() >= DEFAULT_SPEND_AP)
		{
#endif
			switch (skillType)
			{
			// 페인트슛
			case 0: 
				break;
			// 스루패스
			case 1: 
				break;
			// 인터셉트
			case 2:	
				// 현재 공소유자와 다른 편일 경우만
				if (!IsSameTeam(actor, GetBallUser()))
				{
					mExplosionReadyFlag = FALSE;
					// 공 소유권 변경
					SetBallUser(actor);

					ResetExplosionTime();
				}
				else
				{
					WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);

					return;
				}
				break;
			// 카운터슛 레디
			case 3:
				// 원래는 AP 소모등의 작업이 들어간다.
				// 지금은 제외
				break;
			// 카운터슛
			case 4:
				// 현재 공소유자와 다른 편일 경우만
				if (GetBallUser())
				{
					if (!IsSameTeam(actor, GetBallUser()))
					{
						mExplosionReadyFlag = FALSE;
						// 공 소유권 변경
						SetBallUser(actor);

						ResetExplosionTime();
					}
					else
					{
						WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);

						return;
					}
				}
				else
				{
					WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);

					return;
				}
				break;
			}

			actor->SetAP(max(INT(actor->GetAP() - DEFAULT_SPEND_AP), 0));

			// HP, AP 변화도 모두에게 알려준다.
			WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
				WriteBuffer, 
				WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
				(DWORD_PTR) actor, 
				actor->GetHP(), 
				actor->GetAP()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
				(DWORD_PTR) actor,
				actor->GetHP(),
				actor->GetAP());

			// 성공 전송하고 모든사람들에게 알려준다.
			actor->WritePacket(PT_GAME_SKILL_INFO_SUCC_U, 
				WriteBuffer, 
				WRITE_PT_GAME_SKILL_INFO_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_GAME_SKILL_INFO_SUCC_U\n"));

			WriteAll(PT_GAME_SKILL_INFO_M, 
				WriteBuffer, 
				WRITE_PT_GAME_SKILL_INFO_M(WriteBuffer, 
				(DWORD_PTR) actor, 
				skillType,
				(DWORD_PTR) target));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_SKILL_INFO_M 0x%x/%d/0x%x\n"), 
				(DWORD_PTR) actor, 
				skillType,
				(DWORD_PTR) target);
#ifdef _CHECK_MODULE
		}
		else
		{
			WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_SPECIALSHOOT_AP_INCORRECT);
		}
#endif
	}
	else
	{
		WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_CANNOT_USESKILL_OUTCOURT);
	}
}

VOID CRoom::CheckExplosion(CGameIocp *gameIocp)
{
	CThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (GetBallUser())
	{
		if (IsReadyExplosion())
		{
			// 폭발 3초전
			// 모두에게 알려준다.
			WriteAll(PT_GAME_EXPLOSION_READY_M, 
				WriteBuffer, 
				WRITE_PT_GAME_EXPLOSION_READY_M(WriteBuffer, 
				(DWORD_PTR) GetBallUser()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_READY_M 0x%x\n"), 
				(DWORD_PTR) GetBallUser());
		}

		if (!DecreaseExplosionTime())
		{
			// 폭발
			// 때린 사람, 맞은 사람 HP, AP를 계산해 준다.
			GetBallUser()->SetHP(max(INT(GetBallUser()->GetHP() - DEFAULT_DAMAGE), 0));

			// 모두에게 알려준다.
			WriteAll(PT_GAME_EXPLOSION_M, 
				WriteBuffer, 
				WRITE_PT_GAME_EXPLOSION_M(WriteBuffer, 
				(DWORD_PTR) GetBallUser()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_M 0x%x\n"), 
				(DWORD_PTR) GetBallUser());

			// HP, AP 변화도 모두에게 알려준다.
			WriteAll(PT_GAME_PC_INFO_CHANGED_M, 
				WriteBuffer, 
				WRITE_PT_GAME_PC_INFO_CHANGED_M(WriteBuffer, 
				(DWORD_PTR) GetBallUser(), 
				GetBallUser()->GetHP(), 
				GetBallUser()->GetAP()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_PC_INFO_CHANGED_M 0x%x/%d/%d\n"),
				(DWORD_PTR) GetBallUser(),
				GetBallUser()->GetHP(),
				GetBallUser()->GetAP());

			if (GetBallUser()->GetIsInCourt())
			{
				// HP가 0이면 Out 시켜준다.
				if (GetBallUser()->GetHP() == 0)
				{
					GetBallUser()->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) GetBallUser()));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) GetBallUser());
				}

				// All Out인지 확인한다
				if (IsAllOut(GetBallUser()))
				{
					GameEnd(gameIocp);

					WriteAll(PT_GAME_END_M, WriteBuffer, WRITE_PT_GAME_END_M(WriteBuffer));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
				}
			}

			mTurnOverReadyFlag = FALSE;
			// 공 소유권을 풀어준다.
			SetBallUser(NULL);

			ResetTurnOverTime();

			// 한번 터지면 리셋
			ResetExplosionTime();
		}

		printf("# Explosion : %d %d\n", mExplosionTimeForPersion, mExplosionTimeForTeam);
	}
#ifdef _TURNOVER
	else
	{
		// 공이 바닥에 떨어져 있는경우
		if (IsReadyTurnOver())
		{
			// 턴오버 3초전
			// 모두에게 알려준다.
			WriteAll(PT_GAME_TURNOVER_READY_M, 
				WriteBuffer, 
				WRITE_PT_GAME_TURNOVER_READY_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_TURNOVER_READY_M\n"));
		}

		if (!DecreaseTurnOverTime())
		{
			// 모두에게 알려준다.
			WriteAll(PT_GAME_TURNOVER_M, 
				WriteBuffer, 
				WRITE_PT_GAME_TURNOVER_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_M\n"));

			mTurnOverReadyFlag = FALSE;
			// 한번 터지면 리셋
			ResetTurnOverTime();
		}

		printf("# TurnOver : %d\n", mTurnOverTime);
	}
#endif
}