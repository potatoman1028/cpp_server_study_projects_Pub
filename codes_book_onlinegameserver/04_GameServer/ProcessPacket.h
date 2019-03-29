#pragma once

class ProcessPacket
{
public:
	ProcessPacket(void);
	~ProcessPacket(void);
	//클라이언트에서 받은 패킷
	static void fnLoginPlayerRq( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnMovePlayerCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnKeepAliveCn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );

	//NPC서버에서 받은 패킷
	static void fnNPCNpcInfoVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCUpdateNpcVSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCAttackNpcToPlayerSn( Player* pPlayer,  DWORD dwSize , char* pRecvedMsg );

};
