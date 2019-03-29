#pragma once
#include "stdafx.h"

class Packet
{
	void encode()
	{
		

	}

	void decode()
	{

	}
};

class PacketManager
{
	ThreadJobQueue<Packet *> packetQueue_;

public:
	bool inputPacket(Packet *packet)
	{
		packetQueue_.push(packet);
	}

	// ť���� �ϳ� �̾Ƽ� ��Ŀ���� �����ش�.
	Packet *getPacket()
	{
		Packet *packet;
		if (!packetQueue_.pop(packet)) {
			packetQueue_.swap();
			return nullptr;
		}
		return packet;
	}
};