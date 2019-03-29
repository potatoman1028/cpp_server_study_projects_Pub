#pragma once

typedef enum ERROR_CODE
{
	EC_USER_ALREADY_LOGINED,
	EC_CANNOT_CREATE_MORE_ROOM,
	EC_USER_STATUS_INCORRECT,
	EC_PACKET_DATA_INCORRECT,
	EC_ROOM_OBJECT_POINTER_FAILED,
	EC_USER_OBJECT_INCORRECT,
	EC_USER_PERMISSION_INCORRECT,
	EC_ROOM_START_FAIL_ALL_READY,
	EC_ROOM_START_FAIL_TEAM_INCORRECT,
	EC_GAME_BALLUSER_INCORRECT,
	EC_GAME_SHOOTERUSER_INCORRECT,
	EC_GAME_TARGETUSER_INCORRECT,
	EC_GAME_USERTEAM_INCORRECT,
	EC_GAME_LASTSHOOTUSER_INCORRECT,
	EC_GAME_LASTPASSUSER_INCORRECT,
	EC_GAME_TARGETUSER_NOTINCOURT,
	EC_NO_NICKNAME,
	EC_ALREADY_EXIST_NICKNAME,
	EC_ALREADY_EXIST_CHARNAME,
	EC_DATABASE_ERROR,
	EC_NO_CHARACTER_IN_SLOT,
	EC_ITEM_TYPE_INCORRECT,
	EC_GAME_SPECIALSHOOT_AP_INCORRECT,
	EC_GAME_CANNOT_USESKILL_OUTCOURT,
	EC_END
};