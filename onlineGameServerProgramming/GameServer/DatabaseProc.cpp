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

// TODO : 나중에 쿼리들을 SP 형태로 변경해야 한다.
// TODO : SELECT할때 RecordCount == 0이면 FALSE해야 한다.
//BOOL CGameIocp::db_IsExistUserID(LPTSTR userID)
//{
//	if (!userID)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query,
//			MAX_BUFFER_LENGTH,
//			_T("SELECT user_id FROM [USER] WHERE user_id='%s'"),
//			userID);
//
//		_RecordsetPtr	RecordSet = AdoSession->Execute(Query);
//		DWORD			RecordCount = 0;
//
//		while (!RecordSet->GetEndOfFile()) {RecordCount++;RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//
//		if (!RecordCount)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_IsExistNickName(LPTSTR nickName)
//{
//	if (!nickName)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query,
//			MAX_BUFFER_LENGTH,
//			_T("SELECT nick_name FROM [USER] WHERE nick_name='%s'"),
//			nickName);
//
//		_RecordsetPtr	RecordSet = AdoSession->Execute(Query);
//		DWORD			RecordCount = 0;
//
//		while (!RecordSet->GetEndOfFile()) {RecordCount++;RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//
//		if (!RecordCount)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_SetUserID(LPTSTR userID, LPTSTR nickName)
//{
//	if (!userID || !nickName)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query, 
//			MAX_BUFFER_LENGTH,
//			_T("INSERT INTO [USER] (user_id, nick_name) VALUES ('%s', '%s')"),
//			userID,
//			nickName);
//
//		_RecordsetPtr RecordSet = AdoSession->Execute(Query);
//
//		//while (!RecordSet->GetEndOfFile()) {RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_GetUserInfo(LPTSTR userID, LPTSTR nickName, INT64 &gameMoney, INT64 &exp, INT64 &charIndex1, INT64 &charIndex2, INT64 &charIndex3)
//{
//	if (!userID || !nickName)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		BOOL	IsExist						= FALSE;
//		TCHAR	Query[MAX_BUFFER_LENGTH]	= {0,};
//		_sntprintf(Query, 
//			MAX_BUFFER_LENGTH, 
//			_T("SELECT nick_name, game_money, exp, char_index_1, char_index_2, char_index_3 FROM [USER] WHERE user_id='%s'"), 
//			userID);
//	
//		_RecordsetPtr RecordSet = AdoSession->Execute(Query);
//
//		while (!RecordSet->GetEndOfFile())
//		{
//			_tcscpy(nickName, (LPTSTR) RecordSet->GetCollect(_T("nick_name")).bstrVal);
//			gameMoney	= RecordSet->GetCollect(_T("game_money")).llVal;
//			exp			= RecordSet->GetCollect(_T("exp")).llVal;
//			charIndex1	= RecordSet->GetCollect(_T("char_index_1")).llVal;
//			charIndex2	= RecordSet->GetCollect(_T("char_index_2")).llVal;
//			charIndex3	= RecordSet->GetCollect(_T("char_index_3")).llVal;
//
//			RecordSet->MoveNext();
//			
//			IsExist = TRUE;
//		}
//
//		AdoSession->ExecuteFinish();
//
//		if (!IsExist)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_SetGameMoneyAndExp(LPTSTR userID, INT64 charIndex, INT64 gameMoney, INT64 exp)
//{
//	if (gameMoney < 0 || exp < 0)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query, 
//			MAX_BUFFER_LENGTH,
//			_T("UPDATE [USER] SET game_money=%I64d, exp=%I64d WHERE user_id='%s';\
//			   UPDATE BUY_CHAR SET exp=%I64d WHERE char_index=%I64d;"),
//			gameMoney,
//			exp,
//			userID,
//			exp,
//			charIndex);
//
//		_RecordsetPtr RecordSet = AdoSession->Execute(Query);
//
//		//while (!RecordSet->GetEndOfFile()) {RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_GetCharacterInfo(INT64 charIndex, CCharacter *character)
//{
//	if (charIndex == 0)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		BOOL	IsExist						= FALSE;
//		TCHAR	Query[MAX_BUFFER_LENGTH]	= {0,};
//		_sntprintf(Query, 
//			MAX_BUFFER_LENGTH, 
//			_T("SELECT [index],\
//			   name,\
//			   job,\
//			   type,\
//			   exp,\
//			   head_type,\
//			   chest_type,\
//			   pants_type,\
//			   gloves_type,\
//			   socks_type,\
//			   shoes_type,\
//			   accessory_type,\
//			   skill_type_1,\
//			   skill_type_2,\
//			   skill_type_3,\
//			   special_shoot_type_1,\
//			   special_shoot_type_2,\
//			   special_shoot_type_3\
//			   FROM BUY_CHAR WHERE [index]=%I64d"), 
//			charIndex);
//
//		_RecordsetPtr RecordSet = AdoSession->Execute(Query);
//
//		while (!RecordSet->GetEndOfFile())
//		{
//			character->SetIndex(RecordSet->GetCollect(_T("index")).llVal);
//			character->SetName((LPTSTR) RecordSet->GetCollect(_T("name")).bstrVal);
//			character->SetJob(RecordSet->GetCollect(_T("job")).intVal);
//			character->SetType(RecordSet->GetCollect(_T("type")).intVal);
//			character->SetExp(RecordSet->GetCollect(_T("exp")).llVal);
//			character->SetEquip(E_HEAD, RecordSet->GetCollect(_T("head_type")).intVal);
//			character->SetEquip(E_CHEST, RecordSet->GetCollect(_T("chest_type")).intVal);
//			character->SetEquip(E_PANTS, RecordSet->GetCollect(_T("pants_type")).intVal);
//			character->SetEquip(E_GLOVES, RecordSet->GetCollect(_T("gloves_type")).intVal);
//			character->SetEquip(E_SOCKS, RecordSet->GetCollect(_T("socks_type")).intVal);
//			character->SetEquip(E_SHOES, RecordSet->GetCollect(_T("shoes_type")).intVal);
//			character->SetEquip(E_ACCESSORY, RecordSet->GetCollect(_T("accessory_type")).intVal);
//			character->SetEquip(E_SKILL1, RecordSet->GetCollect(_T("skill_type_1")).intVal);
//			character->SetEquip(E_SKILL2, RecordSet->GetCollect(_T("skill_type_2")).intVal);
//			character->SetEquip(E_SKILL3, RecordSet->GetCollect(_T("skill_type_3")).intVal);
//			character->SetEquip(E_SSHOOT1, RecordSet->GetCollect(_T("special_shoot_type_1")).intVal);
//			character->SetEquip(E_SSHOOT2, RecordSet->GetCollect(_T("special_shoot_type_2")).intVal);
//			character->SetEquip(E_SSHOOT3, RecordSet->GetCollect(_T("special_shoot_type_3")).intVal);
//	
//			RecordSet->MoveNext();
//
//			IsExist = TRUE;
//		}
//
//		AdoSession->ExecuteFinish();
//
//		if (!IsExist)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_IsExistCharacterName(LPTSTR characterName)
//{
//	if (!characterName)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query,
//			MAX_BUFFER_LENGTH,
//			_T("SELECT name FROM BUY_CHAR WHERE name='%s'"),
//			characterName);
//
//		_RecordsetPtr	RecordSet = AdoSession->Execute(Query);
//		DWORD			RecordCount = 0;
//
//		while (!RecordSet->GetEndOfFile()) {RecordCount++;RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//
//		if (!RecordCount)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_NewCharacter(LPTSTR userID, INT64 index, LPTSTR name, INT job, INT type, USHORT slot)
//{
//	if (!userID || index == 0)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		TCHAR	FieldName[32]				= {0,};
//
//		switch (slot)
//		{
//		case 0:
//			_tcsncpy(FieldName, _T("char_index_1"), 32);
//			break;
//		case 1:
//			_tcsncpy(FieldName, _T("char_index_2"), 32);
//			break;
//		case 2:
//			_tcsncpy(FieldName, _T("char_index_3"), 32);
//			break;
//		}
//
//		TCHAR	Query[MAX_BUFFER_LENGTH]	= {0,};
//		_sntprintf(Query, 
//			MAX_BUFFER_LENGTH,
//			_T("INSERT INTO BUY_CHAR (user_id, [index], name, job, type) VALUES ('%s', %I64d, '%s', %d, %d);\
//			   UPDATE [USER] SET %s=%I64d WHERE user_id='%s';"),
//			userID,
//			index,
//			name,
//			job,
//			type,
//			FieldName,
//			index,
//			userID);
//
//		_RecordsetPtr	RecordSet = AdoSession->Execute(Query);
//
//		//while (!RecordSet->GetEndOfFile()) {RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_IsOwnItem(LPTSTR userID, USHORT equipLocation, INT itemType)
//{
//	if (!userID || itemType == 0)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		GET_EQUIP_TABLENAME(equipLocation);
//
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query,
//			MAX_BUFFER_LENGTH,
//			_T("SELECT [index] FROM %s WHERE user_id='%s' AND type=%d"),
//			EquipTableName,
//			userID,
//			itemType);
//
//		_RecordsetPtr	RecordSet = AdoSession->Execute(Query);
//		DWORD			RecordCount = 0;
//
//		while (!RecordSet->GetEndOfFile()) {RecordCount++;RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//
//		if (!RecordCount)
//			return FALSE;
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}
//
//BOOL CGameIocp::db_Equipment(INT64 charIndex, USHORT equipLocation, INT itemType)
//{
//	if (charIndex == 0)
//		return FALSE;
//
//	CAdoSession *AdoSession = mDataBase.GetFreeSession();
//
//	if (AdoSession)
//	{
//		GET_EQUIP_FIELDNAME(equipLocation);
//
//		TCHAR Query[MAX_BUFFER_LENGTH] = {0,};
//		_sntprintf(Query,
//			MAX_BUFFER_LENGTH,
//			_T("UPDATE BUY_CHAR SET %s=%d WHERE [index] = %d"), 
//			EquipFieldName, 
//			itemType, 
//			charIndex);
//
//		_RecordsetPtr RecordSet = AdoSession->Execute(Query);
//
//		//while (!RecordSet->GetEndOfFile()) {RecordSet->MoveNext();}
//
//		AdoSession->ExecuteFinish();
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//}