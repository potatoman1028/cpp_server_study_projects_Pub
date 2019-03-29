#pragma once

inline VOID READ_PU_C2S_TUNNELING(BYTE *buffer, S_PU_C2S_TUNNELING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.REMOTE_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REMOTE_PORT);
}

inline VOID READ_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer, S_PU_S2C_TUNNELING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, S_PU_S2C_TUNNELING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PU_C2C_REQ_PING(BYTE *buffer, S_PU_C2C_REQ_PING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.TICK_COUNT);
}

inline VOID READ_PU_C2C_REQ_PING_RESULT(BYTE *buffer, S_PU_C2C_REQ_PING_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.TICK_COUNT);
}

inline VOID READ_PU_C2C_GAME_PC_MOVE_BEGIN(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_BEGIN &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadByte(&parameter.MOVE_TYPE);
	Stream->ReadByte(&parameter.DIRECTION);
	Stream->ReadFloat(&parameter.BEGIN_X);
	Stream->ReadFloat(&parameter.BEGIN_Y);
	Stream->ReadFloat(&parameter.BEGIN_Z);
}

inline VOID READ_PU_C2C_GAME_PC_MOVE_END(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_END &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadByte(&parameter.MOVE_TYPE);
	Stream->ReadByte(&parameter.DIRECTION);
	Stream->ReadFloat(&parameter.END_X);
	Stream->ReadFloat(&parameter.END_Y);
	Stream->ReadFloat(&parameter.END_Z);
}

inline VOID READ_PU_C2C_GAME_PC_MOVE_SYNC(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_SYNC &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadByte(&parameter.MOVE_TYPE);
	Stream->ReadByte(&parameter.DIRECTION);
	Stream->ReadFloat(&parameter.SYNC_X);
	Stream->ReadFloat(&parameter.SYNC_Y);
	Stream->ReadFloat(&parameter.SYNC_Z);
}

inline VOID READ_PU_C2C_GAME_PC_OUT(BYTE *buffer, S_PU_C2C_GAME_PC_OUT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadFloat(&parameter.POS_X);
	Stream->ReadFloat(&parameter.POS_Y);
	Stream->ReadFloat(&parameter.POS_Z);
}

inline VOID READ_PU_C2C_GAME_PC_ACTION(BYTE *buffer, S_PU_C2C_GAME_PC_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.ACTION_TYPE);
	Stream->ReadDWORD(&parameter.ACTION_PARAM);
	Stream->ReadDWORD(&parameter.ACTION_SLOT_POSITION);
	Stream->ReadDWORD(&parameter.TARGET_SLOT_POSITION);
	Stream->ReadFloat(&parameter.ACTION_X);
	Stream->ReadFloat(&parameter.ACTION_Y);
	Stream->ReadFloat(&parameter.ACTION_Z);
	Stream->ReadFloat(&parameter.TARGET_X);
	Stream->ReadFloat(&parameter.TARGET_Y);
	Stream->ReadFloat(&parameter.TARGET_Z);
}

inline VOID READ_PU_C2C_GAME_BALL_MOVE(BYTE *buffer, S_PU_C2C_GAME_BALL_MOVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadFloat(&parameter.BEGIN_X);
	Stream->ReadFloat(&parameter.BEGIN_Y);
	Stream->ReadFloat(&parameter.BEGIN_Z);
	Stream->ReadFloat(&parameter.END_X);
	Stream->ReadFloat(&parameter.END_Y);
	Stream->ReadFloat(&parameter.END_Z);
	Stream->ReadFloat(&parameter.VECTOR_X);
	Stream->ReadFloat(&parameter.VECTOR_Y);
	Stream->ReadFloat(&parameter.VECTOR_Z);
}

inline VOID READ_PU_C2C_GAME_BALL_PROPERTY(BYTE *buffer, S_PU_C2C_GAME_BALL_PROPERTY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadUSHORT(&parameter.PROPERTY);
	Stream->ReadFloat(&parameter.BEGIN_X);
	Stream->ReadFloat(&parameter.BEGIN_Y);
	Stream->ReadFloat(&parameter.BEGIN_Z);
	Stream->ReadFloat(&parameter.END_X);
	Stream->ReadFloat(&parameter.END_Y);
	Stream->ReadFloat(&parameter.END_Z);
	Stream->ReadFloat(&parameter.VECTOR_X);
	Stream->ReadFloat(&parameter.VECTOR_Y);
	Stream->ReadFloat(&parameter.VECTOR_Z);
}

inline VOID READ_PU_C2C_GAME_SPECIAL_SHOOT_READY(BYTE *buffer, S_PU_C2C_GAME_SPECIAL_SHOOT_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadDWORD(&parameter.TARGET_SLOT_POSITION);
	Stream->ReadFloat(&parameter.POSITION_X);
	Stream->ReadFloat(&parameter.POSITION_Y);
	Stream->ReadFloat(&parameter.POSITION_Z);
	Stream->ReadFloat(&parameter.TARGET_POSITION_X);
	Stream->ReadFloat(&parameter.TARGET_POSITION_Y);
	Stream->ReadFloat(&parameter.TARGET_POSITION_Z);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PU_C2C_GAME_SPECIAL_SHOOT_ACTION(BYTE *buffer, S_PU_C2C_GAME_SPECIAL_SHOOT_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadDWORD(&parameter.TARGET_SLOT_POSITION);
	Stream->ReadFloat(&parameter.POSITION_X);
	Stream->ReadFloat(&parameter.POSITION_Y);
	Stream->ReadFloat(&parameter.POSITION_Z);
	Stream->ReadFloat(&parameter.TARGET_POSITION_X);
	Stream->ReadFloat(&parameter.TARGET_POSITION_Y);
	Stream->ReadFloat(&parameter.TARGET_POSITION_Z);
	Stream->ReadUSHORT(&parameter.SHOOT_TYPE);
}

inline VOID READ_PU_C2C_GAME_SETUP_TARGET(BYTE *buffer, S_PU_C2C_GAME_SETUP_TARGET &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadDWORD(&parameter.TARGET_SLOT_POSITION);
}

inline VOID READ_PU_C2C_GAME_SKILL_ACTION(BYTE *buffer, S_PU_C2C_GAME_SKILL_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadDWORD(&parameter.TARGET_SLOT_POSITION);
	Stream->ReadFloat(&parameter.POSITION_X);
	Stream->ReadFloat(&parameter.POSITION_Y);
	Stream->ReadFloat(&parameter.POSITION_Z);
	Stream->ReadUSHORT(&parameter.SKILL_TYPE);
	Stream->ReadDWORD(&parameter.PARAM);
}

inline VOID READ_PU_C2C_GAME_HIT_INFO(BYTE *buffer, S_PU_C2C_GAME_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.SLOT_POSITION);
	Stream->ReadFloat(&parameter.POSITION_X);
	Stream->ReadFloat(&parameter.POSITION_Y);
	Stream->ReadFloat(&parameter.POSITION_Z);
	Stream->ReadFloat(&parameter.HIT_X);
	Stream->ReadFloat(&parameter.HIT_Y);
	Stream->ReadFloat(&parameter.HIT_Z);
	Stream->ReadFloat(&parameter.REFLECT_VELOCITY_X);
	Stream->ReadFloat(&parameter.REFLECT_VELOCITY_Y);
	Stream->ReadFloat(&parameter.REFLECT_VELOCITY_Z);
}

