#include "stdafx.h"
#include "Character.h"
#include "StandAction.h"
#include "MoveAction.h"
#include "AttackAction.h"
#include "RunawayAction.h"

Character::Character( FiniteStateMachine* fsm, int r, int spd )
	: stateMachine( fsm ), radius( r ), speed( spd )
{
	arrayAction[STATE_STAND]   = ( CharacterState* )new StandAction( this );
	arrayAction[STATE_MOVE]    = ( CharacterState* )new MoveAction( this );
	arrayAction[STATE_ATTACK]  = ( CharacterState* )new AttackAction( this );
	arrayAction[STATE_RUNAWAY] = ( CharacterState* )new RunawayAction( this );

	action = arrayAction[STATE_MOVE];
}

Character::~Character()
{
	if( stateMachine != 0 )
		delete stateMachine;

	int i;

	for( i = 0 ; i < STATE_COUNT ; i++ )
		delete arrayAction[i];
}

void Character::process( Character* target )
{
	action->process( target );
}

DWORD Character::getState()
{
	if( stateMachine == 0 )
		return 0;

	return stateMachine->getCurrentStateID();
}

void Character::setTransition( DWORD event )
{
	stateMachine->stateTransition( event );
	action = arrayAction[stateMachine->getCurrentStateID()];
}

void Character::moveTo( int x, int y )
{
	if( position.x < x ) position.x += speed;
	if( position.x > x ) position.x -= speed;
	if( position.y < y ) position.y += speed;
	if( position.y > y ) position.y -= speed;
}