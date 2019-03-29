#ifndef  __RunawayAction_H
#define  __RunawayAction_H

#include "CharacterState.h"

class RunawayAction : public CharacterState
{
public:
			RunawayAction( Character* parent );
	virtual ~RunawayAction();

	virtual	void		move();
	virtual	void		attack();
	virtual	void		process( Character* ch );
};

#endif
