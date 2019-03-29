#pragma once

#define PACKET_HEADER_SIZE (sizeof(DWORD)/*LENGTH*/ + sizeof(DWORD)/*PACKET_NO*/ + sizeof(DWORD)/*CHECK_SUM*/)

typedef enum PACKET_ERROR
{
	PACKET_NOERROR,
	PACKET_NOTENOUGH_LENGTH,
	PACKET_PACKETNO_FAIL,
	PACKET_CHECKSUM_FAIL,
	PACKET_SYSTEM_FAIL
};

class CPacketBox : public CSyncParent<CPacketBox>
{
public:
	CPacketBox(VOID);
	~CPacketBox(VOID);

	BOOL WrapPacket(BYTE *pucSrcPacket, BYTE *pucDestPacket, DWORD &rdwLen);
	PACKET_ERROR WrapOffPacket(BYTE *pucSrcPacket, BYTE *pucDestPacket, DWORD &rdwLen);
	VOID Init(VOID) { CSyncObj Sync; m_dwCurSendNo = 0; m_dwLastRecvNo = 0; }

private:
	DWORD m_dwCurSendNo;
	DWORD m_dwLastRecvNo;
};
