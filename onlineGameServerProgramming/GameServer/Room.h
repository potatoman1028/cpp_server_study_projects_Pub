#pragma once

class CConnectedUser;
class CGameIocp;

extern DWORD	DEFAULT_GAMETIME;
extern DWORD	DEFAULT_MAX_HP;
extern DWORD	DEFAULT_MAX_AP;
extern DWORD	DEFAULT_SPECIAL_DAMAGE;
extern DWORD	DEFAULT_SPECIAL_AP;
extern DWORD	DEFAULT_DAMAGE;
extern DWORD	DEFAULT_AP;
extern DWORD	DEFAULT_CATCH_AP;
extern DWORD	DEFAULT_SPECIAL_CATCH_AP;
extern DWORD	DEFAULT_SPEND_AP;
extern DWORD	DEFAULT_EXP;
extern DWORD	DEFAULT_GAMEMONEY;

extern DWORD	DEFAULT_EXPLOSION_TIME_FOR_PERSION;
extern DWORD	DEFAULT_EXPLOSION_TIME_FOR_TEAM;
extern DWORD	DEFAULT_EXPLISION_ALRAM_TIME;
extern DWORD	DEFAULT_TURNOVER_TIME;

extern FLOAT	DOGS000_ADVANTAGE;
extern FLOAT	DOGS001_ADVANTAGE;
extern FLOAT	DOGS002_ADVANTAGE;
extern FLOAT	DOGS003_ADVANTAGE;
extern FLOAT	DOGS004_ADVANTAGE;
extern FLOAT	DOGS005_ADVANTAGE;
extern FLOAT	DOGS006_ADVANTAGE;
extern FLOAT	DOGS007_ADVANTAGE;

class CRoom : public CMultiThreadSync<CRoom>
{
public:
	CRoom(VOID);
	virtual ~CRoom(VOID);

private:
	DWORD					mIndex;
	TCHAR					mTitle[32];

	BOOL					mIsRoomStarted;
	BOOL					mIsGameStarted;

	DWORD					mMapIndex;

	CConnectedUser*			mRoomUser[8];
	USHORT					mCurrentUserCount;

	CConnectedUser			*mRootUser;

	DWORD					mRemainGameTime;

	//////////////////////////////////////////////////////////////////////////
	// 게임관련 변수들
	CConnectedUser			*mBallUser;
	//CConnectedUser			*mLastShootUser;
	//CConnectedUser			*mLastPassUser;

	DWORD					mExplosionTimeForPersion;
	DWORD					mExplosionTimeForTeam;
	BOOL					mExplosionReadyFlag;
	DWORD					mTurnOverTime;
	BOOL					mTurnOverReadyFlag;
	//////////////////////////////////////////////////////////////////////////

public:
	BOOL					Begin(DWORD index);
	BOOL					End(VOID);

	BOOL					JoinUser(CConnectedUser *connectedUser, USHORT &slotIndex);
	BOOL					LeaveUser(BOOL isDisconnected, CGameIocp *iocp, CConnectedUser *connectedUser);

	BOOL					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);
	BOOL					WriteAllInitHPAP(VOID);

	BOOL					IsAllLoadComplete(VOID);
	BOOL					IsAllIntroComplete(VOID);

	DWORD					RoomStart(VOID);
	BOOL					GameStart(VOID);
	BOOL					GameEnd(CGameIocp *iocp);

	inline BOOL				DecreaseGameTime(VOID){CThreadSync Sync;mRemainGameTime=max(INT(mRemainGameTime--), 0);return mRemainGameTime ? TRUE : FALSE;}
	inline BOOL				GetIsEmpty(VOID){CThreadSync Sync;return mCurrentUserCount ? FALSE : TRUE;}
	inline BOOL				GetIsFull(VOID){CThreadSync Sync;return mCurrentUserCount == 8 ? TRUE : FALSE;}
	inline BOOL				GetIsRoomStarted(VOID){CThreadSync Sync;return mIsRoomStarted;}
	inline BOOL				GetIsGameStarted(VOID){CThreadSync Synd;return mIsGameStarted;}
	inline CConnectedUser** GetRoomUser(VOID){CThreadSync Sync;return mRoomUser;}
	inline DWORD			GetIndex(VOID){CThreadSync Sync;return mIndex;}
	inline LPTSTR			GetTitle(VOID){CThreadSync Sync;return mTitle;}
	inline DWORD			GetMapIndex(VOID){CThreadSync Sync;return mMapIndex;}
	inline DWORD			SetMapIndex(DWORD mapIndex){CThreadSync Sync;mMapIndex=mapIndex;return TRUE;}
	inline USHORT			GetCurrentUserCount(VOID){CThreadSync Sync;return mCurrentUserCount;}
	inline CConnectedUser*	GetRootUser(VOID){CThreadSync Sync;return mRootUser;}

	DWORD					GetTeam(CConnectedUser *player);

	//////////////////////////////////////////////////////////////////////////
	// 게임관련 함수들
	inline BOOL				SetBallUser(CConnectedUser* connectedUser){CThreadSync Sync;mBallUser = connectedUser;return TRUE;}
	inline CConnectedUser*	GetBallUser(VOID){CThreadSync Sync;return mBallUser;}

	//inline BOOL				SetLastShootUser(CConnectedUser* connectedUser){CThreadSync Sync;mLastShootUser = connectedUser;return TRUE;}
	//inline CConnectedUser*	GetLastShootUser(VOID){CThreadSync Sync;return mLastShootUser;}

	//inline BOOL				SetLastPassUser(CConnectedUser* connectedUser){CThreadSync Sync;mLastPassUser = connectedUser;return TRUE;}
	//inline CConnectedUser*	GetLastPassUser(VOID){CThreadSync Sync;return mLastPassUser;}

	inline BOOL				DecreaseExplosionTime(VOID){CThreadSync Sync;mExplosionTimeForPersion=max(INT(mExplosionTimeForPersion--), 0);mExplosionTimeForTeam=max(INT(mExplosionTimeForTeam--), 0);if (!mExplosionTimeForPersion || !mExplosionTimeForTeam)return FALSE;return TRUE;}
	inline BOOL				IsReadyExplosion(VOID){CThreadSync Sync;if (mExplosionReadyFlag)return FALSE;if (mExplosionTimeForPersion <= DEFAULT_EXPLISION_ALRAM_TIME || mExplosionTimeForTeam <= DEFAULT_EXPLISION_ALRAM_TIME){mExplosionReadyFlag = TRUE;return TRUE;}return FALSE;}
	inline VOID				ResetExplosionTime(VOID){CThreadSync Sync;mExplosionTimeForPersion=DEFAULT_EXPLOSION_TIME_FOR_PERSION;mExplosionTimeForTeam=DEFAULT_EXPLOSION_TIME_FOR_TEAM;}
	inline VOID				ResetExplosionTimeForPersion(VOID){CThreadSync Sync;mExplosionTimeForPersion=DEFAULT_EXPLOSION_TIME_FOR_PERSION;}

	inline BOOL				DecreaseTurnOverTime(VOID){CThreadSync Sync;mTurnOverTime=max(INT(mCurrentUserCount--), 0);if (!mTurnOverTime)return FALSE;return TRUE;}
	inline BOOL				IsReadyTurnOver(VOID){CThreadSync Sync;if (mTurnOverReadyFlag)return FALSE;if (mTurnOverTime <= DEFAULT_EXPLISION_ALRAM_TIME){mTurnOverReadyFlag = TRUE;return TRUE;}return FALSE;}
	inline VOID				ResetTurnOverTime(VOID){CThreadSync Sync;mTurnOverTime = DEFAULT_TURNOVER_TIME;}

	BOOL					IsSameTeam(CConnectedUser *player1, CConnectedUser *player2);
	BOOL					IsAllOut(CConnectedUser *player);

	VOID					PassInfo(CConnectedUser *shooter, CConnectedUser *target);
	VOID					PassReceiveInfo(CConnectedUser *shooter, CConnectedUser *target);

	VOID					ShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power);
	VOID					ShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power);
	VOID					ShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT power, CGameIocp *gameIocp);

	VOID					SpecialShootInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType);
	VOID					SpecialShootReceiveInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType);
	VOID					SpecialShootHitInfo(CConnectedUser *shooter, CConnectedUser *target, USHORT shootType, CGameIocp *gameIocp);

	VOID					SkillInfo(CConnectedUser *actor, CConnectedUser *target, USHORT skillType);

	VOID					GetFieldBall(CConnectedUser *user, DWORD parameter);
	VOID					FieldBall(CConnectedUser *user);

	VOID					CheckExplosion(CGameIocp *gameIocp);

	FLOAT					GetAdVantage(CConnectedUser *user);
	//////////////////////////////////////////////////////////////////////////
};
