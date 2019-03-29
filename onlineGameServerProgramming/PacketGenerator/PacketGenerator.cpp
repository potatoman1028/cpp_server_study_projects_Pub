// PacketGenerator.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define MAX_LINE_LENGTH			255
#define MAX_PARAMETER_COUNT		255
#define MAX_PROTOCOL_COUNT		255

typedef struct _PARAMETER
{
	TCHAR			Type[MAX_LINE_LENGTH];
	TCHAR			Name[MAX_LINE_LENGTH];
	DWORD			Length; // _BYTE형에서만 길이가 있다.
} PARAMETER;

typedef struct _PROTOCOL
{
	TCHAR			Name[MAX_LINE_LENGTH];
	DWORD			ParametersCount;
	PARAMETER		Parameters[MAX_PARAMETER_COUNT];
} PROTOCOL;

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 1)
		return 0;

	PROTOCOL	*Protocol						= new PROTOCOL[MAX_PROTOCOL_COUNT];
	DWORD		ProtocolPos						= 0;

	TCHAR		StructureFileName[MAX_PATH]		= {0,};
	TCHAR		ReadPacketFileName[MAX_PATH]	= {0,};
	TCHAR		WritePacketFileName[MAX_PATH]	= {0,};

	_sntprintf(StructureFileName, MAX_PATH, _T("%s_Structure.h"), argv[2]);
	_sntprintf(ReadPacketFileName, MAX_PATH, _T("%s_ReadPacket.h"), argv[2]);
	_sntprintf(WritePacketFileName, MAX_PATH, _T("%s_WritePacket.h"), argv[2]);

	ZeroMemory(Protocol, sizeof(PROTOCOL) * MAX_PROTOCOL_COUNT);

	FILE *SourceFile = _tfopen(argv[1], _T("r"));

	TCHAR	Data[MAX_LINE_LENGTH]	= {0,};
	BOOL	IsParameterOpen			= FALSE;
	DWORD	ParameterPos			= 0;
	while (_fgetts(Data, MAX_LINE_LENGTH, SourceFile))
	{
		// 라인내 공백, 탭등을 없애준다. (앞, 뒤쪽만)
		TCHAR	TempData[MAX_LINE_LENGTH]	= {0,};
		DWORD	TempDataPos					= 0;
		for (DWORD i=0;i<_tcslen(Data);i++)
		{
			if (Data[i] != ' ' && Data[i] != '\t' && Data[i] != '\n' && Data[i] != '\r')
				TempData[TempDataPos++] = Data[i];
		}

		// 이 라인의 데이터가 프로토콜이름인지 파라미터 인지 구분을 해 준다.
		// 파라미터일 경우 형과 변수명으로 이루어지고 프로토콜 이름에 따라간다.
		// 파라미터는 최대 255개 까지 줄 수 있다.

		// 라인 안에 ,가 있으면 프로토콜입니다.
		TCHAR *Pos = _tcsstr(TempData, _T(","));
		if (Pos)
		{
			if (!_tcsstr(TempData, _T("VERSION")))
				_tcsncpy(Protocol[ProtocolPos++].Name, TempData, Pos - TempData);
			else
			{

			}
		}
		else
		{
			// 쓸모없는 데이터이거나
			// 파라미터 정보일 것이다.
			if (_tcsstr(TempData, _T("/*")))
			{
				IsParameterOpen = TRUE;
				ParameterPos	= 0;
				continue;
			}

			if (_tcsstr(TempData, _T("*/")))
			{
				IsParameterOpen							= FALSE;
				Protocol[ProtocolPos-1].ParametersCount = ParameterPos;
				ParameterPos							= 0;
				continue;
			}

			if (_tcslen(TempData) != 0)
			{
				if (IsParameterOpen)
				{
					// 무언가가 있을 경우
					// 파라미터인데, 형, 이름, 사이즈를 분리해 준다.
					// 구분자는 : 와 [] 이다.
					Pos = _tcsstr(TempData, _T(":"));

					if (Pos)
					{
						_tcsncpy(Protocol[ProtocolPos-1].Parameters[ParameterPos++].Type, TempData, Pos - TempData);
					}
					else
						continue;

					TCHAR *StartPos = _tcsstr(TempData, _T("["));
					if (StartPos)
					{
						TCHAR *EndPos = _tcsstr(TempData, _T("]"));
						TCHAR Length[MAX_LINE_LENGTH] = {0,};

						_tcsncpy(Length, StartPos + 1, EndPos - StartPos - 1);
						Protocol[ProtocolPos-1].Parameters[ParameterPos-1].Length = _tstoi(Length);

						_tcsncpy(Protocol[ProtocolPos-1].Parameters[ParameterPos-1].Name, Pos + 1, StartPos - Pos -1);
					}
					else
					{
						Protocol[ProtocolPos-1].Parameters[ParameterPos-1].Length = 0;

						_tcscpy(Protocol[ProtocolPos-1].Parameters[ParameterPos-1].Name, Pos + 1);
					}
				}
			}
		}
	}

	fclose(SourceFile);

	// structure 만들기
	FILE *StructureFile = _tfopen(StructureFileName, _T("w"));

	_ftprintf(StructureFile, _T("#pragma once\n\n"));

	for (DWORD i=0;i<ProtocolPos;i++)
	{
		_ftprintf(StructureFile, _T("typedef struct _S_%s\n{\n"), Protocol[i].Name);
		for (DWORD j=0;j<Protocol[i].ParametersCount;j++)
		{
			if (Protocol[i].Parameters[j].Length == 0)
			{
				_ftprintf(StructureFile, _T("\t%s %s;\n"), Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name);
			}
			else
			{
				_ftprintf(StructureFile, _T("\t%s %s[%d];\n"), Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Length);
			}
		}
		_ftprintf(StructureFile, _T("} S_%s;\n\n"), Protocol[i].Name);
	}

	fclose(StructureFile);

	// Read 함수 만들기
	FILE *ReadPacketFile = _tfopen(ReadPacketFileName, _T("w"));

	_ftprintf(ReadPacketFile, _T("#pragma once\n\n"));

	for (DWORD i=0;i<ProtocolPos;i++)
	{
		_ftprintf(ReadPacketFile, _T("inline VOID READ_%s(BYTE *buffer, S_%s &parameter)\n{\n"), Protocol[i].Name, Protocol[i].Name);
		_ftprintf(ReadPacketFile, _T("\tCStreamSP Stream;\n"));
		_ftprintf(ReadPacketFile, _T("\tStream->SetBuffer(buffer);\n\n"));

		for (DWORD j=0;j<Protocol[i].ParametersCount;j++)
		{
			if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadInt32(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadDWORD(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD_PTR")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadDWORD_PTR(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BYTE")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(ReadPacketFile, _T("\tStream->ReadByte(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
				else
					_ftprintf(ReadPacketFile, _T("\tStream->ReadBytes(parameter.%s, %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("WCHAR")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(ReadPacketFile, _T("\tStream->ReadWCHAR(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
				else
					_ftprintf(ReadPacketFile, _T("\tStream->ReadWCHARs(parameter.%s, %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("FLOAT")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadFloat(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT64")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadInt64(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("USHORT")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadUSHORT(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("SHORT")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadSHORT(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BOOL")))
				_ftprintf(ReadPacketFile, _T("\tStream->ReadBOOL(&parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else
				_ftprintf(ReadPacketFile, _T("\tStream->ReadBytes((BYTE*) parameter.%s, sizeof(%s) * %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Length);
		}
		_ftprintf(ReadPacketFile, _T("}\n\n"));
	}

	fclose(ReadPacketFile);

	// Write 함수 만들기
	FILE *WritePacketFile = _tfopen(WritePacketFileName, _T("w"));

	_ftprintf(WritePacketFile, _T("#pragma once\n\n"));

	for (DWORD i=0;i<ProtocolPos;i++)
	{
		_ftprintf(WritePacketFile, _T("inline DWORD WRITE_%s(BYTE *buffer, S_%s &parameter)\n{\n"), Protocol[i].Name, Protocol[i].Name);
		_ftprintf(WritePacketFile, _T("\tCStreamSP Stream;\n"));
		_ftprintf(WritePacketFile, _T("\tStream->SetBuffer(buffer);\n\n"));

		for (DWORD j=0;j<Protocol[i].ParametersCount;j++)
		{
			if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt32(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD_PTR")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD_PTR(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BYTE")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteByte(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
				else
					_ftprintf(WritePacketFile, _T("\tStream->WriteBytes(parameter.%s, %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("WCHAR")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHAR(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
				else
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHARs(parameter.%s, %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("FLOAT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteFloat(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT64")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt64(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("USHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteUSHORT(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("SHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteSHORT(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BOOL")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteBOOL(parameter.%s);\n"), Protocol[i].Parameters[j].Name);
			else
				_ftprintf(WritePacketFile, _T("\tStream->WriteBytes((BYTE*) parameter.%s, sizeof(%s) * %d);\n"), Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Length);
		}
		_ftprintf(WritePacketFile, _T("\n\treturn Stream->GetLength();\n}\n\n"));
	}

	// 파라미터 타입
	for (DWORD i=0;i<ProtocolPos;i++)
	{
		_ftprintf(WritePacketFile, _T("inline DWORD WRITE_%s(BYTE *buffer"), Protocol[i].Name);

		for (DWORD j=0;j<Protocol[i].ParametersCount;j++)
		{
			if (Protocol[i].Parameters[j].Length > 0)
				_ftprintf(WritePacketFile, _T(", %s *%s"), Protocol[i].Parameters[j].Type, _tcslwr(Protocol[i].Parameters[j].Name));
			else
				_ftprintf(WritePacketFile, _T(", %s %s"), Protocol[i].Parameters[j].Type, _tcslwr(Protocol[i].Parameters[j].Name));
		}

		_ftprintf(WritePacketFile, _T(")\n{\n"));
		_ftprintf(WritePacketFile, _T("\tCStreamSP Stream;\n"));
		_ftprintf(WritePacketFile, _T("\tStream->SetBuffer(buffer);\n\n"));

		for (DWORD j=0;j<Protocol[i].ParametersCount;j++)
		{
			if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt32(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD_PTR")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD_PTR(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BYTE")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteByte(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
				else
					_ftprintf(WritePacketFile, _T("\tStream->WriteBytes(%s, %d);\n"), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("WCHAR")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHAR(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
				else
				{
					_ftprintf(WritePacketFile, _T("\tWCHAR _%s[%d] = {0,};\n"), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
					_ftprintf(WritePacketFile, _T("\t_tcsncpy(_%s, %s, %d);\n"), _tcslwr(Protocol[i].Parameters[j].Name), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHARs(_%s, %d);\n"), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
				}
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("FLOAT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteFloat(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT64")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt64(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("USHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteUSHORT(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("SHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteSHORT(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BOOL")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteBOOL(%s);\n"), _tcslwr(Protocol[i].Parameters[j].Name));
			else
				_ftprintf(WritePacketFile, _T("\tStream->WriteBytes((BYTE*) %s, sizeof(%s) * %d);\n"), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Length);
		}
		_ftprintf(WritePacketFile, _T("\n\treturn Stream->GetLength();\n}\n\n"));
	}

	fclose(WritePacketFile);

	delete[] Protocol;

	return 0;
}

