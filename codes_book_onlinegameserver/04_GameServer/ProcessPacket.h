#pragma once

class ProcessPacket
{
public:
	ProcessPacket(void);
	~ProcessPacket(void);
	//Ŭ���̾�Ʈ���� ���� ��Ŷ
	static void fnLoginPlayerRq( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnMovePlayerCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnKeepAliveCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );

	//NPC�������� ���� ��Ŷ
	static void fnNPCNpcInfoVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCUpdateNpcVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCAttackNpcToPlayerSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );

};
