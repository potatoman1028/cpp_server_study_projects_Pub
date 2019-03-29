#ifndef  __Character_H
#define  __Character_H

#include "FiniteStateMachine.h"
#include "CharacterState.h"

class Character
{
public:
	enum State
	{
		STATE_STAND = 0,
		STATE_MOVE,
		STATE_ATTACK,
		STATE_RUNAWAY,
		STATE_COUNT
	};

	enum Event
	{
		EVENT_FINDTARGET = 0,
		EVENT_LOSTTARGET,
		EVENT_BEATTACKED,
		EVENT_HEALTHDRYUP,
		EVENT_STOPWALK,
		EVENT_PATROL
	};

public:
			Character( FiniteStateMachine* fsm, int r, int spd );
	virtual ~Character();

	virtual	void		process( Character* target );

			void		setPosition( int x, int y ){ position.x = x; position.y = y; }
			void		setPosition( POINT pos ){ position = pos; }

			POINT		getPosition(){ return position; }
			int			getRadius(){ return radius; }
			DWORD		getState();

			void		setTransition( DWORD event );
			void		moveTo( int x, int y );

protected:
			POINT		position;
			int			radius;
			int			speed;

private:
			CharacterState*			action;
			CharacterState*			arrayAction[STATE_COUNT];
			FiniteStateMachine*		stateMachine;
};

#endif