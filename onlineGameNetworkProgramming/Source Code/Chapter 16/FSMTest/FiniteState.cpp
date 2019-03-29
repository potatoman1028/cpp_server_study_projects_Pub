#include "stdafx.h"
#include "FiniteState.h"
#include "FiniteStateMachine.h"

FiniteState::FiniteState( DWORD state )
	: stateID( state )
{
}

FiniteState::~FiniteState()
{
	std::map < DWORD, DWORD >::iterator	iter, iterPrev;

	//  모든 정보를 제거한다.
	iter = mapTransition.begin();
	while( iter != mapTransition.end() )
	{
		iterPrev = iter++;

		mapTransition.erase( iterPrev );
	}
}

void FiniteState::addTransition( DWORD inputEvent, DWORD outputStateID )
{
	mapTransition[inputEvent] = outputStateID;
}

void FiniteState::deleteTransition( DWORD inputEvent )
{
	mapTransition.erase( inputEvent );
}

DWORD FiniteState::outputState( DWORD inputEvent ) throw ( InvalidInputEventException* )
{
	std::map < DWORD, DWORD >::iterator	iter;

	//  전이 조건에 해당하는 전이 상태를 찾아낸다.
	iter = mapTransition.find( inputEvent );
	if( iter == mapTransition.end() )
		throw ( new InvalidInputEventException( inputEvent ) );

	return mapTransition[inputEvent];
}

DWORD FiniteState::getCount()
{
	return mapTransition.size();
}