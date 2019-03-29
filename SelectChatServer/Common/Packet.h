#pragma once

#include "PacketID.h"
#include "ErrorCode.h"

namespace NCommon
{	
#pragma pack(push, 1)
	struct PktHeader
	{
		short TotalSize;
		short Id;
		unsigned char Reserve;
	};

	struct PktBase
	{
		short ErrorCode = (short)ERROR_CODE::NONE;
		void SetError(ERROR_CODE error) { ErrorCode = (short)error; }
	};

	//- �α��� ��û
	const int MAX_USER_ID_SIZE = 16;
	const int MAX_USER_PASSWORD_SIZE = 16;
	struct PktLogInReq
	{
		char szID[MAX_USER_ID_SIZE+1] = { 0, };
		char szPW[MAX_USER_PASSWORD_SIZE+1] = { 0, };
	};

	struct PktLogInRes : PktBase
	{
	};


	//- ä�� ����Ʈ ��û
	// Request���� Body�� ����.

	const int MAX_LOBBY_LIST_COUNT = 20;
	struct LobbyListInfo
	{
		short LobbyId;
		short LobbyUserCount;
		short LobbyMaxUserCount;
	};
	struct PktLobbyListRes : PktBase
	{
		short LobbyCount = 0;
		LobbyListInfo LobbyList[MAX_LOBBY_LIST_COUNT];
	};


	//- �κ� ���� ��û
	struct PktLobbyEnterReq
	{
		short LobbyId;
	};
		
	struct PktLobbyEnterRes : PktBase
	{
		short MaxUserCount;
		short MaxRoomCount;
	};


	
	//- �κ񿡼� ������ ��û
	struct PktLobbyLeaveReq {};

	struct PktLobbyLeaveRes : PktBase
	{
	};
	
	

	//- �뿡 ���� ��û
	const int MAX_ROOM_TITLE_SIZE = 16;
	struct PktRoomEnterReq
	{
		bool IsCreate;
		short RoomIndex;
		wchar_t RoomTitle[MAX_ROOM_TITLE_SIZE + 1];
	};

	struct PktRoomEnterRes : PktBase
	{
	};

		
	//- �뿡 �ִ� �������� ���� ���� ���� ���� �뺸
	struct PktRoomEnterUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};


	//- �� ������ ��û
	struct PktRoomLeaveReq {};

	struct PktRoomLeaveRes : PktBase
	{
	};

	//- �뿡�� ������ ���� �뺸(�κ� �ִ� ��������)
	struct PktRoomLeaveUserInfoNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};
		

	//- �� ä��
	const int MAX_ROOM_CHAT_MSG_SIZE = 256;
	struct PktRoomChatReq
	{
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	struct PktRoomChatRes : PktBase
	{
	};

	struct PktRoomChatNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
		wchar_t Msg[MAX_ROOM_CHAT_MSG_SIZE + 1] = { 0, };
	};

	//- �κ� ä��
	const int MAX_LOBBY_CHAT_MSG_SIZE = 256;
	struct PktLobbyChatReq
	{
		wchar_t Msg[MAX_LOBBY_CHAT_MSG_SIZE + 1] = { 0, };
	};

	struct PktLobbyChatRes : PktBase
	{
	};

	struct PktLobbyChatNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
		wchar_t Msg[MAX_LOBBY_CHAT_MSG_SIZE + 1] = { 0, };
	};


	// ������ ���� ���� ��û
	struct PktRoomMaterGameStartReq
	{};

	struct PktRoomMaterGameStartRes : PktBase
	{};

	struct PktRoomMaterGameStartNtf
	{};


	// ������ �ƴ� ����� ���� ���� ��û
	struct PktRoomGameStartReq
	{};

	struct PktRoomGameStartRes : PktBase
	{};

	struct PktRoomGameStartNtf
	{
		char UserID[MAX_USER_ID_SIZE + 1] = { 0, };
	};




	const int DEV_ECHO_DATA_MAX_SIZE = 1024;

	struct PktDevEchoReq
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

	struct PktDevEchoRes : PktBase
	{
		short DataSize;
		char Datas[DEV_ECHO_DATA_MAX_SIZE];
	};

#pragma pack(pop)


	
}