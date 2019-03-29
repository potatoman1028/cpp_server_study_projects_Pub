#ifndef  __User_H
#define  __User_H

#include "Session.h"
#include "Room.h"

class User : public Session
{
	friend class SessionManager;

public:
			String			getAccount(){ return account; }
			bool			isGameReady(){ return gameReady; }

protected:
			User( SOCKET socketValue, struct sockaddr FAR* addr = 0 );
	virtual	~User();

	virtual void			onCreate();
	virtual	void			onDestroy();

	virtual	void			packetParsing( Packet& packet );

private:
			void			onGGEnterGameRoomReq( Packet& packet );
			void			onGGUserListReq( Packet& packet );
			void			onGGChatMessageReq( Packet& packet );

			void			onGGReadyReq( Packet& packet );
			void			onGGStartGameReq( Packet& packet );
			void			onGGNewBlockReq( Packet& packet );
			void			onGGMoveBlockReq( Packet& packet );
			void			onGGFixBlockReq( Packet& packet );
			void			onGGGameOverReq( Packet& packet );


			String			account;
			bool			gameReady;
};

#endif