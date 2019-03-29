#ifndef  __SessionManager_H
#define  __SessionManager_H

#include <cpp/util/List.h>
#include <cpp/util/Queue.h>

#include "Session.h"

#define  MaxSessionCount		500

class SessionManager
{
public:
	static	SessionManager*		getInstance();
	static	void				releaseInstance();

			void				createNewSession( SOCKET socket, struct sockaddr FAR* addr = 0 );
			void				removeSession( Session* session );

			int					getActiveSessionCount();

private:
			SessionManager();
	virtual ~SessionManager();

	static	SessionManager*				selfInstance;

			cpp::util::Queue			queueInactiveSession;
			cpp::util::List				listActiveSession;
};

#endif
