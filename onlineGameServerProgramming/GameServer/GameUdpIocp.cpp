#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/Iocp.h"
#include "../LowLib/Stream.h"

#include "../Packet/ErrorCode.h"
#include "../Packet/UdpProtocol.h"
#include "../Packet/Udp_Structure.h"
#include "../Packet/Udp_ReadPacket.h"
#include "../Packet/Udp_WritePacket.h"

#include "Character.h"
#include "Room.h"
#include "RoomManager.h"
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"
#include "GameUdpIocp.h"

DWORD WINAPI ResetThreadCallback(LPVOID parameter)
{
	CGameUdpIocp *Owner = (CGameUdpIocp*) parameter;
	Owner->ResetThreadCallback();

	return 0;
}

CGameUdpIocp::CGameUdpIocp(VOID)
{
}

CGameUdpIocp::~CGameUdpIocp(VOID)
{
}

VOID CGameUdpIocp::ResetThreadCallback(VOID)
{
	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mResetThreadDestroyEvent, 1000);

		if (Result == WAIT_OBJECT_0)
			return;

		mUdpSession.ResetUdp();
	}
}

BOOL CGameUdpIocp::Begin(VOID)
{
	if (!CIocp::Begin())
	{	
		CLog::WriteLogNoDate(_T("CIocp::Begin"));

		End();

		return FALSE;
	}

	if (!mUdpSession.Begin())
	{
		CLog::WriteLogNoDate(_T("mUdpSession.Begin"));

		End();

		return FALSE;
	}

	if (!mUdpSession.UdpBind(DEFAULT_PORT))
	{
		CLog::WriteLogNoDate(_T("mUdpSession.UdpBind"));

		End();

		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(mUdpSession.GetSocket(), (ULONG_PTR) &mUdpSession))
	{
		CLog::WriteLogNoDate(_T("CIocp::RegisterSocketToIocp"));

		End();

		return FALSE;
	}

	if (!mUdpSession.InitializeReadFromForIocp())
	{
		CLog::WriteLogNoDate(_T("mUdpSession.InitializeReadFromForIocp"));

		End();

		return FALSE;
	}

	mResetThreadDestroyEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);
	mResetThreadHandle			= CreateThread(NULL, 0, ::ResetThreadCallback, this, 0, NULL);

	return TRUE;
}

BOOL CGameUdpIocp::End(VOID)
{
	SetEvent(mResetThreadDestroyEvent);

	WaitForSingleObject(mResetThreadHandle, INFINITE);

	CloseHandle(mResetThreadDestroyEvent);
	CloseHandle(mResetThreadHandle);

	CIocp::End();

	mUdpSession.End();

	return TRUE;
}

VOID CGameUdpIocp::OnIoConnected(VOID *object){}
VOID CGameUdpIocp::OnIoDisconnected(VOID *object){}

VOID CGameUdpIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	CPacketSession *PacketSession = (CPacketSession*) object;

	DWORD	Protocol						= 0;
	BYTE	Packet[MAX_BUFFER_LENGTH]		= {0,};
	DWORD	PacketLength					= 0;
	CHAR	RemoteAddress[32]				= {0,};
	USHORT	RemotePort						= 0;
	TCHAR	RemoteAddressTCHAR[32]			= {0,};

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= {0,};

	if (PacketSession->ReadFromPacketForIocp(RemoteAddress, RemotePort, dataLength))
	{
		while (PacketSession->GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength))
		{
			MultiByteToWideChar(CP_ACP,
				0,
				RemoteAddress,
				-1,
				RemoteAddressTCHAR,
				32);

			switch (Protocol)
			{
				// PROTOCOL : PU_C2S_TUNNELING
				case PU_C2S_TUNNELING:
				{
					S_PU_C2S_TUNNELING Data;
					READ_PU_C2S_TUNNELING(Packet, Data);
#ifdef _FULL_DEBUG
					CLog::WriteLog(_T("# Read packet : PU_C2S_TUNNELING 0x%x/%s/%d\n"), 
						Data.SESSION_ID, 
						Data.REMOTE_ADDRESS, 
						Data.REMOTE_PORT);
#endif

					CConnectedUser *ConnectedUser = (CConnectedUser*) Data.SESSION_ID;
					if (ConnectedUser)
					{
						if (ConnectedUser->GetStatus() < US_CHANNEL_ENTERED)
						{
							mUdpSession.WriteToPacket(RemoteAddress, 
								RemotePort, 
								PU_S2C_TUNNELING_FAIL_U, 
								WriteBuffer, 
								WRITE_PU_S2C_TUNNELING_FAIL_U(WriteBuffer, 
								EC_USER_STATUS_INCORRECT));

							CLog::WriteLog(_T("# Write packet : PU_S2C_TUNNELING_FAIL_U %d\n"), EC_USER_STATUS_INCORRECT);
						}
						else
						{
							ConnectedUser->SetVirtualInformation(Data.REMOTE_ADDRESS, Data.REMOTE_PORT);
							ConnectedUser->SetRealInformation(RemoteAddressTCHAR, RemotePort);

							mUdpSession.WriteToPacket(RemoteAddress, 
								RemotePort, 
								PU_S2C_TUNNELING_SUCC_U, 
								WriteBuffer, 
								WRITE_PU_S2C_TUNNELING_SUCC_U(WriteBuffer));

#ifdef _FULL_DEBUG
							CLog::WriteLog(_T("# Write packet : PU_S2C_TUNNELING_SUCC_U\n"));
#endif
						}
					}
					else
					{
						mUdpSession.WriteToPacket(RemoteAddress, 
							RemotePort, 
							PU_S2C_TUNNELING_FAIL_U, 
							WriteBuffer, 
							WRITE_PU_S2C_TUNNELING_FAIL_U(WriteBuffer, 
							EC_USER_OBJECT_INCORRECT));

						CLog::WriteLog(_T("# Write packet : PU_S2C_TUNNELING_FAIL_U %d\n"), EC_USER_OBJECT_INCORRECT);
					}
				}
				break;
			}
		}
	}

	if (!PacketSession->InitializeReadFromForIocp())
	{
		CLog::WriteLog(_T("! OnIoRead : PacketSession->InitializeReadFromForIocp"));

		End();

		return;
	}
}

VOID CGameUdpIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	CPacketSession *PacketSession = (CPacketSession*) object;
	
	PacketSession->WriteComplete();

#ifdef _FULL_DEBUG
	CLog::WriteLog(_T("# Client udp data wrote : 0x%x(0x%x)(%d)\n"), PacketSession, PacketSession->GetSocket(), dataLength);
#endif
}