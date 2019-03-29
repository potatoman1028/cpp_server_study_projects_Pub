#ifndef  __AttackAction_H
#define  __AttackAction_H

#include "CharacterState.h"

class AttackAction : public CharacterState
{
public:
			AttackAction( Character* parent );
	virtual ~AttackAction();

	virtual	void		move();
	virtual	void		attack();
	virtual	void		process( Character* ch );
};

#endif
