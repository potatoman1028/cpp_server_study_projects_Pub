#ifndef  __StandAction_H
#define  __StandAction_H

#include "CharacterState.h"

class StandAction : public CharacterState
{
public:
			StandAction( Character* parent );
	virtual ~StandAction();

	virtual	void		move();
	virtual	void		attack();
	virtual	void		process( Character* ch );
};

#endif
