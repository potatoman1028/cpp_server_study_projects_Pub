#ifndef  __SessionManager_H
#define  __SessionManager_H

#include <Windows.h>
#include <List>

#include "Thread.h"
#include "Session.h"

class SessionManager
{
private:
	typedef	std::list	< Session* >::iterator	ListPosition;

public:
	static	SessionManager*		getInstance();
	static	void				releaseInstance();

			void				createNewSession( SOCKET socket );
			void				removeSession( Session* session );

private:
			SessionManager();
	virtual ~SessionManager();

	static	SessionManager*				selfInstance;

			std::list	< Session* >	listSession;
			CRITICAL_SECTION			sectionList;
};

#endif