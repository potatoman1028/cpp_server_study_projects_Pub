#ifndef  __PC_H
#define  __PC_H

#include "Character.h"

using namespace cpp::net;

class PC : public Character
{
	friend class SessionManager;

public:
	virtual void			onCreate();

protected:
			PC( SOCKET s );
	virtual ~PC();

	virtual	void			packetParsing( Packet& packet );

private:
			void			onFSEnterFieldReq( Packet& packet );

			void			loadInfoFromDB();
			void			sendObjectList();

			DWORD			fieldID;
};

#endif