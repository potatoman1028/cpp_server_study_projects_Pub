#ifndef  __Character_H
#define  __Character_H

#include "Session.h"

class Character : public Session
{
public:
			Character( SOCKET s );
	virtual	~Character();
};

#endif