#pragma once

// CIocp를 상속 받습니다.
class CServerIocp : public CIocp
{
public:
	CServerIocp(VOID);
	virtual ~CServerIocp(VOID);

	// 개체를 초기화 하고 시작하기 위한 함수
	BOOL Begin(VOID);
	
	// 개체를 종료하기 위한 함수
	VOID End(VOID);

	// KeepAlive Thread Callback 함수
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp 상속 가상함수들을 재선언 합니다.
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// 서버의 Listen을 담당할 개체
	CNetworkSession *m_pListen;
	CConnectedSessionManager m_oConnectedSessionManager;

	// KeepAlive Thread를 관리하는 핸들값
	HANDLE m_hKeepThread;
	// KeepAlive Thread를 종료할때 사용하는 이벤트
	HANDLE m_hKeepThreadDestroyEvent;

	// USER를 관리하는 map
	std::map<std::wstring, USER*> m_mUser;
	// COMPUTER를 관리하는 map
	std::map<std::wstring, COMPUTER*> m_mComputer;
	// PROGRAM을 관리하는 map
	std::map<std::wstring, PROGRAM*> m_mProgram;

private:
	// 패킷 처리를 할 함수들
	VOID PROC_PT_REG_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_USER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_COMPUTER(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_REG_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_QUERY_PROGRAM(CConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
};
