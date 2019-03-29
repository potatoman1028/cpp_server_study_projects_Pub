#ifndef  __SessionManager_H
#define  __SessionManager_H

#include <cpp/util/List.h>

#include "Session.h"

class SessionManager
{
public:
	static	SessionManager*		getInstance();
	static	void				releaseInstance();

			void				createNewSession( SOCKET socket );
			void				removeSession( Session* session );

private:
			SessionManager();
	virtual ~SessionManager();

	static	SessionManager*				selfInstance;

			cpp::util::List*			listSession;
};

#endif
