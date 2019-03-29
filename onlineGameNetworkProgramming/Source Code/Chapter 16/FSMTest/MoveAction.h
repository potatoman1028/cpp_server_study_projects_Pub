#ifndef  __MoveAction_H
#define  __MoveAction_H

#include "CharacterState.h"

class MoveAction : public CharacterState
{
public:
			MoveAction( Character* parent );
	virtual ~MoveAction();

	virtual	void		move();
	virtual	void		attack();
	virtual	void		process( Character* ch );

private:
			POINT		destPos;
};

#endif