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

	// ���� �´��� Ȯ���Ѵ�.
	USHORT	RedTeamCount	= 0;
	USHORT	BlueTeamCount	= 0;

	for (USHORT i=0;i<8;i++)
	{
		// �ش� ����ڸ� ã�´�.
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
		// �� ���� ��ġ ������ �ʿ��ϴ�.
		// ���ڸ��� ã���ش�.

		if (mRoomUser[i] == NULL)
		{
			mRoomUser[i] = connectedUser;
			connectedUser->SetEnteredRoom(this);

			mCurrentUserCount = min(SHORT(mCurrentUserCount++), 8);

			slotIndex = i;

			// �� ó�� ����
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
		// �ش� ����ڸ� ã�´�.
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
				// �� ������ ������ ������
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
				// Game End�� �����ش�.
				GameEnd(iocp);

				// ��� ��� ����ڿ��� Game End�� ������. (���� ����ڵ��� ������ ���ư� ���� �ȵ� ���·� ���)
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
		// �ش� ����ڸ� ã�´�.
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
	// ������� ���� Ready���� Ȯ���Ѵ�.
	for (USHORT i=0;i<8;i++)
	{
		// �ش� ����ڸ� ã�´�.
		if (mRoomUser[i])
		{
			if (!mRoomUser[i]->GetIsReady())
				return EC_ROOM_START_FAIL_ALL_READY;
		}
	}

	// ����ڰ� ¦������ Ȯ���Ѵ�.
	if (mCurrentUserCount % 2 != 0)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;

	// ���� �´��� Ȯ���Ѵ�.
	USHORT	RedTeamCount	= 0;
	USHORT	BlueTeamCount	= 0;

	for (USHORT i=0;i<8;i++)
	{
		// �ش� ����ڸ� ã�´�.
		if (mRoomUser[i] && i < 4)
			RedTeamCount++;
		else if (mRoomUser[i] && i >= 4)
			BlueTeamCount++;
	}

	if (RedTeamCount != BlueTeamCount)
		return EC_ROOM_START_FAIL_TEAM_INCORRECT;
#endif
	mIsRoomStarted = TRUE;

	// ��� ����� ���¸� ��ȭ�� �ش�.
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

	// ��� IntroComplete���� Ȯ���Ѵ�.
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

	// ����� ��� TRUE�� ���� �� �ְ� LoadComplete �ʱ�ȭ
	// ��� ����� ���ӻ��·� ����
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

	// ���� ���� ó��

	// ��� �뿡 ������� HP�� ���ؼ� �������� �̱�Ŵ�
	// Ȥ�ó� ������ ���º�
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

	// ���º� �϶�
	if (Team0TotalHP == Team1TotalHP)
		WinTeam = 2;

//#ifdef _INCLUDE_DATABASE
//	// ���� �Ӵ�, ����ġ ������Ʈ
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

	// READY, LOADCOMPLETE, IsStarted, USER_STATS �� �ʱ�ȭ
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

	// �켱 ������ ������ Ȯ���Ѵ�.
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

	// �켱 �ڱ����� ã��
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

	// �켱 �ڱ����� ã��
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

	// ���� �� ����� HP Ȯ��
	for (USHORT i=0;i<8;i++)
	{
		if (mRoomUser[i])
		{
			if (i / 4 == PlayerTeam)
			{
				// ���� ���� ���
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

	// ���� �� �����ǰ� Ȯ���Ѵ�.
	if (shooter == GetBallUser())
	{
		// �н��� ������ �ƴ��� Ȯ��
#ifdef _CHECK_MODULE
		if (IsSameTeam(shooter, target))
		{
#endif
			// �� �������� ����ش�.
			//SetBallUser(NULL);
			// ������ �м��� ������ش�.
			//SetLastPassUser(shooter);

			// ������ �����ְ�
			shooter->WritePacket(PT_GAME_PASS_INFO_SUCC_U, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_INFO_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_GAME_PASS_INFO_SUCC_U\n"));

			// �� ����鿡�� �˷��ش�.
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

	// ������ �м� Ȯ��
	//if (GetLastPassUser() == shooter)
	//{
	if (GetBallUser())
	{
		// �� ��밡 ���� ������ Ȯ��
#ifdef _CHECK_MODULE
		if (IsSameTeam(shooter, target))
		{
#endif
			// ������ �м��� Ǯ���ش�.
			//SetLastPassUser(NULL);
			mExplosionReadyFlag = FALSE;

			// �� �ҿ�� ����
			SetBallUser(target);

			// �н��� �޾����� ���� ���� ����
			ResetExplosionTimeForPersion();

			// ���� ��Ŷ ������
			target->WritePacket(PT_GAME_PASS_RECEIVE_INFO_SUCC_U, 
				WriteBuffer, 
				WRITE_PT_GAME_PASS_RECEIVE_INFO_SUCC_U(WriteBuffer));

			CLog::WriteLog(_T("# Write packet : PT_GAME_PASS_RECEIVE_INFO_SUCC_U\n"));
			// ��ο��� �˷��ش�.

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

	// ���� ����� �� ������ ���� Ȯ��
	if (shooter == GetBallUser())
	{
		// �� ��밡 ���� ���� �ƴ��� Ȯ��
		if (!IsSameTeam(shooter, target))
		{
			// �� ��밡 ��Ʈ������ ������ Ȯ��
			//if (target->GetIsInCourt())
			//{
				// �� �������� �Ҵ´�.
				//SetBallUser(NULL);
				// ������ ���ͷ� �����ȴ�.
				//SetLastShootUser(shooter);
		
				mExplosionReadyFlag = FALSE;

				ResetExplosionTime();

				// ���� �����ϰ� ������鿡�� �˷��ش�.
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

	// ������ ���� Ȯ��
	//if (GetLastShootUser() == shooter)
	//{
	if (GetBallUser())
	{
		// �� ��밡 ���� ���� �ƴ��� Ȯ��
		if (!IsSameTeam(shooter, target) && !IsSameTeam(GetBallUser(), target))
		{
			// Ÿ���� ������ Ȯ��
			//if (target->GetIsInCourt())
			//{
				// ������ ���͸� Ǯ���ش�.
				//SetLastShootUser(NULL);
				mExplosionReadyFlag = FALSE;

				// �� �ҿ�� ����
				SetBallUser(target);

				// ������ ����Ǵ� ���� �ð� ����
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

				// ���� ��Ŷ ������
				target->WritePacket(PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U\n"));
				// ��ο��� �˷��ش�.

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

	// ���͸� Ȯ���� �ְ�, �´� ����� Ȯ���� �ش�.(Ȥ�ó�) ��Ʈ�ȿ� �ִ��� Ȯ��
	//if (shooter == GetLastShootUser())
	//{
	//if (GetBallUser())
	//{
		// �ٸ� ������ Ȯ���� �ش�.
		if (!IsSameTeam(shooter, target))// && !IsSameTeam(GetBallUser(), target))
		{
			//if (target->GetIsInCourt())
			//{
				// ���͸� Ǯ���ش�.
				//SetLastShootUser(NULL);
				mTurnOverReadyFlag	= FALSE;

				// �� �����ǵ� Ǯ���ش�.
				SetBallUser(NULL);

				// �¾����� ������ �������� ���� ���� �ð� ����
				ResetTurnOverTime();

				// ���� ���, ���� ��� HP, AP�� ����� �ش�.
				//target->SetAP(min(INT(target->GetAP() + DEFAULT_AP * power), DEFAULT_MAX_AP));
				//target->SetHP(max(INT(target->GetHP() - (DWORD)(DEFAULT_DAMAGE * power * GetAdVantage(shooter))), 0));
				target->SetAP(min(INT(target->GetAP() + DEFAULT_AP), DEFAULT_MAX_AP));
				target->SetHP(max(INT(target->GetHP() - (DWORD)(DEFAULT_DAMAGE * GetAdVantage(shooter))), 0));

				// ���� ��Ŷ ����
				target->WritePacket(PT_GAME_HIT_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_HIT_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_HIT_INFO_SUCC_U\n"));

				// ��ο��� �˷��ش�.
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

				// HP, AP ��ȭ�� ��ο��� �˷��ش�.
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

				// HP�� 0�̸� Out �����ش�.
				if (target->GetHP() == 0 && target->GetIsInCourt())
				{
					target->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) target));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) target);
				}

				// All Out���� Ȯ���Ѵ�
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

	// ���� ����� �� ������ ���� Ȯ��
	if (shooter == GetBallUser())
	{
		// �� ��밡 ���� ���� �ƴ��� Ȯ��
		if (!IsSameTeam(shooter, target))
		{
			// �� ��밡 ��Ʈ������ ������ Ȯ��
			if (target->GetIsInCourt())
			{
#ifdef _CHECK_MODULE
				// AP�� �Ǵ����� Ȯ���Ѵ�
				if (shooter->GetAP() == DEFAULT_MAX_AP)
				{
#endif
					// �� �������� �Ҵ´�.
					//SetBallUser(NULL);
					// ������ �м��� �����ȴ�.
					//SetLastShootUser(shooter);
					mExplosionReadyFlag = FALSE;

					ResetExplosionTime();

					// AP ��� �Ҹ�
					shooter->SetAP(0);

					// HP, AP ��ȭ�� ��ο��� �˷��ش�.
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

					// ���� �����ϰ� ������鿡�� �˷��ش�.
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

	// ������ ���� Ȯ��
	//if (GetLastShootUser() == shooter)
	//{
	if (GetBallUser())
	{
		// �� ��밡 ���� ���� �ƴ��� Ȯ��
		if (!IsSameTeam(shooter, target) && !IsSameTeam(GetBallUser(), target))
		{
			// Ÿ���� ������ Ȯ��
			if (target->GetIsInCourt())
			{
				// ������ ���͸� Ǯ���ش�.
				//SetLastShootUser(NULL);
				mExplosionReadyFlag = FALSE;

				// �� �ҿ�� ����
				SetBallUser(target);

				// ������ ����Ǵ� ���߽ð� ����
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

				// ���� ��Ŷ ������
				target->WritePacket(PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U\n"));
				// ��ο��� �˷��ش�.

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

	// ���͸� Ȯ���� �ְ�, �´� ����� Ȯ���� �ش�.(Ȥ�ó�) ��Ʈ�ȿ� �ִ��� Ȯ��
	//if (shooter == GetLastShootUser())
	//{
	//if (GetBallUser())
	//{
		// �ٸ� ������ Ȯ���� �ش�.
		if (!IsSameTeam(shooter, target))// && !IsSameTeam(GetBallUser(), target))
		{
			if (target->GetIsInCourt())
			{
				// ���͸� Ǯ���ش�.
				//SetLastShootUser(NULL);
				mTurnOverReadyFlag	= FALSE;

				// �� �������� Ǯ���ش�.
				SetBallUser(NULL);

				// �¾����� ������ �������� ���� ���� �ð� ����
				ResetTurnOverTime();

				// ���� ���, ���� ��� HP, AP�� ����� �ش�.
				target->SetAP(min(INT(target->GetAP() + DEFAULT_SPECIAL_AP), DEFAULT_MAX_AP));
				target->SetHP(max(INT(target->GetHP() - DEFAULT_SPECIAL_DAMAGE), 0));

				// ���� ��Ŷ ����
				target->WritePacket(PT_GAME_SPECIAL_HIT_INFO_SUCC_U, 
					WriteBuffer, 
					WRITE_PT_GAME_SPECIAL_HIT_INFO_SUCC_U(WriteBuffer));

				CLog::WriteLog(_T("# Write packet : PT_GAME_SPECIAL_HIT_INFO_SUCC_U\n"));

				// ��ο��� �˷��ش�.
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

				// HP, AP ��ȭ�� ��ο��� �˷��ش�.
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

				// HP�� 0�̸� Out �����ش�.
				if (target->GetHP() == 0)
				{
					target->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) target));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) target);
				}

				// All Out���� Ȯ���Ѵ�
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

	// ���� �������� �ڱ� �ڽ����� Ȯ��
	//if (GetBallUser() == user)
	//{
		mTurnOverReadyFlag	= FALSE;

		// �� ������ ����
		SetBallUser(NULL);

		ResetTurnOverTime();

		// ���� ������
		user->WritePacket(PT_GAME_FIELDBALL_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_FIELDBALL_SUCC_U\n"));

		// ��ο��� �˷��ش�
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

	// ���� �������� ������ Ȯ��
	if (GetBallUser() == NULL)
	{
		mExplosionReadyFlag = FALSE;

		// �� ������ ����
		SetBallUser(user);

		ResetExplosionTime();

		// ���� ������
		user->WritePacket(PT_GAME_GET_FIELDBALL_SUCC_U, 
			WriteBuffer, 
			WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(WriteBuffer));

		CLog::WriteLog(_T("# Write packet : PT_GAME_GET_FIELDBALL_SUCC_U\n"));

		// ��ο��� �˷��ش�
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

	// ��� LoadComplete���� Ȯ���Ѵ�.
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

	// ��� IntroComplete���� Ȯ���Ѵ�.
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

	// �� ��밡 ��Ʈ������ ������ Ȯ��
	if (actor->GetIsInCourt())
	{
#ifdef _CHECK_MODULE
		// AP�� �Ǵ����� Ȯ���Ѵ�
		if (actor->GetAP() >= DEFAULT_SPEND_AP)
		{
#endif
			switch (skillType)
			{
			// ����Ʈ��
			case 0: 
				break;
			// �����н�
			case 1: 
				break;
			// ���ͼ�Ʈ
			case 2:	
				// ���� �������ڿ� �ٸ� ���� ��츸
				if (!IsSameTeam(actor, GetBallUser()))
				{
					mExplosionReadyFlag = FALSE;
					// �� ������ ����
					SetBallUser(actor);

					ResetExplosionTime();
				}
				else
				{
					WRITE_FAIL_U_PACKET2(actor, PT_GAME_SKILL_INFO_FAIL_U, EC_GAME_BALLUSER_INCORRECT);

					return;
				}
				break;
			// ī���ͽ� ����
			case 3:
				// ������ AP �Ҹ���� �۾��� ����.
				// ������ ����
				break;
			// ī���ͽ�
			case 4:
				// ���� �������ڿ� �ٸ� ���� ��츸
				if (GetBallUser())
				{
					if (!IsSameTeam(actor, GetBallUser()))
					{
						mExplosionReadyFlag = FALSE;
						// �� ������ ����
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

			// HP, AP ��ȭ�� ��ο��� �˷��ش�.
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

			// ���� �����ϰ� ������鿡�� �˷��ش�.
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
			// ���� 3����
			// ��ο��� �˷��ش�.
			WriteAll(PT_GAME_EXPLOSION_READY_M, 
				WriteBuffer, 
				WRITE_PT_GAME_EXPLOSION_READY_M(WriteBuffer, 
				(DWORD_PTR) GetBallUser()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_READY_M 0x%x\n"), 
				(DWORD_PTR) GetBallUser());
		}

		if (!DecreaseExplosionTime())
		{
			// ����
			// ���� ���, ���� ��� HP, AP�� ����� �ش�.
			GetBallUser()->SetHP(max(INT(GetBallUser()->GetHP() - DEFAULT_DAMAGE), 0));

			// ��ο��� �˷��ش�.
			WriteAll(PT_GAME_EXPLOSION_M, 
				WriteBuffer, 
				WRITE_PT_GAME_EXPLOSION_M(WriteBuffer, 
				(DWORD_PTR) GetBallUser()));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_M 0x%x\n"), 
				(DWORD_PTR) GetBallUser());

			// HP, AP ��ȭ�� ��ο��� �˷��ش�.
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
				// HP�� 0�̸� Out �����ش�.
				if (GetBallUser()->GetHP() == 0)
				{
					GetBallUser()->SetIsInCourt(FALSE);

					WriteAll(PT_GAME_OUT_INFO_M, 
						WriteBuffer, 
						WRITE_PT_GAME_OUT_INFO_M(WriteBuffer, 
						(DWORD_PTR) GetBallUser()));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_OUT_INFO_M 0x%x\n"), (DWORD_PTR) GetBallUser());
				}

				// All Out���� Ȯ���Ѵ�
				if (IsAllOut(GetBallUser()))
				{
					GameEnd(gameIocp);

					WriteAll(PT_GAME_END_M, WriteBuffer, WRITE_PT_GAME_END_M(WriteBuffer));

					CLog::WriteLog(_T("# WriteAll packet : PT_GAME_END_M\n"));
				}
			}

			mTurnOverReadyFlag = FALSE;
			// �� �������� Ǯ���ش�.
			SetBallUser(NULL);

			ResetTurnOverTime();

			// �ѹ� ������ ����
			ResetExplosionTime();
		}

		printf("# Explosion : %d %d\n", mExplosionTimeForPersion, mExplosionTimeForTeam);
	}
#ifdef _TURNOVER
	else
	{
		// ���� �ٴڿ� ������ �ִ°��
		if (IsReadyTurnOver())
		{
			// �Ͽ��� 3����
			// ��ο��� �˷��ش�.
			WriteAll(PT_GAME_TURNOVER_READY_M, 
				WriteBuffer, 
				WRITE_PT_GAME_TURNOVER_READY_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_TURNOVER_READY_M\n"));
		}

		if (!DecreaseTurnOverTime())
		{
			// ��ο��� �˷��ش�.
			WriteAll(PT_GAME_TURNOVER_M, 
				WriteBuffer, 
				WRITE_PT_GAME_TURNOVER_M(WriteBuffer));

			CLog::WriteLog(_T("# WriteAll packet : PT_GAME_EXPLOSION_M\n"));

			mTurnOverReadyFlag = FALSE;
			// �ѹ� ������ ����
			ResetTurnOverTime();
		}

		printf("# TurnOver : %d\n", mTurnOverTime);
	}
#endif
}