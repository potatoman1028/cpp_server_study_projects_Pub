#include "stdafx.h"
#include "Crypt.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"

CPacketSession::CPacketSession(VOID)
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mCurrentPacketNumber	= 0;

	mLastReadPacketNumber	= 0;
}

CPacketSession::~CPacketSession(VOID)
{
}

BOOL CPacketSession::Begin(VOID)
{
	CThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength			= 0;
	mCurrentPacketNumber	= 0;
	mLastReadPacketNumber	= 0;

	if (!WriteQueue.Begin())
		return FALSE;

	return CNetworkSession::Begin();
}

BOOL CPacketSession::End(VOID)
{
	CThreadSync Sync;

	mLastReadPacketInfoVectorForUdp.clear();

	if (!WriteQueue.End())
		return FALSE;

	return CNetworkSession::End();
}

BOOL CPacketSession::GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	INT PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	if (PacketLength <= mRemainLength)
	{
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/, 
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/, 
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber	= 0;
		DWORD Protocol		= 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer + 
			sizeof(DWORD)/*LENGTH*/ + 
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		protocol		= Protocol;
		packetLength	= PacketLength - 
			sizeof(DWORD)/*LENGTH*/ - 
			sizeof(DWORD)/*PACKETNUMBER*/ - 
			sizeof(DWORD)/*PROTOCOL*/;

		memcpy(packet, mPacketBuffer + 
			sizeof(DWORD)/*LENGTH*/ + 
			sizeof(DWORD)/*PACKETNUMBER*/ + 
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		if (PacketNumber <= mLastReadPacketNumber) // 패킷번호가 이전것이거나 같을 경우(복사) 해당 패킷은 무시한다.
			return FALSE;
		else
		{
			mLastReadPacketNumber = PacketNumber;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CPacketSession::GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	INT PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;

		return FALSE;
	}

	if (PacketLength <= mRemainLength)
	{
		CCrypt::Decrypt(mPacketBuffer + sizeof(DWORD)/*LENGTH*/, 
			mPacketBuffer + sizeof(DWORD)/*LENGTH*/, 
			PacketLength - sizeof(DWORD)/*LENGTH*/);

		DWORD PacketNumber	= 0;
		DWORD Protocol		= 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer + 
			sizeof(DWORD)/*LENGTH*/ + 
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		protocol		= Protocol;
		packetLength	= PacketLength - 
			sizeof(DWORD)/*LENGTH*/ - 
			sizeof(DWORD)/*PACKETNUMBER*/ - 
			sizeof(DWORD)/*PROTOCOL*/;

		memcpy(packet, mPacketBuffer + 
			sizeof(DWORD)/*LENGTH*/ + 
			sizeof(DWORD)/*PACKETNUMBER*/ + 
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);
		
		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		for (DWORD i=0;i<mLastReadPacketInfoVectorForUdp.size();i++)
		{
			READ_PACKET_INFO ReadPacketInfo = mLastReadPacketInfoVectorForUdp[i];

			if (!strcmp(ReadPacketInfo.RemoteAddress, remoteAddress) && ReadPacketInfo.RemotePort == remotePort)
			{
				if (ReadPacketInfo.PacketNumber < PacketNumber)
				{
					mLastReadPacketInfoVectorForUdp[i].PacketNumber = PacketNumber;

					return TRUE;
				}
				else
					return FALSE;
			}
		}

		READ_PACKET_INFO ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort	= remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);

		mLastReadPacketInfoVectorForUdp.push_back(ReadPacketInfo);

		return TRUE;
	}

	return FALSE;
}

// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSession::ReadPacketForIocp(DWORD readLength)
{
	CThreadSync Sync;

	if (!CNetworkSession::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength	+= readLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

// ReadPacketForEventSelect는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSession::ReadPacketForEventSelect(VOID)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadForEventSelect(mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength	+= ReadLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSession::ReadFromPacketForIocp(LPSTR remoteAddress, USHORT &remotePort, DWORD readLength)
{
	CThreadSync Sync;

	if (!CNetworkSession::ReadFromForIocp(remoteAddress, remotePort, mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength	+= readLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

// ReadPacketForEventSelect는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL CPacketSession::ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT &remotePort)
{
	CThreadSync Sync;

	DWORD ReadLength = 0;

	if (!CNetworkSession::ReadFromForEventSelect(remoteAddress, remotePort, mPacketBuffer + mRemainLength, ReadLength))
		return FALSE;

	mRemainLength	+= ReadLength;

	//return getPacket(remoteAddress, remotePort, protocol, packet, packetLength);
	return TRUE;
}

BOOL CPacketSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/ + 
		sizeof(DWORD)/*PROTOCOL*/ + 
		packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = {0,};
	
	// 패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));
	
	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/, 
		&mCurrentPacketNumber, sizeof(DWORD));
	
	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/, 
		&protocol, sizeof(DWORD));

	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::Write(WriteData, PacketLength);
}

BOOL CPacketSession::WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/ + 
		sizeof(DWORD)/*PROTOCOL*/ + 
		packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = {0,};

	//  패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/, 
		&mCurrentPacketNumber, sizeof(DWORD));

	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/, 
		&protocol, sizeof(DWORD));

	memcpy(TempBuffer + 
		sizeof(DWORD)/*LENGTH*/ + 
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	CCrypt::Encrypt(TempBuffer + sizeof(DWORD), TempBuffer + sizeof(DWORD), PacketLength - sizeof(DWORD));

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return CNetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

BOOL CPacketSession::WriteComplete(VOID)
{
	CThreadSync Sync;

	// WriteQueue에서 Pop을 해 주면 된다.
	return WriteQueue.Pop();
}

BOOL CPacketSession::ResetUdp(VOID)
{
	CThreadSync	Sync;

	mLastReadPacketInfoVectorForUdp.clear();

	return TRUE;
}