#ifndef  __GameRoom_H
#define  __GameRoom_H

#include "Room.h"

class GameRoom : public Room
{
public:
	static	GameRoom*	getInstance();
	static	void		releaseInstance();

	virtual	bool		enter( User* user );
	virtual	void		leave( User* user );

			bool		startGame();
			bool		isStarted(){ return isStartedGame; }

private:
			GameRoom();
	virtual	~GameRoom();

	static	GameRoom*	selfInstance;

			bool		isStartedGame;
};

#endif