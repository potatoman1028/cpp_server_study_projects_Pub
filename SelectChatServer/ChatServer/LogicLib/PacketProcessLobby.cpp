#include "../../Common/Packet.h"
#include "../../Common/ErrorCode.h"
#include "../ServerNetLib/TcpNetwork.h"
#include "User.h"
#include "UserManager.h"
#include "Lobby.h"
#include "LobbyManager.h"
#include "PacketProcess.h"

using PACKET_ID = NCommon::PACKET_ID;

namespace NLogicLib
{
	ERROR_CODE PacketProcess::LobbyEnter(PacketInfo packetInfo)
	{
	CHECK_START
		auto reqPkt = (NCommon::PktLobbyEnterReq*)packetInfo.pRefData;
		NCommon::PktLobbyEnterRes resPkt;

		auto pUserRet = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
		auto errorCode = std::get<0>(pUserRet);

		if (errorCode != ERROR_CODE::NONE) {
			CHECK_ERROR(errorCode);
		}

		auto pUser = std::get<1>(pUserRet);

		if (pUser->IsCurDomainInLogIn() == false) {
			CHECK_ERROR(ERROR_CODE::LOBBY_ENTER_INVALID_DOMAIN);
		}

		auto pLobby = m_pRefLobbyMgr->GetLobby(reqPkt->LobbyId);
		if (pLobby == nullptr) {
			CHECK_ERROR(ERROR_CODE::LOBBY_ENTER_INVALID_LOBBY_INDEX);
		}

		auto enterRet = pLobby->EnterUser(pUser);
		if (enterRet != ERROR_CODE::NONE) {
			CHECK_ERROR(enterRet);
		}
				

		resPkt.MaxUserCount = pLobby->MaxUserCount();
		resPkt.MaxRoomCount = pLobby->MaxRoomCount();
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_ENTER_RES, sizeof(NCommon::PktLobbyEnterRes), (char*)&resPkt);
		return ERROR_CODE::NONE;

	CHECK_ERR:
		resPkt.SetError(__result);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_ENTER_RES, sizeof(NCommon::PktLobbyEnterRes), (char*)&resPkt);
		return (ERROR_CODE)__result;
	}
		
	ERROR_CODE PacketProcess::LobbyLeave(PacketInfo packetInfo)
	{
	CHECK_START
		NCommon::PktLobbyLeaveRes resPkt;

		auto pUserRet = m_pRefUserMgr->GetUser(packetInfo.SessionIndex);
		auto errorCode = std::get<0>(pUserRet);

		if (errorCode != ERROR_CODE::NONE) {
			CHECK_ERROR(errorCode);
		}

		auto pUser = std::get<1>(pUserRet);

		if (pUser->IsCurDomainInLobby() == false) {
			CHECK_ERROR(ERROR_CODE::LOBBY_LEAVE_INVALID_DOMAIN);
		}

		auto pLobby = m_pRefLobbyMgr->GetLobby(pUser->GetLobbyIndex());
		if (pLobby == nullptr) {
			CHECK_ERROR(ERROR_CODE::LOBBY_LEAVE_INVALID_LOBBY_INDEX);
		}

		auto enterRet = pLobby->LeaveUser(pUser->GetIndex());
		if (enterRet != ERROR_CODE::NONE) {
			CHECK_ERROR(enterRet);
		}
						
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_LEAVE_RES, sizeof(NCommon::PktLobbyLeaveRes), (char*)&resPkt);
		return ERROR_CODE::NONE;

	CHECK_ERR:
		resPkt.SetError(__result);
		m_pRefNetwork->SendData(packetInfo.SessionIndex, (short)PACKET_ID::LOBBY_LEAVE_RES, sizeof(NCommon::PktLobbyLeaveRes), (char*)&resPkt);
		return (ERROR_CODE)__result;
	}	
}