#pragma once

#define CHECK_USER_STATUS(ERROR_STATUS, PROTOCOL)\
	if (connectedUser->GetStatus() ERROR_STATUS)\
	{\
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	EC_USER_STATUS_INCORRECT));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, EC_USER_STATUS_INCORRECT);\
	return;\
	}

#define ROOM_OBJCT_FAIL(PROTOCOL)\
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	EC_ROOM_OBJECT_POINTER_FAILED));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, EC_ROOM_OBJECT_POINTER_FAILED);

#define WRITE_FAIL_U_PACKET(PROTOCOL, ERROR_CODE) \
	connectedUser->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, ERROR_CODE);

#define WRITE_FAIL_U_PACKET2(OBJECT, PROTOCOL, ERROR_CODE) \
	OBJECT->WritePacket(PROTOCOL, \
	WriteBuffer, \
	WRITE_##PROTOCOL(WriteBuffer, \
	ERROR_CODE));\
	CLog::WriteLog(_T("# Write packet : %S %d\n"), #PROTOCOL, ERROR_CODE);

#define READ_PACKET(PROTOCOL)\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(packet, Data);

#define GET_EQUIP_FIELDNAME(EquipLocation)\
	TCHAR EquipFieldName[32] = {0,};\
	switch (EquipLocation)\
	{\
	case E_HEAD:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_CHEST:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_PANTS:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_GLOVES:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SOCKS:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SHOES:\
		_tcscpy(EquipFieldName, _T("shoes_type"));\
		break;\
	case E_ACCESSORY:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SKILL1:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SKILL2:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SKILL3:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SSHOOT1:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SSHOOT2:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	case E_SSHOOT3:\
		_tcscpy(EquipFieldName, _T("head_type"));\
		break;\
	}

#define GET_EQUIP_TABLENAME(EquipLocation)\
	TCHAR EquipTableName[32] = {0,};\
	switch (equipLocation)\
	{\
	case E_HEAD:\
		_tcscpy(EquipTableName, _T("BUY_HEAD"));\
		break;\
	case E_CHEST:\
		_tcscpy(EquipTableName, _T("BUY_CHEST"));\
		break;\
	case E_PANTS:\
		_tcscpy(EquipTableName, _T("BUY_PANTS"));\
		break;\
	case E_GLOVES:\
		_tcscpy(EquipTableName, _T("BUY_GLOVES"));\
		break;\
	case E_SOCKS:\
		_tcscpy(EquipTableName, _T("BUY_SOCKS"));\
		break;\
	case E_SHOES:\
		_tcscpy(EquipTableName, _T("BUY_SHOES"));\
		break;\
	case E_ACCESSORY:\
		_tcscpy(EquipTableName, _T("BUY_ACCESSORY"));\
		break;\
	case E_SKILL1:\
	case E_SKILL2:\
	case E_SKILL3:\
		_tcscpy(EquipTableName, _T("BUY_SKILL"));\
		break;\
	case E_SSHOOT1:\
	case E_SSHOOT2:\
	case E_SSHOOT3:\
		_tcscpy(EquipTableName, _T("BUY_SPECIAL_SHOOT"));\
		break;\
	}

static INT		Time		= 0;
static INT		Sequence	= 0;
static INT64	MAKE_SERIAL(INT itemType, INT equipLocation, INT sellType, INT createType)
{
	time_t	Raw;
	time(&Raw);

	INT		CurrentTime = (INT) Raw;
	INT64	Serial		= 0;

	Sequence	= (CurrentTime == Time ? Sequence + 1 : 0);

	Serial		= CurrentTime | (Serial << 32);
	Serial		= itemType | (Serial << 16);
	Serial		= equipLocation | (Serial << 4);
	Serial		= sellType | (Serial << 4);
	Serial		= createType | (Serial << 4);
	Serial		= Sequence | (Serial << 4);

	Time		= CurrentTime;

	return Serial;
}

class CGameIocp : public CIocp
{
public:
	CGameIocp(VOID);
	virtual ~CGameIocp(VOID);

private:
	CNetworkSession			mListenSession;
	CConnectedUserManager	mConnectedUserManager;
	CRoomManager			mRoomManager;

	HANDLE					mKeepThreadHandle;
	HANDLE					mKeepThreadDestroyEvent;

	HANDLE					mGameThreadHandle;
	HANDLE					mGameThreadDestroyEvent;

	//CAdoSessionPool			mDataBase;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	VOID	KeepThreadCallback(VOID);
	VOID	GameThreadCallback(VOID);

protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

private:
	//////////////////////////////////////////////////////////////////////////
	// 프로토콜 처리 함수
	// Lobby
	VOID onPT_CHANNEL_ENTER(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_QUICK_JOIN(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_REQ_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_LEAVE(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_MAP_CHANGE(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_CHATTING(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_READY(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_ROOM_START(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_LOAD_COMPLETE(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_INTRO_COMPLETE(CConnectedUser *connectedUser, BYTE *packet);
//#ifdef _INCLUDE_DATABASE
//	VOID onPT_CHANNEL_NICKNAME(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_REQ_USERINFO(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_REQ_CHARINFO(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_CHECK_CHARACTER_NAME(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_CREATE_CHARACTER(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_EQUIP(CConnectedUser *connectedUser, BYTE *packet);
//	VOID onPT_CHANNEL_CHARACTER_SELECT(CConnectedUser *connectedUser, BYTE *packet);
//#endif

	// Game
	VOID onPT_GAME_PASS_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SHOOT_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_HIT_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SHOOT_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_PASS_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_FIELDBALL(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_GET_FIELDBALL(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_CHANGE_BALLOWNER(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SPECIAL_SHOOT_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SPECIAL_HIT_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SPECIAL_RECEIVE_INFO(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_GAME_SKILL_INFO(CConnectedUser *connectedUser, BYTE *packet);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 데이터베이스 처리 함수
	//BOOL db_IsExistUserID(LPTSTR userID);
	//BOOL db_IsExistNickName(LPTSTR nickName);
	//BOOL db_SetUserID(LPTSTR userID, LPTSTR nickName);
	//BOOL db_GetUserInfo(LPTSTR userID, LPTSTR nickName, INT64 &gameMoney, INT64 &exp, INT64 &charIndex1, INT64 &charIndex2, INT64 &charIndex3);
	//BOOL db_GetCharacterInfo(INT64 charIndex, CCharacter *character);
	//BOOL db_IsExistCharacterName(LPTSTR characterName);
	//BOOL db_NewCharacter(LPTSTR userID, INT64 index, LPTSTR name, INT job, INT type, USHORT slot);
	//BOOL db_IsOwnItem(LPTSTR userID, USHORT equipLocation, INT itemType);
	//BOOL db_Equipment(INT64 charIndex, USHORT equipLocation, INT itemType);
//public:
//	BOOL db_SetGameMoneyAndExp(LPTSTR userID, INT64 charIndex, INT64 gameMoney, INT64 exp);
	//////////////////////////////////////////////////////////////////////////
};
