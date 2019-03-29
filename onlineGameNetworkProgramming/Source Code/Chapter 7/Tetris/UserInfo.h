#ifndef  __UserInfo_H
#define  __UserInfo_H

#include <Afx.h>

#include "TetrisBoard.h"

class UserInfo
{
public:
			UserInfo();
	virtual	~UserInfo();

			CString			account;
			TetrisBoard		board;
			bool			ready;
			bool			gameOver;
			int				score;
};

#endif
