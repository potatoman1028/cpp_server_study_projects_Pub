// 패킷 처리를 위해서 새로 작성한 ProcProtocol.cpp
#include "stdafx.h"
#include "ServerIocp.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);

// 사용자 등록 프로토콜 처리
VOID CServerIocp::PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_USER);
	// 실제 코드
	//BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };
	//S_PT_REG_USER Data;
	//READ_PT_REG_USER(pPacket, Data);

	// USER 객체를 한개 생성합니다.
	USER *pUser = new USER();

	// 프로토콜에서 데이터를 생성한 개체로 옮깁니다.
	// USER_ID 복사
	_tcscpy(pUser->szUserID, Data.USER_ID);
	// USER_NAME 복사
	_tcscpy(pUser->szUserName, Data.USER_NAME);
	// ADDRESS 복사
	_tcscpy(pUser->szAddress, Data.ADDRESS);
	// SEX 입력
	pUser->cSex = Data.SEX;
	// AGE 입력
	pUser->dwAge = Data.AGE;

	// USER 객체를 관리하는 map m_mUser에 데이터 입력
	m_mUser.insert(std::make_pair(Data.USER_ID, pUser));
}

// 사용자 검색 프로토콜 처리
VOID CServerIocp::PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_USER);

	// USER_ID를 이용해서 데이터를 검색합니다.
	std::map<std::wstring, USER*>::iterator it = m_mUser.find(Data.USER_ID);

	// 검색 결과가 있을경우
	if (it != m_mUser.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT, // 결과 패킷
			WriteBuffer, // 내용을 저장할 버퍼
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer, // PT_WRITE_PACKET.h에 있는 Write용 함수
			it->second->szUserID, // 검색한 USER_ID
			it->second->szUserName, // 검색한 USER_NAME
			it->second->dwAge, // 검색한 AGE
			it->second->cSex, // 검색한 SEX
			it->second->szAddress)); // 검색한 ADDRESS
	}
	// 검색 결과가 없을 경우 빈값을 보내줍니다.
	else
	{
		USER User;
		ZeroMemory(&User, sizeof(USER));

		// 모두 비어있는 데이터를 보내줍니다.
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer, 
			User.szUserID, 
			User.szUserName, 
			User.dwAge, 
			User.cSex, 
			User.szAddress));
	}
}

// 컴퓨터 등록 프로토콜 처리
VOID CServerIocp::PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_COMPUTER);

	// COMPUTER 객체를 한개 생성합니다.
	COMPUTER *pComputer = new COMPUTER();

	// 프로토콜에서 데이터를 생성한 개체로 옮깁니다.
	// COMPUTER_NAME 복사
	_tcscpy(pComputer->szComputerName, Data.COMPUTER_NAME);
	// IP_ADDRESS 복사
	_tcscpy(pComputer->szIPAddress, Data.IP_ADDRESS);
	// CPU_TYPE 입력
	pComputer->cCPUType = Data.CPU_TYPE;
	// RAM 입력
	pComputer->dwRam = Data.RAM;
	// HDD 입력
	pComputer->dwHDD = Data.HDD;

	// COMPUTER 객체를 관리하는 map m_mComputer에 데이터 입력
	m_mComputer.insert(std::make_pair(Data.COMPUTER_NAME, pComputer));
}

// 컴퓨터 검색 프로토콜 처리
VOID CServerIocp::PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_COMPUTER);

	// COMPUTER_NAME을 이용해서 데이터를 검색합니다.
	std::map<std::wstring, COMPUTER*>::iterator it = m_mComputer.find(Data.COMPUTER_NAME);

	// 검색 결과가 있을경우
	if (it != m_mComputer.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT, // 결과 패킷
			WriteBuffer, // 내용을 저장할 버퍼
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer, // PT_WRITE_PACKET.h에 있는 Write용 함수
			it->second->szComputerName, // 검색한 COMPUTER_NAME
			it->second->szIPAddress, // 검색한 IP_ADDRESS
			it->second->cCPUType, // 검색한 CPU_TYPE
			it->second->dwRam, // 검색한 RAM
			it->second->dwHDD)); // 검색한 HDD
	}
	// 검색 결과가 없을 경우 빈값을 보내줍니다.
	else
	{
		COMPUTER Computer;
		ZeroMemory(&Computer, sizeof(COMPUTER));

		// 모두 비어있는 데이터를 보내줍니다.
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer, 
			Computer.szComputerName,
			Computer.szIPAddress, 
			Computer.cCPUType, 
			Computer.dwRam, 
			Computer.dwHDD));
	}
}

// 프로그램 등록 프로토콜 처리
VOID CServerIocp::PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_REG_PROGRAM);

	// PROGRAM 객체를 한개 생성합니다.
	PROGRAM *pProgram = new PROGRAM();

	// 프로토콜에서 데이터를 생성한 개체로 옮깁니다.
	// PROGRAM_NAME 복사
	_tcscpy(pProgram->szProgramName, Data.PROGRAM_NAME);
	// COMMENT 복사
	_tcscpy(pProgram->szComment, Data.COMMENT);
	// COST 입력
	pProgram->dwCost = Data.COST;

	// PROGRAM 객체를 관리하는 map m_mProgram에 데이터 입력
	m_mProgram.insert(std::make_pair(Data.PROGRAM_NAME, pProgram));
}

// 프로그램 검색 프로토콜 처리
VOID CServerIocp::PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// 전처리 함수로 간략화
	READ_PACKET(PT_QUERY_PROGRAM);

	// PROGRAM_NAME을 이용해서 데이터를 검색합니다.
	std::map<std::wstring, PROGRAM*>::iterator it = m_mProgram.find(Data.PROGRAM_NAME);

	// 검색 결과가 있을경우
	if (it != m_mProgram.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, // 결과 패킷
			WriteBuffer, // 내용을 저장할 버퍼
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, // PT_WRITE_PACKET.h에 있는 Write용 함수
			it->second->szProgramName, // 검색한 PROGRAM_NAME
			it->second->dwCost, // 검색한 COST
			it->second->szComment)); // 검색한 COMMENT
	}
	// 검색 결과가 없을 경우 빈값을 보내줍니다.
	else
	{
		PROGRAM Program;
		ZeroMemory(&Program, sizeof(PROGRAM));

		// 모두 비어있는 데이터를 보내줍니다.
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, 
			Program.szProgramName,
			Program.dwCost, 
			Program.szComment));
	}
}