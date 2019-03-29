#ifndef  __Synchronized_H
#define  __Synchronized_H

#include <Windows.h>

class Synchronized
{
public:
			Synchronized( CRITICAL_SECTION& cs );
	virtual	~Synchronized();

private:

			CRITICAL_SECTION*	criticalSection;
};

#endif