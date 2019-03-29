#pragma once

inline VOID READ_PT_REG_USER(BYTE *buffer, S_PT_REG_USER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.USER_NAME, 32);
	Stream->ReadDWORD(&parameter.AGE);
	Stream->ReadByte(&parameter.SEX);
	Stream->ReadWCHARs(parameter.ADDRESS, 32);
}

inline VOID READ_PT_QUERY_USER(BYTE *buffer, S_PT_QUERY_USER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
}

inline VOID READ_PT_QUERY_USER_RESULT(BYTE *buffer, S_PT_QUERY_USER_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.USER_NAME, 32);
	Stream->ReadDWORD(&parameter.AGE);
	Stream->ReadByte(&parameter.SEX);
	Stream->ReadWCHARs(parameter.ADDRESS, 32);
}

inline VOID READ_PT_REG_COMPUTER(BYTE *buffer, S_PT_REG_COMPUTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
	Stream->ReadWCHARs(parameter.IP_ADDRESS, 32);
	Stream->ReadByte(&parameter.CPU_TYPE);
	Stream->ReadDWORD(&parameter.RAM);
	Stream->ReadDWORD(&parameter.HDD);
}

inline VOID READ_PT_QUERY_COMPUTER(BYTE *buffer, S_PT_QUERY_COMPUTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
}

inline VOID READ_PT_QUERY_COMPUTER_RESULT(BYTE *buffer, S_PT_QUERY_COMPUTER_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.COMPUTER_NAME, 32);
	Stream->ReadWCHARs(parameter.IP_ADDRESS, 32);
	Stream->ReadByte(&parameter.CPU_TYPE);
	Stream->ReadDWORD(&parameter.RAM);
	Stream->ReadDWORD(&parameter.HDD);
}

inline VOID READ_PT_REG_PROGRAM(BYTE *buffer, S_PT_REG_PROGRAM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
	Stream->ReadDWORD(&parameter.COST);
	Stream->ReadWCHARs(parameter.COMMENT, 32);
}

inline VOID READ_PT_QUERY_PROGRAM(BYTE *buffer, S_PT_QUERY_PROGRAM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
}

inline VOID READ_PT_QUERY_PROGRAM_RESULT(BYTE *buffer, S_PT_QUERY_PROGRAM_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.PROGRAM_NAME, 32);
	Stream->ReadDWORD(&parameter.COST);
	Stream->ReadWCHARs(parameter.COMMENT, 32);
}

