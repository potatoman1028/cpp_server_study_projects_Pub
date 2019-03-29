#pragma once

inline DWORD WRITE_PT_CHANNEL_NICKNAME(BYTE *buffer, S_PT_CHANNEL_NICKNAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.NICK_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_SUCC_U(BYTE *buffer, S_PT_CHANNEL_NICKNAME_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.NICK_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_FAIL_U(BYTE *buffer, S_PT_CHANNEL_NICKNAME_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO_SUCC_U(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.NICK_NAME, 32);
	Stream->WriteInt64(parameter.GAME_MONEY);
	Stream->WriteInt64(parameter.EXP);
	Stream->WriteInt64(parameter.CHAR_INDEX_1);
	Stream->WriteInt64(parameter.CHAR_INDEX_2);
	Stream->WriteInt64(parameter.CHAR_INDEX_3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO_FAIL_U(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.CHAR_SLOT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO_SUCC_U(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt64(parameter.CHAR_INDEX);
	Stream->WriteWCHARs(parameter.NAME, 32);
	Stream->WriteInt32(parameter.JOB);
	Stream->WriteInt32(parameter.TYPE);
	Stream->WriteInt64(parameter.EXP);
	Stream->WriteInt32(parameter.HEAD_TYPE);
	Stream->WriteInt32(parameter.CHEST_TYPE);
	Stream->WriteInt32(parameter.PANTS_TYPE);
	Stream->WriteInt32(parameter.GLOVES_TYPE);
	Stream->WriteInt32(parameter.SOCKS_TYPE);
	Stream->WriteInt32(parameter.SHOES_TYPE);
	Stream->WriteInt32(parameter.ACCESSORY_TYPE);
	Stream->WriteInt32(parameter.SKILL_TYPE_1);
	Stream->WriteInt32(parameter.SKILL_TYPE_2);
	Stream->WriteInt32(parameter.SKILL_TYPE_3);
	Stream->WriteInt32(parameter.SPECIAL_SHOOT_TYPE_1);
	Stream->WriteInt32(parameter.SPECIAL_SHOOT_TYPE_2);
	Stream->WriteInt32(parameter.SPECIAL_SHOOT_TYPE_3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO_FAIL_U(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.NAME, 32);
	Stream->WriteInt32(parameter.JOB);
	Stream->WriteInt32(parameter.TYPE);
	Stream->WriteUSHORT(parameter.SLOT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP(BYTE *buffer, S_PT_CHANNEL_EQUIP &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt64(parameter.CHAR_INDEX);
	Stream->WriteUSHORT(parameter.EQUIP_LOCATION);
	Stream->WriteInt32(parameter.ITEM_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP_SUCC_U(BYTE *buffer, S_PT_CHANNEL_EQUIP_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP_FAIL_U(BYTE *buffer, S_PT_CHANNEL_EQUIP_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.SLOT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER(BYTE *buffer, S_PT_CHANNEL_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER_SUCC_U(BYTE *buffer, S_PT_CHANNEL_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);
	Stream->WriteWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REAL_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER_FAIL_U(BYTE *buffer, S_PT_CHANNEL_ENTER_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN(BYTE *buffer, S_PT_ROOM_QUICK_JOIN &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN_SUCC_U(BYTE *buffer, S_PT_ROOM_QUICK_JOIN_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN_FAIL_U(BYTE *buffer, S_PT_ROOM_QUICK_JOIN_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER_M(BYTE *buffer, S_PT_ROOM_ENTER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);
	Stream->WriteWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REAL_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO(BYTE *buffer, S_PT_ROOM_REQ_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO_SUCC_U(BYTE *buffer, S_PT_ROOM_REQ_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ROOM_INDEX);
	Stream->WriteWCHARs(parameter.TITLE, 32);
	Stream->WriteDWORD(parameter.MAP_INDEX);
	Stream->WriteUSHORT(parameter.CURRENT_USER_COUNT);
	Stream->WriteDWORD_PTR(parameter.ROOT_USER_SESSION_ID);
	Stream->WriteBytes((BYTE*) parameter.DATA, sizeof(SLOT_USER_DATA) * 8);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO_FAIL_U(BYTE *buffer, S_PT_ROOM_REQ_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE(BYTE *buffer, S_PT_ROOM_LEAVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer, S_PT_ROOM_LEAVE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_FAIL_U(BYTE *buffer, S_PT_ROOM_LEAVE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_M(BYTE *buffer, S_PT_ROOM_LEAVE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.ROOT_USER_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE(BYTE *buffer, S_PT_ROOM_MAP_CHANGE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.MAP_INDEX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_SUCC_U(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_FAIL_U(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_M(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.MAP_INDEX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING(BYTE *buffer, S_PT_ROOM_CHATTING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.CHAT, 256);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_SUCC_U(BYTE *buffer, S_PT_ROOM_CHATTING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_FAIL_U(BYTE *buffer, S_PT_ROOM_CHATTING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_M(BYTE *buffer, S_PT_ROOM_CHATTING_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.CHAT, 256);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY(BYTE *buffer, S_PT_ROOM_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(parameter.READY);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_SUCC_U(BYTE *buffer, S_PT_ROOM_READY_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_FAIL_U(BYTE *buffer, S_PT_ROOM_READY_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_M(BYTE *buffer, S_PT_ROOM_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteBOOL(parameter.READY);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START(BYTE *buffer, S_PT_ROOM_START &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_SUCC_U(BYTE *buffer, S_PT_ROOM_START_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_FAIL_U(BYTE *buffer, S_PT_ROOM_START_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_M(BYTE *buffer, S_PT_ROOM_START_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_SUCC_U(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_FAIL_U(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_M(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_ALL_LOAD_COMPLETE_M(BYTE *buffer, S_PT_GAME_ALL_LOAD_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_SUCC_U(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_FAIL_U(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_M(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_ALL_INTRO_COMPLETE_M(BYTE *buffer, S_PT_GAME_ALL_INTRO_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_START_M(BYTE *buffer, S_PT_GAME_START_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_END_M(BYTE *buffer, S_PT_GAME_END_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO(BYTE *buffer, S_PT_GAME_PASS_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_PASS_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_PASS_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_M(BYTE *buffer, S_PT_GAME_PASS_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO(BYTE *buffer, S_PT_GAME_SHOOT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SHOOT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SHOOT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_M(BYTE *buffer, S_PT_GAME_SHOOT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO(BYTE *buffer, S_PT_GAME_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_HIT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_HIT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_M(BYTE *buffer, S_PT_GAME_HIT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_POWER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL(BYTE *buffer, S_PT_GAME_FIELDBALL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.DROP_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_SUCC_U(BYTE *buffer, S_PT_GAME_FIELDBALL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_FAIL_U(BYTE *buffer, S_PT_GAME_FIELDBALL_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_M(BYTE *buffer, S_PT_GAME_FIELDBALL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.DROP_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL(BYTE *buffer, S_PT_GAME_GET_FIELDBALL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.GET_SESSION_ID);
	Stream->WriteDWORD(parameter.PARAMETER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_FAIL_U(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_M(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.GET_SESSION_ID);
	Stream->WriteDWORD(parameter.PARAMETER);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PC_INFO_CHANGED_M(BYTE *buffer, S_PT_GAME_PC_INFO_CHANGED_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteDWORD(parameter.HP);
	Stream->WriteDWORD(parameter.AP);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_OUT_INFO_M(BYTE *buffer, S_PT_GAME_OUT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_RESULT_M(BYTE *buffer, S_PT_GAME_RESULT_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(parameter.WIN_TEAM);
	Stream->WriteDWORD(parameter.TEAM0_TOTAL_HP);
	Stream->WriteDWORD(parameter.TEAM1_TOTAL_HP);
	Stream->WriteDWORD(parameter.GET_GAME_MONEY);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_CHANGE_BALLOWNER(BYTE *buffer, S_PT_GAME_CHANGE_BALLOWNER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.NEW_OWNER_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_CHANGE_BALLOWNER_M(BYTE *buffer, S_PT_GAME_CHANGE_BALLOWNER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.NEW_OWNER_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SHOOTER_SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO(BYTE *buffer, S_PT_GAME_SKILL_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SKILL_SESSION_ID);
	Stream->WriteUSHORT(parameter.SKILL_TYPE);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SKILL_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SKILL_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_M(BYTE *buffer, S_PT_GAME_SKILL_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SKILL_SESSION_ID);
	Stream->WriteUSHORT(parameter.SKILL_TYPE);
	Stream->WriteDWORD_PTR(parameter.TARGET_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_EXPLOSION_READY_M(BYTE *buffer, S_PT_GAME_EXPLOSION_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.EXPLOSION_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_EXPLOSION_M(BYTE *buffer, S_PT_GAME_EXPLOSION_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.EXPLOSION_SESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_TURNOVER_READY_M(BYTE *buffer, S_PT_GAME_TURNOVER_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_TURNOVER_M(BYTE *buffer, S_PT_GAME_TURNOVER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME(BYTE *buffer, WCHAR *user_id, WCHAR *nick_name)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _nick_name[32] = {0,};
	_tcsncpy(_nick_name, nick_name, 32);
	Stream->WriteWCHARs(_nick_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_SUCC_U(BYTE *buffer, WCHAR *user_id, WCHAR *nick_name)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _nick_name[32] = {0,};
	_tcsncpy(_nick_name, nick_name, 32);
	Stream->WriteWCHARs(_nick_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_NICKNAME_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO(BYTE *buffer, WCHAR *user_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO_SUCC_U(BYTE *buffer, WCHAR *nick_name, INT64 game_money, INT64 exp, INT64 char_index_1, INT64 char_index_2, INT64 char_index_3)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _nick_name[32] = {0,};
	_tcsncpy(_nick_name, nick_name, 32);
	Stream->WriteWCHARs(_nick_name, 32);
	Stream->WriteInt64(game_money);
	Stream->WriteInt64(exp);
	Stream->WriteInt64(char_index_1);
	Stream->WriteInt64(char_index_2);
	Stream->WriteInt64(char_index_3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_USERINFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO(BYTE *buffer, USHORT char_slot)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(char_slot);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO_SUCC_U(BYTE *buffer, INT64 char_index, WCHAR *name, INT job, INT type, INT64 exp, INT head_type, INT chest_type, INT pants_type, INT gloves_type, INT socks_type, INT shoes_type, INT accessory_type, INT skill_type_1, INT skill_type_2, INT skill_type_3, INT special_shoot_type_1, INT special_shoot_type_2, INT special_shoot_type_3)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt64(char_index);
	WCHAR _name[32] = {0,};
	_tcsncpy(_name, name, 32);
	Stream->WriteWCHARs(_name, 32);
	Stream->WriteInt32(job);
	Stream->WriteInt32(type);
	Stream->WriteInt64(exp);
	Stream->WriteInt32(head_type);
	Stream->WriteInt32(chest_type);
	Stream->WriteInt32(pants_type);
	Stream->WriteInt32(gloves_type);
	Stream->WriteInt32(socks_type);
	Stream->WriteInt32(shoes_type);
	Stream->WriteInt32(accessory_type);
	Stream->WriteInt32(skill_type_1);
	Stream->WriteInt32(skill_type_2);
	Stream->WriteInt32(skill_type_3);
	Stream->WriteInt32(special_shoot_type_1);
	Stream->WriteInt32(special_shoot_type_2);
	Stream->WriteInt32(special_shoot_type_3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_REQ_CHARINFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME(BYTE *buffer, WCHAR *name)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _name[32] = {0,};
	_tcsncpy(_name, name, 32);
	Stream->WriteWCHARs(_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER(BYTE *buffer, WCHAR *name, INT job, INT type, USHORT slot)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _name[32] = {0,};
	_tcsncpy(_name, name, 32);
	Stream->WriteWCHARs(_name, 32);
	Stream->WriteInt32(job);
	Stream->WriteInt32(type);
	Stream->WriteUSHORT(slot);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CREATE_CHARACTER_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP(BYTE *buffer, INT64 char_index, USHORT equip_location, INT item_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt64(char_index);
	Stream->WriteUSHORT(equip_location);
	Stream->WriteInt32(item_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_EQUIP_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT(BYTE *buffer, USHORT slot)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(slot);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_CHARACTER_SELECT_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER(BYTE *buffer, WCHAR *user_id, WCHAR *virtual_address, USHORT virtual_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _virtual_address[32] = {0,};
	_tcsncpy(_virtual_address, virtual_address, 32);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER_SUCC_U(BYTE *buffer, DWORD_PTR session_id, WCHAR *virtual_address, USHORT virtual_port, WCHAR *real_address, USHORT real_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	WCHAR _virtual_address[32] = {0,};
	_tcsncpy(_virtual_address, virtual_address, 32);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);
	WCHAR _real_address[32] = {0,};
	_tcsncpy(_real_address, real_address, 32);
	Stream->WriteWCHARs(_real_address, 32);
	Stream->WriteUSHORT(real_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CHANNEL_ENTER_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_QUICK_JOIN_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER_M(BYTE *buffer, DWORD slot_position, WCHAR *user_id, DWORD_PTR session_id, WCHAR *virtual_address, USHORT virtual_port, WCHAR *real_address, USHORT real_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	Stream->WriteDWORD_PTR(session_id);
	WCHAR _virtual_address[32] = {0,};
	_tcsncpy(_virtual_address, virtual_address, 32);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);
	WCHAR _real_address[32] = {0,};
	_tcsncpy(_real_address, real_address, 32);
	Stream->WriteWCHARs(_real_address, 32);
	Stream->WriteUSHORT(real_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO_SUCC_U(BYTE *buffer, DWORD room_index, WCHAR *title, DWORD map_index, USHORT current_user_count, DWORD_PTR root_user_session_id, SLOT_USER_DATA *data)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(room_index);
	WCHAR _title[32] = {0,};
	_tcsncpy(_title, title, 32);
	Stream->WriteWCHARs(_title, 32);
	Stream->WriteDWORD(map_index);
	Stream->WriteUSHORT(current_user_count);
	Stream->WriteDWORD_PTR(root_user_session_id);
	Stream->WriteBytes((BYTE*) data, sizeof(SLOT_USER_DATA) * 8);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_REQ_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_M(BYTE *buffer, DWORD_PTR session_id, DWORD_PTR root_user_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteDWORD_PTR(root_user_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE(BYTE *buffer, DWORD map_index)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(map_index);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_MAP_CHANGE_M(BYTE *buffer, DWORD map_index)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(map_index);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING(BYTE *buffer, WCHAR *chat)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _chat[256] = {0,};
	_tcsncpy(_chat, chat, 256);
	Stream->WriteWCHARs(_chat, 256);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_CHATTING_M(BYTE *buffer, DWORD_PTR session_id, WCHAR *chat)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	WCHAR _chat[256] = {0,};
	_tcsncpy(_chat, chat, 256);
	Stream->WriteWCHARs(_chat, 256);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY(BYTE *buffer, BOOL ready)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(ready);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_READY_M(BYTE *buffer, DWORD_PTR session_id, BOOL ready)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteBOOL(ready);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_START_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_LOAD_COMPLETE_M(BYTE *buffer, DWORD_PTR session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_ALL_LOAD_COMPLETE_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_INTRO_COMPLETE_M(BYTE *buffer, DWORD_PTR session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_ALL_INTRO_COMPLETE_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_START_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_END_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_HIT_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SHOOT_RECEIVE_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_power)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_power);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PASS_RECEIVE_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL(BYTE *buffer, DWORD_PTR drop_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(drop_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_FIELDBALL_M(BYTE *buffer, DWORD_PTR drop_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(drop_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL(BYTE *buffer, DWORD_PTR get_session_id, DWORD parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(get_session_id);
	Stream->WriteDWORD(parameter);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_GET_FIELDBALL_M(BYTE *buffer, DWORD_PTR get_session_id, DWORD parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(get_session_id);
	Stream->WriteDWORD(parameter);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_PC_INFO_CHANGED_M(BYTE *buffer, DWORD_PTR session_id, DWORD hp, DWORD ap)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteDWORD(hp);
	Stream->WriteDWORD(ap);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_OUT_INFO_M(BYTE *buffer, DWORD_PTR session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_RESULT_M(BYTE *buffer, BOOL win_team, DWORD team0_total_hp, DWORD team1_total_hp, DWORD get_game_money)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(win_team);
	Stream->WriteDWORD(team0_total_hp);
	Stream->WriteDWORD(team1_total_hp);
	Stream->WriteDWORD(get_game_money);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_CHANGE_BALLOWNER(BYTE *buffer, DWORD_PTR new_owner_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(new_owner_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_CHANGE_BALLOWNER_M(BYTE *buffer, DWORD_PTR new_owner_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(new_owner_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_SHOOT_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_HIT_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SPECIAL_RECEIVE_INFO_M(BYTE *buffer, DWORD_PTR shooter_session_id, DWORD_PTR target_session_id, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(shooter_session_id);
	Stream->WriteDWORD_PTR(target_session_id);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO(BYTE *buffer, DWORD_PTR skill_session_id, USHORT skill_type, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(skill_session_id);
	Stream->WriteUSHORT(skill_type);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_SKILL_INFO_M(BYTE *buffer, DWORD_PTR skill_session_id, USHORT skill_type, DWORD_PTR target_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(skill_session_id);
	Stream->WriteUSHORT(skill_type);
	Stream->WriteDWORD_PTR(target_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_EXPLOSION_READY_M(BYTE *buffer, DWORD_PTR explosion_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(explosion_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_EXPLOSION_M(BYTE *buffer, DWORD_PTR explosion_session_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(explosion_session_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_TURNOVER_READY_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_GAME_TURNOVER_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

