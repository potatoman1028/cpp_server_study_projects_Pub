#include "StdAfx.h"
#include "../Utils/Global.h"
#include "../Utils/CriticalSection.h"
#include "../Utils/StaticSyncParent.h"
#include "../Utils/SyncParent.h"
#include "../Utils/MemoryPool.h"
#include "../Utils/ManagedBuf.h"
#include "../Utils/Crypt.h"
#include "PacketBox.h"

CPacketBox::CPacketBox(VOID)
{
	m_dwCurSendNo = 0;
	m_dwLastRecvNo = 0;
}

CPacketBox::~CPacketBox(VOID)
{
}

BOOL CPacketBox::WrapPacket(BYTE *pucSrcPacket, BYTE *pucDestPacket, DWORD &rdwLen)
{
	CSyncObj Sync;

	// ENCRYPT (LENGTH | PACKET_NO || CHECK_SUM)
	if (!pucSrcPacket || !pucDestPacket || rdwLen >= MAX_BUF_LEN - 8)
		return FALSE;

	m_dwCurSendNo++;

	CManagedBufSP Buf;
	CCrypt::Encrypt(pucSrcPacket, Buf->m_aucBuf, rdwLen);
	DWORD dwCheckSum = (rdwLen + m_dwCurSendNo + Buf->m_aucBuf[0] + Buf->m_aucBuf[rdwLen-1]) * 8 / 16;

	CopyMemory(pucDestPacket, &rdwLen, sizeof(DWORD));
	CopyMemory(pucDestPacket + sizeof(DWORD), &m_dwCurSendNo, sizeof(DWORD));
	CopyMemory(pucDestPacket + sizeof(DWORD) + sizeof(DWORD), &dwCheckSum, sizeof(DWORD));
	CopyMemory(pucDestPacket + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD), Buf->m_aucBuf, rdwLen);
	rdwLen = PACKET_HEADER_SIZE + rdwLen;

	return TRUE;
}

// 0 : 성공, 1 : 사이즈가 모자람, 2 : 패킷번호 검증 실패, 3 : 체크섬 검증 실패
PACKET_ERROR CPacketBox::WrapOffPacket(BYTE *pucSrcPacket, BYTE *pucDestPacket, DWORD &rdwLen)
{
	CSyncObj Sync;

	// DECRYPT (LENGTH | PACKET_NO || CHECK_SUM)
	if (!pucSrcPacket || !pucDestPacket || rdwLen > MAX_BUF_LEN_EXTRA)
		return PACKET_SYSTEM_FAIL;

	// 패킷 검증이 필요
	DWORD dwPacketLen = 0;
	CopyMemory(&dwPacketLen, pucSrcPacket, sizeof(DWORD));
	if (dwPacketLen <= rdwLen - PACKET_HEADER_SIZE) // 사이즈 검증
	{
		DWORD dwPacketNo = 0;
		CopyMemory(&dwPacketNo, pucSrcPacket + sizeof(DWORD), sizeof(DWORD));
		if (dwPacketNo == m_dwLastRecvNo + 1) // 패킷 번호 검증
		{
			DWORD dwCheckSum = 0;
			CopyMemory(&dwCheckSum, pucSrcPacket + sizeof(DWORD) + sizeof(DWORD), sizeof(DWORD));
			if (dwCheckSum == (dwPacketLen + dwPacketNo + pucSrcPacket[PACKET_HEADER_SIZE] + pucSrcPacket[PACKET_HEADER_SIZE+dwPacketLen-1]) * 8 / 16)
			{
				CManagedBufSP Buf;
				CCrypt::Decrypt(pucSrcPacket + PACKET_HEADER_SIZE, Buf->m_aucBuf, dwPacketLen);

				CopyMemory(pucDestPacket, Buf->m_aucBuf, dwPacketLen);

				rdwLen = dwPacketLen;
				m_dwLastRecvNo = dwPacketNo;
				return PACKET_NOERROR;
			}
			rdwLen = dwPacketLen;
			return PACKET_CHECKSUM_FAIL;
		}
		rdwLen = dwPacketLen;
		return PACKET_PACKETNO_FAIL;
	}

	return PACKET_NOTENOUGH_LENGTH;
}