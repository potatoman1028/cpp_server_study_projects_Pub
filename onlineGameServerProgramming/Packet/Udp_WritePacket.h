#pragma once

inline DWORD WRITE_PU_C2S_TUNNELING(BYTE *buffer, S_PU_C2S_TUNNELING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.REMOTE_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REMOTE_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer, S_PU_S2C_TUNNELING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, S_PU_S2C_TUNNELING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING(BYTE *buffer, S_PU_C2C_REQ_PING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.TICK_COUNT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING_RESULT(BYTE *buffer, S_PU_C2C_REQ_PING_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.TICK_COUNT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_BEGIN(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_BEGIN &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteByte(parameter.MOVE_TYPE);
	Stream->WriteByte(parameter.DIRECTION);
	Stream->WriteFloat(parameter.BEGIN_X);
	Stream->WriteFloat(parameter.BEGIN_Y);
	Stream->WriteFloat(parameter.BEGIN_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_END(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_END &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteByte(parameter.MOVE_TYPE);
	Stream->WriteByte(parameter.DIRECTION);
	Stream->WriteFloat(parameter.END_X);
	Stream->WriteFloat(parameter.END_Y);
	Stream->WriteFloat(parameter.END_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_SYNC(BYTE *buffer, S_PU_C2C_GAME_PC_MOVE_SYNC &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteByte(parameter.MOVE_TYPE);
	Stream->WriteByte(parameter.DIRECTION);
	Stream->WriteFloat(parameter.SYNC_X);
	Stream->WriteFloat(parameter.SYNC_Y);
	Stream->WriteFloat(parameter.SYNC_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_OUT(BYTE *buffer, S_PU_C2C_GAME_PC_OUT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteFloat(parameter.POS_X);
	Stream->WriteFloat(parameter.POS_Y);
	Stream->WriteFloat(parameter.POS_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_ACTION(BYTE *buffer, S_PU_C2C_GAME_PC_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.ACTION_TYPE);
	Stream->WriteDWORD(parameter.ACTION_PARAM);
	Stream->WriteDWORD(parameter.ACTION_SLOT_POSITION);
	Stream->WriteDWORD(parameter.TARGET_SLOT_POSITION);
	Stream->WriteFloat(parameter.ACTION_X);
	Stream->WriteFloat(parameter.ACTION_Y);
	Stream->WriteFloat(parameter.ACTION_Z);
	Stream->WriteFloat(parameter.TARGET_X);
	Stream->WriteFloat(parameter.TARGET_Y);
	Stream->WriteFloat(parameter.TARGET_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_BALL_MOVE(BYTE *buffer, S_PU_C2C_GAME_BALL_MOVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteFloat(parameter.BEGIN_X);
	Stream->WriteFloat(parameter.BEGIN_Y);
	Stream->WriteFloat(parameter.BEGIN_Z);
	Stream->WriteFloat(parameter.END_X);
	Stream->WriteFloat(parameter.END_Y);
	Stream->WriteFloat(parameter.END_Z);
	Stream->WriteFloat(parameter.VECTOR_X);
	Stream->WriteFloat(parameter.VECTOR_Y);
	Stream->WriteFloat(parameter.VECTOR_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_BALL_PROPERTY(BYTE *buffer, S_PU_C2C_GAME_BALL_PROPERTY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteUSHORT(parameter.PROPERTY);
	Stream->WriteFloat(parameter.BEGIN_X);
	Stream->WriteFloat(parameter.BEGIN_Y);
	Stream->WriteFloat(parameter.BEGIN_Z);
	Stream->WriteFloat(parameter.END_X);
	Stream->WriteFloat(parameter.END_Y);
	Stream->WriteFloat(parameter.END_Z);
	Stream->WriteFloat(parameter.VECTOR_X);
	Stream->WriteFloat(parameter.VECTOR_Y);
	Stream->WriteFloat(parameter.VECTOR_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SPECIAL_SHOOT_READY(BYTE *buffer, S_PU_C2C_GAME_SPECIAL_SHOOT_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteDWORD(parameter.TARGET_SLOT_POSITION);
	Stream->WriteFloat(parameter.POSITION_X);
	Stream->WriteFloat(parameter.POSITION_Y);
	Stream->WriteFloat(parameter.POSITION_Z);
	Stream->WriteFloat(parameter.TARGET_POSITION_X);
	Stream->WriteFloat(parameter.TARGET_POSITION_Y);
	Stream->WriteFloat(parameter.TARGET_POSITION_Z);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SPECIAL_SHOOT_ACTION(BYTE *buffer, S_PU_C2C_GAME_SPECIAL_SHOOT_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteDWORD(parameter.TARGET_SLOT_POSITION);
	Stream->WriteFloat(parameter.POSITION_X);
	Stream->WriteFloat(parameter.POSITION_Y);
	Stream->WriteFloat(parameter.POSITION_Z);
	Stream->WriteFloat(parameter.TARGET_POSITION_X);
	Stream->WriteFloat(parameter.TARGET_POSITION_Y);
	Stream->WriteFloat(parameter.TARGET_POSITION_Z);
	Stream->WriteUSHORT(parameter.SHOOT_TYPE);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SETUP_TARGET(BYTE *buffer, S_PU_C2C_GAME_SETUP_TARGET &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteDWORD(parameter.TARGET_SLOT_POSITION);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SKILL_ACTION(BYTE *buffer, S_PU_C2C_GAME_SKILL_ACTION &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteDWORD(parameter.TARGET_SLOT_POSITION);
	Stream->WriteFloat(parameter.POSITION_X);
	Stream->WriteFloat(parameter.POSITION_Y);
	Stream->WriteFloat(parameter.POSITION_Z);
	Stream->WriteUSHORT(parameter.SKILL_TYPE);
	Stream->WriteDWORD(parameter.PARAM);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_HIT_INFO(BYTE *buffer, S_PU_C2C_GAME_HIT_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.SLOT_POSITION);
	Stream->WriteFloat(parameter.POSITION_X);
	Stream->WriteFloat(parameter.POSITION_Y);
	Stream->WriteFloat(parameter.POSITION_Z);
	Stream->WriteFloat(parameter.HIT_X);
	Stream->WriteFloat(parameter.HIT_Y);
	Stream->WriteFloat(parameter.HIT_Z);
	Stream->WriteFloat(parameter.REFLECT_VELOCITY_X);
	Stream->WriteFloat(parameter.REFLECT_VELOCITY_Y);
	Stream->WriteFloat(parameter.REFLECT_VELOCITY_Z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2S_TUNNELING(BYTE *buffer, DWORD_PTR session_id, WCHAR *remote_address, USHORT remote_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteWCHARs(remote_address, 32);
	Stream->WriteUSHORT(remote_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING(BYTE *buffer, DWORD tick_count)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(tick_count);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING_RESULT(BYTE *buffer, DWORD tick_count)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(tick_count);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_BEGIN(BYTE *buffer, DWORD slot_position, BYTE move_type, BYTE direction, FLOAT begin_x, FLOAT begin_y, FLOAT begin_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteByte(move_type);
	Stream->WriteByte(direction);
	Stream->WriteFloat(begin_x);
	Stream->WriteFloat(begin_y);
	Stream->WriteFloat(begin_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_END(BYTE *buffer, DWORD slot_position, BYTE move_type, BYTE direction, FLOAT end_x, FLOAT end_y, FLOAT end_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteByte(move_type);
	Stream->WriteByte(direction);
	Stream->WriteFloat(end_x);
	Stream->WriteFloat(end_y);
	Stream->WriteFloat(end_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_MOVE_SYNC(BYTE *buffer, DWORD slot_position, BYTE move_type, BYTE direction, FLOAT sync_x, FLOAT sync_y, FLOAT sync_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteByte(move_type);
	Stream->WriteByte(direction);
	Stream->WriteFloat(sync_x);
	Stream->WriteFloat(sync_y);
	Stream->WriteFloat(sync_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_OUT(BYTE *buffer, DWORD slot_position, FLOAT pos_x, FLOAT pos_y, FLOAT pos_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteFloat(pos_x);
	Stream->WriteFloat(pos_y);
	Stream->WriteFloat(pos_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_PC_ACTION(BYTE *buffer, USHORT action_type, DWORD action_param, DWORD action_slot_position, DWORD target_slot_position, FLOAT action_x, FLOAT action_y, FLOAT action_z, FLOAT target_x, FLOAT target_y, FLOAT target_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(action_type);
	Stream->WriteDWORD(action_param);
	Stream->WriteDWORD(action_slot_position);
	Stream->WriteDWORD(target_slot_position);
	Stream->WriteFloat(action_x);
	Stream->WriteFloat(action_y);
	Stream->WriteFloat(action_z);
	Stream->WriteFloat(target_x);
	Stream->WriteFloat(target_y);
	Stream->WriteFloat(target_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_BALL_MOVE(BYTE *buffer, DWORD slot_position, FLOAT begin_x, FLOAT begin_y, FLOAT begin_z, FLOAT end_x, FLOAT end_y, FLOAT end_z, FLOAT vector_x, FLOAT vector_y, FLOAT vector_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteFloat(begin_x);
	Stream->WriteFloat(begin_y);
	Stream->WriteFloat(begin_z);
	Stream->WriteFloat(end_x);
	Stream->WriteFloat(end_y);
	Stream->WriteFloat(end_z);
	Stream->WriteFloat(vector_x);
	Stream->WriteFloat(vector_y);
	Stream->WriteFloat(vector_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_BALL_PROPERTY(BYTE *buffer, DWORD slot_position, USHORT property, FLOAT begin_x, FLOAT begin_y, FLOAT begin_z, FLOAT end_x, FLOAT end_y, FLOAT end_z, FLOAT vector_x, FLOAT vector_y, FLOAT vector_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteUSHORT(property);
	Stream->WriteFloat(begin_x);
	Stream->WriteFloat(begin_y);
	Stream->WriteFloat(begin_z);
	Stream->WriteFloat(end_x);
	Stream->WriteFloat(end_y);
	Stream->WriteFloat(end_z);
	Stream->WriteFloat(vector_x);
	Stream->WriteFloat(vector_y);
	Stream->WriteFloat(vector_z);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SPECIAL_SHOOT_READY(BYTE *buffer, DWORD slot_position, DWORD target_slot_position, FLOAT position_x, FLOAT position_y, FLOAT position_z, FLOAT target_position_x, FLOAT target_position_y, FLOAT target_position_z, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteDWORD(target_slot_position);
	Stream->WriteFloat(position_x);
	Stream->WriteFloat(position_y);
	Stream->WriteFloat(position_z);
	Stream->WriteFloat(target_position_x);
	Stream->WriteFloat(target_position_y);
	Stream->WriteFloat(target_position_z);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SPECIAL_SHOOT_ACTION(BYTE *buffer, DWORD slot_position, DWORD target_slot_position, FLOAT position_x, FLOAT position_y, FLOAT position_z, FLOAT target_position_x, FLOAT target_position_y, FLOAT target_position_z, USHORT shoot_type)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteDWORD(target_slot_position);
	Stream->WriteFloat(position_x);
	Stream->WriteFloat(position_y);
	Stream->WriteFloat(position_z);
	Stream->WriteFloat(target_position_x);
	Stream->WriteFloat(target_position_y);
	Stream->WriteFloat(target_position_z);
	Stream->WriteUSHORT(shoot_type);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SETUP_TARGET(BYTE *buffer, DWORD slot_position, DWORD target_slot_position)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteDWORD(target_slot_position);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_SKILL_ACTION(BYTE *buffer, DWORD slot_position, DWORD target_slot_position, FLOAT position_x, FLOAT position_y, FLOAT position_z, USHORT skill_type, DWORD param)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteDWORD(target_slot_position);
	Stream->WriteFloat(position_x);
	Stream->WriteFloat(position_y);
	Stream->WriteFloat(position_z);
	Stream->WriteUSHORT(skill_type);
	Stream->WriteDWORD(param);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_GAME_HIT_INFO(BYTE *buffer, DWORD slot_position, FLOAT position_x, FLOAT position_y, FLOAT position_z, FLOAT hit_x, FLOAT hit_y, FLOAT hit_z, FLOAT reflect_velocity_x, FLOAT reflect_velocity_y, FLOAT reflect_velocity_z)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(slot_position);
	Stream->WriteFloat(position_x);
	Stream->WriteFloat(position_y);
	Stream->WriteFloat(position_z);
	Stream->WriteFloat(hit_x);
	Stream->WriteFloat(hit_y);
	Stream->WriteFloat(hit_z);
	Stream->WriteFloat(reflect_velocity_x);
	Stream->WriteFloat(reflect_velocity_y);
	Stream->WriteFloat(reflect_velocity_z);

	return Stream->GetLength();
}

