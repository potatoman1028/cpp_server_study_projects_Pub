#pragma once

class ProcessPacket
{
public:
	ProcessPacket(void);
	~ProcessPacket(void);

	static void fnNPCLoginPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCMovePlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCLogoutPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCDeadPlayerCn( Connection* pConnection,  DWORD dwSize , char* pRecvedMsg );
	
};
