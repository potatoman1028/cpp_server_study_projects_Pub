#include "stdafx.h"
#include "FiniteStateMachine.h"

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- Exception objects ----------------------------------------------------

InvalidCurrentStateException::InvalidCurrentStateException( int curStateID )
	: currentStateID( curStateID )
{
}

InvalidCurrentStateException::~InvalidCurrentStateException()
{
}


InvalidInputEventException::InvalidInputEventException( int event )
	: inputEvent( event )
{
}

InvalidInputEventException::~InvalidInputEventException()
{
}




//------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------- Finite State Machine ----------------------------------------------------

FiniteStateMachine::FiniteStateMachine()
	: currentState( 0 )
{
}

FiniteStateMachine::~FiniteStateMachine()
{
	std::map < DWORD, FiniteState* >::iterator iter, iterPrev;

	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		iterPrev = iter++;
		delete iterPrev->second;
		mapState.erase( iterPrev );
	}
}

void FiniteStateMachine::addStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID )
{
	std::map < DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//  동일한 State가 이미 존재하는지를 찾아낸다.
	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		state = iter->second;

		if( state->getStateID() == stateID )
			break;

		iter++;
	}

	//  만일 동일한 State가 존재하지 않는다면 새로 생성한다.
	if( iter == mapState.end() )
	{
		state = new FiniteState( stateID );
		mapState[state->getStateID()] = state;
	}

	//  상태 전이 정보를 추가한다.
	state->addTransition( inputEvent, outputStateID );
}

void FiniteStateMachine::deleteTransition( DWORD stateID, DWORD inputEvent )
{
	std::map < DWORD, FiniteState* >::iterator iter, iterPrev;
	FiniteState* state;

	//  해당 State를 찾는다.
	iter = mapState.begin();
	while( iter != mapState.end() )
	{
		iterPrev = iter;
		state = iter->second;

		if( state->getStateID() == stateID )
			break;

		iter++;
	}

	if( iter == mapState.end() )
		return;

	//  State의 전이 정보를 지운다.
	state->deleteTransition( inputEvent );
	if( state->getCount() == 0 )
	{
		delete state;

		mapState.erase( iterPrev );
	}
}

DWORD FiniteStateMachine::getOutputState( DWORD inputEvent ) throw ( InvalidCurrentStateException* )
{
	std::map < DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//  만일 존재하지 않는 State가 입력되었다면, Exception을 발생시킨다.
	iter = mapState.find( getCurrentStateID() );
	if( iter == mapState.end() )
		throw ( new InvalidCurrentStateException( getCurrentStateID() ) );

	state = mapState[getCurrentStateID()];

	return state->outputState( inputEvent );
}

void FiniteStateMachine::setCurrentState( DWORD stateID ) throw ( InvalidCurrentStateException* )
{
	std::map < DWORD, FiniteState* >::iterator iter;

	iter = mapState.find( stateID );
	if( iter == mapState.end() )
		throw ( new InvalidCurrentStateException( -1 ) );

	currentState = iter->second;
}

DWORD FiniteStateMachine::getCurrentStateID() throw ( InvalidCurrentStateException* )
{
	if( currentState == 0 )
		throw ( new InvalidCurrentStateException( -1 ) );

	return currentState->getStateID();
}

void FiniteStateMachine::stateTransition( int event ) throw ( InvalidCurrentStateException* )
{
	if( currentState == 0 )
		throw ( new InvalidCurrentStateException( -1 ) );

	DWORD outputState;

	try
	{
		outputState = currentState->outputState( event );
	}
	catch( InvalidInputEventException* ex )
	{
		delete ex;

		return;
	}

	currentState = mapState[outputState];
}