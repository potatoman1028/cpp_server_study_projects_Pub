// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "../LowLib/Global.h"

#define DEFAULT_PORT					11550
#define MAX_USER						100

#define LOG_OK							_T("[ ok ]\n")
#define LOG_FAIL						_T("[ fail ]\n\n")

typedef enum USER_STATUS
{
	US_NONE = 0,
	US_CHANNEL_ENTERING,
	US_CHANNEL_ENTERED,
	US_ROOM_ENTERING,
	US_ROOM_ENTERED,
	US_ROOM_LEAVING,
	US_GAME_STARTING,
	US_GAME_STARTED,
	US_GAME_ENDING
};

//#define MAX_DATABASE_CONNECTION_COUNT				4
//#define DATABASE_USERID								_T("xogupigu")
//#define DATABASE_PASSWORD							_T("xogupigu")
//#define DATABASE_ADDRESS							_T("10.20.6.79")

#define CONFIG_FILENAME								_T("./config.ini")