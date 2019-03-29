// ��Ŷ ó���� ���ؼ� ���� �ۼ��� ProcProtocol.cpp
#include "stdafx.h"
#include "ServerIocp.h"

#define READ_PACKET(PROTOCOL)\
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };\
	S_##PROTOCOL Data;\
	READ_##PROTOCOL(pPacket, Data);

// ����� ��� �������� ó��
VOID CServerIocp::PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_USER);
	// ���� �ڵ�
	//BYTE WriteBuffer[MAX_BUFFER_LENGTH] = {0, };
	//S_PT_REG_USER Data;
	//READ_PT_REG_USER(pPacket, Data);

	// USER ��ü�� �Ѱ� �����մϴ�.
	USER *pUser = new USER();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�ϴ�.
	// USER_ID ����
	_tcscpy(pUser->szUserID, Data.USER_ID);
	// USER_NAME ����
	_tcscpy(pUser->szUserName, Data.USER_NAME);
	// ADDRESS ����
	_tcscpy(pUser->szAddress, Data.ADDRESS);
	// SEX �Է�
	pUser->cSex = Data.SEX;
	// AGE �Է�
	pUser->dwAge = Data.AGE;

	// USER ��ü�� �����ϴ� map m_mUser�� ������ �Է�
	m_mUser.insert(std::make_pair(Data.USER_ID, pUser));
}

// ����� �˻� �������� ó��
VOID CServerIocp::PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_USER);

	// USER_ID�� �̿��ؼ� �����͸� �˻��մϴ�.
	std::map<std::wstring, USER*>::iterator it = m_mUser.find(Data.USER_ID);

	// �˻� ����� �������
	if (it != m_mUser.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_USER_RESULT, // ��� ��Ŷ
			WriteBuffer, // ������ ������ ����
			WRITE_PT_QUERY_USER_RESULT(WriteBuffer, // PT_WRITE_PACKET.h�� �ִ� Write�� �Լ�
			it->second->szUserID, // �˻��� USER_ID
			it->second->szUserName, // �˻��� USER_NAME
			it->second->dwAge, // �˻��� AGE
			it->second->cSex, // �˻��� SEX
			it->second->szAddress)); // �˻��� ADDRESS
	}
	// �˻� ����� ���� ��� ���� �����ݴϴ�.
	else
	{
		USER User;
		ZeroMemory(&User, sizeof(USER));

		// ��� ����ִ� �����͸� �����ݴϴ�.
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

// ��ǻ�� ��� �������� ó��
VOID CServerIocp::PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_COMPUTER);

	// COMPUTER ��ü�� �Ѱ� �����մϴ�.
	COMPUTER *pComputer = new COMPUTER();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�ϴ�.
	// COMPUTER_NAME ����
	_tcscpy(pComputer->szComputerName, Data.COMPUTER_NAME);
	// IP_ADDRESS ����
	_tcscpy(pComputer->szIPAddress, Data.IP_ADDRESS);
	// CPU_TYPE �Է�
	pComputer->cCPUType = Data.CPU_TYPE;
	// RAM �Է�
	pComputer->dwRam = Data.RAM;
	// HDD �Է�
	pComputer->dwHDD = Data.HDD;

	// COMPUTER ��ü�� �����ϴ� map m_mComputer�� ������ �Է�
	m_mComputer.insert(std::make_pair(Data.COMPUTER_NAME, pComputer));
}

// ��ǻ�� �˻� �������� ó��
VOID CServerIocp::PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_COMPUTER);

	// COMPUTER_NAME�� �̿��ؼ� �����͸� �˻��մϴ�.
	std::map<std::wstring, COMPUTER*>::iterator it = m_mComputer.find(Data.COMPUTER_NAME);

	// �˻� ����� �������
	if (it != m_mComputer.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_COMPUTER_RESULT, // ��� ��Ŷ
			WriteBuffer, // ������ ������ ����
			WRITE_PT_QUERY_COMPUTER_RESULT(WriteBuffer, // PT_WRITE_PACKET.h�� �ִ� Write�� �Լ�
			it->second->szComputerName, // �˻��� COMPUTER_NAME
			it->second->szIPAddress, // �˻��� IP_ADDRESS
			it->second->cCPUType, // �˻��� CPU_TYPE
			it->second->dwRam, // �˻��� RAM
			it->second->dwHDD)); // �˻��� HDD
	}
	// �˻� ����� ���� ��� ���� �����ݴϴ�.
	else
	{
		COMPUTER Computer;
		ZeroMemory(&Computer, sizeof(COMPUTER));

		// ��� ����ִ� �����͸� �����ݴϴ�.
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

// ���α׷� ��� �������� ó��
VOID CServerIocp::PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_REG_PROGRAM);

	// PROGRAM ��ü�� �Ѱ� �����մϴ�.
	PROGRAM *pProgram = new PROGRAM();

	// �������ݿ��� �����͸� ������ ��ü�� �ű�ϴ�.
	// PROGRAM_NAME ����
	_tcscpy(pProgram->szProgramName, Data.PROGRAM_NAME);
	// COMMENT ����
	_tcscpy(pProgram->szComment, Data.COMMENT);
	// COST �Է�
	pProgram->dwCost = Data.COST;

	// PROGRAM ��ü�� �����ϴ� map m_mProgram�� ������ �Է�
	m_mProgram.insert(std::make_pair(Data.PROGRAM_NAME, pProgram));
}

// ���α׷� �˻� �������� ó��
VOID CServerIocp::PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	// ��ó�� �Լ��� ����ȭ
	READ_PACKET(PT_QUERY_PROGRAM);

	// PROGRAM_NAME�� �̿��ؼ� �����͸� �˻��մϴ�.
	std::map<std::wstring, PROGRAM*>::iterator it = m_mProgram.find(Data.PROGRAM_NAME);

	// �˻� ����� �������
	if (it != m_mProgram.end())
	{
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, // ��� ��Ŷ
			WriteBuffer, // ������ ������ ����
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, // PT_WRITE_PACKET.h�� �ִ� Write�� �Լ�
			it->second->szProgramName, // �˻��� PROGRAM_NAME
			it->second->dwCost, // �˻��� COST
			it->second->szComment)); // �˻��� COMMENT
	}
	// �˻� ����� ���� ��� ���� �����ݴϴ�.
	else
	{
		PROGRAM Program;
		ZeroMemory(&Program, sizeof(PROGRAM));

		// ��� ����ִ� �����͸� �����ݴϴ�.
		pConnectedSession->WritePacket(
			PT_QUERY_PROGRAM_RESULT, 
			WriteBuffer, 
			WRITE_PT_QUERY_PROGRAM_RESULT(WriteBuffer, 
			Program.szProgramName,
			Program.dwCost, 
			Program.szComment));
	}
}