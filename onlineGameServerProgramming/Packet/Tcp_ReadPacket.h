#pragma once

inline VOID READ_PT_CHANNEL_NICKNAME(BYTE *buffer, S_PT_CHANNEL_NICKNAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.NICK_NAME, 32);
}

inline VOID READ_PT_CHANNEL_NICKNAME_SUCC_U(BYTE *buffer, S_PT_CHANNEL_NICKNAME_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.NICK_NAME, 32);
}

inline VOID READ_PT_CHANNEL_NICKNAME_FAIL_U(BYTE *buffer, S_PT_CHANNEL_NICKNAME_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_REQ_USERINFO(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
}

inline VOID READ_PT_CHANNEL_REQ_USERINFO_SUCC_U(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.NICK_NAME, 32);
	Stream->ReadInt64(&parameter.GAME_MONEY);
	Stream->ReadInt64(&parameter.EXP);
	Stream->ReadInt64(&parameter.CHAR_INDEX_1);
	Stream->ReadInt64(&parameter.CHAR_INDEX_2);
	Stream->ReadInt64(&parameter.CHAR_INDEX_3);
}

inline VOID READ_PT_CHANNEL_REQ_USERINFO_FAIL_U(BYTE *buffer, S_PT_CHANNEL_REQ_USERINFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_REQ_CHARINFO(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.CHAR_SLOT);
}

inline VOID READ_PT_CHANNEL_REQ_CHARINFO_SUCC_U(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadInt64(&parameter.CHAR_INDEX);
	Stream->ReadWCHARs(parameter.NAME, 32);
	Stream->ReadInt32(&parameter.JOB);
	Stream->ReadInt32(&parameter.TYPE);
	Stream->ReadInt64(&parameter.EXP);
	Stream->ReadInt32(&parameter.HEAD_TYPE);
	Stream->ReadInt32(&parameter.CHEST_TYPE);
	Stream->ReadInt32(&parameter.PANTS_TYPE);
	Stream->ReadInt32(&parameter.GLOVES_TYPE);
	Stream->ReadInt32(&parameter.SOCKS_TYPE);
	Stream->ReadInt32(&parameter.SHOES_TYPE);
	Stream->ReadInt32(&parameter.ACCESSORY_TYPE);
	Stream->ReadInt32(&parameter.SKILL_TYPE_1);
	Stream->ReadInt32(&parameter.SKILL_TYPE_2);
	Stream->ReadInt32(&parameter.SKILL_TYPE_3);
	Stream->ReadInt32(&parameter.SPECIAL_SHOOT_TYPE_1);
	Stream->ReadInt32(&parameter.SPECIAL_SHOOT_TYPE_2);
	Stream->ReadInt32(&parameter.SPECIAL_SHOOT_TYPE_3);
}

inline VOID READ_PT_CHANNEL_REQ_CHARINFO_FAIL_U(BYTE *buffer, S_PT_CHANNEL_REQ_CHARINFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_CHECK_CHARACTER_NAME(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.NAME, 32);
}

inline VOID READ_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CHECK_CHARACTER_NAME_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_CREATE_CHARACTER(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.NAME, 32);
	Stream->ReadInt32(&parameter.JOB);
	Stream->ReadInt32(&parameter.TYPE);
	Stream->ReadUSHORT(&parameter.SLOT);
}

inline VOID READ_PT_CHANNEL_CREATE_CHARACTER_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_CHANNEL_CREATE_CHARACTER_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CREATE_CHARACTER_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_EQUIP(BYTE *buffer, S_PT_CHANNEL_EQUIP &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadInt64(&parameter.CHAR_INDEX);
	Stream->ReadUSHORT(&parameter.EQUIP_LOCATION);
	Stream->ReadInt32(&parameter.ITEM_TYPE);
}

inline VOID READ_PT_CHANNEL_EQUIP_SUCC_U(BYTE *buffer, S_PT_CHANNEL_EQUIP_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_CHANNEL_EQUIP_FAIL_U(BYTE *buffer, S_PT_CHANNEL_EQUIP_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_CHARACTER_SELECT(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.SLOT);
}

inline VOID READ_PT_CHANNEL_CHARACTER_SELECT_SUCC_U(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_CHANNEL_CHARACTER_SELECT_FAIL_U(BYTE *buffer, S_PT_CHANNEL_CHARACTER_SELECT_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_CHANNEL_ENTER(BYTE *buffer, S_PT_CHANNEL_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
}

inline VOID READ_PT_CHANNEL_ENTER_SUCC_U(BYTE *buffer, S_PT_CHANNEL_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
	Stream->ReadWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REAL_PORT);
}

inline VOID READ_PT_CHANNEL_ENTER_FAIL_U(BYTE *buffer, S_PT_CHANNEL_ENTER_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_QUICK_JOIN(BYTE *buffer, S_PT_ROOM_QUICK_JOIN &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_QUICK_JOIN_SUCC_U(BYTE *buffer, S_PT_ROOM_QUICK_JOIN_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_QUICK_JOIN_FAIL_U(BYTE *buffer, S_PT_ROOM_QUICK_JOIN_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_ENTER_M(BYTE *buffer, S_PT_ROOM_ENTER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
	Stream->ReadWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REAL_PORT);
}

inline VOID READ_PT_ROOM_REQ_INFO(BYTE *buffer, S_PT_ROOM_REQ_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_REQ_INFO_SUCC_U(BYTE *buffer, S_PT_ROOM_REQ_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ROOM_INDEX);
	Stream->ReadWCHARs(parameter.TITLE, 32);
	Stream->ReadDWORD(&parameter.MAP_INDEX);
	Stream->ReadUSHORT(&parameter.CURRENT_USER_COUNT);
	Stream->ReadDWORD_PTR(&parameter.ROOT_USER_SESSION_ID);
	Stream->ReadBytes((BYTE*) parameter.DATA, sizeof(SLOT_USER_DATA) * 8);
}

inline VOID READ_PT_ROOM_REQ_INFO_FAIL_U(BYTE *buffer, S_PT_ROOM_REQ_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_LEAVE(BYTE *buffer, S_PT_ROOM_LEAVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer, S_PT_ROOM_LEAVE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_LEAVE_FAIL_U(BYTE *buffer, S_PT_ROOM_LEAVE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_LEAVE_M(BYTE *buffer, S_PT_ROOM_LEAVE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.ROOT_USER_SESSION_ID);
}

inline VOID READ_PT_ROOM_MAP_CHANGE(BYTE *buffer, S_PT_ROOM_MAP_CHANGE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.MAP_INDEX);
}

inline VOID READ_PT_ROOM_MAP_CHANGE_SUCC_U(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_MAP_CHANGE_FAIL_U(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_MAP_CHANGE_M(BYTE *buffer, S_PT_ROOM_MAP_CHANGE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.MAP_INDEX);
}

inline VOID READ_PT_ROOM_CHATTING(BYTE *buffer, S_PT_ROOM_CHATTING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.CHAT, 256);
}

inline VOID READ_PT_ROOM_CHATTING_SUCC_U(BYTE *buffer, S_PT_ROOM_CHATTING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_CHATTING_FAIL_U(BYTE *buffer, S_PT_ROOM_CHATTING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_CHATTING_M(BYTE *buffer, S_PT_ROOM_CHATTING_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.CHAT, 256);
}

inline VOID READ_PT_ROOM_READY(BYTE *buffer, S_PT_ROOM_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBOOL(&parameter.READY);
}

inline VOID READ_PT_ROOM_READY_SUCC_U(BYTE *buffer, S_PT_ROOM_READY_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_READY_FAIL_U(BYTE *buffer, S_PT_ROOM_READY_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_READY_M(BYTE *buffer, S_PT_ROOM_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadBOOL(&parameter.READY);
}

inline VOID READ_PT_ROOM_START(BYTE *buffer, S_PT_ROOM_START &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_START_SUCC_U(BYTE *buffer, S_PT_ROOM_START_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_START_FAIL_U(BYTE *buffer, S_PT_ROOM_START_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_ROOM_START_M(BYTE *buffer, S_PT_ROOM_START_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_LOAD_COMPLETE(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_LOAD_COMPLETE_SUCC_U(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_LOAD_COMPLETE_FAIL_U(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_LOAD_COMPLETE_M(BYTE *buffer, S_PT_GAME_LOAD_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
}

inline VOID READ_PT_GAME_ALL_LOAD_COMPLETE_M(BYTE *buffer, S_PT_GAME_ALL_LOAD_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_INTRO_COMPLETE(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_INTRO_COMPLETE_SUCC_U(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_INTRO_COMPLETE_FAIL_U(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_INTRO_COMPLETE_M(BYTE *buffer, S_PT_GAME_INTRO_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
}

inline VOID READ_PT_GAME_ALL_INTRO_COMPLETE_M(BYTE *buffer, S_PT_GAME_ALL_INTRO_COMPLETE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_START_M(BYTE *buffer, S_PT_GAME_START_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_END_M(BYTE *buffer, S_PT_GAME_END_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_PASS_INFO(BYTE *buffer, S_PT_GAME_PASS_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_PASS_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_PASS_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_PASS_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_PASS_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_PASS_INFO_M(BYTE *buffer, S_PT_GAME_PASS_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_SHOOT_INFO(BYTE *buffer, S_PT_GAME_SHOOT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_SHOOT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SHOOT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SHOOT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SHOOT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SHOOT_INFO_M(BYTE *buffer, S_PT_GAME_SHOOT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_HIT_INFO(BYTE *buffer, S_PT_GAME_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_HIT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_HIT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_HIT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_HIT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_HIT_INFO_M(BYTE *buffer, S_PT_GAME_HIT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_SHOOT_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SHOOT_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_SHOOT_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_POWER);
}

inline VOID READ_PT_GAME_PASS_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_PASS_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_PASS_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_PASS_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_PASS_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_FIELDBALL(BYTE *buffer, S_PT_GAME_FIELDBALL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.DROP_SESSION_ID);
}

inline VOID READ_PT_GAME_FIELDBALL_SUCC_U(BYTE *buffer, S_PT_GAME_FIELDBALL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_FIELDBALL_FAIL_U(BYTE *buffer, S_PT_GAME_FIELDBALL_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_FIELDBALL_M(BYTE *buffer, S_PT_GAME_FIELDBALL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.DROP_SESSION_ID);
}

inline VOID READ_PT_GAME_GET_FIELDBALL(BYTE *buffer, S_PT_GAME_GET_FIELDBALL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.GET_SESSION_ID);
	Stream->ReadDWORD(&parameter.PARAMETER);
}

inline VOID READ_PT_GAME_GET_FIELDBALL_SUCC_U(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_GET_FIELDBALL_FAIL_U(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_GET_FIELDBALL_M(BYTE *buffer, S_PT_GAME_GET_FIELDBALL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.GET_SESSION_ID);
	Stream->ReadDWORD(&parameter.PARAMETER);
}

inline VOID READ_PT_GAME_PC_INFO_CHANGED_M(BYTE *buffer, S_PT_GAME_PC_INFO_CHANGED_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadDWORD(&parameter.HP);
	Stream->ReadDWORD(&parameter.AP);
}

inline VOID READ_PT_GAME_OUT_INFO_M(BYTE *buffer, S_PT_GAME_OUT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
}

inline VOID READ_PT_GAME_RESULT_M(BYTE *buffer, S_PT_GAME_RESULT_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBOOL(&parameter.WIN_TEAM);
	Stream->ReadDWORD(&parameter.TEAM0_TOTAL_HP);
	Stream->ReadDWORD(&parameter.TEAM1_TOTAL_HP);
	Stream->ReadDWORD(&parameter.GET_GAME_MONEY);
}

inline VOID READ_PT_GAME_CHANGE_BALLOWNER(BYTE *buffer, S_PT_GAME_CHANGE_BALLOWNER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.NEW_OWNER_SESSION_ID);
}

inline VOID READ_PT_GAME_CHANGE_BALLOWNER_M(BYTE *buffer, S_PT_GAME_CHANGE_BALLOWNER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.NEW_OWNER_SESSION_ID);
}

inline VOID READ_PT_GAME_SPECIAL_SHOOT_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SPECIAL_SHOOT_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_SHOOT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SPECIAL_HIT_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SPECIAL_HIT_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SPECIAL_HIT_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SPECIAL_HIT_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_HIT_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SPECIAL_RECEIVE_INFO(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SPECIAL_RECEIVE_INFO_M(BYTE *buffer, S_PT_GAME_SPECIAL_RECEIVE_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SHOOTER_SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PT_GAME_SKILL_INFO(BYTE *buffer, S_PT_GAME_SKILL_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SKILL_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SKILL_TYPE);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_SKILL_INFO_SUCC_U(BYTE *buffer, S_PT_GAME_SKILL_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_SKILL_INFO_FAIL_U(BYTE *buffer, S_PT_GAME_SKILL_INFO_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PT_GAME_SKILL_INFO_M(BYTE *buffer, S_PT_GAME_SKILL_INFO_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SKILL_SESSION_ID);
	Stream->ReadUSHORT(&parameter.SKILL_TYPE);
	Stream->ReadDWORD_PTR(&parameter.TARGET_SESSION_ID);
}

inline VOID READ_PT_GAME_EXPLOSION_READY_M(BYTE *buffer, S_PT_GAME_EXPLOSION_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.EXPLOSION_SESSION_ID);
}

inline VOID READ_PT_GAME_EXPLOSION_M(BYTE *buffer, S_PT_GAME_EXPLOSION_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.EXPLOSION_SESSION_ID);
}

inline VOID READ_PT_GAME_TURNOVER_READY_M(BYTE *buffer, S_PT_GAME_TURNOVER_READY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_GAME_TURNOVER_M(BYTE *buffer, S_PT_GAME_TURNOVER_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

