#pragma once

inline DWORD WRITE_PT_REG_USER(BYTE *buffer, S_PT_REG_USER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.USER_NAME, 32);
	Stream->WriteDWORD(parameter.AGE);
	Stream->WriteByte(parameter.SEX);
	Stream->WriteWCHARs(parameter.ADDRESS, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER(BYTE *buffer, S_PT_QUERY_USER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER_RESULT(BYTE *buffer, S_PT_QUERY_USER_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.USER_NAME, 32);
	Stream->WriteDWORD(parameter.AGE);
	Stream->WriteByte(parameter.SEX);
	Stream->WriteWCHARs(parameter.ADDRESS, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_REG_COMPUTER(BYTE *buffer, S_PT_REG_COMPUTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);
	Stream->WriteWCHARs(parameter.IP_ADDRESS, 32);
	Stream->WriteByte(parameter.CPU_TYPE);
	Stream->WriteDWORD(parameter.RAM);
	Stream->WriteDWORD(parameter.HDD);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER(BYTE *buffer, S_PT_QUERY_COMPUTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER_RESULT(BYTE *buffer, S_PT_QUERY_COMPUTER_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.COMPUTER_NAME, 32);
	Stream->WriteWCHARs(parameter.IP_ADDRESS, 32);
	Stream->WriteByte(parameter.CPU_TYPE);
	Stream->WriteDWORD(parameter.RAM);
	Stream->WriteDWORD(parameter.HDD);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_REG_PROGRAM(BYTE *buffer, S_PT_REG_PROGRAM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);
	Stream->WriteDWORD(parameter.COST);
	Stream->WriteWCHARs(parameter.COMMENT, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM(BYTE *buffer, S_PT_QUERY_PROGRAM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM_RESULT(BYTE *buffer, S_PT_QUERY_PROGRAM_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.PROGRAM_NAME, 32);
	Stream->WriteDWORD(parameter.COST);
	Stream->WriteWCHARs(parameter.COMMENT, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_REG_USER(BYTE *buffer, WCHAR *user_id, WCHAR *user_name, DWORD age, BYTE sex, WCHAR *address)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _user_name[32] = {0,};
	_tcsncpy(_user_name, user_name, 32);
	Stream->WriteWCHARs(_user_name, 32);
	Stream->WriteDWORD(age);
	Stream->WriteByte(sex);
	WCHAR _address[32] = {0,};
	_tcsncpy(_address, address, 32);
	Stream->WriteWCHARs(_address, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER(BYTE *buffer, WCHAR *user_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_USER_RESULT(BYTE *buffer, WCHAR *user_id, WCHAR *user_name, DWORD age, BYTE sex, WCHAR *address)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	_tcsncpy(_user_id, user_id, 32);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _user_name[32] = {0,};
	_tcsncpy(_user_name, user_name, 32);
	Stream->WriteWCHARs(_user_name, 32);
	Stream->WriteDWORD(age);
	Stream->WriteByte(sex);
	WCHAR _address[32] = {0,};
	_tcsncpy(_address, address, 32);
	Stream->WriteWCHARs(_address, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_REG_COMPUTER(BYTE *buffer, WCHAR *computer_name, WCHAR *ip_address, BYTE cpu_type, DWORD ram, DWORD hdd)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	Stream->WriteWCHARs(_computer_name, 32);
	WCHAR _ip_address[32] = {0,};
	_tcsncpy(_ip_address, ip_address, 32);
	Stream->WriteWCHARs(_ip_address, 32);
	Stream->WriteByte(cpu_type);
	Stream->WriteDWORD(ram);
	Stream->WriteDWORD(hdd);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER(BYTE *buffer, WCHAR *computer_name)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	Stream->WriteWCHARs(_computer_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_COMPUTER_RESULT(BYTE *buffer, WCHAR *computer_name, WCHAR *ip_address, BYTE cpu_type, DWORD ram, DWORD hdd)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _computer_name[32] = {0,};
	_tcsncpy(_computer_name, computer_name, 32);
	Stream->WriteWCHARs(_computer_name, 32);
	WCHAR _ip_address[32] = {0,};
	_tcsncpy(_ip_address, ip_address, 32);
	Stream->WriteWCHARs(_ip_address, 32);
	Stream->WriteByte(cpu_type);
	Stream->WriteDWORD(ram);
	Stream->WriteDWORD(hdd);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_REG_PROGRAM(BYTE *buffer, WCHAR *program_name, DWORD cost, WCHAR *comment)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	Stream->WriteWCHARs(_program_name, 32);
	Stream->WriteDWORD(cost);
	WCHAR _comment[32] = {0,};
	_tcsncpy(_comment, comment, 32);
	Stream->WriteWCHARs(_comment, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM(BYTE *buffer, WCHAR *program_name)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	Stream->WriteWCHARs(_program_name, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_QUERY_PROGRAM_RESULT(BYTE *buffer, WCHAR *program_name, DWORD cost, WCHAR *comment)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _program_name[32] = {0,};
	_tcsncpy(_program_name, program_name, 32);
	Stream->WriteWCHARs(_program_name, 32);
	Stream->WriteDWORD(cost);
	WCHAR _comment[32] = {0,};
	_tcsncpy(_comment, comment, 32);
	Stream->WriteWCHARs(_comment, 32);

	return Stream->GetLength();
}

